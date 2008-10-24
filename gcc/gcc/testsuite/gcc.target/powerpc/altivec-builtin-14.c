/* { dg-do compile } */
/* { dg-options "-O2" } */
/* Test that for following code only one recorded vector compare instruction is generated */
#include <altivec.h>
vector char peepholebug(vector char a, vector char b)
{
  vector char mask = (vector char)vec_vcmpgtsb(a, b);
  if (vec_all_gt(a, b))
    return a;
  return mask;
} /* { dg-final { scan-assembler-times "vcmpgtsb\\." 1 } } */
/* { dg-final { scan-assembler-not "vcmpgtsb " } } */
