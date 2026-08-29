/* { dg-do compile } */
/* { dg-options "-O2" } */
#include <altivec.h>

vector float f(vector float a)
{
  float s = a[0];
  vector float d = vec_lvlx (0, &s);
  return vec_splat (d, 0);
}

/* This should be able to optimize to just vspltw as it is just a splat of element 0. */
/* { dg-final { scan-assembler-not "lvlx" } }  */
/* { dg-final { scan-assembler-not "stvewx" } }  */
/* { dg-final { scan-assembler-not "lfsx" } }  */
/* { dg-final { scan-assembler-not "stfs" } }  */
/* { dg-final { scan-assembler "vspltw" } }  */
