/* (C) Copyright
   Sony Computer Entertainment, Inc.,
   Toshiba Corporation,
   International Business Machines Corporation,
   2005.

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

/* Test cases for SPU single precision floating max value. We do not test
   division here. The precision problem of the SPU single precision
   division is a little complicated. Division will be tested else where. 
 */

const float f = 1e38F;
const float pmax = 1e38F * 1e38F;
const float nmax = -1e38F * 1e38F;

/* binary representation checks */
static void
test1 (void)
{
  union {
    float	f;
    int		i;
  } u;

  u.f = pmax;
  if (u.i != 0x7fffffff)
    abort ();

  u.f = nmax;
  if (u.i != 0xffffffff)
    abort ();
}

/* overflow checks */
static void
test2 (void)
{
  if (f * f != pmax)
    abort ();
  if (f * -f != nmax)
    abort ();
  if (-f * f != nmax)
    abort ();
  if (-f * -f != pmax)
    abort ();
}

/* positive max value tests */
void
test3 (void)
{
  if (pmax * pmax != pmax)
    abort ();
  if (pmax - pmax != 0.0F)
    abort ();
  if (pmax * 2.0F != pmax)
    abort ();
  if (pmax * -2.0F != nmax)
    abort ();
  if (pmax * 0.5F == pmax)
    abort ();
  if (pmax + f != pmax)
    abort ();
  if (pmax - f == pmax)
    abort ();
}

/* negative max value tests */
static void
test4 (void)
{
  if (nmax * nmax != pmax)
    abort ();
  if (nmax - nmax != 0.0F)
    abort ();
  if (nmax * 2.0F != nmax)
    abort ();
  if (nmax * -2.0F != pmax)
    abort ();
  if (nmax * 0.5F == nmax)
    abort ();
  if (nmax - f != nmax)
    abort ();
  if (nmax + f == nmax)
    abort ();
}

/* positive & negative max values tests */
static void
test5 (void)
{
  if (nmax != -pmax)
    abort ();
  if (pmax != -nmax)
    abort ();
  if (nmax + pmax != 0.0F)
    abort ();
  if (pmax + nmax != 0.0F)
    abort ();
  if (nmax - pmax != nmax)
    abort ();
  if (pmax - nmax != pmax)
    abort ();
  if (pmax * nmax != nmax)
    abort ();
  if (nmax * pmax != nmax)
    abort ();
}

int
main ()
{
  test1 ();
  test2 ();
  test3 ();
  test4 ();
  test5 ();

  return 0;
}
