/* SPU specific support for 32-bit ELF.

   (C) Copyright
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

/* Extra info kept for SPU sections.  */

struct spu_elf_stack_info;

struct _spu_elf_section_data
{
  struct bfd_elf_section_data elf;

  /* Stack analysis info kept for this section.  */

  struct spu_elf_stack_info *stack_info;
};

#define spu_elf_section_data(sec) \
  ((struct _spu_elf_section_data *) elf_section_data (sec))

extern void spu_elf_plugin (int);
extern bfd_boolean spu_elf_create_sections (bfd *, struct bfd_link_info *);
extern bfd_boolean spu_elf_size_sections (bfd *, struct bfd_link_info *);
extern void spu_elf_set_link_options (struct bfd_link_info *, int, int, int, int, int);
extern asection *spu_elf_check_vma (bfd *, bfd_vma, bfd_vma);
