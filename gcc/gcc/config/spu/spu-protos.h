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

#ifndef _SPU_PROTOS_
#define _SPU_PROTOS_

#include "rtl.h"


/* Prototypes generated using gcc's -aux-info flag. */
extern void spu_init_builtins			(void);
extern tree spu_resolve_overloaded_builtin	(tree, tree);
extern rtx spu_expand_builtin			(tree, rtx, rtx, enum machine_mode, int);
extern void spu_optimization_options		(int level, int size);
extern void spu_override_options		(void); 
extern void spu_c_common_override_options       (void);
extern int valid_subreg				(rtx op); 
extern void spu_extract				(rtx *ops, int unsignedp); 
extern void spu_insert				(rtx *ops); 
extern int spu_expand_block_move		(rtx *ops); 
extern void spu_emit_branch_or_set		(int is_set, enum rtx_code code, rtx *operands); 
extern rtx hwint_to_const_double		(enum machine_mode mode, HOST_WIDE_INT v); 
extern void print_operand_address		(FILE *file, register rtx addr); 
extern void print_operand			(FILE *file, rtx x, int code); 
extern int spu_split_immediate			(rtx * ops);
extern int spu_saved_regs_size			(void); 
extern int direct_return			(void); 
extern void spu_order_regs_for_local_alloc	(void);
extern void spu_expand_prologue			(void); 
extern void spu_expand_epilogue			(unsigned char sibcall_p); 
extern rtx spu_return_addr			(int count, rtx frame); 
extern rtx spu_const 				(enum machine_mode mode, HOST_WIDE_INT val);
extern void spu_builtin_splats			(rtx *ops); 
extern void spu_builtin_extract			(rtx *ops); 
extern void spu_builtin_insert			(rtx *ops); 
extern void spu_builtin_promote			(rtx *ops); 
extern rtx spu_const_vector			(enum machine_mode mode, rtx inner); 
extern rtx spu_const_from_ints			(enum machine_mode mode, int a, int b, int c, int d); 
extern void spu_split_trunc_shift_asm		(rtx *operands, int unsigned_p, int ashift); 
extern rtx spu_float_const			(const char *string, enum machine_mode mode); 
extern int spu_constant_address_p		(rtx x); 
extern int immediate_load_p			(rtx op, enum machine_mode mode); 
extern int logical_immediate_p			(rtx op, enum machine_mode mode); 
extern int iohl_immediate_p			(rtx op, enum machine_mode mode); 
extern int arith_immediate_p			(rtx op, enum machine_mode mode, HOST_WIDE_INT low, HOST_WIDE_INT high); 
extern int spu_legitimate_constant_p		(rtx x); 
extern int spu_legitimate_address		(enum machine_mode mode, rtx x, int reg_ok_strict, int for_split); 
extern rtx spu_legitimize_address		(rtx x, rtx oldx, enum machine_mode mode); 
extern int spu_initial_elimination_offset	(int from, int to); 
extern rtx spu_function_value			(tree type, tree func); 
extern rtx spu_function_arg			(CUMULATIVE_ARGS *cum, enum machine_mode mode, tree type, int named); 
extern void spu_va_start			(tree valist, rtx nextarg); 
extern void spu_setup_incoming_varargs		(CUMULATIVE_ARGS *cum, enum machine_mode mode, tree type, int *pretend_size, int no_rtl); 
extern void spu_conditional_register_usage	(void); 
extern int spu_expand_mov			(rtx *ops, enum machine_mode mode); 
extern int spu_split_load			(rtx *ops); 
extern int spu_split_store			(rtx *ops); 
extern int fsmbi_const_p			(rtx x); 
extern int cpat_const_p 			(rtx x, enum machine_mode mode);
extern void spu_expand_sign_extend		(rtx *ops); 
extern void spu_extendsfdf2			(rtx *ops);
extern void spu_truncdfsf2			(rtx *ops);
extern HOST_WIDE_INT spu_move_by_pieces_ninsns	(unsigned HOST_WIDE_INT l, unsigned int align); 
extern rtx spu_emit_insn			(rtx insn); 
extern enum machine_mode spu_eh_return_filter_mode	(void); 
extern void spu_cpu_cpp_builtins		(struct cpp_reader *);
extern struct cpp_hashnode *spu_macro_to_expand	(struct cpp_reader *,
						 const struct cpp_token *);
extern int spu_mode_offset			(enum machine_mode old_mode, enum machine_mode new_mode); 
extern void spu_allocate_stack			(rtx op0, rtx op1); 
extern void spu_restore_stack_nonlocal		(rtx op0, rtx op1); 
extern void spu_restore_stack_block 		(rtx op0, rtx op1);
extern int spu_safe_dma				(HOST_WIDE_INT channel); 
extern rtx spu_gen_subreg			(enum machine_mode mode, rtx x); 
extern void spu_expand_vector_init		(rtx target, rtx vals); 
extern void constant_to_array			(enum machine_mode mode, rtx x, unsigned char arr[16]);
extern rtx array_to_constant			(enum machine_mode mode, unsigned char arr[16]);

extern bool satisfies_constraint_A (rtx op);
extern bool satisfies_constraint_B (rtx op);
extern bool satisfies_constraint_C (rtx op);
extern bool satisfies_constraint_D (rtx op);
extern bool satisfies_constraint_I (rtx op);
extern bool satisfies_constraint_J (rtx op);
extern bool satisfies_constraint_K (rtx op);
extern bool satisfies_constraint_M (rtx op);
extern bool satisfies_constraint_N (rtx op);
extern bool satisfies_constraint_O (rtx op);
extern bool satisfies_constraint_P (rtx op);
extern bool satisfies_constraint_Q (rtx op);
extern bool satisfies_constraint_R (rtx op);
extern bool satisfies_constraint_S (rtx op);
extern bool satisfies_constraint_T (rtx op);
extern bool satisfies_constraint_U (rtx op);
extern bool satisfies_constraint_W (rtx op);
extern bool satisfies_constraint_Y (rtx op);
extern bool satisfies_constraint_Z (rtx op);
extern bool satisfies_constraint_a (rtx op);
extern bool satisfies_constraint_c (rtx op);
extern bool satisfies_constraint_d (rtx op);
extern bool satisfies_constraint_f (rtx op);
extern bool satisfies_constraint_j (rtx op);
extern bool satisfies_constraint_k (rtx op);
extern bool satisfies_constraint_l (rtx op);
extern bool satisfies_constraint_t (rtx op);
extern bool constraint_satisfied_p (rtx op, int c);

extern const char* spu_emit_pgo_info (rtx *ops, rtx insn);

void spu_init_expanders (void);

struct ddg;
int spu_res_mii (struct ddg *);

/** SCE bugzilla #11003 **/
extern void darwin_pragma_reverse_bitfields (struct cpp_reader *);
extern void darwin_pragma_ms_struct (struct cpp_reader *);
#endif

extern void spu_notice_static_storage_vars (tree vars);
