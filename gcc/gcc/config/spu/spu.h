
/* (C) Copyright
   Sony Computer Entertainment, Inc.,
   Toshiba Corporation,
   International Business Machines Corporation,
   2001,2002,2003,2004,2005,2008

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

/* Allow for SPU specific changes other than those defined in the these
   machine description files. */ 
#ifndef SPU
#define SPU
#endif

/* defined in real.h */
/* #define REAL_ARITHMETIC */

/* Machine-specific symbol_ref flags.  */
#define SYMBOL_FLAG_ALIGN1      (SYMBOL_FLAG_MACH_DEP << 0)

/* Target #defines. */

/* Names to predefine in the preprocessor for this target machine.  */

#define TARGET_CPU_CPP_BUILTINS()	spu_cpu_cpp_builtins(pfile)

#define OVERRIDE_OPTIONS spu_override_options()

#define C_COMMON_OVERRIDE_OPTIONS spu_c_common_override_options()

/* Macro: OPTIMIZATION_OPTIONS (level, size)

  Some machines may desire to change what optimizations are performed
  for various optimization levels. This macro, if defined, is executed
  once just after the optimization level is determined and before the
  remainder of the command options have been parsed. Values set in this
  macro are used as the default values for the other command line
  options. 

  level is the optimization level specified; 2 if -O2 is specified, 1 if
  -O is specified, and 0 if neither is specified. 

  size is nonzero if -Os is specified and zero otherwise.  */
#define OPTIMIZATION_OPTIONS(level,size) \
	  spu_optimization_options(level,size)

/* Print subsidiary information on the compiler version in use.  */
#define TARGET_VERSION fprintf (stderr, " (spu)");

/* target-specific version of the compiler release */
#define TARGET_RELEASE "(SPU 1.2, " __DATE__ ")"

/* Run-time compilation parameters selecting different hardware subsets.  */

extern int target_flags;

extern const char *spu_fixed_range_string;
extern const char *spu_branch_cost_string;
extern int spu_branch_cost;
extern const char *spu_max_nops_str;
extern int spu_hint_dist;
extern const char *spu_float_acc_str;
extern int spu_float_acc;
extern const char *spu_double_acc_str;
extern int spu_double_acc;

enum spu_float_accuracy { SPU_FP_COMPAT, SPU_FP_ACCURATE, SPU_FP_FAST };

/* Default target_flags if no switches specified.  */
#ifndef TARGET_DEFAULT
#define TARGET_DEFAULT (MASK_BRANCH_HINTS | MASK_VECTOR_SIMPLIFY | MASK_SAFE_DMA)
#endif

/* Target machine storage layout */

/* Define this if most significant bit is lowest numbered
   in instructions that operate on numbered bit-fields. */
#define BITS_BIG_ENDIAN 1

/* Define this if most significant byte of a word is the lowest numbered.  */
#define BYTES_BIG_ENDIAN 1

/* Define this if most significant word of a multiword number is numbered.  */
#define WORDS_BIG_ENDIAN 1

/* Number of bits in an addressable storage unit */
#define BITS_PER_UNIT 8

#define INT_TYPE_SIZE 32

#define LONG_TYPE_SIZE 32

#define LONG_LONG_TYPE_SIZE 64

#define FLOAT_TYPE_SIZE 32

#define DOUBLE_TYPE_SIZE 64

#define LONG_DOUBLE_TYPE_SIZE 64

#define VECTOR_TYPE_SIZE 128

#define WIDEST_HARDWARE_FP_SIZE 64 

/* Width in bits of a "word", which is the contents of a machine register.
   Note that this is not necessarily the width of data type `int';
   if using 16-bit ints on a 68000, this would still be 32.
   But on a machine with 16-bit registers, this would be 16.  
   For SPU I tried using 128, but that breaks a bunch of code that assumes
   word_mode/mode(__word__) is the fastest operation. */
#define BITS_PER_WORD 128

#define BITS_PER_FAST 32

/* Width of a word, in units (bytes).  */
#define UNITS_PER_WORD (BITS_PER_WORD/BITS_PER_UNIT)
#define MIN_UNITS_PER_WORD 4

#define MAX_FIXED_MODE_SIZE 128

/* Width in bits of a pointer.
   See also the macro `Pmode' defined below.  */
#define POINTER_SIZE 32


/* Allocation boundary (in *bits*) for storing pointers in memory.  */
#define POINTER_BOUNDARY 32

/* Allocation boundary (in *bits*) for storing arguments in argument list.  */
#define PARM_BOUNDARY 128

/* Allocation boundary (in *bits*) for the code of a function.  */
#define FUNCTION_BOUNDARY 64

/* Alignment of field after `int : 0' in a structure.  */
#define EMPTY_FIELD_BOUNDARY 32

/* Every structure's size must be a multiple of this.  */
/* #define STRUCTURE_SIZE_BOUNDARY 32 */

/* A bitfield declared as `int' forces `int' alignment for the struct.  */
#define PCC_BITFIELD_TYPE_MATTERS 1

/* No data type wants to be aligned rounder than this. (in bits)
 * This macros is used in to many places and ends up wasting space
 * if it's too large.  But we do want to allow data and types to have
 * a larger alignment, so make a change somewhere else. */
#define BIGGEST_ALIGNMENT (128)

/* Define this if move instructions will actually fail to work
   when given unaligned data.  */
#define STRICT_ALIGNMENT 1

#define MINIMUM_ATOMIC_ALIGNMENT 128
#define DATA_ALIGNMENT(TYPE,ALIGN) ((ALIGN) > 128 ? (ALIGN) : 128)
#define CONSTANT_ALIGNMENT(TYPE,ALIGN) ((ALIGN) > 128 ? (ALIGN) : 128)
#define LOCAL_ALIGNMENT(TYPE,ALIGN) ((ALIGN) > 128 ? (ALIGN) : 128)
#define DATA_ALIGNMENT_USER(DECL,ALIGN) (DECL_SECTION_NAME (DECL) || (ALIGN) > 128 ? (ALIGN) : 128)

/* Standard register usage.  */

/* Number of actual hardware registers.
   The hardware registers are assigned numbers for the compiler
   from 0 to just below FIRST_PSEUDO_REGISTER.
   All registers that the compiler knows about must be given numbers,
   even those that are not normally considered general registers.  */
#define FIRST_PSEUDO_REGISTER 132

#define FIXED_REGISTERS {			    \
    1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
    1, 1, 1, 1 \
}

#define CALL_USED_REGISTERS {			    \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
    1, 1, 1, 1 \
}

#define CONDITIONAL_REGISTER_USAGE \
	spu_conditional_register_usage()

/* Return number of consecutive hard regs needed starting at reg REGNO
   to hold something of mode MODE.
   This is ordinarily the length in words of a value of mode MODE
   but can be less for certain modes in special long registers. */
#define HARD_REGNO_NREGS(REGNO, MODE)   \
    ((GET_MODE_BITSIZE(MODE)+MAX_FIXED_MODE_SIZE-1)/MAX_FIXED_MODE_SIZE)

/* Value is 1 if hard register REGNO can hold a value of machine-mode MODE. */
#define HARD_REGNO_MODE_OK(REGNO, MODE) 1

/* Value is 1 if it is a good idea to tie two pseudo registers
   when one has mode MODE1 and one has mode MODE2.
   If HARD_REGNO_MODE_OK could produce different values for MODE1 and MODE2,
   for any hard reg, then this must be 0 for correct output.  */
#define MODES_TIEABLE_P(MODE1, MODE2) \
  (GET_MODE_BITSIZE (MODE1) <= MAX_FIXED_MODE_SIZE \
   && GET_MODE_BITSIZE (MODE2) <= MAX_FIXED_MODE_SIZE)

/* Specify the registers used for certain standard purposes.
   The values of these macros are register numbers.  */

#define LINK_REGISTER_REGNUM 0

/* Register to use for pushing function arguments.  */
#define STACK_POINTER_REGNUM 1

/* Register to use for computing addresses in PIC.  */
#define PIC_OFFSET_TABLE_REGNUM 126

/* Base register for access to local variables of the function.  */
#define FRAME_POINTER_REGNUM 128
#define HARD_FRAME_POINTER_REGNUM 127

#define FRAME_POINTER_REQUIRED 0

/* Because FRAME_POINTER_REQUIRED is 1, I assume the definition of
 * INTIAL_FRAME_POINTER_OFFSET doesn't matter. */
#define INITIAL_FRAME_POINTER_OFFSET(DEPTH) ((DEPTH) = 0)

/* arg pointer doesn't need to be fixed because it is always eliminated.  */
#define ARG_POINTER_REGNUM 129

#define ELIMINABLE_REGS  \
  {{ARG_POINTER_REGNUM,	 STACK_POINTER_REGNUM},				\
  {ARG_POINTER_REGNUM,	 HARD_FRAME_POINTER_REGNUM},			\
  {FRAME_POINTER_REGNUM, STACK_POINTER_REGNUM},				\
  {FRAME_POINTER_REGNUM, HARD_FRAME_POINTER_REGNUM}}

#define CAN_ELIMINATE(FROM,TO) 1 

#define INITIAL_ELIMINATION_OFFSET(FROM, TO, OFFSET) \
  ((OFFSET) = spu_initial_elimination_offset((FROM),(TO)))

/* Register in which static-chain is passed to a function.  Doesn't need
   to be fixed but it does need to be set as call used. */
#define STATIC_CHAIN_REGNUM 2

/* Register in which address to store a structure value
   is passed to a function.  Setting this to 0 means use
   the first argument register to pass a pointer. */
#define STRUCT_VALUE 0

/* 0 means return in register if possible */
#define DEFAULT_PCC_STRUCT_RETURN 0

#define HBR_REGNUM 130

/* This register is used to create dependencies between the channel
 * instruction.  It represents the internal state they modify. */
#define CHANNEL_REGNUM 131


/* Define the classes of registers for register constraints in the
   machine description.  Also define ranges of constants.

   One of the classes must always be named ALL_REGS and include all hard regs.
   If there is more than one class, another class must be named NO_REGS
   and contain no registers.

   The name GENERAL_REGS must be the name of a class (or an alias for
   another name such as ALL_REGS).  This is the class of registers
   that is allowed by "g" or "r" in a register constraint.
   Also, registers outside this class are allocated only when
   instructions express preferences for them.

   The classes must be numbered in nondecreasing order; that is,
   a larger-numbered class must never be contained completely
   in a smaller-numbered class.  For any two classes, it is very 
   desirable that there be another class that represents their union.  */
enum reg_class { 
   NO_REGS, 
   SIBCALL_REGS,
   GENERAL_REGS,
   ALL_REGS,
   LIM_REG_CLASSES 
};

#define N_REG_CLASSES (int) LIM_REG_CLASSES

/* Give names of register classes as strings for dump file.   */
#define REG_CLASS_NAMES \
{  "NO_REGS", \
   "SIBCALL_REGS", \
   "GENERAL_REGS", \
   "ALL_REGS" \
}

/* Define which registers fit in which classes.  This is an initializer 
   for a vector of HARD_REG_SET of length N_REG_CLASSES.  */
#define REG_CLASS_CONTENTS { \
    {0, 0, 0, 0, 0}, /* no regs */ \
    {0xfffffffb, 0xffffffff, 0x0000ffff, 0x00000000, 0x0}, /* sibcall regs */ \
    {0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0x3}, /* general regs */ \
    {0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0x3}} /* all regs */

/* The same information, inverted: Return the class number of the 
   smallest class containing reg number REGNO.  This could be a 
   conditional expression or could index an array.  */ 
#define REGNO_REG_CLASS(REGNO) (((REGNO) == 2 || (REGNO) >= 80) ? GENERAL_REGS : SIBCALL_REGS )

/* The class value for index registers, and the one for base regs.  */

#define INDEX_REG_CLASS GENERAL_REGS
#define BASE_REG_CLASS GENERAL_REGS

/* Get reg_class from a letter such as appears in the machine description.  */

#define REG_CLASS_FROM_LETTER(C) ((C) == 'q' ? SIBCALL_REGS : NO_REGS)

/*
  A - An immediate which can be loaded with the il/ila/ilh/ilhu
      instructions.  const_int is treated as a 32 bit value.
  B - An immediate for arithmetic instructions (e.g., ai, ceqi).
      const_int is treated as a 32 bit value.
  C - An immediate for and/xor/or instructions.  const_int is treated as
      a 32 bit value.
  D - An immediate for iohl instruction.  const_int is treated as a 32
      bit value.
  U - An immediate which can be loaded with the il/ila/ilh/ilhu
      instructions.  const_int is sign extended to 128 bit.
  W - An immediate for shift and rotate instructions.  const_int is
      treated as a 32 bit value.
  Y - An immediate for and/xor/or instructions.  const_int is sign
      extended as a 128 bit.
  Z - An immediate for iohl instruction.  const_int is sign extended to
      128 bit.
  a - An immediate which can be loaded with the il/ila/ilh/ilhu
      instructions.  const_int is treated as a 64 bit value.
  c - An immediate for and/xor/or instructions.  const_int is treated as
      a 64 bit value.
  d - An immediate for iohl instruction.  const_int is treated as a 64
      bit value.
  f - An immediate which can be loaded with fsmbi.
  I - A constant in the range [-64, 63] for shift/rotate instructions.
  J - An unsigned 7-bit constant for conversion/nop/channel
      instructions.
  K - A signed 10-bit constant for most arithmetic instructions.
  M - A signed 16 bit immediate for @code{stop}.
  N - An unsigned 16-bit constant for @code{iohl} and @code{fsmbi}.
  O - An unsigned 7-bit constant whose 3 least significant bits are 0.
  P - An unsigned 3-bit constant for 16-byte rotates and shifts
  R - Call operand, reg, for indirect calls
  S - Call operand, symbol, for relative calls.
  T - Call operand, const_int, for absolute calls.
  */

#define CONST_OK_FOR_LETTER_P(VALUE, C)  \
	  (constraint_satisfied_p (GEN_INT(VALUE), (C)))

#define CONST_DOUBLE_OK_FOR_LETTER_P(OP, C) \
	  (constraint_satisfied_p ((OP), (C)))

#define EXTRA_CONSTRAINT(OP, C)	\
	  (constraint_satisfied_p ((OP), (C)))

/* #define WORD_REGISTER_OPERATIONS */

/* Given an rtx X being reloaded into a reg required to be
   in class CLASS, return the class of reg to actually use.
   In general this is just CLASS; but on some machines
   in some cases it is preferable to use a more restrictive class.  */

#define PREFERRED_RELOAD_CLASS(X,CLASS)  (CLASS)

/*
 * #define SECONDARY_INPUT_RELOAD_CLASS(CLASS, MODE, X)	\
    ((GET_CODE(X) == CONST_INT && CONST_OK_FOR_LETTER_P(INTVAL(X), 'M'))\
     ? GENERAL_REGS : NO_REGS)

#define SECONDARY_OUTPUT_RELOAD_CLASS(CLASS, MODE, X)	\
    ( EXTRA_CONSTRAINT(X,'S') ? GENERAL_REGS : NO_REGS)
     */

/* Return the maximum number of consecutive registers
   needed to represent mode MODE in a register of class CLASS.  */
#define CLASS_MAX_NREGS(CLASS, MODE)	1

#if 0
#define CLASS_CANNOT_CHANGE_MODE GENERAL_REGS

#define CLASS_CANNOT_CHANGE_MODE_P(FROM,TO) \
        (/* (FROM) != REGmode && (TO) != REGmode && */ \
        GET_MODE_SIZE(FROM) != GET_MODE_SIZE(TO))
#endif

/*  Make sure the combine phase only creates valid SUBREG's. */
#define VALID_SUBREG(mode,inner,offset) \
	 (GET_MODE_SIZE(mode) == GET_MODE_SIZE(GET_MODE(inner)) \
	  || (GET_MODE_SIZE (GET_MODE (inner)) <= 4 && GET_MODE_SIZE (mode) <= 4))

/* Stack layout; function entry, exit and calling.  */

/* Define this if pushing a word on the stack
   makes the stack pointer a smaller address.  */
#define STACK_GROWS_DOWNWARD

#define STARTING_FRAME_OFFSET (0)

#define STACK_POINTER_OFFSET 32

#define ACCUMULATE_OUTGOING_ARGS 1

#define REG_PARM_STACK_SPACE(FNDECL) 0

#define OUTGOING_REG_PARM_STACK_SPACE 

#define FIRST_PARM_OFFSET(FNDECL) (0)

#define RETURN_POPS_ARGS(FUNDECL,FUNTYPE,SIZE) (0)

#define FUNCTION_VALUE(VALTYPE, FUNC) \
        (spu_function_value((VALTYPE),(FUNC)))

#define LIBCALL_VALUE(MODE) gen_rtx_REG (MODE, FIRST_RETURN_REGNUM)

#define FUNCTION_VALUE_REGNO_P(N) ((N) >= (FIRST_RETURN_REGNUM) && (N) <= (LAST_RETURN_REGNUM))

#define FIRST_ARG_REGNUM     3
#define LAST_ARG_REGNUM      (FIRST_ARG_REGNUM + MAX_REGISTER_ARGS - 1)
#define FIRST_RETURN_REGNUM  3
#define LAST_RETURN_REGNUM   (FIRST_RETURN_REGNUM + MAX_REGISTER_RETURN - 1)
#define MAX_REGISTER_ARGS    77
#define MAX_REGISTER_RETURN  77

#define FUNCTION_ARG_REGNO_P(N) ((N) >= (FIRST_ARG_REGNUM) && (N) <= (LAST_ARG_REGNUM))

typedef struct {
    unsigned nwords;
    unsigned nregs_used;
} CUMULATIVE_ARGS;


#define INIT_CUMULATIVE_ARGS(CUM,FNTYPE,LIBNAME,FNDECL,N_NAMED_ARGS) \
    do { CUM.nwords = 0; \
	 CUM.nregs_used = 0; \
       } while(0)


/* FUNCTION_ARG_ADVANCE (cum, mode, type, named)
 * A C statement (sans semicolon) to update the summarizer variable cum
 * to advance past an argument in the argument list.  The values mode,
 * type and named describe that argument. Once this is done, the
 * variable cum is suitable for analyzing the following argument with
 * FUNCTION_ARG, etc.
 * 
 * This macro need not do anything if the argument in question was
 * passed on the stack.  The compiler knows how to track the amount of
 * stack space used for arguments without any special help.
 */
#define FUNCTION_ARG_ADVANCE(CUM, MODE, TYPE, NAMED)	\
        ((CUM).nwords += \
	 (TYPE) && TREE_CODE (TYPE_SIZE (TYPE)) != INTEGER_CST ? 1 \
	 : (MODE) == BLKmode ? ((int_size_in_bytes(TYPE)+15) / 16) \
         : (MODE) == VOIDmode ? 1 \
	 : HARD_REGNO_NREGS((CUM).nwords,MODE))

/* FUNCTION_ARG (cum, mode, type, named)
 * A C expression that controls whether a function argument is passed in
 * a register, and which register.
 * 
 * The arguments are cum, which summarizes all the previous arguments;
 * mode, the machine mode of the argument; type, the data type of the
 * argument as a tree node or 0 if that is not known (which happens for
 * C support library functions); and named, which is 1 for an ordinary
 * argument and 0 for nameless arguments that correspond to `...' in the
 * called function's prototype.  type can be an incomplete type if a
 * syntax error has previously occurred.
 * 
 * The value of the expression is usually either a reg RTX for the hard
 * register in which to pass the argument, or zero to pass the argument
 * on the stack.
 * 
 * The value of the expression can also be a parallel RTX.  This is used
 * when an argument is passed in multiple locations.  See manual.
 * */
#define FUNCTION_ARG(CUM, MODE, TYPE, NAMED) \
        (spu_function_arg(&(CUM),(MODE),(TYPE),(NAMED)))

#define FUNCTION_ARG_PADDING(MODE,TYPE) upward

#define FUNCTION_ARG_ADJUST_OFFSET(MODE,TYPE,OFFSET_PTR) \
  do { if ((TYPE) && INTEGRAL_TYPE_P(TYPE) && GET_MODE_SIZE(MODE) < 4) \
         { \
           (OFFSET_PTR)->constant += (4 - GET_MODE_SIZE(MODE)); \
	     if ((OFFSET_PTR)->var) abort(); \
	 } \
     } while (0)

/*  GCC will treat small records just like a scalar.  When the record
    is smaller than 4 bytes we don't want to do this because it leads to
    incorrect positioning of arguments and return values. */
#define RECORD_NOT_SCALAR(TYPE,MODE) (GET_MODE_SIZE(MODE) < 4)

#define RETURN_IN_MEMORY(TYPE) \
        (TYPE_MODE(TYPE) == BLKmode \
         && ((TYPE) == 0 \
	     || TREE_CODE (TYPE_SIZE (TYPE)) != INTEGER_CST \
	     || int_size_in_bytes (TYPE) > (MAX_REGISTER_RETURN * UNITS_PER_WORD)))

#define PAD_VARARGS_DOWN 0

#define EXPAND_BUILTIN_VA_START(valist, nextarg) \
  spu_va_start (valist, nextarg)

/* Output assembler code to FILE to increment profiler label # LABELNO
   for profiling a function entry.  */

#define FUNCTION_PROFILER(FILE, LABELNO)  \
   fprintf (FILE, "\t\n")

#define EXIT_IGNORE_STACK 0

#define DYNAMIC_CHAIN_ADDRESS(FP) plus_constant ((FP), -16)

#define RETURN_ADDR_RTX(COUNT,FP) (spu_return_addr (COUNT, FP))

/* Define this macro as a C expression that is nonzero for registers 
   that are used by the epilogue or the return pattern. The stack 
   and frame pointer registers are already be assumed to be used as
   as needed. */
/* I think this only needs to be set for registers which are not
   already in FIXED_REGISTERS. ts */
#define EPILOGUE_USES(REGNO) ((REGNO)==1 ? 1 : 0)

/* Macros to check register numbers against specific register classes.  */

/* These assume that REGNO is a hard or pseudo reg number.
   They give nonzero only if REGNO is a hard reg of the suitable class
   or a pseudo reg currently allocated to a suitable hard reg.
   Since they use reg_renumber, they are safe only once reg_renumber
   has been allocated, which happens in local-alloc.c.  */
#define REGNO_OK_FOR_INDEX_P(regno)  \
   ((regno) < FIRST_PSEUDO_REGISTER || (regno > LAST_VIRTUAL_REGISTER && reg_renumber[regno] >= 0))

#define REGNO_OK_FOR_BASE_P(regno) \
   ((regno) < FIRST_PSEUDO_REGISTER || (regno > LAST_VIRTUAL_REGISTER && reg_renumber[regno] >= 0))

#define MAX_REGS_PER_ADDRESS 2

/* 1 if X is an rtx for a constant that is a valid address.  */

#define CONSTANT_ADDRESS_P(X)   spu_constant_address_p(X)

/* Nonzero if the constant value X is a legitimate general operand.
   It is given that X satisfies CONSTANT_P or is a CONST_DOUBLE. 
   Only SImode or smaller can be loaded with instructions on SPU.

   We only allow (const (plus/minus (symbol_ref) (const_int))) for
   small ints otherwise we could get relocation errors.  This causes the
   constant address to be accessed from memory, which is inefficient.
   Relocation errors can still occur, but this change does reduce the
   probability. (At least enough to solve some FAILures in the
   testsuite.)

 */

#define LEGITIMATE_CONSTANT_P(X) spu_legitimate_constant_p(X)

/* The macros REG_OK_FOR..._P assume that the arg is a REG rtx
   and check its validity for a certain class.
   We have two alternate definitions for each of them.
   The usual definition accepts all pseudo regs; the other rejects
   them unless they have been allocated suitable hard regs.
   The symbol REG_OK_STRICT causes the latter definition to be used.

   Most source files want to accept pseudo regs in the hope that
   they will get allocated to the class that the insn wants them to be in.
   Source files for reload pass need to be strict.
   After reload, it makes no difference, since pseudo regs have
   been eliminated by then.  */

#ifdef REG_OK_STRICT
# define REG_OK_STRICT_FLAG 1
#else
# define REG_OK_STRICT_FLAG 0
#endif

/* Nonzero if X is a hard reg that can be used as an index.  */
#define INT_REG_OK_FOR_INDEX_P(X,STRICT) \
	((!(STRICT) || REGNO_OK_FOR_INDEX_P (REGNO (X))))
/* Nonzero if X is a hard reg that can be used as a base reg.  */
#define INT_REG_OK_FOR_BASE_P(X,STRICT) \
	((!(STRICT) || REGNO_OK_FOR_BASE_P (REGNO (X))))

#define REG_OK_FOR_INDEX_P(X) INT_REG_OK_FOR_INDEX_P (X, REG_OK_STRICT_FLAG)
#define REG_OK_FOR_BASE_P(X)  INT_REG_OK_FOR_BASE_P (X, REG_OK_STRICT_FLAG)

#define REG_ALIGN(X) \
	(REG_POINTER(X) \
	 	? REGNO_POINTER_ALIGN (ORIGINAL_REGNO (X)) \
		: 0)

/* GO_IF_LEGITIMATE_ADDRESS recognizes an RTL expression
   that is a valid memory address for an instruction.
   The MODE argument is the machine mode for the MEM expression
   that wants to use this address.

   CONSTANT_ADDRESS_P is actually machine-independent. 
 */

#define GO_IF_LEGITIMATE_ADDRESS(MODE, X, ADDR)			\
    { if (spu_legitimate_address (MODE, X, REG_OK_STRICT_FLAG, 0))	\
	goto ADDR;						\
    }


/* Try machine-dependent ways of modifying an illegitimate address
   to be legitimate.  If we find one, return the new, valid address.
   This macro is used in only one place: `memory_address' in explow.c.

   OLDX is the address as it was before break_out_memory_refs was called.
   In some cases it is useful to look at this to decide what needs to be done.

   MODE and WIN are passed so that this macro can use
   GO_IF_LEGITIMATE_ADDRESS.

   It is always safe for this macro to do nothing.  It exists to recognize
   opportunities to optimize the output. */
#define LEGITIMIZE_ADDRESS(X,OLDX,MODE,WIN) \
  {  rtx result = spu_legitimize_address (X, OLDX, MODE);	\
     if (result != NULL_RTX)					\
       {							\
	 (X) = result;						\
	 goto WIN;						\
       }							\
  }

/* Go to LABEL if ADDR (a legitimate address expression)
   has an effect that depends on the machine mode it is used for. */
#define GO_IF_MODE_DEPENDENT_ADDRESS(ADDR,LABEL)

/* symbol_ref's of functions are not aligned to 16 byte boundary. */
#define ALIGNED_SYMBOL_REF_P(X) \
	(GET_CODE (X) == SYMBOL_REF \
          && (SYMBOL_REF_FLAGS (X) & SYMBOL_FLAG_ALIGN1) == 0 \
	  && (! SYMBOL_REF_FUNCTION_P (X) \
	      || align_functions >= 16))


/* Specify the machine mode that this machine uses
   for the index in the tablejump instruction.  */
#define CASE_VECTOR_MODE SImode

/* Define as C expression which evaluates to nonzero if the tablejump
   instruction expects the table to contain offsets from the address of the
   table.
   Do not define this if the table should contain absolute addresses. */
/* #define CASE_VECTOR_PC_RELATIVE 1 */

/* Define this as 1 if `char' should by default be signed; else as 0.  */
#define DEFAULT_SIGNED_CHAR 0

/* This flag, if defined, says the same insns that convert to a signed fixnum
   also convert validly to an unsigned one.  */
/* #define FIXUNS_TRUNC_LIKE_FIX_TRUNC */

/* Max number of bytes we can move from memory to memory
   in one reasonably fast instruction.  */
#define MOVE_MAX 16 

/* Define this if zero-extension is slow (more than one real instruction).  */
/* #define SLOW_ZERO_EXTEND */

/* Nonzero if access to memory by bytes is slow and undesirable.  */
#define SLOW_BYTE_ACCESS 0

/* Define if shifts truncate the shift count
   which implies one can omit a sign-extension or zero-extension
   of a shift count.  */
/* #define SHIFT_COUNT_TRUNCATED */

/* Value is 1 if truncating an integer of INPREC bits to OUTPREC bits
   is done just by pretending it is already truncated.  */
#define TRULY_NOOP_TRUNCATION(OUTPREC, INPREC) ((INPREC) <= 32 && (OUTPREC) <= (INPREC))

/* If defined, a C expression that returns nonzero for a class for which
   a change from mode from to mode to is invalid.  Accept VOIDmode because
   it might be passed by register_operand(). */
#define CANNOT_CHANGE_MODE_CLASS(FROM, TO, CLASS) \
	(!reload_completed \
	 && (TO) != VOIDmode \
	 && GET_MODE_SIZE (FROM) > GET_MODE_SIZE (TO) \
	 && GET_MODE_SIZE (FROM) > 4)

/* When moving complex values, what mode_class should we use.  For
 * SPU we always use MODE_INT. */
#define MODE_CLASS_FOR_COMPLEX(CLASS) MODE_INT

/* Specify the machine mode that pointers have.
   After generation of rtl, the compiler makes no further distinction
   between pointers and any other objects of this machine mode.  */
#define Pmode SImode

/* A function address in a call instruction
   is a byte address (for indexing purposes)
   so give the MEM rtx a byte's mode.  */
#define FUNCTION_MODE QImode

/* Adjust the length of an INSN.  LENGTH is the currently-computed length and
   should be adjusted to reflect any required changes.  This macro is used when
   there is some systematic length adjustment required that would be difficult
   to express in the length attribute.  */

/* #define ADJUST_INSN_LENGTH(X,LENGTH) */

/* A C expression for the cost of a branch instruction.  A value of 1 is
   the default; other values are interpreted relative to that. */
#define BRANCH_COST spu_branch_cost

/* As a first-order estimate, assume the presence of a branch hint at 
   the beginning of the fallthru block. */

#define FALLTHRU_BRANCH_COST		\
	(fallthru_cost < 6)		\
        ? BRANCH_COST			\
	: (BRANCH_COST > fallthru_cost)	\
	? BRANCH_COST - fallthru_cost : 0 

/* Define this macro if it is as good or better to call a constant
   function address than to call an address kept in a register. */
#define NO_FUNCTION_CSE

/*
 * We can use the BSD C library routines for the gnulib calls that are
 * still generated, since that's what they boil down to anyways.
 */

/* #define UDIVSI3_LIBCALL "*udiv" */
/* #define UMODSI3_LIBCALL "*urem" */

/* Tell final.c how to eliminate redundant test instructions.  */

/* Here we define machine-dependent flags and fields in cc_status
   (see `conditions.h').  */

/* Store in cc_status the expressions
   that the condition codes will describe
   after execution of an instruction whose pattern is EXP.
   Do not alter them if the instruction would not alter the cc's.  */

#define NOTICE_UPDATE_CC(EXP, INSN) \
	CC_STATUS_INIT;

#define STORE_FLAG_VALUE -1

/* NO_IMPLICIT_EXTERN_C
 * Define this macro if the system header files support C++ as well as
 * C.  This macro inhibits the usual method of using system header files
 * in C++, which is to pretend that the file's contents are enclosed in
 * `extern "C" {...}'.
 */
#define NO_IMPLICIT_EXTERN_C 1

extern struct rtx_def *spu_compare_op0;
extern struct rtx_def *spu_compare_op1;
/* extern void spu_emit_branch_or_set(int, enum rtx_code, struct rtx_def *); */

/* Control the assembler format that we output.  */

/* Output the name of the file we are compiling.  */
#define ASM_OUTPUT_SOURCE_FILENAME(STREAM, NAME) \
  do {	fprintf (STREAM, "\t.file\t");			\
	output_quoted_string (STREAM, NAME);		\
	fprintf (STREAM, "\n");				\
  } while (0)

/* Output at beginning of assembler file.  */
#if 0
#define ASM_FILE_START(FILE) fputs ("", (FILE));
#endif
/* Output to assembler file text saying following lines
   may contain character constants, extra white space, comments, etc.  */

#define ASM_APP_ON ""

/* Output to assembler file text saying following lines
   no longer contain unusual constructs.  */

#define ASM_APP_OFF ""

/* Output before read-only data.  */
#define TEXT_SECTION_ASM_OP ".text"

/* Output before writable data.  */
#define DATA_SECTION_ASM_OP ".data"
#if 0
#define READONLY_DATA_SECTION data_section
#endif
#define JUMP_TABLES_IN_TEXT_SECTION 1

/* How to refer to registers in assembler output.
   This sequence is indexed by compiler's hard-register-number (see above).  */
#undef REGISTER_NAMES
#define REGISTER_NAMES \
{"$lr", "$sp", "$2", "$3", "$4", "$5", "$6", "$7", "$8", "$9", "$10", "$11", "$12", "$13", "$14", "$15", \
 "$16", "$17", "$18", "$19", "$20", "$21", "$22", "$23", "$24", "$25", "$26", "$27", "$28", "$29", "$30", "$31", \
 "$32", "$33", "$34", "$35", "$36", "$37", "$38", "$39", "$40", "$41", "$42", "$43", "$44", "$45", "$46", "$47", \
 "$48", "$49", "$50", "$51", "$52", "$53", "$54", "$55", "$56", "$57", "$58", "$59", "$60", "$61", "$62", "$63", \
 "$64", "$65", "$66", "$67", "$68", "$69", "$70", "$71", "$72", "$73", "$74", "$75", "$76", "$77", "$78", "$79", \
 "$80", "$81", "$82", "$83", "$84", "$85", "$86", "$87", "$88", "$89", "$90", "$91", "$92", "$93", "$94", "$95", \
 "$96", "$97", "$98", "$99", "$100", "$101", "$102", "$103", "$104", "$105", "$106", "$107", "$108", "$109", "$110", "$111", \
 "$112", "$113", "$114", "$115", "$116", "$117", "$118", "$119", "$120", "$121", "$122", "$123", "$124", "$125", "$126", "$127", \
 "$vfp", "$vap", "hbr", "channel" \
}

#define SCRATCH_REG_0 2

/* This is BSD, so it wants DBX format.  */
/* #define DBX_DEBUGGING_INFO */

/* How to renumber registers for dbx and gdb. */
#define DBX_REGISTER_NUMBER(REGNO) (REGNO)

/* Do not break .stabs pseudos into continuations.  */
#define DBX_CONTIN_LENGTH 0

/* This is the char to use for continuation (in case we need to turn
   continuation back on).  */
#define DBX_CONTIN_CHAR '?'

/* Don't use the `xsfoo;' construct in DBX output; this system
   doesn't support it.  */
#define DBX_NO_XREFS

/* This is how to output the definition of a user-level label named NAME,
   such as the label on a static function or variable NAME.  */

#define ASM_OUTPUT_LABEL(FILE,NAME)	\
  do { assemble_name (FILE, NAME); fputs (":\n", FILE); } while (0)

/* This is how to output a reference to a user-level label named NAME.
   `assemble_name' uses this.  */
#undef  ASM_OUTPUT_LABELREF
#define ASM_OUTPUT_LABELREF(FILE, NAME) \
  asm_fprintf (FILE, "%U%s", default_strip_name_encoding (NAME))

/* The prefix to add to user-visible assembler symbols. */
#define USER_LABEL_PREFIX ""

#define LOCAL_LABEL_PREFIX "."

/* This is how to output an element of a case-vector that is absolute. */
#define ASM_OUTPUT_ADDR_VEC_ELT(FILE, VALUE)  \
  fprintf (FILE, "\t.word .L%d\n", VALUE)

/* This is how to output an element of a case-vector that is relative.  */

/* When this is defined jump tables will be used for switch statements
  when generating PIC.  */
#define ASM_OUTPUT_ADDR_DIFF_ELT(FILE, BODY, VALUE, REL)  \
  fprintf (FILE, "\t.word .L%d-.L%d\n", VALUE, REL)

/* This is how to output an assembler line
   that says to advance the location counter
   to a multiple of 2**LOG bytes.  */

#define ASM_OUTPUT_ALIGN(FILE,LOG)  \
  do { if (LOG!=0) fprintf (FILE, "\t.align\t%d\n", (LOG)); } while (0)



/* This is how to output an assembler line that says to advance the
   location counter by SIZE bytes.  */

#undef SKIP_ASM_OP
#define SKIP_ASM_OP	"\t.space\t"

/* This says how to output an assembler line
   to define a global common symbol.  */

#define ASM_OUTPUT_COMMON(FILE, NAME, SIZE, ROUNDED)  \
( fputs (".comm ", (FILE)),			\
  assemble_name ((FILE), (NAME)),		\
  fprintf ((FILE), ",%d\n", (ROUNDED)))

/* This says how to output an assembler line
   to define a local common symbol.  */

#define ASM_OUTPUT_LOCAL(FILE, NAME, SIZE, ROUNDED)  \
( fputs (".lcomm ", (FILE)),			\
  assemble_name ((FILE), (NAME)),		\
  fprintf ((FILE), ",%d\n", (ROUNDED)))

/* Store in OUTPUT a string (made with alloca) containing
   an assembler-name for a local static variable named NAME.
   LABELNO is an integer which is different for each call.  */

#define ASM_FORMAT_PRIVATE_NAME(OUTPUT, NAME, LABELNO)	\
( (OUTPUT) = (char *) alloca (strlen ((NAME)) + 10),	\
  sprintf ((OUTPUT), "%s.%d", (NAME), (LABELNO)))

/* Print an instruction operand X on file FILE.
   CODE is the code from the %-spec that requested printing this operand;
   if `%z3' was used to print operand 3, then CODE is 'z'.  */

#define PRINT_OPERAND(FILE, X, CODE)  print_operand(FILE, X, CODE)
/* Print a memory operand whose address is X, on file FILE. */

#define PRINT_OPERAND_ADDRESS(FILE, ADDR)  \
 print_operand_address (FILE, ADDR)


/* FIXME: TRANSMETA had 16 for SIZE */
#define TRAMPOLINE_SIZE 8
#define TRAMPOLINE_ALIGNMENT 128

/* This could be better if we added relocations which allow the
 * trampoline insns to be created using only an ilhu and iohl.
 * ilhu $14,tramp_hi(FNADDR)
 * iohl $14,tramp_lo(FNADDR)
 * ilhu $15,chain_hi(CXT)
 * iohl $15,chain_lo(CXT)
 */
#define  INITIALIZE_TRAMPOLINE(TRAMP,FNADDR,CXT) do {	\
  rtx ila = gen_reg_rtx(SImode); \
  rtx bra = gen_reg_rtx(SImode); \
  rtx i1 = gen_reg_rtx(SImode); \
  rtx i2 = gen_reg_rtx(SImode); \
  emit_move_insn(ila, CXT); \
  emit_move_insn(bra, FNADDR); \
  emit_move_insn(ila, gen_rtx_ASHIFT(SImode, ila, GEN_INT(7))); \
  emit_move_insn(bra, gen_rtx_ASHIFT(SImode, bra, GEN_INT(5))); \
  emit_move_insn(i1,  GEN_INT (0x42000000+STATIC_CHAIN_REGNUM)); \
  emit_move_insn(i2,  GEN_INT (0x30000000)); \
  emit_insn(gen_addsi3(ila,ila,i1)); \
  emit_insn(gen_addsi3(bra,bra,i2)); \
  emit_move_insn (gen_rtx_MEM ( Pmode, memory_address (Pmode, TRAMP)), ila); \
  emit_move_insn (gen_rtx_MEM ( Pmode, memory_address (Pmode, plus_constant (TRAMP, 4))), bra); \
  emit_insn(gen_sync()); \
} while (0)

#define STACK_BOUNDARY 128

#define STACK_CHECK_BUILTIN 1

#define STACK_SIZE_MODE SImode

#define STACK_SAVEAREA_MODE(save_level)    \
  (save_level == SAVE_FUNCTION ? VOIDmode  \
  : save_level == SAVE_NONLOCAL ? SImode : Pmode)



#define MULTILIB_DEFAULTS { "msmall-mem" }



#define INCOMING_RETURN_ADDR_RTX gen_rtx_REG(Pmode, LINK_REGISTER_REGNUM)
#define DWARF_FRAME_RETURN_COLUMN DWARF_FRAME_REGNUM (LINK_REGISTER_REGNUM)

#define ARG_POINTER_CFA_OFFSET(FNDECL) (-STACK_POINTER_OFFSET)

/* Defines for exception handling.  Use the DWARF2 unwinder not SJLJ. */
/* Doesn't work yet and currently has a larger memory requiremnt than
 * SJLJ.  The larger memory requirement can be fixed.
#define DWARF2_FRAME_INFO 1
#define DWARF2_UNWIND 1
#define EH_RETURN_DATA_REGNO(N)	((N) < 4 ? (N) + 11 : INVALID_REGNUM)
#define EH_RETURN_STACKADJ_RTX	gen_rtx_MEM (SImode, gen_rtx_REG(SImode, STACK_POINTER_REGNUM))
#define EH_RETURN_HANDLER_RTX gen_rtx_MEM (SImode, gen_rtx_PLUS(SImode, gen_rtx_REG(Pmode, STACK_POINTER_REGNUM), GEN_INT(16)))
*/

#define SAME_MODES(ops) (GET_MODE(ops[0]) == GET_MODE(ops[1]))
#define SAME_MODES3(ops) (GET_MODE(ops[0]) == GET_MODE(ops[1]) \
    			&& GET_MODE(ops[1]) == GET_MODE(ops[2]))
#define SAME_MODES4(ops) (GET_MODE(ops[0]) == GET_MODE(ops[1]) \
    			&& GET_MODE(ops[1]) == GET_MODE(ops[2]) \
    			&& GET_MODE(ops[2]) == GET_MODE(ops[3]))

/*  When moving MEMs into register during loop analysis, GCC will do
 *  copy propogation on the new register.  Loop unrolling only works for
 *  register that existed before loop analysis started, so it doesn't
 *  like the new registers generated.  We get better unrolling if we
 *  don't propogate the new register. */
#define DONT_PROPAGATE_IN_LOOP_ANALYSIS

#define TARGET_FLOAT_FORMAT     	SPU_FLOAT_FORMAT

#define ROUND_TOWARDS_ZERO 1

/* ROUND_TOWARDS_ZERO above controls all floating point modes but
   single and double have different rounding on the SPU */
#define DOUBLE_ROUND_TOWARDS_ZERO 0

#ifndef MODE_HAS_NANS
#define MODE_HAS_NANS(MODE)                                     \
  (FLOAT_MODE_P (MODE) 						\
   && MODE != SFmode						\
   && TARGET_FLOAT_FORMAT == SPU_FLOAT_FORMAT                  	\
   && !LARGEST_EXPONENT_IS_NORMAL (GET_MODE_BITSIZE (MODE)))
#endif
                                                                              
#ifndef MODE_HAS_INFINITIES
#define MODE_HAS_INFINITIES(MODE)                               \
  (FLOAT_MODE_P (MODE) 						\
   && MODE != SFmode                                            \
   && TARGET_FLOAT_FORMAT == SPU_FLOAT_FORMAT  	                \
   && !LARGEST_EXPONENT_IS_NORMAL (GET_MODE_BITSIZE (MODE)))
#endif
                                                                              
#ifndef MODE_HAS_SIGN_DEPENDENT_ROUNDING
#define MODE_HAS_SIGN_DEPENDENT_ROUNDING(MODE)                  \
  (FLOAT_MODE_P (MODE)                                          \
    && MODE != SFmode                                           \
    && TARGET_FLOAT_FORMAT == SPU_FLOAT_FORMAT                  \
   && !ROUND_TOWARDS_ZERO)
#endif

#ifndef MODE_HAS_SIGNED_ZEROS
#define MODE_HAS_SIGNED_ZEROS(MODE) \
  (FLOAT_MODE_P (MODE) && TARGET_FLOAT_FORMAT == SPU_FLOAT_FORMAT)
#endif

/* With single precision fp, the instructions never return negative zero
   so we cannot optimize based on that folding z - 0 is not always z.  
   Similarily for x*1 or x/1 which cannot be simplified to x.  This check
   also succesfully prevents the optimizations for denorms. 
   */
#define SAFE_FOR_MINUS_ZERO(MODE)                   \
  (MODE != SFmode || spu_float_acc == SPU_FP_COMPAT || flag_unsafe_math_optimizations)

#define CC1_SPEC \
	"%{!fno-aggressive-cmov:-faggressive-cmov} "

extern int spu_move_ratio;
#define MOVE_RATIO spu_move_ratio 

#define ADDRESSES_NEVER_TRAP

#define MOVE_BY_PIECES_P(SIZE, ALIGN)  (spu_move_by_pieces_ninsns (SIZE, ALIGN) < (MOVE_RATIO))

#undef TARGET_MODE_OFFSET
#define TARGET_MODE_OFFSET(OLDMODE, NEWMODE)	\
	spu_mode_offset(OLDMODE, NEWMODE)

#define INIT_EXPANDERS spu_init_expanders()

extern int pragma_loop_seen;

#define ORDER_REGS_FOR_LOCAL_ALLOC spu_order_regs_for_local_alloc()

/* Provide an accurate MII for modulo scheduling. */
#define RES_MII(G) spu_res_mii(G)

/* With so many registers we want to increase the threshold the loop
   optimizer uses to determing if invariants should be moved out of a
   loop. */
#define INCREASE_LOOP_THRESHOLD 128

/* Target pragma.  */
#define REGISTER_TARGET_PRAGMAS() do {				\
  /** SCE bugilla #11003 **/ \
  /* APPLE LOCAL begin pragma reverse_bitfileds, ms_struct */ \
  c_register_pragma (0, "reverse_bitfields", darwin_pragma_reverse_bitfields);	\
  c_register_pragma (0, "ms_struct", darwin_pragma_ms_struct);	\
  /* APPLE LOCAL end pragma reverse_bitfileds, ms_struct */ \
} while (0)

/** SCE bugilla #11003 **/
/* APPLE LOCAL begin pragma reverse_bitfields, ms_struct */
/* True if pragma reverse_bitfields is in effect.  */
extern int darwin_reverse_bitfields;
/* True if pragma ms_struct is in effect.  */
extern int darwin_ms_struct;
/* APPLE LOCAL end pragma reverse_bitfields, ms_struct */

/* We want to generate warnings when in pic code */
#define NOTICE_STATIC_STORAGE_VARS(VARS)  spu_notice_static_storage_vars(VARS)


/* sce bugzilla 43400 */
/* XXX: This is sce original macro extension.

   If this is macro defined, tree expansion path does not
   check if 2nd arg of __builtin_expect is constant.
   SPU GCC port allows __bulitin_epand to have variable args. So the macro is defiend. */
#define SKIP_BUILTIN_ARG_CHECK 1
