/* (C) Copyright
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

/* As a special exception, if you link this library with files compiled with
   GCC to produce an executable, this does not cause the resulting executable
   to be covered by the GNU General Public License.  The exception does not
   however invalidate any other reasons why the executable file might be covered
   by the GNU General Public License. */

#include <spu_intrinsics.h>

extern vector float __sqrtv4sf2 (vector float);

#if 1
/* Single precision accurate version.  */
vec_float4 __sqrtv4sf2(vec_float4 in)
{
  vec_uint4 exp, valid;
  vec_uint4 mask = (vec_uint4)(0xFF000000);
  vec_uint4 half = (vec_uint4)(0x00800000);
  vec_float4 one = (vec_float4)(1.0f);
  vec_float4 three = (vec_float4)(3.0f);
  vec_float4 onep = (vec_float4)(1.00000011920928955078125f);
  vec_float4 x, y0, y1, y1_n1, y1_p1, y1_p2, y1_p3;
  vec_float4 mant, err, err_p1, err_p2, err_p3;
  vec_float4 out;
  vec_float4 y0x, y0xhalf;

  /* Compute the mantissa of the result seperately from 
   * the exponent to assure complete accuracy over the allowable
   * input range. The mantissa is computed for inputs in the 
   * range [1.0, 4.0).
   */
  x = spu_sel(in, one, mask);
  y0 = spu_rsqrte(x);
  
  /* Perform one iteration of the Newton-Raphsom method in single precision
   * arithmetic.
   */
#if 1
  y1 = spu_mul(spu_nmsub(x, spu_mul(y0, y0), three), 
	       spu_mul(y0, (vec_float4)(spu_sub((vec_uint4)(x), half))));
#else
  y0x = spu_mul (y0, x);
  y0xhalf = spu_mul (y0x, spu_splats(0.5f));
  y1 = spu_madd (spu_nmsub (y0, y0x, onep), y0xhalf, y0x);
#endif

  /* Correct the result for possible error. The range of error is -3 to +1.
   * Identify the extent of the error and correct for it.
   */
  y1_p3 = (vec_float4)spu_add((vec_uint4)(y1), 3);
  y1_p2 = (vec_float4)spu_add((vec_uint4)(y1), 2);
  y1_p1 = (vec_float4)spu_add((vec_uint4)(y1), 1);
  y1_n1 = (vec_float4)spu_add((vec_uint4)(y1), -1);

  err    = spu_nmsub(y1,    y1,    x);
  err_p1 = spu_nmsub(y1_p1, y1_p1, x);
  err_p2 = spu_nmsub(y1_p2, y1_p2, x);
  err_p3 = spu_nmsub(y1_p3, y1_p3, x);

  mant = spu_sel(y1_n1, y1,    spu_cmpgt((vec_int4)(err),    -1));
  mant = spu_sel(mant,  y1_p1, spu_cmpgt((vec_int4)(err_p1), -1));
  mant = spu_sel(mant,  y1_p2, spu_cmpgt((vec_int4)(err_p2), -1));
  mant = spu_sel(mant,  y1_p3, spu_cmpgt((vec_int4)(err_p3), -1));

  /* Compute the expected exponent. If the exponent is zero or the input is
   * negative, then set the result to zero.
   */
  exp = spu_rlmask(spu_add((vec_uint4)(in), (vec_uint4)(one)), -1);

  valid = spu_cmpgt(spu_and((vec_int4)(in), (vec_int4)(mask)), 0);
    
  /* Merge the computed exponent and mantissa.
   */
  out = spu_and(spu_sel(mant, (vec_float4)(exp), (vec_uint4)(0xFF800000)), (vec_float4)(valid));

  return (out);
}
#endif

#if 0
/* Single precision fast version with compensation for identify results.
 *
 * Histogram of results over the range 1.0 to 3.9999999.
 *  3      0
 * -2      68
 * -1      384895
 *  0      5985155
 *  1      8611186
 *  2      1752588
 *  3      43324
 */
vec_float4 sqrtf4(vec_float4 in)
{
  vec_float4 y0, out;
  
  /* Perform one iteration of the Newton-Raphsom method in single precision
   * arithmetic.
   */
  y0 = spu_rsqrte(in);
  out = spu_mul(spu_nmsub(in, spu_mul(y0, y0), (vec_float4)((vec_uint4)(0x40400001))), 
		spu_mul(y0, spu_mul(in, (vec_float4)(0.5f))));

  return (out);
}



#include <stdio.h>
int main()
{
  printf("2 -> %20.16e\n", spu_extract(sqrtf4((vec_float4)(2.0)), 0));
  printf("4 -> %20.16e\n", spu_extract(sqrtf4((vec_float4)(4.0)), 0));
  printf("6 -> %20.16e\n", spu_extract(sqrtf4((vec_float4)(6.0)), 0));
  printf("3 -> %20.16e\n", spu_extract(sqrtf4((vec_float4)(3.0)), 0));
  printf("1 -> %20.16e\n", spu_extract(sqrtf4((vec_float4)(1.0)), 0));
  printf("0 -> %20.16e\n", spu_extract(sqrtf4((vec_float4)(0.0)), 0));
}
#endif

