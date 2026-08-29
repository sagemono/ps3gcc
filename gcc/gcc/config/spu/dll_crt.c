/* (C) Copyright
 * Sony Computer Entertainment, Inc.,
 * 2009.
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

#include <stddef.h>

#define NATS 80

static void (*_Atfuns[NATS])(void);
static size_t _Atcount = {NATS};

int atexit (void (*)(void)) __attribute__((visibility("hidden")));
int
atexit (void (*func)(void))
{
  int ret = 0;

  if (_Atcount <= 0)
#if 1
    __asm__ volatile ("stopd 0, 0, 0");
#else
    ret = -1;
#endif
  else
    _Atfuns[--_Atcount] = func;

  return ret;
}

static void __fini_atexit (void) __attribute__ ((naked, section(".fini.atexit","ax"), used));
static void
__fini_atexit (void)
{
  while (_Atcount < NATS)
    (_Atfuns[_Atcount++])();
}

