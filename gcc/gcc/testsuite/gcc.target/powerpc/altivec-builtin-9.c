/* { dg-do compile } */
/* { dg-options "-O2" } */
/* Test that for following code only one recorded vector compare instruction is generated */
#include <altivec.h>
vector unsigned int peepholebug(vector unsigned int a, vector unsigned int b)
{
  vector unsigned int mask = (vector unsigned int)vec_vcmpgtuw(a, b);
  if (vec_all_gt(a, b))
    return a;
  return mask;
} /* { dg-final { scan-assembler-times "vcmpgtuw\\." 1 } } */
/* { dg-final { scan-assembler-not "vcmpgtuw " } } */
