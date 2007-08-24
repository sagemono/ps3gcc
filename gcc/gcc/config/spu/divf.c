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

/* This function has been designed to provide a
 * full function operation that presisely computes
 * the quotient for the entire range of extended
 * single precision inputs <a> and <b>. This includes:
 *
 * 1) Computing the quotient to full single precision
 *    floating point accuracy.
 * 2) Round the result consistently with the rounding
 *    mode of the processor - truncated toward zero.
 * 3) Underflow and overflow results are clamped to
 *    Smin and Smax and flagged with the appropriate
 *    UNF or OVF exception in the FPSCR.
 * 4) Divide By Zero (DBZ) exception is produced when
 *    the divisor <b> has a zero exponent. A quotient
 *    of correctly signed Smax is produced.
 * 5) Denorm/zero divided by a denorm/zero generates 
 *    a DBZ with the results undefined.
 * 6) Resulting denorm quotients will be coerced to +0.
 * 7) If a non-compliant IEEE result is produced, the
 *    a DIFF exception is generated.
 */
vector float __divv4sf3(vector float a, vector float b)
{
  vector float inv_b, err, q0, q1, q2;
  vector float mult;
  vector float mant_a, mant_b;
  vector float one = (vector float)(1.0f);
  vector unsigned int exp, exp_a, exp_b, overflow;
  vector unsigned int exp_mask = (vector unsigned int)(0x7F800000);

  /* If b has a zero exponent, then set the divide by zero
   * (DBZ) exception flag. The estimate result is discarded.
   * Note: This must be implemented as inline assembly. Otherwise
   * the optimizer removes it.
   */
  (volatile vector float)si_frest((qword)(b));
  
  /* For computing the quotient, force the divisor and 
   * dividend into the range (1.0 <= 0 < 2.0).
   */
  mant_a = spu_sel(a, one, exp_mask);
  mant_b = spu_sel(b, one, exp_mask);

  /* Compute the quotient using reciprocal estimate
   * followed by one iteration of the Newton-Raphson.
   */
  inv_b = spu_re(mant_b);
  q0 = spu_mul(mant_a, inv_b);
  q1 = spu_nmsub(mant_b, q0, mant_a);
  q1 = spu_madd(inv_b, q1, q0);

  /* Due to truncation error, the quotient result
   * may be low by 1 ulp (unit of least position),
   * Conditionally add one if the estimate is too
   * small.
   */
  q2 = (vector float)spu_add((vector unsigned int)(q1), 1);
  err = spu_nmsub(mant_b, q2, mant_a);
  q2 = spu_sel(q1, q2, spu_cmpgt((vector signed int)err, -1));


  /* Compute the quotient's expected exponent. If the exponent
   * is out of range, then force the resulting exponent to 0.
   * (127 with the bias). We correct for the out of range 
   * values by computing a multiplier (mult) that will force the 
   * result to the correct out of range value and set the 
   * correct exception flag (UNF, OVF, or neither). The multiplier
   * is also conditioned to generate correctly signed Smax if the 
   * divisor b is a denorm or zero.
   */
  exp_a = spu_and((vector unsigned int)a, exp_mask);
  exp_b = spu_and((vector unsigned int)b, exp_mask);
  exp   = spu_add(spu_sub(spu_add(exp_a, (vector unsigned int)one), exp_b), spu_cmpabsgt(mant_b, mant_a));

  /* The default multiplier is 1.0. If an underflow is detected (ie, 
   * either the dividend <a> is a denorm/zero, or the computed exponent is 
   * less than or equal to a biased 0), force the multiplier to 0.0.
   */
  mult = spu_and(one, (vector float)spu_cmpgt((vector signed int)exp, 0));

  /* Force the multiplier to positive Smax (0x7FFFFFFF) and the biased exponent 
   * to 127, if the divisor is denorm/zero or the computed biased exponent is 
   * greater than 255.
   */

  overflow = spu_or(spu_cmpeq(exp_b, 0), spu_cmpeq(spu_rlmask(exp, -30), 2));
  exp = spu_sel(exp, (vector unsigned int)one, overflow);

  mult = spu_or(mult, (vector float)spu_rlmask(overflow, -1));
  mult = spu_andc(mult, (vector float)spu_cmpeq(exp_a, 0));

  /* Insert the exponent into the result and perform the
   * final multiplication.
   */
  q2    = spu_sel(q2, (vector float)exp, exp_mask);
  q2    = spu_mul(q2, mult);

  return (q2);
}
