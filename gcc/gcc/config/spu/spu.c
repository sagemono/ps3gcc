
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
#include "rtl.h"
#include "regs.h"
#include "hard-reg-set.h"
#include "real.h"
#include "insn-config.h"
#include "conditions.h"
#include "insn-attr.h"
#include "flags.h"
#include "recog.h"
#include "obstack.h"
#include "tree.h"
#include "expr.h"
#include "optabs.h"
#include "except.h"
#include "function.h"
#include "output.h"
#include "basic-block.h"
#include "integrate.h"
#include "toplev.h"
#include "ggc.h"
#include "hashtab.h"
#include "tm_p.h"
#include "target.h"
#include "target-def.h"
#include "langhooks.h"
#include "reload.h"
#include "cfglayout.h"
#include "sched-int.h"
#include "params.h"
#include "assert.h"
#include "c-common.h"
#include "machmode.h"
#include "spu_types.h"
#include "tree-gimple.h"
#include "cfgloop.h"

/*  Target specific attribute specifications.  */
char regs_ever_allocated[FIRST_PSEUDO_REGISTER];

/*  Prototypes and external defs.  */

/* Prototypes generated using gcc's -aux-info flag. */
static rtx adjust_operand 			(rtx op, HOST_WIDE_INT *start);
static HOST_WIDE_INT const_double_to_hwint	(rtx);
static rtx frame_emit_store 			(int regno, rtx addr, HOST_WIDE_INT offset);
static rtx frame_emit_load 			(int regno, rtx addr, HOST_WIDE_INT offset);
static rtx frame_emit_add_imm 			(rtx dst, rtx src, HOST_WIDE_INT imm, rtx scratch);
static const char *fsmbi_for_array		(unsigned char arr[16]);
static const char *cxd_for_array		(unsigned char arr[16]);
static const char *move_int32_asm		(HOST_WIDE_INT);
static void pad_bb				(void);
static void emit_nop_for_insn			(basic_block bb, rtx insn);
static void spu_emit_branch_hint 		(rtx before, rtx branch, rtx target, int distance, int offset_of_branch);
static rtx get_branch_target 			(rtx branch);
static int split_int32 				(rtx to, HOST_WIDE_INT val, int force, int vec);
static int uses_ls_unit				(rtx);
static int get_pipe				(rtx);
int legitimate_const				(rtx);
static tree spu_handle_fndecl_attribute 	(tree *node, tree name, tree args, int flags, bool *no_add_attrs);
static tree spu_handle_vector_attribute 	(tree *node, tree name, tree args, int flags, bool *no_add_attrs);
static int spu_naked_function_p 		(tree func);
static int regno_aligned_for_load		(int regno);
static int aligned_mem				(rtx mem);
static int mem_is_padded_component_ref		(rtx x);
static void constant_to_array			(enum machine_mode mode, rtx x, unsigned char arr[16]);
static rtx array_to_constant			(enum machine_mode mode, unsigned char arr[16]);

extern const char *reg_names[];
rtx spu_compare_op0, spu_compare_op1;

const char *spu_branch_cost_string;
int spu_branch_cost = 20;

const char *spu_max_nops_str;

/* The hardware requires 8 insns between a hint and the branch it
   effects.  This variable describes how many rtl instructions the
   compiler needs to see before inserting a hint, and then the compiler
   will insert enough nops to make it at least 8 insns.  The default is
   for the compiler to allow up to 4 nops be emitted.  The nops are
   inserted in pairs, so we round down. */
int spu_hint_dist = (8*4) - (2*4);

/* The SPU language extensions spec documents how floating point should
 * behave.  It also allows for a fast-math option which can be set
 * indepently for floats and doubles.  We provide a compatibilty mode
 * for previous SDK users. */
const char *spu_float_acc_str;
int spu_float_acc = SPU_FP_FAST;
const char *spu_double_acc_str;
int spu_double_acc = SPU_FP_ACCURATE;

/* The ratio represents approximately how many instructions to use for
 * an inline memcpy.  Otherwise some form of memcpy will be called.  */
int spu_move_ratio = 32;

/*  TARGET overrides.  */

enum machine_mode spu_eh_return_filter_mode	PARAMS((void));
#undef TARGET_EH_RETURN_FILTER_MODE
#define TARGET_EH_RETURN_FILTER_MODE spu_eh_return_filter_mode

/* Enable altivec style vector initializers. */
#undef TARGET_CAST_EXPR_AS_VECTOR_INIT
#define TARGET_CAST_EXPR_AS_VECTOR_INIT true

/* The .8byte directive doesn't seem to work well for a 32 bit
 * architecture. */
#undef TARGET_ASM_UNALIGNED_DI_OP
#define TARGET_ASM_UNALIGNED_DI_OP NULL

void spu_init_builtins			PARAMS((void));
#undef TARGET_INIT_BUILTINS
#define TARGET_INIT_BUILTINS spu_init_builtins

tree spu_select_overloaded_builtin	PARAMS((tree, tree));
#undef TARGET_SELECT_OVERLOADED_BUILTIN
#define TARGET_SELECT_OVERLOADED_BUILTIN  spu_select_overloaded_builtin

rtx spu_expand_builtin	PARAMS((tree, rtx, rtx, enum machine_mode, int));
#undef TARGET_EXPAND_BUILTIN
#define TARGET_EXPAND_BUILTIN spu_expand_builtin

tree spu_expand_tree_builtin	PARAMS((tree function, tree params, tree coerced_params));
#undef TARGET_EXPAND_TREE_BUILTIN
#define TARGET_EXPAND_TREE_BUILTIN spu_expand_tree_builtin

static bool spu_rtx_costs (rtx x, int code, int outer_code, int *total);
#undef TARGET_RTX_COSTS
#define TARGET_RTX_COSTS spu_rtx_costs

#undef TARGET_ADDRESS_COST
#define TARGET_ADDRESS_COST hook_int_rtx_0

static int spu_sched_issue_rate	PARAMS((void));
#undef TARGET_SCHED_ISSUE_RATE
#define TARGET_SCHED_ISSUE_RATE spu_sched_issue_rate

static void spu_sched_init PARAMS((FILE *, int, int));
#undef TARGET_SCHED_INIT
#define TARGET_SCHED_INIT spu_sched_init

static void spu_sched_finish PARAMS((FILE *, int));
#undef TARGET_SCHED_FINISH
#define TARGET_SCHED_FINISH spu_sched_finish

static int spu_sched_variable_issue PARAMS((FILE *, int, rtx, int));
#undef TARGET_SCHED_VARIABLE_ISSUE
#define TARGET_SCHED_VARIABLE_ISSUE spu_sched_variable_issue

static int spu_sched_reorder PARAMS((FILE *, int, rtx *, int *, int));
#undef TARGET_SCHED_REORDER
#define TARGET_SCHED_REORDER spu_sched_reorder
#undef TARGET_SCHED_REORDER2
#define TARGET_SCHED_REORDER2 spu_sched_reorder

static int spu_sched_adjust_cost PARAMS((rtx, rtx, rtx, int));
#undef TARGET_SCHED_ADJUST_COST
#define TARGET_SCHED_ADJUST_COST spu_sched_adjust_cost

static int spu_sched_adjust_priority PARAMS((rtx, int));
#undef TARGET_SCHED_ADJUST_PRIORITY
#define TARGET_SCHED_ADJUST_PRIORITY spu_sched_adjust_priority

static int spu_sched_can_schedule PARAMS((rtx));
#undef TARGET_SCHED_CAN_SCHEDULE
#define TARGET_SCHED_CAN_SCHEDULE spu_sched_can_schedule

const struct attribute_spec spu_attribute_table[];
#undef  TARGET_ATTRIBUTE_TABLE
#define TARGET_ATTRIBUTE_TABLE spu_attribute_table

static bool spu_assemble_integer (rtx x, unsigned int size, int aligned_p);
#undef TARGET_ASM_INTEGER
#define TARGET_ASM_INTEGER spu_assemble_integer

const char *spu_strip_name_encoding PARAMS((const char *));
#undef TARGET_STRIP_NAME_ENCODING
#define TARGET_STRIP_NAME_ENCODING spu_strip_name_encoding

static bool spu_scalar_mode_supported_p PARAMS((enum machine_mode));
#undef TARGET_SCALAR_MODE_SUPPORTED_P
#define TARGET_SCALAR_MODE_SUPPORTED_P	spu_scalar_mode_supported_p

static bool spu_vector_mode_supported_p PARAMS((enum machine_mode));
#undef TARGET_VECTOR_MODE_SUPPORTED_P
#define TARGET_VECTOR_MODE_SUPPORTED_P	spu_vector_mode_supported_p

static bool spu_function_ok_for_sibcall (tree, tree);
#undef TARGET_FUNCTION_OK_FOR_SIBCALL
#define TARGET_FUNCTION_OK_FOR_SIBCALL spu_function_ok_for_sibcall

static void spu_asm_globalize_label PARAMS((FILE *, const char *));
#undef TARGET_ASM_GLOBALIZE_LABEL
#define TARGET_ASM_GLOBALIZE_LABEL spu_asm_globalize_label

static rtx spu_simplify_unspec PARAMS((rtx, rtx, rtx, rtx));
#undef TARGET_SIMPLIFY_UNSPEC
#define TARGET_SIMPLIFY_UNSPEC spu_simplify_unspec

static bool spu_cant_combine PARAMS((rtx, rtx, rtx));
#undef TARGET_CANT_COMBINE
#define TARGET_CANT_COMBINE spu_cant_combine

static bool spu_pass_by_reference (CUMULATIVE_ARGS *, enum machine_mode,
				  tree, bool);
#undef TARGET_PASS_BY_REFERENCE
#define TARGET_PASS_BY_REFERENCE spu_pass_by_reference

#undef TARGET_MUST_PASS_IN_STACK
#define TARGET_MUST_PASS_IN_STACK must_pass_in_stack_var_size

static tree spu_build_builtin_va_list (void);
#undef TARGET_BUILD_BUILTIN_VA_LIST
#define TARGET_BUILD_BUILTIN_VA_LIST spu_build_builtin_va_list

#undef TARGET_SETUP_INCOMING_VARARGS
#define TARGET_SETUP_INCOMING_VARARGS spu_setup_incoming_varargs

static void spu_machine_dependent_reorg (void);
#undef TARGET_MACHINE_DEPENDENT_REORG
#define TARGET_MACHINE_DEPENDENT_REORG spu_machine_dependent_reorg


static tree spu_gimplify_va_arg_expr PARAMS((tree, tree, tree*, tree*));
#undef TARGET_GIMPLIFY_VA_ARG_EXPR
#define TARGET_GIMPLIFY_VA_ARG_EXPR spu_gimplify_va_arg_expr

static void spu_init_libfuncs (void);
#undef TARGET_INIT_LIBFUNCS
#define TARGET_INIT_LIBFUNCS spu_init_libfuncs

static bool spu_cannot_copy_insn_p (rtx insn);
#undef TARGET_CANNOT_COPY_INSN_P
#define TARGET_CANNOT_COPY_INSN_P spu_cannot_copy_insn_p

struct gcc_target targetm = TARGET_INITIALIZER;

/* Sometimes certain combinations of command options do not make sense
   on a particular target machine.  You can define a macro
   OVERRIDE_OPTIONS to take account of this. This macro, if defined, is
   executed once just after all the command options have been parsed.  */
void
spu_override_options (void)
{

  /* Don't give warnings about the main() function. */
  warn_main = 0;

  /* Override some of the default param values.  With so many registers
     larger values are better for these params.  */
  if (MAX_UNROLLED_INSNS == 100)
    MAX_UNROLLED_INSNS = 250;
  if (MAX_PENDING_LIST_LENGTH == 32)
    MAX_PENDING_LIST_LENGTH = 128;

  flag_omit_frame_pointer = 1;
  /* if (align_labels < 8) align_labels = 8; */
  if (align_functions < 8) align_functions = 8;

  if (flag_pic && TARGET_LARGE_MEM)
    {
      flag_pic = 0;
      warning ("PIC disabled, it is not supported with -mlarge-mem\n");
    }

  if (TARGET_LARGE_MEM && TARGET_TEST_ABI)
    {
      error ("Cannot use -mtest-abi and -mlarge-mem together\n");
    }

  if (TARGET_REORDER_BLOCKS)
    flag_reorder_blocks = 0;

  if (spu_branch_cost_string)
    {
      spu_branch_cost = atoi(spu_branch_cost_string);
      warning("spu_branch_cost = %d\n", spu_branch_cost);
    }

  if (spu_max_nops_str)
    spu_hint_dist = 8*4 - (atoi(spu_max_nops_str) - 1) * 4;
  if (spu_hint_dist < 0) 
    spu_hint_dist = 0;

  if (spu_float_acc_str)
    {
      if (strcmp (&spu_float_acc_str[0], "compat") == 0)
	spu_float_acc = SPU_FP_COMPAT;
      else if (strcmp (&spu_float_acc_str[0], "accurate") == 0)
	spu_float_acc = SPU_FP_ACCURATE;
      else if (strcmp (&spu_float_acc_str[0], "fast") == 0)
	spu_float_acc = SPU_FP_FAST;
      else
	error ("unknown float mode \"%s\"", &spu_float_acc_str[0]);
    }
  if (spu_double_acc_str)
    {
      if (strcmp (&spu_double_acc_str[0], "compat") == 0)
	spu_double_acc = SPU_FP_COMPAT;
      else if (strcmp (&spu_double_acc_str[0], "accurate") == 0)
	spu_double_acc = SPU_FP_ACCURATE;
      else if (strcmp (&spu_double_acc_str[0], "fast") == 0)
	spu_double_acc = SPU_FP_FAST;
      else
	error ("unknown double mode \"%s\"", &spu_double_acc_str[0]);
    }
  REAL_MODE_FORMAT (SFmode) = spu_float_acc == SPU_FP_COMPAT ? &spu_extended_format_compat : &spu_extended_format;
  REAL_MODE_FORMAT (DFmode) = spu_double_acc == SPU_FP_COMPAT ? &spu_double_format_compat : &spu_double_format;
}


/* Handle an attribute requiring a FUNCTION_DECL; arguments as in
   struct attribute_spec.handler.  */

/*  Table of machine attributes.  */
const struct attribute_spec spu_attribute_table[] =
{
  /* { name, min_len, max_len, decl_req, type_req, fn_type_req, handler } */
  { "naked",          0, 0, true,  false, false, spu_handle_fndecl_attribute },
  { "spu_vector",     0, 0, false, true,  false, spu_handle_vector_attribute },
  { NULL,             0, 0, false, false, false, NULL }
};

/*  Return the pointer to the first char in NAME immediately following
    the dont_save_regs encoding prefix, if there's the prefix,
    otherwise return NAME.  */
const char *
spu_strip_name_encoding (const char * name)
{
  return default_strip_name_encoding (name);
}

/* Test for a valid operand for a call instruction.  */
int
call_operand (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  if (EXTRA_CONSTRAINT (op, 'S'))
    return !TARGET_LARGE_MEM;
  return (EXTRA_CONSTRAINT (op, 'T')
	  && REGNO (XEXP (op, 0)) != FRAME_POINTER_REGNUM
	  && REGNO (XEXP (op, 0)) != ARG_POINTER_REGNUM
	  && (REGNO (XEXP (op, 0)) < FIRST_PSEUDO_REGISTER
	      || REGNO (XEXP (op, 0)) > LAST_VIRTUAL_REGISTER));
}

/* Test for a valid operand for a sibcall instruction.  */
int
sibcall_operand (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  if (EXTRA_CONSTRAINT (op, 'S'))
    return !TARGET_LARGE_MEM;

  if (EXTRA_CONSTRAINT (op, 'T'))
    {
      int regno = REGNO (XEXP (op, 0));

      return (regno != FRAME_POINTER_REGNUM
	      && regno != ARG_POINTER_REGNUM
	      && (regno < FIRST_PSEUDO_REGISTER
		  || regno > LAST_VIRTUAL_REGISTER));
    }
  return 0;
}

/* True if MODE is valid for the target.  By "valid", we mean able to
   be manipulated in non-trivial ways.  In particular, this means all
   the arithmetic is supported.  */
static bool 
spu_scalar_mode_supported_p (enum machine_mode mode)
{
  switch (mode)
  {
  case QImode:
  case HImode:
  case SImode:
  case DImode:
  case TImode:
  case SFmode:
  case DFmode:
    return true;

  default:
    return false;
  }
}

/* Similarly for vector modes.  "Supported" here is less strict.  At
   least some operations are supported; need to check optabs or builtins
   for further details.  */
static bool
spu_vector_mode_supported_p (enum machine_mode mode)
{
  switch (mode)
  {
  case V16QImode:
  case V8HImode:
  case V4SImode:
  case V2DImode:
  case V4SFmode:
  case V2DFmode:
    return true;

  default:
    return false;
  }
}


/* Return nonzero if OPERAND is valid as a source operand for a move
   instruction.  */

int
move_operand (rtx op, enum machine_mode mode)
{
  /* Accept any general operand after reload has started; doing so
     avoids losing if reload does an in-place replacement of a register
     with a SYMBOL_REF or CONST.  */
  return general_operand (op, mode)
	 && GET_CODE(op) != HIGH
	 && (!TARGET_LARGE_MEM
	     || !(GET_CODE(op) == SYMBOL_REF
	          || GET_CODE(op) == LABEL_REF
		  || GET_CODE(op) == CONST));
}


int
immediate_operand_K (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  return (GET_CODE (op) == CONST_INT
	  && CONST_OK_FOR_LETTER_P(INTVAL(op),'K'));
}

int
rK_operand (rtx op, enum machine_mode mode)
{
  return register_operand(op, mode)
         || (GET_CODE (op) == CONST_INT && CONST_OK_FOR_LETTER_P(INTVAL(op),'K'));
}

int
rKN_operand (rtx op, enum machine_mode mode)
{
  return register_operand(op, mode)
         || (GET_CODE (op) == CONST_INT
	     && (CONST_OK_FOR_LETTER_P(INTVAL(op),'K')
	         || CONST_OK_FOR_LETTER_P(INTVAL(op),'N')));
}

/* Check for a const_vector:mode which has all elements the same. */
int
vec_imm_operand (rtx op, enum machine_mode mode, int range)
{
  int i, j, units, size, symbol;
  unsigned char arr[16];
  HOST_WIDE_INT val;
  enum machine_mode imode;

  if (GET_CODE(op) != CONST_VECTOR)
    return 0;

  units = CONST_VECTOR_NUNITS (op);

  if (mode != VOIDmode && units != GET_MODE_NUNITS (mode))
    return 0;

  mode = GET_MODE(op);

  symbol = 0;
  for (i = 0; i < units; i++)
    {
      rtx x = CONST_VECTOR_ELT (op, i);
      if (GET_CODE(x) == SYMBOL_REF || GET_CODE(x) == CONST)
	{
	  if (mode != V4SImode 
	      || (i > 0 && x != CONST_VECTOR_ELT (op, 0)))
	    return 0;
	  symbol++;
	}
      else if (GET_CODE(x) != CONST_INT && GET_CODE(x) != CONST_DOUBLE)
	return 0;
    }

  imode = mode_for_size(GET_MODE_BITSIZE(GET_MODE_INNER(mode)), MODE_INT, 0);

  if (symbol)
    return symbol == units && mode == V4SImode;

  constant_to_array(mode, op, arr);
  val = (arr[0] << 24) | (arr[1] << 16) | (arr[2] << 8) | (arr[3]);
  val = trunc_int_for_mode (val, imode);

  if (range == 0)
    {
      if (fsmbi_for_array(arr))
	return 1;
      if (cxd_for_array(arr))
	return 1;
    }

  /* Check that every element is the same.  We treat move insns
   * (range == 0) and mpy insns (range < 0) like they are V4SImode. */
  size = range > 0 ? GET_MODE_SIZE(imode) : 4;
  units = range > 0 ? units : 4;
  for (i = 0; i < size; i++)
    for (j = 1; j < units; j++)
      if (arr[i] != arr[size*j+i])
	return 0;

  /* Only mpy insns need this special treatment because we want to
   * ignore the even half words. */
  if (range < 0)
    range = -range;

  /* We test the range as part of the predicate because we don't have
   * enough letters available to use as a constraint. */
  if (range && !CONST_OK_FOR_LETTER_P(val, range))
    return 0;

  return 1;
}

/* Test for a valid operand for some vector instructions.  */
int
vec_regimm_operand (rtx op, enum machine_mode mode)
{
  if (register_operand(op, mode))
    return 1;
  /* This happens for spu_and, spu_or and spu_xor. */
  if (mode == VOIDmode && GET_MODE (op) == V2DImode)
    return 0;
  if (GET_MODE_CLASS(GET_MODE(op)) != MODE_VECTOR_FLOAT
      && vec_imm_operand(op,mode,'K'))
    {
      HOST_WIDE_INT val = INTVAL(CONST_VECTOR_ELT (op, 0));
      if (val >= -0x200 && val <= 0x1ff)
	return 1;
    }
  return 0;
}

int
vec_iohl_operand (rtx op, enum machine_mode mode)
{
  if (vec_imm_operand(op,mode,'N'))
    return 1;
  return 0;
}

int
vec_mpy_operand (rtx op, enum machine_mode mode)
{
  if (register_operand(op, mode))
    return 1;
  if (vec_imm_operand(op,mode,-'K'))
    return 1;
  return 0;
}

/* Test for a valid operand for a vector move instruction.  */
int
vector_operand (rtx op, enum machine_mode mode)
{
  return memory_operand(op, mode) || register_operand(op, mode)
         || GET_CODE (op) == CONST_VECTOR || vec_imm_operand(op,mode,0);
}

int
const_zero_operand(rtx op, enum machine_mode mode)
{
  return op == CONST0_RTX(mode);
}

int
const_one_operand (rtx op, enum machine_mode mode)
{
  return op == CONST1_RTX(mode);
}

/* Return 1 if OP is a comparison operation that is valid for a branch insn.
   We only check the opcode against the mode of the register value here.  */

int
branch_comparison_operator (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  enum rtx_code code = GET_CODE (op);
  enum machine_mode cc_mode;

  if (code != EQ && code != NE)
    return 0;

  cc_mode = GET_MODE (XEXP (op, 0));
  if (cc_mode != HImode && cc_mode != SImode)
    return 0;

  return 1;
}


static rtx
adjust_operand(rtx op, HOST_WIDE_INT *start)
{
  enum machine_mode mode;
  int op_size;
  if (GET_CODE(op) == SUBREG)
    {
      op = SUBREG_REG(op);
      if (start)
        *start -= 128 - GET_MODE_BITSIZE(GET_MODE(op));
    }
  op_size = GET_MODE_BITSIZE(GET_MODE(op));
  if (op_size < 32)
    {
      if (start)
        *start += 32 - op_size;
      op_size = 32;
    }
  mode = mode_for_size(op_size, MODE_INT, 0);
  if (mode != GET_MODE(op))
    op = gen_rtx_SUBREG(mode, op, 0);
  return op;
}

/* structure extract */
void
spu_extract (rtx ops[], int unsignedp)
{
    HOST_WIDE_INT width = INTVAL(ops[2]);
    HOST_WIDE_INT start = INTVAL(ops[3]);
    HOST_WIDE_INT src_size, dst_size;
    enum machine_mode src_mode, dst_mode;
    rtx dst, src;
    rtx s;

    dst = adjust_operand(ops[0], 0);
    src = adjust_operand(ops[1], &start);
    src_mode = GET_MODE(src);
    src_size = GET_MODE_BITSIZE(GET_MODE(src));
    dst_mode = GET_MODE(dst);
    dst_size = GET_MODE_BITSIZE(GET_MODE(dst));

    if (start > 0)
      {
        emit_insn(gen_rtx_SET(VOIDmode,
                              s = gen_reg_rtx(src_mode),
                              gen_rtx_ASHIFT(src_mode,
                                             src,
                                             GEN_INT(start))));
        src = s;
      }

    if (width < src_size)
      {
	rtx pat;
	int icode;
	switch (src_mode)
	{
	case SImode: icode = unsignedp ? CODE_FOR_lshrsi3 : CODE_FOR_ashrsi3; break;
	case DImode: icode = unsignedp ? CODE_FOR_lshrdi3 : CODE_FOR_ashrdi3; break;
	case TImode: icode = unsignedp ? CODE_FOR_lshrti3 : CODE_FOR_ashrti3; break;
	default: abort();
	}
	s = gen_reg_rtx(src_mode);
        pat = GEN_FCN (icode) (s, src, GEN_INT(src_size-width));
	emit_insn(pat);
        src = s;
      }

    convert_move(dst, src, unsignedp);
    return;

/*
extracterr:
    printf("extract %s %d\n", cfun->emit->x_last_filename, cfun->emit->x_last_linenum);
    debug_rtx(ops[0]);
    debug_rtx(ops[1]);
    debug_rtx(ops[2]);
    debug_rtx(ops[3]);
    abort();
*/
}

void
spu_insert (rtx ops[])
{
    HOST_WIDE_INT width = INTVAL(ops[1]);
    HOST_WIDE_INT start = INTVAL(ops[2]);
    HOST_WIDE_INT maskbits;
    enum machine_mode dst_mode, src_mode;
    rtx dst, src;
    int dst_size, src_size;

    dst = adjust_operand(ops[0], &start);
    dst_mode = GET_MODE(dst);
    dst_size = GET_MODE_BITSIZE(GET_MODE(dst));

    src = ops[3];
    if (CONSTANT_P(src))
    {
        enum machine_mode m = (width <= 32 ? SImode : DImode);
        src = force_reg(m, convert_to_mode (m, src, 0));
    }
    src = adjust_operand(src, 0);
    src_mode = GET_MODE(src);
    src_size = GET_MODE_BITSIZE(GET_MODE(src));

#if 1
    /* If width and start are a multiple of 8 then use shufb. */
    if ((width & 7) == 0 && (start & 7) == 0)
    {
        rtx pattern = gen_reg_rtx(V16QImode);
	unsigned char arr[16];
	int src_off = (src_size - width) / 8
	            + (src_size < 32 ? (32 - src_size) / 8 : 0);
	int dst_off = start / 8
	            + (dst_size < 32 ? (32 - dst_size) / 8 : 0);
	int i;
	for (i = 0; i < 16; i++)
	  arr[i] = 16 + i;
	for (i = 0; i < width/8; i++)
	  arr[i+dst_off] = i + src_off;
	emit_move_insn(pattern, array_to_constant(V16QImode, arr));
        spu_emit_insn(gen_spu_shufb(dst, src, dst, pattern));
    }
    else
#endif
    {
        rtx mask;
        rtx shift_reg;
        int shift;

        mask = gen_reg_rtx(dst_mode);
        shift_reg = gen_reg_rtx(dst_mode);
        shift = dst_size - start - width ;

        /* It's not safe to use subreg here because the compiler assumes
           that the SUBREG_REG is right justified in the SUBREG. */
        convert_move(shift_reg, src, 1);

        if (shift > 0)
            emit_insn(gen_rtx_SET(VOIDmode,
                                  shift_reg,
                                  gen_rtx_ASHIFT(dst_mode,
                                                 shift_reg,
                                                 GEN_INT(shift))));
        else if (shift < 0)
            goto inserterr;

        switch (dst_size)
        {
        case 32:
	    maskbits = (-1ll << (32-width-start));
	    if (start) maskbits += (1ll << (32-start));
	    emit_move_insn(mask, GEN_INT(maskbits));
	    spu_emit_insn(gen_spu_selb(dst, dst, shift_reg, mask));
            break;
        case 64:
	    maskbits = (-1ll << (64-width-start));
	    if (start) maskbits += (1ll << (64-start));
	    emit_move_insn(mask, GEN_INT(maskbits));
	    spu_emit_insn(gen_spu_selb(dst, dst, shift_reg, mask));
            break;
        case 128:
	    {
	      unsigned char arr[16];
	      int i = start/8;
	      memset(arr, 0, sizeof(arr));
	      if (i == (start+width-1) / 8)
		{
		  arr[i] = 0xff >> (start & 7);
		  arr[i] &= 0xff << (7-((start+width-1) & 7));
		}
	      else
		{
		  arr[i] = 0xff >> (start & 7);
		  for (i++; i < (start+width-1)/8; i++)
		    arr[i] = 0xff;
		  arr[i] = 0xff << (7-((start+width-1) & 7));
		}
	      /* GCC doesn't handle TImode CONST well because CONST is
	       * only 64 bits.. */
	      emit_move_insn(gen_rtx_SUBREG(V16QImode, mask, 0),
			     array_to_constant(V16QImode, arr));
	      spu_emit_insn(gen_spu_selb(dst, dst, shift_reg, mask));
	    }
            break;
        default:
            goto inserterr;
        }
    }
    return;
inserterr:
    abort();
}

int
spu_expand_block_move(rtx ops[])
{
  HOST_WIDE_INT bytes, align, offset;
  rtx src, dst, sreg, dreg, target;
  int i;
  if (GET_CODE (ops[2]) != CONST_INT
      || GET_CODE (ops[3]) != CONST_INT
      || INTVAL (ops[2]) > (HOST_WIDE_INT)(MOVE_RATIO * 8))
    return 0;

  bytes = INTVAL(ops[2]);
  align = INTVAL(ops[3]);

  if (bytes <= 0)
    return 1;

  dst = ops[0];
  src = ops[1];

  if (align == 16)
    {
      for (offset = 0; offset + 16 <= bytes; offset += 16)
	{
	  dst = adjust_address (ops[0], V16QImode, offset);
	  src = adjust_address (ops[1], V16QImode, offset);
	  emit_move_insn(dst, src);
	}
      if (offset < bytes)
	{
	  rtx mask;
	  unsigned char arr[16] = {0};
	  for (i = 0; i < bytes - offset; i++)
	    arr[i] = 0xff;
	  dst = adjust_address (ops[0], V16QImode, offset);
	  src = adjust_address (ops[1], V16QImode, offset);
	  mask = gen_reg_rtx(V16QImode);
	  sreg = gen_reg_rtx(V16QImode);
	  dreg = gen_reg_rtx(V16QImode);
	  target = gen_reg_rtx(V16QImode);
	  emit_move_insn(mask, array_to_constant(V16QImode, arr));
	  emit_move_insn(dreg, dst);
	  emit_move_insn(sreg, src);
	  spu_emit_insn(gen_spu_selb(target, dreg, sreg, mask));
	  emit_move_insn(dst, target);
	}
      return 1;
    }

  return 0;
}

enum spu_comp_code { SPU_EQ, SPU_GT, SPU_GTU };


int spu_comp_icode[8][3] = {
 {CODE_FOR_ceq_qi, CODE_FOR_cgt_qi, CODE_FOR_clgt_qi},
 {CODE_FOR_ceq_hi, CODE_FOR_cgt_hi, CODE_FOR_clgt_hi},
 {CODE_FOR_ceq_si, CODE_FOR_cgt_si, CODE_FOR_clgt_si},
 {CODE_FOR_ceq_di, CODE_FOR_cgt_di, CODE_FOR_clgt_di},
 {CODE_FOR_ceq_ti, 0, 0},
 {CODE_FOR_ceq_sf, CODE_FOR_cgt_sf, 0},
 {CODE_FOR_ceq_df, CODE_FOR_cgt_df, 0},
 {CODE_FOR_ceq_vec, 0, 0},
};

/* Generate a compare for CODE.  Return a brand-new rtx that represents
   the result of the compare.   GCC can figure this out too if we don't
   provide all variations of compares, but GCC always wants to use
   WORD_MODE, we can generate better code in most cases if we do it
   ourselves.  */
void
spu_emit_branch_or_set (int is_set, enum rtx_code code, rtx operands[])
{
  int reverse_compare = 0;
  int reverse_test = 0;
  rtx compare_result;
  rtx comp_rtx;
  rtx target = operands[0];
  enum machine_mode comp_mode;
  enum machine_mode op_mode;
  enum spu_comp_code scode;
  int index;

  /* When spu_compare_op1 is a CONST_INT change (X >= C) to (X > C-1),
   * and so on, to keep the constant in operand 1.*/
  if (GET_CODE(spu_compare_op1) == CONST_INT)
    {
      HOST_WIDE_INT val = INTVAL (spu_compare_op1) - 1;
      if (trunc_int_for_mode(val, GET_MODE(spu_compare_op0)) == val)
        switch (code)
        {
        case GE:  spu_compare_op1 = GEN_INT(val); code = GT; break;
        case LT:  spu_compare_op1 = GEN_INT(val); code = LE; break;
        case GEU: spu_compare_op1 = GEN_INT(val); code = GTU; break;
        case LTU: spu_compare_op1 = GEN_INT(val); code = LEU; break;
	default: break;
        }
    }

  switch (code)
  {
  case GE:  reverse_compare = 1; reverse_test = 1; scode = SPU_GT; break;
  case LE:  reverse_compare = 0; reverse_test = 1; scode = SPU_GT; break;
  case LT:  reverse_compare = 1; reverse_test = 0; scode = SPU_GT; break;
  case GEU: reverse_compare = 1; reverse_test = 1; scode = SPU_GTU; break;
  case LEU: reverse_compare = 0; reverse_test = 1; scode = SPU_GTU; break;
  case LTU: reverse_compare = 1; reverse_test = 0; scode = SPU_GTU; break;
  case NE:  reverse_compare = 0; reverse_test = 1; scode = SPU_EQ; break;

  case EQ:  scode = SPU_EQ; break;
  case GT:  scode = SPU_GT; break;
  case GTU: scode = SPU_GTU; break;
  default:  scode = SPU_EQ; break;
  }

  comp_mode = SImode;
  op_mode = GET_MODE(spu_compare_op0);

  switch (op_mode)
  {
  case QImode: index = 0; comp_mode = QImode; break;
  case HImode: index = 1; comp_mode = HImode; break;
  case SImode: index = 2; break;
  case DImode: index = 3; break;
  case TImode: index = 4; break;
  case SFmode: index = 5; break;
  case DFmode: index = 6; break;
  case V16QImode:
  case V8HImode:
  case V4SImode:
  case V2DImode:
  case V4SFmode: 
  case V2DFmode: index = 7; break;
  default: abort();
  }

  if (GET_MODE(spu_compare_op1) == DFmode)
    {
      rtx reg = gen_reg_rtx(DFmode);
      if (!flag_unsafe_math_optimizations
          || (scode != SPU_GT && scode != SPU_EQ))
	abort();
      if (reverse_compare)
	emit_insn(gen_subdf3(reg, spu_compare_op1, spu_compare_op0));
      else
	emit_insn(gen_subdf3(reg, spu_compare_op0, spu_compare_op1));
      reverse_compare = 0;
      spu_compare_op0 = reg;
      spu_compare_op1 = CONST0_RTX(DFmode);
    }

  if (is_set == 0 && spu_compare_op1 == const0_rtx
      && (GET_MODE(spu_compare_op0) == SImode
          || GET_MODE(spu_compare_op0) == HImode)
      && scode == SPU_EQ)
    {
      /* Don't need to set a register with the result when we are 
       * comparing against zero and branching. */
      reverse_test = !reverse_test;
      compare_result = spu_compare_op0;
    }
  else
    {
      compare_result = gen_reg_rtx (comp_mode);

      if (reverse_compare)
      {
	rtx t = spu_compare_op1;
	spu_compare_op1 = spu_compare_op0;
	spu_compare_op0 = t;
      }

      if (spu_comp_icode[index][scode] == 0)
	abort();

      if (! (*insn_data[spu_comp_icode[index][scode]].operand[1].predicate) (spu_compare_op0, op_mode))
	spu_compare_op0 = force_reg(op_mode, spu_compare_op0);
      if (! (*insn_data[spu_comp_icode[index][scode]].operand[2].predicate) (spu_compare_op1, op_mode))
	spu_compare_op1 = force_reg(op_mode, spu_compare_op1);
      comp_rtx = GEN_FCN (spu_comp_icode[index][scode]) (compare_result,
	                                                 spu_compare_op0,
							 spu_compare_op1);
      if (comp_rtx == 0)
	abort();
      emit_insn (comp_rtx);

    }

  if (is_set == 0)
    {
      rtx bcomp;
      rtx loc_ref;

      /* We don't have branch on QI compare insns, so we convert the
       * QI compare result to a HI result. */
      if (comp_mode == QImode)
	{
	  rtx old_res = compare_result;
	  compare_result = gen_reg_rtx(HImode);
	  comp_mode = HImode;
	  emit_insn(gen_extendqihi2(compare_result,old_res));
	}

      if (reverse_test)
          bcomp = gen_rtx_EQ(comp_mode, compare_result, const0_rtx);
      else
          bcomp = gen_rtx_NE(comp_mode, compare_result, const0_rtx);

      loc_ref = gen_rtx_LABEL_REF (VOIDmode, target);
      emit_jump_insn (gen_rtx_SET (VOIDmode, pc_rtx,
                                   gen_rtx_IF_THEN_ELSE (VOIDmode, bcomp,
                                                         loc_ref, pc_rtx)));
    }
  else if (is_set == 2)
    {
      int compare_size = GET_MODE_BITSIZE(comp_mode);
      int target_size = GET_MODE_BITSIZE(GET_MODE(target));
      enum machine_mode mode = mode_for_size(target_size, MODE_INT, 0);
      rtx select_mask;
      rtx op_t = operands[2];
      rtx op_f = operands[3];

      /* The result of the comparison can be SI, HI or QI mode.  Create a
       * mask based on that result. */
      if (target_size > compare_size)
	{
	  select_mask = gen_reg_rtx(mode);
	  spu_emit_insn(gen_extend_compare(select_mask, compare_result));
	}
      else if (target_size < compare_size)
	select_mask = gen_rtx_SUBREG(mode, compare_result, (compare_size - target_size)/BITS_PER_UNIT);
      else if (comp_mode != mode)
	select_mask = gen_rtx_SUBREG(mode, compare_result, 0);
      else
	select_mask = compare_result;

      if (GET_MODE(target) != GET_MODE(op_t)
          || GET_MODE(target) != GET_MODE(op_f)) 
	abort();

      if (reverse_test)
	spu_emit_insn (gen_spu_selb(target, op_t, op_f, select_mask));
      else
	spu_emit_insn (gen_spu_selb(target, op_f, op_t, select_mask));
    }
  else 
    {
      if (reverse_test)
	emit_insn (gen_rtx_SET (VOIDmode, compare_result,
			       gen_rtx_NOT (comp_mode, compare_result)));
      if (GET_MODE(target) == SImode && GET_MODE(compare_result) == HImode)
	emit_insn(gen_extendhisi2(target,compare_result));
      else if (GET_MODE(target) == SImode && GET_MODE(compare_result) == QImode)
	spu_emit_insn(gen_extend_compare(target, compare_result));
      else
	emit_move_insn(target, compare_result);
    }
      
}

static HOST_WIDE_INT
const_double_to_hwint(rtx x)
{
  HOST_WIDE_INT val;
  REAL_VALUE_TYPE rv;
  if (GET_MODE(x) == SFmode)
    {
      REAL_VALUE_FROM_CONST_DOUBLE (rv, x);
      REAL_VALUE_TO_TARGET_SINGLE (rv, val);
    }
  else
    {
      long l[2];
      REAL_VALUE_FROM_CONST_DOUBLE (rv, x);
      REAL_VALUE_TO_TARGET_DOUBLE (rv, l);
      val = l[0];
      val = (val << 32) | (l[1] & 0xffffffff);
    }
  return val;
}

void
print_operand_address (FILE *file, register rtx addr)
{
  rtx reg;
  rtx offset;

  switch (GET_CODE (addr))
    {
    case REG:
      fprintf (file, "0(%s)", reg_names[REGNO (addr)]);
      break;

    case PLUS:
      reg = XEXP (addr, 0);
      offset = XEXP (addr, 1);
      if (GET_CODE (offset) == REG)
	{
          fprintf (file, "%s,%s", reg_names[REGNO (reg)], reg_names[REGNO (offset)]);
	}
      else if (GET_CODE (offset) == CONST_INT)
        {
	  fprintf (file, HOST_WIDE_INT_PRINT_DEC "(%s)",
             INTVAL (offset), reg_names[ REGNO (reg) ]);
	}
      else
	abort();
      break;

    case CONST:
    case LABEL_REF:
    case SYMBOL_REF:
    case CONST_INT:
      output_addr_const (file, addr);
      break;

    default:
      debug_rtx(addr);
      abort();
    }
}

void
print_operand(FILE *file, rtx x, int code)
{
  if (code == 'f')
    {
      if (GET_CODE(x) == MEM)
	x = XEXP(x, 0);

      if (GET_CODE(x) == REG)
	fprintf (file, "d");
      else if (GET_CODE(x) == PLUS)
	{
	  if (GET_CODE(XEXP(x, 1)) == REG)
	    fprintf (file, "x");
	  else
	    fprintf (file, "d");
	}
      else 
	fprintf (file, flag_pic && GET_CODE (x) != CONST_INT ? "r" : "a");
      return;
    }
  if (GET_CODE (x) == REG)
    {
      if (code == 'b')
	fprintf (file, "%s", GET_MODE (x) == HImode ? "h" : "");
      else if (code != 'I')
	fprintf (file, "%s", reg_names[REGNO (x)]);
    }
  else if (COMPARISON_P (x))
    {
      if (code == 'b')
	fprintf (file, "%s", GET_CODE (x) == NE ? "n" : "");
    }
  else if (GET_CODE (x) == CONST_VECTOR)
    {
      if (code == 'I')
	{
	  if (GET_MODE (x) == V16QImode)
	    fprintf (file, "bi");
	  else if (GET_MODE (x) == V8HImode)
	    fprintf (file, "hi");
	  else
	    fprintf (file, "i");
	}
      else if (code == 'M' || code == 'L' || code == 'H')
	{
	  /* Vector move insns have one of these three codes or
	   * 'F' or 'C'. */
	  HOST_WIDE_INT val;
	  unsigned char arr[16];
	  constant_to_array (GET_MODE (x), x, arr);
	  val = (arr[0] << 24) | (arr[1] << 16) | (arr[2] << 8) | (arr[3]);
	  val = trunc_int_for_mode (val, SImode);
	  print_operand (file, GEN_INT (val), code);
	}
      else if (code == 'F')
	{			/* immediate operand for fsmbi */
	  int i;
	  HOST_WIDE_INT val = 0;
	  unsigned char arr[16];
	  constant_to_array (GET_MODE (x), x, arr);
	  for (i = 0; i < 16; i++)
	    {
	      val <<= 1;
	      val |= arr[i] & 1;
	    }
	  print_operand (file, GEN_INT (val), 0);
	}
      else if (code == 'C')
	{			/* immediate operand for c*d */
	  HOST_WIDE_INT val = 0;
	  unsigned char arr[16];
	  constant_to_array (GET_MODE (x), x, arr);
	  for (val = 0; val < 16; val++)
	    if (arr[val] < 16)
	      break;
	  print_operand (file, GEN_INT (val), 0);
	}
      else if (code == 'O')
	{
	  HOST_WIDE_INT val;
	  unsigned char arr[16];
	  constant_to_array (GET_MODE (x), x, arr);
	  val = (arr[2] << 8) | (arr[3]);
	  val = trunc_int_for_mode (val, HImode);
	  print_operand (file, GEN_INT (val), 0);
	}
      else if (code == 'S')
	{			/* Mask the immediate for shift left insns. */
	  HOST_WIDE_INT val = INTVAL (CONST_VECTOR_ELT (x, 0));
	  print_operand (file, GEN_INT (val & 127), 0);
	}
      else if (code == 'R')
	{			/* Mask the immediate for shift right insns. */
	  HOST_WIDE_INT val = INTVAL (CONST_VECTOR_ELT (x, 0));
	  if (GET_MODE (x) == V4SImode)
	    val = val ? val | -64 : val;
	  else if (GET_MODE (x) == V8HImode)
	    val = val ? val | -32 : val;
	  print_operand (file, GEN_INT (val), 0);
	}
      else
	{
	  rtx elt;
	  int units;
	  units = CONST_VECTOR_NUNITS (x);
	  elt = CONST_VECTOR_ELT (x, 0);
	  print_operand (file, elt, code);
	}
    }
  else if (GET_CODE (x) == CONST_INT || GET_CODE (x) == CONST_DOUBLE)
    {
      HOST_WIDE_INT val;
      if (GET_CODE (x) == CONST_INT)
	val = INTVAL (x);
      else if (GET_MODE (x) == VOIDmode)
	{
	  unsigned char arr[16];
	  constant_to_array (TImode, x, arr);
	  val = (arr[0] << 24) | (arr[1] << 16) | (arr[2] << 8) | arr[3];
	  val = trunc_int_for_mode (val, SImode);
	}
      else
	val = const_double_to_hwint (x);
      switch (code)
	{
	case 's':
	  /* number of bits to shift the sign register when shifting
	   * right a TImode operand by 'val' bits. */
	  val = 128 - val;
	  fprintf (file, HOST_WIDE_INT_PRINT_DEC, (val & 0x7));
	  return;
	case 'S':
	  /* number of bytes to shift the sign register when shifting
	   * right a TImode operand by 'val' bits. */
	  val = 128 - val;
	  fprintf (file, HOST_WIDE_INT_PRINT_DEC, ((val >> 3) & 0x1f));
	  return;

	case 'r':
	  /* number of bits to rotate when rotating right a TImode
	     operand by 'val' bits. */
	  val = -val;
	  fprintf (file, HOST_WIDE_INT_PRINT_DEC, (val << 61) >> 61);
	  return;
	case 'R':
	  /* number of bytes to rotate when rotating right a TImode
	     operand by 'val' bits. */
	  val = -val + 7;
	  fprintf (file, HOST_WIDE_INT_PRINT_DEC, ((val << 56) >> 59));
	  return;

	case 'b':
	  /* number of bits to rotate when rotating a TImode operand
	     by 'val' bits. */
	  fprintf (file, HOST_WIDE_INT_PRINT_DEC, (val & 0x7));
	  return;
	case 'B':
	  /* number of bytes to rotate when rotating a TImode operand
	     by 'val' bits. */
	  fprintf (file, HOST_WIDE_INT_PRINT_DEC, ((val >> 3) & 0x1f));
	  return;

	  /* Loading a 32 bit immediate: ilhu $0,H ; iohl $0,L */
	case 'H':
	  fprintf (file, HOST_WIDE_INT_PRINT_DEC, (((val << 32) >> 48)));
	  return;
	case 'L':
	  fprintf (file, HOST_WIDE_INT_PRINT_DEC, ((val) & 0xffff));
	  return;

	case 'Q':
	  fprintf (file, HOST_WIDE_INT_PRINT_DEC,
		   ((val << 8) | ((val) & 0xff)));
	  return;

	default:
	  if (GET_CODE (x) == CONST_DOUBLE)
	    {
	      fprintf (file, HOST_WIDE_INT_PRINT_DEC, val);
	      return;
	    }
	  break;
	}
	output_addr_const (file, x);
    }
  else if (GET_CODE (x) == MEM)
    {
      if (code == 'i' && GET_CODE (XEXP (x, 0)) == REG)
	/* Used in indirect function calls. */
	fprintf (file, "%s", reg_names[REGNO (XEXP(x, 0))]);
      else
	output_address (XEXP (x, 0));
    }
  else if (CONSTANT_P (x))
    {
      output_addr_const (file, x);
      if (code == 'H')
	fprintf (file, "@h");
      else if (code == 'L')
	fprintf (file, "@l");
    }
  else if (GET_CODE(x) == CODE_LABEL)
    output_addr_const (file, x);
  else
    abort();
}

int
store_operand (register rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  rtx addr;
  if (GET_CODE(op) != MEM) return 0;
  addr = XEXP(op, 0);
  if (GET_CODE (addr) == REG) return 1;
  if (GET_CODE (addr) == PLUS)
    {
      /* Handle [index]<address> represented with index-sum outermost */
      if (GET_CODE (XEXP (addr, 0)) == REG
	  && INT_REG_OK_FOR_BASE_P (XEXP (addr, 0), 0)
	  && GET_CODE (XEXP (addr, 1)) == CONST_INT)
	return 1;
      if (GET_CODE (XEXP (addr, 1)) == REG
	  && INT_REG_OK_FOR_BASE_P (XEXP (addr, 1), 0)
	  && GET_CODE (XEXP (addr, 0)) == CONST_INT)
	return 1;
    }
  return 0;
}

extern char call_used_regs[];
extern char regs_ever_live[];

/* For PIC mode we've reserved PIC_OFFSET_TABLE_REGNUM, which is a
   caller saved register.  For leaf functions it is more efficient to
   use a volatile register because we won't need to save and restore the
   pic register.  This routine is only valid after register allocation
   is completed, so we can pick an unused register.  */
static rtx
get_pic_reg (void)
{
  rtx pic_reg = pic_offset_table_rtx;
  if (!reload_completed)
    abort();
  if (current_function_is_leaf && !regs_ever_live[LAST_ARG_REGNUM])
    pic_reg = gen_rtx_REG(SImode, LAST_ARG_REGNUM);
  return pic_reg;
}

/* SAVING is TRUE when we are generating the actual load and store
 * instructions for REGNO.  When determining the size of the stack
 * needed for saving register we must allocate enough space for the
 * worst case, because we don't always have the information early enough
 * to not allocate it.  But we can at least eliminate the actual loads
 * and stores during the prologue/epilogue.  */
static int
need_to_save_reg (int regno, int saving)
{
  if (regs_ever_live[regno] && ! call_used_regs[regno])
    return 1;
  if (flag_pic
      && regno == PIC_OFFSET_TABLE_REGNUM 
      && (!saving || current_function_uses_pic_offset_table)
      && (!saving
	  || !current_function_is_leaf
	  || regs_ever_live[LAST_ARG_REGNUM]))
    return 1;
  return 0;
}

/* This function is only correct starting with local register
 * allocation */
int
spu_saved_regs_size (void)
{
  int reg_save_size = 0;
  int regno;

  for (regno = FIRST_PSEUDO_REGISTER - 1; regno >= 0; --regno)
    if (need_to_save_reg (regno, 0))
      reg_save_size += 0x10;
  return reg_save_size;
}

static rtx
frame_emit_store(int regno, rtx addr, HOST_WIDE_INT offset)
{
  rtx reg = gen_rtx_REG(V4SImode, regno);
  rtx mem = gen_rtx_MEM (V4SImode, gen_rtx_PLUS(Pmode, addr, GEN_INT(offset)));
  set_mem_alias_set(mem, get_frame_alias_set());
  return emit_insn(gen_movv4si(mem, reg));
}

static rtx
frame_emit_load(int regno, rtx addr, HOST_WIDE_INT offset)
{
  rtx reg = gen_rtx_REG(V4SImode, regno);
  rtx mem = gen_rtx_MEM (V4SImode, gen_rtx_PLUS(Pmode, addr, GEN_INT(offset)));
  set_mem_alias_set(mem, get_frame_alias_set());
  return emit_insn(gen_movv4si(reg, mem));
}

/* This happens after reload, so we need to expand it.  */
static rtx 
frame_emit_add_imm(rtx dst, rtx src, HOST_WIDE_INT imm, rtx scratch)
{
  rtx insn;
  if ( CONST_OK_FOR_LETTER_P(imm, 'K') )
    {
      insn = emit_insn (gen_addsi3 (dst, src, GEN_INT (imm)));
    }
  else 
    {
      insn = emit_insn (gen_movsi (scratch, gen_int_mode (imm, SImode)));
      REG_NOTES(insn) = gen_rtx_EXPR_LIST (REG_MAYBE_DEAD, const0_rtx,
					   REG_NOTES (insn));
      insn = emit_insn (gen_addsi3 (dst, src, scratch));
      if (REGNO(src) == REGNO(scratch))
	abort();
    }
  if (REGNO(dst) == REGNO(scratch))
    REG_NOTES(insn) = gen_rtx_EXPR_LIST (REG_MAYBE_DEAD, const0_rtx,
					 REG_NOTES (insn));
  return insn;
}

/* Return nonzero if this function is known to have a null epilogue.  */

int
direct_return (void)
{
  if (reload_completed)
    {
      if (cfun->static_chain_decl == 0
	  && (spu_saved_regs_size ()
	      + get_frame_size()
	      + current_function_outgoing_args_size
	      + current_function_pretend_args_size == 0)
          &&  current_function_is_leaf)
	return 1;
    }

  return 0;
}

/* Only call this from spu_expand_prologue and spu_expand_epilogue.
   Those functions need 1 or 2 scratch registers for various reasons.
   Pick them here.  The first call, when PREV == 0, will always be one
   of SCRATCH_REG_0 or LINK_REGISTER_REGNUM.  The second one can be
   anything or NULL. */
static rtx
get_scratch_reg (rtx prev)
{
  int regno = prev ? (int)REGNO (prev) : -1;

  switch (regno)
    {
    case -1:
      if (cfun->static_chain_decl == NULL)
	return gen_rtx_REG (Pmode, SCRATCH_REG_0);

    case SCRATCH_REG_0:
      if ((!current_function_is_leaf || cfun->static_chain_decl != NULL)
	  && prev == 0)
	return gen_rtx_REG (Pmode, LINK_REGISTER_REGNUM);

    case LINK_REGISTER_REGNUM:
      /* For stdarg the register has already been saved. */
      if ((current_function_stdarg || current_function_args_info < MAX_REGISTER_ARGS)
	  && !(current_function_is_leaf && current_function_uses_pic_offset_table))
	return gen_rtx_REG (Pmode, LAST_ARG_REGNUM);

    case LAST_ARG_REGNUM:
      /* For stdarg the register has already been saved. */
      if (current_function_stdarg || current_function_args_info < MAX_REGISTER_ARGS-1)
	return gen_rtx_REG (Pmode, LAST_ARG_REGNUM-1);

    case LAST_ARG_REGNUM-1:
      if (regs_ever_live[LAST_ARG_REGNUM+1])
	return gen_rtx_REG (Pmode, LAST_ARG_REGNUM+1);
    }
  return 0;
}

/*
   According to the ABI it should be like this:
         +-------------+
         |  incoming   | 
      AP |    args     | 
         +-------------+
         | $lr save    |
         +-------------+
 prev SP | back chain  | 
         +-------------+
         |  var args   | 
         |  reg save   | current_function_pretend_args_size bytes
         +-------------+
         |    ...      | 
         | saved regs  | spu_saved_regs_size() bytes
         +-------------+
         |    ...      | 
      FP |   vars      | get_frame_size()  bytes
         +-------------+
         |    ...      | 
         |  outgoing   | 
         |    args     | current_function_outgoing_args_size bytes
         +-------------+
         | $lr of next |
         |   frame     | 
         +-------------+
      SP | back chain  | 
         +-------------+

*/
void
spu_expand_prologue (void)
{
  HOST_WIDE_INT size = get_frame_size (), offset, regno;
  HOST_WIDE_INT total_size;
  HOST_WIDE_INT saved_regs_size;
  rtx sp_reg = gen_rtx_REG (Pmode, STACK_POINTER_REGNUM);
  rtx scratch_reg_0, scratch_reg_1;
  rtx insn, real;

  /* A NOTE_INSN_DELETED is supposed to be at the start and end of
     the "toplevel" insn chain.  */
  emit_note (NOTE_INSN_DELETED);

  /* bugzilla #8989
     If case of  -O0 and -fPIC compilation, spu_expand_prologue is callled
     before checking the PIC base register is used in the function.
     To force the compiler to update PIC base register for each function
     for the option combination of -O0 and -fPIC.
     the flag current_function_uses_pic_offset_table is asssered here. 

     FIXME: this fix is adhoc. please fix it in better way. */
  if (flag_pic && optimize == 0)
    current_function_uses_pic_offset_table = 1;
 
  if (spu_naked_function_p (current_function_decl))
    return;

  scratch_reg_0 = get_scratch_reg(0);
  scratch_reg_1 = get_scratch_reg(scratch_reg_0);

  saved_regs_size = spu_saved_regs_size ();
  total_size = size + saved_regs_size
	       + current_function_outgoing_args_size
	       + current_function_pretend_args_size;

  if (!current_function_is_leaf 
      || current_function_calls_alloca
      || total_size > 0)
    total_size += STACK_POINTER_OFFSET;

  /* Save this first because code after this might use the link
   * register as a scratch register. */
  if (!current_function_is_leaf || REGNO (scratch_reg_0) == LINK_REGISTER_REGNUM)
    {
      insn = frame_emit_store (LINK_REGISTER_REGNUM, sp_reg, 16);
      RTX_FRAME_RELATED_P(insn) = 1;
    }

  if (total_size > 0)
    {

      if (flag_stack_check)
	{
	  /* We compare agains total_size-1 because
	     ($sp >= total_size) <=> ($sp > total_size-1) */
	  rtx scratch_v4si = gen_rtx_REG(V4SImode, REGNO (scratch_reg_0));
	  rtx sp_v4si = gen_rtx_REG(V4SImode, STACK_POINTER_REGNUM);
	  rtx size_v4si = spu_const_vector(V4SImode, GEN_INT(total_size-1));
	  if (!CONST_OK_FOR_LETTER_P (total_size-1, 'K'))
	    {
	      emit_move_insn (scratch_v4si, size_v4si);
	      size_v4si = scratch_v4si;
	    }
	  spu_emit_insn (gen_spu_cgt(scratch_v4si, sp_v4si, size_v4si));
	  spu_emit_insn (gen_spu_rotqby(scratch_v4si, scratch_v4si, GEN_INT(4)));
	  spu_emit_insn (gen_spu_heq (scratch_reg_0, GEN_INT(0) ));
	}

      offset = -current_function_pretend_args_size;
      for (regno = 0; regno < FIRST_PSEUDO_REGISTER; ++regno)
	if (need_to_save_reg (regno, 1))
	  {
	    offset -= 16;
	    insn = frame_emit_store (regno, sp_reg, offset);
	    RTX_FRAME_RELATED_P(insn) = 1;
	  }

    }

  if (flag_pic && current_function_uses_pic_offset_table)
    {
      rtx pic_reg = get_pic_reg();
      insn = emit_insn(gen_load_pic_offset(pic_reg, scratch_reg_0));
      REG_NOTES(insn) = gen_rtx_EXPR_LIST (REG_MAYBE_DEAD, const0_rtx,
					   REG_NOTES (insn));
      insn = emit_insn(gen_subsi3(pic_reg, pic_reg, scratch_reg_0));
      REG_NOTES(insn) = gen_rtx_EXPR_LIST (REG_MAYBE_DEAD, const0_rtx,
					   REG_NOTES (insn));
    }

  if (total_size > 0)
    {
      /* Adjust the stack pointer, and make sure scratch_reg_0 contains
	 the value of the previous $sp because we save it as the back
	 chain. */
      if (total_size <= 2000)
	{
	  /* In this case we save the back chain first. */
	  insn = frame_emit_store (STACK_POINTER_REGNUM, sp_reg, -total_size);
	  RTX_FRAME_RELATED_P(insn) = 1;
	  insn = frame_emit_add_imm (sp_reg, sp_reg, -total_size, scratch_reg_0);
	}
      else if ( CONST_OK_FOR_LETTER_P(-total_size, 'K') )
	{
	  insn = emit_move_insn (scratch_reg_0, sp_reg);
	  RTX_FRAME_RELATED_P(insn) = 1;
	  insn = emit_insn (gen_addsi3 (sp_reg, sp_reg, GEN_INT (-total_size)));
	}
      else if (scratch_reg_1)
	{
	  insn = emit_move_insn (scratch_reg_0, sp_reg);
	  RTX_FRAME_RELATED_P(insn) = 1;
	  insn = frame_emit_add_imm (sp_reg, sp_reg, -total_size, scratch_reg_1);
	}
      else 
	{
	  /* This case doesn't use a second scratch register, but has a
	   * longer dependency chain. */
	  insn = emit_insn (gen_movsi (scratch_reg_0, GEN_INT (total_size)));
	  RTX_FRAME_RELATED_P(insn) = 1;
	  /* We lie here so frame info works out correctly. */
	  real = gen_rtx_SET(Pmode, scratch_reg_0, sp_reg);
	  REG_NOTES (insn) = 
	    gen_rtx_EXPR_LIST (REG_FRAME_RELATED_EXPR,
			       real,
			       REG_NOTES (insn));
	  REG_NOTES(insn) = gen_rtx_EXPR_LIST (REG_MAYBE_DEAD, const0_rtx,
					       REG_NOTES (insn));
	  insn = emit_insn (gen_subsi3 (sp_reg, sp_reg, scratch_reg_0));
	  emit_insn (gen_addsi3 (scratch_reg_0, sp_reg, scratch_reg_0));
	}
      RTX_FRAME_RELATED_P(insn) = 1;
      real = gen_addsi3(sp_reg, sp_reg, GEN_INT(-total_size));
      REG_NOTES (insn) = 
	gen_rtx_EXPR_LIST (REG_FRAME_RELATED_EXPR,
			   real,
			   REG_NOTES (insn));

      if (total_size > 2000)
	{
	  /* Save the back chain ptr */
	  insn = frame_emit_store (REGNO (scratch_reg_0), sp_reg, 0);
	  RTX_FRAME_RELATED_P(insn) = 1;
	}

      if (frame_pointer_needed)
	{
	  rtx fp_reg = gen_rtx_REG (Pmode, HARD_FRAME_POINTER_REGNUM);
	  HOST_WIDE_INT fp_offset = STACK_POINTER_OFFSET
	    + current_function_outgoing_args_size;
	  /* Set the new frame_pointer */
	  frame_emit_add_imm (fp_reg, sp_reg, fp_offset, scratch_reg_0);
	  REGNO_POINTER_ALIGN (HARD_FRAME_POINTER_REGNUM) = STACK_BOUNDARY;
	}
    }

  emit_note (NOTE_INSN_DELETED);
}

void
spu_expand_epilogue (bool sibcall_p)
{
  int size = get_frame_size (), offset, regno;
  HOST_WIDE_INT saved_regs_size, total_size;
  rtx sp_reg = gen_rtx_REG (Pmode, STACK_POINTER_REGNUM);
  rtx jump, scratch_reg_0;

  /* A NOTE_INSN_DELETED is supposed to be at the start and end of
     the "toplevel" insn chain.  */
  emit_note (NOTE_INSN_DELETED);

  if (spu_naked_function_p (current_function_decl))
    return;

  scratch_reg_0 = get_scratch_reg(0);

  saved_regs_size = spu_saved_regs_size ();
  total_size = size + saved_regs_size
	       + current_function_outgoing_args_size
	       + current_function_pretend_args_size;

  if (!current_function_is_leaf
      || current_function_calls_alloca
      || total_size > 0)
    total_size += STACK_POINTER_OFFSET;

  if (total_size > 0)
    {
      if (current_function_calls_alloca)
	/* Load it from the back chain because our save_stack_block and
	   restore_stack_block do nothing. */
	frame_emit_load (STACK_POINTER_REGNUM, sp_reg, 0);
      else 
	frame_emit_add_imm (sp_reg, sp_reg, total_size, scratch_reg_0);


      if (saved_regs_size > 0)
	{
	  offset = -current_function_pretend_args_size;
	  for (regno = 0; regno < FIRST_PSEUDO_REGISTER; ++regno)
	    if (need_to_save_reg (regno, 1))
	      {
		offset -= 0x10;
		frame_emit_load (regno, sp_reg, offset);
	      }
	}
    }

  if (!current_function_is_leaf || REGNO (scratch_reg_0) == LINK_REGISTER_REGNUM)
    frame_emit_load (LINK_REGISTER_REGNUM, sp_reg, 16);

  if (!sibcall_p)
    {
      emit_insn(gen_rtx_USE(VOIDmode, gen_rtx_REG(SImode, LINK_REGISTER_REGNUM)));
      jump = emit_jump_insn (gen_return_internal ());
      emit_barrier_after (jump);
    }

  emit_note (NOTE_INSN_DELETED);
}

rtx
spu_return_addr (int count, rtx frame ATTRIBUTE_UNUSED)
{
  if (count != 0)
      return 0;
  /* This is inefficient because it ends up copying to a save-register
     which then gets saved even though $lr has already been saved.  But
     it does generate better code for leaf functions and we don't need
     to use RETURN_ADDRESS_POINTER_REGNUM to get it working.  It's only
     used for __builtin_return_address anyway, so maybe we don't care if
     it's inefficient. */
  return get_hard_reg_initial_val (Pmode, LINK_REGISTER_REGNUM);
}


rtx
spu_force_reg (enum machine_mode mode, rtx op)
{
  rtx x, r;
  if (GET_MODE (op) == VOIDmode || GET_MODE (op) == BLKmode)
    {
      if ((SCALAR_INT_MODE_P (mode) && GET_CODE (op) == CONST_INT)
	  || GET_MODE (op) == BLKmode)
	return force_reg (mode, convert_to_mode (mode, op, 0));
      abort();
    }

  r = force_reg(GET_MODE(op), op);
  if (GET_MODE_SIZE (GET_MODE (op)) == GET_MODE_SIZE (mode))
    {
      x = simplify_gen_subreg(mode, r, GET_MODE (op), 0);
      if (x)
	return x;
    }

  x = gen_reg_rtx(mode);
  spu_emit_insn(gen_spu_convert(x, r));
  return x;
}

void
spu_builtin_splats (rtx ops[])
{
  enum machine_mode mode = GET_MODE(ops[0]);
  if (CONSTANT_P(ops[1]))
    {
      emit_move_insn(ops[0], spu_const_vector(mode, ops[1]));
    }
  else
    {
      rtx reg = gen_reg_rtx(V16QImode);
      rtx shuf, insn;
      switch (mode)
        {
        case V2DImode:
        case V2DFmode:
 	 shuf = spu_const_from_ints(V16QImode, 0x10111213, 0x14151617, 0x00010203, 0x04050607);
 	 break;
        case V4SImode:
        case V4SFmode:
 	 shuf = spu_const_from_ints(V16QImode, 0x00010203, 0x00010203, 0x00010203, 0x00010203);
 	 break;
        case V8HImode:
 	 shuf = spu_const_from_ints(V16QImode, 0x02030203, 0x02030203, 0x02030203, 0x02030203);
 	 break;
        case V16QImode:
 	 shuf = spu_const_from_ints(V16QImode, 0x03030303, 0x03030303, 0x03030303, 0x03030303);
 	 break;
        default:
 	 abort();
        }
      emit_move_insn(reg, shuf);
      insn = spu_emit_insn(gen_spu_shufb(ops[0], ops[1], ops[1], reg));
      REG_NOTES (insn) = gen_rtx_EXPR_LIST (REG_EQUAL,
					    gen_rtx_VEC_DUPLICATE(mode, ops[1]),
					    REG_NOTES (insn)); 
    }
}

void
spu_builtin_extract (rtx ops[])
{
  enum machine_mode mode;
  rtx rot, from;
  HOST_WIDE_INT pos;
  HOST_WIDE_INT rot_amt = 0;

  mode = GET_MODE (ops[1]);

  if (reload_completed)
    rot = gen_rtx_REG(mode, REGNO(ops[0]));
  else
    rot = gen_reg_rtx(mode);
  from = ops[1];

  if (GET_CODE (ops[2]) == CONST_INT)
    {
      pos = INTVAL (ops[2]);
      switch (mode)
      {
        case V16QImode:
	  rot_amt = pos-3;
          break;
        case V8HImode:
	  rot_amt = 2 * (pos-1);
          break;
        case V4SFmode:
        case V4SImode:
	  rot_amt = 4 * pos;
          break;
        case V2DImode:
        case V2DFmode:
	  rot_amt = 8* pos;
          break;
        default: 
          abort ();
      }
      if (rot_amt == 0)
	emit_move_insn(rot, from);
      else
	spu_emit_insn(gen_spu_rotqby(rot, from, GEN_INT (rot_amt)));
    }
  else
    {
      rtx tmp = ops[3];
      switch (mode)
      {
        case V16QImode:
          emit_insn(gen_addsi3(tmp, ops[2], GEN_INT(-3)));
          break;
        case V8HImode:
          emit_insn(gen_addsi3(tmp, ops[2], ops[2]));
          emit_insn(gen_addsi3(tmp, tmp, GEN_INT(-2)));
          break;
        case V4SFmode:
        case V4SImode:
          emit_insn(gen_ashlsi3(tmp, ops[2], GEN_INT (2)));
          break;
        case V2DImode:
        case V2DFmode:
          emit_insn(gen_ashlsi3(tmp, ops[2], GEN_INT (3)));
          break;
        default: 
          abort();
      }
      spu_emit_insn(gen_spu_rotqby(rot, from, tmp));
    }
  if (!reload_completed)
    spu_emit_insn(gen_spu_convert(ops[0], rot));
}

void
spu_builtin_insert (rtx ops[])
{
  enum machine_mode mode = GET_MODE (ops[0]);
  rtx sp = gen_rtx_REG (Pmode, STACK_POINTER_REGNUM);
  rtx mask = gen_reg_rtx (V16QImode);

  if (GET_CODE (ops[3]) == CONST_INT)
    {
      HOST_WIDE_INT pos = INTVAL (ops[3]);

      switch (mode)
        {
          case V16QImode:
            spu_emit_insn (gen_spu_cbx(mask, sp, GEN_INT (pos))); 
            break;

          case V8HImode:
            spu_emit_insn (gen_spu_chx(mask, sp, GEN_INT (pos*2))); 
            break;

          case V4SImode:
          case V4SFmode:
            spu_emit_insn (gen_spu_cwx(mask, sp, GEN_INT (pos*4))); 
            break;

          case V2DImode:
          case V2DFmode:
            spu_emit_insn (gen_spu_cdx(mask, sp, GEN_INT (pos*8))); 
            break;

          default: 
            abort();
        }
    }
  else
    {
      rtx tmp = gen_reg_rtx(SImode);

      switch (mode)
        {
          case V16QImode:
            spu_emit_insn (gen_spu_cbx(mask, sp, ops[3])); 
            break;

          case V8HImode:
            emit_insn (gen_ashlsi3(tmp, ops[3], GEN_INT (1)));
            spu_emit_insn (gen_spu_chx(mask, sp, tmp)); 
            break;

          case V4SFmode:
          case V4SImode:
            emit_insn (gen_ashlsi3(tmp, ops[3], GEN_INT (2)));
            spu_emit_insn (gen_spu_cwx(mask, sp, tmp)); 
            break;

          case V2DImode:
          case V2DFmode:
            emit_insn (gen_ashlsi3(tmp, ops[3], GEN_INT (3)));
            spu_emit_insn (gen_spu_cdx(mask, sp, tmp)); 
            break;

          default: 
            abort();
        }
    }
  spu_emit_insn (gen_spu_shufb (ops[0], ops[1], ops[2], mask));

}

void
spu_builtin_promote (rtx ops[])
{
  enum machine_mode mode, innermode; 
  rtx rot, from;
  HOST_WIDE_INT pos;

  mode = GET_MODE (ops[0]);
  innermode = GET_MODE_INNER (mode);

  from  = ops[1];
  rot = ops[0];
 
  if (GET_CODE (ops[2]) == CONST_INT)
    {
      pos = INTVAL (ops[2]);
      switch (mode)
      {
        case V16QImode:
          spu_emit_insn(gen_spu_rotqby(rot, from, GEN_INT (3-pos)));
          break;
        case V8HImode:
          spu_emit_insn(gen_spu_rotqby(rot, from, GEN_INT (2*(1-pos))));
          break;
        case V4SFmode:
        case V4SImode:
          spu_emit_insn(gen_spu_rotqby(rot, from, GEN_INT (-4*pos)));
          break;
        case V2DImode:
        case V2DFmode:
          spu_emit_insn(gen_spu_rotqby(rot, from, GEN_INT (-8*pos)));
          break;
        default: 
          abort ();
      }
    }
  else
    {
      rtx tmp = gen_reg_rtx(SImode);
      switch (mode)
      {
        case V16QImode:
          emit_insn(gen_subsi3(tmp, GEN_INT (3), ops[2])); 
          spu_emit_insn(gen_spu_rotqby(rot, from, tmp));
          break;
        case V8HImode:
          emit_insn(gen_subsi3(tmp, GEN_INT (1), ops[2])); 
          emit_insn(gen_addsi3(tmp, tmp, tmp));
          spu_emit_insn(gen_spu_rotqby(rot, from, tmp));
          break;
        case V4SFmode:
        case V4SImode:
          emit_insn(gen_subsi3(tmp, GEN_INT (0), ops[2])); 
          emit_insn(gen_ashlsi3(tmp, tmp, GEN_INT (2)));
          spu_emit_insn(gen_spu_rotqby(rot, from, tmp));
          break;
        case V2DImode:
        case V2DFmode:
          emit_insn(gen_ashlsi3(tmp, ops[2], GEN_INT (3)));
          spu_emit_insn(gen_spu_rotqby(rot, from, tmp));
          break;
        default: 
          abort();
      }
    }
}

static const char *
fsmbi_for_array(unsigned char arr[16])
{
  int v = 0;
  int i;
  int fsmbi = 1;
  for (i = 0; i < 16; i++)
    {
      if (arr[i] != 0 && arr[i] != 0xff)
	fsmbi = 0;
      v <<= 1;
      v |= arr[i] & 1;
    }
  if (!fsmbi 
      || v == 0x0000  /* 00000000  il   %0,0x0000 */
      || v == 0x1111  /* 000000ff  il   %0,0x00ff */
      || v == 0x2222  /* 0000ff00  ila  %0,0xff00 */
      || v == 0x3333  /* 0000ffff  ila  %0,0xffff */
      || v == 0x4444  /* 00ff0000  ilhu %0,0x00ff */
      || v == 0x5555  /* 00ff00ff  ilh  %0,0x00ff */
      || v == 0x8888  /* ff000000  ilhu %0,0xff00 */
      || v == 0xaaaa  /* ff00ff00  ilh  %0,0xff00 */
      || v == 0xcccc  /* ffff0000  ilhu %0,0xffff */
      || v == 0xeeee  /* ffffff00  il   %0,-256   */
      || v == 0xffff) /* ffffffff  il   %0,-1     */
    return 0;
  return "fsmbi\t%0,%F1";
}

static const char *
cxd_for_array(unsigned char arr[16])
{
  int i;
  int cxd = 1, run = 0;
  for (i = 0; i < 16; i++)
    if (arr[i] != i+16)
      {
	if (!run)
	  {
	    if (arr[i] == 3)
	      run = 1;
	    else if (arr[i] == 2 && arr[i+1] == 3)
	      run = 2;
	    else if (arr[i] == 0)
	      {
		while (arr[i+run] == run && i+run < 16)
		  run++;
		if (run != 4 && run != 8)
		  cxd = 0;
	      }
	    else
	      cxd = 0;
	    if ((i & (run-1)) != 0)
	      cxd = 0;
	    i += run;
	  }
	else
	  cxd = 0;
      }

  if (!cxd)
    return 0;
  if (run == 1)
    return "cbd\t%0,%C1($sp)";
  if (run == 2)
    return "chd\t%0,%C1($sp)";
  if (run == 4)
    return "cwd\t%0,%C1($sp)";
  if (run == 8)
    return "cdd\t%0,%C1($sp)";
  return 0;
}

static const char *
move_int32_asm (HOST_WIDE_INT val)
{
    int high = (val >> 16) & 0xffff;
    int low = val & 0xffff;

    if (val <= 0x7fffll && val >= -0x8000ll)
    {
      return "il\t%0,%M1";
    }
    else if (val <= 0x3ffffll  && val >= 0ll)
    {
      return "ila\t%0,%M1";
    }
  else if (high == low)
    {
      return "ilh\t%0,%L1";
    }
  else if (low == 0)
    {
      return "ilhu\t%0,%H1";
    }
  else if (TARGET_DONT_SPLIT)
    {
      return "ilhu\t%0,%H1\n\tiohl\t%0,%L1";
    }
  abort();
}

const char *
spu_emit_move_asm (rtx *ops, enum machine_mode mode)
{
  rtx dst = ops[0];
  rtx src = ops[1];
  static char buf[1024];
  rtx from = src;
  rtx to = dst;
  int size = GET_MODE_SIZE(mode);

  if (GET_CODE (from) == SUBREG || GET_CODE (to) == SUBREG)
    abort();

  if (GET_CODE (to) == REG)
    {
      if (GET_CODE(from) == REG)
        {
	  return "ori\t%0,%1,0";
        }
      else if (GET_CODE (from) == CONST_INT)
        {
	  HOST_WIDE_INT val = INTVAL(from);
	  switch(size)
	    {
	    case 1:
	    case 2:
	      return "il\t%0,%1";
	    case 4:
	      return move_int32_asm(val);
	    case 8:
              if (val == -1ll || val == 0ll)
                  return move_int32_asm(val);
              else if (TARGET_DONT_SPLIT && val >= -0x80000000ll && val <= 0x7fffffffll)
		{
                  sprintf(buf, "%s\n\txswd\t%%0,%%0", move_int32_asm(val));
                  return buf;
		}
              else if (TARGET_DONT_SPLIT && val > 0x7fffffffll && val <= 0xffffffffll)
		{
                  sprintf(buf, "%s\n\trotqmbyi\t%%0,%%0,-4", move_int32_asm(val));
                  return buf;
		}
	      else
		abort();
	    case 16:
              if (val == -1ll || val == 0ll)
                  return move_int32_asm(val);
              else if (TARGET_DONT_SPLIT && val > 0ll && val <= 0xffffffffll)
		{
                  sprintf(buf, "%s\n\trotqmbyi\t%%0,%%0,-12", move_int32_asm(val));
                  return buf;
		}
	      else
		abort();
            }
        }
      else if (GET_CODE (from) == CONST_DOUBLE && GET_MODE(from) == VOIDmode)
	{
	  HOST_WIDE_INT val;
	  unsigned char arr[16];
	  constant_to_array (TImode, from, arr);
	  val = (arr[0] << 24) | (arr[1] << 16) | (arr[2] << 8) | arr[3];
	  val = trunc_int_for_mode (val, SImode);
	  return move_int32_asm (val);
	}
      else if (GET_CODE (from) == CONST_DOUBLE)
	{
	  HOST_WIDE_INT val = const_double_to_hwint(from);
	  if (GET_MODE(from) == DFmode && val != 0)
	    abort();
	  return move_int32_asm(val);
	}
      else if (GET_CODE (from) == CONST_VECTOR)
	{
          HOST_WIDE_INT val;
	  unsigned char arr[16];
	  const char *asmstr;
	  rtx elt = CONST_VECTOR_ELT (from, 0);
	  if (GET_CODE(elt) == CONST || GET_CODE(elt) == SYMBOL_REF)
	    return "ila\t%0,%1";
	  constant_to_array(GET_MODE(from), from, arr);
	  if ((asmstr = fsmbi_for_array(arr)))
	      return asmstr;
	  if ((asmstr = cxd_for_array(arr)))
	      return asmstr;
	  val = (arr[0] << 24) | (arr[1] << 16) | (arr[2] << 8) | (arr[3]);
	  val = (val << 32) >> 32;
          return move_int32_asm(val);
      }
      else if (GET_CODE (from) == MEM)
	{
	  return "lq%f1\t%0,%1";
	}
      else if (!TARGET_LARGE_MEM && CONSTANT_P(from))
	{
	  if (!flag_pic ||
	      (GET_CODE(from) != CONST
	       && GET_CODE(from) != SYMBOL_REF
	       && GET_CODE(from) != LABEL_REF))
	    return "ila\t%0,%1";
	}
    }
  else if (GET_CODE(to) == MEM && GET_CODE(from) == REG)
    {
      return "stq%f0\t%1,%0";
    }
  abort();
}

rtx
spu_const_vector(enum machine_mode mode, rtx inner)
{
  rtvec v;
  int units, i;

  units = GET_MODE_NUNITS (mode);

  v = rtvec_alloc (units);

  for (i = 0; i < units; ++i)
    RTVEC_ELT (v, i) = inner;

  return gen_rtx_CONST_VECTOR (mode, v);
}

/* Create a MODE vector constant from 4 ints. */
rtx
spu_const_from_ints(enum machine_mode mode, int a, int b, int c, int d)
{
  unsigned char arr[16];
  arr[0] = (a >> 24) & 0xff;
  arr[1] = (a >> 16) & 0xff;
  arr[2] = (a >> 8) & 0xff;
  arr[3] = (a >> 0) & 0xff;
  arr[4] = (b >> 24) & 0xff;
  arr[5] = (b >> 16) & 0xff;
  arr[6] = (b >> 8) & 0xff;
  arr[7] = (b >> 0) & 0xff;
  arr[8] = (c >> 24) & 0xff;
  arr[9] = (c >> 16) & 0xff;
  arr[10] = (c >> 8) & 0xff;
  arr[11] = (c >> 0) & 0xff;
  arr[12] = (d >> 24) & 0xff;
  arr[13] = (d >> 16) & 0xff;
  arr[14] = (d >> 8) & 0xff;
  arr[15] = (d >> 0) & 0xff;
  return array_to_constant(mode, arr);
}


/* Routines for reordering basic blocks */
struct spu_bb_info
{
  rtx prop_jump; /* propogated from another block */
  int bb_index;  /* the orignal block. */
};
static struct spu_bb_info *spu_bb_info;

#define STOP_HINT_P(INSN) \
		(GET_CODE(INSN) == CALL_INSN \
		 || INSN_CODE(INSN) == CODE_FOR_divmodsi4 \
		 || INSN_CODE(INSN) == CODE_FOR_udivmodsi4)

static void
emit_nop_for_insn(basic_block bb, rtx insn)
{
  int p;
  rtx new_insn;
  p = get_pipe(insn);
  if (p == 0)
    {
      new_insn = emit_insn_after(gen_spu_lnop(), insn);
      if (bb && BB_END (bb) == insn)
	BB_END (bb) = new_insn;
    }
  else
    {
      new_insn = emit_insn_before(gen_spu_nopn(GEN_INT(127)), insn);
      if (bb && BB_HEAD (bb) == insn)
	BB_HEAD (bb) = new_insn;
      PUT_MODE(new_insn, TImode);
      PUT_MODE(insn, VOIDmode);
    }
  INSN_BLOCK_CYCLE(new_insn) = INSN_BLOCK_CYCLE(insn);
  recog_memoized(new_insn);
}

/* Insert nops in basic blocks to meet dual issue alignment and to pad
 * out a block to an 8-byte boundary.
 * Actually, we don't need to pad out basic blocks to an 8 byte boundary
 * anymore, our previous scheduler assumed we did. */
static void
pad_bb(void)
{
  rtx insn, last_insn, prev_single, hint = NULL_RTX, note;
  int i, length, hint_pos = 0;
  int cycle_inc = 0;
  length = 0;
  last_insn = 0;
  prev_single = 0;
  for (i = 0; i < n_basic_blocks; ++i)
    {
      basic_block bb = BASIC_BLOCK (i);
      for (insn = BB_HEAD (bb); insn; insn = NEXT_INSN(insn))
	{
	  if (INSN_P(insn))
	    {
	      if (INSN_CODE(insn) < 0)
		recog_memoized(insn);

	      if (INSN_CODE(insn) >= 0)
		{
		  INSN_BLOCK_CYCLE(insn) += cycle_inc;
		  if (last_insn && GET_MODE(last_insn) == TImode)
		    {
		      /* TImode has been set by the scheduler on each
		       * insn that starts a new cycle.  We use that to
		       * determine where we need to insert nops to force
		       * correct alignment for dual issue. */
		      if ((length & 7) != 0
			  && get_attr_type(insn) == TYPE_MULTI0)
			{
			  emit_nop_for_insn(bb, last_insn);
			  length += 4;
			  prev_single = 0;
			}
		      else if (GET_MODE(insn) == TImode)
			{
			  prev_single = (length & 7) ? last_insn : 0;
			}
		      else if (prev_single)
			{
			  emit_nop_for_insn(bb, prev_single);
			  length += 4;
			  prev_single = 0;
			}
		    }
		  last_insn = insn;
		}
	      length += get_attr_length(insn);
	      if (INSN_CODE (insn) == CODE_FOR_spu_hbr
	          || INSN_CODE (insn) == CODE_FOR_spu_hbrr)
		{
		  hint = insn;
		  hint_pos = length;
		}
	      else 
		{
		  if (hint
		       && (GET_CODE (insn) == JUMP_INSN
			   || GET_CODE (insn) == CALL_INSN)
		       && length - hint_pos <= 32
		       && (note = find_reg_note (insn, REG_BR_HINT, 0)) != 0
		       && hint == XEXP (XEXP (note, 0), 0))
		    {
		      /* Insert nops in stalls.  Only insert pairs of nops to maintain
		       * alignment.  If there are no stalls to fill then insert nops before
		       * the branch. */
		      rtx nop, prev = hint;
		      while (length - hint_pos <= 32 && hint && hint != insn)
			{
			  hint = NEXT_INSN(hint);
			  if (hint && INSN_P(hint) && INSN_CODE(hint) >= 0)
			    {
			      int c = INSN_BLOCK_CYCLE(prev) + 1;
			      int nc = INSN_BLOCK_CYCLE(hint) - 1;
			      while ((c+1 < nc || hint == insn) && length - hint_pos <= 32) 
				{
				  nop = emit_insn_before (gen_spu_nopn(GEN_INT(127)), hint);
				  INSN_BLOCK_CYCLE(nop) = c;
				  nop = emit_insn_before (gen_spu_nopn(GEN_INT(127)), hint);
				  INSN_BLOCK_CYCLE(nop) = c+1;
				  length += 8;
				  c+=2;
				}
			      prev = hint;
			    }
			}
		      hint = 0;
		    }
		}
	    }
	  if (insn == BB_END (bb))
	    break;
	}
      if (!last_insn) /* An empty block */
	continue;
      cycle_inc = INSN_BLOCK_CYCLE(last_insn) + 1;
      /*
      if ((length & 7) != 0)
	{
	  emit_nop_for_insn(bb, last_insn);
	  length += 4;
	}
      if (hint)
	hint_pos -= length;
	*/
    }
}


/* Routines for branch hints. */

static rtx
hbr_branch (rtx insn)
{
  rtx unspec = SET_SRC (XVECEXP (PATTERN (insn), 0, 0));
  return XVECEXP (unspec, 0, 0);
}

static rtx
hbr_target (rtx insn)
{
  rtx unspec = SET_SRC (XVECEXP (PATTERN (insn), 0, 0));
  return XVECEXP (unspec, 0, 1);
}

static void
spu_emit_branch_hint (rtx before, rtx branch, rtx target,
		      int distance, /* estimate of maximum distance from hint
				       to branch. */
		      int offset_of_branch)
{
  rtx branch_label = 0;
  rtx hint;
  rtx insn;
  rtx note;

  if (before == 0 || branch == 0 || target == 0)
    return;

  /* While scheduling we require hints to be no further than 600, so
     we need to enforce that here too */
  if (distance > 600)
    return;

  if ((note = find_reg_note (branch, REG_BR_HINT, 0)))
    {
      hint = XEXP (XEXP (note, 0), 0);
      branch_label = hbr_branch (hint);
      SET_INSN_DELETED (hint);
    }

  if (branch_label == 0 || branch_label == const0_rtx)
    {
      branch_label = gen_label_rtx();
      LABEL_NUSES (branch_label)++;
      LABEL_PRESERVE_P (branch_label) = 1;
      insn = emit_label_before(branch_label, branch);
      delete_insn(insn); /* delete it so schedule_insns ignores it */
      branch_label = gen_rtx_LABEL_REF (VOIDmode, branch_label);
    }

  /* LABEL_NUSES (branch_label)++; LABEL_ALIGN in spu.h checks this so
   * it doesn't align a label on a branch. */

  if ((TARGET_LARGE_MEM || flag_pic)
      && GET_CODE(target) != REG
      && GET_CODE (target) != CONST_INT)
    hint = gen_spu_hbrr (branch_label, target);
  else
    hint = gen_spu_hbr (branch_label, target);

  hint = emit_insn_before (hint, before);
  if (note)
    XEXP (XEXP (note, 0), 0) = hint;
  else
    REG_NOTES (branch)
      = gen_rtx_EXPR_LIST (REG_BR_HINT,
			   gen_rtx_INSN_LIST(0, hint, 0), REG_NOTES (branch));
  INSN_BLOCK_CYCLE (hint) = offset_of_branch;
  recog_memoized(hint);

}

/* Returns 0 if we don't want a hint for this branch.  Otherwise return
 * the rtx for the branch target. */
static rtx
get_branch_target (rtx branch)
{
  rtx note = find_reg_note (branch, REG_BR_HINT, 0);
  if (note)
    return hbr_target (XEXP (XEXP (note, 0), 0));

  if (GET_CODE(branch) == JUMP_INSN)
    {
      rtx set, src;

      /* Return statements */
      if (GET_CODE(PATTERN (branch)) == RETURN)
	return gen_rtx_REG(SImode, LINK_REGISTER_REGNUM);

      /* jump table */
      if (GET_CODE(PATTERN (branch)) == ADDR_VEC
         || GET_CODE(PATTERN (branch)) == ADDR_DIFF_VEC)
	return 0;

      set = single_set(branch);
      src = SET_SRC(set);
      if (GET_CODE(SET_DEST(set)) != PC)
	abort();

      if (GET_CODE(src) == IF_THEN_ELSE)
	{
	  rtx lab = 0;
	  note = find_reg_note (branch, REG_BR_PROB, 0);
	  if (note)
	    { 
	      /* If the more probable case is not a fall through, then
	       * try a branch hint.  */
	      HOST_WIDE_INT prob = INTVAL (XEXP (note, 0));
	      if (prob > (REG_BR_PROB_BASE * 6 / 10) && GET_CODE(XEXP(src, 1)) != PC)
		lab = XEXP(src, 1);
	      else if (prob < (REG_BR_PROB_BASE * 4 / 10) && GET_CODE(XEXP(src, 2)) != PC)
		lab = XEXP(src, 2);
	    }
	  if (lab)
	    {
	      if (GET_CODE (lab) == RETURN)
		return gen_rtx_REG(SImode, LINK_REGISTER_REGNUM);
	      return lab;
	    }
	  return 0;
	}

      return src;
    }
  else if (GET_CODE(branch) == CALL_INSN)
    {
      rtx call;
      /* All of our call patterns are in a PARALLEL and the CALL is
	 the first pattern in the PARALLEL. */
      if (GET_CODE (PATTERN (branch)) != PARALLEL) 
	abort();
      call = XVECEXP (PATTERN (branch), 0, 0);
      if (GET_CODE(call) == SET)
	call = SET_SRC(call);
      if (GET_CODE(call) != CALL)
	abort();
      return XEXP(XEXP(call, 0), 0);
    }
  return 0;
}

/* Return true if ref is the call target.  First find CALL in X, then
 * compare it to REF.  */
static bool
referenced_in_call (rtx x, rtx ref)
{
  enum rtx_code code = GET_CODE (x);
  int i;
  const char *fmt;

  if (code == CALL)
    return MEM_P (XEXP (x, 0))
           && rtx_equal_p(XEXP (XEXP (x, 0), 0), ref);

  fmt = GET_RTX_FORMAT (code);
  for (i = GET_RTX_LENGTH (code) - 1; i >= 0; i--)
    {
      if (fmt[i] == 'e')
	return referenced_in_call (XEXP (x, i), ref);
      else if (fmt[i] == 'E')
	{
	  int j;
	  for (j = 0; j < XVECLEN (x, i); j++)
	    if (referenced_in_call(XVECEXP (x, i, j), ref))
	      return TRUE;
	}
    }
  return FALSE;
}

/* A branch hint can be inserted by the user with either
 * __builtin_expect, __builtin_expect_call or __builtin_branch_hint.
 * For __builtin_expect_call we find the next call insn and move the
 * REG_BR_HINT note.  For __builtin_branch_hint we find the next branch
 * that needs a hint and force a hint for it.  */
static void
fixup_user_hints(void)
{
  rtx insn, note, prefetch_hint = 0, prefetch_call = 0, prefetch_target, branch_target;
  for (insn = get_insns(); insn; insn = NEXT_INSN (insn))
    if (INSN_P (insn))
      {
	note = find_reg_note(insn, REG_BR_HINT, 0);
	if (note && NONJUMP_INSN_P (insn))
	  {
	    rtx set;
	    /* __builtin_expect_call will put a REG_BR_HINT on
	     * a NONJUMP_INSN, we need move it to the matching
	     * call instruction. */
	    if (prefetch_call && XEXP (XEXP (prefetch_call, 0), 0) != insn)
	      SET_INSN_DELETED (XEXP (XEXP (prefetch_call, 0), 0));
	    if ((set = single_set (insn)))
	      {
		prefetch_call = note;
		prefetch_target = SET_DEST (single_set (insn));
	      }
	    else
	      SET_INSN_DELETED (XEXP (XEXP (note, 0), 0));
	    remove_note (insn, note);
	  }
	else if (prefetch_call && CALL_P (insn)
	         && referenced_in_call (PATTERN (insn), prefetch_target))
	  {
	    XEXP (prefetch_call, 1) = REG_NOTES (insn);
	    REG_NOTES (insn) = prefetch_call;
	    prefetch_call = 0;
	  }
	if (INSN_CODE (insn) == CODE_FOR_spu_hbr
	    && hbr_target (insn) == const0_rtx)
	  {
	    prefetch_hint = insn;
	    SET_INSN_DELETED (prefetch_hint);
	  }
	else if (prefetch_hint
		 && (JUMP_P (insn) || CALL_P (insn))
		 && (branch_target = get_branch_target(insn))
		 && find_reg_note (insn, REG_BR_HINT, 0) == 0)
	  {
	    /* When the prefetch is too far from the branch, or when a
	     * hint isn't actually needed, we ignore the prefetch
	     * request.  It's not always possible for a user to predict
	     * when a hint is needed, for example, maybe they wanted to
	     * hint a function call that gets inlined, or hint a
	     * conditional branch which gets properly reordered so its
	     * more probable case has no penalty. */
	    spu_emit_branch_hint (insn, insn, branch_target, 0, 0);
	    prefetch_hint = 0;
	  }
      }
  if (prefetch_call)
    SET_INSN_DELETED (XEXP (XEXP (prefetch_call, 0), 0));
}

/* The SPU_CONVERT unspec emits no instruction, but it has a slight
 * effect on the insn scheduler, so we remove them. */
static void
strip_spu_convert(void)
{
  rtx insn;
  for (insn = get_insns(); insn; insn = NEXT_INSN (insn))
    if (INSN_P (insn) && INSN_CODE (insn) == CODE_FOR_spu_convert)
      SET_INSN_DELETED (insn);
}

/* The special $hbr register is used to prevent the insn scheduler from
 * moving hbr insns across instructions which invalidate them.  It
 * should only be used in a clobber, and this function searches for
 * insns which clobber it.  */
static bool
insn_clobbers_hbr (rtx insn)
{
  if (INSN_P (insn)
      && GET_CODE (PATTERN (insn)) == PARALLEL)
    {
      rtx parallel = PATTERN (insn);
      rtx clobber;
      int j;
      for (j = XVECLEN (parallel, 0) - 1; j >= 0; j--)
	{
	  clobber = XVECEXP (parallel, 0, j);
	  if (GET_CODE (clobber) == CLOBBER
	      && GET_CODE (XEXP (clobber, 0)) == REG
	      && REGNO (XEXP (clobber, 0)) == HBR_REGNUM)
	    return 1;
	}
    }
  return 0;
}

static int in_spu_reorg;

/* Insert branch hints.  There are no branch optimizations after this
 * pass, so it's safe to set our branch hints now. */
static void
spu_machine_dependent_reorg (void)
{
  rtx branch, insn, note;
  rtx branch_target = 0;
  int branch_addr, insn_addr, head_addr, required_dist;
  int i;
  unsigned int j;

  if (!TARGET_BRANCH_HINTS || optimize == 0)
    return;

  in_spu_reorg = 1;

  compact_blocks();

  spu_bb_info = (struct spu_bb_info *)xcalloc(n_basic_blocks, sizeof(struct spu_bb_info));

  /* We need exact insn addresses and lengths.  */
  shorten_branches (get_insns ());

  strip_spu_convert();

  fixup_user_hints();

  for (i = n_basic_blocks-1; i >= 0; i--)
    {
      basic_block bb = BASIC_BLOCK (i);
      head_addr = INSN_ADDRESSES (INSN_UID (BB_HEAD (bb)));
      branch = 0;
      if (spu_bb_info[i].prop_jump)
	{
	  branch = spu_bb_info[i].prop_jump;
	  branch_target = get_branch_target(branch);
	  branch_addr = INSN_ADDRESSES (INSN_UID (branch));
	  required_dist = spu_hint_dist;
	  if (find_reg_note (branch, REG_BR_HINT, 0))
	    required_dist = 0;
	}
      /* Search from end of a block to beginning.   In this loop, find
       * jumps which need a branch and emit them only when:
       *   - it's an indirect branch and we're at the insn which sets
       *     the register  
       *   - we're at an insn that will invalidate the hint. e.g., a
       *     call, another hint insn, inline asm that clobbers $hbr, and
       *     some inlined operations (divmodsi4).  Don't consider jumps
       *     because they are only at the end of a block and are
       *     considered when we are deciding whether to propagate
       *   - we're getting too far away from the branch.  The hbr insns
       *     only have a signed 10 bit offset
       * We go back as far as possible so the branch will be considered
       * for propagation when we get to the beginning of the block.  */
      for (insn = BB_END(bb); insn; insn = PREV_INSN(insn))
	{
	  if (INSN_P(insn))
	    {
	      insn_addr = INSN_ADDRESSES (INSN_UID (insn));
	      if (branch
		  && ((GET_CODE(branch_target) == REG
		       && set_of(branch_target, insn) != NULL_RTX)
		      || insn_clobbers_hbr(insn)
		      || branch_addr - insn_addr > 600))
		{
		  rtx next = NEXT_INSN (insn);
		  int next_addr = INSN_ADDRESSES (INSN_UID (next));
		  if (insn != BB_END (bb)
		      && branch_addr - next_addr >= required_dist)
		    {
		      if (dump_file)
			fprintf(dump_file, "hint for %i in block %i before %i\n",
				INSN_UID (branch), bb->index, INSN_UID (next));
		      spu_emit_branch_hint (next, branch, branch_target,
					    branch_addr - next_addr, branch_addr - head_addr);
		    }
		  branch = 0;
		}

	      /* JUMP_P will only be true at the end of a block.  When
	       * branch is already set it means we've previously decided
	       * to propagate a hint for that branch into this block. */
	      if (CALL_P (insn) || (JUMP_P (insn) && !branch))
		{
		  branch = 0;
		  if ((branch_target = get_branch_target(insn)))
		    {
		      branch = insn;
		      branch_addr = insn_addr;
		      required_dist = spu_hint_dist;
		      if (find_reg_note (branch, REG_BR_HINT, 0))
			required_dist = 0;
		    }
		}
	    }
	  if (insn == BB_HEAD (bb))
	    break;
	}

      if (branch)
	{
	  /* If we haven't emitted a hint for this branch yet, it might
	   * be profitable to emit it in one of the predecessor blocks,
	   * especially for loops.  */
	  rtx bbend;
	  basic_block prev = 0, prop = 0, prev2 = 0;
	  int loop_exit = 0, simple_loop = 0;
	  int next_addr = INSN_ADDRESSES (INSN_UID (NEXT_INSN (insn)));

	  for (j = 0; j < EDGE_COUNT (bb->preds); j++)
	     if (EDGE_PRED (bb, j)->flags & EDGE_FALLTHRU)
	       prev = EDGE_PRED (bb, j)->src;
	     else
	       prev2 = EDGE_PRED (bb, j)->src;

	  for (j = 0; j < EDGE_COUNT (bb->succs); j++)
	     if (EDGE_SUCC (bb, j)->flags & EDGE_LOOP_EXIT)
	       loop_exit = 1;
	     else if (EDGE_SUCC (bb, j)->dest == bb)
	       simple_loop = 1;

	  /* If this branch is a loop exit then propagate to previous
	   * fallthru block. This catches the cases when it is a simple
	   * loop or when there is an initial branch into the loop. */
	  if (prev && loop_exit
	      && prev->loop_depth <= bb->loop_depth)
	    prop = prev;

	  /* If there is only one adjacent predecessor.  Don't propagate
	   * outside this loop.  This loop_depth test isn't perfect, but
	   * I'm not sure the loop_father member is valid at this point.  */
	  else if (prev && single_pred_p (bb)
		   && prev->loop_depth == bb->loop_depth)
	    prop = prev;

	  /* If this is the JOIN block of a simple IF-THEN then
	   * propogate the hint to the HEADER block. */
	  else if (prev && prev2
	           && EDGE_COUNT (bb->preds) == 2
		   && EDGE_COUNT (prev->preds) == 1
		   && EDGE_PRED (prev, 0)->src == prev2
		   && prev2->loop_depth == bb->loop_depth
		   && GET_CODE (branch_target) != REG)
	    prop = prev;

	  /* Don't propagate when:
	   *   - this is a simple loop and the hint would be too far
	   *   - this is not a simple loop and there are 16 insns in
	   *     this block already
	   *   - the predecessor block ends in a branch that will be
	   *     hinted
	   *   - the predecessor block ends in an insn that invalidates
	   *     the hint */
	  if (prop
	      && prop->index >= 0
	      && (bbend = BB_END (prop))
	      && branch_addr - INSN_ADDRESSES (INSN_UID (bbend)) < (simple_loop ? 600 : 16 * 4)
	      && get_branch_target (bbend) == 0
	      && (JUMP_P (bbend) || !insn_clobbers_hbr(bbend)))
	    {
	      if (dump_file)
		fprintf(dump_file, "propagate from %i to %i (loop depth %i) "
		                   "for %i (loop_exit %i simple_loop %i dist %i)\n",
			bb->index, prop->index, bb->loop_depth, INSN_UID (branch),
			loop_exit, simple_loop, branch_addr - INSN_ADDRESSES (INSN_UID (bbend)));

	      spu_bb_info[prop->index].prop_jump = branch;
	      spu_bb_info[prop->index].bb_index = i;
	    }
	  else if (branch_addr - next_addr >= required_dist)
	    {
	      if (dump_file)
		fprintf(dump_file, "hint for %i in block %i before %i\n",
			INSN_UID (branch), bb->index, INSN_UID (NEXT_INSN (insn)));
	      spu_emit_branch_hint (NEXT_INSN (insn), branch, branch_target,
				    branch_addr - next_addr, branch_addr - head_addr);
	    }
	  branch = 0;
	}
    }
  free(spu_bb_info);

  /* The hints need to be scheduled, so call it again.  We could disable
   * the second scheduling pass in certain situations, but we don't
   * bother for now. */
  compute_bb_for_insn();
  update_life_info (0, UPDATE_LIFE_GLOBAL, 0);
  schedule_insns(dump_file);
  free_bb_for_insn();

  pad_bb();

  /* This bit of code places labels for branch hints.  We don't do it
   * earlier because branch optimizations and scheduling will change
   * their locations, so we just place them once, here, at the end.  */
  for (insn = get_insns(); insn; insn = NEXT_INSN(insn))
    {
      rtx branch_label;
      if ((GET_CODE (insn) == JUMP_INSN || GET_CODE (insn) == CALL_INSN)
	  && (note = find_reg_note(insn, REG_BR_HINT, 0)))
	{
	  rtx insn_list = XEXP (note, 0);
	  rtx hint = XEXP (insn_list, 0);
	  /* Move the hints labels.  Hints we just added above will not
	   * have lables as part of the reg note.  */
	  branch_label = XEXP(hbr_branch(hint), 0);
	  remove_insn(branch_label);
	  add_insn_before(branch_label, insn);
	  if ((insn_list = XEXP (insn_list, 1)))
	    {
	      rtx true_label = XEXP (insn_list, 0);
	      rtx false_label = XEXP (XEXP (insn_list, 1), 0);
	      remove_insn(false_label);
	      remove_insn(true_label);
	      add_insn_before(false_label, next_active_insn(insn));
	      add_insn_after(true_label, JUMP_LABEL(insn));
	    }
	}
    }

  in_spu_reorg = 0;
}


/* Routines for splitting insns which improves insn scheduling. */

static int
split_int32 (rtx to, HOST_WIDE_INT val, int force, int vec)
{
  HOST_WIDE_INT high = (val >> 16) & 0xffff;
  HOST_WIDE_INT low = val & 0xffff;

  if ((val <= 0x3ffffll && val >= -0x8000ll) || (high == low) || (low == 0))
    {
      if (force)
	{
	  val = trunc_int_for_mode (val, SImode);
	  if (vec)
	    {
	      rtx reg = gen_rtx_REG(V4SImode, REGNO(to));
	      emit_insn (gen_movv4si(reg, spu_const_vector(V4SImode, GEN_INT(val))));
	    }
	  else
	    {
	      rtx reg = gen_rtx_REG(SImode, REGNO(to));
	      emit_insn (gen_movsi(reg, GEN_INT(val)));
	    }
	  return 1;
	}
      return 0;
    }
  else
    {
      high = trunc_int_for_mode (high, HImode);
      if (vec)
	{
	  rtx reg = gen_rtx_REG(V4SImode, REGNO(to));
	  spu_emit_insn (gen_spu_ilhu(reg, GEN_INT (high)));
	  spu_emit_insn (gen_spu_iohl(reg, reg, GEN_INT (low)));
	}
      else
	{
	  rtx reg = gen_rtx_REG(SImode, REGNO(to));
	  emit_insn (gen_movsi(reg, GEN_INT (high << 16)));
	  emit_insn (gen_iorsi3(reg, reg, GEN_INT (low)));
	}
      return 1;
    }
}

int
spu_split_move (rtx *ops, enum machine_mode mode)
{
  int size = GET_MODE_SIZE (mode);
  rtx to = ops[0];
  rtx from = ops[1];

  if (GET_CODE (from) == SUBREG || GET_CODE (to) == SUBREG)
    abort ();

  if (GET_CODE (to) == REG && CONSTANT_P(from))
    {
      if (GET_CODE (from) == CONST_INT)
	{
	  HOST_WIDE_INT val = INTVAL (from);
	  switch (size)
	    {
	    case 1:
	    case 2:
	      return 0;
	    case 4:
	      return split_int32 (to, val, 0, 0);
	      break;
	    case 8:
	      if (val != -1ll && val != 0ll)
		{
		  split_int32 (to, val, 1, 0);
		  if (val > 0x7fffffffll && val <= 0xffffffffll)
		    {
		      rtx to_v16 = gen_rtx_REG (V16QImode, REGNO (to));
		      spu_emit_insn (gen_spu_rotqmby (to_v16, to_v16, GEN_INT(-4)));
		    }
		  else
		    {
		      rtx to_v2 = gen_rtx_REG (V2DImode, REGNO (to));
		      rtx to_v4 = gen_rtx_REG (V4SImode, REGNO (to));
		      spu_emit_insn (gen_spu_xswd (to_v2, to_v4));
		    }
		  return 1;
		}
	      else
	        return 0;
	      break;
	    case 16:
	      if (val != -1ll && val != 0ll)
		{
		  rtx to_v16 = gen_rtx_REG (V16QImode, REGNO (to));
		  split_int32 (to, val, 1, 0);
		  spu_emit_insn (gen_spu_rotqmby (to_v16, to_v16, GEN_INT(-12)));
		  return 1;
		}
	      else 
		return 0;
	      break;
	    }
	}
      else if (GET_CODE (from) == CONST_DOUBLE && GET_MODE(from) == SFmode)
	{
	  HOST_WIDE_INT val = const_double_to_hwint(from);
	  return split_int32 (to, val, 0, 0);
	}
      else if (GET_CODE (from) == CONST_DOUBLE && GET_MODE(from) == VOIDmode)
	{
	  HOST_WIDE_INT val;
	  unsigned char arr[16];
	  constant_to_array (TImode, from, arr);
	  val = (arr[0] << 24) | (arr[1] << 16) | (arr[2] << 8) | arr[3];
	  val = trunc_int_for_mode (val, SImode);
	  return split_int32 (to, val, 0, 1);
	}
      else if (GET_CODE (from) == CONST_VECTOR)
	{
          HOST_WIDE_INT val;
	  unsigned char arr[16];
	  if (GET_CODE (CONST_VECTOR_ELT (from, 0)) == SYMBOL_REF
	      || GET_CODE (CONST_VECTOR_ELT (from, 0)) == CONST)
	    {
	      /* This only occurs when all elements are the same. */
	      if (TARGET_LARGE_MEM)
		{
		  spu_emit_insn(gen_high_v4si(to, from));
		  spu_emit_insn(gen_low_v4si(to, to, from));
		  return 1;
		}
	      return 0;
	    }
	  constant_to_array(GET_MODE(from), from, arr);
	  if (fsmbi_for_array(arr))
	      return 0;
	  if (cxd_for_array(arr))
	      return 0;
	  val = (arr[0] << 24) | (arr[1] << 16) | (arr[2] << 8) | (arr[3]);
	  val = (val << 32) >> 32;
          return split_int32(to, val, 0, 1);
	}
      else if (CONSTANT_P(from))
	{
	  if (mode != SImode
	      || (GET_CODE(from) != LABEL_REF
		  && GET_CODE(from) != SYMBOL_REF
		  && GET_CODE(from) != CONST))
	    abort();
	  if (TARGET_LARGE_MEM
	      || (GET_CODE (from) == CONST && !legitimate_const (from)))
	    {
	      emit_insn(gen_high(to, from));
	      emit_insn(gen_low(to,to,from));
	      if (flag_pic)
		{
		  rtx pic_reg = get_pic_reg();
		  emit_insn(gen_addsi3(to, to, pic_reg));
		  current_function_uses_pic_offset_table = 1;
		}
	      return 1;
	    }
	  else if (flag_pic)
	    {
	      rtx pic_reg = get_pic_reg();
	      emit_insn(gen_pic(to, from));
	      emit_insn(gen_addsi3(to, to, pic_reg));
	      current_function_uses_pic_offset_table = 1;
	      return 1;
	    }
	}
    }
  return 0;
}

void
spu_split_trunc_shift_asm (rtx operands[], int unsigned_p, int ashift)
{
  HOST_WIDE_INT shift;
  HOST_WIDE_INT dst_size = GET_MODE_BITSIZE (GET_MODE (operands[0]));
  int reg0, reg1;
  if (ashift)
    shift = INTVAL (operands[3]) - INTVAL (operands[2]);
  else
    shift = INTVAL (operands[2]);
  shift -=
    GET_MODE_BITSIZE (GET_MODE (operands[1])) - (dst_size <
						 32 ? 32 : dst_size);
  
  if (GET_CODE(operands[0]) == SUBREG)
    reg0 = REGNO(SUBREG_REG(operands[0]));
  else
    reg0 = REGNO(operands[0]);

  if (GET_CODE(operands[1]) == SUBREG)
    reg1 = REGNO(SUBREG_REG(operands[1]));
  else
    reg1 = REGNO(operands[1]);

  if (shift == 0)
    {
      emit_insn(gen_movv4si(gen_rtx_REG(V4SImode, reg0),
                            gen_rtx_REG(V4SImode, reg1)));
    }
  else if (shift > 0)
    {
      if (!unsigned_p)
	{
	  if (GET_MODE (operands[0]) == DImode)
	    abort ();
	  emit_insn(gen_ashrsi3(gen_rtx_REG(SImode, reg0),
				gen_rtx_REG(SImode, reg1),
				GEN_INT (shift)));
	}
      else if (GET_MODE (operands[0]) == DImode)
	{
	  if (shift > 7)
	    {
	      spu_emit_insn(gen_spu_rotqmby(gen_rtx_REG(V16QImode, reg0),
					gen_rtx_REG(V16QImode, reg1),
					GEN_INT(((-shift+7)<<56)>>59)));
	      reg1 = reg0;
	    }
	  if ((shift & 7))
	    spu_emit_insn(gen_spu_rotqmbi(gen_rtx_REG(V16QImode, reg0),
				      gen_rtx_REG(V16QImode, reg1),
				      GEN_INT(((-shift)<<61)>>61)));
	}
      else
	emit_insn(gen_lshrsi3(gen_rtx_REG(SImode, reg0),
			      gen_rtx_REG(SImode, reg1),
			      GEN_INT(shift)));
    }
  else
    {
      shift = -shift;
      if (shift > 7)
	{
	  spu_emit_insn(gen_spu_shlqby(gen_rtx_REG(V16QImode, reg0),
				   gen_rtx_REG(V16QImode, reg1),
				   GEN_INT((shift>>3)&0x1f)));
	  reg1 = reg0;
	}
      if ((shift & 7))
	spu_emit_insn(gen_spu_shlqbi(gen_rtx_REG(V16QImode, reg0),
				 gen_rtx_REG(V16QImode, reg1),
				 GEN_INT(shift&7)));
    }
}

/* Insn scheduling routines, primarily for dual issue. */
static int
spu_sched_issue_rate (void)
{
  return 2;
}

static int
uses_ls_unit(rtx insn)
{
  rtx set = single_set(insn);
  if (set != 0
      && (GET_CODE(SET_DEST(set)) == MEM
          || GET_CODE(SET_SRC(set)) == MEM))
    return 1;
  return 0;
}

static int
get_pipe(rtx insn)
{
  enum attr_type t;
  /* Handle inline asm */
  if (INSN_CODE (insn) == -1) 
    return -1;
  t = get_attr_type (insn);
  switch (t)
    {
    case TYPE_CONVERT:
      return -1;

    case TYPE_FX2:
    case TYPE_FX3:
    case TYPE_SPR:
    case TYPE_NOP:
    case TYPE_FXB:
    case TYPE_FPD:
    case TYPE_FP6:
    case TYPE_FP7:
    case TYPE_MULTI0:
    case TYPE_IPREFETCH:
      return 0;

    case TYPE_LNOP:
    case TYPE_SHUF:
    case TYPE_LOAD:
    case TYPE_STORE:
    case TYPE_BR:
    case TYPE_MULTI1:
    case TYPE_HBR:
      return 1;
    default:
      abort();
    }
}

/* This is used to keep track of insn alignment.  Set to 0 at the
 * beginning of each block and increased by the "length" attr of each
 * insn scheduled. */
static int spu_sched_length;

/* Need to keep track of length without nops because we need it to
 * schedule branch hints. */
static int spu_sched_length_no_nop;

/* The SPU needs to load the next ilb sometime during the execution of
 * the previous ilb.  There is a potential conflict if every cycle has a
 * load or store.  To avoid the conflict we make sure the load/store
 * unit is free for at least one cycle during the execution of insns in
 * the previous ilb. */
static int spu_ls_first;
static int prev_ls_clock;

/* Record when we've issued pipe0 and pipe1 insns so we can reorder the
 * ready list appropriately in spu_sched_reorder(). */
static int pipe0_clock;
static int pipe1_clock;

static int prev_clock_var;

#define iprefetch_dist 60

static void
spu_sched_init (FILE *file ATTRIBUTE_UNUSED, int verbose ATTRIBUTE_UNUSED,
		int max_ready ATTRIBUTE_UNUSED)
{
  spu_sched_length = 0;
  spu_sched_length_no_nop = 0;
  spu_ls_first = INT_MAX;
  prev_ls_clock = -1;
  pipe0_clock = -1;
  pipe1_clock = -1;
  prev_clock_var = -1;
}

static void
spu_sched_finish (FILE *file ATTRIBUTE_UNUSED, int verbose ATTRIBUTE_UNUSED)
{
}

static int
spu_sched_variable_issue (FILE *file ATTRIBUTE_UNUSED,
			  int verbose ATTRIBUTE_UNUSED, rtx insn, int more)
{
  int len;
  int p;
  if (GET_CODE (PATTERN (insn)) == USE
      || GET_CODE (PATTERN (insn)) == CLOBBER
      || (len = get_attr_length(insn)) == 0)
    return more;

  spu_sched_length += len;
  spu_sched_length_no_nop += len;

  /* Reset on inline asm */
  if (INSN_CODE (insn) == -1)
    {
      spu_ls_first = INT_MAX;
      pipe0_clock = -1;
      pipe1_clock = -1;
      return 0;
    }
  if (reload_completed)
    {
      if (clock_var - prev_ls_clock > 1)
	spu_ls_first = INT_MAX;
      if (uses_ls_unit(insn))
	{
	  if (spu_ls_first == INT_MAX)
	    spu_ls_first = spu_sched_length;
	  prev_ls_clock = clock_var;
	}

      /* The scheduler hasn't inserted the nop, but we will later on.
       * Include thos nops in spu_sched_length. */
      if (prev_clock_var == clock_var && (spu_sched_length & 7))
	spu_sched_length += 4;
      prev_clock_var = clock_var;
    }
  p = get_pipe(insn);
  if (p == 0)
    pipe0_clock = clock_var;
  else
    pipe1_clock = clock_var;
  /* Always try issueing more insns.  spu_sched_reorder will decide 
     when the cycle should be advanced. */
  return 1;
}

/* This function is called for both TARGET_SCHED_REORDER and
 * TARGET_SCHED_REORDER2.  */
static int
spu_sched_reorder (FILE *file ATTRIBUTE_UNUSED, int verbose ATTRIBUTE_UNUSED,
		   rtx *ready, int *nreadyp, int clock)
{
  int p, i, have_pipe1, pipe1_insn, nready = *nreadyp;
  rtx insn;

  if (nready <= 0 || pipe1_clock >= clock)
    return 0;

  /* Find any rtl insns that don't generate assembly insns and schedule
   * them first. */
  for (i = nready - 1; i >= 0; i--)
    {
      insn = ready[i];
      if (GET_CODE (PATTERN (insn)) == USE
	  || GET_CODE (PATTERN (insn)) == CLOBBER
	  || get_attr_length(insn) == 0)
	{
	  ready[i] = ready[nready-1];
	  ready[nready-1] = insn;
	  return 1;
	}
    }

  if (!TARGET_DUAL_NOPS
#if 0
      /* When optimizing for size only emit dual-issue nops when we
       * think it is likely to make a difference.  Ideally, we really
       * want to check if an instruction in the ready queue is on the
       * critical path and could be issued a cycle earlier by doing an
       * extra nop here. */
      || (optimize_size
	  && INSN_PRIORITY (ready[nready-1]) + clock + 2
		< current_sched_info->sched_max_insns_priority)
#endif
      )
    {
      /* When we are at an even address and we are not issueing nops to
	 improve scheduling then we need to advance the cycle.  */
      if ((spu_sched_length & 7) == 0 && prev_clock_var == clock)
	return 0;
      /* When at an odd address, we simply schedule the highest priority
         insn without considering pipeline. */ 
      if ((spu_sched_length & 7) == 4 && prev_clock_var != clock)
	return 1;
    }

  if (pipe0_clock < clock)
    {
      /* We haven't issued a pipe0 insn yet this cycle, if there is a
       * pipe0 insn in the ready list, make sure it appears first in the
       * ready list. 
       * Or if we encounter inline assembly emit it as a pipe0 insn. */
      for (i = nready - 1; i >= 0; i--)
	{
	  insn = ready[i];
	  p = get_pipe(insn);
	  if (p == 0
	      || p == -1)
	    {
	      ready[i] = ready[nready-1];
	      ready[nready-1] = insn;
	      return 1;
	    }
	}
    }
  /* Either we've scheduled a pipe0 insn already or there is no pipe0
   * insn to schedule.  Put a pipe1 insn at the front of the ready list.
   * */
  have_pipe1 = 0;
  pipe1_insn = -1;
  for (i = nready-1; i >= 0; i--)
    {
      insn = ready[i];
      p = get_pipe(insn);
      if (p == 1)
	{
	  have_pipe1 = 1;
	  /* If we have too many loads and stores in a row then keep
	   * looking for pipe1 insn that is not a load or a store. */
	  if (in_spu_reorg
	      && spu_sched_length - spu_ls_first >= iprefetch_dist
	      && uses_ls_unit(insn))
	    continue;
	  /* Make sure a hint instructions isn't too far from it's
	   * branch.  We've saved the previous offset of the hints
	   * branch in INSN_BLOCK_CYCLE.  */
	  if (in_spu_reorg
	      && (INSN_CODE (insn) == CODE_FOR_spu_hbr
	          || INSN_CODE (insn) == CODE_FOR_spu_hbrr)
	      && INSN_BLOCK_CYCLE (insn) - spu_sched_length_no_nop > 600)
	    continue;

	  pipe1_insn = i;
	  break;
	}
    }
  if (in_spu_reorg
      && pipe0_clock >= clock
      /* && (spu_sched_length & 7) == 4 */)
    {
      /* Check for the case where we can convert a pipe0 insns into
       * pipe1 insns.  Only do it if it's more profitable than using an
       * existing pipe1 insn in the ready queue. */
      rtx pat, r0, r1;
      enum machine_mode mode;
      for (i = nready-1; i > pipe1_insn; i--)
	{
	  insn = ready[i];

	  if (pipe1_insn > -1
	      && INSN_PRIORITY(ready[pipe1_insn])+1 >= INSN_PRIORITY(ready[i]))
	    break;

	  pat = PATTERN(insn);
	  mode = GET_MODE(XEXP(pat, 0));
	  if (GET_CODE(pat) == SET
	      && GET_CODE(XEXP(pat, 0)) == REG
	      && GET_CODE(XEXP(pat, 1)) == REG
	      && !RTX_FRAME_RELATED_P(insn) )
	    {
	      r0 = gen_rtx_REG(V16QImode, REGNO(XEXP(pat, 0)));
	      r1 = gen_rtx_REG(V16QImode, REGNO(XEXP(pat, 1)));
	      XEXP(pat, 0) = r0;
	      XEXP(pat, 1) = gen_rtx_UNSPEC(V16QImode,
					    gen_rtvec (2, r1, const0_rtx),
					    UNSPEC_SPU_SHLQBY);
	      INSN_CODE (insn) = recog (PATTERN (insn), insn, 0);
	      dfa_insn_code_reset (insn);
	      ready[i] = ready[nready-1];
	      ready[nready-1] = insn;
	      return 1;
	    }
	  else if (GET_CODE(pat) == SET
	      && GET_CODE(XEXP(pat, 0)) == REG
	      && (XEXP(pat, 1) == CONST0_RTX(mode)
	          || (XEXP(pat, 1) == CONSTM1_RTX(mode)
		      && (GET_MODE_CLASS (mode) == MODE_INT
			  || GET_MODE_CLASS (mode) == MODE_VECTOR_INT))))
	    {
	      rtvec v;
	      if (XEXP(pat, 1) == CONST0_RTX(mode))
		v = gen_rtvec (1, GEN_INT(0));
	      else if (XEXP(pat, 1) == CONSTM1_RTX(mode))
		v = gen_rtvec (1, GEN_INT(0xffff));
	      else 
		abort();
	      XEXP(pat, 0) = gen_rtx_REG(V16QImode, REGNO(XEXP(pat, 0)));
	      XEXP(pat, 1) = gen_rtx_UNSPEC(V16QImode, v, UNSPEC_SPU_FSMB);
	      INSN_CODE (insn) = recog (PATTERN (insn), insn, 0);
	      dfa_insn_code_reset (insn);
	      ready[i] = ready[nready-1];
	      ready[nready-1] = insn;
	      return 1;
	    }
	  else if (INSN_CODE(insn) == CODE_FOR_lshrsi3
	           && GET_CODE(XEXP(SET_SRC(pat), 1)) == CONST_INT)
	    {
	      HOST_WIDE_INT v = INTVAL(XEXP(SET_SRC(pat), 1));
	      if ((v % 8) == 0)
		{
		  r0 = gen_rtx_REG(V16QImode, REGNO(XEXP(pat, 0)));
		  r1 = gen_rtx_REG(V16QImode, REGNO(XEXP(SET_SRC(pat), 0)));
		  XEXP(pat, 0) = r0;
		  XEXP(pat, 1) = gen_rtx_UNSPEC(V16QImode,
						gen_rtvec (2, r1, GEN_INT(-v/8)),
						UNSPEC_SPU_ROTQMBY);
		  INSN_CODE (insn) = recog (PATTERN (insn), insn, 0);
		  dfa_insn_code_reset (insn);
		  ready[i] = ready[nready-1];
		  ready[nready-1] = insn;
		  return 1;
		}
	      else if ((v / 8) == 0)
		{
		  r0 = gen_rtx_REG(V16QImode, REGNO(XEXP(pat, 0)));
		  r1 = gen_rtx_REG(V16QImode, REGNO(XEXP(SET_SRC(pat), 0)));
		  XEXP(pat, 0) = r0;
		  XEXP(pat, 1) = gen_rtx_UNSPEC(V16QImode,
						gen_rtvec (2, r1, GEN_INT(-v)),
						UNSPEC_SPU_ROTQMBI);
		  INSN_CODE (insn) = recog (PATTERN (insn), insn, 0);
		  dfa_insn_code_reset (insn);
		  ready[i] = ready[nready-1];
		  ready[nready-1] = insn;
		  return 1;
		}
	    }
	}
    }
  if (pipe1_insn > -1)
    {
      insn = ready[pipe1_insn];
      ready[pipe1_insn] = ready[nready-1];
      ready[nready-1] = insn;
      return 1;
    }
  if (in_spu_reorg
      && spu_sched_length - spu_ls_first >= iprefetch_dist
      && have_pipe1
      && (pipe0_clock < clock || (spu_sched_length & 7) == 4))
    {
      insn = sched_emit_insn(gen_iprefetch());
      recog_memoized(insn);
      if (pipe0_clock < clock_var)
	PUT_MODE (insn, TImode);
      spu_sched_length += 4;
      spu_ls_first = INT_MAX;
    }
  return 0;
}

static int
spu_sched_can_schedule (rtx insn)
{
  /* Make sure a hint instructions isn't too far from it's branch.
   * We've saved the previous offset of the hints branch in
   * INSN_BLOCK_CYCLE.  That offset was determined before inserting
   * nops. */
  if (in_spu_reorg
      && (INSN_CODE (insn) == CODE_FOR_spu_hbr
	  || INSN_CODE (insn) == CODE_FOR_spu_hbrr)
      && INSN_BLOCK_CYCLE (insn) - spu_sched_length_no_nop  > 600)
    return 0;
  return 1;
}


/* INSN is dependent on DEP_INSN. */
static int
spu_sched_adjust_cost (rtx insn, rtx link ATTRIBUTE_UNUSED,
		       rtx dep_insn ATTRIBUTE_UNUSED, int cost)
{
  if (GET_CODE(insn) == CALL_INSN)
    return cost - 2;
  /* The dfa scheduler sets cost to 0 for all anti-dependencies and the
   * scheduler makes every insn in a block anti-dependent on the final
   * jump_insn.  We adjust here so higher cost insns will get scheduled
   * earlier. */
  if (GET_CODE(insn) == JUMP_INSN
      && REG_NOTE_KIND (link) == REG_DEP_ANTI)
    return INSN_COST(dep_insn) - 3;
  return cost;
}

static int
spu_sched_adjust_priority (
  rtx insn ATTRIBUTE_UNUSED,
  int priority)
{
  return priority;
}

/* Create a CONST_DOUBLE from a string.  */

struct rtx_def *
spu_float_const (
     const char *string,
     enum machine_mode mode)
{
  REAL_VALUE_TYPE value;
  value = REAL_VALUE_ATOF (string, mode);
  return CONST_DOUBLE_FROM_REAL_VALUE (value, mode);
}

int
spu_constant_address_p(rtx x)
{
  return (GET_CODE (x) == CONST_INT && INTVAL(x) >= -0x20000ll && INTVAL(x) <= 0x3ffffll)
         || GET_CODE (x) == LABEL_REF
	 || (!TARGET_LARGE_MEM
	     && (GET_CODE (x) == SYMBOL_REF
		 || GET_CODE (x) == CONST));
}

int
spu_legitimate_constant_p(rtx x)
{
  if (x == CONST0_RTX(GET_MODE(x)))
    return 1;

  if (GET_CODE(x) == CONST_DOUBLE)
    return GET_MODE(x) == SFmode
      || (GET_MODE (x) == VOIDmode && spu_legitimate_ti_const_p (x));

  if (GET_CODE(x) == CONST_INT)
    return INTVAL(x) >= -0x80000000ll && INTVAL(x) <= 0xffffffffll;

  return 1;
}

int
spu_legitimate_ti_const_p(rtx x)
{
  unsigned char arr[16];
  int i, j;
  constant_to_array(TImode, x, arr);

  for (i = 4; i < 16; i += 4)
    for (j = 0; j < 4; j++)
      if (arr[j] != arr[i+j])
	return 0;
  return 1;
}

int
spu_legitimate_address (
     enum machine_mode mode ATTRIBUTE_UNUSED,
     rtx x,
     int reg_ok_strict)
{
  if (CONSTANT_ADDRESS_P (x))
    return 1;
  if (GET_CODE (x) == SUBREG)
    x = XEXP (x, 0);
  if (GET_CODE (x) == REG && INT_REG_OK_FOR_BASE_P (x, reg_ok_strict))
    return 1;
  if (GET_CODE (x) == PLUS || GET_CODE (x) == LO_SUM)
    {
      rtx op0 = XEXP (x, 0);
      rtx op1 = XEXP (x, 1);
      if (GET_CODE (op0) == SUBREG)
	op0 = XEXP (op0, 0);
      if (GET_CODE (op1) == SUBREG)
	op1 = XEXP (op1, 0);
      /* We can't just accept any aligned register because CSE can
       * change it to a register that is not marked aligned and then
       * recog will fail.   So we only accept frame registers because
       * they will only be changed to other frame registers. */
      if (GET_CODE (op0) == REG
	  && INT_REG_OK_FOR_BASE_P (op0, reg_ok_strict)
	  && GET_CODE (op1) == CONST_INT
	  && CONST_OK_FOR_LETTER_P (INTVAL (op1), 'L')
	  && (regno_aligned_for_load (REGNO (op0))
	      || (INTVAL (op1) & 15) == 0))
	return 1;
      if (GET_CODE (op0) == REG
	  && INT_REG_OK_FOR_BASE_P (op0, reg_ok_strict)
	  && GET_CODE (op1) == REG
	  && INT_REG_OK_FOR_INDEX_P (op1, reg_ok_strict))
	return 1;
    }
  return 0;
}

rtx
spu_legitimize_address (rtx x, rtx oldx ATTRIBUTE_UNUSED,
			   enum machine_mode mode)
{
  rtx op0, op1;
  /* Make sure both operands are registers.  */
  if (GET_CODE (x) == PLUS)
    {
      op0 = XEXP (x, 0);
      op1 = XEXP (x, 1);
      if (ALIGNED_SYMBOL_REF_P(op0))
	{
	  op0 = force_reg (Pmode, op0);
	  mark_reg_pointer (op0, 128);
	}
      else if (GET_CODE (op0) != REG)
	op0 = force_reg (Pmode, op0);
      if (ALIGNED_SYMBOL_REF_P(op1))
	{
	  op1 = force_reg (Pmode, op1);
	  mark_reg_pointer (op1, 128);
	}
      else if (GET_CODE (op1) != REG)
	op1 = force_reg (Pmode, op1);
      x = gen_rtx_PLUS (Pmode, op0, op1);
      if (spu_legitimate_address (mode, x, 0))
	return x;
    }
  return NULL_RTX;
}

/* Handle an attribute requiring a FUNCTION_DECL; arguments as in
   struct attribute_spec.handler.  */
static tree
spu_handle_fndecl_attribute (
     tree *node,
     tree name,
     tree args ATTRIBUTE_UNUSED,
     int flags ATTRIBUTE_UNUSED,
     bool *no_add_attrs)
{
  if (TREE_CODE (*node) != FUNCTION_DECL)
    {
      warning ("`%s' attribute only applies to functions",
	       IDENTIFIER_POINTER (name));
      *no_add_attrs = true;
    }

  return NULL_TREE;
}

/* Handle the "vector" attribute.
 */

#define SPU_VECTOR_MODE(MODE)	\
	 ((MODE) == V16QImode		\
	  || (MODE) == V8HImode		\
	  || (MODE) == V4SFmode		\
	  || (MODE) == V4SImode		\
	  || (MODE) == V2DFmode		\
	  || (MODE) == V2DImode)

static tree
spu_handle_vector_attribute (tree *node, tree name, tree args ATTRIBUTE_UNUSED,
			     int flags ATTRIBUTE_UNUSED, bool *no_add_attrs)
{
  tree type = *node, result = NULL_TREE;
  enum machine_mode mode;
  int unsigned_p;
  int tree_index = 0;

  while (POINTER_TYPE_P (type)
	 || TREE_CODE (type) == FUNCTION_TYPE
	 || TREE_CODE (type) == METHOD_TYPE
	 || TREE_CODE (type) == ARRAY_TYPE)
    type = TREE_TYPE (type);

  mode = TYPE_MODE (type);

  unsigned_p = TYPE_UNSIGNED (type);
  switch (mode)
    {
    case DImode:
      tree_index = (unsigned_p ?  SPU_TI_UV2DI_TYPE : SPU_TI_V2DI_TYPE);
      break;

    case SImode:
      tree_index = (unsigned_p ?  SPU_TI_UV4SI_TYPE : SPU_TI_V4SI_TYPE);
      break;

    case HImode:
      tree_index = (unsigned_p ?  SPU_TI_UV8HI_TYPE : SPU_TI_V8HI_TYPE);
      break;

    case QImode:
      tree_index = (unsigned_p ?  SPU_TI_UV16QI_TYPE : SPU_TI_V16QI_TYPE);
      break;

    case SFmode:
      tree_index = SPU_TI_V4SF_TYPE;
      break;

    case DFmode:
      tree_index = SPU_TI_V2DF_TYPE;
      break;

    default: break;
    }

  if (tree_index)
    result = spu_get_type_node_by_tree_index (tree_index);

  /* APPLE LOCAL begin AltiVec */
  /* Propagate qualifiers attached to the element type
     onto the vector type.  */
  if (result && result != type && TYPE_QUALS (type))
    result = build_qualified_type (result, TYPE_QUALS (type));
  /* APPLE LOCAL end AltiVec */

  *no_add_attrs = true;  /* No need to hang on to the attribute.  */

  if (!result)
    warning ("`%s' attribute ignored", IDENTIFIER_POINTER (name));
  else
    *node = reconstruct_complex_type (*node, result);

  return NULL_TREE;
}

/* Return non-zero if FUNC is a naked function.  */

static int
spu_naked_function_p (tree func)
{
  tree a;

  if (TREE_CODE (func) != FUNCTION_DECL)
    abort ();
  
  a = lookup_attribute ("naked", DECL_ATTRIBUTES (func));
  return a != NULL_TREE;
}

int
spu_initial_elimination_offset(int from, int to)
{
  int saved_regs_size = spu_saved_regs_size ();
  int sp_offset = 0;
  if (!current_function_is_leaf || current_function_outgoing_args_size
      || get_frame_size() || saved_regs_size)
    sp_offset = STACK_POINTER_OFFSET;
  if (from == FRAME_POINTER_REGNUM && to == STACK_POINTER_REGNUM)
    return (sp_offset + current_function_outgoing_args_size);
  else if (from == FRAME_POINTER_REGNUM && to == HARD_FRAME_POINTER_REGNUM)
    return 0;
  else if (from == ARG_POINTER_REGNUM && to == STACK_POINTER_REGNUM)
    return sp_offset + current_function_outgoing_args_size 
           + get_frame_size() + saved_regs_size
           + STACK_POINTER_OFFSET; 
  else if (from == ARG_POINTER_REGNUM && to == HARD_FRAME_POINTER_REGNUM)
    return get_frame_size() + saved_regs_size
           + sp_offset; 
  return 0;
}


static void
spu_record_to_rtvec(tree type, int first_regno, int nregs, rtvec vec, int first_index)
{
  tree field;
  int index = 0;
  HOST_WIDE_INT offset = 0;
  for (field = TYPE_FIELDS (type); field && index < nregs; field = TREE_CHAIN (field)) {
    /* Static member is represented as var_decl and should not be counted as
     * occupying the record space
     */
    if (TREE_CODE (field) == VAR_DECL)
        continue;
    if (DECL_SIZE (field) && DECL_FIELD_OFFSET (field))
      {
	tree field_type = TREE_TYPE (field);
	HOST_WIDE_INT field_size = tree_low_cst (DECL_SIZE (field), 1);
	HOST_WIDE_INT field_offset = tree_low_cst (DECL_FIELD_OFFSET (field), 1) * BITS_PER_UNIT;
	enum machine_mode field_mode = TYPE_MODE (field_type);
	if (DECL_FIELD_BIT_OFFSET (field))
	  field_offset += tree_low_cst (DECL_FIELD_BIT_OFFSET (field), 1);
	while (field_offset > offset && index < nregs)
	  {
	    RTVEC_ELT (vec, first_index + index) = gen_rtx_EXPR_LIST (VOIDmode,
					gen_rtx_REG (TImode, first_regno + index),
					GEN_INT(UNITS_PER_WORD * (first_index + index)));
	    index++;
	    offset += BITS_PER_WORD;
	  }
	if (field_offset == offset && field_size == BITS_PER_WORD
	    && field_mode != VOIDmode
	    && field_mode != BLKmode
	    && (first_regno + index >= FIRST_PSEUDO_REGISTER
		|| HARD_REGNO_MODE_OK (first_regno + index, field_mode)))
	  {
	    RTVEC_ELT (vec, first_index + index) = gen_rtx_EXPR_LIST (VOIDmode,
					gen_rtx_REG (field_mode, first_regno + index),
					GEN_INT(UNITS_PER_WORD * (first_index + index)));
	    index++;
	    offset += BITS_PER_WORD;
	  }
        else if (field_offset == offset && TREE_CODE (field_type) == ARRAY_TYPE)
	  {
	    enum machine_mode elem_mode = TYPE_MODE (TREE_TYPE (field_type));
	    if (GET_MODE_BITSIZE (elem_mode) == BITS_PER_WORD)
	      {
		int nelem = field_size / BITS_PER_WORD;
		while (nelem > 0)
		  {
		    RTVEC_ELT (vec, first_index + index) = gen_rtx_EXPR_LIST (VOIDmode,
						gen_rtx_REG (elem_mode, first_regno + index),
						GEN_INT(UNITS_PER_WORD * (first_index + index)));
		    index++;
		    offset += BITS_PER_WORD;
		    nelem--;
		  }
	      }
	  }
	else if (field_offset == offset && TREE_CODE (field_type) == RECORD_TYPE
	         && field_size >= BITS_PER_WORD)
	  {
	    int field_nregs = MIN (field_size / BITS_PER_WORD, nregs - index);
	    spu_record_to_rtvec(field_type, first_regno + index, field_nregs,
				vec, (first_index + index));
	    index += field_nregs;
	    offset += field_nregs * BITS_PER_WORD;
	  }
      }
  }
  while (index < nregs)
    {
      RTVEC_ELT (vec, first_index + index) = gen_rtx_EXPR_LIST (VOIDmode,
				  gen_rtx_REG (TImode, first_regno + index),
				  GEN_INT(UNITS_PER_WORD * (first_index + index)));
      index++;
    }
  if (index > nregs)
    abort();
}

rtx
spu_function_value(tree type, tree func ATTRIBUTE_UNUSED)
{
  enum machine_mode mode = TYPE_MODE (type);
  int byte_size = ((mode == BLKmode)
                  ? int_size_in_bytes (type) : GET_MODE_SIZE (mode));

  /* Make sure small structs are left justified in a register. */
  if ((mode == BLKmode || (type && AGGREGATE_TYPE_P (type)))
    && byte_size <= UNITS_PER_WORD * MAX_REGISTER_RETURN
    && byte_size > 0)
    {
      enum machine_mode smode;
      rtvec v;
      int i;
      int nregs = (byte_size + UNITS_PER_WORD - 1) / UNITS_PER_WORD;
      int n = byte_size / UNITS_PER_WORD;
      v = rtvec_alloc(nregs);
      if (flag_copy_by_field && byte_size > UNITS_PER_WORD && type && TREE_CODE (type) == RECORD_TYPE)
	{
	  spu_record_to_rtvec(type, FIRST_RETURN_REGNUM, n, v, 0);
	  byte_size -= n * UNITS_PER_WORD;
	}
      else
	{
	  for (i = 0; i < n; i++)
	    {
	      RTVEC_ELT (v, i) = gen_rtx_EXPR_LIST (VOIDmode,
					  gen_rtx_REG (TImode, FIRST_RETURN_REGNUM + i),
					  GEN_INT(UNITS_PER_WORD * i));
	      byte_size -= UNITS_PER_WORD;
	    }
	}

      if (n < nregs)
	{
	  if (byte_size < 4) byte_size = 4;
	  smode = smallest_mode_for_size(byte_size * BITS_PER_UNIT, MODE_INT);
	  RTVEC_ELT (v, n) = gen_rtx_EXPR_LIST (VOIDmode,
				      gen_rtx_REG (smode, FIRST_RETURN_REGNUM + n),
				      GEN_INT(UNITS_PER_WORD * n));
	}
      return gen_rtx_PARALLEL (mode, v);
    }
  return gen_rtx_REG (mode, FIRST_RETURN_REGNUM);
}

rtx
spu_function_arg (CUMULATIVE_ARGS cum,
		  enum machine_mode mode,
		  tree type, int named ATTRIBUTE_UNUSED)
{
  int byte_size;

  if (cum >= MAX_REGISTER_ARGS)
    return 0;

  byte_size = ((mode == BLKmode)
              ? int_size_in_bytes (type) : GET_MODE_SIZE (mode));

  /* The ABI does not allow parameters to be passed partially in
   * reg and partially in stack. */
  if ((cum + (byte_size + 15) / 16) > MAX_REGISTER_ARGS)
    return 0;

  /* Make sure small structs are left justified in a register. */
  if ((mode == BLKmode || (type && AGGREGATE_TYPE_P (type)))
    && byte_size < UNITS_PER_WORD
    && byte_size > 0)
    {
      enum machine_mode smode;
      rtx gr_reg;
      if (byte_size < 4) byte_size = 4;
      smode = smallest_mode_for_size(byte_size * BITS_PER_UNIT, MODE_INT);
      gr_reg = gen_rtx_EXPR_LIST (VOIDmode,
				  gen_rtx_REG (smode, FIRST_ARG_REGNUM + cum),
				  const0_rtx);
      return gen_rtx_PARALLEL (mode, gen_rtvec (1, gr_reg));
    }
  else
    return gen_rtx_REG (mode, FIRST_ARG_REGNUM + cum);
}

/* Variable sized types are passed by reference.  */
static bool
spu_pass_by_reference (CUMULATIVE_ARGS *cum ATTRIBUTE_UNUSED,
		       enum machine_mode mode ATTRIBUTE_UNUSED,
		       tree type, bool named ATTRIBUTE_UNUSED)
{
  return type && TREE_CODE (TYPE_SIZE (type)) != INTEGER_CST;
}


/* Var args. */

/* Create and return the va_list datatype.

   On SPU, va_list is an array type equivalent to

      typedef struct __va_list_tag
        {
            void *__args __attribute__((__aligned(16)));
            void *__skip __attribute__((__aligned(16)));
            
        } va_list[1];

   wheare __args points to the arg that will be returned by the next
   va_arg(), and __skip points to the previous stack frame such that
   when __args == __skip we should advance __args by 32 bytes. */
static tree
spu_build_builtin_va_list (void)
{
  tree f_args, f_skip, record, type_decl;
  bool owp;


  record = (*lang_hooks.types.make_type) (RECORD_TYPE);

  type_decl =
    build_decl (TYPE_DECL, get_identifier ("__va_list_tag"), record);

  f_args = build_decl (FIELD_DECL, get_identifier ("__args"),
		      ptr_type_node);
  f_skip = build_decl (FIELD_DECL, get_identifier ("__skip"),
		      ptr_type_node);

  DECL_FIELD_CONTEXT (f_args) = record;
  DECL_ALIGN (f_args) = 128;
  DECL_USER_ALIGN (f_args) = 1;

  DECL_FIELD_CONTEXT (f_skip) = record;
  DECL_ALIGN (f_skip) = 128;
  DECL_USER_ALIGN (f_skip) = 1;

  TREE_CHAIN (record) = type_decl;
  TYPE_NAME (record) = type_decl;
  TYPE_FIELDS (record) = f_args;
  TREE_CHAIN (f_args) = f_skip;

  /* We know this is being padded and we want it too.  It is an internal
     type so hide the warnings from the user. */
  owp = warn_padded;
  warn_padded = false;

  layout_type (record);

  warn_padded = owp;

  /* The correct type is an array type of one element.  */
  return build_array_type (record, build_index_type (size_zero_node));
}

/* Implement va_start by filling the va_list structure VALIST.
   NEXTARG points to the first anonymous stack argument.

   The following global variables are used to initialize
   the va_list structure:

     current_function_args_info;
       the CUMULATIVE_ARGS for this function

     current_function_arg_offset_rtx:
       holds the offset of the first anonymous stack argument
       (relative to the virtual arg pointer).  */

void
spu_va_start (tree valist, rtx nextarg)
{
  tree f_args, f_skip;
  tree args, skip, t;

  f_args = TYPE_FIELDS (TREE_TYPE (va_list_type_node));
  f_skip = TREE_CHAIN (f_args);

  valist = build1 (INDIRECT_REF, TREE_TYPE (TREE_TYPE (valist)), valist);
  args = build (COMPONENT_REF, TREE_TYPE (f_args), valist, f_args, NULL_TREE);
  skip = build (COMPONENT_REF, TREE_TYPE (f_skip), valist, f_skip, NULL_TREE);

  /* Find the __args area.  */
  t = make_tree (TREE_TYPE (args), nextarg);
  if (current_function_pretend_args_size > 0)
    t = build (PLUS_EXPR, TREE_TYPE (args), t,
	       build_int_cst (integer_type_node, -STACK_POINTER_OFFSET));
  t = build (MODIFY_EXPR, TREE_TYPE (args), args, t);
  TREE_SIDE_EFFECTS (t) = 1;
  expand_expr (t, const0_rtx, VOIDmode, EXPAND_NORMAL);

  /* Find the __skip area.  */
  t = make_tree (TREE_TYPE (skip), virtual_incoming_args_rtx);
  t = build (PLUS_EXPR, TREE_TYPE (skip), t,
	     build_int_cst (integer_type_node, 
			    (current_function_pretend_args_size
			     - STACK_POINTER_OFFSET)));
  t = build (MODIFY_EXPR, TREE_TYPE (skip), skip, t);
  TREE_SIDE_EFFECTS (t) = 1;
  expand_expr (t, const0_rtx, VOIDmode, EXPAND_NORMAL);
}

/* Gimplify va_arg by updating the va_list structure 
   VALIST as required to retrieve an argument of type
   TYPE, and returning that argument. 
   
   ret = va_arg(VALIST, TYPE);

   generates code equivalent to:
   
    paddedsize = (sizeof(TYPE) + 15) & -16;
    if (VALIST.__args + paddedsize > VALIST.__skip
	&& VALIST.__args <= VALIST.__skip)
      addr = VALIST.__skip + 32;
    else
      addr = VALIST.__args;
    VALIST.__args = addr + paddedsize;
    ret = *(TYPE *)addr;

   */

static tree
spu_gimplify_va_arg_expr (tree valist, tree type, tree *pre_p,
			  tree *post_p ATTRIBUTE_UNUSED)
{
  tree f_args, f_skip;
  tree args, skip;
  HOST_WIDE_INT size, rsize;
  tree paddedsize, addr, tmp;
  bool pass_by_reference_p;

  f_args = TYPE_FIELDS (TREE_TYPE (va_list_type_node));
  f_skip = TREE_CHAIN (f_args);

  valist = build1 (INDIRECT_REF, TREE_TYPE (TREE_TYPE (valist)), valist);
  args = build (COMPONENT_REF, TREE_TYPE (f_args), valist, f_args, NULL_TREE);
  skip = build (COMPONENT_REF, TREE_TYPE (f_skip), valist, f_skip, NULL_TREE);

  addr = create_tmp_var(ptr_type_node, "va_arg");
  DECL_POINTER_ALIAS_SET (addr) = get_varargs_alias_set ();

  /* if an object is dynamically sized, a pointer to it is passed
     instead of the object itself. */
  pass_by_reference_p = spu_pass_by_reference (NULL, TYPE_MODE (type), type,
					       false);
  if (pass_by_reference_p)
    type = build_pointer_type (type);
  size = int_size_in_bytes (type);
  rsize = ((size + UNITS_PER_WORD - 1) / UNITS_PER_WORD) * UNITS_PER_WORD;

  /* build conditional expression to calculate addr. The expression
     will be gimplified later. */
  paddedsize = fold_convert (ptr_type_node, size_int (rsize));
  tmp = build2 (PLUS_EXPR, ptr_type_node, args, paddedsize);
  tmp = build2 (TRUTH_AND_EXPR, boolean_type_node,
		 build2 (GT_EXPR, boolean_type_node, tmp, skip),
		 build2 (LE_EXPR, boolean_type_node, args, skip));

  tmp = build3 (COND_EXPR, ptr_type_node, tmp,
		build2 (PLUS_EXPR, ptr_type_node, skip,
			fold_convert (ptr_type_node, size_int(32))),
		args);

  tmp = build (MODIFY_EXPR, ptr_type_node, addr, tmp);
  gimplify_and_add(tmp, pre_p);

  /* update VALIST.__args */
  tmp = build2 (PLUS_EXPR, ptr_type_node, addr, paddedsize);
  tmp = build2 (MODIFY_EXPR, TREE_TYPE(args), args, tmp);
  gimplify_and_add(tmp, pre_p);
 
  addr = fold_convert (build_pointer_type (type), addr);
  
  if (pass_by_reference_p)
    addr = build_va_arg_indirect_ref (addr);

  return build_va_arg_indirect_ref (addr);
}

/* Save parameter registers starting with the register that corresponds
 * to the first unnamed parameters.  If the first unnamed parameter is
 * in the stack then save no registers.  Set pretend_args_size to the
 * amount of space needed to save the registers. */
void
spu_setup_incoming_varargs (CUMULATIVE_ARGS *cum, enum machine_mode mode,
                            tree type, int *pretend_size, int no_rtl)
{

  if (! no_rtl)
    {
      rtx tmp;
      int set = get_varargs_alias_set ();
      int regno;
      int offset;
      int ncum = *cum;

      /* cum currently points to the last named argument, we want to
       * start at the next argument. */
      FUNCTION_ARG_ADVANCE(ncum, mode, type, 1);

      offset = -STACK_POINTER_OFFSET;
      for (regno = ncum; regno < MAX_REGISTER_ARGS; regno++)
	{
	  tmp = gen_rtx_MEM (V4SImode,
			     plus_constant (virtual_incoming_args_rtx, offset));
	  set_mem_alias_set(tmp, set);
	  emit_move_insn(tmp, gen_rtx_REG (V4SImode, FIRST_ARG_REGNUM + regno));
	  offset += 16;
	}
      *pretend_size = offset + STACK_POINTER_OFFSET;
    }
}

void
spu_conditional_register_usage(void)
{
  if (flag_pic)
    {
      fixed_regs[PIC_OFFSET_TABLE_REGNUM] = 1;
      call_used_regs[PIC_OFFSET_TABLE_REGNUM] = 1;
    }
}

/* This is called to decide when we can simplify a load instruction.  We
   must only return true for registers which we know will always be
   aligned.  Taking into account that CSE might replace this reg with
   another one that has not been marked aligned.  
   So this is really only true for frame, stack and virtual registers,
   which we know are always aligned and should not be adversly effected
   by CSE. */
static int 
regno_aligned_for_load (int regno)
{
  return regno == FRAME_POINTER_REGNUM
	 || (frame_pointer_needed && regno == HARD_FRAME_POINTER_REGNUM)
	 || regno == STACK_POINTER_REGNUM
	 || (regno >= FIRST_VIRTUAL_REGISTER
	     && regno <= LAST_VIRTUAL_REGISTER);
}

static int
aligned_mem(rtx mem)
{
  if (MEM_ALIGN(mem) >= 128)
    return 1;
  if (GET_CODE(XEXP(mem,0)) == PLUS)
    {
      rtx p0 = XEXP(XEXP(mem, 0), 0);
      rtx p1 = XEXP(XEXP(mem, 0), 1);
      if (regno_aligned_for_load (REGNO (p0)))
	{
	  if (GET_CODE(p1) == REG
	      && regno_aligned_for_load (REGNO (p1)))
	    return 1;
	  if (GET_CODE(p1) == CONST_INT && (INTVAL(p1) & 15) == 0)
	    return 1;
	}
    }
  else if (GET_CODE(XEXP(mem,0)) == REG)
    {
      if (regno_aligned_for_load (REGNO (XEXP (mem,0))))
	return 1;
    }
  else if (ALIGNED_SYMBOL_REF_P (XEXP(mem,0)))
    return 1;
  else if (GET_CODE(XEXP(mem,0)) == CONST)
    {
      rtx p0 = XEXP(XEXP(XEXP(mem, 0), 0), 0);
      rtx p1 = XEXP(XEXP(XEXP(mem, 0), 0), 1);
      if (GET_CODE(p0) == SYMBOL_REF
          && GET_CODE(p1) == CONST_INT
	  && (INTVAL(p1) & 15) == 0)
	return 1;
    }
  return 0;
}

/* Return TRUE if we are certain the mem refers to a scalar var.
   This means it is safe to load and store from this var with a single
   instructions. 
   It is not safe to use MEM_SCALAR_P because it is set even when we use
   a smaller mode to reference a part of a larger mode. */
static int
is_scalar_var(rtx mem)
{
  rtx sym;
  tree decl;
  sym = XEXP (mem, 0);
  if (GET_CODE (sym) == SYMBOL_REF
      && !SYMBOL_REF_FUNCTION_P (sym)
      && SYMBOL_REF_DECL (sym))
    {
      decl = SYMBOL_REF_DECL (sym); 
      if (TREE_CODE (decl) == VAR_DECL
	  && TYPE_MODE (TREE_TYPE (decl)) == GET_MODE (mem))
	return 1;
    }
  return 0;
}


int
spu_expand_mov(rtx *ops, enum machine_mode mode)
{
  /* At least one of the operands needs to be a register. */
  if ((reload_in_progress | reload_completed) == 0
      && !register_operand (ops[0], mode)
      && !register_operand (ops[1], mode))
    {
      rtx temp = force_reg (mode, ops[1]);
      emit_move_insn (ops[0], temp);
      return 1;
    }
  if (reload_in_progress || reload_completed)
    {
      if ((mode == TImode
	  && GET_CODE (ops[1]) == CONST_INT
	  && INTVAL (ops[1]) < -1ll)
	  || (GET_CODE (ops[1]) == CONST_VECTOR
	      && !vec_imm_operand (ops[1], mode, 0)))
	{
	  rtx mem = force_const_mem (mode, ops[1]);
	  if (TARGET_LARGE_MEM)
	    {
	      rtx addr = gen_rtx_REG (Pmode, REGNO (ops[0]));
	      emit_move_insn (addr, XEXP (mem, 0));
	      mem = replace_equiv_address (mem, addr);
	    }
	  emit_move_insn (ops[0], mem);
	  return 1;
	}

      /* Assume that load's and stores are a result of spills, in which
       * case we just save the whole register and restore it later.
       * We assume that any other move matches it's constraints too. */
      return 0;
    }
  else
    {
      if (GET_CODE(ops[0]) == MEM)
	{
	  if (!spu_valid_move(ops))
	    {
	      emit_insn(gen_store(ops[0], ops[1], gen_reg_rtx(V16QImode),
						  gen_reg_rtx(V16QImode)));
	      return 1;
	    }
	}
      else if (GET_CODE(ops[1]) == MEM)
	{
	  if (!spu_valid_move(ops))
	    {
	      emit_insn(gen_load(ops[0], ops[1], gen_reg_rtx(V16QImode), gen_reg_rtx(SImode)));
	      return 1;
	    }
	}
      /* Catch the SImode immediates greater than 0x7fffffff, and sign
       * extend them. */
      if (GET_CODE(ops[1]) == CONST_INT)
	{
	  HOST_WIDE_INT val = trunc_int_for_mode(INTVAL(ops[1]), mode);
	  if (val != INTVAL(ops[1]))
	    {
	      emit_move_insn(ops[0], GEN_INT(val));
	      return 1;
	    }
	}
    }
  return 0;
}

void
spu_split_load(rtx *ops)
{
  enum machine_mode mode = GET_MODE(ops[0]);
  rtx addr, load, rot, insn;
  int rot_amt;

  addr = XEXP(ops[1],0);

  rot = 0;
  rot_amt = 0;
  if (MEM_ALIGN(ops[1]) < 128 || GET_MODE_SIZE(mode) < 4)
    {
      if (GET_CODE(addr) == PLUS)
	{
	  if (GET_CODE(XEXP(addr, 0)) == REG
	      && REG_ALIGN(XEXP(addr, 0)) < 128)
	    rot = XEXP(addr, 0);
	  if (GET_CODE(XEXP(addr, 1)) == REG
	      && REG_ALIGN(XEXP(addr, 1)) < 128)
	    {
	      if (rot)
		{
		  emit_insn(gen_addsi3(ops[3], rot, XEXP(addr, 1)));
		  rot = ops[3];
		}
	      else
		rot = XEXP(addr, 1);
	    }
	  else if (GET_CODE(XEXP(addr, 1)) == CONST_INT)
	    rot_amt = INTVAL(XEXP(addr, 1));
	  if (GET_CODE(XEXP(addr, 0)) == REG
	      && REG_ALIGN(XEXP(addr, 0)) >= 128
	      && GET_CODE(XEXP(addr, 1)) == CONST_INT)
	    {
	      if (rot_amt & -16)
		addr = gen_rtx_PLUS (Pmode,
				     XEXP(addr, 0), 
				     GEN_INT (rot_amt & -16));
	      else
		addr = XEXP(addr, 0);
	    }
	}
      else if (GET_CODE(addr) == REG)
	{
	  if (REG_ALIGN(addr) < 128)
	    rot = addr;
	}
      else if (GET_CODE(addr) == CONST)
	{
	  if (GET_CODE(XEXP(addr, 0)) == PLUS
	     && ALIGNED_SYMBOL_REF_P (XEXP(XEXP(addr, 0), 0))
	     && GET_CODE(XEXP(XEXP(addr, 0), 1)) == CONST_INT)
	    {
	      rot_amt = INTVAL(XEXP(XEXP(addr, 0), 1));
	      if (rot_amt & -16)
		addr = gen_rtx_CONST (Pmode,
				      gen_rtx_PLUS (Pmode,
						    XEXP(XEXP(addr, 0), 0),
						    GEN_INT (rot_amt & -16)));
	      else
		addr = XEXP(XEXP(addr, 0), 0);
	    }
	  else
	    rot = addr;
	}
      else if (GET_CODE(addr) == CONST_INT)
	{
	  rot_amt = INTVAL(addr);
	  addr = GEN_INT (rot_amt & -16);
	}
      else if (! ALIGNED_SYMBOL_REF_P (addr))
	rot = addr;
    }

  if (GET_MODE_SIZE(mode) < 4)
    rot_amt += GET_MODE_SIZE(mode)-4;

  rot_amt &= 15;

  if (rot && rot_amt)
    {
      emit_insn(gen_addsi3(ops[3], rot, GEN_INT(rot_amt)));
      rot = ops[3];
      rot_amt = 0;
    }

  load = ops[2];

  if (GET_CODE(addr) == PLUS && GET_CODE (XEXP(addr,1)) != REG)
    insn = spu_emit_insn(gen_spu_lqd(load, XEXP(addr,0), XEXP(addr,1)));
  else if (GET_CODE(addr) == PLUS && GET_CODE (XEXP(addr,1)) == REG)
    insn = spu_emit_insn(gen_spu_lqx(load, XEXP(addr,0), XEXP(addr,1)));
  else if (GET_CODE(addr) == REG)
    insn = spu_emit_insn(gen_spu_lqd(load, addr, const0_rtx));
  else if (flag_pic && GET_CODE(addr) != CONST_INT)
    insn = spu_emit_insn(gen_spu_lqr(load, addr));
  else
    insn = spu_emit_insn(gen_spu_lqa(load, addr));

  if (GET_CODE(PATTERN(insn)) == SET)
    {
      rtx mem = SET_SRC(PATTERN(insn));
      set_mem_align(mem, 128);
      set_mem_size(mem, GEN_INT(16));
      /*if (is_scalar_var (ops[1])) */
	set_mem_alias_set (mem, MEM_ALIAS_SET (ops[1]));
      RTX_FLAG (mem, in_struct) = RTX_FLAG (ops[1], in_struct);
      RTX_FLAG (mem, volatil) = RTX_FLAG (ops[1], volatil);
      /* FIXME - bccheng - integrated is gone in gcc 4.0.0 
      RTX_FLAG (mem, integrated) = RTX_FLAG (ops[1], integrated);
       */
      RTX_FLAG (mem, frame_related) = RTX_FLAG (ops[1], frame_related);
    }

    if (rot)
      spu_emit_insn (gen_spu_rotqby (ops[0], load, rot));
    else if (rot_amt)
      spu_emit_insn (gen_spu_rotqby (ops[0], load, GEN_INT(rot_amt)));
    else if (reload_completed)
      emit_move_insn (ops[0], gen_rtx_REG(GET_MODE(ops[0]), REGNO(load)));
    else
      spu_emit_insn(gen_spu_convert(ops[0], load));

}

void
spu_split_store(rtx *ops)
{
  enum machine_mode mode = GET_MODE(ops[0]);
  rtx pat = ops[2];
  rtx reg = ops[3];
  rtx addr, p0, p1, p1_lo, store;
  int aform;
  int scalar;

  addr = XEXP(ops[0],0);

  if (GET_CODE(addr) == PLUS)
    {
      aform = 0;
      p0 = XEXP(addr, 0);
      p1 = p1_lo = XEXP(addr, 1);
      if (GET_CODE (p0) == REG && REG_ALIGN (p0) >= 128
	  && GET_CODE (p1) == CONST_INT)
	{
	  p1_lo = GEN_INT (INTVAL (p1) & 15);
	  p1 = GEN_INT (INTVAL (p1) & -16);
	}
    }
  else if (GET_CODE(addr) == REG)
    {
      aform = 0;
      p0 = addr;
      p1 = p1_lo = const0_rtx;
    }
  else
    {
      aform = 1;
      p0 = gen_rtx_REG(SImode, STACK_POINTER_REGNUM);
      p1 = 0;  /* aform doesn't use p1 */
      p1_lo = addr;
      if (ALIGNED_SYMBOL_REF_P (addr))
	p1_lo = const0_rtx;
      else if (GET_CODE(addr) == CONST)
	{
	  if (GET_CODE(XEXP(addr,0)) == PLUS
	      && ALIGNED_SYMBOL_REF_P (XEXP(XEXP(addr,0),0))
	      && GET_CODE(XEXP(XEXP(addr,0),1)) == CONST_INT)
	    {
	      HOST_WIDE_INT v = INTVAL(XEXP(XEXP(addr,0),1));
	      if ((v & -16) != 0)
		addr = gen_rtx_CONST (Pmode,
			 gen_rtx_PLUS (Pmode, XEXP(XEXP(addr,0),0), GEN_INT (v & -16)));
	      else
		addr = XEXP(XEXP(addr,0),0);
	      p1_lo = GEN_INT(v & 15);
	    }
	}
      else if (GET_CODE(addr) == CONST_INT)
	{
	  p1_lo = GEN_INT(INTVAL(addr) & 15);
	  addr =  GEN_INT(INTVAL(addr) & -16);
	}
    }

  scalar = is_scalar_var(ops[0]);
  if (!scalar)
    {
      rtx base;
      if (aform && flag_pic && GET_CODE(addr) != CONST_INT)
	spu_emit_insn(gen_spu_lqr(reg, addr));
      else if (aform)
	spu_emit_insn(gen_spu_lqa(reg, addr));
      else if (GET_CODE (p1) == REG)
	spu_emit_insn(gen_spu_lqx(reg, p0, p1));
      else
	spu_emit_insn(gen_spu_lqd(reg, p0, p1));

      /* We could copy the flags from the ops[0] MEM to the MEM
         generated by the loads, but we don't because we want this
         load to be optimized away if it can be, and copying the
	 flags will prevent that in certain cases, e.g. consider
	 the volatile flag. */

      base = REG_ALIGN(p0) >= 128 ? gen_rtx_REG(SImode, STACK_POINTER_REGNUM) : p0;

      switch (GET_MODE_SIZE(mode))
      {
      case 1:  spu_emit_insn(gen_spu_cbx(pat, base, p1_lo)); break;
      case 2:  spu_emit_insn(gen_spu_chx(pat, base, p1_lo)); break;
      case 4:  spu_emit_insn(gen_spu_cwx(pat, base, p1_lo)); break;
      case 8:  spu_emit_insn(gen_spu_cdx(pat, base, p1_lo)); break;
      default: abort();
      }
      spu_emit_insn(gen_spu_shufb(reg, ops[1], reg, pat));
    }
  else if (reload_completed)
    {
      if (GET_CODE(ops[1]) == REG)
	emit_move_insn(reg, gen_rtx_REG (GET_MODE (reg), REGNO(ops[1])));
      else if (GET_CODE(ops[1]) == SUBREG)
	emit_move_insn(reg, gen_rtx_REG (GET_MODE (reg), REGNO(SUBREG_REG(ops[1]))));
      else
	abort();
    }
  else 
    {
      if (GET_CODE(ops[1]) == REG)
	spu_emit_insn(gen_spu_convert(reg, ops[1]));
      else if (GET_CODE(ops[1]) == SUBREG)
	spu_emit_insn(gen_spu_convert(reg, SUBREG_REG(ops[1])));
      else
	abort();
    }

  if (GET_MODE_SIZE(mode) < 4 && scalar)
    spu_emit_insn(gen_spu_shlqby(reg, reg, GEN_INT (4-GET_MODE_SIZE (mode))));

  if (aform && flag_pic && GET_CODE(addr) != CONST_INT)
    store = spu_emit_insn(gen_spu_stqr(reg, addr));
  else if (aform)
    store = spu_emit_insn(gen_spu_stqa(reg, addr));
  else if (GET_CODE (p1) == REG)
    store = spu_emit_insn(gen_spu_stqx(reg, p0, p1));
  else
    store = spu_emit_insn(gen_spu_stqd(reg, p0, p1));
  if (GET_CODE(PATTERN(store)) == SET)
    {
      rtx mem = SET_DEST(PATTERN(store));
      set_mem_align(mem, 128);
      set_mem_size(mem, GEN_INT(16));
      RTX_FLAG (mem, in_struct) = RTX_FLAG (ops[0], in_struct);
      RTX_FLAG (mem, volatil) = RTX_FLAG (ops[0], volatil);
      /* FIXME - bccheng - integrated is gone in gcc 4.0.0 
      RTX_FLAG (mem, integrated) = RTX_FLAG (ops[0], integrated);
       */
      RTX_FLAG (mem, frame_related) = RTX_FLAG (ops[0], frame_related);
    }
}

/* Return TRUE if X is MEM which is a struct member reference
 * and the member can safely be loaded and stored with a single
 * instruction because it is padded. */
static int
mem_is_padded_component_ref(rtx x)
{
  tree t = MEM_EXPR(x);
  tree r;
  if (!t || TREE_CODE(t) != COMPONENT_REF)
    return 0;
  t = TREE_OPERAND(t, 1);
  if (!t || TREE_CODE(t) != FIELD_DECL
      || DECL_ALIGN(t) < 128
      || AGGREGATE_TYPE_P(TREE_TYPE(t)))
    return 0;
  /* Only do this for RECORD_TYPEs, not UNION_TYPEs. */
  r = DECL_FIELD_CONTEXT(t);
  if (!r || TREE_CODE(r) != RECORD_TYPE)
    return 0;
  /* Make sure they are the same mode */
  if (GET_MODE (x) != TYPE_MODE (TREE_TYPE (t)))
    return 0;
  /* If there are no following fields then the field alignment assures
   * the structure is padded to the alignement which means this field is
   * padded too. */
  if (TREE_CHAIN(t) == 0)
    return 1;
  /* If the following field is also aligned then this field will be
   * padded. */
  t = TREE_CHAIN(t);
  if (TREE_CODE(t) == FIELD_DECL
      && DECL_ALIGN(t) >= 128)
    return 1;
  return 0;
}

int
spu_valid_move(rtx *ops)
{
  enum machine_mode mode = GET_MODE(ops[0]);
  if (!register_operand(ops[0], mode) && !register_operand(ops[1], mode))
    return 0;

  /* init_expr_once tries to recog against load and store insns to set
   * the direct_load[] and direct_store[] arrays.  We always want to
   * consider those loads and stores valid.  init_expr_once is called in
   * the context of a dummy function which does not have a decl. */
  if (cfun->decl == 0)
    return 1;

  /* Don't allows loads/stores which would require more than 1 insn.
   * During and after reload we assume loads and stores only take 1
   * insn. */
  if (GET_MODE_SIZE(mode) < 16 && !reload_in_progress && !reload_completed)
    {
      if (GET_CODE(ops[0]) == MEM
	  && (GET_MODE_SIZE(mode) < 4
	      || !(is_scalar_var(ops[0])
		   || mem_is_padded_component_ref(ops[0]))))
	return 0;
      /* We can't use MEM_SCALAR_P for loads because the compiler will
       * load a smaller type from a larger type, e.g. load an SI type
       * from a DI scalar. (Maybe MEM_OFFSET will tells us that.) */
      if (GET_CODE(ops[1]) == MEM
	  && (GET_MODE_SIZE(mode) < 4
	      || !aligned_mem(ops[1])))
	return 0;
    }
  return 1;
}

int
pic_address_operand (rtx op, enum machine_mode mode)
{
  return (GET_CODE(op) == SYMBOL_REF || GET_CODE(op) == LABEL_REF || GET_CODE(op) == CONST)
         && address_operand(op, mode);
}

static void
constant_to_array(enum machine_mode mode, rtx x, unsigned char arr[16])
{
  HOST_WIDE_INT val;
  int i, j, first;

  memset(arr,0,16);
  mode = VECTOR_MODE_P(mode) ? GET_MODE_INNER(mode) : mode;
  if (GET_CODE(x) == CONST_INT
      || (GET_CODE(x) == CONST_DOUBLE
	  && (GET_MODE(x) == SFmode || GET_MODE(x) == DFmode)))
    {
      if (GET_CODE(x) == CONST_DOUBLE)
	val = const_double_to_hwint(x);
      else
	val = INTVAL(x);
      if (GET_MODE_SIZE(mode) <= 4)
	first = 3;
      else
	first = GET_MODE_SIZE(mode) - 1;
      for (i = first; i >= 0; i--)
	{
	  arr[i] = val & 0xff;
	  val >>= 8;
	}
      /* Splat the constant across the whole array. */
      for (j = 0, i = first + 1; i < 16; i++)
	{
	  arr[i] = arr[j];
	  j = (j == first) ? 0 : j+1;
	}
    }
  else if (GET_CODE(x) == CONST_DOUBLE)
    {
      val = CONST_DOUBLE_LOW(x);
      for (i = 15; i >= 8; i--)
	{
	  arr[i] = val & 0xff;
	  val >>= 8;
	}
      val = CONST_DOUBLE_HIGH(x);
      for (i = 7; i >= 0; i--)
	{
	  arr[i] = val & 0xff;
	  val >>= 8;
	}
    }
  else if (GET_CODE(x) == CONST_VECTOR)
    {
      int units;
      rtx elt;
      mode = GET_MODE_INNER(GET_MODE(x));
      units = CONST_VECTOR_NUNITS (x);
      for (i = 0; i < units; i++)
	{
	  elt = CONST_VECTOR_ELT (x, i);
	  if (GET_CODE(elt) == CONST_INT ||  GET_CODE(elt) == CONST_DOUBLE)
	    {
	      if (GET_CODE(elt) == CONST_DOUBLE)
		val = const_double_to_hwint(elt);
	      else
		val = INTVAL(elt);
	      first = GET_MODE_SIZE(mode) - 1;
	      if (first + i * GET_MODE_SIZE(mode) > 16)
		abort();
	      for (j = first; j >= 0; j--)
		{
		  arr[j + i * GET_MODE_SIZE(mode)] = val & 0xff;
		  val >>= 8;
		}
	    }
	}
    }
  else
    abort();
}

static rtx
array_to_constant(enum machine_mode mode, unsigned char arr[16])
{
  enum machine_mode inner_mode;
  rtvec v;
  int units, size, i, j, k;
  HOST_WIDE_INT val;

  if (GET_MODE_CLASS (mode) == MODE_INT
      && GET_MODE_BITSIZE (mode) <= HOST_BITS_PER_WIDE_INT)
    {
      j = GET_MODE_SIZE (mode);
      i = j < 4 ? 4 - j : 0;
      for (val = 0; i < j; i++)
	val = (val << 8) | arr[i];
      val = trunc_int_for_mode (val, mode);
      return GEN_INT(val);
    }

  if (mode == TImode)
    {
      HOST_WIDE_INT high;
      for (i = high = 0; i < 8; i++)
	high = (high << 8) | arr[i];
      for (i = 8, val = 0; i < 16; i++)
	val = (val << 8) | arr[i];
      return immed_double_const (val, high, TImode);
    }

  if (!VECTOR_MODE_P(mode))
    return 0;

  units = GET_MODE_NUNITS (mode);
  size = GET_MODE_UNIT_SIZE(mode);
  inner_mode = GET_MODE_INNER(mode);
  v = rtvec_alloc (units);

  for (k = i = 0; i < units; ++i)
    {
      long tv[2];
      val = 0;
      for (j = 0; j < size; j++, k++)
	val = (val << 8) | arr[k];

      if (GET_MODE_CLASS(inner_mode) == MODE_FLOAT)
	{
	  REAL_VALUE_TYPE rv;

	  if (inner_mode == SFmode)
	    tv[0] = (val << 32) >> 32;
	  else if (inner_mode == DFmode)
	    {
	      tv[1] = (val << 32) >> 32;
	      tv[0] = val >> 32;
	    }
	  else
	    abort();

	  real_from_target (&rv, tv, inner_mode);
	  RTVEC_ELT (v, i) = CONST_DOUBLE_FROM_REAL_VALUE (rv, inner_mode);
	}
      else
	{
          /* sign extend */
          val = (val << (64-size*8)) >> (64-size*8);

	  RTVEC_ELT (v, i) = GEN_INT(val);
	}
    }
  if (k > 16)
    abort();

  return gen_rtx_CONST_VECTOR (mode, v);
}

static HOST_WIDE_INT
array_to_int( unsigned char arr[16], int start, int length, int sign_extend)
{
  HOST_WIDE_INT val;
  int i;
  val = (sign_extend && (arr[start] & 0x80)) ? -1 : 0;
  for (i = start; i < start + length; i++)
    val = (val << 8) + arr[i];
  return val;
}

static void
int_to_array(HOST_WIDE_INT val, unsigned char arr[16], int start, int length)
{
  int i;
  assert(length <= (int)sizeof(val));
  for (i = start + length - 1; i >= start; i--)
    {
      arr[i] = val & 0xff;
      val >>= 8;
    }
}

static void
reloc_diagnostic (rtx x)
{
  tree loc_decl, decl = 0;
  const char *msg;
  if (!flag_pic || !(TARGET_WARN_RELOC || TARGET_ERROR_RELOC))
    return;

  if (GET_CODE (x) == SYMBOL_REF)
    decl = SYMBOL_REF_DECL (x);
  else if (GET_CODE (x) == CONST
      && GET_CODE (XEXP (XEXP (x, 0), 0)) == SYMBOL_REF)
    decl = SYMBOL_REF_DECL (XEXP (XEXP (x, 0), 0));

  /* SYMBOL_REF_DECL is not necessarily a DECL. */
  if (decl && !DECL_P (decl))
    decl = 0;

  /* We use last_assemble_variable_decl to get line information.  It's
   * not always going to be right and might not even be close, but will
   * be right for the more common cases. */
  if (!last_assemble_variable_decl || in_ctor_section())
    loc_decl = decl;
  else 
    loc_decl = last_assemble_variable_decl;

  if (decl)
    msg = "%Jcreating run-time relocation for '%D'";
  else
    msg = "creating run-time relocation";

  if (TARGET_ERROR_RELOC) /** default : error reloc **/
    error (msg, loc_decl, decl);
  else
    warning (msg, loc_decl, decl);
}

static bool
spu_assemble_integer (rtx x, unsigned int size, int aligned_p)
{
  /* By default run-time relocations aren't supported, but we allow them
     in case users support it in their own run-time loader.  And we provide
     a warning for those users that don't*/
  if ((GET_CODE (x) == SYMBOL_REF)
      || GET_CODE (x) == LABEL_REF
       || GET_CODE (x) == CONST)
    reloc_diagnostic (x);

  return default_assemble_integer (x, size, aligned_p);
}

static void
spu_asm_globalize_label (FILE * file, const char * name)
{
  fputs ("\t.global\t", file);
  assemble_name (file, name);
  fputs ("\n", file);
}

int
const_vector_uses_shuf(rtx op)
{
  unsigned char arr[16];
  if (GET_CODE(op) != CONST_VECTOR)
    return 0;
  constant_to_array(GET_MODE(op), op, arr);
  return (fsmbi_for_array(arr) != 0 || cxd_for_array(arr) != 0);
}

static bool
spu_rtx_costs (rtx x, int code, int outer_code ATTRIBUTE_UNUSED, 
		  int *total)
{
  enum machine_mode mode = GET_MODE(x);
  int cost = COSTS_N_INSNS(2);

  /* Folding to a CONST_VECTOR will use extra space but there might
     be only a small savings in cycles.  We'd like to use a CONST_VECTOR
     only if it allows us to fold away multiple insns.  Changin the cost
     of a CONST_VECTOR here (or in CONST_COSTS) doesn't help though
     because this cost will only be compared agains a single insn. 
  if (code == CONST_VECTOR)
    return (LEGITIMATE_CONSTANT_P(x)) ? cost : COSTS_N_INSNS(6);
   */

  /* Use defaults for float operations.  Not accurate but good enough. */
  if (mode == DFmode)
    {
      *total = COSTS_N_INSNS (13);
      return true;
    }
  if (mode == SFmode)
    {
      *total = COSTS_N_INSNS (6);
      return true;
    }
  switch (code)
    {
    case CONST_INT:
      if (CONST_OK_FOR_LETTER_P(INTVAL(x),'K'))
	*total = 0;
      else if (INTVAL(x) >= -0x80000000ll
	       && INTVAL(x) <= 0xffffffffll)
	*total = COSTS_N_INSNS(1);
      else
	*total = COSTS_N_INSNS(3);
      return true;

    case CONST:
      *total = COSTS_N_INSNS(3);
      return true;

    case LABEL_REF:
    case SYMBOL_REF:
      *total = COSTS_N_INSNS(0);
      return true;

    case CONST_DOUBLE:
      *total = COSTS_N_INSNS(5);
      return true;

    case FLOAT_EXTEND:
    case FLOAT_TRUNCATE:
    case FLOAT:
    case UNSIGNED_FLOAT:
    case FIX:
    case UNSIGNED_FIX:
	*total = COSTS_N_INSNS (7);
	return true;

    case PLUS:
	if (mode == TImode)
	  {
	    *total = COSTS_N_INSNS (9);
	    return true;
	  }
	break;

    case MULT:
	cost = GET_CODE(XEXP(x, 0)) == REG ? COSTS_N_INSNS (12) : COSTS_N_INSNS (7);
	if (mode == SImode && GET_CODE(XEXP(x, 0)) == REG)
	  {
	    if (GET_CODE(XEXP(x, 1)) == CONST_INT)
	      {
		HOST_WIDE_INT val = INTVAL(XEXP(x, 1));
		cost = COSTS_N_INSNS (14);
		if ((val & 0xffff) == 0)
		  cost = COSTS_N_INSNS (9);
		else if (val > 0 && val < 0x10000)
		  cost = COSTS_N_INSNS (11);
	      }
	  }
	*total = cost;
	return true;
    case DIV:
    case UDIV:
    case MOD:
    case UMOD:
	*total = COSTS_N_INSNS (20);
	return true;
    case ROTATE:
    case ROTATERT:
    case ASHIFT:
    case ASHIFTRT:
    case LSHIFTRT:
	*total = COSTS_N_INSNS (4);
	return true;
    case UNSPEC:
	if (XINT(x, 1) == UNSPEC_SPU_CONVERT)
	  *total = COSTS_N_INSNS (0);
	else
	  *total = COSTS_N_INSNS (4);
	return true;
    }
  /* Scale cost by mode size.  Except when initializing (cfun->decl == 0). */
  if (GET_MODE_CLASS(mode) == MODE_INT
      && GET_MODE_SIZE(mode) > GET_MODE_SIZE(SImode)
      && cfun && cfun->decl)
    cost = cost * (GET_MODE_SIZE(mode) / GET_MODE_SIZE(SImode))
                * (GET_MODE_SIZE(mode) / GET_MODE_SIZE(SImode));
  *total = cost;
  return true;
}

#if 0
/* Uncomment this to debug spu_simplify_unspec */
static rtx spu_simplify_unspec_1 (rtx, rtx, rtx, rtx);
static rtx
spu_simplify_unspec (x, c0, c1, c2)
     rtx x, c0, c1, c2;
{
  rtx new;
  new = spu_simplify_unspec_1 (x, c0, c1, c2);
  if (new && new != x)
    {
      fprintf (stderr, "BEFORE\n");
      debug_rtx (x);
      if (c0)
	{
	  fprintf (stderr, "c0\n");
	  debug_rtx (c0);
	}
      if (c1)
	{
	  fprintf (stderr, "c1\n");
	  debug_rtx (c1);
	}
      if (c2)
	{
	  fprintf (stderr, "c2\n");
	  debug_rtx (c2);
	}
      fprintf (stderr, "AFTER\n");
      debug_rtx (new);
    }
  return new;
}
#define spu_simplify_unspec spu_simplify_unspec_1
#endif

static rtx
spu_simplify_unspec (rtx x, rtx c0, rtx c1, rtx c2)
{
  enum machine_mode mode = GET_MODE (x);
  int unspec_code = XINT (x, 1);
  HOST_WIDE_INT val;
  unsigned HOST_WIDE_INT v0, v1, v2;
  rtx op0, op1, op2, new;
  int i, j;
  int l = XVECLEN(x, 0);
  int opsize = 1;
  unsigned char arr0[16], arr1[16], arr2[16], dst[16];

  if (!TARGET_VECTOR_SIMPLIFY)
    return 0;

  if (!VECTOR_MODE_P(mode))
    return 0;
  
  op0 = l >= 1 ? (c0 ? c0 : XVECEXP(x, 0, 0)) : 0;
  op1 = l >= 2 ? (c1 ? c1 : XVECEXP(x, 0, 1)) : 0;
  op2 = l >= 3 ? (c2 ? c2 : XVECEXP(x, 0, 2)) : 0;
  /* Check for cases involving CONST0_RTX */
  switch (unspec_code)
    {
    case UNSPEC_SPU_ADDX:
      if (op0 == CONST0_RTX(GET_MODE(op0))
	  && op1 == CONST0_RTX(GET_MODE(op1))
	  && op2 == CONST0_RTX(GET_MODE(op2)))
	return CONST0_RTX(mode);
      break;
    case UNSPEC_SPU_CG:
    case UNSPEC_SPU_AVGB:
    case UNSPEC_SPU_ABSDB:
    case UNSPEC_SPU_SUMB:
      if (op0 == CONST0_RTX(GET_MODE(op0))
	  && op1 == CONST0_RTX(GET_MODE(op1)))
	return CONST0_RTX(mode);
      break;
    case UNSPEC_SPU_FSMB:
    case UNSPEC_SPU_FSMH:
    case UNSPEC_SPU_FSM:
      if (op0 == const0_rtx)
	return CONST0_RTX(mode);
      break;
    case UNSPEC_SPU_SHLQBY:
    case UNSPEC_SPU_ROTQBY:
    case UNSPEC_SPU_SHLQBI:
    case UNSPEC_SPU_ROTQBI:
    case UNSPEC_SPU_ROTQMBI:
    case UNSPEC_SPU_ROTQMBY:
    case UNSPEC_SPU_SHLQBYBI:
    case UNSPEC_SPU_ROTQBYBI:
    case UNSPEC_SPU_ROTQMBYBI:
    case UNSPEC_SPU_SHLH:
    case UNSPEC_SPU_SHL:
    case UNSPEC_SPU_ROTHM:
    case UNSPEC_SPU_ROTM:
    case UNSPEC_SPU_ROTMAH:
    case UNSPEC_SPU_ROTMA:
      if (op1 == CONST0_RTX(GET_MODE(op1))
	  || op0 == CONST0_RTX(GET_MODE(op0)))
	return mode == GET_MODE (op0) ? op0 
			: gen_rtx_UNSPEC (mode,
					  gen_rtvec (1, op0),
					  UNSPEC_SPU_CONVERT);
      break;
    }
  switch (unspec_code)
    {
    case UNSPEC_FREST:
    case UNSPEC_FRSQEST:
    case UNSPEC_FI:
      break;
    case UNSPEC_EXTEND_CMP:
      if (GET_CODE (op0) == CONST_INT
	  && (GET_MODE_CLASS (mode) == MODE_INT
	      || GET_MODE_CLASS (mode) == MODE_VECTOR_INT))
	return op0 == const0_rtx ? CONST0_RTX (mode) : CONSTM1_RTX (mode);
      break;
    case UNSPEC_SPU_CBX:
    case UNSPEC_SPU_CHX:
    case UNSPEC_SPU_CWX:
    case UNSPEC_SPU_CDX:
      if (GET_CODE(op1) == CONST_INT
	  && (GET_CODE(op0) == CONST_INT 
	      || (GET_CODE(op0) == REG
		  && REG_ALIGN(op0) >= 128)))
	{
	  int offset, shift, isize;
	  for (i = 0; i < 16; i++)
	    dst[i] = i + 16;
          if (unspec_code == UNSPEC_SPU_CBX)
	    isize = 1, shift = 3;
          else if (unspec_code == UNSPEC_SPU_CHX)
	    isize = 2, shift = 2;
          else if (unspec_code == UNSPEC_SPU_CWX)
	    isize = 4, shift = 0;
          else /* if (unspec_code == UNSPEC_SPU_CDX) */
	    isize = 8, shift = 0;
	  offset = (INTVAL(op1) + (GET_CODE(op0) == CONST_INT ? INTVAL(op0) : 0)) & 15;
	  for (i = 0; i < isize; i++)
	    dst[offset+i] = i + shift;
	  return array_to_constant(mode, dst);
	}
      break;
    case UNSPEC_SPU_ADDX:
      if (GET_CODE(op0) == CONST_VECTOR
	  && GET_CODE(op1) == CONST_VECTOR
	  && GET_CODE(op2) == CONST_VECTOR)
	{
	  constant_to_array(GET_MODE(op0), op0, arr0);
	  constant_to_array(GET_MODE(op1), op1, arr1);
	  constant_to_array(GET_MODE(op2), op2, arr2);
	  for (i = 0; i < 4; i++)
	    {
  	      v0 = array_to_int(arr0, i*4, 4, 0);
  	      v1 = array_to_int(arr1, i*4, 4, 0);
  	      v2 = array_to_int(arr2, i*4, 4, 0);
	      val = v0 + v1 + (v2 & 1);
	      int_to_array(val, dst, i*4, 4);
	    }
	  return array_to_constant(mode, dst);
	}
      break;
    case UNSPEC_SPU_CG:
      if (GET_CODE(op0) == CONST_VECTOR
	  && GET_CODE(op1) == CONST_VECTOR)
	{
	  constant_to_array(GET_MODE(op0), op0, arr0);
	  constant_to_array(GET_MODE(op1), op1, arr1);
	  for (i = 0; i < 4; i++)
	    {
  	      v0 = array_to_int(arr0, i*4, 4, 0);
  	      v1 = array_to_int(arr1, i*4, 4, 0);
	      val = ((v0 + v1) & 0xffffffffu) < v0;
	      int_to_array(val, dst, i*4, 4);
	    }
	  return array_to_constant(mode, dst);
	}
      break;
    case UNSPEC_SPU_CGX:
      if (GET_CODE(op0) == CONST_VECTOR
	  && GET_CODE(op1) == CONST_VECTOR
	  && GET_CODE(op2) == CONST_VECTOR)
	{
	  constant_to_array(GET_MODE(op0), op0, arr0);
	  constant_to_array(GET_MODE(op1), op1, arr1);
	  constant_to_array(GET_MODE(op2), op2, arr2);
	  for (i = 0; i < 4; i++)
	    {
  	      v0 = array_to_int(arr0, i*4, 4, 0);
  	      v1 = array_to_int(arr1, i*4, 4, 0);
  	      v2 = array_to_int(arr2, i*4, 4, 0) & 1;
	      val = ((v0 + v1) & 0xffffffffu) < v0 || ((v0 + v1 + v2) & 0xffffffffu) < v2;
	      int_to_array(val, dst, i*4, 4);
	    }
	  return array_to_constant(mode, dst);
	}
      break;
    case UNSPEC_SPU_SFX:
      if (GET_CODE(op0) == CONST_VECTOR
	  && GET_CODE(op1) == CONST_VECTOR
	  && GET_CODE(op2) == CONST_VECTOR)
	{
	  constant_to_array(GET_MODE(op0), op0, arr0);
	  constant_to_array(GET_MODE(op1), op1, arr1);
	  constant_to_array(GET_MODE(op2), op2, arr2);
	  for (i = 0; i < 4; i++)
	    {
  	      v0 = array_to_int(arr0, i*4, 4, 0);
  	      v1 = array_to_int(arr1, i*4, 4, 0);
  	      v2 = array_to_int(arr2, i*4, 4, 0);
	      val = v0 - v1;
	      if ((v2 & 1) == 0)
		val--;
	      int_to_array(val, dst, i*4, 4);
	    }
	  return array_to_constant(mode, dst);
	}
      break;
    case UNSPEC_SPU_BG:
      if (GET_CODE(op0) == CONST_VECTOR
	  && GET_CODE(op1) == CONST_VECTOR)
	{
	  constant_to_array(GET_MODE(op0), op0, arr0);
	  constant_to_array(GET_MODE(op1), op1, arr1);
	  for (i = 0; i < 4; i++)
	    {
  	      v0 = array_to_int(arr0, i*4, 4, 0);
  	      v1 = array_to_int(arr1, i*4, 4, 0);
	      val = v1 > v0 ? 0 : 1;
	      int_to_array(val, dst, i*4, 4);
	    }
	  return array_to_constant(mode, dst);
	}
      break;
    case UNSPEC_SPU_BGX:
      break;
    case UNSPEC_SPU_CLZ:
      if (GET_CODE(op0) == CONST_VECTOR)
	{
	  constant_to_array(GET_MODE(op0), op0, arr0);
	  for (i = 0; i < 4; i++)
	    {
  	      val = array_to_int(arr0, i*4, 4, 0);
	      for (j = 0; j < 32; j++)
		if (val & (1 << (31-j)))
		    break;
	      int_to_array((HOST_WIDE_INT)j, dst, i*4, 4);
	    }
	  return array_to_constant(mode, dst);
	}
      break;
    case UNSPEC_SPU_CNTB:
      if (GET_CODE(op0) == CONST_VECTOR)
	{
	  constant_to_array(GET_MODE(op0), op0, arr0);
	  for (i = 0; i < 16; i++)
	    {
	      int bits = 0;
  	      val = arr0[i];
	      for (j = 0; j < 8 && val; j++, val >>= 1)
		if (val & 1)
		    bits++;
	      dst[i] = bits;
	    }
	  return array_to_constant(mode, dst);
	}
      break;
    case UNSPEC_SPU_FSM:
      opsize *= 2;
    case UNSPEC_SPU_FSMH:
      opsize *= 2;
    case UNSPEC_SPU_FSMB:
      if (GET_CODE(op0) == CONST_INT)
	{
	  val = INTVAL(op0);
	  for (i = 0, j = (1 << (16/opsize-1)); i < 16; i += opsize, j >>= 1)
	    {
	      HOST_WIDE_INT v = (val & j) ? -1 : 0;
	      int_to_array(v, dst, i, opsize);
	    }
	  return array_to_constant(mode, dst);
	}
      break;
    case UNSPEC_SPU_GBB:
    case UNSPEC_SPU_GBH:
    case UNSPEC_SPU_GB:
    case UNSPEC_SPU_AVGB:
    case UNSPEC_SPU_ABSDB:
    case UNSPEC_SPU_SUMB:
      break;
    case UNSPEC_SPU_SHUFB:
      if (GET_CODE(op2) == CONST_VECTOR)
	{
	  enum machine_mode mode0 = GET_MODE(op0);
	  enum machine_mode mode1 = GET_MODE(op1);
	  if (mode0 == VOIDmode)
	      mode0 = GET_MODE (XVECEXP(x, 0, 0));
	  if (mode1 == VOIDmode)
	      mode1 = GET_MODE (XVECEXP(x, 0, 1));
	  if (mode0 == VOIDmode || mode1 == VOIDmode)
	    return 0;
	  constant_to_array(GET_MODE(op2), op2, arr2);
	  if ((GET_CODE (op0) == CONST_INT
		|| GET_CODE (op0) == CONST_DOUBLE
		|| GET_CODE (op0) == CONST_VECTOR) 
	      && (GET_CODE (op1) == CONST_INT
		|| GET_CODE (op1) == CONST_DOUBLE
		|| GET_CODE (op1) == CONST_VECTOR))
	    {
	      unsigned char arr[32];
	      constant_to_array(mode0, op0, arr);
	      constant_to_array(mode1, op1, &arr[16]);
	      for (i = 0; i < 16; i++)
		{
		  if ((arr2[i] & 0xc0) == 0x80)
		    dst[i] = 0;
		  else if ((arr2[i] & 0xe0) == 0xc0)
		    dst[i] = 0xff;
		  else if ((arr2[i] & 0xe0) == 0xe0)
		    dst[i] = 0x80;
		  else
		    dst[i] = arr[arr2[i]&31];
		}
	      return array_to_constant(mode, dst);
	    }
	  /* Catch the special case of splating a pointer which we can
	     optimize to an ila instruction. */
	  if (mode == V4SImode && mode0 == SImode
	      && (GET_CODE (op0) == CONST || GET_CODE (op0) == SYMBOL_REF)
	      && op0 == op1
	      && arr2[0] == 0 && arr2[1] == 1 && arr2[2] == 2 && arr2[3] == 3
	      && arr2[4] == 0 && arr2[5] == 1 && arr2[6] == 2 && arr2[7] == 3
	      && arr2[8] == 0 && arr2[9] == 1 && arr2[10] == 2 && arr2[11] == 3
	      && arr2[12] == 0 && arr2[13] == 1 && arr2[14] == 2 && arr2[15] == 3)
	    {
	     return spu_const_vector(mode, op0);
	    }
	  if (GET_MODE_CLASS(mode0) == MODE_INT
	      && GET_MODE_SIZE(mode0) >= 4
	      && (GET_CODE(op0) == ASHIFT
	          || GET_CODE(op0) == ASHIFTRT
	          || GET_CODE(op0) == LSHIFTRT)
	      && GET_CODE(XEXP(op0, 1)) == CONST_INT
	      && (INTVAL(XEXP(op0, 1)) & 7) == 0)
	    {
	      int shift0 = GET_CODE(op0) == ASHIFT ? INTVAL(XEXP(op0, 1)) : -INTVAL(XEXP(op0, 1));
	      shift0 /= 8;
	      for (i = 0; i < 16; i++)
		if (arr2[i] < GET_MODE_SIZE(mode0))
		  {
		    arr2[i] += shift0;
		    if (arr2[i] >= GET_MODE_SIZE(mode0))
		      {
			if (GET_CODE(op0) == ASHIFTRT)
			  return 0;
			arr2[i] = 0x80;
		      }
		  }
		else if (arr2[i] < 16)
		  abort();
	      op0 = XEXP(op0, 0);
	      op2 = array_to_constant(V16QImode, arr2);
	      new = gen_rtx_UNSPEC (mode,
				    gen_rtvec (3, op0, op1, op2),
				    UNSPEC_SPU_SHUFB);
	      return new;
	    }
	  if (GET_MODE_CLASS(mode1) == MODE_INT
	      && GET_MODE_SIZE(mode1) >= 4
	      && (GET_CODE(op1) == ASHIFT
	          || GET_CODE(op1) == ASHIFTRT
	          || GET_CODE(op1) == LSHIFTRT)
	      && GET_CODE(XEXP(op1, 1)) == CONST_INT
	      && (INTVAL(XEXP(op1, 1)) & 7) == 0)
	    {
	      int shift1 = GET_CODE(op1) == ASHIFT ? INTVAL(XEXP(op1, 1)) : -INTVAL(XEXP(op1, 1));
	      for (i = 0; i < 16; i++)
		if (arr2[i] >= 16 && arr2[i] < 16 + GET_MODE_SIZE(mode1))
		  {
		    arr2[i] += shift1;
		    if (arr2[i] >= GET_MODE_SIZE(mode1))
		      {
			if (GET_CODE(op1) == ASHIFTRT)
			  return 0;
			arr2[i] = 0x80;
		      }
		  }
		else if (arr2[i] >= 16 && arr2[i] < 32)
		  abort();
	      op1 = XEXP(op1, 0);
	      op2 = array_to_constant(V16QImode, arr2);
	      new = gen_rtx_UNSPEC (mode,
				    gen_rtvec (3, op0, op1, op2),
				    UNSPEC_SPU_SHUFB);
	      return new;
	    }
	}
      break;
    case UNSPEC_SPU_SHLQBY:
    case UNSPEC_SPU_ROTQBY:
    case UNSPEC_SPU_SHLQBYBI:
    case UNSPEC_SPU_ROTQBYBI:
      if (GET_CODE(op0) == CONST_VECTOR 
	       && GET_CODE(op1) == CONST_INT)
	{
	  int shift = INTVAL(op1);
	  if (unspec_code == UNSPEC_SPU_SHLQBYBI
	      || unspec_code == UNSPEC_SPU_ROTQBYBI)
	    shift >>= 3;
	  shift &= 31;
	  constant_to_array(GET_MODE(op0), op0, arr0);
	  if (unspec_code == UNSPEC_SPU_SHLQBY
	      || unspec_code == UNSPEC_SPU_SHLQBYBI)
	    for (i = 0; i < shift && i < 16; i++)
	      arr0[i] = 0;
	  for (i = 0; i < 16; i++)
	    dst[i] =  arr0[(i+shift)&15];
	  return array_to_constant(mode, dst);
	}
      break;
    case UNSPEC_SPU_SHLQBI:
    case UNSPEC_SPU_ROTQBI:
      if (GET_CODE(op0) == CONST_VECTOR 
	  && GET_CODE(op1) == CONST_INT)
	{
	  int shift = INTVAL(op1) & 7;
	  constant_to_array(GET_MODE(op0), op0, arr0);
	  for (i = 0; i < 15; i++)
	    dst[i] =  (arr0[i] << shift) | (arr0[i+1] >> (8-shift));
	  dst[15] = arr0[15] << shift;
	  if (unspec_code == UNSPEC_SPU_ROTQBI)
	    dst[15] |= (arr0[0] >> (8-shift));
	  return array_to_constant(mode, dst);
	}
      break;

    case UNSPEC_SPU_ROTQMBI:
      if (GET_CODE(op0) == CONST_VECTOR 
	  && GET_CODE(op1) == CONST_INT)
	{
	  int shift = (-INTVAL(op1)) & 7;
	  constant_to_array(GET_MODE(op0), op0, arr0);
	  dst[0] = arr0[0] >> shift;
	  for (i = 1; i < 16; i++)
	    dst[i] =  (arr0[i] >> shift) | (arr0[i-1] << (8-shift));
	  return array_to_constant(mode, dst);
	}
      break;

    case UNSPEC_SPU_ROTQMBY:
    case UNSPEC_SPU_ROTQMBYBI:
      if (GET_CODE(op0) == CONST_VECTOR 
	       && GET_CODE(op1) == CONST_INT)
	{
	  int shift = INTVAL(op1);
	  if (unspec_code == UNSPEC_SPU_ROTQMBYBI)
	    shift >>= 3;
	  shift = (-shift) & 31;
	  if (shift >= 16)
	    return CONST0_RTX(mode);
	  constant_to_array(GET_MODE(op0), op0, arr0);
	  for (i = 0; i < shift; i++)
	    dst[i] = 0;
	  for (; i < 16; i++)
	    dst[i] = arr0[i-shift];
	  return array_to_constant(mode, dst);
	}
      break;

    case UNSPEC_SPU_FREST:
    case UNSPEC_SPU_FRSQEST:
    case UNSPEC_SPU_FI:
    case UNSPEC_SPU_CSFLT:
    case UNSPEC_SPU_CFLTS:
    case UNSPEC_SPU_CUFLT:
    case UNSPEC_SPU_CFLTU:
      break;
    case UNSPEC_SPU_SHL:
      opsize *= 2;
    case UNSPEC_SPU_SHLH:
      opsize *= 2;
      if (GET_CODE (op0) == CONST_VECTOR
	  && GET_CODE (op1) == CONST_VECTOR)
	{
	  constant_to_array(GET_MODE(op0), op0, arr0);
	  constant_to_array(GET_MODE(op1), op1, arr1);
	  for (i = 0; i < 16; i += opsize)
	    {
	      HOST_WIDE_INT shift;
  	      val = array_to_int(arr0, i, opsize, 0);
  	      shift = array_to_int(arr1, i, opsize, 0);
	      shift &= (16 * opsize - 1);
	      val = val << shift;
	      int_to_array(val, dst, i, opsize);
	    }
	  return array_to_constant(mode, dst);
	}
      break;
    case UNSPEC_SPU_ROTM:
    case UNSPEC_SPU_ROTMA:
      opsize *= 2;
    case UNSPEC_SPU_ROTHM:
    case UNSPEC_SPU_ROTMAH:
      opsize *= 2;
      if (GET_CODE (op1) == CONST_VECTOR)
	{
	  int arith = (unspec_code == UNSPEC_SPU_ROTMAH
		       || unspec_code == UNSPEC_SPU_ROTMA);
	  if (GET_CODE (op0) == CONST_VECTOR)
	    constant_to_array(GET_MODE(op0), op0, arr0);
	  constant_to_array(GET_MODE(op1), op1, arr1);
	  for (i = 0; i < 16; i += opsize)
	    {
	      HOST_WIDE_INT shift;
	      shift = array_to_int(arr1, i, opsize, 0);
	      shift &= 0x7f;
	      if (shift & 0x40)
		shift |= -0x40;
	      shift = (-shift) & (opsize * 16 - 1);
	      if (GET_CODE (op0) == CONST_VECTOR)
		{
		  HOST_WIDE_INT sign;
		  val = array_to_int(arr0, i, opsize, 1);
		  sign = ((arith) && (val & (1 << (opsize * 8 - 1))) != 0);
		  if (shift >= opsize * 8)
		    val = -sign;
		  else if (shift > 0)
		    {
		      if (arith && sign)
			val = (val >> shift) | (-1 << (opsize * 8 - shift));
		      else
			val = (val >> shift) & ((1 << (opsize * 8 - shift)) - 1);
		    }
		  int_to_array (val, dst, i, opsize);
		}
	      else if (shift >= opsize * 8 && !arith)
		int_to_array ((HOST_WIDE_INT)0, dst, i, opsize);
	      else
		return 0;
	    }
	  return array_to_constant(mode, dst);
	}
      break;
    case UNSPEC_SPU_CONVERT:
      {
	enum machine_mode mode_op0 = GET_MODE (XVECEXP (x, 0, 0));
	if (mode_op0 == mode)
	  return op0;
	if ((GET_CODE (op0) == CONST_INT
	     || GET_CODE (op0) == CONST_DOUBLE
	     || GET_CODE (op0) == CONST_VECTOR)
	    &&  mode_op0 != VOIDmode
	    && GET_MODE_SIZE (mode_op0) >= GET_MODE_SIZE (GET_MODE (x)))
	  {
	    constant_to_array(mode_op0, op0, arr0);
	    return array_to_constant(mode, arr0);
	  }
	if (GET_CODE (op0) == SUBREG
	    && GET_MODE_SIZE(GET_MODE(op0)) >= GET_MODE_SIZE(GET_MODE(SUBREG_REG(op0))))
	  op0 = SUBREG_REG(op0);
	/* Change
	    (unspec:M [ (unspec:N [ ... ] UNSPEC_SPU_*) ] UNSPEC_CONVERT)
	  to
	    (unspec:M [ ... ] UNSPEC_SPU_*) 
	  We specifically don't specify the modes on unspec's in spu.md
	  so this will work.  This helps eliminate extra copies.
	*/
	if (GET_CODE (op0) == UNSPEC && XINT(op0, 1) > UNSPEC_ADDX)
	  {
	    return gen_rtx_UNSPEC(mode, XVEC(op0, 0), XINT(op0, 1));
	  }
      }
      return 0;
    default:
      return 0;
    }
#if 0
  {
    int constant, j;
    constant = 1;
    for (j = 0; j < XVECLEN (x, 0); j++)
      {
	if (!CONSTANT_P (XVECEXP (x, 0, j))
	    && ((j == 0 && !c0)
	        || (j == 1 && !c1)
	        || (j == 2 && !c2)
		|| (j > 2)))
	  constant = 0;
      }
    if (constant)
      {
	fprintf (stderr, "****    SIMPLIFY ME!    ****\n");
	debug_rtx (x);
	if (c0) { fprintf(stderr, "c0\n"); debug_rtx(c0); }
	if (c1) { fprintf(stderr, "c1\n"); debug_rtx(c1); }
	if (c2) { fprintf(stderr, "c2\n"); debug_rtx(c2); }
      }
  }
#endif
  return 0;
}

static bool
float_op_p (rtx x)
{
  int code;
  if (GET_CODE (x) != SET)
    return false;
  x = SET_SRC (x);
  code = GET_CODE (x);
  return (code == PLUS || code == MINUS)
          && GET_CODE (XEXP (x, 0)) != MULT
          && GET_CODE (XEXP (x, 1)) != MULT
	  ? 2 : (code == MULT ? 1 : 0);
}

/* We don't allow creating new fma insns, but we do allowing existing
   fma insns to be optimized to variants involving negation. */
bool
spu_cant_combine (rtx i3, rtx i2, rtx i1)
{
  enum machine_mode mode = GET_CODE (PATTERN (i3)) == SET ? GET_MODE (SET_DEST (PATTERN (i3))) : VOIDmode;
  if ((mode == SFmode && spu_float_acc == SPU_FP_ACCURATE)
      || (mode == DFmode && spu_double_acc == SPU_FP_ACCURATE))
    {
      int f3 = float_op_p (PATTERN (i3));
      int f2 = float_op_p (PATTERN (i2));
      int f1 = i1 && float_op_p (PATTERN (i1));
      return (f3 | f2 | f1) == 3;
    }
  return false;
}

void
spu_expand_sign_extend(rtx ops[])
{
  unsigned char arr[16];
  rtx pat = gen_reg_rtx(V16QImode);
  rtx sign;
  int i, last;
  last = GET_MODE (ops[0]) == DImode ? 7 : 15;
  if (GET_MODE (ops[1]) == QImode)
    {
      sign = gen_reg_rtx (HImode);
      emit_insn (gen_extendqihi2 (sign, ops[1]));
      for (i = 0; i < 16; i++)
        arr[i] = 0x12;
      arr[last] = 0x13;
    }
  else
    {
      for (i = 0; i < 16; i++)
	arr[i] = 0x10;
      switch (GET_MODE (ops[1]))
      {
      case HImode: sign = gen_reg_rtx (SImode);
		   emit_insn (gen_extendhisi2 (sign, ops[1]));
		   arr[last] = 0x03;
		   arr[last-1] = 0x02;
		   break;
      case SImode: sign = gen_reg_rtx (SImode);
		   emit_insn (gen_ashrsi3 (sign, ops[1], GEN_INT (31)));
		   for (i = 0; i < 4; i++)
		     arr[last-i] = 3-i ;
		   break;
      case DImode: sign = gen_reg_rtx (V4SImode);
		   spu_emit_insn (gen_spu_rotmai (sign, gen_rtx_SUBREG(V4SImode, ops[1], 0), GEN_INT (31)));
		   for (i = 0; i < 8; i++)
		     arr[last-i] = 7-i ;
		   break;
      default:
		   abort();
      }
    }
  emit_move_insn (pat, array_to_constant (V16QImode, arr));
  if (GET_MODE (ops[0]) == DImode)
    emit_insn (gen_sign_extend_di (ops[0], ops[1], sign, pat));
  else
    emit_insn (gen_sign_extend_ti (ops[0], ops[1], sign, pat));
}

void
spu_extendsfdf2(rtx ops[])
{
  unsigned char arr[16] = {0, 1, 17, 18, 19, 0x80, 0x80, 0x80, 8, 9, 25, 26, 27, 0x80, 0x80, 0x80};
  rtx pat = gen_reg_rtx(V16QImode);
  rtx from, from1, to, sign, exponent, ztmp1,ztmp, ztmp2, etmp, eoffset, eoffset1, highoffset, op1, op2, op3;
  rtx zero;
  emit_move_insn (pat, array_to_constant (V16QImode, arr));

  zero = gen_reg_rtx(SImode);
  sign = gen_reg_rtx(V4SImode);
  exponent = gen_reg_rtx(V4SImode);
  ztmp1 = gen_reg_rtx(V16QImode);
  ztmp = gen_reg_rtx(V4SImode);
  ztmp2 = gen_reg_rtx(V4SFmode);
  etmp = gen_reg_rtx(V4SImode);
  
  eoffset = gen_reg_rtx(V4SImode);
  eoffset1 = gen_reg_rtx(V4SImode);
  highoffset = gen_reg_rtx(V4SImode);
  op1 =  gen_reg_rtx(V4SFmode);
  op2 =  gen_reg_rtx(V4SFmode);
  op3 =  gen_reg_rtx(V4SFmode);
  from1 = gen_reg_rtx(V4SFmode);
  from = gen_reg_rtx(V4SImode);
  to = gen_reg_rtx(V4SImode);
    
  spu_emit_insn (gen_spu_ilhu(etmp, GEN_INT(0x7ff8)));
  spu_emit_insn (gen_spu_fsmb(ztmp1, GEN_INT(0)));
  spu_emit_insn (gen_spu_convert(from1, ops[1]));
  spu_emit_insn (gen_spu_convert(ztmp2, ztmp1));
  spu_emit_insn (gen_spu_fcmeq(ztmp, from1, ztmp2 ));
  spu_emit_insn (gen_spu_convert(from, ops[1]));
  
  spu_emit_insn (gen_spu_and(etmp, from, etmp));
  spu_emit_insn (gen_spu_rotmi(etmp, etmp, GEN_INT(-3)));
  spu_emit_insn (gen_spu_ilhu(eoffset, GEN_INT(0x3800)));
  spu_emit_insn (gen_spu_ilhu(highoffset, GEN_INT(0x8000)));
  spu_emit_insn (gen_spu_andc(eoffset1, eoffset, ztmp));
  spu_emit_insn (gen_spu_shli(op1, from, GEN_INT(5)));
  spu_emit_insn (gen_spu_and(sign,  from, highoffset));
  spu_emit_insn (gen_spu_a(exponent, etmp, eoffset1));
  spu_emit_insn (gen_spu_or(to, sign, exponent));
  spu_emit_insn (gen_spu_shufb(to, to, op1, pat));
  spu_emit_insn (gen_spu_convert(ops[0], to));
}

void
spu_truncdfsf2(rtx ops[])
{
  unsigned char arr[16] = {0xff, 0x10, 0, 0, 0xff, 0x10, 0, 0, 0xff, 0x10, 0, 0, 0xff, 0x10, 0, 0}; 
  rtx pat = gen_reg_rtx(V16QImode);
  rtx from, from1, to, sign, smask, ztmp, ztmp1, ztmp2, etmp, emask, exp, exp1, eoffset, eoffset1, e, se, frac, fmax, fmaxu, fmaxl, overflow, ofb, ofbq, ofq, zero, mask, result;
  emit_move_insn (pat, array_to_constant (V16QImode, arr));

  smask = gen_reg_rtx(V4SImode);
  emask = gen_reg_rtx(V4SImode);
  sign = gen_reg_rtx(V4SImode);
  
  ztmp1 = gen_reg_rtx(V16QImode);
  ztmp = gen_reg_rtx(V4SImode);
  ztmp2 = gen_reg_rtx(V4SFmode);
  etmp = gen_reg_rtx(V4SImode);
  exp = gen_reg_rtx(V4SImode);
  exp1 = gen_reg_rtx(V4SImode); 
  eoffset = gen_reg_rtx(V4SImode);
  eoffset1 = gen_reg_rtx(V4SImode);
  
  ofb = gen_reg_rtx(V4SImode);
  ofbq = gen_reg_rtx(V16QImode);
  ofq = gen_reg_rtx(V16QImode);
  overflow = gen_reg_rtx(SImode);
  zero = gen_reg_rtx(V16QImode);
  
  e = gen_reg_rtx(V4SImode);
  se = gen_reg_rtx(V4SImode);
  frac = gen_reg_rtx(V4SImode);
  
  fmaxu = gen_reg_rtx(V4SImode);
  fmaxl = gen_reg_rtx(V4SImode);
  fmax = gen_reg_rtx(V4SImode); 
  mask = gen_reg_rtx(V4SImode);
  result = gen_reg_rtx(V4SImode);
  
  from1 = gen_reg_rtx(V4SFmode);  
  from = gen_reg_rtx(V4SImode);
  to = gen_reg_rtx(V4SImode);
 
  spu_emit_insn (gen_spu_convert(from, ops[1]));
  spu_emit_insn (gen_spu_ilhu(smask, GEN_INT(0x8000)));
  spu_emit_insn (gen_spu_and(sign, from, smask));
  spu_emit_insn (gen_spu_ilhu(emask, GEN_INT(0x7ff0)));
  spu_emit_insn (gen_spu_and(etmp, from, emask));
  spu_emit_insn (gen_spu_rotmi(exp, etmp, GEN_INT(-4)));
  
  spu_emit_insn (gen_spu_fsmb(ztmp1, GEN_INT(0)));
  spu_emit_insn (gen_spu_convert(from1, ops[1]));
  spu_emit_insn (gen_spu_convert(ztmp2, ztmp1)); 
  spu_emit_insn (gen_spu_fcmeq(ztmp, from1, ztmp2 ));
 
  spu_emit_insn (gen_spu_ilhu(eoffset, GEN_INT(0x380))); 
  spu_emit_insn (gen_spu_andc(eoffset1, eoffset, ztmp));
  spu_emit_insn (gen_spu_sf(exp1, exp, eoffset1));
  
  spu_emit_insn (gen_spu_rotqby(ofb, exp1, GEN_INT(-3)));
  spu_emit_insn (gen_spu_ilhu(fmaxu, GEN_INT(0x7fff)));
  spu_emit_insn (gen_spu_iohl(fmaxl, fmaxu, GEN_INT(0xffff)));
  spu_emit_insn (gen_spu_or(fmax, fmaxl, sign));
  
  spu_emit_insn (gen_spu_convert(ofbq, ofb)); 
  spu_emit_insn (gen_spu_fsmb(zero, GEN_INT(0)));
  spu_emit_insn (gen_spu_ceqb(ofq, ofbq, zero));
  spu_emit_insn (gen_spu_convert(overflow, ofq));
  spu_emit_insn (gen_spu_fsm(mask, overflow));
   
  spu_emit_insn (gen_spu_shli(e, exp1, GEN_INT(7)));
  spu_emit_insn (gen_spu_or(se, sign, e));
    
  spu_emit_insn (gen_spu_shlqbi(frac, from, GEN_INT(3)));
  spu_emit_insn (gen_spu_selb(result, frac, se, pat));
  spu_emit_insn (gen_spu_selb(to, fmax, result, mask)); 
  spu_emit_insn (gen_spu_convert(ops[0], to));
}
/* L is the length in bytes.
 * ALIGN is the alignment in bits.
 * The actual alignment when doing the copy could be higher, which is
 * why we don't use the worst case estimate. */
HOST_WIDE_INT
spu_move_by_pieces_ninsns (unsigned HOST_WIDE_INT l, unsigned int align)
{
  /* Estimate as:
   *   1 load and 1 store for every 16 bytes
   *   1 shufb for every group of bytes of size align 
   *   1 generate shufb pattern for every 128/align 
   * Actual worst case is
   *   load + rotate + load + shuf + store fore every group of align bytes 
   *   i.e. 5 * (l / balign) for align < 128
   */
  unsigned int balign = align / 8;
  unsigned HOST_WIDE_INT nshuf = l / balign;
  unsigned HOST_WIDE_INT npat = MIN (nshuf, 16 / balign);
  return (l + 15) / 16 * 2 + (align < 128 ? nshuf + npat : 0);
}

static int
put_mode_for_set(rtx *x, void *d ATTRIBUTE_UNUSED)
{
  if (*x && GET_CODE(*x) == SET)
    {
      rtx set = *x;
      if (GET_MODE(SET_SRC(set)) == VOIDmode)
	PUT_MODE(SET_SRC(set), GET_MODE(SET_DEST(set)));
      return -1;
    }
  return 0;
}

/* This is called every time we generate an "spu_*" pattern.  Many of
 * those patterns don't set the mode on SET_SRC and we fix that here. */
rtx
spu_emit_insn(rtx insn)
{
  for_each_rtx(&insn, put_mode_for_set, 0);
  return emit_insn(insn);
}

enum machine_mode
spu_eh_return_filter_mode (void)
{
  return fast_mode;
}

/* Decide whether we can make a sibling call to a function.  DECL is the
   declaration of the function being targeted by the call and EXP is the
   CALL_EXPR representing the call.  */

static bool
spu_function_ok_for_sibcall (tree decl, tree exp ATTRIBUTE_UNUSED)
{
  if (TARGET_LARGE_MEM)
    return false;

  if (!decl)
    return false;

  return true;
}

static int
spu_mode_lsb_offset (enum machine_mode mode)
{
  switch (mode)
  {
  case QImode:
  case HImode:
  case SImode:
  case SFmode:
    return 31;

  case DFmode:
  case DImode:
    return 63;

  default:
    return 127;
  }
}

int
spu_mode_offset (enum machine_mode old_mode, enum machine_mode new_mode)
{
    int old_lsb_offset, new_lsb_offset;

    old_lsb_offset = spu_mode_lsb_offset (old_mode);
    new_lsb_offset = spu_mode_lsb_offset (new_mode);

    return old_lsb_offset - new_lsb_offset;
}

void
spu_allocate_stack (rtx op0, rtx op1)
{
  rtx chain = gen_reg_rtx (V4SImode);
  rtx stack_bot = gen_rtx_MEM (V4SImode, stack_pointer_rtx);
  rtx sp = gen_rtx_SUBREG (V4SImode, stack_pointer_rtx, 0);
  rtx neg_op0;

  /* copy the back chain so we can save it back again. */
  emit_move_insn (chain, stack_bot);

  neg_op0 = gen_reg_rtx (V4SImode);
  spu_emit_insn (gen_spu_splats (neg_op0, op1));
  spu_emit_insn (gen_spu_sfi (neg_op0, const0_rtx, neg_op0));

  spu_emit_insn (gen_spu_a (sp, sp, neg_op0));

  if (flag_stack_check)
    {
      rtx result = gen_reg_rtx(SImode);
      spu_emit_insn (gen_spu_rotqby(result, sp, GEN_INT(4)));
      spu_emit_insn (gen_cgt_si(result, result, CONSTM1_RTX (SImode)));
      spu_emit_insn (gen_spu_heq (result, GEN_INT(0) ));
    }

  emit_move_insn (stack_bot, chain);

  emit_move_insn (op0, virtual_stack_dynamic_rtx);
}

void
spu_restore_stack_nonlocal (rtx op0 ATTRIBUTE_UNUSED, rtx op1)
{
  static unsigned char arr[16] = { 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3 };
  rtx temp = gen_reg_rtx (SImode);
  rtx temp2 = gen_reg_rtx (SImode);
  rtx temp3 = gen_reg_rtx (V4SImode);
  rtx temp4 = gen_reg_rtx (V4SImode);
  rtx pat = gen_reg_rtx (V16QImode);
  rtx sp = gen_rtx_SUBREG (V4SImode, stack_pointer_rtx, 0);

  /* Restore the backchain from the first word, sp from the second.  */
  emit_move_insn (temp2, adjust_address_nv (op1, SImode, 0));
  emit_move_insn (temp, adjust_address_nv (op1, SImode, 4));

  emit_move_insn (pat, array_to_constant (V16QImode, arr));

  /* Compute Available Stack Size for sp*/
  emit_insn (gen_subsi3 (temp, temp, stack_pointer_rtx));
  spu_emit_insn (gen_spu_shufb (temp3, temp, temp, pat));

  /* Compute Available Stack Size for back chain */
  emit_insn (gen_subsi3 (temp2, temp2, stack_pointer_rtx));
  spu_emit_insn (gen_spu_shufb (temp4, temp2, temp2, pat));
  spu_emit_insn (gen_spu_a (temp4, sp, temp4));

  spu_emit_insn (gen_spu_a (sp, sp, temp3));
  emit_move_insn (gen_rtx_MEM(V4SImode, stack_pointer_rtx), temp4);
}

int
spu_safe_dma(HOST_WIDE_INT channel)
{
  return TARGET_SAFE_DMA && (channel >= 21 && channel <= 27);
}

static void
spu_init_libfuncs (void)
{
  if (spu_float_acc == SPU_FP_ACCURATE)
    {
      set_optab_libfunc (sdiv_optab, SFmode, "__divv4sf3");
      set_optab_libfunc (sqrt_optab, SFmode, "__sqrtv4sf2");
    }
}

void
spu_init_expanders (void)
{
  /* HARD_FRAME_REGISTER is only 128 bit aligned when
   * frame_pointer_needed is true.  We don't know that until
   * we're expanding the prologue. */
  if (cfun)
    REGNO_POINTER_ALIGN (HARD_FRAME_POINTER_REGNUM) = 8;
}

/* Don't copy hint related code.  This is mostly for hints that were
   generated by __builtin_expect.  Other hints are generated late enough
   that GCC doesn't ever try to copy them. */
static bool
spu_cannot_copy_insn_p (rtx insn)
{
  return INSN_P (insn)
	 && (INSN_CODE (insn) == CODE_FOR_spu_hbr
	     || INSN_CODE (insn) == CODE_FOR_spu_hbrr
	     || find_reg_note (insn, REG_BR_HINT, 0));
}
