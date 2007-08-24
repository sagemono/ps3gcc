
/* (C) Copyright
   Sony Computer Entertainment, Inc.,
   Toshiba Corporation,
   International Business Machines Corporation,
   2001,2002,2003,2004,2005,2006,2007

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

#undef CPP_SPEC
#define	CPP_SPEC "%{mis:-D__IS__}"

#define TARGET_OS_CPP_BUILTINS()	\
  do {					\
    builtin_define("__CELLOS_LV2__");	\
    if (spu_float_acc == SPU_FP_ACCURATE) \
      builtin_define("__FLOAT_ACCURATE__");	\
    else if (spu_float_acc == SPU_FP_FAST) \
      builtin_define("__FLOAT_FAST__");	\
    if (spu_double_acc == SPU_FP_ACCURATE) \
      builtin_define("__DOUBLE_ACCURATE__");	\
    else if (spu_double_acc == SPU_FP_FAST) \
      builtin_define("__DOUBLE_FAST__");	\
  } while (0)

/* The GCC testsuite is very sensitive to changes in some default options.
   We use the -testing flag to disable some of our defaults.  */
#undef CC1_SPEC
#define CC1_SPEC \
	"%{!testing:%{!fbuiltin:-fno-builtin}} " \
	"%{testing:%{!mno-error-reloc:;!merror-reloc:;!mwarn-reloc:;:-mno-warn-reloc}} " \
	"%{!fno-aggressive-cmov:-faggressive-cmov} " \
	"%{!fno-strict-aligned:-fstrict-aligned} " \
	"%{Os:%{!mhint-max-nops*:-mhint-max-nops=0} %{!mdual-nops:-mno-dual-nops}} " 

#undef CC1PLUS_SPEC
#define CC1PLUS_SPEC \
"%{!testing:%{!fexceptions:-fno-exceptions %{!frtti: -fno-rtti}}} " \
"%{!fthreadsafe-statics:-fno-threadsafe-statics}"


#undef CC1_ONLY_SPEC
#define CC1_ONLY_SPEC "%{!testing:%{!std=*:-std=gnu99}}"

#undef ASM_SPEC
#define ASM_SPEC  "%{w:-W}"

#undef  STARTFILE_SPEC
#define STARTFILE_SPEC	"%{cstdmain:crt2%O%s; " \
                        "  mraw|mapusim:crt3%O%s; " \
                        "  mcellsim|mspusim:cs-crt0%O%s; " \
                        "  mis:crt4%O%s; " \
                        "  mspurs-job:job_start_gcc%O%s; " \
                        "  mspurs-task:spurs_task%O%s; " \
                        "  :crt1%O%s} "

#undef LIB_SPEC
#define LIB_SPEC "--start-group -lc -lgcc -lstdc++ -lsupc++ " \
		 "%{mspurs-task:-lspurs -lsync} " \
		 "%{mspurs-job:-lspurs -lsync} " \
		 "%{mraw:-lrawspu; mapusim:-lapusim; mcellsim|mspusim:-lcellsim; mis: ; :-lsputhread} " \
		 "--end-group" \
                 "%{mspurs-job: %{fPIC:-Ttext=0x0; fpic:-Ttext=0x0; :-Ttext=0x4c00}} " \
                 "%{mspurs-task:-Ttext=0x3000} "

#undef LINK_GCC_C_SEQUENCE_SPEC 
#define LINK_GCC_C_SEQUENCE_SPEC "%L"

#undef	MULTILIB_DEFAULTS

#undef STARTFILE_PREFIX_SPEC
#define STARTFILE_PREFIX_SPEC "/lib/"

#undef STANDARD_INCLUDE_DIR
#define STANDARD_INCLUDE_DIR "/include"

#define INCLUDE_DEFAULTS		   \
{					   \
  /* This is the dir for fixincludes and for gcc's private headers.  */ \
  { GCC_INCLUDE_DIR, "GCC", 0, 0, 0 },     \
  { "/include", STANDARD_INCLUDE_COMPONENT, 0, 0, 1 }, \
  { "/../common/include", STANDARD_INCLUDE_COMPONENT, 0, 0, 1 }, \
  { 0, 0, 0, 0, 0 }			   \
}

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

/*
     An expression whose value is 1 or 0, according to whether the type
     `char' should be signed or unsigned by default.  The user can
     always override this default with the options `-fsigned-char' and
     `-funsigned-char'.
*/
/*
  To keep type definitions between PPU and SPU consistent,
  the plain char should be equivalent to 'signed char'.
 */
#undef  DEFAULT_SIGNED_CHAR
#define DEFAULT_SIGNED_CHAR   1

/*
  Definition of wchar_t
*/
#undef  WCHAR_TYPE
#define WCHAR_TYPE "short unsigned int"

#undef  WCHAR_TYPE_SIZE
#define WCHAR_TYPE_SIZE 16

#undef  WINT_TYPE
#define WINT_TYPE "int"
