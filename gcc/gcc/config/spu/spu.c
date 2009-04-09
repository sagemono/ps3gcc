
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
#include "output.h"
#include "ddg.h"

/*  Target specific attribute specifications.  */
char regs_ever_allocated[FIRST_PSEUDO_REGISTER];

/*  Prototypes and external defs.  */

/* Prototypes generated using gcc's -aux-info flag. */
static rtx adjust_operand 			(rtx op, HOST_WIDE_INT *start);
static HOST_WIDE_INT const_double_to_hwint	(rtx);
static rtx frame_emit_store 			(int regno, rtx addr, HOST_WIDE_INT offset);
static rtx frame_emit_load 			(int regno, rtx addr, HOST_WIDE_INT offset);
static rtx frame_emit_add_imm 			(rtx dst, rtx src, HOST_WIDE_INT imm, rtx scratch);
static void pad_bb				(void);
static void emit_nop_for_insn			(rtx insn);
static void spu_emit_branch_hint 		(rtx before, rtx branch, rtx target, int distance);
static rtx get_branch_target 			(rtx branch);
static int uses_ls_unit				(rtx);
static int get_pipe				(rtx);
static tree spu_handle_fndecl_attribute 	(tree *node, tree name, tree args, int flags, bool *no_add_attrs);
static tree spu_handle_vector_attribute 	(tree *node, tree name, tree args, int flags, bool *no_add_attrs);
static int spu_naked_function_p 		(tree func);
static int spu_init_fini_function_p 		(tree func);
static int mem_is_padded_component_ref		(rtx x);
static int reg_aligned_for_addr			(rtx x, int aligned);
static bool spu_cannot_modify_jumps_p 		(void);

static void fix_range (const char *);

extern const char *reg_names[];
rtx spu_compare_op0, spu_compare_op1;

/* The hardware requires 8 insns between a hint and the branch it
   effects.  This variable describes how many rtl instructions the
   compiler needs to see before inserting a hint, and then the compiler
   will insert enough nops to make it at least 8 insns.  The default is
   for the compiler to allow up to 2 nops be emitted.  The nops are
   inserted in pairs, so we round down. */
int spu_hint_dist = (8*4) - (2*4);

/* The SPU language extensions spec documents how floating point should
 * behave.  It also allows for a fast-math option which can be set
 * indepently for floats and doubles.  We provide a compatibilty mode
 * for previous SDK users. */
int spu_float_acc = SPU_FP_FAST;
int spu_double_acc = SPU_FP_ACCURATE;

/* The ratio represents approximately how many instructions to use for
 * an inline memcpy.  Otherwise some form of memcpy will be called.  */
int spu_move_ratio = 32;

enum spu_immediate {
  SPU_NONE,
  SPU_IL,
  SPU_ILA,
  SPU_ILH,
  SPU_ILHU,
  SPU_ORI,
  SPU_ORHI,
  SPU_ORBI,
  SPU_IOHL
};
enum immediate_class
{
  IC_POOL,			/* constant pool */
  IC_IL1,			/* one il* instruction */
  IC_IL2,			/* both ilhu and iohl instructions */
  IC_IL1s,			/* one il* instruction */
  IC_IL2s,			/* both ilhu and iohl instructions */
  IC_FSMBI,			/* the fsmbi instruction */
  IC_CPAT,			/* one of the c*d instructions */
  IC_FSMBI2			/* fsmbi plus 1 other instruction */
};

static enum spu_immediate which_immediate_load (HOST_WIDE_INT val);
static enum spu_immediate which_logical_immediate (HOST_WIDE_INT val);
static int cpat_info(unsigned char *arr, int size, int *prun, int *pstart);
static enum immediate_class classify_immediate (rtx op,
						enum machine_mode mode);



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

tree spu_resolve_overloaded_builtin	PARAMS((tree, tree));
#undef TARGET_RESOLVE_OVERLOADED_BUILTIN
#define TARGET_RESOLVE_OVERLOADED_BUILTIN  spu_resolve_overloaded_builtin

rtx spu_expand_builtin	PARAMS((tree, rtx, rtx, enum machine_mode, int));
#undef TARGET_EXPAND_BUILTIN
#define TARGET_EXPAND_BUILTIN spu_expand_builtin

static bool spu_rtx_costs (rtx x, int code, int outer_code, int *total);
#undef TARGET_RTX_COSTS
#define TARGET_RTX_COSTS spu_rtx_costs

#undef TARGET_ADDRESS_COST
#define TARGET_ADDRESS_COST hook_int_rtx_0

static int spu_sched_issue_rate	PARAMS((void));
#undef TARGET_SCHED_ISSUE_RATE
#define TARGET_SCHED_ISSUE_RATE spu_sched_issue_rate

static void spu_sched_init_global PARAMS((FILE *, int, int));
#undef TARGET_SCHED_INIT_GLOBAL
#define TARGET_SCHED_INIT_GLOBAL spu_sched_init_global

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

static bool spu_sched_no_barrier_for_unspec_volatile (rtx, rtx);
#undef TARGET_SCHED_NO_BARRIER_FOR_UNSPEC_VOLATILE
#define TARGET_SCHED_NO_BARRIER_FOR_UNSPEC_VOLATILE spu_sched_no_barrier_for_unspec_volatile

const struct attribute_spec spu_attribute_table[];
#undef  TARGET_ATTRIBUTE_TABLE
#define TARGET_ATTRIBUTE_TABLE spu_attribute_table

static bool spu_assemble_integer (rtx x, unsigned int size, int aligned_p);
#undef TARGET_ASM_INTEGER
#define TARGET_ASM_INTEGER spu_assemble_integer

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

static const char* spu_invalid_conversion PARAMS ((tree, tree));
#undef TARGET_INVALID_CONVERSION
#define TARGET_INVALID_CONVERSION spu_invalid_conversion

#undef TARGET_DEFAULT_TARGET_FLAGS
#define TARGET_DEFAULT_TARGET_FLAGS (TARGET_DEFAULT)

static void spu_init_libfuncs (void);
#undef TARGET_INIT_LIBFUNCS
#define TARGET_INIT_LIBFUNCS spu_init_libfuncs

static void spu_encode_section_info(tree, rtx, int);
#undef  TARGET_ENCODE_SECTION_INFO
#define TARGET_ENCODE_SECTION_INFO spu_encode_section_info

tree spu_builtin_mask_for_load PARAMS((void));
#undef TARGET_VECTORIZE_BUILTIN_MASK_FOR_LOAD
#define TARGET_VECTORIZE_BUILTIN_MASK_FOR_LOAD spu_builtin_mask_for_load

tree spu_builtin_mul_widen_even PARAMS((tree));
#undef TARGET_VECTORIZE_BUILTIN_MUL_WIDEN_EVEN
#define TARGET_VECTORIZE_BUILTIN_MUL_WIDEN_EVEN spu_builtin_mul_widen_even

tree spu_builtin_mul_widen_odd PARAMS((tree));
#undef TARGET_VECTORIZE_BUILTIN_MUL_WIDEN_ODD
#define TARGET_VECTORIZE_BUILTIN_MUL_WIDEN_ODD spu_builtin_mul_widen_odd

static bool spu_cannot_copy_insn_p (rtx insn);
#undef TARGET_CANNOT_COPY_INSN_P
#define TARGET_CANNOT_COPY_INSN_P spu_cannot_copy_insn_p

/** SCE bugilla #11003 **/
static bool spu_ms_bitfield_layout_p (tree);
#undef TARGET_MS_BITFIELD_LAYOUT_P
#define TARGET_MS_BITFIELD_LAYOUT_P spu_ms_bitfield_layout_p

static bool spu_reverse_bitfields_p (tree);
#undef TARGET_REVERSE_BITFIELDS_P
#define TARGET_REVERSE_BITFIELDS_P spu_reverse_bitfields_p

#undef TARGET_CANNOT_MODIFY_JUMPS_P
#define TARGET_CANNOT_MODIFY_JUMPS_P spu_cannot_modify_jumps_p

struct gcc_target targetm = TARGET_INITIALIZER;

void
spu_optimization_options (int level ATTRIBUTE_UNUSED, int size)
{
/* removed to avoid lib00003333 (workaround)
  	if (level >= 2) \
	  flag_rerun_cse_before_sched = 1; */

  /* Small loops will be unpeeled at -O3.  For SPU it is more important
     to keep code small by default. */
  if (!flag_unroll_loops && !flag_peel_loops)
    PARAM_VALUE (PARAM_MAX_COMPLETELY_PEEL_TIMES) = 1;

  /* Override some of the default param values.  With so many registers
     larger values are better for these params.  */
  MAX_PENDING_LIST_LENGTH = 128;

  if (size)
    spu_dual_nops = 0;

  /* With so many registers this is better on by default. */
  flag_rename_registers = 1;

  /* We want to split before CSE, but if we split loads early, we don't
   * want to do copy-by-fields.  */
  flag_split_before_cse2 = 1;
  flag_copy_by_field = 0;
}

/* Sometimes certain combinations of command options do not make sense
   on a particular target machine.  You can define a macro
   OVERRIDE_OPTIONS to take account of this. This macro, if defined, is
   executed once just after all the command options have been parsed.  */
void
spu_override_options (void)
{
  /* Don't give warnings about the main() function. */
  warn_main = 0;

  flag_omit_frame_pointer = 1;

  /* Functions must be 8 byte aligned so we correctly handle dual issue */
  if (align_functions < 8) align_functions = 8;

  spu_hint_dist = 8*4 - spu_max_nops*4;
  if (spu_hint_dist < 0) 
    spu_hint_dist = 0;

  if (spu_fixed_range_string)
    fix_range (spu_fixed_range_string);

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

  /** SCE bugilla #11003 **/
  /* APPLE LOCAL pragma reverse_bitfields, ms_struct */
  darwin_reverse_bitfields = false;
  darwin_ms_struct = false;
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

/* GCC assumes that in a paradoxical SUBREG the inner mode occupies the
   least significant bytes of the outer mode.  This function returns
   TRUE for the SUBREG's where this is correct.  */
int
valid_subreg (rtx op)
    {
  enum machine_mode om = GET_MODE (op);
  enum machine_mode im = GET_MODE (SUBREG_REG (op));
  return om != VOIDmode && im != VOIDmode
    && (GET_MODE_SIZE (im) == GET_MODE_SIZE (om)
	|| (GET_MODE_SIZE (im) <= 4 && GET_MODE_SIZE (om) <= 4));
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

static rtx
gen_ashift(enum machine_mode mode, rtx op0, rtx op1, rtx op2)
{
  switch (mode)
  {
    case SImode: return gen_ashlsi3 (op0, op1, op2);
    case DImode: return gen_ashldi3 (op0, op1, op2);
    case TImode: return gen_ashlti3 (op0, op1, op2);
    default:
      gcc_unreachable();
  }
  gcc_unreachable();
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

    if (start + width <= 32 && src_size > 32)
      {
	rtx r = gen_reg_rtx (SImode);
	emit_insn (gen_spu_convert (r, src));
	src = r;
	src_mode = SImode;
	src_size = 32;
      }

    if (start > 0)
      {
	s = gen_reg_rtx(src_mode);
        emit_insn (gen_ashift (src_mode, s, src, GEN_INT(start)));
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
        rtx pattern = gen_reg_rtx(TImode);
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
	emit_move_insn(pattern, array_to_constant(TImode, arr));
        spu_emit_insn(gen_shufb(dst, src, dst, pattern));
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
	  emit_insn (gen_ashift (dst_mode, shift_reg, shift_reg, GEN_INT(shift)));
        else if (shift < 0)
	  goto inserterr;

        switch (dst_size)
        {
        case 32:
	    maskbits = (-1ll << (32-width-start));
	    if (start) maskbits += (1ll << (32-start));
	    emit_move_insn(mask, GEN_INT(maskbits));
	    spu_emit_insn(gen_selb(dst, dst, shift_reg, mask));
            break;
        case 64:
	    maskbits = (-1ll << (64-width-start));
	    if (start) maskbits += (1ll << (64-start));
	    emit_move_insn(mask, GEN_INT(maskbits));
	    spu_emit_insn(gen_selb(dst, dst, shift_reg, mask));
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
	      emit_move_insn(mask, array_to_constant(TImode, arr));
	      spu_emit_insn(gen_selb(dst, dst, shift_reg, mask));
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
	  spu_emit_insn(gen_selb(target, dreg, sreg, mask));
	  emit_move_insn(dst, target);
	}
      return 1;
    }

  return 0;
}

enum spu_comp_code { SPU_EQ, SPU_GT, SPU_GTU };


int spu_comp_icode[11][3] = {
 {CODE_FOR_ceq_qi, CODE_FOR_cgt_qi, CODE_FOR_clgt_qi},
 {CODE_FOR_ceq_hi, CODE_FOR_cgt_hi, CODE_FOR_clgt_hi},
 {CODE_FOR_ceq_si, CODE_FOR_cgt_si, CODE_FOR_clgt_si},
 {CODE_FOR_ceq_di, CODE_FOR_cgt_di, CODE_FOR_clgt_di},
 {CODE_FOR_ceq_ti, CODE_FOR_cgt_ti, CODE_FOR_clgt_ti},
 {CODE_FOR_ceq_sf, CODE_FOR_cgt_sf, 0},
 {CODE_FOR_ceq_df, CODE_FOR_cgt_df, 0},
 {CODE_FOR_ceq_v16qi, CODE_FOR_cgt_v16qi, CODE_FOR_clgt_v16qi},
 {CODE_FOR_ceq_v8hi,  CODE_FOR_cgt_v8hi,  CODE_FOR_clgt_v8hi},
 {CODE_FOR_ceq_v4si,  CODE_FOR_cgt_v4si,  CODE_FOR_clgt_v4si},
 {CODE_FOR_ceq_v4sf,  CODE_FOR_cgt_v4sf, 0},
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
  case V16QImode: index = 7;  comp_mode = op_mode; break;
  case V8HImode:  index = 8;  comp_mode = op_mode; break;
  case V4SImode:  index = 9;  comp_mode = op_mode; break;
  case V4SFmode:  index = 10; comp_mode = V4SImode; break;
  case V2DImode:
  case V2DFmode:
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
      && scode == SPU_EQ
      && !VECTOR_MODE_P (comp_mode))
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

      if (VECTOR_MODE_P (comp_mode))
	{
	  rtx gather = gen_reg_rtx (HImode);
	  spu_emit_insn (gen_cmp_gbb (gather, compare_result)); 
	  comp_mode = HImode;
	  compare_result = gen_reg_rtx (HImode);
	  if (reverse_test)
	    emit_insn (gen_clgt_hi (compare_result, gather, GEN_INT(0)));
	  else
	    emit_insn (gen_ceq_hi (compare_result, gather, GEN_INT(-1)));
	}
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
          bcomp = gen_rtx_EQ(comp_mode, compare_result, CONST0_RTX (GET_MODE (compare_result)));
      else
          bcomp = gen_rtx_NE(comp_mode, compare_result, CONST0_RTX (GET_MODE (compare_result)));

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
	spu_emit_insn (gen_selb(target, op_t, op_f, select_mask));
      else
	spu_emit_insn (gen_selb(target, op_f, op_t, select_mask));
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


HOST_WIDE_INT
const_double_to_hwint(rtx x)
{
  HOST_WIDE_INT val;
  REAL_VALUE_TYPE rv;
  if (GET_MODE(x) == SFmode)
    {
      REAL_VALUE_FROM_CONST_DOUBLE (rv, x);
      REAL_VALUE_TO_TARGET_SINGLE (rv, val);
    }
  else if (GET_MODE (x) == DFmode)
    {
      long l[2];
      REAL_VALUE_FROM_CONST_DOUBLE (rv, x);
      REAL_VALUE_TO_TARGET_DOUBLE (rv, l);
      val = l[0];
      val = (val << 32) | (l[1] & 0xffffffff);
    }
  else
    abort ();
  return val;
}

rtx
hwint_to_const_double (enum machine_mode mode, HOST_WIDE_INT v)
{
  long tv[2];
  REAL_VALUE_TYPE rv;
  gcc_assert (mode == SFmode || mode == DFmode);

  if (mode == SFmode)
    tv[0] = (v << 32) >> 32;
  else if (mode == DFmode)
    {
      tv[1] = (v << 32) >> 32;
      tv[0] = v >> 32;
    }
  real_from_target (&rv, tv, mode);
  return CONST_DOUBLE_FROM_REAL_VALUE (rv, mode);
}

void
print_operand_address (FILE *file, register rtx addr)
{
  rtx reg;
  rtx offset;

  if (GET_CODE (addr) == AND
      && GET_CODE (XEXP (addr, 1)) == CONST_INT
      && INTVAL (XEXP (addr, 1)) == -16)
    addr = XEXP (addr, 0);

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
	  fprintf (file, "%s,%s", reg_names[REGNO (reg)],
		   reg_names[REGNO (offset)]);
	}
      else if (GET_CODE (offset) == CONST_INT)
        {
	  /* We mask off the offset because the only time it will be
	   * unaligned is when reload changes an offset to load an HI or
	   * QI from an SI that was spilled to the stack. */
	  fprintf (file, HOST_WIDE_INT_PRINT_DEC "(%s)",
             (INTVAL (offset) & -16), reg_names[ REGNO (reg) ]);
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
print_operand (FILE * file, rtx x, int code)
{
  enum machine_mode mode = GET_MODE (x);
  HOST_WIDE_INT val;
  unsigned char arr[16];
  int xcode = GET_CODE (x);
  int i, info;
  if (GET_MODE (x) == VOIDmode)
    switch (code)
      {
      case 'L':			/* 128 bits, signed */
      case 'm':			/* 128 bits, signed */
      case 'T':			/* 128 bits, signed */
      case 't':			/* 128 bits, signed */
	mode = TImode;
	break;
      case 'K':			/* 64 bits, signed */
      case 'k':			/* 64 bits, signed */
      case 'D':			/* 64 bits, signed */
      case 'd':			/* 64 bits, signed */
	mode = DImode;
	break;
      case 'J':			/* 32 bits, signed */
      case 'j':			/* 32 bits, signed */
      case 's':			/* 32 bits, signed */
      case 'S':			/* 32 bits, signed */
	mode = SImode;
	break;
      }
  switch (code)
    {

    case 'j':			/* 32 bits, signed */
    case 'k':			/* 64 bits, signed */
    case 'm':			/* 128 bits, signed */
      if (xcode == CONST_INT
	  || xcode == CONST_DOUBLE || xcode == CONST_VECTOR)
	{
	  gcc_assert (logical_immediate_p (x, mode));
	  constant_to_array (mode, x, arr);
	  val = (arr[0] << 24) | (arr[1] << 16) | (arr[2] << 8) | arr[3];
	  val = trunc_int_for_mode (val, SImode);
	  switch (which_logical_immediate (val))
	  {
	  case SPU_ORI:
	    break;
	  case SPU_ORHI:
	    fprintf (file, "h");
	    break;
	  case SPU_ORBI:
	    fprintf (file, "b");
	    break;
	  default:
	    gcc_unreachable ();
	  }
	}
      else
	gcc_unreachable ();
      return;

    case 'J':			/* 32 bits, signed */
    case 'K':			/* 64 bits, signed */
    case 'L':			/* 128 bits, signed */
      if (xcode == CONST_INT
	  || xcode == CONST_DOUBLE || xcode == CONST_VECTOR)
	{
	  gcc_assert (logical_immediate_p (x, mode)
		      || iohl_immediate_p (x, mode));
	  constant_to_array (mode, x, arr);
	  val = (arr[0] << 24) | (arr[1] << 16) | (arr[2] << 8) | arr[3];
	  val = trunc_int_for_mode (val, SImode);
	  switch (which_logical_immediate (val))
	  {
	  case SPU_ORI:
	  case SPU_IOHL:
	    break;
	  case SPU_ORHI:
	    val = trunc_int_for_mode (val, HImode);
	    break;
	  case SPU_ORBI:
	    val = trunc_int_for_mode (val, QImode);
	    break;
	  default:
	    gcc_unreachable ();
	  }
	  fprintf (file, HOST_WIDE_INT_PRINT_DEC, val);
	}
      else
	gcc_unreachable ();
      return;

    case 't':			/* 128 bits, signed */
    case 'd':			/* 64 bits, signed */
    case 's':			/* 32 bits, signed */
      if (CONSTANT_P (x))
	{
	  enum immediate_class c = classify_immediate (x, mode);
	  switch (c)
	    {
	    case IC_IL1:
	      constant_to_array (mode, x, arr);
	      val = (arr[0] << 24) | (arr[1] << 16) | (arr[2] << 8) | arr[3];
	      val = trunc_int_for_mode (val, SImode);
	      switch (which_immediate_load (val))
		{
		case SPU_IL:
		  break;
		case SPU_ILA:
		  fprintf (file, "a");
		  break;
		case SPU_ILH:
		  fprintf (file, "h");
		  break;
		case SPU_ILHU:
		  fprintf (file, "hu");
		  break;
		default:
		  gcc_unreachable ();
		}
	      break;
	    case IC_CPAT:
	      constant_to_array (mode, x, arr);
	      cpat_info (arr, GET_MODE_SIZE (mode), &info, 0);
	      if (info == 1)
		fprintf (file, "b");
	      else if (info == 2)
		fprintf (file, "h");
	      else if (info == 4)
		fprintf (file, "w");
	      else if (info == 8)
		fprintf (file, "d");
	      break;
	    case IC_IL1s:
	      if (xcode == CONST_VECTOR)
		{
		  x = CONST_VECTOR_ELT (x, 0);
		  xcode = GET_CODE (x);
		}
	      if (xcode == SYMBOL_REF || xcode == LABEL_REF || xcode == CONST)
		fprintf (file, "a");
	      else if (xcode == HIGH)
		fprintf (file, "hu");
	      break;
	    case IC_FSMBI:
	    case IC_FSMBI2:
	    case IC_IL2:
	    case IC_IL2s:
	    case IC_POOL:
	      abort ();
	    }
	}
      else
	gcc_unreachable ();
      return;

    case 'T':			/* 128 bits, signed */
    case 'D':			/* 64 bits, signed */
    case 'S':			/* 32 bits, signed */
      if (CONSTANT_P (x))
	{
	  enum immediate_class c = classify_immediate (x, mode);
	  switch (c)
	    {
	    case IC_IL1:
	      constant_to_array (mode, x, arr);
	      val = (arr[0] << 24) | (arr[1] << 16) | (arr[2] << 8) | arr[3];
	      val = trunc_int_for_mode (val, SImode);
	      switch (which_immediate_load (val))
		{
		case SPU_IL:
		case SPU_ILA:
		  break;
		case SPU_ILH:
		case SPU_ILHU:
		  val = trunc_int_for_mode (((arr[0] << 8) | arr[1]), HImode);
		  break;
		default:
		  gcc_unreachable ();
		}
	      fprintf (file, HOST_WIDE_INT_PRINT_DEC, val);
	      break;
	    case IC_FSMBI:
	      constant_to_array (mode, x, arr);
	      val = 0;
	      for (i = 0; i < 16; i++)
		{
		  val <<= 1;
		  val |= arr[i] & 1;
		}
	      print_operand (file, GEN_INT (val), 0);
	      break;
	    case IC_CPAT:
	      constant_to_array (mode, x, arr);
	      cpat_info (arr, GET_MODE_SIZE (mode), 0, &info);
	      fprintf (file, HOST_WIDE_INT_PRINT_DEC, (HOST_WIDE_INT)info);
	      break;
	    case IC_IL1s:
	      if (xcode == HIGH)
		x = XEXP (x, 0);
	      if (GET_CODE (x) == CONST_VECTOR)
		x = CONST_VECTOR_ELT (x, 0);
	      output_addr_const (file, x);
	      if (xcode == HIGH)
		fprintf (file, "@h");
	      break;
	    case IC_IL2:
	    case IC_IL2s:
	    case IC_FSMBI2:
	    case IC_POOL:
	      abort ();
	    }
	}
      else
	gcc_unreachable ();
      return;

    case 'C':
      if (xcode == CONST_INT)
	{
	  /* Only 4 least significant bits are relevant for generate
	     control word instructions. */
	  fprintf (file, HOST_WIDE_INT_PRINT_DEC, INTVAL (x) & 15);
	  return;
	}
      break;

    case 'M':			/* print code for c*d */
      if (GET_CODE (x) == CONST_INT)
	switch (INTVAL (x))
	  {
	  case 1:
	    fprintf (file, "b");
	    break;
	  case 2:
	    fprintf (file, "h");
	    break;
	  case 4:
	    fprintf (file, "w");
	    break;
	  case 8:
	    fprintf (file, "d");
	    break;
	  default:
	    gcc_unreachable ();
	  }
      else
	gcc_unreachable ();
      return;

    case 'N':			/* Negate the operand */
      if (xcode == CONST_INT)
	fprintf (file, HOST_WIDE_INT_PRINT_DEC, -INTVAL (x));
      else if (xcode == CONST_VECTOR)
	fprintf (file, HOST_WIDE_INT_PRINT_DEC,
		 -INTVAL (CONST_VECTOR_ELT (x, 0)));
      return;

    case 'I':			/* enable/disable interrupts */
      if (xcode == CONST_INT)
	fprintf (file, "%s", INTVAL (x) == 0 ? "d" : "e");
      return;

    case 'b':			/* branch modifiers */
      if (xcode == REG)
	fprintf (file, "%s", GET_MODE (x) == HImode ? "h" : "");
      else if (COMPARISON_P (x))
	fprintf (file, "%s", xcode == NE ? "n" : "");
      return;

    case 'i':			/* indirect call */
      if (xcode == MEM)
	{
	  if (GET_CODE (XEXP (x, 0)) == REG)
	    /* Used in indirect function calls. */
	    fprintf (file, "%s", reg_names[REGNO (XEXP (x, 0))]);
	  else
	    output_address (XEXP (x, 0));
	}
      return;

    case 'p':			/* load/store */
      if (xcode == MEM)
	{
	  x = XEXP (x, 0);
	  xcode = GET_CODE (x);
	}
      if (xcode == AND)
	{
	  x = XEXP (x, 0);
	  xcode = GET_CODE (x);
	}
      if (xcode == REG)
	fprintf (file, "d");
      else if (xcode == CONST_INT)
	fprintf (file, "a");
      else if (xcode == CONST || xcode == SYMBOL_REF || xcode == LABEL_REF)
	fprintf (file, "r");
      else if (xcode == PLUS || xcode == LO_SUM)
	{
	  if (GET_CODE (XEXP (x, 1)) == REG)
	    fprintf (file, "x");
	  else
	    fprintf (file, "d");
	}
      return;

    case 'e':
      val = xcode == CONST_INT ? INTVAL (x) : INTVAL (CONST_VECTOR_ELT (x, 0));
      val &= 0x7;
      output_addr_const (file, GEN_INT (val));
      return;

    case 'f':
      val = xcode == CONST_INT ? INTVAL (x) : INTVAL (CONST_VECTOR_ELT (x, 0));
      val &= 0x1f;
      output_addr_const (file, GEN_INT (val));
      return;

    case 'g':
      val = xcode == CONST_INT ? INTVAL (x) : INTVAL (CONST_VECTOR_ELT (x, 0));
      val &= 0x3f;
      output_addr_const (file, GEN_INT (val));
      return;

    case 'h':
      val = xcode == CONST_INT ? INTVAL (x) : INTVAL (CONST_VECTOR_ELT (x, 0));
      val = (val >> 3) & 0x1f;
      output_addr_const (file, GEN_INT (val));
      return;

    case 'E':
      val = xcode == CONST_INT ? INTVAL (x) : INTVAL (CONST_VECTOR_ELT (x, 0));
      val = -val;
      val &= 0x7;
      output_addr_const (file, GEN_INT (val));
      return;

    case 'F':
      val = xcode == CONST_INT ? INTVAL (x) : INTVAL (CONST_VECTOR_ELT (x, 0));
      val = -val;
      val &= 0x1f;
      output_addr_const (file, GEN_INT (val));
      return;

    case 'G':
      val = xcode == CONST_INT ? INTVAL (x) : INTVAL (CONST_VECTOR_ELT (x, 0));
      val = -val;
      val &= 0x3f;
      output_addr_const (file, GEN_INT (val));
      return;

    case 'H':
      val = xcode == CONST_INT ? INTVAL (x) : INTVAL (CONST_VECTOR_ELT (x, 0));
      val = -(val & -8ll);
      val = (val >> 3) & 0x1f;
      output_addr_const (file, GEN_INT (val));
      return;

    case 'u':
      /* The operand of mpyui.  When it is between 0xfe00 and 0xffff it
       * needs to be sign extend from 16 bits. */
      val = xcode == CONST_INT ? INTVAL (x) : INTVAL (CONST_VECTOR_ELT (x, 0));
      val = val << (HOST_BITS_PER_WIDE_INT - 16);
      val = val >> (HOST_BITS_PER_WIDE_INT - 16); 
      output_addr_const (file, GEN_INT (val));
      return;

    case 'v':
    case 'w':
      constant_to_array (mode, x, arr);
      val = (((arr[0] << 1) + (arr[1] >> 7)) & 0xff) - 127;
      output_addr_const (file, GEN_INT (code == 'w' ? -val : val));
      return;

    case 0:
      if (xcode == REG)
	fprintf (file, "%s", reg_names[REGNO (x)]);
      else if (xcode == MEM)
	output_address (XEXP (x, 0));
      else if (xcode == CONST_VECTOR)
	print_operand (file, CONST_VECTOR_ELT (x, 0), 0);
      else
	output_addr_const (file, x);
      return;

      /* unsed letters
	              o qr t    yz
	AB            OPQR  UVWXYZ */
    default:
      output_operand_lossage ("invalid %%xn code");
    }
  gcc_unreachable ();
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
  if (!reload_completed && !reload_in_progress)
    abort();
  if (current_function_is_leaf && !regs_ever_live[LAST_ARG_REGNUM]
      && !fixed_regs[LAST_ARG_REGNUM] && !TARGET_FIXED_PIC_REG)
    pic_reg = gen_rtx_REG(SImode, LAST_ARG_REGNUM);
  return pic_reg;
}

/* Split constant addresses to handle cases that are too large. 
   Add in the pic register when in PIC mode.
   Split immediates that require more than 1 instruction. */
int
spu_split_immediate (rtx * ops)
{
  enum machine_mode mode = GET_MODE (ops[0]);
  enum immediate_class c = classify_immediate (ops[1], mode);

  switch (c)
    {
    case IC_IL2:
      {
	unsigned char arrhi[16];
	unsigned char arrlo[16];
	rtx to, hi, lo;
	int i;
	constant_to_array (mode, ops[1], arrhi);
	to = no_new_pseudos ? ops[0] : gen_reg_rtx (mode);
	for (i = 0; i < 16; i += 4)
	  {
	    arrlo[i + 2] = arrhi[i + 2];
	    arrlo[i + 3] = arrhi[i + 3];
	    arrlo[i + 0] = arrlo[i + 1] = 0;
	    arrhi[i + 2] = arrhi[i + 3] = 0;
	  }
	hi = array_to_constant (mode, arrhi);
	lo = array_to_constant (mode, arrlo);
	emit_move_insn (to, hi);
	emit_insn (gen_rtx_SET
		   (VOIDmode, ops[0], gen_rtx_IOR (mode, to, lo)));
	return 1;
      }
    case IC_FSMBI2:
      {
	unsigned char arr_fsmbi[16];
	unsigned char arr_andbi[16];
	rtx to, reg_fsmbi, reg_and;
	int i;
	enum machine_mode imode = mode;
	/* We need to do reals as ints because the constant used in the
	 * AND might not be a legitimate real constant. */
	imode = int_mode_for_mode (mode);
	constant_to_array (mode, ops[1], arr_fsmbi);
	if (imode != mode)
	  to = simplify_gen_subreg(imode, ops[0], GET_MODE (ops[0]), 0);
	else
	  to = ops[0];
	for (i = 0; i < 16; i++)
	  if (arr_fsmbi[i] != 0)
	    {
	      arr_andbi[0] = arr_fsmbi[i];
	      arr_fsmbi[i] = 0xff;
	    }
	for (i = 1; i < 16; i++)
	  arr_andbi[i] = arr_andbi[0];
	reg_fsmbi = array_to_constant (imode, arr_fsmbi);
	reg_and = array_to_constant (imode, arr_andbi);
	emit_move_insn (to, reg_fsmbi);
	emit_insn (gen_rtx_SET
		   (VOIDmode, to, gen_rtx_AND (imode, to, reg_and)));
	return 1;
      }
    case IC_POOL:
      if (reload_in_progress || reload_completed)
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
      break;
    case IC_IL1s:
    case IC_IL2s:
      if (reload_completed && GET_CODE (ops[1]) != HIGH)
	{
	  if (c == IC_IL2s)
	    {
	      emit_move_insn (ops[0], gen_rtx_HIGH (mode, ops[1]));
	      emit_move_insn (ops[0], gen_rtx_LO_SUM (mode, ops[0], ops[1]));
	    }
	  else if (flag_pic)
	    emit_insn (gen_pic (ops[0], ops[1]));
	  if (flag_pic)
	    {
	      rtx pic_reg = get_pic_reg ();
	      emit_insn (gen_add_pic (ops[0], ops[0], pic_reg, ops[1]));
	      gcc_assert (current_function_uses_pic_offset_table);
	    }
	  return flag_pic || c == IC_IL2s;
	}
      break;
    case IC_IL1:
    case IC_FSMBI:
    case IC_CPAT:
      break;
    }
  return 0;
}

static int
func_uses_pic_reg (void)
{
  rtx insn, set;
  for (insn = get_insns(); insn; insn = NEXT_INSN (insn))
    if ((set = single_set (insn))
	&& CONSTANT_P (SET_SRC (set)))
      {
	enum machine_mode mode = GET_MODE (SET_DEST (set));
	enum immediate_class c = classify_immediate (SET_SRC (set), mode);
	if (c == IC_IL1s || c == IC_IL2s || c == IC_POOL)
	  return 1;
      }
  return 0;
}

/* We want to set current_function_uses_pic_offset_table before reload
 * starts.  ORDER_REGS_FOR_LOCAL_ALLOC is a convenient hook to do it. */
void
spu_order_regs_for_local_alloc(void)
{
  if (flag_pic)
    current_function_uses_pic_offset_table = func_uses_pic_reg();
}

/* Decide when we need to save REGNO.  We handle the PIC register
 * specially, because we don't want to create a stack frame in a 
 * leaf function when we don't use it. */
static int
need_to_save_reg (int regno)
{
  if (regs_ever_live[regno] && ! call_used_regs[regno])
    return 1;
  if (flag_pic
      && regno == PIC_OFFSET_TABLE_REGNUM 
      && (current_function_uses_pic_offset_table)
      && !TARGET_FIXED_PIC_REG
      && (!current_function_is_leaf
	  || regs_ever_live[LAST_ARG_REGNUM]
	  || fixed_regs[LAST_ARG_REGNUM]))
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
    if (need_to_save_reg (regno))
      reg_save_size += 0x10;
  return reg_save_size;
}

static rtx
frame_emit_store(int regno, rtx addr, HOST_WIDE_INT offset)
{
  rtx reg = gen_rtx_REG (V4SImode, regno);
  rtx mem =
    gen_frame_mem (V4SImode, gen_rtx_PLUS (Pmode, addr, GEN_INT (offset)));
  return emit_insn (gen_movv4si (mem, reg));
}

static rtx
frame_emit_load(int regno, rtx addr, HOST_WIDE_INT offset)
{
  rtx reg = gen_rtx_REG (V4SImode, regno);
  rtx mem =
    gen_frame_mem (V4SImode, gen_rtx_PLUS (Pmode, addr, GEN_INT (offset)));
  return emit_insn (gen_movv4si (reg, mem));
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
          &&  current_function_is_leaf
          && !spu_naked_function_p (current_function_decl))
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
      if ((current_function_stdarg || current_function_args_info.nwords < MAX_REGISTER_ARGS)
	  && !(current_function_is_leaf && current_function_uses_pic_offset_table))
	return gen_rtx_REG (Pmode, LAST_ARG_REGNUM);

    case LAST_ARG_REGNUM:
      /* For stdarg the register has already been saved. */
      if (current_function_stdarg || current_function_args_info.nwords < MAX_REGISTER_ARGS-1)
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

  scratch_reg_0 = get_scratch_reg(0);
  scratch_reg_1 = get_scratch_reg(scratch_reg_0);

  if (spu_naked_function_p (current_function_decl))
    {
      if (spu_init_fini_function_p (current_function_decl))
	{
	  /* The special __init() and __fini() functions only save
	   * registers $80, $81 and $126.  Make sure other non-volatile
	   * registers are not used. */
	  for (regno = 81; regno < FIRST_PSEUDO_REGISTER; ++regno)
	    if (regno != PIC_OFFSET_TABLE_REGNUM && need_to_save_reg (regno))
	      break;
	  if (regno < FIRST_PSEUDO_REGISTER)
	    error ("%qD, in section `%s', must not use registers $82 or higher",
		   current_function_decl,
		   TREE_STRING_POINTER (DECL_SECTION_NAME (current_function_decl)));
	  if (current_function_calls_alloca || size > 0)
	    error ("%qD, in section `%s', must not use the stack",
		   current_function_decl,
		   TREE_STRING_POINTER (DECL_SECTION_NAME (current_function_decl)));
	}
      if (flag_pic && current_function_uses_pic_offset_table
	  && !TARGET_FIXED_PIC_REG)
	{
	  rtx pic_reg = get_pic_reg();
	  insn = emit_insn(gen_load_pic_offset(pic_reg, scratch_reg_0));
	  REG_NOTES(insn) = gen_rtx_EXPR_LIST (REG_MAYBE_DEAD, const0_rtx,
					       REG_NOTES (insn));
	  insn = emit_insn(gen_subsi3(pic_reg, pic_reg, scratch_reg_0));
	  REG_NOTES(insn) = gen_rtx_EXPR_LIST (REG_MAYBE_DEAD, const0_rtx,
					       REG_NOTES (insn));
	}
      return;
    }

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
      offset = -current_function_pretend_args_size;
      for (regno = 0; regno < FIRST_PSEUDO_REGISTER; ++regno)
	if (need_to_save_reg (regno))
	  {
	    offset -= 16;
	    insn = frame_emit_store (regno, sp_reg, offset);
	    RTX_FRAME_RELATED_P(insn) = 1;
	  }
    }

  if (flag_pic && current_function_uses_pic_offset_table
      && !TARGET_FIXED_PIC_REG)
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
	  insn = frame_emit_add_imm (sp_reg, sp_reg, -total_size, scratch_reg_0);
	}
      else if ( CONST_OK_FOR_LETTER_P(-total_size, 'K') )
	{
	  insn = emit_move_insn (scratch_reg_0, sp_reg);
	  insn = emit_insn (gen_addsi3 (sp_reg, sp_reg, GEN_INT (-total_size)));
	}
      else if (scratch_reg_1)
	{
	  insn = emit_move_insn (scratch_reg_0, sp_reg);
	  insn = frame_emit_add_imm (sp_reg, sp_reg, -total_size, scratch_reg_1);
	}
      else 
	{
	  /* This case doesn't use a second scratch register, but has a
	   * longer dependency chain. */
	  insn = emit_insn (gen_movsi (scratch_reg_0, GEN_INT (total_size)));
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
	}

      if (frame_pointer_needed)
	{
	  rtx fp_reg = gen_rtx_REG (Pmode, HARD_FRAME_POINTER_REGNUM);
	  HOST_WIDE_INT fp_offset = STACK_POINTER_OFFSET
	    + current_function_outgoing_args_size;
	  /* Set the new frame_pointer */
	  insn = frame_emit_add_imm (fp_reg, sp_reg, fp_offset, scratch_reg_0);
	  RTX_FRAME_RELATED_P (insn) = 1;
	  real = gen_addsi3 (fp_reg, sp_reg, GEN_INT (fp_offset));
	  REG_NOTES (insn) = 
	    gen_rtx_EXPR_LIST (REG_FRAME_RELATED_EXPR,
			       real, REG_NOTES (insn));
          REGNO_POINTER_ALIGN (HARD_FRAME_POINTER_REGNUM) = STACK_BOUNDARY;
	}

      if (flag_stack_check)
	{
	  rtx scratch_v2di = gen_rtx_REG(V2DImode, REGNO (scratch_reg_0));
	  rtx sp_v4si = gen_rtx_REG(V4SImode, STACK_POINTER_REGNUM);
	  emit_insn (gen_spu_xswd (scratch_v2di, sp_v4si));
	  emit_insn (gen_halt_stack (scratch_v2di));
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

  /* Load the link register early to improve the chance of hinting it. */
  if (REGNO (scratch_reg_0) != LINK_REGISTER_REGNUM
      && !current_function_is_leaf
      && !current_function_calls_alloca
      && total_size + 16 <= 0x1fff)
    frame_emit_load (LINK_REGISTER_REGNUM, sp_reg, total_size + 16);

  if (total_size > 0)
    {
      if (current_function_calls_alloca)
	frame_emit_load (STACK_POINTER_REGNUM, sp_reg, 0);
      else 
	frame_emit_add_imm (sp_reg, sp_reg, total_size, scratch_reg_0);


      if (saved_regs_size > 0)
	{
	  offset = -current_function_pretend_args_size;
	  for (regno = 0; regno < FIRST_PSEUDO_REGISTER; ++regno)
	    if (need_to_save_reg (regno))
	      {
		offset -= 0x10;
		frame_emit_load (regno, sp_reg, offset);
	      }
	}
    }

  if (REGNO (scratch_reg_0) == LINK_REGISTER_REGNUM
      || (!current_function_is_leaf
          && (current_function_calls_alloca
	      || total_size + 16 > 0x1fff)))
    frame_emit_load (LINK_REGISTER_REGNUM, sp_reg, 16);

  if (!sibcall_p)
    {
      emit_insn(gen_rtx_USE(VOIDmode, gen_rtx_REG(SImode, LINK_REGISTER_REGNUM)));
      jump = emit_jump_insn (gen__return ());
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

/* Given VAL, generate a constant appropriate for MODE.
   If MODE is a vector mode, every element will be VAL.
   For TImode, VAL will be zero extended to 128 bits. */
rtx
spu_const (enum machine_mode mode, HOST_WIDE_INT val)
{
  rtx inner;
  rtvec v;
  int units, i;

  gcc_assert (GET_MODE_CLASS (mode) == MODE_INT
	      || GET_MODE_CLASS (mode) == MODE_FLOAT
	      || GET_MODE_CLASS (mode) == MODE_VECTOR_INT
	      || GET_MODE_CLASS (mode) == MODE_VECTOR_FLOAT);

  if (GET_MODE_CLASS (mode) == MODE_INT)
    return immed_double_const (val, 0, mode);

  /* val is the bit representation of the float */
  if (GET_MODE_CLASS (mode) == MODE_FLOAT)
    return hwint_to_const_double (mode, val);

  if (GET_MODE_CLASS (mode) == MODE_VECTOR_INT)
    inner = immed_double_const (val, 0, GET_MODE_INNER (mode));
  else 
    inner = hwint_to_const_double (GET_MODE_INNER (mode), val);

  units = GET_MODE_NUNITS (mode);

  v = rtvec_alloc (units);

  for (i = 0; i < units; ++i)
    RTVEC_ELT (v, i) = inner;

  return gen_rtx_CONST_VECTOR (mode, v);
}

void
spu_builtin_splats (rtx ops[])
{
  enum machine_mode mode = GET_MODE(ops[0]);
  if (GET_CODE (ops[1]) == CONST_INT || GET_CODE (ops[1]) == CONST_DOUBLE)
    {
      unsigned char arr[16];
      constant_to_array (GET_MODE_INNER (mode), ops[1], arr);
      emit_move_insn (ops[0], array_to_constant (mode, arr));
    }
  else if (!flag_pic && GET_MODE (ops[0]) == V4SImode && CONSTANT_P (ops[1]))
    {
      rtvec v = rtvec_alloc (4);
      RTVEC_ELT (v, 0) = ops[1];
      RTVEC_ELT (v, 1) = ops[1];
      RTVEC_ELT (v, 2) = ops[1];
      RTVEC_ELT (v, 3) = ops[1];
      emit_move_insn (ops[0], gen_rtx_CONST_VECTOR (mode, v));
    }
  else
    {
      rtx reg = gen_reg_rtx (TImode);
      rtx shuf;
      if (GET_CODE (ops[1]) != REG
	  && GET_CODE (ops[1]) != SUBREG)
	ops[1] = force_reg (GET_MODE_INNER (mode), ops[1]);
      switch (mode)
        {
        case V2DImode:
        case V2DFmode:
	  shuf =
	    immed_double_const (0x0001020304050607ll, 0x1011121314151617ll,
				TImode);
 	 break;
        case V4SImode:
        case V4SFmode:
	  shuf =
	    immed_double_const (0x0001020300010203ll, 0x0001020300010203ll,
				TImode);
 	 break;
        case V8HImode:
	  shuf =
	    immed_double_const (0x0203020302030203ll, 0x0203020302030203ll,
				TImode);
 	 break;
        case V16QImode:
	  shuf =
	    immed_double_const (0x0303030303030303ll, 0x0303030303030303ll,
				TImode);
 	 break;
        default:
 	 abort();
        }
      emit_move_insn(reg, shuf);
      emit_insn (gen_shufb (ops[0], ops[1], ops[1], reg));
    }
}

void
spu_builtin_extract (rtx ops[])
{
  enum machine_mode mode;
  rtx rot, from, tmp;

  mode = GET_MODE (ops[1]);

  if (GET_CODE (ops[2]) == CONST_INT)
    {
      int cst = INTVAL (ops[2]);
      switch (mode)
      {
        case V16QImode:
	  emit_insn (gen_vec_extractv16qi (ops[0], ops[1], GEN_INT (cst & 0xF)));
          break;
        case V8HImode:
	  emit_insn (gen_vec_extractv8hi (ops[0], ops[1], GEN_INT (cst & 0x7)));
          break;
        case V4SFmode:
	  emit_insn (gen_vec_extractv4sf (ops[0], ops[1], GEN_INT (cst & 0x3)));
	  break;
        case V4SImode:
	  emit_insn (gen_vec_extractv4si (ops[0], ops[1], GEN_INT (cst & 0x3)));
          break;
        case V2DImode:
	  emit_insn (gen_vec_extractv2di (ops[0], ops[1], GEN_INT (cst & 0x1)));
	  break;
        case V2DFmode:
	  emit_insn (gen_vec_extractv2df (ops[0], ops[1], GEN_INT (cst & 0x1)));
          break;
        default: 
          abort ();
      }
      return;
    }

  from = spu_gen_subreg (TImode, ops[1]);
  rot = gen_reg_rtx (TImode);
  tmp = gen_reg_rtx (SImode);

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
  emit_insn (gen_rotqby_ti (rot, from, tmp));

  emit_insn (gen_spu_convert (ops[0], rot));
}

void
spu_builtin_insert (rtx ops[])
{
  enum machine_mode mode = GET_MODE (ops[0]);
  enum machine_mode imode = GET_MODE_INNER (mode);
  rtx mask = gen_reg_rtx (TImode);
  rtx offset;

  if (GET_CODE (ops[3]) == CONST_INT)
    offset = GEN_INT (INTVAL (ops[3]) * GET_MODE_SIZE (imode));
  else
    {
      offset = gen_reg_rtx (SImode);
      emit_insn (gen_mulsi3
		 (offset, ops[3], GEN_INT (GET_MODE_SIZE (imode))));
    }
  emit_insn (gen_cpat
	     (mask, stack_pointer_rtx, offset,
	      GEN_INT (GET_MODE_SIZE (imode))));
  emit_insn (gen_shufb (ops[0], ops[1], ops[2], mask));
}

void
spu_builtin_promote (rtx ops[])
{
  enum machine_mode mode, imode;
  rtx rot, from, offset;
  HOST_WIDE_INT pos;

  mode = GET_MODE (ops[0]);
  imode = GET_MODE_INNER (mode);

  from = gen_reg_rtx (TImode);
  rot = spu_gen_subreg (TImode, ops[0]);

  emit_insn (gen_spu_convert (from, ops[1]));
 
  if (GET_CODE (ops[2]) == CONST_INT)
    {
      pos = -GET_MODE_SIZE (imode) * INTVAL (ops[2]);
      if (GET_MODE_SIZE (imode) < 4)
	pos += 4 - GET_MODE_SIZE (imode);
      offset = GEN_INT (pos & 15);
    }
  else
    {
      offset = gen_reg_rtx (SImode);
      switch (mode)
      {
        case V16QImode:
	  emit_insn (gen_subsi3 (offset, GEN_INT (3), ops[2]));
          break;
        case V8HImode:
	  emit_insn (gen_subsi3 (offset, GEN_INT (1), ops[2]));
	  emit_insn (gen_addsi3 (offset, offset, offset));
          break;
        case V4SFmode:
        case V4SImode:
	  emit_insn (gen_subsi3 (offset, GEN_INT (0), ops[2]));
	  emit_insn (gen_ashlsi3 (offset, offset, GEN_INT (2)));
          break;
        case V2DImode:
        case V2DFmode:
	  emit_insn (gen_ashlsi3 (offset, ops[2], GEN_INT (3)));
          break;
        default: 
          abort();
      }
    }
  emit_insn (gen_rotqby_ti (rot, from, offset));
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

/* 1 when RTX is a code_label that has been hinted.  */
#define LABEL_HINTED_P(RTX)						\
  (RTL_FLAG_CHECK2("LABEL_HINTED_P", (RTX), CODE_LABEL, NOTE)->unchanging)

/* 1 when this instruction needs to be scheduled on an even address */
#define SCHED_ON_EVEN_P(RTX)						\
  (RTL_FLAG_CHECK2("LABEL_HINTED_P", (RTX), JUMP_INSN, CALL_INSN)->call)

/* Emit a nop for INSN such that the two will dual issue.  This assumes
   INSN is 8-byte aligned.  When INSN is inline asm we emit an lnop.
   We check for TImode to handle a MULTI1 insn which has dual issued its
   first instruction.  get_pipe returns -1 for MULTI0, inline asm, or
   ADDR_VEC insns. */
static void
emit_nop_for_insn (rtx insn)
{
  int p;
  rtx new_insn;
  p = get_pipe(insn);
  if ((CALL_P (insn) || JUMP_P (insn)) && SCHED_ON_EVEN_P (insn))
    new_insn = emit_insn_after (gen_lnop (), insn);
  else if (p == 1 && GET_MODE (insn) == TImode)
    {
      new_insn = emit_insn_before (gen_nopn (GEN_INT (127)), insn);
      PUT_MODE(new_insn, TImode);
      PUT_MODE(insn, VOIDmode);
    }
  else
    new_insn = emit_insn_after (gen_lnop (), insn);
  INSN_BLOCK_CYCLE(new_insn) = INSN_BLOCK_CYCLE(insn);
  recog_memoized(new_insn);
}

/* Insert nops in basic blocks to meet dual issue alignment
 * requirements.  Also make sure hbrp and hint instructions are at least
 * one cycle apart, possibly inserting a nop.  */
static void
pad_bb(void)
{
  rtx insn, next_insn, prev_insn, hbr_insn = 0;
  int length;
  int addr;

  /* This sets up INSN_ADDRESSES. */
  shorten_branches (get_insns ());

  /* Keep track of length added by nops. */
  length = 0;

  prev_insn = 0;
  insn = get_insns ();
  if (!active_insn_p (insn))
    insn = next_active_insn (insn);
  for (; insn; insn = next_insn)
    {
      next_insn = next_active_insn (insn);
      if (INSN_CODE (insn) == CODE_FOR_iprefetch
	  || INSN_CODE (insn) == CODE_FOR_hbr)
	{
	  if (hbr_insn)
	    {
	      int a0 = INSN_ADDRESSES (INSN_UID (hbr_insn));
	      int a1 = INSN_ADDRESSES (INSN_UID (insn));
	      if ((a1 - a0 == 8 && GET_MODE (insn) != TImode)
		  || (a1 - a0 == 4))
		{
		  prev_insn = emit_insn_before (gen_lnop (), insn);
		  PUT_MODE (prev_insn, GET_MODE (insn));
		  PUT_MODE (insn, TImode);
		  length += 4;
		}
	    }
	  hbr_insn = insn;
	}
      if (INSN_CODE (insn) == CODE_FOR_blockage)
      {
	if (GET_MODE (insn) == TImode)
	  PUT_MODE (next_insn, TImode);
	insn = next_insn;
	next_insn = next_active_insn (insn);
      }
      addr = INSN_ADDRESSES (INSN_UID (insn));
      if ((CALL_P (insn) || JUMP_P (insn)) && SCHED_ON_EVEN_P (insn))
	{
	  if (((addr + length) & 7) != 0)
	    {
	      emit_nop_for_insn (prev_insn);
	      length += 4;
	    }
	}
      else if (GET_MODE (insn) == TImode
	  && ((next_insn && GET_MODE (next_insn) != TImode)
	      || get_attr_type (insn) == TYPE_MULTI0)
	  && ((addr + length) & 7) != 0)
	{
	  /* prev_insn will always be set because the first insn is
	     always 8-byte aligned. */
	  emit_nop_for_insn (prev_insn);
	  length += 4;
	}
      prev_insn = insn;
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
		      int distance) /* estimate of maximum distance from hint
				       to branch. */
{
  rtx branch_label = 0;
  rtx hint;
  rtx insn;
  rtx note;
  rtx table;

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
      distance -= 4;
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

  hint = gen_hbr (branch_label, target);

  hint = emit_insn_before (hint, before);

  if (note)
    XEXP (XEXP (note, 0), 0) = hint;
  else
    REG_NOTES (branch)
      = gen_rtx_EXPR_LIST (REG_BR_HINT,
			   gen_rtx_INSN_LIST(0, hint, 0), REG_NOTES (branch));
  recog_memoized(hint);

  /* This hint was emitted by fixup_user_hints and will be emitted again
     later. */
  if (distance == -1)
    return;

  if (GET_CODE (target) == LABEL_REF)
    LABEL_HINTED_P (XEXP (target, 0)) = 1;
  else if (tablejump_p (branch, 0, &table))
    {
      rtvec vec;
      int j;
      if (GET_CODE (PATTERN (table)) == ADDR_VEC)
	vec = XVEC (PATTERN (table), 0);
      else
	vec = XVEC (PATTERN (table), 1);
      for (j = GET_NUM_ELEM (vec) - 1; j >= 0; --j)
	LABEL_HINTED_P (XEXP (RTVEC_ELT (vec, j), 0)) = 1;
    }

  if (distance >= 588)
    {
      /* Make sure the hint isn't scheduled any earlier than this point,
         which could make it too far for the branch offest to fit */
      recog_memoized (emit_insn_before (gen_blockage (), hint));
    }
  else if (distance <= 8*4)
    {
      /* To guarantee at least 8 insns between the hint and branch we
         insert nops. */
      int d;
      for (d = distance; d < 8*4; d += 4)
        {
          insn = emit_insn_after (gen_nopn_nv(gen_rtx_REG (SImode, 127)), hint);
          recog_memoized (insn);
        }

      /* Make sure any nops inserted aren't scheduled before the hint. */
      recog_memoized (emit_insn_after (gen_blockage (), hint));

      /* Make sure any nops inserted aren't scheduled after the call. */
      if (CALL_P (branch) && distance < 8*4)
	recog_memoized (emit_insn_before (gen_blockage (), branch));
    }
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
  rtx insn, note, prefetch_hint = 0, prefetch_call = 0, prefetch_target = 0, branch_target;
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
	if (INSN_CODE (insn) == CODE_FOR_hbr
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
	    spu_emit_branch_hint (insn, insn, branch_target, -1);
	    prefetch_hint = 0;
	  }
      }
  if (prefetch_call)
    SET_INSN_DELETED (XEXP (XEXP (prefetch_call, 0), 0));
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

/* Search up to 32 insns starting at FIRST:
 * - at any kind of hinted branch, just return
 * - at any unconditional branch in the first 15 insns, just return
 * - at a call or indirect branch, after the first 15 insns, force it to
 *   an even address and return
 * - at any unconditional branch, after the first 15 insns, force it to
 *   an even address. 
 * At then end of the search, insert an hbrp within 4 insns of FIRST,
 * and an hbrp within 16 instructions of FIRST.
 */
static void
insert_hbrp_for_ilb_runout (rtx first)
{
  rtx insn, before_4 = 0, before_16 = 0;
  int addr = 0, length, first_addr = -1;
  int hbrp_addr0 = 128*4, hbrp_addr1 = 128*4;
  for (insn = first; insn; insn = NEXT_INSN (insn))
    if (INSN_P (insn))
      {
	if (first_addr == -1)
	  first_addr = INSN_ADDRESSES (INSN_UID (insn)); 
	addr = INSN_ADDRESSES (INSN_UID (insn)) - first_addr;
	length = get_attr_length (insn);

	if (before_4 == 0 && addr + length >= 4 * 4)
	  before_4 = insn;
	if (before_16 == 0 && addr + length >= 16 * 4)
	  before_16 = insn;

	if (INSN_CODE (insn) == CODE_FOR_iprefetch)
	  {
	    if (addr < hbrp_addr0)
	      hbrp_addr0 = addr;
	    else if (addr < hbrp_addr1)
	      hbrp_addr1 = addr;
	  }

	if (CALL_P (insn) || JUMP_P (insn))
	  {
	    if (find_reg_note (insn, REG_BR_HINT, 0))
	      return;

	    /* Any branch after the first 15 insns should be on an even
	     * address to avoid a special case branch.  There might be
	     * some nops and/or hbrps inserted, so we test after 10
	     * insns. */
	    if (addr > 10 * 4)
	      SCHED_ON_EVEN_P (insn) = 1;
	  }

	if (CALL_P (insn) || tablejump_p (insn, 0, 0))
	  return;


	if (addr + length >= 32 * 4)
	  {
	    gcc_assert (before_4 && before_16);
	    if (hbrp_addr0 > 4 * 4)
	      {
		recog_memoized (insn = emit_insn_before (gen_iprefetch (GEN_INT (1)), before_4));
		INSN_ADDRESSES_NEW (insn, INSN_ADDRESSES (INSN_UID (before_4)));
		PUT_MODE (insn, GET_MODE (before_4));
		PUT_MODE (before_4, TImode);
	      }
	    if ((hbrp_addr0 <= 4 * 4 || hbrp_addr0 > 16 * 4)
		&& hbrp_addr1 > 16 * 4)
	      {
		recog_memoized (insn = emit_insn_before (gen_iprefetch (GEN_INT (2)), before_16));
		INSN_ADDRESSES_NEW (insn, INSN_ADDRESSES (INSN_UID (before_16)));
		PUT_MODE (insn, GET_MODE (before_16));
		PUT_MODE (before_16, TImode);
	      }
	    return;
	  }
      }
    else if (BARRIER_P (insn))
      return;

}

/* The SPU might hang when it executes 48 inline instructions after a
 * hinted branch jumps to its hinted target.  The beginning of a
 * function and the return from a call might have been hinted, and must
 * be handled as well.  To prevent a hang we insert 2 hbrps.  The first
 * should be within 6 insns of the branch target.  The second should be
 * within 22 insns of the branch target.  When determining if hbrps are
 * necessary, we look for only 32 inline instructions, because up to to
 * 12 nops and 4 hbrps could be inserted.  Similarily, when inserting
 * new hbrps, we insert them within 4 and 16 insns of the target.  */
static void
insert_hbrp (void)
{
  rtx insn;
  if (TARGET_SAFE_HINTS)
    {
      shorten_branches (get_insns ());
      /* Insert hbrp at beginning of function */
      insn = next_active_insn (get_insns ());
      if (insn)
	insert_hbrp_for_ilb_runout (insn);
      /* Insert hbrp after hinted targets. */
      for (insn = get_insns (); insn; insn = NEXT_INSN (insn))
	if ((LABEL_P (insn) && LABEL_HINTED_P (insn)) || CALL_P (insn))
	  insert_hbrp_for_ilb_runout (next_active_insn (insn));
    }
}

static int in_spu_reorg;

/* Insert branch hints.  There are no branch optimizations after this
 * pass, so it's safe to set our branch hints now. */
static void
spu_machine_dependent_reorg (void)
{
  basic_block bb;
  rtx branch, insn, note, block_insn;
  rtx branch_target = 0;
  int branch_addr = 0, insn_addr, required_dist = 0;
  int i, max;
  unsigned int j;
  struct rtx_def *save_var_loc; /* sce local , Bz #20822 */

  if (!TARGET_BRANCH_HINTS || optimize == 0)
    {
      /* We still do it for unoptimized code because an external
       * function might have hinted a call or return. */
      insert_hbrp ();
      pad_bb();
      return;
    }

  in_spu_reorg = 1;

  compact_blocks();

  spu_bb_info = (struct spu_bb_info *)xcalloc(n_basic_blocks, sizeof(struct spu_bb_info));

  /* We need exact insn addresses and lengths.  */
  shorten_branches (get_insns ());

  fixup_user_hints();

  shorten_branches (get_insns ());

  for (i = n_basic_blocks-1; i >= 0; i--)
    {
      bb = BASIC_BLOCK (i);
      block_insn = branch = 0;
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
		      if (block_insn && GET_CODE (branch_target) == REG
			  && branch_addr - next_addr < 24 * 4)
			recog_memoized (emit_insn_before (gen_blockage (), block_insn));
		      if (dump_file)
			fprintf(dump_file, "hint for %i in block %i before %i\n",
				INSN_UID (branch), bb->index, INSN_UID (next));
		      spu_emit_branch_hint (next, branch, branch_target,
					    branch_addr - next_addr);
		    }
		  block_insn = branch = 0;
		}
	      /* When a hint target is a register, scheduling might
	         cause the hint to get too close to the branch.  We
	         prevent this by inserting a blockage instruction
		 with at least 8 insns between. */
	      if (branch && GET_CODE (branch_target) == REG
		  && branch_addr - insn_addr == 8 * 4)
		block_insn = insn;

	      /* JUMP_P will only be true at the end of a block.  When
	       * branch is already set it means we've previously decided
	       * to propagate a hint for that branch into this block. */
	      if (CALL_P (insn) || (JUMP_P (insn) && !branch))
		{
		  block_insn = branch = 0;
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
	  if (prev && (loop_exit || simple_loop)
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
		   && (GET_CODE (branch_target) != REG
		       || REGNO (branch_target) == LINK_REGISTER_REGNUM))
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
				    branch_addr - next_addr);
	    }
	}
    }
  free(spu_bb_info);

  /* begin sce local , Bz #20822 */
  save_var_loc = XCNEWVEC (struct rtx_def, (get_max_uid () + 1));
  for (insn = get_insns (), max = 0; insn; insn = NEXT_INSN (insn))
    {
      if (NOTE_P (insn)
	  && NOTE_LINE_NUMBER (insn) == NOTE_INSN_VAR_LOCATION)
	save_var_loc[max++] = *insn;
    }
  /* end sce local , Bz #20822 */

  /* We have to schedule to make sure alignment is ok. */
  FOR_EACH_BB (bb)
    bb->flags &= ~BB_DISABLE_SCHEDULE;

  /* The hints need to be scheduled, so call it again.  We could disable
   * the second scheduling pass in certain situations, but we don't
   * bother for now. */
  compute_bb_for_insn();
  update_life_info (0, UPDATE_LIFE_GLOBAL, 0);
  schedule_insns(dump_file);
  free_bb_for_insn();

  insert_hbrp ();

  pad_bb();

  /* begin sce local , Bz #20822 */
  for (i = 0; i < max; i++)
    {
      for (insn = get_insns (); insn; insn = NEXT_INSN (insn))
	{
	  if (INSN_UID (&save_var_loc[i]) == INSN_UID (insn))
	    {
	      rtx prev = PREV_INSN (&save_var_loc[i]);
	      NEXT_INSN (PREV_INSN (insn)) = NEXT_INSN (insn);
	      PREV_INSN (NEXT_INSN (insn)) = PREV_INSN (insn);
	      PREV_INSN (NEXT_INSN (prev)) = insn;
	      NEXT_INSN (insn) = NEXT_INSN (prev);
	      NEXT_INSN (prev) = insn;
	      PREV_INSN (insn) = prev;
	      break;
	    }
	}
    }
  free (save_var_loc);
  /* end sce local , Bz #20822 */

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
	  branch_label = XEXP(hbr_branch (hint), 0);
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

void
spu_split_trunc_shift_asm (rtx operands[], int unsigned_p, int ashift)
{
  enum machine_mode mode0 = GET_MODE (operands[0]);
  enum machine_mode mode1 = GET_MODE (operands[1]);
  HOST_WIDE_INT lshift, rshift, trunc;
  HOST_WIDE_INT dst_size = GET_MODE_BITSIZE (mode0);
  int reg0, reg1;
  trunc = GET_MODE_BITSIZE (mode1) - 32;
  if (ashift)
    {
      if (INTVAL (operands[3]) > trunc + 32 - dst_size)
	{
	  lshift = INTVAL (operands[2]);
	  rshift = INTVAL (operands[3]) - trunc;
	}
      else if (INTVAL (operands[2]) + trunc > INTVAL (operands[3]))
	{
	  lshift = INTVAL (operands[2]) + trunc - INTVAL (operands[3]);
	  rshift = 0;
	}
      else 
	{
	  lshift = 0;
	  rshift =  INTVAL (operands[3]) - (INTVAL (operands[2]) + trunc) ;
	}
    }
  else if (INTVAL (operands[2]) > trunc)
    {
      lshift = 0;
      rshift = INTVAL (operands[2]) - trunc;
    }
  else
    {
      lshift = trunc - INTVAL (operands[2]);
      rshift = 0;
    }
  
  if (GET_CODE(operands[0]) == SUBREG)
    reg0 = REGNO(SUBREG_REG(operands[0]));
  else
    reg0 = REGNO(operands[0]);

  if (GET_CODE(operands[1]) == SUBREG)
    reg1 = REGNO(SUBREG_REG(operands[1]));
  else
    reg1 = REGNO(operands[1]);

  if (mode1 == DImode && lshift - rshift > trunc)
    {
      emit_insn (gen_rtx_SET (VOIDmode,
			      gen_rtx_REG (TImode, reg0),
			      gen_rtx_ASHIFT (TImode,
					      gen_rtx_REG (TImode, reg1),
					      GEN_INT (trunc))));
      lshift -= trunc;
      emit_insn (gen_rtx_SET (VOIDmode,
			      gen_rtx_REG (SImode, reg0),
			      gen_rtx_ASHIFT (SImode,
					      gen_rtx_REG (SImode, reg0),
					      GEN_INT (lshift))));
      reg1 = reg0;
    }

  else if (lshift)
    {
      enum machine_mode lmode;
      if (lshift > rshift)
	lmode = TImode;
      else
	lmode = SImode;
      emit_insn (gen_rtx_SET (VOIDmode,
			      gen_rtx_REG (lmode, reg0),
			      gen_rtx_ASHIFT (lmode,
					      gen_rtx_REG (lmode, reg1),
					      GEN_INT (lshift))));
      reg1 = reg0;
    }

  if (rshift)
    {
      rtx s;
      if (unsigned_p)
	s = gen_rtx_LSHIFTRT (SImode, gen_rtx_REG (SImode, reg1), GEN_INT (rshift));
      else
	s = gen_rtx_ASHIFTRT (SImode, gen_rtx_REG (SImode, reg1), GEN_INT (rshift));
      emit_insn (gen_rtx_SET (VOIDmode, gen_rtx_REG (SImode, reg0), s));
      reg1 = reg0;
    }

  if (lshift == 0 && rshift == 0)
    {
      emit_insn(gen_rtx_SET(VOIDmode,
			    gen_rtx_REG(mode0, reg0),
                            gen_rtx_REG(mode0, reg1)));
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
    case TYPE_MULTI0:
      return -1;

    case TYPE_FX2:
    case TYPE_FX3:
    case TYPE_NOP:
    case TYPE_FXB:
    case TYPE_FPD:
    case TYPE_FP6:
    case TYPE_FP7:
      return 0;

    case TYPE_LNOP:
    case TYPE_SHUF:
    case TYPE_LOAD:
    case TYPE_STORE:
    case TYPE_BR:
    case TYPE_MULTI1:
    case TYPE_HBR:
    case TYPE_IPREFETCH:
    case TYPE_SPR:
      return 1;
    default:
      abort();
    }
}

static bool
channel_insn_p (rtx insn)
{
  return INSN_CODE (insn) == CODE_FOR_spu_rdch_clobber
	 || INSN_CODE (insn) == CODE_FOR_spu_rchcnt_clobber
	 || INSN_CODE (insn) == CODE_FOR_spu_wrch_clobber
	 || INSN_CODE (insn) == CODE_FOR_spu_rdch_noclobber
	 || INSN_CODE (insn) == CODE_FOR_spu_rchcnt_noclobber
	 || INSN_CODE (insn) == CODE_FOR_spu_wrch_noclobber;
}

/* This is used to keep track of insn alignment.  Set to 0 at the
 * beginning of each block and increased by the "length" attr of each
 * insn scheduled. */
static int spu_sched_length;

/* Record when we've issued pipe0 and pipe1 insns so we can reorder the
 * ready list appropriately in spu_sched_reorder(). */
static int pipe0_clock;
static int pipe1_clock;

static int prev_clock_var;

/* Some branches need to be put on an 8-byte boundary to avoid a
   potential SPU hang. */
static int sched_last_even;

static int prev_priority;

/* The SPU needs to load the next ilb sometime during the execution of
 * the previous ilb.  There is a potential conflict if every cycle has a
 * load or store.  To avoid the conflict we make sure the load/store
 * unit is free for at least one cycle during the execution of insns in
 * the previous ilb. */
static int spu_ls_first;
static int prev_ls_clock;

static void
spu_sched_init_global (FILE *file ATTRIBUTE_UNUSED, int verbose ATTRIBUTE_UNUSED,
		       int max_ready ATTRIBUTE_UNUSED)
{
  sched_last_even = 0;
  spu_sched_length = 0;
}

static void
spu_sched_init (FILE *file ATTRIBUTE_UNUSED, int verbose ATTRIBUTE_UNUSED,
		int max_ready ATTRIBUTE_UNUSED)
{
  if (align_labels > 4
      || align_loops > 4
      || align_jumps > 4)
    {
      /* When any block might be at least 8-byte aligned, assume they
       * will all be at least 8-byte aligned to make sure dual issue
       * works out correctly. */
      sched_last_even = 0;
      spu_sched_length = 0;
    }
  spu_ls_first = INT_MAX;
  prev_ls_clock = -1;
  pipe0_clock = -1;
  pipe1_clock = -1;
  prev_clock_var = -1;
  prev_priority = -1;
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
  int sched_last_even2;
  if (GET_CODE (PATTERN (insn)) == USE
      || GET_CODE (PATTERN (insn)) == CLOBBER
      || (len = get_attr_length(insn)) == 0)
    return more;

  spu_sched_length += len;

  /* Reset on inline asm */
  if (INSN_CODE (insn) == -1)
    {
      spu_ls_first = INT_MAX;
      pipe0_clock = -1;
      pipe1_clock = -1;
      return 0;
    }
  p = get_pipe(insn);
  if (p == 0)
    pipe0_clock = clock_var;
  else
    pipe1_clock = clock_var;

  if (in_spu_reorg)
    {
      if (clock_var - prev_ls_clock > 1
	  || INSN_CODE (insn) == CODE_FOR_iprefetch)
	spu_ls_first = INT_MAX;
      if (uses_ls_unit(insn))
	{
	  if (spu_ls_first == INT_MAX)
	    spu_ls_first = spu_sched_length;
	  prev_ls_clock = clock_var;
	}

      /* The scheduler hasn't inserted the nop, but we will later on.
       * Include those nops in spu_sched_length. */
      if (prev_clock_var == clock_var && (spu_sched_length & 7))
	spu_sched_length += 4;
      prev_clock_var = clock_var;
  
      /* more is -1 when called from spu_sched_reorder for new insns
       * that don't have INSN_PRIORITY */
      if (more >= 0)
	prev_priority = INSN_PRIORITY (insn);

      sched_last_even2 = sched_last_even;
      sched_last_even = (JUMP_P (insn) || CALL_P (insn)) && SCHED_ON_EVEN_P (insn);
      if (sched_last_even && ((spu_sched_length - len) & 7) != 0)
	spu_sched_length += 4;
      /* When the instruction issued 2 instructions ago was even, we want
       * the instruction following it, this one, to single issue. */
      if (sched_last_even2)
	return 0;
    }

  /* Always try issueing more insns.  spu_sched_reorder will decide 
     when the cycle should be advanced. */
  return 1;
}

static int
max_queued_priority (int n_cycles)
{
  rtx link, insn;
  int i;
  int p = 0;
  for (i = 1; i <= n_cycles; i++)
    for (link = get_queued_insns (i); link; link = XEXP (link, 1))
      {
	insn = XEXP (link, 0);
	if (INSN_PRIORITY (insn) > p)
	  p = INSN_PRIORITY (insn);
      }
  return p;
}

/* This function is called for both TARGET_SCHED_REORDER and
 * TARGET_SCHED_REORDER2.  */
static int
spu_sched_reorder (FILE *file ATTRIBUTE_UNUSED, int verbose ATTRIBUTE_UNUSED,
		   rtx *ready, int *nreadyp, int clock)
{
  int i, nready = *nreadyp;
  int pipe_0, pipe_1, pipe_hbrp, pipe_ls, schedule_i, pipe_early;
  rtx insn;

  if (nready <= 0 || pipe1_clock >= clock)
    return 0;

  /* Find any rtl insns that don't generate assembly insns and schedule
   * them first. */
  for (i = nready - 1; i >= 0; i--)
    {
      insn = ready[i];
      if (INSN_CODE (insn) == -1
	  || INSN_CODE (insn) == CODE_FOR_blockage 
	  || INSN_CODE (insn) == CODE_FOR__spu_convert)
	{
	  ready[i] = ready[nready-1];
	  ready[nready-1] = insn;
	  return 1;
	}
    }

  /* When the previous instruction had to be scheduled on an even
   * boundary, this one will be on an odd boundary and won't dual
   * issue, so just schedule the highest priority insn.  */
  if (sched_last_even)
    return 1;

  pipe_0 = pipe_1 = pipe_hbrp = pipe_ls = pipe_early = schedule_i = -1;
  for (i = 0; i < nready; i++)
    if (INSN_CODE (ready[i]) != -1)
      {
	insn = ready[i];
	if (!in_spu_reorg && INSN_SCHED_EARLY (insn))
	  pipe_early = i;
	switch (get_attr_type (insn))
	  {
	  default:
	  case TYPE_MULTI0:
	  case TYPE_CONVERT:
	  case TYPE_FX2:
	  case TYPE_FX3:
	  case TYPE_NOP:
	  case TYPE_FXB:
	  case TYPE_FPD:
	  case TYPE_FP6:
	  case TYPE_FP7:
	    pipe_0 = i;
	    break;
	  case TYPE_LOAD:
	  case TYPE_STORE:
	    pipe_ls = i;
	  case TYPE_LNOP:
	  case TYPE_SHUF:
	  case TYPE_BR:
	  case TYPE_MULTI1:
	  case TYPE_HBR:
	  case TYPE_SPR:
	    pipe_1 = i;
	    break;
	  case TYPE_IPREFETCH:
	    pipe_hbrp = i;
	    break;
	  }
      }

  /* In the first scheduling phase, schedule loads and stores together
   * to increase the chance they will get merged during postreload CSE. */
  if (!reload_completed && pipe_ls >= 0)
    {
      insn = ready[pipe_ls];
      ready[pipe_ls] = ready[nready-1];
      ready[nready-1] = insn;
      return 1;
    }

  /* In the first scheduling phase, schedule register copies as soon as
   * possible to make it more likely the copy will be eliminated. */
  if (!in_spu_reorg && pipe_early >= 0)
    {
      insn = ready[pipe_early];
      ready[pipe_early] = ready[nready-1];
      ready[nready-1] = insn;
      return 1;
    }

  /* If there is an hbrp ready, prefer it over other pipe 1 insns. */
  if (pipe_hbrp >= 0)
    pipe_1 = pipe_hbrp;

  /* When we have loads/stores in every cycle of the last 15 insns and
   * we are about to schedule another load/store, emit an hbrp insn
   * instead. */
  if (in_spu_reorg
      && spu_sched_length - spu_ls_first >= 4 * 15
      && !(pipe0_clock < clock && pipe_0 >= 0)
      && pipe_1 == pipe_ls)
    {
      insn = sched_emit_insn (gen_iprefetch (GEN_INT (3)));
      recog_memoized (insn);
      if (pipe0_clock < clock_var)
	PUT_MODE (insn, TImode);
      spu_sched_variable_issue (file, verbose, insn, -1);
      return 0;
    }

  /* In general, we want to emit nops to increase dual issue, but dual
   * issue isn't faster when one of the insns could be scheduled later
   * without effecting the critical path.  We look at INSN_PRIORITY to
   * make a good guess, but it isn't perfect so -mdual-nops=n can be
   * used to effect it. */
  if (in_spu_reorg && spu_dual_nops < 10)
    {
      /* When we are at an even address and we are not issueing nops to
	 improve scheduling then we need to advance the cycle.  */
      if ((spu_sched_length & 7) == 0 && prev_clock_var == clock
	  && (spu_dual_nops == 0
	      || (pipe_1 != -1
		  && prev_priority > INSN_PRIORITY (ready[pipe_1]) + spu_dual_nops)))
	return 0;

      /* When at an odd address, schedule the highest priority insn
       * without considering pipeline. */
      if ((spu_sched_length & 7) == 4 && prev_clock_var != clock
	  && (spu_dual_nops == 0
	      || (prev_priority > max_queued_priority (spu_dual_nops/2) + spu_dual_nops
		  && prev_priority > INSN_PRIORITY (ready[nready-1]) + spu_dual_nops)))
	return 1;
    }


  /* We haven't issued a pipe0 insn yet this cycle, if there is a
   * pipe0 insn in the ready list, schedule it. */
  if (pipe0_clock < clock && pipe_0 >= 0)
    schedule_i = pipe_0;

  /* Either we've scheduled a pipe0 insn already or there is no pipe0
   * insn to schedule.  Put a pipe1 insn at the front of the ready list. */
  else
    {
      schedule_i = pipe_1;

      if (in_spu_reorg
	  && pipe0_clock >= clock
	  && pipe_hbrp == -1
	  /* && (spu_sched_length & 7) == 4 */)
	{
	  /* Check for the case where we can convert a pipe0 insns into
	   * pipe1 insns.  Only do it if it's more profitable than using an
	   * existing pipe1 insn in the ready queue. */
	  rtx pat, r0, r1;
	  enum machine_mode mode;
	  for (i = nready-1; i > pipe_1; i--)
	    {
	      insn = ready[i];

	      if (pipe_1 > -1
		  && INSN_PRIORITY(ready[pipe_1])+1 >= INSN_PRIORITY(ready[i]))
		break;

	      pat = PATTERN(insn);
	      if (GET_CODE (pat) != SET)
		continue;

	      mode = GET_MODE(XEXP(pat, 0));
	      if (GET_CODE(pat) == SET
		  && GET_CODE(XEXP(pat, 0)) == REG
		  && GET_CODE(XEXP(pat, 1)) == REG
		  && !RTX_FRAME_RELATED_P(insn) )
		{
		  r0 = gen_rtx_REG(TImode, REGNO(XEXP(pat, 0)));
		  r1 = gen_rtx_REG(TImode, REGNO(XEXP(pat, 1)));
		  XEXP(pat, 0) = r0;
		  XEXP(pat, 1) = gen_rtx_ASHIFT(TImode, r1, const0_rtx);
		  INSN_CODE (insn) = recog (PATTERN (insn), insn, 0);
		  dfa_insn_code_reset (insn);
		  schedule_i = i;
		  break;
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
		  XEXP(pat, 1) = gen_rtx_UNSPEC(V16QImode, v, UNSPEC_FSMB);
		  INSN_CODE (insn) = recog (PATTERN (insn), insn, 0);
		  dfa_insn_code_reset (insn);
		  schedule_i = i;
		  break;
		}
#if 0
	      else if (INSN_CODE(insn) == CODE_FOR_lshrsi3
		       && GET_CODE(XEXP(SET_SRC(pat), 1)) == CONST_INT)
		{
		  HOST_WIDE_INT v = INTVAL(XEXP(SET_SRC(pat), 1));
		  if ((v % 8) == 0 || (v / 8) == 0)
		    {
		      r0 = gen_rtx_REG(TImode, REGNO(XEXP(pat, 0)));
		      r1 = gen_rtx_REG(TImode, REGNO(XEXP(SET_SRC(pat), 0)));
		      XEXP(pat, 0) = r0;
		      XEXP(pat, 1) = gen_rtx_LSHIFTRT(TImode, r1, GEN_INT (v));
		      INSN_CODE (insn) = recog (PATTERN (insn), insn, 0);
		      dfa_insn_code_reset (insn);
		      ready[i] = ready[nready-1];
		      ready[nready-1] = insn;
		      return 1;
		    }
		}
#endif
	    }
	}
    }
  if (schedule_i > -1)
    {
      insn = ready[schedule_i];
      ready[schedule_i] = ready[nready-1];
      ready[nready-1] = insn;
      return 1;
    }
  return 0;
}

/* INSN is dependent on DEP_INSN. */
static int
spu_sched_adjust_cost (rtx insn, rtx link, rtx dep_insn, int cost)
{
  rtx set;

  /* The blockage pattern is used to prevent instructions from being
     moved across it and has no cost. */
  if (INSN_CODE (insn) == CODE_FOR_blockage
      || INSN_CODE (dep_insn) == CODE_FOR_blockage)
    return 0;

  if (INSN_CODE (insn) == CODE_FOR__spu_convert
      || INSN_CODE (dep_insn) == CODE_FOR__spu_convert)
    return 0;

  /* Make sure hbrps are spread out. */
  if (INSN_CODE (insn) == CODE_FOR_iprefetch
      && INSN_CODE (dep_insn) == CODE_FOR_iprefetch)
    return 8;

  /* Make sure hints and hbrps are 2 cycles apart. */
  if ((INSN_CODE (insn) == CODE_FOR_iprefetch
       || INSN_CODE (insn) == CODE_FOR_hbr)
       && (INSN_CODE (dep_insn) == CODE_FOR_iprefetch
	   || INSN_CODE (dep_insn) == CODE_FOR_hbr))
    return 2;

  /* An hbrp has no real dependency on other insns. */
  if (INSN_CODE (insn) == CODE_FOR_iprefetch
      || INSN_CODE (dep_insn) == CODE_FOR_iprefetch)
    return 0;

  /* Give a copy instruction a cost of 0 during the first scheduling
   * pass so it gets scheduled with the instruction which makes it
   * more likely the copy instruction gets deleted. */
  if (!in_spu_reorg
      && INSN_DEPEND (dep_insn)
      && (! XEXP (INSN_DEPEND (dep_insn), 1)
          || ! XEXP (XEXP (INSN_DEPEND (dep_insn), 1), 1))
      && (set = single_set (insn))
      && register_operand (SET_SRC (set), VOIDmode)
      && register_operand (SET_DEST (set), VOIDmode))
    {
      INSN_SCHED_EARLY (insn) = 1;
      return 0;
    }

  /* Assuming that it is unlikely an argument register will be used in
   * the first cycle of the called function, we reduce the cost for
   * slightly better scheduling of dep_insn.  When not hinted, the
   * mispredicted branch would hide the cost as well.  */
  if (CALL_P (insn))
  {
    rtx target = get_branch_target (insn);
    if (GET_CODE (target) != REG || !set_of (target, insn))
      return cost - 2;
    return cost;
  }

  /* And when returning from a function, let's assume the return values
   * are completed sooner too. */
  if (CALL_P (dep_insn))
    return cost - 2;

  /* Make sure an instruction that loads from the back chain is schedule
   * away from the return instruction so a hint is more likely to get
   * issued. */
  if (INSN_CODE (insn) == CODE_FOR__return
      && (set = single_set (dep_insn))
      && GET_CODE (SET_DEST (set)) == REG
      && REGNO (SET_DEST (set)) == LINK_REGISTER_REGNUM)
    return 20;

  /* The dfa scheduler sets cost to 0 for all anti-dependencies and the
   * scheduler makes every insn in a block anti-dependent on the final
   * jump_insn.  We adjust here so higher cost insns will get scheduled
   * earlier. */
  if (JUMP_P (insn) && REG_NOTE_KIND (link) == REG_DEP_ANTI)
    return INSN_COST(dep_insn) - 3;

  /* The channel instructions have fake dependencies on register 131 to
   * prevent scheduling across each other, but it doesn't cause a stall.
   * We ignore the case where the result of a rdch is being used in a
   * wrch; it should cause a stall. */
  if (channel_insn_p (insn) && channel_insn_p (dep_insn))
    return 1;

  return cost;
}

static int
spu_sched_adjust_priority (
  rtx insn ATTRIBUTE_UNUSED,
  int priority)
{
  return priority;
}

static bool
spu_sched_no_barrier_for_unspec_volatile (rtx insn, rtx unspec ATTRIBUTE_UNUSED)
{
  return channel_insn_p (insn);
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
  return (GET_CODE (x) == LABEL_REF || GET_CODE (x) == SYMBOL_REF
	  || GET_CODE (x) == CONST_INT || GET_CODE (x) == CONST
	  || GET_CODE (x) == HIGH);
}

static enum spu_immediate
which_immediate_load (HOST_WIDE_INT val)
{
  gcc_assert (val == trunc_int_for_mode (val, SImode));

  if (val >= -0x8000 && val <= 0x7fff)
    return SPU_IL;
  if (val >= 0 && val <= 0x3ffff)
    return SPU_ILA;
  if ((val & 0xffff) == ((val >> 16) & 0xffff))
    return SPU_ILH;
  if ((val & 0xffff) == 0)
    return SPU_ILHU;

  return SPU_NONE;
}

/* Return true when OP can be loaded by one of the il instructions, or
   when flow2 is not completed and OP can be loaded using ilhu and iohl. */
int
immediate_load_p (rtx op, enum machine_mode mode)
{
  if (CONSTANT_P (op))
    {
      enum immediate_class c = classify_immediate (op, mode);
      return c == IC_IL1 || c == IC_IL1s
	     || (!flow2_completed && (c == IC_IL2 || c == IC_IL2s));
    }
  return 0;
}

/* Return true if the first SIZE bytes of arr is a constant that can be
   generated with cbd, chd, cwd or cdd.  When non-NULL, PRUN and PSTART
   represent the size and offset of the instruction to use. */
static int
cpat_info(unsigned char *arr, int size, int *prun, int *pstart)
{
  int cpat, run, i, start;
  cpat = 1;
  run = 0;
  start = -1;
  for (i = 0; i < size && cpat; i++)
    if (arr[i] != i+16)
      { 
	if (!run)
	  {
	    start = i;
	    if (arr[i] == 3)
	      run = 1;
	    else if (arr[i] == 2 && arr[i+1] == 3)
	      run = 2;
	    else if (arr[i] == 0)
	      {
		while (arr[i+run] == run && i+run < 16)
		  run++;
		if (run != 4 && run != 8)
		  cpat = 0;
	      }
	    else
	      cpat = 0;
	    if ((i & (run-1)) != 0)
	      cpat = 0;
	    i += run;
	  }
	else
	  cpat = 0;
      }
  if (cpat && (run || size < 16))
    {
      if (run == 0)
	run = 1;
      if (prun)
	*prun = run;
      if (pstart)
	*pstart = start == -1 ? 16-run : start;
      return 1;
    }
  return 0;
}

/* OP is a CONSTANT_P.  Determine what instructions can be used to load
   it into a regiser.  MODE is only valid when OP is a CONST_INT. */
static enum immediate_class
classify_immediate (rtx op, enum machine_mode mode)
{
  HOST_WIDE_INT val;
  unsigned char arr[16];
  int i, j, repeated, fsmbi, repeat;

  gcc_assert (CONSTANT_P (op));

  if (GET_MODE (op) != VOIDmode)
    mode = GET_MODE (op);

  /* A V4SI const_vector with all identical symbols is ok. */
  if (!flag_pic
      && mode == V4SImode
      && GET_CODE (op) == CONST_VECTOR
      && GET_CODE (CONST_VECTOR_ELT (op, 0)) != CONST_INT
      && CONST_VECTOR_ELT (op, 0) == CONST_VECTOR_ELT (op, 1)
      && CONST_VECTOR_ELT (op, 1) == CONST_VECTOR_ELT (op, 2)
      && CONST_VECTOR_ELT (op, 2) == CONST_VECTOR_ELT (op, 3))
    op = CONST_VECTOR_ELT (op, 0);

  switch (GET_CODE (op))
    {
    case SYMBOL_REF:
    case LABEL_REF:
      return TARGET_LARGE_MEM ? IC_IL2s : IC_IL1s;

    case CONST:
      /* We can never know if the resulting address fits in 18 bits and can be
	 loaded with ila.  For now, assume the address will not overflow if
	 the displacement is "small" (fits 'K' constraint).  */
      if (!TARGET_LARGE_MEM && GET_CODE (XEXP (op, 0)) == PLUS)
	{
	  rtx sym = XEXP (XEXP (op, 0), 0);
	  rtx cst = XEXP (XEXP (op, 0), 1);

	  if (GET_CODE (sym) == SYMBOL_REF
	      && GET_CODE (cst) == CONST_INT
	      && satisfies_constraint_K (cst))
	    return IC_IL1s;
	}
      return IC_IL2s;

    case HIGH:
      return IC_IL1s;

    case CONST_VECTOR:
      for (i = 0; i < GET_MODE_NUNITS (mode); i++)
	if (GET_CODE (CONST_VECTOR_ELT (op, i)) != CONST_INT
	    && GET_CODE (CONST_VECTOR_ELT (op, i)) != CONST_DOUBLE)
	  return IC_POOL;
      /* Fall through. */

    case CONST_INT:
    case CONST_DOUBLE:
      constant_to_array (mode, op, arr);

      /* Check that each 4-byte slot is identical. */
      repeated = 1;
      for (i = 4; i < 16; i += 4)
	for (j = 0; j < 4; j++)
	  if (arr[j] != arr[i + j])
	    repeated = 0;

      if (repeated)
	{
	  val = (arr[0] << 24) | (arr[1] << 16) | (arr[2] << 8) | arr[3];
	  val = trunc_int_for_mode (val, SImode);

	  if (which_immediate_load (val) != SPU_NONE)
	    return IC_IL1;
	}

      /* Any mode of 2 bytes or smaller can be loaded with an il
         instruction. */
      gcc_assert (GET_MODE_SIZE (mode) > 2);

      fsmbi = 1;
      repeat = 0;
      for (i = 0; i < 16 && fsmbi; i++)
	if (arr[i] != 0 && repeat == 0)
	  repeat = arr[i];
	else if (arr[i] != 0 && arr[i] != repeat)
	  fsmbi = 0;
      if (fsmbi)
	return repeat == 0xff ? IC_FSMBI : IC_FSMBI2;

      if (cpat_info (arr, GET_MODE_SIZE (mode), 0, 0))
	return IC_CPAT;

      if (repeated)
	return IC_IL2;

      return IC_POOL;
    default:
      break;
    }
  gcc_unreachable ();
}

static enum spu_immediate
which_logical_immediate (HOST_WIDE_INT val)
{
  gcc_assert (val == trunc_int_for_mode (val, SImode));

  if (val >= -0x200 && val <= 0x1ff)
    return SPU_ORI;
  if (val >= 0 && val <= 0xffff)
    return SPU_IOHL;
  if ((val & 0xffff) == ((val >> 16) & 0xffff))
    {
      val = trunc_int_for_mode (val, HImode);
      if (val >= -0x200 && val <= 0x1ff)
	return SPU_ORHI;
      if ((val & 0xff) == ((val >> 8) & 0xff))
	{
	  val = trunc_int_for_mode (val, QImode);
	  if (val >= -0x200 && val <= 0x1ff)
	    return SPU_ORBI;
	}
    }
  return SPU_NONE;
}

/* Return TRUE when X, a CONST_VECTOR, only contains CONST_INTs or
   CONST_DOUBLEs. */
static int
const_vector_immediate_p (rtx x)
{
  int i;
  gcc_assert (GET_CODE (x) == CONST_VECTOR);
  for (i = 0; i < GET_MODE_NUNITS (GET_MODE (x)); i++)
    if (GET_CODE (CONST_VECTOR_ELT (x, i)) != CONST_INT
	&& GET_CODE (CONST_VECTOR_ELT (x, i)) != CONST_DOUBLE)
      return 0;
  return 1;
}

int
logical_immediate_p (rtx op, enum machine_mode mode)
{
  HOST_WIDE_INT val;
  unsigned char arr[16];
  int i, j;

  gcc_assert (GET_CODE (op) == CONST_INT || GET_CODE (op) == CONST_DOUBLE
	      || GET_CODE (op) == CONST_VECTOR);

  if (GET_CODE (op) == CONST_VECTOR
      && !const_vector_immediate_p (op))
    return 0;

  if (GET_MODE (op) != VOIDmode)
    mode = GET_MODE (op);

  constant_to_array (mode, op, arr);

  /* Check that bytes are repeated. */
  for (i = 4; i < 16; i += 4)
    for (j = 0; j < 4; j++)
      if (arr[j] != arr[i + j])
	return 0;

  val = (arr[0] << 24) | (arr[1] << 16) | (arr[2] << 8) | arr[3];
  val = trunc_int_for_mode (val, SImode);

  i = which_logical_immediate (val);
  return i != SPU_NONE && i != SPU_IOHL;
}

int
iohl_immediate_p (rtx op, enum machine_mode mode)
{
  HOST_WIDE_INT val;
  unsigned char arr[16];
  int i, j;

  gcc_assert (GET_CODE (op) == CONST_INT || GET_CODE (op) == CONST_DOUBLE
	      || GET_CODE (op) == CONST_VECTOR);

  if (GET_CODE (op) == CONST_VECTOR
      && !const_vector_immediate_p (op))
    return 0;

  if (GET_MODE (op) != VOIDmode)
    mode = GET_MODE (op);

  constant_to_array (mode, op, arr);

  /* Check that bytes are repeated. */
  for (i = 4; i < 16; i += 4)
    for (j = 0; j < 4; j++)
      if (arr[j] != arr[i + j])
	return 0;

  val = (arr[0] << 24) | (arr[1] << 16) | (arr[2] << 8) | arr[3];
  val = trunc_int_for_mode (val, SImode);

  return val >= 0 && val <= 0xffff;
}

int
arith_immediate_p (rtx op, enum machine_mode mode,
		   HOST_WIDE_INT low, HOST_WIDE_INT high)
{
  HOST_WIDE_INT val;
  unsigned char arr[16];
  int bytes, i, j;

  gcc_assert (GET_CODE (op) == CONST_INT || GET_CODE (op) == CONST_DOUBLE
	      || GET_CODE (op) == CONST_VECTOR);

  if (GET_CODE (op) == CONST_VECTOR
      && !const_vector_immediate_p (op))
    return 0;

  if (GET_MODE (op) != VOIDmode)
    mode = GET_MODE (op);

  constant_to_array (mode, op, arr);

  if (VECTOR_MODE_P (mode))
    mode = GET_MODE_INNER (mode);

  bytes = GET_MODE_SIZE (mode);
  mode = mode_for_size (GET_MODE_BITSIZE (mode), MODE_INT, 0);

  /* Check that bytes are repeated. */
  for (i = bytes; i < 16; i += bytes)
    for (j = 0; j < bytes; j++)
      if (arr[j] != arr[i + j])
	return 0;

  val = arr[0];
  for (j = 1; j < bytes; j++)
    val = (val << 8) | arr[j];

  val = trunc_int_for_mode (val, mode);

  return val >= low && val <= high;
}

/* TRUE when op is an immediate and an exact power of 2, and given that
   OP is 2^scale, scale >= LOW && scale <= HIGH.  When OP is a vector,
   all entries must be the same. */
bool
exp2_immediate_p (rtx op, enum machine_mode mode, int low, int high)
{
  enum machine_mode int_mode;
  HOST_WIDE_INT val;
  unsigned char arr[16];
  int bytes, i, j;

  gcc_assert (GET_CODE (op) == CONST_INT || GET_CODE (op) == CONST_DOUBLE
	      || GET_CODE (op) == CONST_VECTOR);

  if (GET_CODE (op) == CONST_VECTOR
      && !const_vector_immediate_p (op))
    return 0;

  if (GET_MODE (op) != VOIDmode)
    mode = GET_MODE (op);

  constant_to_array (mode, op, arr);

  if (VECTOR_MODE_P (mode))
    mode = GET_MODE_INNER (mode);

  bytes = GET_MODE_SIZE (mode);
  int_mode = mode_for_size (GET_MODE_BITSIZE (mode), MODE_INT, 0);

  /* Check that bytes are repeated. */
  for (i = bytes; i < 16; i += bytes)
    for (j = 0; j < bytes; j++)
      if (arr[j] != arr[i + j])
	return 0;

  val = arr[0];
  for (j = 1; j < bytes; j++)
    val = (val << 8) | arr[j];

  val = trunc_int_for_mode (val, int_mode);

  /* Currently, we only handle SFmode */
  gcc_assert (mode == SFmode);
  if (mode == SFmode)
    {
      int exp = (val >> 23) - 127;
      return val > 0 && (val & 0x007fffff) == 0
	     &&  exp >= low && exp <= high;
    }
  return FALSE;
}

/* We only reject CONST_VECTOR's that contain symbolic addresses, but we
   do accept a V4SI const_vector when all symbol elements are identical. */
int
spu_legitimate_constant_p(rtx x)
{
  if (GET_CODE (x) == HIGH)
    x = XEXP (x, 0);
  /* V4SI with all identical symbols is valid. */
  if (!flag_pic
      && GET_CODE (x) == CONST_VECTOR
      && GET_MODE (x) == V4SImode
      && (GET_CODE (CONST_VECTOR_ELT (x, 0)) == SYMBOL_REF
	  || GET_CODE (CONST_VECTOR_ELT (x, 0)) == LABEL_REF
	  || GET_CODE (CONST_VECTOR_ELT (x, 0)) == CONST))
    return CONST_VECTOR_ELT (x, 0) == CONST_VECTOR_ELT (x, 1)
	   && CONST_VECTOR_ELT (x, 1) == CONST_VECTOR_ELT (x, 2)
	   && CONST_VECTOR_ELT (x, 2) == CONST_VECTOR_ELT (x, 3);

  if (GET_CODE (x) == CONST_VECTOR
      && !const_vector_immediate_p (x))
    return 0;
  return 1;
}

/* Valid address are:
   - symbol_ref, label_ref, const
   - reg
   - reg + const, where const is 16 byte aligned
   - reg + reg, alignment doesn't matter
  The alignment matters in the reg+const case because lqd and stqd
  ignore the 4 least significant bits of the const.  

  Addresses are handled in 4 phases. 
  1) from the beginning of rtl expansion until the split0 pass.  Any
     address is acceptable.  
  2) The split0 pass. It is responsible for making every load and store
     valid.  It calls legitimate_address with FOR_SPLIT set to 1.  This
     is where non-16-byte aligned loads/stores are split into multiple
     instructions to extract or insert just the part we care about.
  3) From the split0 pass to the beginning of reload.  During this
     phase the constant part of an address must be 16 byte aligned, and
     we don't allow any loads/store of less than 4 bytes.  We also
     allow a mask of -16 to be part of the address as an optimization.
  4) From reload until the end.  Reload can change the modes of loads
     and stores to something smaller than 4-bytes which we need to allow
     now, and it also adjusts the address to match.  So in this phase we
     allow that special case.  Still allow addresses with a mask of -16.

  FOR_SPLIT is only set to 1 for phase 2, otherwise it is 0.  */
int
spu_legitimate_address (enum machine_mode mode, rtx x, int reg_ok_strict, int for_split)
{
  int aligned = (split0_completed || for_split)
		 && !reload_in_progress && !reload_completed;
  int const_aligned = split0_completed || for_split;
  if (GET_MODE_SIZE (mode) >= 16)
    aligned = 0;
  else if (aligned && GET_MODE_SIZE (mode) < 4)
    return 0;
  if (split0_completed
      && (GET_CODE (x) == AND
	  && GET_CODE (XEXP (x, 1)) == CONST_INT
	  && INTVAL (XEXP (x, 1)) == (HOST_WIDE_INT) - 16
          && !CONSTANT_P (XEXP (x, 0))))
    x = XEXP (x, 0);
  switch (GET_CODE (x))
    {
    case LABEL_REF:
      return !TARGET_LARGE_MEM && !aligned;

    case SYMBOL_REF:
      return !TARGET_LARGE_MEM && (!aligned || ALIGNED_SYMBOL_REF_P (x));

    case CONST:
      if (!TARGET_LARGE_MEM && GET_CODE (XEXP (x, 0)) == PLUS)
	{
	  rtx sym = XEXP (XEXP (x, 0), 0);
	  rtx cst = XEXP (XEXP (x, 0), 1);

	  /* Accept any symbol_ref + constant, assuming it does not
	     wrap around the local store addressability limit.  */
	  if (GET_CODE (sym) == SYMBOL_REF && GET_CODE (cst) == CONST_INT)
	    {
	      /* Check for alignment if required.  */
	      if (!aligned)
		return 1;
	      if ((INTVAL (cst) & 15) == 0 && ALIGNED_SYMBOL_REF_P (sym))
		return 1;
	    }
	}
      return 0;

    case CONST_INT:
      /* We don't test alignement here.  For an absolute address we
         assume the user knows what they are doing. */
      return INTVAL (x) >= 0 && INTVAL (x) <= 0x3ffff;

    case SUBREG:
      x = XEXP (x, 0);
      if (GET_CODE (x) != REG)
	return 0;

    case REG:
      return INT_REG_OK_FOR_BASE_P (x, reg_ok_strict)
	&& reg_aligned_for_addr (x, 0);

    case PLUS:
    case LO_SUM:
      {
	rtx op0 = XEXP (x, 0);
	rtx op1 = XEXP (x, 1);
	if (GET_CODE (op0) == SUBREG)
	  op0 = XEXP (op0, 0);
	if (GET_CODE (op1) == SUBREG)
	  op1 = XEXP (op1, 0);
	if (GET_CODE (op0) == REG
	    && INT_REG_OK_FOR_BASE_P (op0, reg_ok_strict)
	    && GET_CODE (op1) == CONST_INT
	    && INTVAL (op1) >= -0x2000
	    && INTVAL (op1) <= 0x1fff
	    && reg_aligned_for_addr (op0, 0)
	    && (!const_aligned
		|| (INTVAL (op1) & 15) == 0
		|| ((reload_in_progress || reload_completed)
		    && GET_MODE_SIZE (mode) < 4
		    && (INTVAL (op1) & 15) == 4 - GET_MODE_SIZE (mode))
		/* Some passes create a fake register for testing valid
		 * addresses, be more lenient when we see those.  ivopts
		 * and reload do it. */
		|| REGNO (op0) == LAST_VIRTUAL_REGISTER + 1
		|| REGNO (op0) == LAST_VIRTUAL_REGISTER + 2))
	  return 1;
	if (GET_CODE (op0) == REG
	    && INT_REG_OK_FOR_BASE_P (op0, reg_ok_strict)
	    && reg_aligned_for_addr (op0, 0)
	    && GET_CODE (op1) == REG
	    && INT_REG_OK_FOR_INDEX_P (op1, reg_ok_strict)
	    && reg_aligned_for_addr (op1, 0))
	  return 1;
      }
      break;

    default:
      break;
    }
  return 0;
}

/* When the address is reg + const_int, force the const_int into a
   regiser. */
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
      if (spu_legitimate_address (mode, x, 0, 0))
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
      warning (0, "`%s' attribute only applies to functions",
	       IDENTIFIER_POINTER (name));
      *no_add_attrs = true;
    }

  return NULL_TREE;
}

/* Handle the "vector" attribute.  */
static tree
spu_handle_vector_attribute (tree * node, tree name,
			     tree args ATTRIBUTE_UNUSED,
			     int flags ATTRIBUTE_UNUSED, bool *no_add_attrs)
{
  tree type = *node, result = NULL_TREE;
  enum machine_mode mode;
  int unsigned_p;

  while (POINTER_TYPE_P (type)
	 || TREE_CODE (type) == FUNCTION_TYPE
	 || TREE_CODE (type) == METHOD_TYPE || TREE_CODE (type) == ARRAY_TYPE)
    type = TREE_TYPE (type);

  mode = TYPE_MODE (type);

  unsigned_p = TYPE_UNSIGNED (type);
  switch (mode)
    {
    case DImode:
      result = (unsigned_p ? unsigned_V2DI_type_node : V2DI_type_node);
      break;
    case SImode:
      result = (unsigned_p ? unsigned_V4SI_type_node : V4SI_type_node);
      break;
    case HImode:
      result = (unsigned_p ? unsigned_V8HI_type_node : V8HI_type_node);
      break;
    case QImode:
      result = (unsigned_p ? unsigned_V16QI_type_node : V16QI_type_node);
      break;
    case SFmode:
      result = V4SF_type_node;
      break;
    case DFmode:
      result = V2DF_type_node;
      break;
    default:
      break;
    }

  /* Propagate qualifiers attached to the element type
     onto the vector type.  */
  if (result && result != type && TYPE_QUALS (type))
    result = build_qualified_type (result, TYPE_QUALS (type));

  *no_add_attrs = true;  /* No need to hang on to the attribute.  */

  if (!result)
    warning (0, "`%s' attribute ignored", IDENTIFIER_POINTER (name));
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
    return 0;
  
  a = lookup_attribute ("naked", DECL_ATTRIBUTES (func));
  return (a != NULL_TREE || spu_init_fini_function_p (func));
}

/* Return non-zero if FUNC is marked with section(".init") or
 * section(".fini"). */
static int
spu_init_fini_function_p (tree func)
{
  const char *name;
  if (!func || TREE_CODE (func) != FUNCTION_DECL
      || !DECL_SECTION_NAME (func))
    return 0;
  name = TREE_STRING_POINTER (DECL_SECTION_NAME (func));
  return ((strncmp (name, ".init", 5) == 0
	   || strncmp (name, ".fini", 5) == 0)
	  && (name[5] == 0 || name[5] == '.'));
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


/* Parse the -mfixed-range= option string.  */
static void
fix_range (const char *const_str)
{
  int i, first, last;
  char *str, *dash, *comma;

  /* str must be of the form REG1'-'REG2{,REG1'-'REG} where REG1 and
     REG2 are either register names or register numbers.  The effect
     of this option is to mark the registers in the range from REG1 to
     REG2 as ``fixed'' so they won't be used by the compiler.  */

  i = strlen (const_str);
  str = (char *) alloca (i + 1);
  memcpy (str, const_str, i + 1);

  while (1)
    {
      dash = strchr (str, '-');
      if (!dash)
        {
          warning (0, "value of -mfixed-range must have form REG1-REG2");
          return;
        }
      *dash = '\0';

      comma = strchr (dash + 1, ',');
      if (comma)
        *comma = '\0';

      first = decode_reg_name (str);
      if (first < 0)
        {
          warning (0, "unknown register name: %s", str);
          return;
        }

      last = decode_reg_name (dash + 1);
      if (last < 0)
        {
          warning (0, "unknown register name: %s", dash + 1);
          return;
        }

      *dash = '-';

      if (first > last)
        {
          warning (0, "%s-%s is an empty range", str, dash + 1);
          return;
        }

      for (i = first; i <= last; ++i)
        fixed_regs[i] = call_used_regs[i] = 1;

      if (!comma)
        break;

      *comma = ',';
      str = comma + 1;
    }
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
spu_function_arg (CUMULATIVE_ARGS *cum,
		  enum machine_mode mode,
		  tree type, int named ATTRIBUTE_UNUSED)
{
  int byte_size;
  int nwords = cum->nwords;
  int new_nwords;


  if (nwords >= MAX_REGISTER_ARGS)
    return 0;

  byte_size = ((mode == BLKmode)
              ? int_size_in_bytes (type) : GET_MODE_SIZE (mode));

  /* The ABI does not allow parameters to be passed partially in
   * reg and partially in stack. */
  if ((new_nwords = nwords + (byte_size + 15) / 16) > MAX_REGISTER_ARGS)
    return 0;

  cum->nregs_used = new_nwords;	

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
				  gen_rtx_REG (smode, FIRST_ARG_REGNUM + nwords),
				  const0_rtx);
      return gen_rtx_PARALLEL (mode, gen_rtvec (1, gr_reg));
    }
  else
    return gen_rtx_REG (mode, FIRST_ARG_REGNUM + nwords);
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
      int regno;
      int offset;
      CUMULATIVE_ARGS ncum = *cum;

      /* cum currently points to the last named argument, we want to
       * start at the next argument. */
      FUNCTION_ARG_ADVANCE(ncum, mode, type, 1);

      offset = -STACK_POINTER_OFFSET;
      for (regno = ncum.nwords; regno < MAX_REGISTER_ARGS; regno++)
	{
	  tmp = gen_frame_mem (V4SImode,
			       plus_constant (virtual_incoming_args_rtx,
					      offset));
	  emit_move_insn (tmp,
			  gen_rtx_REG (V4SImode, FIRST_ARG_REGNUM + regno));
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

/* This is called any time we inspect the alignment of a register for
 * addresses.  */
static int 
reg_aligned_for_addr (rtx x, int aligned)
{
  int regno = REGNO (x) < FIRST_PSEUDO_REGISTER ? ORIGINAL_REGNO (x) : REGNO (x);
  if (!aligned)
    return 1;
  return REGNO_POINTER_ALIGN (regno) >= 128;
}

/* Encode symbol attributes (local vs. global, tls model) of a SYMBOL_REF
   into its SYMBOL_REF_FLAGS.  */
static void
spu_encode_section_info (tree decl, rtx rtl, int first)
{
  default_encode_section_info (decl, rtl, first);

  /* If a variable has a forced alignment to < 16 bytes, mark it with
     SYMBOL_FLAG_ALIGN1 have the real user defined alignment.  */
  if (TREE_CODE (decl) == VAR_DECL
      && DECL_USER_ALIGN (decl) && DECL_ALIGN (decl) < 128)
  {
    if (!lookup_attribute ("section", DECL_ATTRIBUTES (decl)))
      warning (0,
	       "%qD has %d-byte alignment.  An alignment less than 16-byte is not supported in default sections",
	       decl, DECL_ALIGN (decl) / BITS_PER_UNIT );
    SYMBOL_REF_FLAGS (XEXP (rtl, 0)) |= SYMBOL_FLAG_ALIGN1;
  }
}

/* Return TRUE if we are certain the mem refers to a complete object
   which is both 16-byte aligned and padded to a 16-byte boundary.  This
   would make it safe to store with a single instruction. 
   We guarantee the alignment and padding for static objects by aligning
   all of them to 16-bytes. (DATA_ALIGNMENT and CONSTANT_ALIGNMENT.)
   FIXME: We currently cannot guarantee this for objects on the stack
   because assign_parm_setup_stack calls assign_stack_local with the
   alignment of the parameter mode and in that case the alignment never
   gets adjusted by LOCAL_ALIGNMENT. */
static int
store_with_one_insn_p (rtx mem)
{
  enum machine_mode mode = GET_MODE (mem);
  rtx addr = XEXP (mem, 0);
  if (mode == BLKmode)
    return 0;
  if (GET_MODE_SIZE (mode) >= 16)
    return 1;
  /* Only static objects. */
  if (GET_CODE (addr) == SYMBOL_REF)
    {
      /* We use the associated declaration to make sure the access is
         refering to the whole object.
         We check both MEM_EXPR and and SYMBOL_REF_DECL.  I'm not sure
         if it is necessary.  Will there be cases where one exists, and
         the other does not?  Will there be cases where both exist, but
         have different types?  */
      tree decl = MEM_EXPR (mem);
      if (decl
	  && TREE_CODE (decl) == VAR_DECL
	  && GET_MODE (mem) == TYPE_MODE (TREE_TYPE (decl)))
	return 1;
      decl = SYMBOL_REF_DECL (addr);
      if (decl
	  && TREE_CODE (decl) == VAR_DECL
	  && GET_MODE (mem) == TYPE_MODE (TREE_TYPE (decl)))
	return 1;
    }
  return 0;
}

/* Return 1 when the address is not valid for a simple load and store as
 * required by the '_mov*' patterns.   We could make this less strict
 * for loads, but we prefer mem's to look the same so they are more
 * likely to be merged.  */
static int
address_needs_split (rtx mem)
{
  if (GET_MODE_SIZE (GET_MODE (mem)) < 16
      && (GET_MODE_SIZE (GET_MODE (mem)) < 4
	  || !(store_with_one_insn_p (mem)
	       || mem_is_padded_component_ref (mem))))
    return 1;

  return 0;
}

int
spu_expand_mov(rtx *ops, enum machine_mode mode)
{
  if (GET_CODE (ops[0]) == SUBREG && !valid_subreg (ops[0]))
    abort ();

  if (GET_CODE (ops[1]) == SUBREG && !valid_subreg (ops[1]))
    {
      rtx from = SUBREG_REG (ops[1]);
      enum machine_mode imode = GET_MODE (from);

      gcc_assert (GET_MODE_CLASS (mode) == MODE_INT
		  && GET_MODE_CLASS (imode) == MODE_INT
		  && subreg_lowpart_p (ops[1]));

      if (GET_MODE_SIZE (imode) < 4)
	{
	  from = gen_rtx_SUBREG (SImode, from, 0);
	  imode = SImode;
	}

      if (GET_MODE_SIZE (mode) < GET_MODE_SIZE (imode))
	{
	  enum insn_code icode = trunc_optab->handlers[mode][imode].insn_code;
	  emit_insn (GEN_FCN (icode) (ops[0], from));
	}
      else
	emit_insn (gen_extend_insn (ops[0], from, mode, imode, 1));
      return 1;
    }

  /* At least one of the operands needs to be a register. */
  if ((reload_in_progress | reload_completed) == 0
      && !register_operand (ops[0], mode) && !register_operand (ops[1], mode))
    {
      rtx temp = force_reg (mode, ops[1]);
      emit_move_insn (ops[0], temp);
      return 1;
    }
  if (reload_in_progress || reload_completed)
    {
      if (CONSTANT_P (ops[1]))
	return spu_split_immediate (ops);
      return 0;
    }
  else
    {
      /* Catch the SImode immediates greater than 0x7fffffff, and sign
         extend them. */
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

int
spu_split_load (rtx * ops)
{
  enum machine_mode mode = GET_MODE (ops[0]);
  rtx addr, load, rot, mem, p0, p1;
  int rot_amt;

  addr = XEXP (ops[1],0);
  gcc_assert (GET_CODE (addr) != AND);

  if (!address_needs_split (ops[1]))
    {
      addr = XEXP (ops[1], 0);
      if (spu_legitimate_address (mode, addr, 0, 1))
	return 0;
      ops[1] = change_address (ops[1], VOIDmode, force_reg (Pmode, addr));
      emit_move_insn (ops[0], ops[1]);
      return 1;
    }

  rot = 0;
  rot_amt = 0;
  if (GET_CODE (addr) == PLUS)
    {
      /* 8 cases:
         aligned reg   + aligned reg     => lqx
         aligned reg   + unaligned reg   => lqx, rotqby
         aligned reg   + aligned const   => lqd
         aligned reg   + unaligned const => lqd, rotqbyi
         unaligned reg + aligned reg     => lqx, rotqby
         unaligned reg + unaligned reg   => lqx, a, rotqby (1 scratch)
         unaligned reg + aligned const   => lqd, rotqby
         unaligned reg + unaligned const => il, lqx, a, rotqby
       */
      p0 = XEXP (addr, 0);
      p1 = XEXP (addr, 1);
      if (!reg_aligned_for_addr (p0, 1))
	{
	  if (GET_CODE (p1) == REG && !reg_aligned_for_addr (p1, 1))
	    {
	      rot = gen_reg_rtx (SImode);
	      emit_insn (gen_addsi3 (rot, p0, p1));
	    }
	  else if (GET_CODE (p1) == CONST_INT && (INTVAL (p1) & 15))
	    {
	      if (INTVAL (p1) > 0
		  && INTVAL (p1) * BITS_PER_UNIT < REG_ALIGN (p0))
		{
		  rot = gen_reg_rtx (SImode);
		  emit_insn (gen_addsi3 (rot, p0, p1));
		  addr = p0;
		}
	      else
		{
		  addr = rot = gen_reg_rtx (SImode);
		  if (!spu_arith_operand (p1, SImode) || !optimize_size)
		    {
		      p1 = force_reg (SImode, p1);
		      addr = gen_rtx_PLUS (SImode, p0, p1);
		    }
		  emit_insn (gen_addsi3 (rot, p0, p1));
		}
	    }
	  else
	    rot = p0;
	}
      else
	{
	  if (GET_CODE (p1) == CONST_INT && (INTVAL (p1) & 15))
	    {
	      rot_amt = INTVAL (p1) & 15;
	      if (INTVAL (p1) & -16)
		{
		  p1 = GEN_INT (INTVAL (p1) & -16);
		  addr = gen_rtx_PLUS (SImode, p0, p1);
		}
	      else
		addr = p0;
	    }
	  else if (GET_CODE (p1) == REG && !reg_aligned_for_addr (p1, 1))
	    rot = p1;
	}
    }
  else if (GET_CODE (addr) == REG)
    {
      if (!reg_aligned_for_addr (addr, 1))
	rot = addr;
    }
  else if (GET_CODE (addr) == CONST)
    {
      if (GET_CODE (XEXP (addr, 0)) == PLUS
	  && ALIGNED_SYMBOL_REF_P (XEXP (XEXP (addr, 0), 0))
	  && GET_CODE (XEXP (XEXP (addr, 0), 1)) == CONST_INT)
	{
	  rot_amt = INTVAL (XEXP (XEXP (addr, 0), 1));
	  if (rot_amt & -16)
	    addr = gen_rtx_CONST (Pmode,
				  gen_rtx_PLUS (Pmode,
						XEXP (XEXP (addr, 0), 0),
						GEN_INT (rot_amt & -16)));
	  else
	    addr = XEXP (XEXP (addr, 0), 0);
	}
      else
	{
	  rot = gen_reg_rtx (Pmode);
	  emit_move_insn (rot, addr);
	}
    }
  else if (GET_CODE (addr) == CONST_INT)
    {
      rot_amt = INTVAL (addr);
      addr = GEN_INT (rot_amt & -16);
    }
  else if (!ALIGNED_SYMBOL_REF_P (addr))
    {
      rot = gen_reg_rtx (Pmode);
      emit_move_insn (rot, addr);
    }

  if (GET_MODE_SIZE (mode) < 4)
    rot_amt += GET_MODE_SIZE (mode) - 4;

  rot_amt &= 15;

  if (rot && rot_amt)
    {
      rtx x = gen_reg_rtx (SImode);
      emit_insn (gen_addsi3 (x, rot, GEN_INT (rot_amt)));
      rot = x;
      rot_amt = 0;
    }

  load = gen_reg_rtx (TImode);

  mem = change_address (ops[1], TImode, copy_rtx (addr));

  emit_insn (gen_movti (load, mem));

  if (rot)
    emit_insn (gen_rotqby_ti (load, load, rot));
  else if (rot_amt)
    emit_insn (gen_rotlti3 (load, load, GEN_INT (rot_amt * 8)));

  emit_insn (gen_spu_convert (ops[0], load));
  return 1;
}

int
spu_split_store(rtx *ops)
{
  enum machine_mode mode = GET_MODE(ops[0]);
  rtx reg;
  rtx addr, p0, p1, p1_lo, smem;
  int aform;
  int scalar;

  if (!address_needs_split (ops[0]))
    {
      addr = XEXP (ops[0], 0);
      if (spu_legitimate_address (mode, addr, 0, 1))
	return 0;
      ops[0] = change_address (ops[0], VOIDmode, force_reg (Pmode, addr));
      emit_move_insn (ops[0], ops[1]);
      return 1;
    }

  addr = XEXP(ops[0],0);
  gcc_assert (GET_CODE (addr) != AND);

  if (GET_CODE(addr) == PLUS)
    {
      /* 8 cases:
         aligned reg   + aligned reg     => lqx, c?x, shuf, stqx
         aligned reg   + unaligned reg   => lqx, c?x, shuf, stqx
         aligned reg   + aligned const   => lqd, c?d, shuf, stqx
         aligned reg   + unaligned const => lqd, c?d, shuf, stqx
         unaligned reg + aligned reg     => lqx, c?x, shuf, stqx
         unaligned reg + unaligned reg   => lqx, c?x, shuf, stqx
         unaligned reg + aligned const   => lqd, c?d, shuf, stqx
         unaligned reg + unaligned const -> lqx, c?d, shuf, stqx
       */
      aform = 0;
      p0 = XEXP(addr, 0);
      p1 = p1_lo = XEXP(addr, 1);
      if (GET_CODE (p0) == REG && GET_CODE (p1) == CONST_INT)
	{
	  p1_lo = GEN_INT (INTVAL (p1) & 15);
	  if (reg_aligned_for_addr (p0, 1))
	    {
	      p1 = GEN_INT (INTVAL (p1) & -16);
	      if (p1 == const0_rtx)
		addr = p0;
	      else
		addr = gen_rtx_PLUS (SImode, p0, p1);
	    }
	  else
	    {
	      rtx x = gen_reg_rtx (SImode);
	      emit_move_insn (x, p1);
	      addr = gen_rtx_PLUS (SImode, p0, x);
	    }
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
      else if (GET_CODE(addr) == CONST
	       && GET_CODE(XEXP(addr,0)) == PLUS
	       && ALIGNED_SYMBOL_REF_P (XEXP(XEXP(addr,0),0))
	       && GET_CODE(XEXP(XEXP(addr,0),1)) == CONST_INT)
	{
	  HOST_WIDE_INT v = INTVAL(XEXP(XEXP(addr,0),1));
	  if ((v & -16) != 0)
	    addr = gen_rtx_CONST (Pmode,
				  gen_rtx_PLUS (Pmode,
						XEXP (XEXP (addr, 0), 0),
						GEN_INT (v & -16)));
	  else
	    addr = XEXP(XEXP(addr,0),0);
	  p1_lo = GEN_INT(v & 15);
	}
      else if (GET_CODE(addr) == CONST_INT)
	{
	  p1_lo = GEN_INT(INTVAL(addr) & 15);
	  addr =  GEN_INT(INTVAL(addr) & -16);
	}
      else
	{
	  p1_lo = gen_reg_rtx (SImode);
	  emit_move_insn (p1_lo, addr);
	}
    }

  reg = gen_reg_rtx (TImode);

  scalar = store_with_one_insn_p (ops[0]);
  if (!scalar)
    {
      /* We could copy the flags from the ops[0] MEM to mem here,
         We don't because we want this load to be optimized away if
         possible, and copying the flags will prevent that in certain
         cases, e.g. consider the volatile flag. */

      rtx pat = gen_reg_rtx (TImode);
      rtx lmem = change_address (ops[0], TImode, copy_rtx (addr));
      set_mem_alias_set (lmem, 0);
      emit_insn (gen_movti (reg, lmem));

      if (!p0 || reg_aligned_for_addr (p0, 1))
	p0 = stack_pointer_rtx;
      if (!p1_lo)
	p1_lo = const0_rtx;

      emit_insn (gen_cpat (pat, p0, p1_lo, GEN_INT (GET_MODE_SIZE (mode))));
      emit_insn (gen_shufb (reg, ops[1], reg, pat));
    }
  else 
    {
      if (GET_CODE(ops[1]) == REG)
	emit_insn (gen_spu_convert (reg, ops[1]));
      else if (GET_CODE(ops[1]) == SUBREG)
	emit_insn (gen_spu_convert (reg, SUBREG_REG (ops[1])));
      else
	abort();
    }

  if (GET_MODE_SIZE(mode) < 4 && scalar)
    emit_insn (gen_ashlti3
	       (reg, reg, GEN_INT (32 - GET_MODE_BITSIZE (mode))));

  smem = change_address (ops[0], TImode, copy_rtx (addr));
  /* We can't use the previous alias set because the memory has changed
     size and can potentially overlap objects of other types.  */
  set_mem_alias_set (smem, 0);

  emit_insn (gen_movti (smem, reg));
  return 1;
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

/* Return TRUE if x is a CONST_INT, CONST_DOUBLE or CONST_VECTOR that
   can be generated using the fsmbi instruction. */
int
fsmbi_const_p (rtx x)
{
  if (CONSTANT_P (x))
    {
      /* We can always choose DImode for CONST_INT because the high bits
         of an SImode will always be all 1s, i.e., valid for fsmbi. */
      enum immediate_class c = classify_immediate (x, TImode);
      return c == IC_FSMBI || (!flow2_completed && c == IC_FSMBI2);;
    }
  return 0;
}

/* Return TRUE if x is a CONST_INT, CONST_DOUBLE or CONST_VECTOR that
   can be generated using the cbd, chd, cwd or cdd instruction. */
int
cpat_const_p (rtx x, enum machine_mode mode)
{
  if (CONSTANT_P (x))
    {
      enum immediate_class c = classify_immediate (x, mode);
      return c == IC_CPAT;
    }
  return 0;
}

/* Convert a CONST_INT, CONST_DOUBLE, or CONST_VECTOR into a 16 byte
   array.  Use MODE for CONST_INT's.  When the constant's mode is smaller
   than 16 bytes, the value is repeated across the rest of the array. */
void
constant_to_array(enum machine_mode mode, rtx x, unsigned char arr[16])
{
  HOST_WIDE_INT val;
  int i, j, first;

  memset(arr,0,16);
  mode = GET_MODE (x) != VOIDmode ? GET_MODE (x) : mode;
  if (GET_CODE(x) == CONST_INT
      || (GET_CODE(x) == CONST_DOUBLE
	  && (mode == SFmode || mode == DFmode)))
    {
      gcc_assert (mode != VOIDmode && mode != BLKmode);

      if (GET_CODE(x) == CONST_DOUBLE)
	val = const_double_to_hwint(x);
      else
	val = INTVAL(x);
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
      mode = GET_MODE_INNER (mode);
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
    gcc_unreachable();
}

/* Convert a 16 byte array to a constant of mode MODE.  When MODE is
   smaller than 16 bytes, use the bytes that would represent that value
   in a register, e.g., for QImode return the value of arr[3].  */
rtx
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
      j += i;
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
  if (mode == SFmode)
    {
      val = (arr[0] << 24) | (arr[1] << 16) | (arr[2] << 8) | arr[3];
      val = trunc_int_for_mode (val, SImode);
      return hwint_to_const_double (SFmode, val);
    }
  if (mode == DFmode)
    {
      val = (arr[0] << 24) | (arr[1] << 16) | (arr[2] << 8) | arr[3];
      val <<= 32;
      val |= (arr[4] << 24) | (arr[5] << 16) | (arr[6] << 8) | arr[7];
      return hwint_to_const_double (DFmode, val);
    }

  if (!VECTOR_MODE_P(mode))
    abort ();

  units = GET_MODE_NUNITS (mode);
  size = GET_MODE_UNIT_SIZE(mode);
  inner_mode = GET_MODE_INNER(mode);
  v = rtvec_alloc (units);

  for (k = i = 0; i < units; ++i)
    {
      val = 0;
      for (j = 0; j < size; j++, k++)
	val = (val << 8) | arr[k];

      if (GET_MODE_CLASS(inner_mode) == MODE_FLOAT)
	RTVEC_ELT (v, i) = hwint_to_const_double (inner_mode, val);
      else
	RTVEC_ELT (v, i) = GEN_INT (trunc_int_for_mode (val, inner_mode));
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
  if (!last_assemble_variable_decl || in_section == ctors_section)
    loc_decl = decl;
  else 
    loc_decl = last_assemble_variable_decl;

  /* The decl could be a string constant.  */
  if (decl && DECL_P (decl))
   msg = "%Jcreating run-time relocation for %qD";
  else
   msg = "creating run-time relocation";

  if (TARGET_ERROR_RELOC) /** default : error reloc **/
    error (msg, loc_decl, decl);
  else
    warning (0, msg, loc_decl, decl);
}

static bool
spu_assemble_integer (rtx x, unsigned int size, int aligned_p)
{
  /* By default run-time relocations aren't supported, but we allow them
     in case users support it in their own run-time loader.  And we provide
     a warning for those users that don't.  */
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
  if (mode == DFmode || mode == V2DFmode)
    {
      *total = COSTS_N_INSNS (13);
      return true;
    }
  if (mode == SFmode || mode == V4SFmode)
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
    case MINUS:
	if (!FLOAT_MODE_P (mode) && GET_CODE (XEXP (x, 0)) == MULT)
	  cost = COSTS_N_INSNS (12);
	else if (mode == TImode)
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
	if (mode == TImode
	    && (GET_CODE (XEXP (x, 1)) == REG
	      || (GET_CODE (XEXP (x, 1)) == CONST_INT
		  && (INTVAL (XEXP (x, 1)) & 7)
		  && (INTVAL (XEXP (x, 1)) > 7))))
	  *total = COSTS_N_INSNS (8);
	else if (code == ASHIFT && XEXP (x, 1) == CONST1_RTX (mode))
	  *total = COSTS_N_INSNS (2);
	return true;
    case UNSPEC:
	/* An UNSPEC_CONVERT really costs nothing, but we say it has a
	   high cost so it will get folded away when possible. */
	if (XINT(x, 1) == UNSPEC_CONVERT)
	  *total = COSTS_N_INSNS (8);
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

  if (!(VECTOR_MODE_P(mode) || mode == TImode)
      && unspec_code != UNSPEC_CONVERT)
    return 0;

  op0 = l >= 1 ? (c0 ? c0 : XVECEXP(x, 0, 0)) : 0;
  op1 = l >= 2 ? (c1 ? c1 : XVECEXP(x, 0, 1)) : 0;
  op2 = l >= 3 ? (c2 ? c2 : XVECEXP(x, 0, 2)) : 0;

  /* Check for cases involving CONST0_RTX */
  switch (unspec_code)
    {
    case UNSPEC_ADDX:
      if (op0 == CONST0_RTX(GET_MODE(op0))
	  && op1 == CONST0_RTX(GET_MODE(op1))
	  && op2 == CONST0_RTX(GET_MODE(op2)))
	return CONST0_RTX(mode);
      break;
    case UNSPEC_CG:
    case UNSPEC_AVGB:
    case UNSPEC_ABSDB:
    case UNSPEC_SUMB:
      if (op0 == CONST0_RTX(GET_MODE(op0))
	  && op1 == CONST0_RTX(GET_MODE(op1)))
	return CONST0_RTX(mode);
      break;
    case UNSPEC_FSMB:
    case UNSPEC_FSMH:
    case UNSPEC_FSM:
      if (op0 == const0_rtx)
	return CONST0_RTX(mode);
      break;
    }
  switch (unspec_code)
    {
    case UNSPEC_EXTEND_CMP:
      if (GET_CODE (op0) == CONST_INT
	  && (GET_MODE_CLASS (mode) == MODE_INT
	      || GET_MODE_CLASS (mode) == MODE_VECTOR_INT))
	return op0 == const0_rtx ? CONST0_RTX (mode) : CONSTM1_RTX (mode);
      break;
    case UNSPEC_CPAT:
      if (GET_CODE(op1) == CONST_INT
	  && (GET_CODE(op0) == CONST_INT 
	      || (GET_CODE(op0) == REG
		  && REG_ALIGN(op0) >= 128)))
	{
	  int offset, shift, isize, mask;
	  for (i = 0; i < 16; i++)
	    dst[i] = i + 16;
	  isize = INTVAL (op2);
          shift = 0;
          if (isize == 1)
	    shift = 3, mask = 0xf;
          else if (isize == 2)
	    shift = 2, mask = 0xe;
          else if (isize == 4)
	    mask = 0xc;
          else
            mask = 0x8;
	  offset = (INTVAL(op1) + (GET_CODE(op0) == CONST_INT ? INTVAL(op0) : 0)) & mask;
	  for (i = 0; i < isize; i++)
	    dst[offset+i] = i + shift;
	  return array_to_constant(mode, dst);
	}
      break;
    case UNSPEC_ADDX:
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
    case UNSPEC_CG:
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
    case UNSPEC_CGX:
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
    case UNSPEC_SFX:
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
    case UNSPEC_BG:
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
    case UNSPEC_BGX:
      break;
    case UNSPEC_CNTB:
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
    case UNSPEC_FSM:
      opsize *= 2;
    case UNSPEC_FSMH:
      opsize *= 2;
    case UNSPEC_FSMB:
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
    case UNSPEC_GBB:
    case UNSPEC_GBH:
    case UNSPEC_GB:
    case UNSPEC_AVGB:
    case UNSPEC_ABSDB:
    case UNSPEC_SUMB:
      break;
    case UNSPEC_SHUFB:
      if (GET_CODE(op2) == CONST_DOUBLE)
	{
	  enum machine_mode mode0 = GET_MODE(op0);
	  enum machine_mode mode1 = GET_MODE(op1);
	  if (mode0 == VOIDmode)
	      mode0 = GET_MODE (XVECEXP(x, 0, 0));
	  if (mode1 == VOIDmode)
	      mode1 = GET_MODE (XVECEXP(x, 0, 1));
	  if (mode0 == VOIDmode || mode1 == VOIDmode)
	    return 0;
	  constant_to_array(TImode, op2, arr2);
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
	      op2 = array_to_constant(TImode, arr2);
	      new = gen_rtx_UNSPEC (mode,
				    gen_rtvec (3, op0, op1, op2),
				    UNSPEC_SHUFB);
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
	      op2 = array_to_constant(TImode, arr2);
	      new = gen_rtx_UNSPEC (mode,
				    gen_rtvec (3, op0, op1, op2),
				    UNSPEC_SHUFB);
	      return new;
	    }
	}
      if (GET_CODE (op1) == UNSPEC && XINT (op1, 1) == UNSPEC_SHUFB
	  && op2 == XVECEXP(op1, 0, 2))
	{
	  /* (shufb a, (shufb b, c, d), d) -> (shufb a, c, d) */
	  new = gen_rtx_UNSPEC (mode,
				gen_rtvec (3, op0, XVECEXP(op1, 0, 1), op2),
				UNSPEC_SHUFB);
	  return new;
	}
      if (GET_CODE (op0) == UNSPEC && XINT (op0, 1) == UNSPEC_SHUFB
	  && op2 == XVECEXP(op0, 0, 2))
	{
	  /* (shufb (shufb a, b, d), c, d) -> (shufb a, c, d) */
	  new = gen_rtx_UNSPEC (mode,
				gen_rtvec (3, XVECEXP(op0, 0, 0), op1, op2),
				UNSPEC_SHUFB);
	  return new;
	}
      break;

    case UNSPEC_FREST:
      {
	static int frest_table[] = {
	  0x7ffbe0, 0x7f87a6, 0x70ef72, 0x708b40, 0x638b12, 0x633aea, 0x5792c4, 0x574aa0,
	  0x4cca7e, 0x4c9262, 0x430a44, 0x42d62a, 0x3a2e12, 0x39fdfa, 0x3215e4, 0x31f1d2,
	  0x2aa9be, 0x2a85ac, 0x23d59a, 0x23bd8e, 0x1d8576, 0x1d8576, 0x17ad5a, 0x17ad5a,
	  0x124543, 0x124543, 0x0d392d, 0x0d392d, 0x08851a, 0x08851a, 0x041d07, 0x041d07
	};
	if (GET_CODE (op0) == CONST_DOUBLE
	    || GET_CODE (op0) == CONST_VECTOR)
	  {
	    constant_to_array(GET_MODE (op0), op0, arr0);
	    for (i = 0; i < 16; i+= 4)
	      {
		unsigned int w = array_to_int (arr0, i, 4, 0);
		unsigned int r = w & 0x80000000;
		unsigned int e = (w >> 23) & 0xff;
		/* Don't fold when the DBZ flag would be set. */
	        if (e == 0 && !flag_unsafe_math_optimizations)
		  break;
		if (e == 0)
		  r |= 0x7f800000;
		else if (e <= 253)
		  r |= (253 - e) << 23;
		r |= frest_table[(w >> 18) & 0x1f];
		int_to_array (r, arr0, i, 4);
	      }
	    if (i == 16)
	      return array_to_constant(mode, arr0);
	  }
      }
      break;

    case UNSPEC_FRSQEST:
      {
	static int frsqest_table[] = {
	  0x350160, 0x34e954, 0x2f993d, 0x2f993d, 0x2aa523, 0x2aa523, 0x26190d, 0x26190d,
	  0x21e4f9, 0x21e4f9, 0x1e00e9, 0x1e00e9, 0x1a5cd9, 0x1a5cd9, 0x16f8cb, 0x16f8cb,
	  0x13ccc0, 0x13ccc0, 0x10ccb3, 0x10ccb3, 0x0e00aa, 0x0e00aa, 0x0b58a1, 0x0b58a1,
	  0x08d498, 0x08d498, 0x067491, 0x067491, 0x043089, 0x043089, 0x020c83, 0x020c83,
	  0x7ffdf4, 0x7fd1de, 0x7859c8, 0x783dba, 0x71559c, 0x71559c, 0x6ae57c, 0x6ae57c,
	  0x64f561, 0x64f561, 0x5f7149, 0x5f7149, 0x5a4d33, 0x5a4d33, 0x55811f, 0x55811f,
	  0x51050f, 0x51050f, 0x4cc8fe, 0x4cc8fe, 0x48d0f0, 0x48d0f0, 0x4510e4, 0x4510e4,
	  0x4180d7, 0x4180d7, 0x3e24cc, 0x3e24cc, 0x3af4c3, 0x3af4c3, 0x37e8ba, 0x37e8ba,
	};
	if (GET_CODE (op0) == CONST_DOUBLE
	    || GET_CODE (op0) == CONST_VECTOR)
	  {
	    constant_to_array(GET_MODE (op0), op0, arr0);
	    for (i = 0; i < 16; i+= 4)
	      {
		unsigned int w = array_to_int (arr0, i, 4, 0);
		unsigned int r = 0;
		unsigned int e = (w >> 23) & 0xff;
		/* Don't fold when the DBZ flag would be set. */
	        if (e == 0 && !flag_unsafe_math_optimizations)
		  break;
		if (e == 0)
		  r |= 0x7f800000;
		else 
		  r |= ((380 - e) & 0x1fe) << 22;
		r |= frsqest_table[(w >> 18) & 0x3f];
		int_to_array (r, arr0, i, 4);
	      }
	    if (i == 16)
	      return array_to_constant(mode, arr0);
	  }
      }
      break;

    case UNSPEC_FI:
	if ((GET_CODE (op0) == CONST_DOUBLE
	     || GET_CODE (op0) == CONST_VECTOR)
	    && (GET_CODE (op1) == CONST_DOUBLE
		|| GET_CODE (op1) == CONST_VECTOR))
	{
	    constant_to_array(GET_MODE (op0), op0, arr0);
	    constant_to_array(GET_MODE (op1), op1, arr1);
	    for (i = 0; i < 16; i+= 4)
	      {
		unsigned int w0 = array_to_int (arr0, i, 4, 0);
		unsigned int w1 = array_to_int (arr1, i, 4, 0);
		unsigned int sign = w1 & 0x80000000;
		unsigned int exp  = w1 & 0x7f800000;
		unsigned int base = w1 & 0x007ffc00;
		unsigned int step = w1 & 0x000003ff;
		unsigned int y    = w0 & 0x0007ffff;
		unsigned int r, a, b;

		/* Don't fold when the DIFF flag would be set. */
	        if (((exp == 0 && (w1 & 0x007fffff)) || exp == 0x7f800000
		      || ((w0 & 0x7f800000) == 0 && (w0 & 0x007fffff))
		      || (w0 & 0x7f800000) == 0x7f800000)
		    && !flag_unsafe_math_optimizations)
		  break;

		/* For outputs of frest and frsqest, a is never less
		 * than b, for any value of y.  Don't fold when a is
		 * less than b. */
		a = (base << 9);
		b = step * y;
		if (a < b)
		  break;

		/* Don't fold when the DIFF flag would be set. */
	        if (exp == 0 && ((a - b) != 0 || sign != 0)
		    && !flag_unsafe_math_optimizations)
		  break;

		if (exp == 0)
		  r = 0;
		else
		  r = sign | exp | ((a - b) >> 9);
		int_to_array (r, arr0, i, 4);
	      }
	    if (i == 16)
	      return array_to_constant(mode, arr0);
	}
      break;

    case UNSPEC_CSFLT:
    case UNSPEC_CFLTS:
    case UNSPEC_CUFLT:
    case UNSPEC_CFLTU:
      break;
    case UNSPEC_CONVERT:
      {
	enum machine_mode mode_op0 = GET_MODE (XVECEXP (x, 0, 0));
	if (mode_op0 == mode)
	  return op0;
	if ((GET_CODE (op0) == CONST_INT
	     || GET_CODE (op0) == CONST_DOUBLE
	     || GET_CODE (op0) == CONST_VECTOR)
	    &&  mode_op0 != VOIDmode)
	  {
	    constant_to_array(mode_op0, op0, arr0);
	    return array_to_constant(mode, arr0);
	  }
	if (GET_CODE (op0) == SUBREG
	    && GET_MODE_SIZE(GET_MODE(op0)) >= GET_MODE_SIZE(GET_MODE(SUBREG_REG(op0))))
	  op0 = SUBREG_REG(op0);
	/* Change
	    (unspec:M [ (unspec:N [ ... ] UNSPEC_*) ] UNSPEC_CONVERT)
	  to
	    (unspec:M [ ... ] UNSPEC_*) 
	  We specifically don't specify the modes on unspec's in spu.md
	  so this will work.  This helps eliminate extra copies.
	*/
	if (GET_CODE (op0) == UNSPEC && XINT(op0, 1) == UNSPEC_CONVERT)
	  {
	    rtx inner = XVECEXP(op0, 0, 0);
	    if (GET_MODE (inner) == mode)
	      return inner;
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
  rtx pat = gen_reg_rtx (TImode);
  rtx sign, c;
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
	case HImode:
	  sign = gen_reg_rtx (SImode);
		   emit_insn (gen_extendhisi2 (sign, ops[1]));
		   arr[last] = 0x03;
		   arr[last-1] = 0x02;
		   break;
	case SImode:
	  sign = gen_reg_rtx (SImode);
		   emit_insn (gen_ashrsi3 (sign, ops[1], GEN_INT (31)));
		   for (i = 0; i < 4; i++)
		     arr[last-i] = 3-i ;
		   break;
	case DImode:
	  sign = gen_reg_rtx (SImode);
	  c = gen_reg_rtx (SImode);
	  emit_insn (gen_spu_convert (c, ops[1]));
	  emit_insn (gen_ashrsi3 (sign, c, GEN_INT (31)));
		   for (i = 0; i < 8; i++)
		     arr[last-i] = 7-i ;
		   break;
      default:
		   abort();
      }
    }
  emit_move_insn (pat, array_to_constant (TImode, arr));
  emit_insn (gen_shufb (ops[0], ops[1], sign, pat));
}

void
spu_extendsfdf2(rtx ops[])
{
  unsigned char arr[16] = {0, 1, 17, 18, 19, 0x80, 0x80, 0x80, 8, 9, 25, 26, 27, 0x80, 0x80, 0x80};
  rtx pat = gen_reg_rtx(TImode);
  rtx fzero = gen_reg_rtx(V4SFmode);
  rtx iszero = gen_reg_rtx(V4SImode);
  rtx exp0 = gen_reg_rtx(V4SImode);
  rtx exp1 = gen_reg_rtx(V4SImode);
  rtx exp2 = gen_reg_rtx(V4SImode);
  rtx bias0 = gen_reg_rtx(V4SImode);
  rtx bias1 = gen_reg_rtx(V4SImode);
  rtx smask = gen_reg_rtx(V4SImode);
  rtx frac =  gen_reg_rtx(V4SImode);
  rtx from = gen_reg_rtx(V4SImode);
  rtx to = gen_reg_rtx(V4SImode);
    
  emit_move_insn (pat, array_to_constant (TImode, arr));

  /* Do operations in V4SImode */
  emit_insn (gen_spu_convert(from, ops[1]));

  /* Extract exponent and part of fraction */
  emit_move_insn (smask, spu_const (V4SImode, 0x80000000));
  emit_insn (gen_andc_v4si(exp0, from, smask));

  /* Set the bias when the exponent is non zero.
   * Using cmeq is ok because denorms are treated as zero. */
  emit_move_insn (fzero, CONST0_RTX (V4SFmode));
  emit_insn (gen_cmeq_v4sf(iszero, spu_gen_subreg (V4SFmode, from), fzero));
  emit_move_insn (bias0, spu_const (V4SImode, 0x38000000ll));
  emit_insn (gen_andc_v4si(bias1, bias0, iszero));

  /* Compute new exponent keeping the fraction part unchanged */
  emit_insn (gen_lshrv4si3(exp1, exp0, spu_const (V4SImode, 3ll)));
  emit_insn (gen_addv4si3(exp2, exp1, bias1));

  /* Combine sign and exponent */
  emit_insn (gen_selb(to, exp2, from, smask));

  /* Position fraction for shufb, shifting in zeroes. */
  emit_insn (gen_ashlv4si3(frac, from, spu_const (V4SImode, 5)));

  /* Combine fraction and sign/exponent */
  emit_insn (gen_shufb(ops[0], to, frac, pat));
}

void
spu_truncdfsf2(rtx ops[])
{
  unsigned char arr[16] = {0x38, 0, 0, 0, 0, 0, 0, 0, 0x38, 0, 0, 0, 0, 0, 0, 0};
  rtx from = gen_reg_rtx(V4SImode);
  rtx emask = gen_reg_rtx(V4SImode);
  rtx exp0 = gen_reg_rtx(V4SImode);
  rtx exp1 = gen_reg_rtx(V4SImode); 
  rtx notzero = gen_reg_rtx(V4SImode);
  rtx bias = gen_reg_rtx(V4SImode);
  rtx emax = gen_reg_rtx(V4SImode);
  rtx result0 = gen_reg_rtx(V4SImode);
  rtx result1 = gen_reg_rtx(TImode);
  rtx result2 = gen_reg_rtx(V4SImode);
  rtx result3 = gen_reg_rtx(V4SImode);
  rtx result4 = gen_reg_rtx(TImode);
  rtx result5 = gen_reg_rtx(V4SImode);
  rtx result6 = gen_reg_rtx(V4SImode);
  rtx toolarge = gen_reg_rtx(V4SImode);
  rtx fmax = gen_reg_rtx(V4SImode);
  rtx justright = gen_reg_rtx(V4SImode);
  
  emit_insn (gen_spu_convert(from, ops[1]));

  /* Compute the new number assuming it is non-zero and in the range of
   * a float */
  emit_move_insn (bias, array_to_constant (V4SImode, arr)); 
  emit_insn (gen_subv4si3(result0, from, bias));
  emit_insn (gen_ashlti3(result1, spu_gen_subreg (TImode, result0), GEN_INT(3)));

  /* Extract just the exponent for testing special cases */
  emit_move_insn (emask, spu_const (V4SImode, 0x7ff00000));
  emit_insn (gen_andv4si3(exp0, from, emask));

  /* Subtract the bias from exponent */
  emit_insn (gen_subv4si3(exp1, exp0, bias));

  /* If the exponent is too large return a max float */
  emit_move_insn (emax, spu_const (V4SImode, 0x0ff00000));
  emit_insn (gen_cgt_v4si(toolarge, exp1, emax));
  emit_move_insn (fmax, spu_const (V4SImode, 0x7fffffff));
  emit_insn (gen_selb(result2, spu_gen_subreg (V4SImode, result1), fmax, toolarge)); 

  /* If the exponent is too small return zero */
  emit_insn (gen_cgt_v4si(justright, exp1, CONST0_RTX (V4SImode)));
  emit_insn (gen_andv4si3(result3, result2, justright)); 

  /* If the exponenent is zero use a shifted original */
  emit_insn (gen_clgt_v4si(notzero, exp0, CONST0_RTX (V4SImode)));
  emit_insn (gen_ashlti3(result4, spu_gen_subreg (TImode, from), GEN_INT(3)));
  emit_insn (gen_selb(result5, spu_gen_subreg (V4SImode, result4), result3, notzero));

  /* Copy the sign */
  emit_insn (gen_selb(result6, from, result5, fmax));

  emit_insn (gen_spu_convert(ops[0], result6));
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
      if (GET_MODE(SET_SRC(set)) == VOIDmode
	  && !CONSTANT_P(SET_SRC(set)))
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
  if (spu_init_fini_function_p (decl))
    return false;

  if (TARGET_LARGE_MEM)
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

/* We need to correctly update the back chain pointer and the Available
   Stack Size (which is in the second slot of the sp register.) */
void
spu_allocate_stack (rtx op0, rtx op1)
{
  HOST_WIDE_INT v;
  rtx chain = gen_reg_rtx (V4SImode);
  rtx stack_bot = gen_frame_mem (V4SImode, stack_pointer_rtx);
  rtx sp = gen_reg_rtx (V4SImode);
  rtx splatted = gen_reg_rtx (V4SImode);
  rtx pat = gen_reg_rtx (TImode);

  /* copy the back chain so we can save it back again. */
  emit_move_insn (chain, stack_bot);

  op1 = force_reg (SImode, op1);

  v = 0x1020300010203ll;
  emit_move_insn (pat, immed_double_const (v, v, TImode));
  emit_insn (gen_shufb (splatted, op1, op1, pat));

  emit_insn (gen_spu_convert (sp, stack_pointer_rtx));
  emit_insn (gen_subv4si3 (sp, sp, splatted));

  if (flag_stack_check)
    {
      rtx sign = gen_reg_rtx(V2DImode);
      emit_insn (gen_spu_xswd (sign, sp));
      emit_insn (gen_halt_stack (sign));
    }

  emit_insn (gen_spu_convert (stack_pointer_rtx, sp));

  emit_move_insn (stack_bot, chain);

  emit_move_insn (op0, virtual_stack_dynamic_rtx);
}

void
spu_restore_stack_nonlocal (rtx op0 ATTRIBUTE_UNUSED, rtx op1)
{
  static unsigned char arr[16] =
    { 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3 };
  rtx temp = gen_reg_rtx (SImode);
  rtx temp2 = gen_reg_rtx (SImode);
  rtx temp3 = gen_reg_rtx (V4SImode);
  rtx temp4 = gen_reg_rtx (V4SImode);
  rtx pat = gen_reg_rtx (TImode);
  rtx sp = gen_rtx_REG (V4SImode, STACK_POINTER_REGNUM);

  /* Restore the backchain from the first word, sp from the second.  */
  emit_move_insn (temp2, adjust_address_nv (op1, SImode, 0));
  emit_move_insn (temp, adjust_address_nv (op1, SImode, 4));

  emit_move_insn (pat, array_to_constant (TImode, arr));

  /* Compute Available Stack Size for sp*/
  emit_insn (gen_subsi3 (temp, temp, stack_pointer_rtx));
  emit_insn (gen_shufb (temp3, temp, temp, pat));

  /* Compute Available Stack Size for back chain */
  emit_insn (gen_subsi3 (temp2, temp2, stack_pointer_rtx));
  emit_insn (gen_shufb (temp4, temp2, temp2, pat));
  emit_insn (gen_addv4si3 (temp4, sp, temp4));

  emit_insn (gen_addv4si3 (sp, sp, temp3));
  emit_move_insn (gen_frame_mem (V4SImode, stack_pointer_rtx), temp4);
}

void
spu_restore_stack_block (rtx op0 ATTRIBUTE_UNUSED, rtx op1)
{         
  static unsigned char arr[16] =
    { 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3 };
    
  rtx temp = gen_reg_rtx (Pmode);
  rtx temp2 = gen_reg_rtx (V4SImode);
  rtx temp3 = gen_reg_rtx (V4SImode); 
  rtx pat = gen_reg_rtx (TImode);
  rtx sp = gen_rtx_REG (V4SImode, STACK_POINTER_REGNUM);
    
  emit_move_insn (pat, array_to_constant (TImode, arr));

  /* Restore the sp.  */
  emit_move_insn (temp, op1);
  emit_move_insn (temp2, gen_frame_mem (V4SImode, stack_pointer_rtx));

  /* Compute available stack size for sp.  */
  emit_insn (gen_subsi3 (temp, temp, stack_pointer_rtx));
  emit_insn (gen_shufb (temp3, temp, temp, pat));

  emit_insn (gen_addv4si3 (sp, sp, temp3));
  emit_move_insn (gen_frame_mem (V4SImode, stack_pointer_rtx), temp2);
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

  set_conv_libfunc (ufloat_optab, DFmode, SImode, "__float_unssidf");
  set_conv_libfunc (ufloat_optab, DFmode, DImode, "__float_unsdidf");
}

/* Make a subreg, stripping any existing subreg.  We could possibly just
   call simplify_subreg, but in this case we know what we want. */
rtx
spu_gen_subreg (enum machine_mode mode, rtx x)
{
  if (GET_CODE (x) == SUBREG)
    x = SUBREG_REG (x);
  if (GET_MODE (x) == mode)
    return x;
  return gen_rtx_SUBREG (mode, x, 0);
}


/*
 * SPU Language Extension Specification says that conversion between
 * vector and scalar types are not allowed.
 */
static const char*
spu_invalid_conversion (tree fromtype, tree totype)
{
  bool from_vector_p = TREE_CODE (fromtype) == VECTOR_TYPE;
  bool to_vector_p = TREE_CODE (totype) == VECTOR_TYPE;
  tree other_type;

  /* This is more complex than it needs to be. We do not
     want to override gcc's error message since that cause
     dejagnu issues. So we skip certain cases and rely on
     gcc to generate an error. */

  /* if both are vector or both are non-vector. gcc will handle it */
  if (from_vector_p == to_vector_p)
    return NULL;

  /* If we convert to a real mode, gcc will handle it */
  if (from_vector_p && SCALAR_FLOAT_TYPE_P (totype))
    return NULL;

  /* scalar and vector types are of different sizes, gcc will handle it */
  if (!tree_int_cst_equal (TYPE_SIZE (fromtype), TYPE_SIZE (totype)))
    return NULL;

  /* Other mode is non-scalar. gcc will handle it */
  other_type = from_vector_p ? totype : fromtype;
  if (!SCALAR_FLOAT_TYPE_P (other_type) && !INTEGRAL_TYPE_P (other_type))
    return NULL;

  /* The string "can't convert" is special because some dejagnu
     tests grep for it. Do not change the error message below. */
  return "can't convert between vector and scalar types.";
}

/*
 * expand vector initialization. If there are any constant parts,
 * load constant parts first. Then load any non-constant parts.
 */
void
spu_expand_vector_init (rtx target, rtx vals)
{
  enum machine_mode mode = GET_MODE (target);
  int n_elts = GET_MODE_NUNITS (mode);
  int n_var = 0;
  bool all_same = true;
  rtx first, x = NULL_RTX, first_constant = NULL_RTX;
  int i;

  first = XVECEXP (vals, 0, 0); 
  for (i = 0; i < n_elts; ++i)
    {
      x = XVECEXP (vals, 0, i);
      if (!CONSTANT_P (x))
	++n_var;
      else
	{
	  if (first_constant == NULL_RTX)
	    first_constant = x;
	}
      if (i > 0 && !rtx_equal_p (x, first))
	all_same = false;
    }

  /* if all elements are the same, use splats to repeat elements */
  if (all_same)
    {
      if (GET_CODE (first) == VEC_SELECT)
	{
	  rtx r = gen_reg_rtx (GET_MODE (first));
	  emit_move_insn (r, first);
	  first = r;
	}
      if (!CONSTANT_P (first)
	  && !register_operand (first, GET_MODE (x)))
	first = force_reg (GET_MODE (first), first);
      emit_insn (gen_spu_splats (target, first));
      return;
    }

  /* load constant parts */
  if (n_var != n_elts)
    {
      if (n_var == 0)
	{
	  emit_move_insn (target,
			  gen_rtx_CONST_VECTOR (mode, XVEC (vals, 0)));
	}
      else
	{
	  rtx constant_parts_rtx = copy_rtx (vals);

	  gcc_assert (first_constant != NULL_RTX);
	  /* fill empty slots with the first constant, this increases
	     our chance of using splats in the recursive call below. */
	  for (i = 0; i < n_elts; ++i)
	    if (!CONSTANT_P (XVECEXP (constant_parts_rtx, 0, i)))
	      XVECEXP (constant_parts_rtx, 0, i) = first_constant;

	  spu_expand_vector_init (target, constant_parts_rtx);
	}
    }

  /* load variable parts */
  if (n_var != 0)
    {
      rtx insert_operands[4];

      insert_operands[0] = target;
      insert_operands[2] = target;
      for (i = 0; i < n_elts; ++i)
	{
	  x = XVECEXP (vals, 0, i);
	  if (!CONSTANT_P (x))
	    {
	      if (!register_operand (x, GET_MODE (x)))
		x = force_reg (GET_MODE (x), x);
	      insert_operands[1] = x;
	      insert_operands[3] = GEN_INT (i);
	      spu_builtin_insert (insert_operands);
	    }
	}
    }
}

bool
satisfies_constraint_A (rtx op)
{
  return ((GET_CODE (op) == CONST_INT || GET_CODE (op) == CONST_DOUBLE
	   || GET_CODE (op) == CONST_VECTOR)
	   && immediate_load_p (op, SImode))
	  || GET_CODE (op) == SYMBOL_REF
	  || GET_CODE (op) == LABEL_REF
	  || GET_CODE (op) == HIGH
	  || GET_CODE (op) == CONST;
}
bool
satisfies_constraint_B (rtx op)
{
  switch (GET_CODE (op))
    {
    case CONST_INT:
    case CONST_DOUBLE:
    case CONST_VECTOR:
      break;
    default:
      return false;
    }
  return (arith_immediate_p (op, SImode, -0x200, 0x1ff));
}
bool
satisfies_constraint_C (rtx op)
{
  switch (GET_CODE (op))
    {
    case CONST_INT:
    case CONST_DOUBLE:
    case CONST_VECTOR:
      break;
    default:
      return false;
    }
  return (logical_immediate_p (op, SImode));
}
bool
satisfies_constraint_D (rtx op)
{
  switch (GET_CODE (op))
    {
    case CONST_INT:
    case CONST_DOUBLE:
    case CONST_VECTOR:
      break;
    default:
      return false;
    }
  return (iohl_immediate_p (op, SImode));
}
bool
satisfies_constraint_Q (rtx op)
{
  enum machine_mode mode = GET_MODE (op);
  if (mode == VOIDmode)
    return op == const1_rtx;
  switch (GET_CODE (op))
    {
    case CONST_INT:
    case CONST_DOUBLE:
    case CONST_VECTOR:
      break;
    default:
      return false;
    }
  return op == CONST1_RTX (mode);
}
bool
satisfies_constraint_U (rtx op)
{
  switch (GET_CODE (op))
    {
    case CONST_INT:
    case CONST_DOUBLE:
    case CONST_VECTOR:
      break;
    default:
      return false;
    }
  return (immediate_load_p (op, TImode));
}
bool
satisfies_constraint_W (rtx op)
{
  switch (GET_CODE (op))
    {
    case CONST_INT:
    case CONST_DOUBLE:
    case CONST_VECTOR:
      break;
    default:
      return false;
    }
  return (arith_immediate_p (op, SImode, -0x80000000ll, 0x7fffffffll));
}
bool
satisfies_constraint_Y (rtx op)
{
  switch (GET_CODE (op))
    {
    case CONST_INT:
    case CONST_DOUBLE:
    case CONST_VECTOR:
      break;
    default:
      return false;
    }
  return (logical_immediate_p (op, TImode));
}
bool
satisfies_constraint_Z (rtx op)
{
  switch (GET_CODE (op))
    {
    case CONST_INT:
    case CONST_DOUBLE:
    case CONST_VECTOR:
      break;
    default:
      return false;
    }
  return (iohl_immediate_p (op, TImode));
}
bool
satisfies_constraint_a (rtx op)
{
  return (GET_CODE (op) == CONST_INT) && ((immediate_load_p (op, DImode)));
}
bool
satisfies_constraint_c (rtx op)
{
  return (GET_CODE (op) == CONST_INT) && ((logical_immediate_p (op, DImode)));
}
bool
satisfies_constraint_d (rtx op)
{
  return (GET_CODE (op) == CONST_INT) && ((iohl_immediate_p (op, DImode)));
}
bool
satisfies_constraint_f (rtx op)
{
  switch (GET_CODE (op))
    {
    case CONST_INT:
    case CONST_DOUBLE:
    case CONST_VECTOR:
      break;
    default:
      return false;
    }
  return (fsmbi_const_p (op));
}
bool
satisfies_constraint_j (rtx op)
{
  switch (GET_CODE (op))
    {
    case CONST_INT:
    case CONST_DOUBLE:
    case CONST_VECTOR:
      break;
    default:
      return false;
    }
  return (cpat_const_p (op, SImode));
}
bool
satisfies_constraint_k (rtx op)
{
  switch (GET_CODE (op))
    {
    case CONST_INT:
    case CONST_DOUBLE:
    case CONST_VECTOR:
      break;
    default:
      return false;
    }
  return (cpat_const_p (op, DImode));
}
bool
satisfies_constraint_l (rtx op)
{
  switch (GET_CODE (op))
    {
    case CONST_DOUBLE:
    case CONST_VECTOR:
      break;
    default:
      return false;
    }
  return (cpat_const_p (op, TImode));
}
bool
satisfies_constraint_I (rtx op)
{
  HOST_WIDE_INT ival = 0;
  if (GET_CODE (op) == CONST_INT)
    ival = INTVAL (op);
  return (GET_CODE (op) == CONST_INT) && ((ival >= -0x80000000ll && ival <= 0x7fffffffll));
}
bool
satisfies_constraint_J (rtx op)
{
  HOST_WIDE_INT ival = 0;
  if (GET_CODE (op) == CONST_INT)
    ival = INTVAL (op);
  return (GET_CODE (op) == CONST_INT) && ((ival >= 0 && ival <= 0x7f));
}
bool
satisfies_constraint_K (rtx op)
{
  HOST_WIDE_INT ival = 0;
  if (GET_CODE (op) == CONST_INT)
    ival = INTVAL (op);
  return (GET_CODE (op) == CONST_INT) && ((ival >= -0x200 && ival <= 0x1ff));
}
bool
satisfies_constraint_M (rtx op)
{
  HOST_WIDE_INT ival = 0;
  if (GET_CODE (op) == CONST_INT)
    ival = INTVAL (op);
  return (GET_CODE (op) == CONST_INT) && ((ival >= -0x8000ll
					   && ival <= 0x7fffll));
}
bool
satisfies_constraint_N (rtx op)
{
  HOST_WIDE_INT ival = 0;
  if (GET_CODE (op) == CONST_INT)
    ival = INTVAL (op);
  return (GET_CODE (op) == CONST_INT) && ((ival >= 0 && ival <= 0xffff));
}
bool
satisfies_constraint_O (rtx op)
{
  HOST_WIDE_INT ival = 0;
  if (GET_CODE (op) == CONST_INT)
    ival = INTVAL (op);
  return (GET_CODE (op) == CONST_INT) && (ival & 7) == 0;
}
bool
satisfies_constraint_P (rtx op)
{
  HOST_WIDE_INT ival = 0;
  if (GET_CODE (op) == CONST_INT)
    ival = INTVAL (op);
  return (GET_CODE (op) == CONST_INT) && ((ival >= 0 && ival <= 7));
}
bool
satisfies_constraint_R (rtx op)
{
  return (GET_CODE (op) == MEM) && ((GET_CODE (XEXP (op, 0)) == REG));
}
bool
satisfies_constraint_S (rtx op)
{
  return (GET_CODE (op) == MEM) && ((!TARGET_LARGE_MEM
				     &&
				     ((GET_CODE (XEXP (op, 0)) == SYMBOL_REF
				       || GET_CODE (XEXP (op, 0)) ==
				       LABEL_REF))));
}
bool
satisfies_constraint_T (rtx op)
{
  return (GET_CODE (op) == MEM) && ((GET_CODE (XEXP (op, 0)) == CONST_INT
				     && INTVAL (XEXP (op, 0)) >= 0
				     && INTVAL (XEXP (op, 0)) <= 0x3ffff));
}

bool
satisfies_constraint_u (rtx op)
{
  switch (GET_CODE (op))
    {
    case CONST_INT:
    case CONST_DOUBLE:
    case CONST_VECTOR:
      break;
    default:
      return false;
    }
  return (arith_immediate_p (op, SImode, 0, 0x1ff)
          || arith_immediate_p (op, SImode, 0xfe00, 0xffff));
}

bool
satisfies_constraint_v (rtx op)
{
  switch (GET_CODE (op))
    {
    case CONST_DOUBLE:
    case CONST_VECTOR:
      break;
    default:
      return false;
    }
  return exp2_immediate_p (op, VOIDmode, 0, 127);
}

bool
satisfies_constraint_w (rtx op)
{
  switch (GET_CODE (op))
    {
    case CONST_DOUBLE:
    case CONST_VECTOR:
      break;
    default:
      return false;
    }
  return exp2_immediate_p (op, VOIDmode, -126, 0);
}

/*       ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz
 * GCC:      ffffiiiiiiii     x x        x x   xxxx xx
 * SPU:  xxxx    xxx xxxxxxxxx x xxx xx x   xxx        xxx
 * FREE:     ffff   i               a  a  a        a  a   aaa
 * x - used
 * a - available
 * i - available for integer immediates
 * f - available for floating point immediates
 */

bool
constraint_satisfied_p (rtx op, int c)
{
  switch (c)
    {
    case 'A': return satisfies_constraint_A (op);
    case 'B': return satisfies_constraint_B (op);
    case 'C': return satisfies_constraint_C (op);
    case 'D': return satisfies_constraint_D (op);
    case 'I': return satisfies_constraint_I (op);
    case 'J': return satisfies_constraint_J (op);
    case 'K': return satisfies_constraint_K (op);
    case 'M': return satisfies_constraint_M (op);
    case 'N': return satisfies_constraint_N (op);
    case 'O': return satisfies_constraint_O (op);
    case 'P': return satisfies_constraint_P (op);
    case 'Q': return satisfies_constraint_Q (op);
    case 'R': return satisfies_constraint_R (op);
    case 'S': return satisfies_constraint_S (op);
    case 'T': return satisfies_constraint_T (op);
    case 'U': return satisfies_constraint_U (op);
    case 'W': return satisfies_constraint_W (op);
    case 'Y': return satisfies_constraint_Y (op);
    case 'Z': return satisfies_constraint_Z (op);
    case 'a': return satisfies_constraint_a (op);
    case 'c': return satisfies_constraint_c (op);
    case 'd': return satisfies_constraint_d (op);
    case 'f': return satisfies_constraint_f (op);
    case 'j': return satisfies_constraint_j (op);
    case 'k': return satisfies_constraint_k (op);
    case 'l': return satisfies_constraint_l (op);
    case 'u': return satisfies_constraint_u (op);
    case 'v': return satisfies_constraint_v (op);
    case 'w': return satisfies_constraint_w (op);
    default: break;
    }
  return false;
}



#include "value-prof.h"
#include "gcov-io.h"

const char*
spu_emit_pgo_info (rtx *operands, rtx insn)
{
  /* FIXME: Might wish to make this a nop for a couple of reasons:
     - scheduler will, I think, assume the length is 4 and make various
       decisions using that
     - may need to distinguish an edge from the subsequent code */

  /*unsigned word_mode_bitsize = GET_MODE_BITSIZE (word_mode); */
  enum rtx_code kind_code = GET_CODE (operands[0]);
  int kind;
  rtx prev_insn;
  const char* nop = "nop";

  gcc_assert (kind_code == CONST_INT);
  kind = INTVAL (operands[0]);

  gcc_assert (GET_CODE (operands[1]) == CONST_INT);

  /* ??? Maybe we should just use fprintf so we can use asm_addr,
     but being able to use %= is nice. */
  output_asm_insn ("\t.L_pgo_%=:", operands);

  fprintf (asm_out_file, "\t\t.pushsection .pgo_info\n");
  fprintf (asm_out_file, "\t\t.byte %u\n", get_pgo_rec_nr (kind));

  /* ??? Maybe we should just use fprintf so we can use asm_addr,
     but being able to use %= is nice. */
  output_asm_insn ("\t.4byte .L_pgo_%=", operands);

  /* The edge number or value number. */
  output_asm_insn ("\t.4byte %1", operands);

  /* We want to print the register _number_ here, so we have to do
     things differently, we can't use any of GCC's extra %-modifiers.
     The mode is printed as 4 bytes to allow for expansion beyond just
     simple modes.

     We _have_ to emit a real instruction here (or rewrite things adding
     a whole lot more complexity).  Counter records are distinguished by pc
     and we don't want records colliding that otherwise shouldn't.  Without a
     nop (or something) a branch around a pgo_info record will end up at the
     same pc as the pgo_info record, and thus the data will be collected
     regardless of whether the branch is taken or not.
    
     However, for any group of consecutive pgo_info records, we only need
     one nop.  So only emit a nop if the previous non-note insn is not a
     pgo_info rtx.
     ??? GCC thinks this rtl generates an insn of length 4 bytes.  */
  prev_insn = prev_nonnote_insn (insn);
  if (prev_insn != NULL
      && GET_CODE (prev_insn) == INSN
      && GET_CODE (PATTERN (prev_insn)) == PARALLEL
      && (GET_CODE (XVECEXP (PATTERN (prev_insn), 0, 0)) == PGO_INFO_ARC
	  || GET_CODE (XVECEXP (PATTERN (prev_insn), 0, 0)) == PGO_INFO_VALUE))
    nop = "#nop";

  switch (kind)
    {
    case GCOV_COUNTER_ARCS:
      fprintf (asm_out_file, "\t\t.popsection\n");
      return nop;

    case GCOV_COUNTER_V_INTERVAL:
      gcc_assert (GET_CODE (operands[2]) == REG);
      gcc_assert (GET_CODE (operands[3]) == CONST_INT);
      gcc_assert (GET_CODE (operands[4]) == CONST_INT);
      fprintf (asm_out_file, "\t\t.4byte %u\n", get_pgo_mode (GET_MODE (operands[2])));
      fprintf (asm_out_file, "\t\t.4byte %u\n", get_pgo_regno (REGNO (operands[2])));
      /* ??? Stripping off high 32 bits by casting to int is risky.
         What if we discard something important? */
      fprintf (asm_out_file, "\t\t.4byte %d\n", (int) INTVAL (operands[3]));
      fprintf (asm_out_file, "\t\t.4byte %d\n", (int) INTVAL (operands[4]));
      fprintf (asm_out_file, "\t\t.popsection\n");
      return nop;

    case GCOV_COUNTER_V_POW2:
      gcc_assert (GET_CODE (operands[2]) == REG);
      fprintf (asm_out_file, "\t\t.4byte %u\n", get_pgo_mode (GET_MODE (operands[2])));
      fprintf (asm_out_file, "\t\t.4byte %u\n", get_pgo_regno (REGNO (operands[2])));
      fprintf (asm_out_file, "\t\t.popsection\n");
      return nop;

    case GCOV_COUNTER_V_SINGLE:
      gcc_assert (GET_CODE (operands[2]) == REG);
      fprintf (asm_out_file, "\t\t.4byte %u\n", get_pgo_mode (GET_MODE (operands[2])));
      fprintf (asm_out_file, "\t\t.4byte %u\n", get_pgo_regno (REGNO (operands[2])));
      fprintf (asm_out_file, "\t\t.popsection\n");
      return nop;

    case GCOV_COUNTER_V_DELTA:
      gcc_assert (GET_CODE (operands[2]) == REG);
      fprintf (asm_out_file, "\t\t.4byte %u\n", get_pgo_mode (GET_MODE (operands[2])));
      fprintf (asm_out_file, "\t\t.4byte %u\n", get_pgo_regno (REGNO (operands[2])));
      fprintf (asm_out_file, "\t\t.popsection\n");
      return nop;

    default:
      fprintf (asm_out_file, "\t\t.popsection\n");
      return "";
    }
}

void
spu_init_expanders (void)
{   
  if (cfun)
    {
      rtx r0, r1;
      /* HARD_FRAME_REGISTER is only 128 bit aligned when
       * frame_pointer_needed is true.  We don't know that until we're
       * expanding the prologue. */
      REGNO_POINTER_ALIGN (HARD_FRAME_POINTER_REGNUM) = 8;

      /* A number of passes use LAST_VIRTUAL_REGISTER+1 and
       * LAST_VIRTUAL_REGISTER+2 to test the back-end.  We want to
       * handle those cases specially, so we reserve those two registers
       * here by generating them. */
      r0 = gen_reg_rtx(SImode);
      r1 = gen_reg_rtx(SImode);
      mark_reg_pointer (r0, 128);
      mark_reg_pointer (r1, 128);
      gcc_assert (REGNO (r0) == LAST_VIRTUAL_REGISTER+1
		  && REGNO (r1) == LAST_VIRTUAL_REGISTER+2);
    }
}       

/* Don't copy hint related code.  This is mostly for hints that were
   generated by __builtin_expect.  Other hints are generated late enough
   that GCC doesn't ever try to copy them. */
static bool
spu_cannot_copy_insn_p (rtx insn)
{
  return INSN_P (insn)
	 && (INSN_CODE (insn) == CODE_FOR_hbr
	     || find_reg_note (insn, REG_BR_HINT, 0));
}

/* Count the total number of instructions in each pipe and return the
   maximum, which is used as the Minimum Iteration Interval (MII) in the
   modulo scheduler.  get_pipe() will return -2, -1, 0, or 1.  -2 are
   instructions that can go in pipe0 or pipe1. */
int
spu_res_mii (ddg_ptr g)
{
  int i;
  unsigned t[4] = {0, 0, 0, 0};
  for (i = 0; i < g->num_nodes; i++)
    {
      rtx insn = g->nodes[i].insn;
      int p = get_pipe (insn) + 2;
      t[p]++;
      if (dump_file && INSN_P (insn))
	    fprintf (dump_file, "i%d %s %d %d\n",
		     INSN_UID (insn),
		     insn_data[INSN_CODE(insn)].name,
		     p, t[p]);
    }
  if (dump_file)
    fprintf (dump_file, "%d %d %d %d\n", t[0], t[1], t[2], t[3]);
  return MAX ((t[0] + t[2] + t[3] + 1) / 2, MAX (t[2], t[3]));
}

/** SCE bugilla #11003 **/
/* APPLE LOCAL pragma reverse_bitfields, ms_struct */
int darwin_reverse_bitfields = false;

/* Pragma reverse_bitfields.  For compatibility with CW.
   This feature is not well defined by CW, and results in
   code that does not work in some cases!  Bug compatibility
   is the requirement, however.  */

static bool
spu_reverse_bitfields_p (tree record_type ATTRIBUTE_UNUSED)
{
  return darwin_reverse_bitfields;
}

/* True if we're setting "#pragma ms_struct on".  */
int darwin_ms_struct = false;

static bool
spu_ms_bitfield_layout_p (tree record_type ATTRIBUTE_UNUSED)
{
  return darwin_ms_struct;
}

void
spu_notice_static_storage_vars (tree vars)
{
  tree v;

  if (!flag_pic || !(TARGET_WARN_RELOC || TARGET_ERROR_RELOC))
    return;

  for (v = vars; v; v = TREE_CHAIN (v))
    {
      tree var = TREE_VALUE (v);
      warning (0, "%Jstatic initializer/destructor for %qD leads to a run-time relocation", var, var);
    }
}

/* Generate a constant or register which contains 2^SCALE.  We assume
   the result is valid for MODE.  Currently, MODE must be V4SFmode and
   SCALE must be SImode. */
rtx
spu_gen_exp2 (enum machine_mode mode, rtx scale)
{
  gcc_assert (mode == V4SFmode);
  gcc_assert (GET_MODE (scale) == SImode || GET_CODE (scale) == CONST_INT);
  if (GET_CODE (scale) != CONST_INT)
    {
      /* unsigned int exp = (127 + scale) << 23;
	__vector float m = (__vector float) spu_splats (exp); */
      rtx reg = force_reg (SImode, scale);
      rtx exp = gen_reg_rtx (SImode);
      rtx mul = gen_reg_rtx (mode);
      emit_insn (gen_addsi3 (exp, reg, GEN_INT (127)));
      emit_insn (gen_ashlsi3 (exp, exp, GEN_INT (23)));
      emit_insn (gen_spu_splats (mul, gen_rtx_SUBREG (GET_MODE_INNER (mode), exp, 0)));
      return mul;
    }
  else 
    {
      HOST_WIDE_INT exp = 127 + INTVAL (scale);
      unsigned char arr[16];
      arr[0] = arr[4] = arr[8] = arr[12] = exp >> 1;
      arr[1] = arr[5] = arr[9] = arr[13] = exp << 7;
      arr[2] = arr[6] = arr[10] = arr[14] = 0;
      arr[3] = arr[7] = arr[11] = arr[15] = 0;
      return array_to_constant (mode, arr);
    }
}

static bool
spu_cannot_modify_jumps_p (void)
{
  /* Disable basic block reordering for naked functions */
  return reload_completed && spu_naked_function_p (current_function_decl);
}
