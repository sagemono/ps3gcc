/* { dg-do compile } */
/* { dg-options "-O2" } */
/* Test that for following code only one recorded vector compare instruction is generated */
#include <altivec.h>
vector unsigned short peepholebug(vector unsigned short a, vector unsigned short b)
{
  vector unsigned short mask = (vector unsigned short)vec_vcmpequh(a, b);
  if (vec_all_eq(a, b))
    return a;
  return mask;
} /* { dg-final { scan-assembler-times "vcmpequh\\." 1 } } */
/* { dg-final { scan-assembler-not "vcmpequh " } } */

