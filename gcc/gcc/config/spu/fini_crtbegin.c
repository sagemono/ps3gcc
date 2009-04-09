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

/* We want __CTOR_LIST__ and __DTOR_LIST__ to be the first entries in
 * .ctors and .dtors.   We achieve this by putting them in the special
 * sections .ctors_head and .dtors_head. 
 *
 * We want __fini_dtors to be at the end of .fini.  We achieve this by
 * putting it in the special section .fini.dtors.
 *
 * We want all of this in the same file so we can declare the
 * __CTOR_LIST__ and __DTOR_LIST__ as static so they don't show up as
 * dynamic symbols. */

typedef void (*func_ptr) (void);

static func_ptr __CTOR_LIST__[1]
  __attribute__ ((section(".ctors_head"), aligned(4), used))
  = { (func_ptr) (-1) };

static func_ptr __DTOR_LIST__[1]
  __attribute__((section(".dtors_head"), aligned(4)))
  = { (func_ptr) (-1) };

static void __fini_dtors (void) __attribute__ ((naked, section(".fini.dtors","ax"), used));

static void
__fini_dtors (void)
{
  static func_ptr *p = 0;
  /* Use p to make sure we only call the destructors once. */
  if (!p)
    {
      func_ptr *lp;
      /* __DTOR_LIST__ is not 16 byte aligned, but the compiler assumes
       * all symbols are, so we need to use this asm to load it.  */
      asm ("" : "=r" (lp) : "0" (__DTOR_LIST__ + 1));
      p = lp;
      for (; *lp; lp++)
	(*lp) ();
    }
}
