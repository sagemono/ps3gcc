/* { dg-do compile } */
/* { dg-options "-O2" } */
/* Test that for following code only one recorded vector compare instruction is generated */
#include <altivec.h>
vector unsigned char peepholebug(vector unsigned char a, vector unsigned char b)
{
  vector unsigned char mask = (vector unsigned char)vec_vcmpequb(a, b);
  if (vec_all_eq(a, b))
    return a;
  return mask;
} /* { dg-final { scan-assembler-times "vcmpequb\\." 1 } } */
/* { dg-final { scan-assembler-not "vcmpequb " } } */
