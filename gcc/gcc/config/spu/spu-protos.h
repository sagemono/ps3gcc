
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
extern void spu_clobber_dont_save_regs 		(rtx sym_ref);
extern const char *spu_strip_name_encoding 	(const char *name);
extern int call_operand 			(rtx op, enum machine_mode mode);
extern int sibcall_operand 			(rtx op, enum machine_mode mode);
extern int move_operand 			(rtx op, enum machine_mode mode);
extern int rK_operand 				(rtx op, enum machine_mode mode);
extern int rKN_operand 				(rtx op, enum machine_mode mode);
extern int immediate_operand_K 			(rtx op, enum machine_mode mode);
extern void spu_override_options 		(void);
extern int vec_imm_operand 			(rtx op, enum machine_mode mode, int range);
extern int vec_regimm_operand 			(rtx op, enum machine_mode mode);
extern int vector_operand 			(rtx op, enum machine_mode mode);
extern int const_zero_operand 			(rtx op, enum machine_mode mode);
extern int const_one_operand 			(rtx op, enum machine_mode mode);
extern int branch_comparison_operator		(rtx op, enum machine_mode mode);
extern void spu_extract 			(rtx *ops, int unsignedp);
extern void spu_insert 				(rtx *ops);
extern int spu_expand_block_move		(rtx *ops);
extern void spu_emit_branch_or_set 		(int is_set, enum rtx_code code, rtx *operands);
extern void print_operand_address 		(FILE *file, register rtx addr);
extern void print_operand 			(FILE *file, rtx x, int code);
extern int store_operand 			(register rtx op, enum machine_mode mode);
extern void spu_attach_dont_save_regs_attribute (void);
extern int spu_saved_regs_size 			(void);
extern int direct_return 			(void);
extern void spu_expand_prologue 		(void);
extern void spu_expand_epilogue 		(bool);
extern rtx spu_return_addr 			(int count, rtx frame);
extern void spu_setup_incoming_varargs 		(int *cum, enum machine_mode mode, tree type, int *pretend_size, int no_rtl);
extern const char *spu_mov128 			(rtx *ops);
extern const char *spu_emit_move_asm 		(rtx *ops, enum machine_mode mode);
extern rtx spu_const_vector 			(enum machine_mode mode, rtx inner);
extern rtx spu_const_from_ints 			(enum machine_mode mode, int a, int b, int c, int d);
extern int spu_split_move 			(rtx *ops, enum machine_mode mode);
extern void spu_split_trunc_shift_asm 		(rtx *operands, int unsigned_p, int ashift);
extern struct rtx_def *spu_float_const 		(const char *string, enum machine_mode mode);
extern int spu_constant_address_p 		(rtx x);
extern int spu_legitimate_constant_p 		(rtx x);
extern int spu_legitimate_ti_const_p 		(rtx x);
extern int spu_legitimate_address		(enum machine_mode, rtx, int);
extern rtx spu_legitimize_address		(rtx, rtx, enum machine_mode);
extern int spu_entry_function_p			(tree func);
extern int spu_initial_elimination_offset 	(int from, int to);
extern rtx spu_function_value			(tree type, tree func);
extern rtx spu_function_arg			(int cum, enum machine_mode mode, tree type, int named);
extern int spu_function_arg_pass_by_reference 	(int *cum, enum machine_mode mode, tree type, int named);
extern tree spu_build_va_list			(void);
extern void spu_va_start			(tree valist, rtx nextarg);
extern rtx spu_va_arg 				(tree valist, tree type);
extern void spu_conditional_register_usage 	(void);
extern int spu_expand_mov			(rtx *ops, enum machine_mode mode);
extern void spu_split_load			(rtx *ops);
extern void spu_split_store			(rtx *ops);
extern int spu_valid_move			(rtx *ops);
extern rtx spu_force_reg 			(enum machine_mode mode, rtx op);
extern void spu_builtin_splats			(rtx *ops);
extern void spu_builtin_extract			(rtx *ops);
extern void spu_builtin_insert			(rtx *ops);
extern void spu_builtin_promote			(rtx *ops);
extern int spu_split_extract_shufb		(rtx *ops);
extern int const_vector_uses_shuf		(rtx op);
extern void spu_reloadin			(rtx *ops);
extern int reloadin_operand			(rtx op, enum machine_mode mode);
extern void spu_expand_sign_extend		(rtx *ops);
extern void spu_extendsfdf2			(rtx *ops);
extern HOST_WIDE_INT spu_move_by_pieces_ninsns	(unsigned HOST_WIDE_INT l, unsigned int align);
extern rtx spu_emit_insn			(rtx);
extern void spu_expandsfdf2			(rtx *ops);
extern void spu_truncdfsf2 			(rtx *ops);
extern void spu_allocate_stack 			(rtx op0, rtx op1);
extern void spu_restore_stack_nonlocal 		(rtx op0, rtx op1);
extern int spu_safe_dma				(HOST_WIDE_INT channel);
extern void spu_cpu_cpp_builtins		(struct cpp_reader *);
extern struct cpp_hashnode *spu_macro_to_expand	(struct cpp_reader *,
						 const struct cpp_token *);
extern int spu_mode_offset			(enum machine_mode, enum machine_mode);
extern void spu_init_expanders			(void);
#endif
