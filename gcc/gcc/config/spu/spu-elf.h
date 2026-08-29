
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

#ifndef OBJECT_FORMAT_ELF
 #error elf.h included before elfos.h
#endif

/* Describe how to emit uninitialized external linkage items.  */
#define BSS_SECTION_ASM_OP "\t.section .bss"
/* Describe how to emit uninitialized external linkage items.  */
#define ASM_OUTPUT_ALIGNED_BSS(FILE, DECL, NAME, SIZE, ALIGN) \
            asm_output_aligned_bss (FILE, DECL, NAME, SIZE, ALIGN)

/* #undef DOLLARS_IN_IDENTIFIERS */
/* #define DOLLARS_IN_IDENTIFIERS 0 */

/* Add sysroot prefixes.  The following macros are used only when building
   the compiler with --with-sysroot.  They have the effect of allowing to
   build a pair of PPU and SPU cross-compilers with a common sysroot; the
   SPU compiler will search for its files in ${sysroot}/usr/spu/include
   and ${sysroot}/usr/spu/lib.  */

#undef STANDARD_STARTFILE_PREFIX_1
#define STANDARD_STARTFILE_PREFIX_1 ""
#undef STANDARD_STARTFILE_PREFIX_2
#define STANDARD_STARTFILE_PREFIX_2 "/usr/spu/lib/"
#undef STANDARD_INCLUDE_DIR
#define STANDARD_INCLUDE_DIR "/usr/spu/include"

#undef  STARTFILE_SPEC
#define STARTFILE_SPEC	"%{stdmain: crt2%O%s} %{!stdmain: crt1%O%s}"

#undef  ENDFILE_SPEC
#define ENDFILE_SPEC	"crtend1%O%s"

#define PREFERRED_DEBUGGING_TYPE DWARF2_DEBUG

#define DWARF2_DEBUGGING_INFO 1
#define DWARF2_ASM_LINE_DEBUG_INFO 1

#define SET_ASM_OP		"\t.set\t"

#undef TARGET_ASM_NAMED_SECTION
#define TARGET_ASM_NAMED_SECTION  default_elf_asm_named_section

#define EH_FRAME_IN_DATA_SECTION 1

#define LINK_SPEC "%{mlarge-mem: --defsym __stack=0xfffffff0 }"

#define LIB_SPEC \
	"-( %{!shared:%{g*:-lg}} -lc -lgloss -)"

/* Turn off warnings in the assembler too. */
#undef ASM_SPEC
#define ASM_SPEC  "%{w:-W}"

