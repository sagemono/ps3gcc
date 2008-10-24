/* { dg-do compile } */
/* { dg-options "-O2" } */
/* Test that for following code only one recorded vector compare instruction is generated */
#include <altivec.h>
vector signed short peepholebug(vector signed short a, vector signed short b)
{
  vector signed short mask = (vector signed short)vec_vcmpgtsh(a, b);
  if (vec_all_gt(a, b))
    return a;
  return mask;
} /* { dg-final { scan-assembler-times "vcmpgtsh\\." 1 } } */
/* { dg-final { scan-assembler-not "vcmpgtsh " } } */
