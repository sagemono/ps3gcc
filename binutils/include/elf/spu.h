/* SPU ELF support for BFD.
   Copyright 1999, 2000 Free Software Foundation, Inc.

   This file is part of BFD, the Binary File Descriptor library.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

#ifndef _ELF_SPU_H
#define _ELF_SPU_H

#include "elf/reloc-macros.h"

/* elf32-spu.c depends on these being consecutive. */
START_RELOC_NUMBERS (elf_spu_reloc_type)
     RELOC_NUMBER (R_SPU_NONE,		 0)
     RELOC_NUMBER (R_SPU_ADDR10,	 1)
     RELOC_NUMBER (R_SPU_ADDR16,	 2)
     RELOC_NUMBER (R_SPU_ADDR16_HI,	 3)
     RELOC_NUMBER (R_SPU_ADDR16_LO,	 4)
     RELOC_NUMBER (R_SPU_ADDR18,	 5)
     RELOC_NUMBER (R_SPU_GLOB_DAT,	 6)
     RELOC_NUMBER (R_SPU_REL16,		 7)
     RELOC_NUMBER (R_SPU_ADDR7,		 8)
     RELOC_NUMBER (R_SPU_REL9,		 9)
     RELOC_NUMBER (R_SPU_REL9I,		10)
     RELOC_NUMBER (R_SPU_ADDR10I,	11)
     RELOC_NUMBER (R_SPU_ADDR16I,	12)
END_RELOC_NUMBERS (R_SPU_max)


/* Processor specific flags for the ELF header e_flags field.  */

#define	EF_PPC_EMB		0x80000000	/* PowerPC embedded flag  */

						/* CYGNUS local bits below */
#define	EF_PPC_RELOCATABLE	0x00010000	/* PowerPC -mrelocatable flag */
#define	EF_PPC_RELOCATABLE_LIB	0x00008000	/* PowerPC -mrelocatable-lib flag */

/* Processor specific section headers, sh_type field */

#define SHT_ORDERED		SHT_HIPROC	/* Link editor is to sort the \
						   entries in this section \
						   based on the address \
						   specified in the associated \
						   symbol table entry.  */

/* Processor specific section flags, sh_flags field */

#define SHF_EXCLUDE		0x80000000	/* Link editor is to exclude \
						   this section from executable \
						   and shared objects that it \
						   builds when those objects \
						   are not to be furhter \
						   relocated.  */

#if (defined(BPA))
/* Program header extensions */
#define PT_SPU_INFO             0x70000000      /* SPU Dynamic Object Information */
#endif

/* SPU plugin information */
#define SPU_PLUGIN_NAMESZ               8
#define SPU_PLUGIN_NAME         "SPUNAME"
#define SPU_PTNOTE_SPUNAME	".note.spu_name"
#define SPU_PLUGIN_LOOKUPNAMESZ        32

typedef struct {
  unsigned long namesz;
  unsigned long descsz;
  unsigned long type;
  char          name[SPU_PLUGIN_NAMESZ];
  char          lookupname[SPU_PLUGIN_LOOKUPNAMESZ];
} SPUPLUGIN_INFO;

#endif /* _ELF_SPU_H */
