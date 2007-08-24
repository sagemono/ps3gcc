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

/* test cases for SPU single precision floating max value. We do not test
   division here. The precision problem of the SPU single precision
   division is a little complicated. Division will be tested else where. 

   These tests verify that we do constant folding of max values.
*/

extern void link_error (void);

const float f = 1e38F;
const float pmax = 1e38F * 1e38F;
const float nmax = -1e38F * 1e38F;

/* overflow checks */
void
test1 (void)
{
  if (f * f != pmax)
    link_error ();
}

void
test2 (void)
{
  if (f * -f != nmax)
    link_error ();
}

void
test3 (void)
{
  if (-f * f != nmax)
    link_error ();
}

void
test4 (void)
{
  if (-f * -f != pmax)
    link_error ();
}

/* positive max value tests */
void
test5 (void)
{
  if (pmax * pmax != pmax)
    link_error ();
}

void
test6 (void)
{
  if (pmax - pmax != 0.0F)
    link_error ();
}

void
test7 (void)
{
  if (pmax * 2.0F != pmax)
    link_error ();
}

void
test8 (void)
{
  if (pmax * -2.0F != nmax)
    link_error ();
}

void
test9 (void)
{
  if (pmax * 0.5F == pmax)
    link_error ();
}

void
test10 (void)
{
  if (pmax + f != pmax)
    link_error ();
}

void
test11 (void)
{
  if (pmax - f == pmax)
    link_error ();
}

/* negative max value tests */
void
test12 (void)
{
  if (nmax * nmax != pmax)
    link_error ();
}

void
test13 (void)
{
  if (nmax - nmax != 0.0F)
    link_error ();
}

void
test14 (void)
{
  if (nmax * 2.0F != nmax)
    link_error ();
}

void
test15 (void)
{
  if (nmax * -2.0F != pmax)
    link_error ();
}

void
test16 (void)
{
  if (nmax * 0.5F == nmax)
    link_error ();
}

void
test17 (void)
{
  if (nmax - f != nmax)
    link_error ();
}

void
test18 (void)
{
  if (nmax + f == nmax)
    link_error ();
}

/* positive & negative max values tests */
void
test19 (void)
{
  if (nmax != -pmax)
    link_error ();
}

void
test20 (void)
{
  if (pmax != -nmax)
    link_error ();
}

void
test21 (void)
{
  if (nmax + pmax != 0.0F)
    link_error ();
}

void
test22 (void)
{
  if (pmax + nmax != 0.0F)
    link_error ();
}

void
test23 (void)
{
  if (nmax - pmax != nmax)
    link_error ();
}

void
test24 (void)
{
  if (pmax - nmax != pmax)
    link_error ();
}

void
test25 (void)
{
  if (pmax * nmax != nmax)
    link_error ();
}

void
test26 (void)
{
  if (nmax * pmax != nmax)
    link_error ();
}

int
main ()
{
  test1 ();
  test2 ();
  test3 ();
  test4 ();
  test5 ();
  test6 ();
  test7 ();
  test8 ();
  test9 ();
  test10 ();
  test11 ();
  test12 ();
  test13 ();
  test14 ();
  test15 ();
  test16 ();
  test17 ();
  test18 ();
  test19 ();
  test20 ();
  test21 ();
  test22 ();
  test23 ();
  test24 ();
  test25 ();
  test26 ();
  return 0;
}

#ifndef __OPTIMIZE__
void
link_error ()
{
  abort ();
}
#endif
