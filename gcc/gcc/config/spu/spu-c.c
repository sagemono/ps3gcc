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
tree spu_resolve_overloaded_builtin (tree, tree);
int legitimate_const (rtx, int);
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
 {   -0x40ll,    0x7fll}, /* SPU_BTI_7     */
 {   -0x40ll,    0x3fll}, /* SPU_BTI_S7    */
 {       0ll,    0x7fll}, /* SPU_BTI_U7    */
 {  -0x200ll,   0x1ffll}, /* SPU_BTI_S10   */
 { -0x2000ll,  0x1fffll}, /* SPU_BTI_S10_4 */
 {       0ll,  0x3fffll}, /* SPU_BTI_U14   */
 { -0x8000ll,  0xffffll}, /* SPU_BTI_16    */
 { -0x8000ll,  0x7fffll}, /* SPU_BTI_S16   */
 {-0x20000ll, 0x1ffffll}, /* SPU_BTI_S16_2 */
 {       0ll,  0xffffll}, /* SPU_BTI_U16   */
 {       0ll, 0x3ffffll}, /* SPU_BTI_U16_2 */
 {       0ll, 0x3ffffll}, /* SPU_BTI_U18   */
};

struct spu_builtin_description spu_builtins[] = {
#define DEF_BUILTIN(fcode, icode, name, type, params) \
  {fcode, icode, name, type, params, NULL_TREE},
#include "spu_builtins.def"
#undef DEF_BUILTIN
};

/* Built in types.  */
tree spu_builtin_types[SPU_BTI_MAX];


/*
 * target hook for resolve_overloaded_builtin(). Returns a
 * function call RTX if we can resolve the overloaded builtin
 */
tree
spu_resolve_overloaded_builtin (tree fndecl, tree fnargs)
{
#define SCALAR_TYPE_P(t) (INTEGRAL_TYPE_P (t) \
			  || SCALAR_FLOAT_TYPE_P (t) \
			  || POINTER_TYPE_P (t))
  enum spu_function_code new_fcode, fcode = DECL_FUNCTION_CODE (fndecl) - END_BUILTINS;
  struct spu_builtin_description *desc;
  tree match = NULL_TREE; 

  /* The vector types are not available if the backend is not initalized */
  gcc_assert (!flag_preprocess_only);

  if (fcode >= NUM_SPU_BUILTINS)
    return build_function_call_expr (fndecl, fnargs);

  desc = &spu_builtins[fcode];
  if (desc->type != B_OVERLOAD)
     return NULL_TREE;

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
                return error_mark_node;
              }

            var = TREE_VALUE (arg);

            if (TREE_CODE (var) == NON_LVALUE_EXPR)
              var = TREE_OPERAND (var, 0);

            if (TREE_CODE (var) == ERROR_MARK)
              return NULL_TREE; /* Let somebody else deal with the problem. */

            arg_type = TREE_TYPE (var);

	    /* This crazy compiler has two TREE_CODE's for each integer
	       type.  We call type_for_mode() to make sure we're getting
	       the same types for the arguments and the parameters. */
	    if (INTEGRAL_TYPE_P(arg_type))
	      arg_type = (*lang_hooks.types.type_for_mode)(TYPE_MODE (arg_type), TYPE_UNSIGNED (arg_type));

	    /* When a vecreg is used it must be promoted to a vector type to
 	       be in the same format as the builtin's parameters */	    

	    if (TREE_CODE (arg_type) == VECTOR_TYPE && TYPE_VECREG (arg_type)
	         && TREE_CODE (param_type) == VECTOR_TYPE)
	    {	
		arg_type = build_vector_type (TREE_TYPE (arg_type),TYPE_VECTOR_SUBPARTS (arg_type));
	    }

	       /* The intrinsics spec does not specify precisely how to
	       resolve generic intrinsics.  We require an exact match
	       for vector types and let C do it's usual parameter type
	       checking/promotions for scalar arguments, except for the
	       first argument of spu_splats and spu_promote which have
	       no vector parameters. */
            if ((!SCALAR_TYPE_P (param_type)
		 || !SCALAR_TYPE_P (arg_type)
		 || ((fcode == SPU_SPLATS || fcode == SPU_PROMOTE
		      || fcode == SPU_HCMPEQ || fcode == SPU_HCMPGT
		      || fcode == SPU_MASKB || fcode == SPU_MASKH || fcode == SPU_MASKW)
		     && p == 0))
		/* In gcc4 comptypes only accepts 2 arguments */
		&& !comptypes (TYPE_MAIN_VARIANT(param_type), TYPE_MAIN_VARIANT(arg_type)))
              break;
         }
         if (param == void_list_node)
           {
             if (arg)
               {
                 error("too many arguments to overloaded function %s",
                        desc->name);
                 return error_mark_node;
               }

             match = decl;
             break;
           }
    }

  if (match == NULL_TREE)
    {
      error ("parameter list does not match a valid signature for %s()", 
      	     desc->name);
      return error_mark_node;
    }

  return build_function_call (match, fnargs);
#undef SCALAR_TYPE_P
}

static void
spu_check_builtin_parm( struct spu_builtin_description *d, rtx op, int p)
{
  HOST_WIDE_INT v = 0;
  int lsbits;
  /* Check the range of immediate operands. */
  if (p >= SPU_BTI_7 && p <= SPU_BTI_U18)
    {
      int range = p - SPU_BTI_7;

      if (!CONSTANT_P (op))
	error ("%s expects an integer literal in the range [%d, %d].",
	       d->name,
	       spu_builtin_range[range].low, spu_builtin_range[range].high);

      if (GET_CODE (op) == CONST
	  && (GET_CODE (XEXP (op, 0)) == PLUS
	      || GET_CODE (XEXP (op, 0)) == MINUS))
	{
	  v = INTVAL (XEXP (XEXP (op, 0), 1));
	  op = XEXP (XEXP (op, 0), 0);
	}
      else if (GET_CODE (op) == CONST_INT)
	v = INTVAL (op);
      else if (GET_CODE (op) == CONST_VECTOR
	       && GET_CODE (CONST_VECTOR_ELT (op, 0)) == CONST_INT)
	v = INTVAL (CONST_VECTOR_ELT (op, 0));

      /* The default for v is 0 which is valid in every range. */
      if (v < spu_builtin_range[range].low
	  || v > spu_builtin_range[range].high)
	error ("%s expects an integer literal in the range [%d, %d]. ("
	       HOST_WIDE_INT_PP_PRINT_DEC ")",
	       d->name,
	       spu_builtin_range[range].low, spu_builtin_range[range].high,
	       v);

      if (d->fcode == SI_STOP && v == 0 && TARGET_WARN_STOP0)
	warning (0, "Recommend using non-zero argument to si_stop and spu_stop.");

      switch (p)
	{
	case SPU_BTI_S10_4:
	  lsbits = 4;
	  break;
	case SPU_BTI_U16_2:
	  /* This is only used in lqa, and stqa.  Even though the insns
	     encode 16 bits of the address (all but the 2 least
	     significant), only 14 bits are used because it is masked to
	     be 16 byte aligned. */
	  lsbits = 4;
	  break;
	case SPU_BTI_S16_2:
	  /* This is used for lqr and stqr. */
	  lsbits = 2;
	  break;
	default:
	  lsbits = 0;
	}

      if (GET_CODE (op) == LABEL_REF
	  || (GET_CODE (op) == SYMBOL_REF
	      && SYMBOL_REF_FUNCTION_P (op))
	  || (v & ((1 << lsbits) - 1)) != 0)
	warning (0, "%d least significant bits of %s are ignored.", lsbits,
		 d->name);
    }
}

static void
expand_builtin_args (struct spu_builtin_description *d, tree arglist,
		     rtx target, rtx ops[])
{
  enum insn_code icode = d->icode;
  int i = 0;

  /* Expand the arguments into rtl. */

  if (d->parm[0] != SPU_BTI_VOID)
    ops[i++] = target;

  for (; i < insn_data[icode].n_operands; i++)
    {
      tree arg = TREE_VALUE (arglist);
      if (arg == 0)
	abort ();
      ops[i] = expand_expr (arg, NULL_RTX, VOIDmode, 0);
      arglist = TREE_CHAIN (arglist);
    }
}

static rtx
spu_force_reg_operand (enum machine_mode mode, rtx op)
{
  rtx x, r;
  if (GET_MODE (op) == VOIDmode || GET_MODE (op) == BLKmode)
    {
      if ((SCALAR_INT_MODE_P (mode) && GET_CODE (op) == CONST_INT)
	  || GET_MODE (op) == BLKmode)
	return force_reg (mode, convert_to_mode (mode, op, 0));
      abort();
    }

  r = op;
  if (GET_CODE (r) == SUBREG)
    {
      enum machine_mode imode = GET_MODE (SUBREG_REG (r));
      if (GET_MODE_SIZE (GET_MODE (r)) == GET_MODE_SIZE (imode))
	r = SUBREG_REG (op);
      if (GET_MODE (r) == mode)
	return r;
    }
  else if (GET_CODE (r) != REG)
    r = force_reg(GET_MODE(op), op);

  if (GET_MODE_SIZE (GET_MODE (r)) == GET_MODE_SIZE (mode))
    {
      x = simplify_gen_subreg(mode, r, GET_MODE (r), 0);
      if (x)
	return x;
    }

  x = gen_reg_rtx(mode);
  spu_emit_insn(gen_spu_convert(x, r));
  return x;
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
  if (d->parm[0] != SPU_BTI_VOID)
    {

      /* We prefer the mode specified for the match_operand otherwise
       * use the mode from the builtin function prototype. */
      tmode = insn_data[d->icode].operand[0].mode;
      if (tmode == VOIDmode)
	tmode = TYPE_MODE (spu_builtin_types[d->parm[0]]);

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

    if (d->fcode == SPU_MASK_FOR_LOAD)
    {
      int icode = (int) CODE_FOR_spu_lvsr;
      enum machine_mode mode = insn_data[icode].operand[1].mode;
      tree arg;
      rtx addr, op, pat;

      /* get addr */
      arg = TREE_VALUE (arglist);
      gcc_assert (TREE_CODE (TREE_TYPE (arg)) == POINTER_TYPE);
      op = expand_expr (arg, NULL_RTX, Pmode, EXPAND_NORMAL);
      addr = memory_address (mode, op);

      /* negate addr */
      op = gen_reg_rtx (GET_MODE (addr));
      emit_insn (gen_rtx_SET (VOIDmode, op,
                 gen_rtx_NEG (GET_MODE (addr), addr)));
      op = gen_rtx_MEM (mode, op);

      pat = GEN_FCN (icode) (target, op);
      if (!pat)
        return 0;
      emit_insn (pat);
      return target;
    }

  /* Handle the rest of the operands. */
  for (p = 1; i < insn_data[icode].n_operands; i++, p++)
    {
      if (insn_data[d->icode].operand[i].mode != VOIDmode)
	mode = insn_data[d->icode].operand[i].mode;
      else
	mode = TYPE_MODE (spu_builtin_types[d->parm[i]]);

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
					 TYPE_UNSIGNED (spu_builtin_types [d->parm[i]]));
	      spu_emit_insn(gen_spu_splats(reg, ops[i]));
	      ops[i] = reg;
	    }
	}

      spu_check_builtin_parm(d, ops[i], d->parm[p]);

      if (! (*insn_data[icode].operand[i].predicate) (ops[i], mode))
	ops[i] = spu_force_reg_operand (mode, ops[i]);

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

  if (d->type == B_BISLED)
    emit_call_insn (pat);
  else
    spu_emit_insn (pat);

  return_type = spu_builtin_types[d->parm[0]];
  if (d->parm[0] != SPU_BTI_VOID
      && GET_MODE (target) != TYPE_MODE (return_type))
    {
      /* target is the return value.  It should always be the mode of
       * the builtin function prototype. */
      target = spu_force_reg_operand (TYPE_MODE(return_type), target);
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
    return emit_insn(gen_hbr(const0_rtx, const0_rtx));
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
      hint = emit_insn(gen_hbr(const0_rtx, target2));
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
  gcc_unreachable ();
  return const0_rtx;
}

void
spu_init_builtins (void)
{
  tree p;
  struct spu_builtin_description *d;
  unsigned int i;

  V16QI_type_node = build_vector_type (intQI_type_node, 16);
  V8HI_type_node = build_vector_type (intHI_type_node, 8);
  V4SI_type_node = build_vector_type (intSI_type_node, 4);
  V2DI_type_node = build_vector_type (intDI_type_node, 2);
  V4SF_type_node = build_vector_type (float_type_node, 4);
  V2DF_type_node = build_vector_type (double_type_node, 2);

  unsigned_V16QI_type_node = build_vector_type (unsigned_intQI_type_node, 16);
  unsigned_V8HI_type_node = build_vector_type (unsigned_intHI_type_node, 8);
  unsigned_V4SI_type_node = build_vector_type (unsigned_intSI_type_node, 4);
  unsigned_V2DI_type_node = build_vector_type (unsigned_intDI_type_node, 2);

  spu_builtin_types[SPU_BTI_QUADWORD] = V16QI_type_node;

  spu_builtin_types[SPU_BTI_7] = global_trees[TI_INTSI_TYPE];
  spu_builtin_types[SPU_BTI_S7] = global_trees[TI_INTSI_TYPE];
  spu_builtin_types[SPU_BTI_U7] = global_trees[TI_INTSI_TYPE];
  spu_builtin_types[SPU_BTI_S10] = global_trees[TI_INTSI_TYPE];
  spu_builtin_types[SPU_BTI_S10_4] = global_trees[TI_INTSI_TYPE];
  spu_builtin_types[SPU_BTI_U14] = global_trees[TI_INTSI_TYPE];
  spu_builtin_types[SPU_BTI_16] = global_trees[TI_INTSI_TYPE];
  spu_builtin_types[SPU_BTI_S16] = global_trees[TI_INTSI_TYPE];
  spu_builtin_types[SPU_BTI_S16_2] = global_trees[TI_INTSI_TYPE];
  spu_builtin_types[SPU_BTI_U16] = global_trees[TI_INTSI_TYPE];
  spu_builtin_types[SPU_BTI_U16_2] = global_trees[TI_INTSI_TYPE];
  spu_builtin_types[SPU_BTI_U18] = global_trees[TI_INTSI_TYPE];

  spu_builtin_types[SPU_BTI_INTQI] = global_trees[TI_INTQI_TYPE];
  spu_builtin_types[SPU_BTI_INTHI] = global_trees[TI_INTHI_TYPE];
  spu_builtin_types[SPU_BTI_INTSI] = global_trees[TI_INTSI_TYPE];
  spu_builtin_types[SPU_BTI_INTDI] = global_trees[TI_INTDI_TYPE];
  spu_builtin_types[SPU_BTI_UINTQI] = global_trees[TI_UINTQI_TYPE];
  spu_builtin_types[SPU_BTI_UINTHI] = global_trees[TI_UINTHI_TYPE];
  spu_builtin_types[SPU_BTI_UINTSI] = global_trees[TI_UINTSI_TYPE];
  spu_builtin_types[SPU_BTI_UINTDI] = global_trees[TI_UINTDI_TYPE];

  spu_builtin_types[SPU_BTI_FLOAT] = global_trees[TI_FLOAT_TYPE];
  spu_builtin_types[SPU_BTI_DOUBLE] = global_trees[TI_DOUBLE_TYPE];

  spu_builtin_types[SPU_BTI_VOID] = global_trees[TI_VOID_TYPE];

  spu_builtin_types[SPU_BTI_PTR] =
    build_pointer_type (build_qualified_type
			(void_type_node,
			 TYPE_QUAL_CONST | TYPE_QUAL_VOLATILE));

  /* For each builtin we build a new prototype.  The tree code will make
     sure nodes are shared. */
  for (i = 0, d = spu_builtins; i < NUM_SPU_BUILTINS; i++, d++)
    {
      char name[64];		/* build_function will make a copy. */
      int parm;

      if (d->name == 0)
	continue;

      /* find last parm */
      for (parm = 1; d->parm[parm] != SPU_BTI_END_OF_PARAMS; parm++)
	{
	}

      p = void_list_node;
      while (parm > 1)
	p = tree_cons (NULL_TREE, spu_builtin_types[d->parm[--parm]], p);

      p = build_function_type (spu_builtin_types[d->parm[0]], p);

      sprintf(name, "__builtin_%s", d->name);

      d->fndecl = builtin_function (name, p, END_BUILTINS + i, BUILT_IN_MD, NULL, NULL_TREE);
      if (d->fcode == SPU_MASK_FOR_LOAD)
	TREE_READONLY (d->fndecl) = 1;  
    }

    builtin_function ("__builtin_branch_hint",
		      build_function_type (spu_builtin_types[SPU_BTI_PTR], void_list_node),
		      END_BUILTINS + BUILTIN_BRANCH_HINT,
		      BUILT_IN_MD, NULL, NULL_TREE);

    p = void_list_node;
    p = tree_cons(NULL_TREE,spu_builtin_types[SPU_BTI_PTR],p);
    p = tree_cons(NULL_TREE,spu_builtin_types[SPU_BTI_PTR],p);
    builtin_function ("__builtin_expect_call",
		      build_function_type (spu_builtin_types[SPU_BTI_PTR], p),
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

      if (ident == vector_cpp_hashnode)
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

  if (ident != expand_this)
    expand_this = NULL;

  __vector_cpp_hashnode =
    CPP_HASHNODE (GCC_IDENT_TO_HT_IDENT (__vector_keyword));

  if (ident == __vector_cpp_hashnode)
    {
      int idx = 0;
      do
	tok = _cpp_peek_token (pfile, idx++);
      while (tok->type == CPP_PADDING);
      ident = spu_categorize_keyword (tok);

      if (ident)
	{
	  enum rid rid_code = (enum rid)(ident->rid_code);
	  if (ident->type == NT_MACRO)
	    {
	      do
		(void)cpp_get_token (pfile);
	      while (--idx > 0);
	      do
		tok = _cpp_peek_token (pfile, idx++);
	      while (tok->type == CPP_PADDING);
	      ident = spu_categorize_keyword (tok);
	      if (ident)
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

/* Defined in c-cppbuiltin.c and not declared in any header. */
extern void builtin_define_std (const char *);

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

/* Implement targetm.vectorize.builtin_mask_for_load.  */
tree
spu_builtin_mask_for_load (void)
{
  struct spu_builtin_description *d = &spu_builtins[SPU_MASK_FOR_LOAD];
  gcc_assert (d);
  return d->fndecl;
}

/* Implement targetm.vectorize.builtin_mul_widen_even.  */
tree
spu_builtin_mul_widen_even (tree type)
{
  struct spu_builtin_description *d;
  switch (TYPE_MODE (type))
    {
    case V8HImode:
      d = TYPE_UNSIGNED (type) ? &spu_builtins[SPU_MULE_0] :
                                 &spu_builtins[SPU_MULE_1];
      break;
    default:
      return NULL_TREE;
    }

  return d->fndecl;
}

/* Implement targetm.vectorize.builtin_mul_widen_odd.  */
tree
spu_builtin_mul_widen_odd (tree type)
{
  struct spu_builtin_description *d;
  switch (TYPE_MODE (type))
    {
    case V8HImode:
      d = TYPE_UNSIGNED (type) ? &spu_builtins[SPU_MULO_1] :
                                 &spu_builtins[SPU_MULO_0];
      break;
    default:
      return NULL_TREE;
    }

  return d->fndecl;
}

void
spu_c_common_override_options (void)
{
  warn_main = 0;
}

/** SCE bugilla #11003 **/
/* APPLE LOCAL begin pragma reverse_bitfields, ms_struct */
/* Handle the reverse_bitfields pragma.  */
extern int darwin_reverse_bitfields;

void
darwin_pragma_reverse_bitfields (cpp_reader *pfile ATTRIBUTE_UNUSED)
{
  const char* arg;
  tree t;

  if (c_lex (&t) != CPP_NAME) {
    warning (0, "malformed '#pragma options', ignoring");
    return;
  }
  arg = IDENTIFIER_POINTER (t);

  if (!strcmp (arg, "on")) {
    darwin_reverse_bitfields = true;
  }
  else if (!strcmp (arg, "off") || !strcmp (arg, "reset"))
    darwin_reverse_bitfields = false;
  else
    warning (0, "malformed '#pragma reverse_bitfields {on|off|reset}', ignoring");
  if (c_lex (&t) != CPP_EOF)
    warning (0, "junk at end of '#pragma reverse_bitfields'");
}

extern int darwin_ms_struct;

/* Parse the ms_struct pragma.  */
void
darwin_pragma_ms_struct (cpp_reader *pfile ATTRIBUTE_UNUSED)
{
  const char *arg;
  tree t;

  if (c_lex (&t) != CPP_NAME) {
    warning (0, "malformed '#pragma ms_struct', ignoring");
    return;
  }
  arg = IDENTIFIER_POINTER (t);

  if (!strcmp (arg, "on")) {
    darwin_ms_struct = true;
  }
  else if (!strcmp (arg, "off") || !strcmp (arg, "reset"))
    darwin_ms_struct = false;
  else
    warning (0, "malformed '#pragma ms_struct {on|off|reset}', ignoring");

  if (c_lex (&t) != CPP_EOF)
    warning (0, "junk at end of '#pragma ms_struct'");
}
/* APPLE LOCAL end pragma reverse_bitfields, ms_struct */


#include "gt-spu-c.h"
