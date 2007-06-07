
/* (C) Copyright
   Sony Computer Entertainment, Inc.,
   Toshiba Corporation,
   International Business Machines Corporation,
   2001,2002,2003,2004,2005.

   This file is free software; you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free
   Software Foundation; either version 2 of the License, or (at your option) 
   any later version.

   This file is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
   for more details.

   You should have received a copy of the GNU General Public License
   along with this file; see the file COPYING.  If not, write to the Free
   Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
   02110-1301, USA.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "cpplib.h"
#include "tree.h"
#include "c-tree.h"
#include "c-pragma.h"
#include "function.h"
#include "rtl.h"
#include "expr.h"
#include "errors.h"
#include "tm_p.h"
#include "langhooks.h"
#include "insn-config.h"
#include "insn-codes.h"
#include "recog.h"
#include "optabs.h"
/* APPLE LOCAL begin AltiVec */
#include "c-common.h"
#include "../libcpp/internal.h"
#include "target.h"
#include "spu_types.h"

static cpp_hashnode *spu_categorize_keyword (const cpp_token *);
static void spu_init_vector_keywords (cpp_reader *pfile);
/* APPLE LOCAL end AltiVec */

struct spu_builtin_description;
void spu_init_builtins (void);
static rtx spu_expand_builtin_1 (struct spu_builtin_description *d,
				 tree arglist, rtx target);
tree spu_select_overloaded_builtin (tree, tree);
tree spu_expand_tree_builtin (tree, tree, tree);
int legitimate_const (rtx);
rtx spu_expand_builtin (tree, rtx, rtx, enum machine_mode, int);
static void spu_check_builtin_parm (struct spu_builtin_description *, rtx,
				    int);
static void expand_builtin_args (struct spu_builtin_description *, tree, rtx,
				 rtx[]);

/* Builtin types, data and prototypes. */
struct spu_builtin_range {
    int low, high;
};

static struct spu_builtin_range spu_builtin_range[] = {
 {   -0x40ll,    0x7fll}, /* SPU_TI_7     */
 {   -0x40ll,    0x3fll}, /* SPU_TI_S7    */
 {       0ll,    0x7fll}, /* SPU_TI_U7    */
 {  -0x200ll,   0x1ffll}, /* SPU_TI_S10   */
 { -0x2000ll,  0x1fffll}, /* SPU_TI_S10_4 */
 {       0ll,  0x3fffll}, /* SPU_TI_U14   */
 { -0x8000ll,  0xffffll}, /* SPU_TI_16    */
 { -0x8000ll,  0x7fffll}, /* SPU_TI_S16   */
 {-0x20000ll, 0x1ffffll}, /* SPU_TI_S16_2 */
 {       0ll,  0xffffll}, /* SPU_TI_U16   */
 {       0ll, 0x3ffffll}, /* SPU_TI_U16_2 */
 {       0ll, 0x3ffffll}, /* SPU_TI_U18   */
};

enum spu_builtin_type {
    B_INSN,
    B_JUMP,
    B_CJUMP,	/* conditional jump */
    B_BISLED,
    B_CALL,
    B_HINT,
    B_OVERLOAD, 
    B_INTERNAL
};

static GTY(()) tree spu_type_node_by_offset_ti[SPU_TI_LAST - SPU_TI_FIRST + 1];

#define SPU_INTERNAL_TYPE_NODE(i) \
	(spu_type_node_by_offset_ti[(i) - SPU_TI_FIRST])

typedef enum {
#define DEF_BUILTIN(fcode, icode, name, type, params) fcode,
#include "spu_builtins.def"
#undef DEF_BUILTIN
   NUM_SPU_BUILTINS,
   BUILTIN_BRANCH_HINT,
   BUILTIN_EXPECT_CALL
} function_code;

struct spu_builtin_description {
    function_code fcode;
    int icode;
    const char *name;
    enum spu_builtin_type type;

    /* The first element of parm is always the return type.  The rest
     * are a zero terminated list of parameters. */
    int parm[5];

    tree fndecl;
};


static struct spu_builtin_description spu_builtins[] = {
#define DEF_BUILTIN(fcode, icode, name, type, params) \
  {fcode, icode, name, type, params, NULL_TREE},
#include "spu_builtins.def"
#undef DEF_BUILTIN
};


tree
spu_get_type_node_by_tree_index (int ti)
{
  if (spu_tree_index_p (ti))
    return SPU_INTERNAL_TYPE_NODE (ti);
  else
    {
      gcc_assert (ti < TI_MAX);
      return global_trees[ti];
    }
}

void
spu_set_type_node_by_tree_index (int ti, tree t)
{
  gcc_assert (spu_tree_index_p (ti));
  SPU_INTERNAL_TYPE_NODE (ti) = t;
}

tree
spu_select_overloaded_builtin (tree fndecl, tree fnargs)
{
  function_code new_fcode, fcode = DECL_FUNCTION_CODE (fndecl) - END_BUILTINS;
  struct spu_builtin_description *desc;
  tree match = NULL_TREE; 

  /* The vector types are not available if the backend is not initalized */
  gcc_assert (!flag_preprocess_only);

  if (fcode >= NUM_SPU_BUILTINS)
    return fndecl;

  desc = &spu_builtins[fcode];
  if (desc->type != B_OVERLOAD)
     return fndecl;

  /* Compare the signature of each internal builtin function with the
     function arguments until a match found. */

  for (new_fcode = fcode+1; spu_builtins[new_fcode].type == B_INTERNAL; new_fcode++)
    {
       tree decl = spu_builtins[new_fcode].fndecl;
       tree params = TYPE_ARG_TYPES (TREE_TYPE (decl));
       tree arg, param;
       int p;

       for (param = params, arg = fnargs, p = 0;
            param != void_list_node;
            param = TREE_CHAIN (param), arg = TREE_CHAIN (arg), p++)
         {
            tree var, arg_type, param_type = TREE_VALUE (param);

            if (!arg)
              {
                error("insufficient arguments to overloaded function %s",
                      desc->name);
                return match;
              }

            var = TREE_VALUE (arg);

            if (TREE_CODE (var) == NON_LVALUE_EXPR)
              var = TREE_OPERAND (var, 0);

            if (TREE_CODE (var) == ERROR_MARK)
              return fndecl; /* Let somebody else deal with the problem. */

            arg_type = TREE_TYPE (var);

	    /* This crazy compiler has two TREE_CODE's for each integer
	       type.  We call type_for_mode() to make sure we're getting
	       the same types for the arguments and the parameters. */
	    if (INTEGRAL_TYPE_P(arg_type))
	      arg_type = (*lang_hooks.types.type_for_mode)(TYPE_MODE (arg_type), TYPE_UNSIGNED (arg_type));

	    /* The intrinsics spec does not specify precisely how to
	       resolve generic intrinsics.  We require an exact match
	       for vector types and let C do it's usual parameter type
	       checking/promotions for scalar arguments, except for the
	       first argument of spu_splats and spu_promote which have
	       no vector parameters. */
            if ((TREE_CODE (param_type) == VECTOR_TYPE
		 || ((fcode == SPU_SPLATS || fcode == SPU_PROMOTE
		      || fcode == SPU_HCMPEQ || fcode == SPU_HCMPGT
		      || fcode == SPU_MASKB || fcode == SPU_MASKH || fcode == SPU_MASKW)
		     && p == 0))
		/* In gcc4 comptypes only accepts 2 arguments */
		&& !comptypes (TYPE_MAIN_VARIANT(param_type),TYPE_MAIN_VARIANT(arg_type)))
              break;
         }
         if (param == void_list_node)
           {
             if (arg)
               {
                 error("too many arguments to overloaded function %s",
                        desc->name);
                 return match;
               }

             match = decl;
             break;
           }
    }
    if (match == NULL_TREE)
      error("parameter list does not match a valid signature for %s()", 
             desc->name);
    return match;
}

tree
spu_expand_tree_builtin (tree function, tree params ATTRIBUTE_UNUSED,
			 tree coerced_params)
{
  unsigned int fcode = DECL_FUNCTION_CODE (function) - END_BUILTINS;
  struct spu_builtin_description *d;
  tree arg;
  int p;

  if (coerced_params
      && fcode < sizeof (spu_builtins) / sizeof (spu_builtins[0]))
    {
      d = &spu_builtins[fcode];
      for (p = 1; d->parm[p] != 0; p++)
	{
	  if (d->parm[p] == P_BLAB)
	    {
	      arg = TREE_VALUE (coerced_params);
	      STRIP_NOPS (arg);
	      if (TREE_CODE (arg) != ADDR_EXPR)
		break;
	      arg = TREE_OPERAND (arg, 0);
	      if (TREE_CODE (arg) != LABEL_DECL)
		break;
	      define_label (input_location, DECL_NAME (arg));
	    }
	  coerced_params = TREE_CHAIN (coerced_params);
	}
    }
  return NULL_TREE;
}

/* Given a (CONST (PLUS (SYMBOL_REF) (CONST_INT))) return TRUE when the
 * CONST_INT fits in signed 18 bits. */
int
legitimate_const(rtx x)
{
 /* We can never know if the resulting address fits in 18 bits and can be
 loaded with ila.  Instead we should use the HI and LO relocations to
 load a 32 bit address. */
  rtx sym, cst;
  HOST_WIDE_INT v;
  if (GET_CODE(XEXP(x,0)) != PLUS
      || GET_CODE(sym = XEXP(XEXP(x,0),0)) != SYMBOL_REF
      || GET_CODE(cst = XEXP(XEXP(x,0),1)) != CONST_INT)
    return 0;
  /* Assume that a very small constant is ok */
  if (CONST_OK_FOR_LETTER_P(v = INTVAL(cst),'K'))
    return 1;
  return 0;
}

static void
spu_check_builtin_parm( struct spu_builtin_description *d, rtx op, int p)
{
  /* P_BLAB is used for labels in branch and branch hint intrinsics. */
  if (p == P_BLAB)
    {
      /* P_BLAB is always the last operand so nothing else will
	 be emitted between the label and the branch. */
      if (op == const0_rtx)
	return;
      if (GET_CODE(op) != LABEL_REF)
	error("Invalid label address to %s", d->name);
      emit_label(XEXP(op,0));
      forced_labels = gen_rtx_EXPR_LIST (VOIDmode,
					 XEXP(op,0),
					 forced_labels);
    }
  /* Check the range of immediate operands. */
  else if (spu_tree_index_int_p (p))
    {
      int range =  p - SPU_TI_INT_FIRST;
      if (!CONSTANT_P(op)
	  || (GET_CODE(op) == CONST_INT
	      && (INTVAL(op) < spu_builtin_range[range].low
		  || INTVAL(op) > spu_builtin_range[range].high)))
	error("%s expects an integer literal in the range [%d, %d].",
	      d->name,
	      spu_builtin_range[range].low,
	      spu_builtin_range[range].high);
      if (GET_CODE(op) == CONST_INT)
	{
	  int mask;
	  switch (p) {
	  case SPU_TI_S10_4:
	    mask = 15;
	    break;
	  case SPU_TI_S16_2:
	  case SPU_TI_U16_2:
	    mask = 3;
	    break;
	  default:
	    mask = 0;
	  }
	  if ((INTVAL(op) & mask) != 0)
	    warning("least significant bits of %s are ignored.", d->name);
	}
    }
}

static void
expand_builtin_args (struct spu_builtin_description *d, tree arglist,
		     rtx target, rtx ops[])
{
  enum insn_code icode = d->icode;
  int i = 0;

  /* Expand the arguments into rtl. */

  if (d->parm[0] != TI_VOID_TYPE)
      ops[i++] = target;

  for (; i < insn_data[icode].n_operands; i++)
    if (insn_data[icode].operand[i].predicate != scratch_operand)
      {
	tree arg = TREE_VALUE (arglist);
	if (arg == 0)
	  abort();
	ops[i] = expand_expr(arg, NULL_RTX, VOIDmode, EXPAND_NORMAL);
	arglist = TREE_CHAIN(arglist);
      }
}

static rtx
spu_expand_builtin_1 (struct spu_builtin_description *d,
		      tree arglist, rtx target)
{
  rtx pat;
  rtx ops[8];
  enum insn_code icode = d->icode;
  enum machine_mode mode, tmode;
  int i, p;
  tree return_type;

  /* Set up ops[] with values from arglist. */
  expand_builtin_args(d, arglist, target, ops);

  /* Handle the target operand which must be operand 0. */
  i = 0;
  if (d->parm[0] != TI_VOID_TYPE)
    {

      /* We prefer the mode specified for the match_operand otherwise
       * use the mode from the builtin function prototype. */
      tmode = insn_data[d->icode].operand[0].mode;
      if (tmode == VOIDmode)
	tmode = TYPE_MODE(spu_get_type_node_by_tree_index(d->parm[0]));

      /* Try to use target because not using it can lead to extra copies
       * and when we are using all of the registers extra copies leads
       * to extra spills.  See EEMBC full-fury FFT code. */
      if (target && GET_CODE(target) == REG && GET_MODE(target) == tmode)
	ops[0] = target;
      /*  Using SUBREG in the target prevents the combine phase from
       *  working well because it moves the SUBREG from the DEST to the
       *  SOURCE.
      else if (target && GET_CODE(target) == REG
	  && GET_MODE_SIZE(tmode) == GET_MODE_SIZE(GET_MODE(target)))
	ops[0] = gen_rtx_SUBREG(tmode, target, 0);
      */
      else
	target = ops[0] = gen_reg_rtx(tmode);

      if (!(*insn_data[icode].operand[0].predicate) (ops[0], tmode))
        abort();

      i++;
    }

  /* Ignore align_hint, but still expand it's args in case they have
   * side effects. */
  if (icode == CODE_FOR_spu_align_hint)
    return 0;

  /* Handle the rest of the operands. */
  for (p = 1; i < insn_data[icode].n_operands; i++, p++)
    {
      if (insn_data[d->icode].operand[i].mode != VOIDmode)
	mode = insn_data[d->icode].operand[i].mode;
      else if (icode == CODE_FOR_spu_splats
	       || icode == CODE_FOR_spu_extract
	       || icode == CODE_FOR_spu_insert
	       || icode == CODE_FOR_spu_promote
	       || icode == CODE_FOR_spu_convert
	       || icode == CODE_FOR_spu_selb)
	mode = TYPE_MODE(spu_get_type_node_by_tree_index(d->parm[i]));
      else /* The logical operations.  */
	mode = TYPE_MODE(spu_get_type_node_by_tree_index(d->parm[0]));

      /* mode can be VOIDmode here for labels */

      /* For specific intrinsics with an immediate operand, e.g.,
       * si_ai(), we sometimes need to convert the scalar argument to a
       * vector argument by splatting the scalar. */
      if (VECTOR_MODE_P(mode)
	  && (GET_CODE(ops[i]) == CONST_INT
	      || GET_MODE_CLASS(GET_MODE(ops[i])) == MODE_INT
	      || GET_MODE_CLASS(GET_MODE(ops[i])) == MODE_FLOAT))
	{
	  if (GET_CODE(ops[i]) == CONST_INT)
	    ops[i] = spu_const_vector(mode, ops[i]);
	  else
	    {
	      rtx reg = gen_reg_rtx(mode);
	      enum machine_mode imode = GET_MODE_INNER (mode);
	      if (! (*insn_data[CODE_FOR_spu_splats].operand[1].predicate) (ops[i], GET_MODE(ops[i])))
		ops[i] = force_reg(GET_MODE(ops[i]), ops[i]);
	      if (imode != GET_MODE(ops[i]))
		ops[i] = convert_to_mode(imode, ops[i],
					 TYPE_UNSIGNED (spu_get_type_node_by_tree_index(d->parm[i])));
	      spu_emit_insn(gen_spu_splats(reg, ops[i]));
	      ops[i] = reg;
	    }
	}

      if (! (*insn_data[icode].operand[i].predicate) (ops[i], mode))
	ops[i] = spu_force_reg(mode, ops[i]);

      spu_check_builtin_parm(d, ops[i], d->parm[p]);

    }

  switch (insn_data[icode].n_operands)
  {
  case 0: pat = GEN_FCN (icode) (0); break;
  case 1: pat = GEN_FCN (icode) (ops[0]); break;
  case 2: pat = GEN_FCN (icode) (ops[0], ops[1]); break;
  case 3: pat = GEN_FCN (icode) (ops[0], ops[1], ops[2]); break;
  case 4: pat = GEN_FCN (icode) (ops[0], ops[1], ops[2], ops[3]); break;
  case 5: pat = GEN_FCN (icode) (ops[0], ops[1], ops[2], ops[3], ops[4]); break;
  case 6: pat = GEN_FCN (icode) (ops[0], ops[1], ops[2], ops[3], ops[4], ops[5]); break;
  default: abort();
  }

  if (! pat)
    abort();

  if (d->type == B_CALL || d->type == B_BISLED)
    emit_call_insn (pat);
  else if (d->type == B_CJUMP)
    emit_jump_insn (pat);
  else if (d->type == B_JUMP)
    {
      emit_jump_insn (pat);
      emit_barrier();
    }
  else
    spu_emit_insn (pat);

  return_type = spu_get_type_node_by_tree_index(d->parm[0]);
  if (d->parm[0] != TI_VOID_TYPE
      && GET_MODE(target) != TYPE_MODE(return_type))
    {
      /* target is the return value.  It should always be the mode of
       * the builtin function prototype. */
      target = spu_force_reg(TYPE_MODE(return_type), target);
    }

  return target;
}

rtx
spu_expand_builtin (
     tree exp,
     rtx target,
     rtx subtarget ATTRIBUTE_UNUSED,
     enum machine_mode mode ATTRIBUTE_UNUSED,
     int ignore ATTRIBUTE_UNUSED)
{
  tree fndecl = TREE_OPERAND (TREE_OPERAND (exp, 0), 0);
  unsigned int fcode = DECL_FUNCTION_CODE (fndecl) - END_BUILTINS;
  tree arglist = TREE_OPERAND (exp, 1);
  struct spu_builtin_description *d;

  if (fcode < NUM_SPU_BUILTINS)
  {
    d = &spu_builtins[fcode];
    return spu_expand_builtin_1(d, arglist, target);
  }
  if (fcode == BUILTIN_BRANCH_HINT)
    return emit_insn(gen_branch_hint(const0_rtx, const0_rtx));
  if (fcode == BUILTIN_EXPECT_CALL)
    {
      tree arg1 = TREE_VALUE (arglist);
      tree arg2 = TREE_VALUE (TREE_CHAIN (arglist));
      rtx insns, insn, set, target2, hint;
      start_sequence ();
      target2 = expand_expr(arg2, NULL, Pmode, EXPAND_NORMAL);
      target2 = force_reg (Pmode, target2);
      target = expand_expr(arg1, target, Pmode, EXPAND_NORMAL);
      target = force_reg (Pmode, target);
      hint = emit_insn(gen_branch_hint(const0_rtx, target2));
      insns = get_insns();
      end_sequence();
      gcc_assert(target && target != const0_rtx);
      for (insn = insns, set = 0; insn ; insn = NEXT_INSN (insn))
	if ((set_of(target, insn)) != NULL_RTX)
	  set = insn;
      if (set)
	REG_NOTES (set)
	  = gen_rtx_EXPR_LIST (REG_BR_HINT,
			       gen_rtx_INSN_LIST(0, hint, 0), REG_NOTES (set));
      emit_insn(insns);
      return target;
    }
  /* @@@ Should really do something sensible here.  */
  abort ();
  return const0_rtx;
}

static void
spu_init_internal_types (void)
{
  enum spu_tree_index ti;

  for (ti = SPU_TI_INT_FIRST; ti <= SPU_TI_INT_LAST; ti++)
    SPU_INTERNAL_TYPE_NODE(ti) = integer_type_node;

  SPU_INTERNAL_TYPE_NODE(SPU_TI_UV16QI_TYPE) = build_vector_type (unsigned_intQI_type_node, 16);
  SPU_INTERNAL_TYPE_NODE(SPU_TI_UV8HI_TYPE) = build_vector_type (unsigned_intHI_type_node, 8);
  SPU_INTERNAL_TYPE_NODE(SPU_TI_UV4SI_TYPE) = build_vector_type (unsigned_intSI_type_node, 4);
  SPU_INTERNAL_TYPE_NODE(SPU_TI_UV2DI_TYPE) = build_vector_type (unsigned_intDI_type_node, 2);

  SPU_INTERNAL_TYPE_NODE(SPU_TI_V16QI_TYPE) = build_vector_type (intQI_type_node, 16);
  SPU_INTERNAL_TYPE_NODE(SPU_TI_V8HI_TYPE) = build_vector_type (intHI_type_node, 8);
  SPU_INTERNAL_TYPE_NODE(SPU_TI_V4SI_TYPE) = build_vector_type (intSI_type_node, 4);
  SPU_INTERNAL_TYPE_NODE(SPU_TI_V2DI_TYPE) = build_vector_type (intDI_type_node, 2);
  SPU_INTERNAL_TYPE_NODE(SPU_TI_V4SF_TYPE) = build_vector_type (float_type_node, 4);
  SPU_INTERNAL_TYPE_NODE(SPU_TI_V2DF_TYPE) = build_vector_type (double_type_node, 2);
}

void
spu_init_builtins (void)
{
  struct spu_builtin_description *d;
  unsigned int i;
  tree p, rtype;
  tree cvptr;

  /* we need the backend to initialize register type information. If
     we do not invoke the backend, we cannot initialize the builtins. */
  if (flag_preprocess_only)
    return;

  cvptr = build_pointer_type (build_qualified_type (void_type_node, TYPE_QUAL_CONST | TYPE_QUAL_VOLATILE));

  spu_init_internal_types();

  for (i = 0, d = spu_builtins; i < NUM_SPU_BUILTINS; i++, d++)
    {
      char name[64];  /* build_function will make a copy. */
      int parm;

      if (d->name == 0)
	continue;

      /* find last parm */
      for (parm = 1; d->parm[parm] != 0; parm++)
	{ }

      p = void_list_node;
      while (parm > 1)
	{
	  parm--;

	  if (d->parm[parm] == P_BLAB)
	    p = tree_cons(NULL_TREE, const_ptr_type_node, p);
	  else
            {
              tree type = spu_get_type_node_by_tree_index(d->parm[parm]);
              
              if (POINTER_TYPE_P (type))
                p = tree_cons(NULL_TREE,cvptr,p);
              else
	        p = tree_cons(NULL_TREE, 
                              (*lang_hooks.types.type_for_mode)(TYPE_MODE (type), TYPE_UNSIGNED (type)), 
                              p);
            }
	}
      rtype = spu_get_type_node_by_tree_index(d->parm[0]);
      rtype = (*lang_hooks.types.type_for_mode)(TYPE_MODE (rtype), TYPE_UNSIGNED (rtype));

      p = build_function_type (rtype, p);

      sprintf(name, "__builtin_%s", d->name);
      d->fndecl = builtin_function (name, p, END_BUILTINS + i, BUILT_IN_MD, NULL, NULL_TREE);
    }

    builtin_function ("__builtin_branch_hint",
		      build_function_type (cvptr, void_list_node),
		      END_BUILTINS + BUILTIN_BRANCH_HINT,
		      BUILT_IN_MD, NULL, NULL_TREE);

    p = void_list_node;
    p = tree_cons(NULL_TREE,cvptr,p);
    p = tree_cons(NULL_TREE,cvptr,p);
    builtin_function ("__builtin_expect_call",
		      build_function_type (cvptr, p),
		      END_BUILTINS + BUILTIN_EXPECT_CALL,
		      BUILT_IN_MD, NULL, NULL_TREE);
}

/* The following are originally Apple's code in their 4.0.0 port. I adapted
 * and simplified it for the SPU.
 */
#define builtin_define(TXT) cpp_define (pfile, TXT)

/* APPLE LOCAL begin AltiVec */
/* Keep the AltiVec keywords handy for fast comparisons.  */

/* Apple's code has a bug. The cpp_hashnodes are embedded into identifier tree
   nodes So we should tell the garbage collector about the container tree 
   nodes */ 
static GTY(()) tree __vector_keyword;
static GTY(()) tree vector_keyword;

static cpp_hashnode *
spu_categorize_keyword (const cpp_token *tok)
{
  if (tok->type == CPP_NAME)
    {
      cpp_hashnode *ident = tok->val.node;
      cpp_hashnode *vector_cpp_hashnode =
	CPP_HASHNODE (GCC_IDENT_TO_HT_IDENT (vector_keyword));
      cpp_hashnode *__vector_cpp_hashnode =
	CPP_HASHNODE (GCC_IDENT_TO_HT_IDENT (__vector_keyword));

      if (ident == vector_cpp_hashnode || ident == __vector_cpp_hashnode)
	return __vector_cpp_hashnode;

      return ident;
    }

  return 0;
}

/* Called to decide whether a conditional macro should be expanded.
   Since we have exactly one such macro (i.e, 'vector'), we do not
   need to examine the 'tok' parameter.  */

cpp_hashnode *
spu_macro_to_expand (cpp_reader *pfile, const cpp_token *tok)
{
  static bool vector_keywords_init = false;
  cpp_hashnode *expand_this = tok->val.node;
  cpp_hashnode *ident;
  cpp_hashnode *__vector_cpp_hashnode;

  if (!vector_keywords_init)
    {
      spu_init_vector_keywords (pfile);
      vector_keywords_init = true;
    }

  ident = spu_categorize_keyword (tok);
  __vector_cpp_hashnode =
    CPP_HASHNODE (GCC_IDENT_TO_HT_IDENT (__vector_keyword));

  if (ident == __vector_cpp_hashnode)
    {
      tok = _cpp_peek_token (pfile, 0);
      ident = spu_categorize_keyword (tok);

      if (ident)
	{
	  enum rid rid_code = (enum rid)(ident->rid_code);
	  if (ident->type == NT_MACRO)
	    {
	      (void)cpp_get_token (pfile);
	      tok = _cpp_peek_token (pfile, 0);
	      ident = spu_categorize_keyword (tok);
	      rid_code = (enum rid)(ident->rid_code);
	    }

	  /* we requires integer types to be either signed or unsigned.
	     in other words, "vector signed int" is legal but not
	     "vector int" */
	  if (rid_code == RID_UNSIGNED || rid_code == RID_SIGNED
	      || rid_code == RID_FLOAT || rid_code == RID_DOUBLE)
	    {
	      expand_this = __vector_cpp_hashnode;
	    }
	}
    }

  return expand_this;
}

static void
spu_init_vector_keywords (cpp_reader *pfile)
{
  cpp_hashnode *node;

  /* Keywords without two leading underscores are context-sensitive, and
     hence implemented as conditional macros, controlled by the
     spu_macro_to_expand() function above.  */

  node = cpp_lookup (pfile, DSC ("__vector"));
  node->flags |= NODE_CONDITIONAL;
  __vector_keyword = HT_IDENT_TO_GCC_IDENT (& node->ident);

  node = cpp_lookup (pfile, DSC ("vector"));
  node->flags |= NODE_CONDITIONAL;
  vector_keyword = HT_IDENT_TO_GCC_IDENT (& node->ident);

  return;
}

/* APPLE LOCAL end AltiVec */

void
spu_cpu_cpp_builtins (cpp_reader *pfile)
{
  builtin_define_std ("__SPU__");
  cpp_assert (pfile, "cpu=spu");
  cpp_assert (pfile, "machine=spu");

  /* Define the AltiVec syntactic elements. */
  builtin_define ("__vector=__attribute__((spu_vector))");

  /* The prototype for __builtin_expect_call uses void *, but we want it
   * to be usable without casts, so we add the casts with a define. */
  builtin_define ("__builtin_expect_call(a,b)=((__typeof(a))__builtin_expect_call((void*)a,(void*)b))");

  /* APPLE LOCAL begin AltiVec */
  builtin_define ("vector=vector");
  spu_init_vector_keywords (pfile);

  /* Enable context-sensitive macros. */
  cpp_get_callbacks (pfile)->macro_to_expand = spu_macro_to_expand;
  /* APPLE LOCAL end AltiVec */
}

#include "gt-spu-c.h"
