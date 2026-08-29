/* Try to promote in some cases, scalar values to vectors.
   Copyright (C) 2008 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2, or (at your option) any
later version.

GCC is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to the Free
Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "ggc.h"
#include "tree.h"
#include "rtl.h"
#include "flags.h"
#include "tm_p.h"
#include "basic-block.h"
#include "timevar.h"
#include "diagnostic.h"
#include "tree-flow.h"
#include "tree-pass.h"
#include "tree-dump.h"
#include "langhooks.h"
#include "optabs.h"
#include "tree-ssa-propagate.h"


/* The idea of this pass is to look for all BIT_FIELD_REF of vectors and see if
   we should promote those to vectors.
   Currently we only handle comparisions against a constant value.  */

/* Create a promoted variable from ELEMENT, placing the CONSTRUCTOR before 
   BSI.  */
static tree
create_promoted_var (tree type, tree element, block_stmt_iterator bsi)
{
  tree var = create_tmp_var (type, NULL);
  tree ssa_name;
  VEC(constructor_elt, gc) *v;
  unsigned i;
  tree newstmt, constructor;

  DECL_GIMPLE_REG_P (var) = 1;
  add_referenced_tmp_var (var);
  ssa_name = make_ssa_name (var, NULL);
  v = VEC_alloc (constructor_elt, gc, TYPE_VECTOR_SUBPARTS (type));

  /* Splat the element across the CONSTRUCTOR of the vector.  */
  for (i = 0; i < TYPE_VECTOR_SUBPARTS (type); i++)
    {
      constructor_elt *elt;
      elt = VEC_quick_push (constructor_elt, v, NULL);
      elt->index = NULL;
      elt->value = element;
    }
  /* For constant elements, produce a VECTOR_CST, otherwise CONSTRUCTOR.  */
  if (CONSTANT_CLASS_P (element))
    constructor = build_vector_from_ctor (type, v);
  else
    constructor = build_constructor (type, v);

  newstmt = build2 (MODIFY_EXPR, type, ssa_name, constructor);
  SSA_NAME_DEF_STMT (ssa_name) = newstmt;
  bsi_insert_before (&bsi, newstmt, BSI_NEW_STMT);
  return ssa_name;
}


/* Try to promote an use of an extaction, it might already be promoted. This
   can be recusive if need to be.  */
static void
try_promote (use_operand_p use_p, tree lhs, int elements, bool already_promoted)
{
  tree use_stmt = USE_STMT (use_p);
  tree rhs_use = get_rhs (use_stmt);
  enum tree_code code;
  tree vectortype;

  /* If we can't get a right hand side for the non promoted version, punt. */
  if (rhs_use == NULL_TREE)
    return;

  /* If the the lhs is already promoted then we can just use that vector.  */
  if (already_promoted)
    vectortype = TREE_TYPE (lhs);
  else
    vectortype = build_vector_type (TREE_TYPE (lhs), elements);

  code = TREE_CODE (rhs_use);

  /* Handle comparisions, they don't produce vector results.  */
  if (TREE_CODE_CLASS (code) == tcc_comparison)
    {
      enum can_compare_purpose purp = ccp_store_flag;

      /* If we are in an conditional expression instead of an assignment
         statement, try to see if we can use the comparision using jumps.  */
      if (TREE_CODE (use_stmt) == COND_EXPR)
	purp = ccp_jump;

      /* See if we can do the comparision in the vector mode. */
      if (can_compare_p (get_rtx_code (code, TYPE_UNSIGNED (vectortype)),
			 TYPE_MODE (vectortype), purp))
	{
	  block_stmt_iterator bsi;
	  bsi = bsi_for_stmt (use_stmt);
	  tree lhs1, rhs1;
	  tree newcompare;
	  lhs1 = TREE_OPERAND (rhs_use, 0);
	  rhs1 = TREE_OPERAND (rhs_use, 1);

	  /* FIXME: For now only handle the case where one of
	     the operands are constant as it does not detect if
	     the other expression profitable to be promoted or not.  */
	  if (!CONSTANT_CLASS_P (lhs1)
	      && !CONSTANT_CLASS_P (rhs1))
	      return;
	  lhs1 = create_promoted_var (vectortype, lhs1, bsi);
	  rhs1 = create_promoted_var (vectortype, rhs1, bsi);
	  newcompare = build2 (code, TREE_TYPE (rhs_use), lhs1, rhs1);
	  if (!set_rhs (&use_stmt, newcompare))
	    gcc_unreachable ();
	  update_stmt (use_stmt);
	}
    }
  /* FIXME handle other classes of expressions, unary and binary, etc..   */
  
}

/* Implement the pass, over the trees.  */
static void
tree_promote_vector (void)
{
  basic_block bb;
  block_stmt_iterator i;
  FOR_EACH_BB (bb)
    {
      for (i = bsi_start (bb); ! bsi_end_p (i); bsi_next (&i))
	{
	  tree stmt = bsi_stmt (i);
	  tree rhs = get_rhs (stmt);
	  tree lhs;
	  use_operand_p use_p;
	  imm_use_iterator use_iter;

	  if (rhs == NULL_TREE)
	    continue;
	  
	  /* Find BIT_FIELD_REF with vector types, the extraction points. */
	  if (TREE_CODE (rhs) != BIT_FIELD_REF
	      || TREE_CODE (TREE_TYPE (TREE_OPERAND (rhs, 0))) != VECTOR_TYPE)
	    continue;
	  lhs = TREE_OPERAND (stmt, 0);

	  /* For each use of the extraction, decide if we are going to promote
	     the use back to a vector or not. */
	  /*FOR_EACH_IMM_USE_SAFE (use_p, use_iter, lhs)*/
	    {
	      tree use_stmt;
	      int elems = TYPE_VECTOR_SUBPARTS (TREE_TYPE (TREE_OPERAND (rhs,
	      								 0)));
	      /* FIXME: we only handle variables which are used only once.  See BZ 52621 for
	         more information on why this is not done yet.  */
	      if (!single_imm_use (lhs, &use_p, &use_stmt))
	        continue;

	      /* Try to promote this use recursively if needed.  */
	      try_promote (use_p, lhs, elems, false);
	    }
	}
    }
}

static bool
gate_promote_vector (void)
{
  /* For now, always execute.  Maybe there are better checks to make sure
     we have vector modes and such. */
  return flag_promote_vector;
}

struct tree_opt_pass pass_promote_vector =
{
  "prom_vec",				/* name */
  gate_promote_vector,			/* gate */
  tree_promote_vector,			/* execute */
  NULL,					/* sub */
  NULL,					/* next */
  0,					/* static_pass_number */
  0,					/* tv_id */
  PROP_cfg | PROP_ssa | PROP_alias,	/* properties_required */
  0,					/* properties_provided */
  0,					/* properties_destroyed */
  0,					/* todo_flags_start */
  TODO_cleanup_cfg
    | TODO_dump_func
    | TODO_ggc_collect
    | TODO_verify_ssa
    | TODO_verify_flow
    | TODO_verify_stmts,		/* todo_flags_finish */
  0					/* letter */
};
