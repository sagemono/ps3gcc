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

/* We want __CTOR_END__ and __DTOR_END__ to be the last entries in
 * .ctors and .dtors.   We achieve this by naming the file with crtend. 
 *
 * We want __init_ctors to be at the end of .init.  We achieve this by
 * placing it at the start of the link command line which is special
 * cased by the linker script.
 *
 * We want all of this in the same file so we can declare the
 * __CTOR_END__ and __DTOR_END__ as static so they don't show up as
 * dynamic symbols. */

typedef void (*func_ptr) (void);

static func_ptr __CTOR_END__[1]
  __attribute__ ((section(".ctors"), aligned(4)))
  = { (func_ptr) (0) };

static func_ptr __DTOR_END__[1]
  __attribute__((section(".dtors"), aligned(4), used))
  = { (func_ptr) (0) };

void __init_ctors (void) __attribute__ ((naked, section(".init")));

void __init_ctors (void)
{
  func_ptr *p;

  /* The compiler assumes all symbols are 16 byte aligned, which is
   * not the case for __CTOR_END__.  This inline assembly makes sure
   * the address is loaded into a register for which the compiler does
   * not assume anything about alignment. */
  asm ("\n" : "=r" (p) : "0" (__CTOR_END__ - 1));

  for (; *p != (func_ptr) -1; p--)
    (*p) ();
}
