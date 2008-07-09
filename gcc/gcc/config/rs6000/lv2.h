/* Definitions of target machine for GNU compiler,
   for 64 bit PowerPC linux. */

/* (C) Copyright
   Sony Computer Entertainment, Inc.,
   Toshiba Corporation,
   International Business Machines Corporation,
   2001,2002,2003,2004,2005,2006,2007.

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

#undef	DEFAULT_ABI
#define	DEFAULT_ABI ABI_AIX

#undef	TARGET_64BIT
#define	TARGET_64BIT 1

#define	DEFAULT_ARCH64_P 1
#define	RS6000_BI_ARCH_P 0

#undef	TARGET_AIX
#define	TARGET_AIX TARGET_64BIT

#undef PROCESSOR_DEFAULT64
#define PROCESSOR_DEFAULT64 PROCESSOR_CELLPPU

#undef	TARGET_RELOCATABLE
#define	TARGET_RELOCATABLE (!TARGET_64BIT && (target_flags & MASK_RELOCATABLE))

#undef	RS6000_ABI_NAME
#define	RS6000_ABI_NAME (TARGET_64BIT ? "aixdesc" : "sysv")

#define INVALID_64BIT "-m%s not supported in this configuration"
#define INVALID_32BIT INVALID_64BIT

#undef	SUBSUBTARGET_OVERRIDE_OPTIONS
#define	SUBSUBTARGET_OVERRIDE_OPTIONS				\
  do								\
    {								\
      if (!rs6000_explicit_options.alignment)			\
	rs6000_alignment_flags = MASK_ALIGN_NATURAL;		\
      if (TARGET_64BIT)						\
	{							\
	  if (DEFAULT_ABI != ABI_AIX)				\
	    {							\
	      rs6000_current_abi = ABI_AIX;			\
	      error (INVALID_64BIT, "call");			\
	    }							\
	  if (target_flags & MASK_RELOCATABLE)			\
	    {							\
	      target_flags &= ~MASK_RELOCATABLE;		\
	      error (INVALID_64BIT, "relocatable");		\
	    }							\
	  if (target_flags & MASK_EABI)				\
	    {							\
	      target_flags &= ~MASK_EABI;			\
	      error (INVALID_64BIT, "eabi");			\
	    }							\
	  if (target_flags & MASK_PROTOTYPE)			\
	    {							\
	      target_flags &= ~MASK_PROTOTYPE;			\
	      error (INVALID_64BIT, "prototype");		\
	    }							\
          if ((target_flags & MASK_POWERPC64) == 0)		\
	    {							\
	      target_flags |= MASK_POWERPC64;			\
	      error ("-m64 requires a PowerPC64 cpu");		\
	    }							\
	}							\
      else							\
	{							\
	  if (!RS6000_BI_ARCH_P)				\
	    error (INVALID_32BIT, "32");			\
	  if (TARGET_PROFILE_KERNEL)				\
	    {							\
	      target_flags &= ~MASK_PROFILE_KERNEL;		\
	      error (INVALID_32BIT, "profile-kernel");		\
	    }							\
	}							\
    }								\
  while (0)


/* 64-bit PowerPC Linux is always big-endian.  */
#undef	TARGET_LITTLE_ENDIAN
#define TARGET_LITTLE_ENDIAN	0

/* 64-bit PowerPC Linux always has a TOC.  */
#undef  TARGET_TOC
#define	TARGET_TOC		1

/* Some things from sysv4.h we don't do when 64 bit.  */
#undef	TARGET_RELOCATABLE
#define	TARGET_RELOCATABLE	0
#undef	TARGET_EABI
#define	TARGET_EABI		0
#undef	TARGET_PROTOTYPE
#define	TARGET_PROTOTYPE	0

/* LV2 has all C99 functions */
#undef TARGET_C99_FUNCTIONS
#define TARGET_C99_FUNCTIONS 1

#undef TARGET_POSIX_IO

/* We use glibc _mcount for profiling.  */
#define NO_PROFILE_COUNTERS TARGET_64BIT
#define PROFILE_HOOK(LABEL) \
  do { if (TARGET_64BIT) output_profile_hook (LABEL); } while (0)

/* We don't need to generate entries in .fixup.  */
#undef RELOCATABLE_NEEDS_FIXUP

/* PowerPC64 Linux word-aligns FP doubles when -malign-power is given.  */
#undef  ADJUST_FIELD_ALIGN
#define ADJUST_FIELD_ALIGN(FIELD, COMPUTED) \
  ((TARGET_ALTIVEC && TREE_CODE (TREE_TYPE (FIELD)) == VECTOR_TYPE)	\
   ? 128								\
   : (TARGET_64BIT							\
      && TARGET_ALIGN_NATURAL == 0					\
      && TYPE_MODE (TREE_CODE (TREE_TYPE (FIELD)) == ARRAY_TYPE		\
		    ? get_inner_array_type (FIELD)			\
		    : TREE_TYPE (FIELD)) == DFmode)			\
   ? MIN ((COMPUTED), 32)						\
   : (COMPUTED))

/* PowerPC64 Linux increases natural record alignment to doubleword if
   the first field is an FP double, only if in power alignment mode.  */
#undef  ROUND_TYPE_ALIGN
#define ROUND_TYPE_ALIGN(STRUCT, COMPUTED, SPECIFIED)			\
  ((TARGET_ALTIVEC && TREE_CODE (STRUCT) == VECTOR_TYPE			\
    && ALTIVEC_VECTOR_MODE (TYPE_MODE (STRUCT)))			\
   ? MAX (MAX ((COMPUTED), (SPECIFIED)), 128)				\
   : (TARGET_64BIT							\
      && (TREE_CODE (STRUCT) == RECORD_TYPE				\
	  || TREE_CODE (STRUCT) == UNION_TYPE				\
	  || TREE_CODE (STRUCT) == QUAL_UNION_TYPE)			\
      && TARGET_ALIGN_NATURAL == 0)					\
   ? rs6000_special_round_type_align (STRUCT, COMPUTED, SPECIFIED)	\
   : MAX ((COMPUTED), (SPECIFIED)))

/* Indicate that jump tables go in the text section.  */
#undef  JUMP_TABLES_IN_TEXT_SECTION
#define JUMP_TABLES_IN_TEXT_SECTION TARGET_64BIT

/* The linux ppc64 ABI isn't explicit on whether aggregates smaller
   than a doubleword should be padded upward or downward.  You could
   reasonably assume that they follow the normal rules for structure
   layout treating the parameter area as any other block of memory,
   then map the reg param area to registers.  ie. pad updard.
   Setting both of the following defines results in this behavior.
   Setting just the first one will result in aggregates that fit in a
   doubleword being padded downward, and others being padded upward.
   Not a bad idea as this results in struct { int x; } being passed
   the same way as an int.  */
#define AGGREGATE_PADDING_FIXED TARGET_64BIT
#define AGGREGATES_PAD_UPWARD_ALWAYS 0

/* Specify padding for the last element of a block move between
   registers and memory.  FIRST is nonzero if this is the only
   element.  */
#define BLOCK_REG_PADDING(MODE, TYPE, FIRST) \
  (!(FIRST) ? upward : FUNCTION_ARG_PADDING (MODE, TYPE))

/* __throw will restore its own return address to be the same as the
   return address of the function that the throw is being made to.
   This is unfortunate, because we want to check the original
   return address to see if we need to restore the TOC.
   So we have to squirrel it away with this.  */
#define SETUP_FRAME_ADDRESSES() \
  do { if (TARGET_64BIT) rs6000_aix_emit_builtin_unwind_init (); } while (0)

/* Override svr4.h  */
#undef MD_EXEC_PREFIX
#undef MD_STARTFILE_PREFIX

#undef  TARGET_OS_CPP_BUILTINS
#define TARGET_OS_CPP_BUILTINS()            		\
  do							\
    {							\
      if (TARGET_64BIT)					\
	{						\
	  builtin_define ("__PPC__");			\
	  builtin_define ("__PPC64__");			\
	  builtin_define ("__powerpc__");		\
	  builtin_define ("__powerpc64__");		\
	  builtin_define ("__PIC__");			\
	  builtin_assert ("cpu=powerpc64");		\
	  builtin_assert ("machine=powerpc64");		\
	  if (TARGET_PPC64_LP32)          		\
	    builtin_define ("__POINTER_32BIT__");	\
	}						\
      else						\
	{						\
	  builtin_define_std ("PPC");			\
	  builtin_define_std ("powerpc");		\
	  builtin_assert ("cpu=powerpc");		\
	  builtin_assert ("machine=powerpc");		\
	  TARGET_OS_SYSV_CPP_BUILTINS ();		\
	}						\
    }							\
  while (0)

#undef  TOC_SECTION_ASM_OP
#define TOC_SECTION_ASM_OP \
  (TARGET_64BIT						\
   ? "\t.section\t\".toc\",\"aw\""			\
   : "\t.section\t\".got\",\"aw\"")

#undef  MINIMAL_TOC_SECTION_ASM_OP
#define MINIMAL_TOC_SECTION_ASM_OP \
  (TARGET_64BIT						\
   ? "\t.section\t\".toc1\",\"aw\""			\
   : ((TARGET_RELOCATABLE || flag_pic)			\
      ? "\t.section\t\".got2\",\"aw\""			\
      : "\t.section\t\".got1\",\"aw\""))

/* collect2.c also refers to TARGET_VERSION. Don't access 
   variables defines in rs6000.c in this macro. */
#undef  TARGET_VERSION
#define TARGET_VERSION fprintf (stderr, " (PowerPC64, CellOS Lv-2 ILP32 model)")

/*
     An expression whose value is 1 or 0, according to whether the type
     `char' should be signed or unsigned by default.  The user can
     always override this default with the options `-fsigned-char' and
     `-funsigned-char'.

     In case of -mlp64 option, the compiler driver should specify 
     -funsigned-char because -mlp64 options orders the compiler
     to generate code under PPC64 Linux ABI.
*/
#undef  DEFAULT_SIGNED_CHAR
#define DEFAULT_SIGNED_CHAR   1

/* A C expression for the size in bits of the type `long' on the
   target machine.  If you don't define this, the default is one
   word.  */
#undef  LONG_TYPE_SIZE
#define LONG_TYPE_SIZE (TARGET_32BIT || TARGET_PPC64_LP32? 32 : 64)

/*
     A C expression for a string describing the name of the data type
     to use for size values.  The typedef name `size_t' is defined
     using the contents of the string.

     The string can contain more than one keyword.  If so, separate
     them with spaces, and write first any length keyword, then
     `unsigned' if appropriate, and finally `int'.  The string must
     exactly match one of the data type names defined in the function
     `init_decl_processing' in the file `c-decl.c'.  You may not omit
     `int' or change the order--that would cause the compiler to crash
     on startup.

     If you don't define this macro, the default is `"long unsigned
     int"'. 
*/
/* Must be at least as big as our pointer type.  */
#undef	SIZE_TYPE
#define	SIZE_TYPE (TARGET_32BIT	|| TARGET_PPC64_LP32)		\
                     ? "unsigned int"				\
                     : "long unsigned int"

/*
  Width of a pointer, in bits.  You must specify a value no wider
  than the width of `Pmode'.  If it is not equal to the width of
  `Pmode', you must define `POINTERS_EXTEND_UNSIGNED'.  If you do
  not specify a value the default is `BITS_PER_WORD'.
*/
#undef  POINTER_SIZE
#define POINTER_SIZE ((TARGET_64BIT && ! TARGET_PPC64_LP32)? 64: 32)

/* Specify the machine mode that pointers have.
   After generation of rtl, the compiler makes no further distinction
   between pointers and any other objects of this machine mode.  */
#undef  Pmode
#define Pmode (TARGET_32BIT ? SImode : DImode)

/*
  If defined, an expression of type `enum machine_mode' that
  specifies the mode of the size increment operand of an
  `allocate_stack' named pattern (*note Standard Names::).

  You need not define this macro if it always returns `word_mode'.
  You would most commonly define this macro if the `allocate_stack'
  pattern needs to support both a 32- and a 64-bit mode.
*/
#undef  STACK_SIZE_MODE
#define STACK_SIZE_MODE    Pmode

/*
  A C expression for a string describing the name of the data type
  to use for the result of subtracting two pointers.  The typedef
  name `ptrdiff_t' is defined using the contents of the string.  See
  `SIZE_TYPE' above for more information.
  
  If you don't define this macro, the default is `"long int"'.
*/
#undef	PTRDIFF_TYPE
#define	PTRDIFF_TYPE ((TARGET_64BIT && ! TARGET_PPC64_LP32)? "long int" : "int")

/*
`POINTERS_EXTEND_UNSIGNED'
     A C expression whose value is greater than zero if pointers that
     need to be extended from being `POINTER_SIZE' bits wide to `Pmode'
     are to be zero-extended and zero if they are to be sign-extended.
     If the value is less then zero then there must be an "ptr_extend"
     instruction that extends a pointer from `POINTER_SIZE' to `Pmode'.

     You need not define this macro if the `POINTER_SIZE' is equal to
     the width of `Pmode'.
*/
#undef  POINTERS_EXTEND_UNSIGNED
#define POINTERS_EXTEND_UNSIGNED -1


#undef	WCHAR_TYPE
#define	WCHAR_TYPE "short unsigned int"

#undef  WCHAR_TYPE_SIZE
#define WCHAR_TYPE_SIZE 16

#undef  WINT_TYPE
#define WINT_TYPE "int"

/* Override rs6000.h definition.  */
#undef  ASM_APP_ON
#define ASM_APP_ON "#APP\n"

/* Override rs6000.h definition.  */
#undef  ASM_APP_OFF
#define ASM_APP_OFF "#NO_APP\n"

/* PowerPC no-op instruction.  */
#undef  RS6000_CALL_GLUE
#define RS6000_CALL_GLUE (TARGET_64BIT ? "nop" : "cror 31,31,31")

#undef  RS6000_MCOUNT
#define RS6000_MCOUNT "_mcount"

#ifdef __powerpc64__
/* _init and _fini functions are built from bits spread across many
   object files, each potentially with a different TOC pointer.  For
   that reason, place a nop after the call so that the linker can
   restore the TOC pointer if a TOC adjusting call stub is needed.  */
#define CRT_CALL_STATIC_FUNCTION(SECTION_OP, FUNC)	\
  asm (SECTION_OP "\n"					\
"	bl ." #FUNC "\n"				\
"	nop\n"						\
"	.previous");
#endif

#undef	FP_SAVE_INLINE
#define FP_SAVE_INLINE(FIRST_REG) 1

/* FP save and restore routines.  */
#undef  SAVE_FP_PREFIX
#define SAVE_FP_PREFIX "_savefpr_"
#undef  SAVE_FP_SUFFIX
#define SAVE_FP_SUFFIX ""
#undef  RESTORE_FP_PREFIX
#define RESTORE_FP_PREFIX "_restfpr_"
#undef  RESTORE_FP_SUFFIX
#define RESTORE_FP_SUFFIX "" 


/* Macros for using external functions to save general registers.
   Currently never use inline stores. */
#undef	GP_SAVE_INLINE
#define GP_SAVE_INLINE(FIRST_REG)  1

#undef  SAVE_GP_PREFIX
#define SAVE_GP_PREFIX "_savegpr1_"
#undef  SAVE_GP_SUFFIX
#define SAVE_GP_SUFFIX ""
#undef  RESTORE_GP_PREFIX
#define RESTORE_GP_PREFIX "_restgpr1_"
#undef  RESTORE_GP_SUFFIX
#define RESTORE_GP_SUFFIX "" 
#undef  SAVE_GP_LR_PREFIX
#define SAVE_GP_LR_PREFIX "_savegpr0_"
#undef  SAVE_GP_LR_SUFFIX
#define SAVE_GP_LR_SUFFIX ""
#undef  RESTORE_GP_LR_PREFIX
#define RESTORE_GP_LR_PREFIX "_restgpr0_"
#undef  RESTORE_GP_LR_SUFFIX
#define RESTORE_GP_LR_SUFFIX "" 

/* Dwarf2 debugging.  */
#undef  PREFERRED_DEBUGGING_TYPE
#define PREFERRED_DEBUGGING_TYPE DWARF2_DEBUG

/* This is how to declare the size of a function.  */
#undef	ASM_DECLARE_FUNCTION_SIZE
#define	ASM_DECLARE_FUNCTION_SIZE(FILE, FNAME, DECL)			\
  do									\
    {									\
      if (!flag_inhibit_size_directive)					\
	{								\
	  fputs ("\t.size\t", (FILE));					\
	  if (TARGET_64BIT)						\
	    putc ('.', (FILE));						\
	  assemble_name ((FILE), (FNAME));				\
	  fputs (",.-", (FILE));					\
	  if (TARGET_64BIT)						\
	    putc ('.', (FILE));						\
	  assemble_name ((FILE), (FNAME));				\
	  putc ('\n', (FILE));						\
	}								\
    }									\
  while (0)

/* Return nonzero if this entry is to be written into the constant
   pool in a special way.  We do so if this is a SYMBOL_REF, LABEL_REF
   or a CONST containing one of them.  If -mfp-in-toc (the default),
   we also do this for floating-point constants.  We actually can only
   do this if the FP formats of the target and host machines are the
   same, but we can't check that since not every file that uses
   GO_IF_LEGITIMATE_ADDRESS_P includes real.h.  We also do this when
   we can write the entry into the TOC and the entry is not larger
   than a TOC entry.  */

#undef  ASM_OUTPUT_SPECIAL_POOL_ENTRY_P
#define ASM_OUTPUT_SPECIAL_POOL_ENTRY_P(X, MODE)			\
  (TARGET_TOC								\
   && rs6000_base_toc  < 2						\
   && (GET_CODE (X) == SYMBOL_REF					\
       || (GET_CODE (X) == CONST && GET_CODE (XEXP (X, 0)) == PLUS	\
	   && GET_CODE (XEXP (XEXP (X, 0), 0)) == SYMBOL_REF)		\
       || GET_CODE (X) == LABEL_REF					\
       || (GET_CODE (X) == CONST_INT 					\
	   && GET_MODE_BITSIZE (MODE) <= GET_MODE_BITSIZE (Pmode))	\
       || (GET_CODE (X) == CONST_DOUBLE					\
	   && ((TARGET_64BIT						\
		&& (TARGET_POWERPC64					\
		    || TARGET_MINIMAL_TOC				\
		    || (GET_MODE_CLASS (GET_MODE (X)) == MODE_FLOAT	\
			&& ! TARGET_NO_FP_IN_TOC)))			\
	       || (!TARGET_64BIT					\
		   && !TARGET_NO_FP_IN_TOC				\
		   && !TARGET_RELOCATABLE				\
		   && GET_MODE_CLASS (GET_MODE (X)) == MODE_FLOAT	\
		   && BITS_PER_WORD == HOST_BITS_PER_INT)))))

/* This ABI cannot use DBX_LINES_FUNCTION_RELATIVE, nor can it use
   dbxout_stab_value_internal_label_diff, because we must
   use the function code label, not the function descriptor label.  */
#define	DBX_OUTPUT_SOURCE_LINE(FILE, LINE, COUNTER)			\
do									\
  {									\
    char temp[256];							\
    const char *s;							\
    ASM_GENERATE_INTERNAL_LABEL (temp, "LM", COUNTER);			\
    dbxout_begin_stabn_sline (LINE);					\
    assemble_name (FILE, temp);						\
    putc ('-', FILE);							\
    s = XSTR (XEXP (DECL_RTL (current_function_decl), 0), 0);		\
    rs6000_output_function_entry (FILE, s);				\
    putc ('\n', FILE);							\
    targetm.asm_out.internal_label (FILE, "LM", COUNTER);		\
    COUNTER += 1;							\
  }									\
while (0)

/* Similarly, we want the function code label here.  Cannot use
   dbxout_stab_value_label_diff, as we have to use
   rs6000_output_function_entry.  FIXME.  */
#define DBX_OUTPUT_BRAC(FILE, NAME, BRAC)				\
  do									\
    {									\
      const char *s;							\
      dbxout_begin_stabn (BRAC);					\
      assemble_name (FILE, NAME);					\
      putc ('-', FILE);							\
      s = XSTR (XEXP (DECL_RTL (current_function_decl), 0), 0);		\
      rs6000_output_function_entry (FILE, s);				\
      putc ('\n', FILE);						\
    }									\
  while (0)

#define DBX_OUTPUT_LBRAC(FILE, NAME) DBX_OUTPUT_BRAC (FILE, NAME, N_LBRAC)
#define DBX_OUTPUT_RBRAC(FILE, NAME) DBX_OUTPUT_BRAC (FILE, NAME, N_RBRAC)

/* Another case where we want the dot name.  */
#define	DBX_OUTPUT_NFUN(FILE, LSCOPE, DECL)				\
  do									\
    {									\
      const char *s;							\
      dbxout_begin_empty_stabs (N_FUN);					\
      assemble_name (FILE, LSCOPE);					\
      putc ('-', FILE);							\
      s = XSTR (XEXP (DECL_RTL (current_function_decl), 0), 0);		\
      rs6000_output_function_entry (FILE, s);				\
      putc ('\n', FILE);						\
    }									\
  while (0)

/* Select a format to encode pointers in exception handling data.  CODE
   is 0 for data, 1 for code labels, 2 for function pointers.  GLOBAL is
   true if the symbol may be affected by dynamic relocations.  */
#undef	ASM_PREFERRED_EH_DATA_FORMAT
#define	ASM_PREFERRED_EH_DATA_FORMAT(CODE, GLOBAL) \
  ((TARGET_64BIT || flag_pic || TARGET_RELOCATABLE)			\
   ? (((GLOBAL) ? DW_EH_PE_indirect : 0) | DW_EH_PE_pcrel		\
      | (TARGET_64BIT ? DW_EH_PE_udata8 : DW_EH_PE_sdata4))		\
   : DW_EH_PE_absptr)

/* For backward compatibility, we must continue to use the AIX
   structure return convention.  */
#undef DRAFT_V4_STRUCT_RET
#define DRAFT_V4_STRUCT_RET (!TARGET_64BIT)

#undef TARGET_HAS_F_SETLKW

/* If the current unwind info (FS) does not contain explicit info
   saving R2, then we have to do a minor amount of code reading to
   figure out if it was saved.  The big problem here is that the
   code that does the save/restore is generated by the linker, so
   we have no good way to determine at compile time what to do.  */

#define MD_FROB_UPDATE_CONTEXT(CTX, FS)					\
  do {									\
    if ((FS)->regs.reg[2].how == REG_UNSAVED)				\
      {									\
	unsigned int *insn						\
	  = (unsigned int *)						\
	    _Unwind_GetGR ((CTX), LINK_REGISTER_REGNUM);		\
	if (*insn == 0xE8410028)					\
	  _Unwind_SetGRPtr ((CTX), 2, (CTX)->cfa + 40);			\
      }									\
  } while (0)

/* Do code reading to identify a signal frame, and set the frame
   state data appropriately.  See unwind-dw2.c for the structs.  */

#if 0
#ifdef IN_LIBGCC2
#include <signal.h>
#include <sys/ucontext.h>

enum { SIGNAL_FRAMESIZE = 128 };

#endif

#define MD_FALLBACK_FRAME_STATE_FOR(CONTEXT, FS, SUCCESS)		\
  do {									\
    unsigned char *pc_ = (CONTEXT)->ra;					\
    struct sigcontext *sc_;						\
    long new_cfa_;							\
    int i_;								\
									\
    /* addi r1, r1, 128; li r0, 0x0077; sc  (sigreturn) */		\
    /* addi r1, r1, 128; li r0, 0x00AC; sc  (rt_sigreturn) */		\
    if (*(unsigned int *) (pc_+0) != 0x38210000 + SIGNAL_FRAMESIZE	\
	|| *(unsigned int *) (pc_+8) != 0x44000002)			\
      break;								\
    if (*(unsigned int *) (pc_+4) == 0x38000077)			\
      {									\
	struct sigframe {						\
	  char gap[SIGNAL_FRAMESIZE];					\
	  struct sigcontext sigctx;					\
	} *rt_ = (CONTEXT)->cfa;					\
	sc_ = &rt_->sigctx;						\
      }									\
    else if (*(unsigned int *) (pc_+4) == 0x380000AC)			\
      {									\
	struct rt_sigframe {						\
	  int tramp[6];							\
	  struct siginfo *pinfo;					\
	  struct ucontext *puc;						\
	} *rt_ = (struct rt_sigframe *) pc_;				\
	sc_ = &rt_->puc->uc_mcontext;					\
      }									\
    else								\
      break;								\
    									\
    new_cfa_ = sc_->regs->gpr[STACK_POINTER_REGNUM];			\
    (FS)->cfa_how = CFA_REG_OFFSET;					\
    (FS)->cfa_reg = STACK_POINTER_REGNUM;				\
    (FS)->cfa_offset = new_cfa_ - (long) (CONTEXT)->cfa;		\
    									\
    for (i_ = 0; i_ < 32; i_++)						\
      if (i_ != STACK_POINTER_REGNUM)					\
	{	    							\
	  (FS)->regs.reg[i_].how = REG_SAVED_OFFSET;			\
	  (FS)->regs.reg[i_].loc.offset 				\
	    = (long)&(sc_->regs->gpr[i_]) - new_cfa_;			\
	}								\
									\
    (FS)->regs.reg[LINK_REGISTER_REGNUM].how = REG_SAVED_OFFSET;	\
    (FS)->regs.reg[LINK_REGISTER_REGNUM].loc.offset 			\
      = (long)&(sc_->regs->link) - new_cfa_;				\
									\
    (FS)->regs.reg[ARG_POINTER_REGNUM].how = REG_SAVED_OFFSET;		\
    (FS)->regs.reg[ARG_POINTER_REGNUM].loc.offset 			\
      = (long)&(sc_->regs->nip) - new_cfa_;				\
    (FS)->retaddr_column = ARG_POINTER_REGNUM;				\
    goto SUCCESS;							\
  } while (0)

#endif


#define OS_MISSING_POWERPC64 !TARGET_64BIT

/* Set defaults for CELL PPU
   linux64.h is the last rs6000 specific header included by tm.h */
#undef ASM_DEFAULT_SPEC
#undef TARGET_DEFAULT
#undef PROCESSOR_DEFAULT64
#undef PROCESSOR_DEFAULT

#define	ASM_DEFAULT_SPEC ""
#define TARGET_DEFAULT (MASK_POWERPC | MASK_POWERPC64 | MASK_64BIT \
			| MASK_NEW_MNEMONICS | MASK_ALTIVEC | MASK_PPC64_LP32 \
			| MASK_PPC_GPOPT | MASK_PPC_GFXOPT | MASK_MFCRF )

#define PROCESSOR_DEFAULT64 PROCESSOR_CELLPPU
#define PROCESSOR_DEFAULT PROCESSOR_CELLPPU

#undef TARGET_ASM_FILE_END

#undef	SUBTARGET_EXTRA_SPECS


/*  Macro: OPTION_DEFAULT_SPECS

    A list of specs used to support configure-time default options (i.e.
    --with options) in the driver. It should be a suitable initializer
    for an array of structures, each containing two strings, without the
    outermost pair of surrounding braces. 

    The first item in the pair is the name of the default. This must
    match the code in config.gcc for the target. The second item is a
    spec to apply if a default with this name was specified. The string
    %(VALUE) in the spec will be replaced by the value of the default
    everywhere it occurs. 

    The driver will apply these specs to its own command line between
    loading default specs files and processing DRIVER_SELF_SPECS, using
    the same mechanism as DRIVER_SELF_SPECS.  */

/* defined in rs6000.h */

/*  Macro: CPP_SPEC

    A C string constant that tells the GCC driver program options to
    pass to CPP. It can also specify how to translate options you give
    to GCC into options for GCC to pass to the CPP.  */

#undef CPP_SPEC
#define	CPP_SPEC "-D__PPU__ -D__CELLOS_LV2__ " \
        "%{fno-rtti:-D__NO_RTTI} " \
        "%{fno-exceptions:-D_NO_EX} " \
        "%{mlp64:-funsigned-char} "

/*  Macro: CPLUSPLUS_CPP_SPEC

    This macro is just like CPP_SPEC, but is used for C++, rather than
    C. If you do not define this macro, then the value of CPP_SPEC (if
    any) will be used instead.  */


/*  Macro: CC1_SPEC

    A C string constant that tells the GCC driver program options to
    pass to cc1, cc1plus, f771, and the other language front ends. It
    can also specify how to translate options you give to GCC into
    options for GCC to pass to front ends.  */

/* In sysv4.h, this is used to set endian.  We are always big endian */
/* The GCC testsuite is very sensitive to changes in some default options.
   We use the -testing flag to disable some of our defaults.  */
/* Strict alignment is enabled because LV2 does not have the unaligned
   handlers.  Default to unsigned char for LV1 (LP64).  */
#undef CC1_SPEC
#define CC1_SPEC \
	"%{!mno-altivec:%{!mabi=*:-mabi=altivec}} " \
	"%{!mno-altivec:-maltivec} " \
	"%{!mno-strict-align:-mstrict-align} " \
	"%{!fno-strict-aligned:-fstrict-aligned} " \
        "%{mlp64:-funsigned-char} " \
        "%{!mvrsave:-mno-vrsave} " \
        "%{!mtraceback=*:-mtraceback=none} "

/*  Macro: CC1PLUS_SPEC

    A C string constant that tells the GCC driver program options to
    pass to cc1plus. It can also specify how to translate options you
    give to GCC into options for GCC to pass to the cc1plus. 

    Note that everything defined in CC1_SPEC is already passed to
    cc1plus so there is no need to duplicate the contents of CC1_SPEC in
    CC1PLUS_SPEC.  */

#undef CC1PLUS_SPEC
#define CC1PLUS_SPEC \
	"%{!fthreadsafe-statics:-fno-threadsafe-statics}"

/*  Macro: CC1_ONLY_SPEC */
#undef CC1_ONLY_SPEC
#define CC1_ONLY_SPEC "%{!testing:%{!std=*:-std=gnu99}}"

/*  Macro: ASM_SPEC

    A C string constant that tells the GCC driver program options to
    pass to the assembler. It can also specify how to translate options
    you give to GCC into options for GCC to pass to the assembler. See
    the file sun3.h for an example of this.  */

#undef	ASM_SPEC
#define ASM_SPEC "%{!mlp64:-mcelloslv2} " \
"-a64 " \
"-mbig -mcellppu " \
"%{.s: %{mregnames} %{mno-regnames}} " \
"%{.S: %{mregnames} %{mno-regnames}} " \
"%{v:-V} %{Qy:} %{!Qn:-Qy} %{Wa,*:%*} "

/*  Macro: ASM_FINAL_SPEC

    A C string constant that tells the GCC driver program how to run any
    programs which cleanup after the normal assembler. Normally, this is
    not needed. See the file mips.h for an example of this.  */

/*  Macro: LINK_COMMAND_SPEC
    A C string constant giving the complete command line need to
    execute the linker.  When you do this, you will need to update
    your port each time a change is made to the link command line
    within `gcc.c'.  Therefore, define this macro only if you need to
    completely redefine the command line for invoking the linker and
    there is no other way to accomplish the effect you need.
    Overriding this macro may be avoidable by  overriding
    `LINK_GCC_C_SEQUENCE_SPEC' instead. */
/* Bugzilla 11401
   Most part of LINK_COMMAND_SPEC is a copied from the original definition
   in gcc.c. The only thing modified here is that prx_fixup spec is inserted
   at the last of this spec string to invoke ppu-lv2-prx-fixup after linking */
/* Bugzilla 13303 
   If -mprx option is specified, invoke addtional hairy command sequences.
   Actual specs are defiend in SUBTARGET_EXTRA_SPECS. See below */
#undef LINK_COMMAND_SPEC
#define LINK_COMMAND_SPEC "%{mprx|mprx-with-runtime: %(prx_link_command); : %(lv2_old_link_command)} "

/*  Macro: LINK_SPEC

    A C string constant that tells the GCC driver program options to
    pass to the linker. It can also specify how to translate options you
    give to GCC into options for GCC to pass to the linker.  */
/* sce local bugzilla 37796
   add condition to handle mno-sn-ld option. */
/* sce local bugzilla 50260
   PPU GUID option for SN LD */
#undef	LINK_SPEC
#define	LINK_SPEC \
  "%{h*} %{v:-V} %{!msdata=none:%{G*}} %{msdata=none:-G0} " \
  "%{YP,*} %{R*} "					 \
  "%{Qy:} %{!Qn:-Qy} "					 \
  "%{mlp64:-melf64ppc} "						\
  "%{mno-sn-ld|mprx|mprx-with-runtime: ; : " \
  "  %{!mlp64:--alternative-ld=ps3ppuld --gnu-mode %{!mno-prxfixup: %{!mforce-prx-fixup:--prx-fixup}} %{mprx|mprx-with-runtime:--no-check-unresolved} " \
  "    %{mppuguid: %{!r:-ppuguid}} " \
  "    %{r|mno-ppuguid:-no-ppuguid} } } " \
  "%{shared}"

/*  Macro: LIB_SPEC

    Another C string constant used much like LINK_SPEC. The difference
    between the two is that LIB_SPEC is used at the end of the command
    given to the linker. 

    If this macro is not defined, a default is provided that loads the
    standard C library from the usual place. See gcc.c.  */

#undef LIB_SPEC
 /* TRANSMETA begin
    - for incremental linking: "-r" == "-Wl,-r -mno-prxfixup -mlv2-stub" */
#define LIB_SPEC \
  "%{!mno-sn-ld: -L%R/lib} " \
  "--start-group -lc -lgcc -lstdc++ -lsupc++ %:if-file-exist(%:prepend-cmddir(../../../target/ppu/lib/libsnc.a) -lsnc)" \
  " %{mlv2-stub|!mno-prxfixup:-llv2_stub; :-llv2} -lsyscall --end-group" \
  " %{r|mno-prxfixup|T: ; : -T %R/lib/elf64_lv2_prx.x} "
/* TRANSMETA end */

/*  Macro: LIBGCC_SPEC

    Another C string constant that tells the GCC driver program how and
    when to place a reference to libgcc.a into the linker command line.
    This constant is placed both before and after the value of LIB_SPEC. 

    If this macro is not defined, the GCC driver provides a default that
    passes the string -lgcc to the linker.  */

/*  Macro: LINK_EH_SPEC

    If defined, this C string constant is added to LINK_SPEC. When
    USE_LD_AS_NEEDED is zero or undefined, it also affects the
    modifications to LIBGCC_SPEC mentioned in REAL_LIBGCC_SPEC.  */

/* Use define from sysv4.h */

/*  Macro: STARTFILE_SPEC

    Another C string constant used much like LINK_SPEC. The difference
    between the two is that STARTFILE_SPEC is used at the very beginning
    of the command given to the linker. 

    If this macro is not defined, a default is provided that loads the
    standard C startup file from the usual place. See gcc.c.  */
/* sce local bugzilla 50260
   PPU GUID option for GNU LD */

#undef STARTFILE_SPEC
#define STARTFILE_SPEC \
	"ecrti.o%s " \
	"%{!shared:%{!mmambo:crt0.o%s crt1.o%s;: " \
	"mmambo:--whole-archive mambo-crt1.o%s libmambo.a%s --no-whole-archive}} "\
	"%{shared: crtbeginS.o%s; :crtbegin.o%s} " \
	"%{mppuguid: %{!r: %{mno-sn-ld: %R/lib/crtid.o;:}}} "

/*  Macro: ENDFILE_SPEC

    Another C string constant used much like LINK_SPEC. The difference
    between the two is that ENDFILE_SPEC is used at the very end of the
    command given to the linker.  */

#undef ENDFILE_SPEC
#define ENDFILE_SPEC "%{shared: crtendS.o%s; :crtend.o%s} ecrtn.o%s"

/*  Macro: SYSROOT_SUFFIX_SPEC

    Define this macro to add a suffix to the target sysroot when GCC is
    configured with a sysroot. This will cause GCC to search for
    usr/lib, et al, within sysroot+suffix.  */

#undef	SYSROOT_SUFFIX_SPEC

/*  Macro: SYSROOT_HEADERS_SUFFIX_SPEC

    Define this macro to add a headers_suffix to the target sysroot when
    GCC is configured with a sysroot. This will cause GCC to pass the
    updated sysroot+headers_suffix to CPP, causing it to search for
    usr/include, et al, within sysroot+headers_suffix. */

#undef	SYSROOT_HEADERS_SUFFIX_SPEC

/*  Macro: EXTRA_SPECS

    Define this macro to provide additional specifications to put in the
    specs file that can be used in various specifications like CC1_SPEC. 

    The definition should be an initializer for an array of structures,
    containing a string constant, that defines the specification name,
    and a string constant that provides the specification. 

    EXTRA_SPECS is useful when an architecture contains several related
    targets, which have various ..._SPECS which are similar to each
    other, and the maintainer would like one central place to keep these
    definitions.  */

/* defined in rs6000.h, where it will include SUBTARGET_EXTRA_SPECS */

/* Bugzilla 11401  PRX requires the compiler driver to execute fixup command
   after linking. */
/* Bugzilla 13302  implement -mprx option.
   FIXME: Please someone clean up following spec defs. */
#undef	SUBTARGET_EXTRA_SPECS
#define        SUBTARGET_EXTRA_SPECS \
  { "prx_fixup",       PRX_FIXUP_COMMAND_SPEC },       \
  { "prx_link_command", PRX_LINK_COMMAND_SPEC },       \
  { "lv2_old_link_command", LV2_OLD_LINK_COMMAND_SPEC },

#define PRX_FIXUP_COMMAND_SPEC         "ppu-lv2-prx-fixup"

#define PRX_LINK_COMMAND_SPEC ""

#define LV2_OLD_LINK_COMMAND_SPEC \
"%{!fsyntax-only:%{!c:%{!M:%{!MM:%{!E:%{!S: " \
"%(linker) %l " LINK_PIE_SPEC "%X %{o*} %{A} %{d} %{e*} %{m} %{N} %{n} %{r} " \
"%{s} %{t} %{u*} %{x} %{z} %{Z} %{!A:%{!nostdlib:%{!nostartfiles:%S}}} " \
"%{static:} %{L*} %(mfwrap) %(link_libgcc) %o %(mflib) " \
"%{fprofile-arcs|fprofile-generate|coverage:-lgcov} " \
"%{!nostdlib:%{!nodefaultlibs:%(link_gcc_c_sequence)}} " \
"%{!A:%{!nostdlib:%{!nostartfiles:%E}}} %{T*} " \
"  %{r|mno-prxfixup: ; :" \
"     \n " \
"     %{mforce-prx-fixup|mno-sn-ld:%(prx_fixup) --stub-fix-only %{!o: a.out} %{o*: %*}} " \
"  }" \
"}}}}}} "



/*  Macro: LINK_LIBGCC_SPECIAL

    Define this macro if the driver program should find the library
    libgcc.a itself and should not pass -L options to the linker. If you
    do not define this macro, the driver program will pass the argument
    -lgcc to tell the linker to do the search and will pass -L options
    to it.  */

/*  Macro: LINK_LIBGCC_SPECIAL_1

    Define this macro if the driver program should find the library
    libgcc.a. If you do not define this macro, the driver program will
    pass the argument -lgcc to tell the linker to do the search. This
    macro is similar to LINK_LIBGCC_SPECIAL, except that it does not
    affect -L options.  */

/*  Macro: LINK_GCC_C_SEQUENCE_SPEC

    The sequence in which libgcc and libc are specified to the linker.
    By default this is %G %L %G.  */

#undef LINK_GCC_C_SEQUENCE_SPEC 
#define LINK_GCC_C_SEQUENCE_SPEC "%L"

/*  Macro: MULTILIB_DEFAULTS

    Define this macro as a C expression for the initializer of an array
    of string to tell the driver program which options are defaults for
    this target and thus do not need to be handled specially when using
    MULTILIB_OPTIONS. 

    Do not define this macro if MULTILIB_OPTIONS is not defined in the
    target makefile fragment or if none of the options listed in
    MULTILIB_OPTIONS are set by default. See Target Fragment.  */

#undef	MULTILIB_DEFAULTS

/*  Macro: RELATIVE_PREFIX_NOT_LINKDIR

    Define this macro to tell gcc that it should only translate a -B
    prefix into a -L linker option if the prefix indicates an absolute
    file name.  */

/*  Macro: MD_EXEC_PREFIX

    If defined, this macro is an additional prefix to try after
    STANDARD_EXEC_PREFIX. MD_EXEC_PREFIX is not searched when the -b
    option is used, or the compiler is built as a cross compiler. If you
    define MD_EXEC_PREFIX, then be sure to add it to the list of
    directories used to find the assembler in configure.in.  */

/*  Macro: STANDARD_STARTFILE_PREFIX

    Define this macro as a C string constant if you wish to override the
    standard choice of libdir as the default prefix to try when
    searching for startup files such as crt0.o.
    STANDARD_STARTFILE_PREFIX is not searched when the compiler is built
    as a cross compiler.  */

/*  Macro: STANDARD_STARTFILE_PREFIX_1

    Define this macro as a C string constant if you wish to override the
    standard choice of /lib as a prefix to try after the default prefix
    when searching for startup files such as crt0.o.
    STANDARD_STARTFILE_PREFIX_1 is not searched when the compiler is
    built as a cross compiler.  */

/*  Macro: STANDARD_STARTFILE_PREFIX_2

    Define this macro as a C string constant if you wish to override the
    standard choice of /lib as yet another prefix to try after the
    default prefix when searching for startup files such as crt0.o.
    STANDARD_STARTFILE_PREFIX_2 is not searched when the compiler is
    built as a cross compiler.  */

/*  Macro: MD_STARTFILE_PREFIX

    If defined, this macro supplies an additional prefix to try after
    the standard prefixes. MD_EXEC_PREFIX is not searched when the -b
    option is used, or when the compiler is built as a cross compiler.  */


/*  undefine this so the correct sysrooted dirs are searched */
#undef STARTFILE_PREFIX_SPEC
#define STARTFILE_PREFIX_SPEC "/lib/ /lib/sys/ /lib/drivers/"


/*  Macro: MD_STARTFILE_PREFIX_1

    If defined, this macro supplies yet another prefix to try after the
    standard prefixes. It is not searched when the -b option is used, or
    when the compiler is built as a cross compiler.  */

/*  Macro: LOCAL_INCLUDE_DIR

    Define this macro as a C string constant if you wish to override the
    standard choice of /usr/local/include as the default prefix to try
    when searching for local header files. LOCAL_INCLUDE_DIR comes
    before SYSTEM_INCLUDE_DIR in the search order. 

    Cross compilers do not search either /usr/local/include or its
    replacement.  */

/*  Macro: SYSTEM_INCLUDE_DIR

    Define this macro as a C string constant if you wish to specify a
    system-specific directory to search for header files before the
    standard directory. SYSTEM_INCLUDE_DIR comes before
    STANDARD_INCLUDE_DIR in the search order. 

    Cross compilers do not use this macro and do not search the
    directory specified.  */

/*  Macro: STANDARD_INCLUDE_DIR

    Define this macro as a C string constant if you wish to override the
    standard choice of /usr/include as the default prefix to try when
    searching for header files. 

    Cross compilers ignore this macro and do not search either
    /usr/include or its replacement.  */

#define STANDARD_INCLUDE_DIR "/include"

/*  Macro: STANDARD_INCLUDE_COMPONENT

    The component corresponding to STANDARD_INCLUDE_DIR. See
    INCLUDE_DEFAULTS, below, for the description of components. If you
    do not define this macro, no component is used.  */

/*  Macro: INCLUDE_DEFAULTS

    Define this macro if you wish to override the entire default search
    path for include files. For a native compiler, the default search
    path usually consists of GCC_INCLUDE_DIR, LOCAL_INCLUDE_DIR,
    SYSTEM_INCLUDE_DIR, GPLUSPLUS_INCLUDE_DIR, and STANDARD_INCLUDE_DIR.
    In addition, GPLUSPLUS_INCLUDE_DIR and GCC_INCLUDE_DIR are defined
    automatically by Makefile, and specify private search areas for GCC.
    The directory GPLUSPLUS_INCLUDE_DIR is used only for C++ programs. 

    The definition should be an initializer for an array of structures.
    Each array element should have five elements: the directory name (a
    string constant), the component name (also a string constant), a
    flag for C++-only directories, a flag showing that the includes
    in the directory don't need to be wrapped in extern C when compiling
    C++, and a flag indicating whether it should be prefixed by sysroot.
    Mark the end of the array with a null element. 

    The component name denotes what GNU package the include file is part
    of, if any, in all uppercase letters. For example, it might be GCC
    or BINUTILS. If the package is part of a vendor-supplied operating
    system, code the component name as 0. 

    */

/* We only use the sysroot directory.  Our install process will copy all
   the appropriate gcc includes there. */
#define INCLUDE_DEFAULTS		   \
{					   \
  { GCC_INCLUDE_DIR, "GCC", 0, 0, 0 },     \
  { "/include", STANDARD_INCLUDE_COMPONENT, 0, 0, 1 }, \
  { "/../common/include", STANDARD_INCLUDE_COMPONENT, 0, 0, 1 }, \
  { 0, 0, 0, 0 }			   \
}

#undef DRIVER_SELF_SPECS
#define DRIVER_SELF_SPECS "%{m32:%eThe lv2 compiler is 64 bit only.}" \
  "%{mlp64: %{msn-ld: %eSN linker cannot handle objects compiled with -mlp64.} }"

/* This is normally set by configure if prefix and sysroot are the same.
   This is not true for lv2, but we still want the sysroot to be
   relocatable because we know it will always be in the same place
   relative to the prefix. */
#ifdef TARGET_SYSTEM_ROOT
#define TARGET_SYSTEM_ROOT_RELOCATABLE
#endif

/* The MinGW hosted cross compiler doesn't configure this define
   correctly in a Canadian cross build so we set it explicitly here. */
# define ASM_SECTION_START_OP	"\t.subsection\t-1"
# define ASM_OUTPUT_SECTION_START(FILE)	\
  fprintf ((FILE), "%s\n", ASM_SECTION_START_OP)

/* Same with the following */
#define HAVE_INITFINI_ARRAY 1
#define HAVE_LD_EH_FRAME_HDR 1
#define HAVE_LD_RO_RW_SECTION_MIXING 1
#undef USE_AS_TRADITIONAL_FORMAT

/* Specify the cost of a branch insn; roughly the number of extra insns that
   should be added to avoid a branch.

   Set this to 3 on the RS/6000 since that is roughly the average cost of an
   unscheduled conditional branch.  */

#undef BRANCH_COST
#define BRANCH_COST 8

/* lv2 doesn't support chain register/ nested functions, but we set this
 * here to avoid FAILs in the gcc testsuite. */
#define STATIC_CHAIN gen_rtx_REG (SImode, STATIC_CHAIN_REGNUM)


/* begin sce local bugzilla 23831 */
/* WARNING! This macro is one of SCE extention. No such macro in FSF GCC.

   Macro: MD_SPEC_FILE

   Specify the additional spec file that describes the machine dependent
   special processes. GCC try to find the filename from startfiles
   search path. If the file is found, the spec file is added and/or overwriten
   default one.*/
#define MD_SPEC_FILE "prxspec"
/* end sce local bugzilla 23831 */

/* begin sce local bugzilla 37796 */
/* Bugzilla 51422 : '--sysroot' is not specified when linking PRX */
#undef  SYSROOT_SPEC
#define SYSROOT_SPEC "%{mno-sn-ld|mprx|mprx-with-runtime|mlp64: --sysroot=%R; : } "
/* end sce local bugzilla 37796 */

/* begin sce local bugzilla 37796 */
/* WARNING! This macro is one of SCE extention. No such macro in FSF GCC.

   Macro: SYSTEM_EXEC_DIR_RELATIVE
   Specify additional directory list where the system commands are installed.
   This macro should be the list of relative paths from the prefix directory
   (specified by --prefix), that are concatinated with ':'. */
#define SYSTEM_EXEC_DIR_RELATIVE  "../sn/bin:../bin"
/* end sce local bugzilla 37796 */
