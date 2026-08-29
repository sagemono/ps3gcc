/* { dg-do compile } */
/* { dg-options "-O2" } */
#include <altivec.h>

vector float f(float a)
{
  vector float d = vec_lvlx (0, &a);
  return vec_splat (d, 0);
}

/* This should be able to optimize to just lvlx or a lvewx with a splat. */
/* { dg-final { scan-assembler "lvlx|lvewx" } }  */
/* { dg-final { scan-assembler-times "vspltw" "1" } }  */
