/* { dg-do compile } */
/* { dg-options "-O2" } */
/* Test that for following code only one recorded vector compare instruction is generated */
#include <altivec.h>
vector int peepholebug(vector int a, vector int b)
{
  vector int mask = (vector int)vec_vcmpgtsw(a, b);
  if (vec_all_gt(a, b))
    return a;
  return mask;
} /* { dg-final { scan-assembler-times "vcmpgtsw\\." 1 } } */
/* { dg-final { scan-assembler-not "vcmpgtsw " } } */
