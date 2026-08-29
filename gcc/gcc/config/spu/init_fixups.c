/* (C) Copyright
 * Sony Computer Entertainment, Inc.,
 * 2008.
 *
 * This file is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * In addition to the permissions in the GNU General Public License, the
 * Free Software Foundation gives you unlimited permission to link the
 * compiled version of this file with other programs, and to distribute
 * those programs without any restriction coming from the use of this
 * file.  (The General Public License restrictions do apply in other
 * respects; for example, they cover modification of the file, and
 * distribution when not linked into another program.)
 *
 * This file is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this file; see the file COPYING.  If not, write to the Free
 * Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.  */

/* Patch fixups.  For code to really be PIC, we must update any pointers
 * that are stored in data.  The linker has recorded these locations in
 * the .fixup section.
 * The linker scripts makes sure __fixup_start is the first symbol in
 * .fixup.  We define it here as static, so it won't become a dynamic
 * symbol. */

#include <spu_intrinsics.h>
#include <stdint.h>

static unsigned int __fixup_start[0] __attribute__((section(".fixup_head", "a"), aligned(4), used));

/* We use $126 explicitly here and compile with -mfixed-range=79 to make
 * sure it is setup properly when compiling with -fPIC.  When this file
 * is not compiled with -fPIC, the user must setup up $126. */
register unsigned int si_r126 asm ("$126");

static void __init_fixups (void) __attribute__ ((naked, section(".init.fixups","ax"), used));
static void
__init_fixups (void)
{
  unsigned int * fixup = __fixup_start;
  __vector unsigned int sbase = spu_splats (si_r126);
  for ( ; *fixup; fixup++)
    {
      unsigned int info = *fixup;
      __vector unsigned int *addr =  
		  (__vector unsigned int *)((si_r126 + info) & -16);
      *addr = *addr + (sbase & spu_maskw (info));
    }
}
