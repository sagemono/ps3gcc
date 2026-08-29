/* { dg-do compile } */
/* { dg-options "-O2" } */
#include <altivec.h>
vector float f(float *b)
{
  vector float a0 = (vector float)vec_lvewx (0, (int*)b);
  vector float a1 = (vector float)vec_lvewx (0, (int*)b);
  vector float a2 = (vector float)vec_lvewx (0, (int*)b);
  return a0 + a1 + a2;
} /* { dg-final { scan-assembler-times "lvewx " 1 } } */
