/* Definitions of target machine for GNU compiler,
   for 64 bit PowerPC linux. */

/* (C) Copyright
   Sony Computer Entertainment, Inc.,
   Toshiba Corporation,
   International Business Machines Corporation,
   2001,2002,2003,2004,2005,2006.

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

/* Override sysv4.h.  */
#undef	EXTRA_SUBTARGET_SWITCHES
#define EXTRA_SUBTARGET_SWITCHES					\
  {"profile-kernel",	 MASK_PROFILE_KERNEL,				\
   N_("Call mcount for profiling before a function prologue") },	\
  {"no-profile-kernel",	-MASK_PROFILE_KERNEL,				\
   N_("Call mcount for profiling after a function prologue") },		\
  {"p32",		MASK_PPC64_LP32,				\
   N_("same as -mlp32 option")},					\
  {"lp32",		MASK_PPC64_LP32,				\
   N_("Generate codes in ILP32 model.(defualt)")},			\
  {"lp64",		~MASK_PPC64_LP32,				\
   N_("Generate codes in LP64 model.")},				\
  {"prxfixup",          0,						\
   N_("invoke ppu-lv2-prx-fixup after linking. This is default.") },	\
  {"no-prxfixup",       0,						\
   N_("does not invoke ppu-lv2-prx-fixup after linking. Special option for toolchain developer.") }, \
  {"prx",          0,							\
   N_("Link objects in PRX mode.") },			\
  { "tb",		~MASK_PPC64_LP32, N_("Generate testbench code") },

/* collect2.c also refers to TARGET_VERSION. Don't access 
   variables defines in rs6000.c in this macro. */
#undef  TARGET_VERSION
#define TARGET_VERSION fprintf (stderr, " (PowerPC64, TestBench 64bit pointer)")

/* Set defaults for CELL PPU
   linux64.h is the last rs6000 specific header included by tm.h */
#undef TARGET_DEFAULT
#define TARGET_DEFAULT (MASK_POWERPC | MASK_POWERPC64 | MASK_64BIT \
			| MASK_NEW_MNEMONICS | MASK_ALTIVEC \
			| MASK_PPC_GPOPT | MASK_PPC_GFXOPT | MASK_MFCRF )

/*  Macro: CPP_SPEC */
#undef CPP_SPEC
#define	CPP_SPEC "-D__PPU__ -D__TESTBENCH__ " \
	"%{fno-exceptions:-D_NO_EX} " \
        "%{mlp64:-funsigned-char} "

/*  Macro: ASM_SPEC */
#undef	ASM_SPEC
#define ASM_SPEC "-a64 " \
"-mbig -mcellppu " \
"%{.s: %{mregnames} %{mno-regnames}} " \
"%{.S: %{mregnames} %{mno-regnames}} " \
"%{v:-V} %{Qy:} %{!Qn:-Qy} %{Wa,*:%*} "

/*  Macro: LINK_COMMAND_SPEC */
#undef LINK_COMMAND_SPEC
#define LINK_COMMAND_SPEC \
"%{!fsyntax-only:%{!c:%{!M:%{!MM:%{!E:%{!S: " \
"%(linker) %l " LINK_PIE_SPEC "%X %{o*} %{A} %{d} %{e*} %{m} %{N} %{n} %{r} " \
"%{s} %{t} %{u*} %{x} %{z} %{Z} %{!A:%{!nostdlib:%{!nostartfiles:%S}}} " \
"%{static:} %{L*} %(mfwrap) %(link_libgcc) %o %(mflib) " \
"%{fprofile-arcs|fprofile-generate:-lgcov} " \
"%{!nostdlib:%{!nodefaultlibs:%(link_gcc_c_sequence)}} " \
"%{!A:%{!nostdlib:%{!nostartfiles:%E}}} %{T*} " \
"}}}}}} "

/*  Macro: LIB_SPEC */
#undef LIB_SPEC
#define LIB_SPEC "--start-group -lc -lgcc -lstdc++ -lsupc++ --end-group"

/*  Macro: STARTFILE_SPEC */
#undef STARTFILE_SPEC
#define STARTFILE_SPEC ""

/*  Macro: ENDFILE_SPEC */
#undef ENDFILE_SPEC
#define ENDFILE_SPEC ""

/*  Macro: EXTRA_SPECS */
#undef	SUBTARGET_EXTRA_SPECS
#define	SUBTARGET_EXTRA_SPECS

/*  Macro: INCLUDE_DEFAULTS */
#undef INCLUDE_DEFAULTS
#define INCLUDE_DEFAULTS		   \
{					   \
  { GCC_INCLUDE_DIR, "GCC", 0, 0, 0 },     \
  { "/include", STANDARD_INCLUDE_COMPONENT, 0, 0, 1 }, \
  { "/include/sys", STANDARD_INCLUDE_COMPONENT, 0, 0, 1 }, \
  { 0, 0, 0, 0 }			   \
}


