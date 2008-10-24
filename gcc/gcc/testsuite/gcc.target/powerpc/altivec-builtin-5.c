/* { dg-do compile } */
/* { dg-options "-O2" } */
/* Test that for following code only one recorded vector compare instruction is generated */
#include <altivec.h>
vector float peepholebug(vector float a, vector float b)
{
  vector float mask = (vector float)vec_vcmpgefp(a, b);
  if (vec_all_ge(a, b))
    return a;
  return mask;
} /* { dg-final { scan-assembler-times "vcmpgefp\\." 1 } } */
/* { dg-final { scan-assembler-not "vcmpgefp " } } */

