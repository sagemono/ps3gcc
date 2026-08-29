/* { dg-do compile } */
/* { dg-options "-O2" } */
#include <altivec.h>

vector float f(float a)
{
  return vec_lvlx (0, &a);
}

/* This should be able to optimize to just lvlx or a lvewx but currently
   we get a splat also. */
/* { dg-final { scan-assembler "lvlx|lvewx" } }  */
/* { dg-final { scan-assembler-not "vspltw" { xfail *-*-* } } }  */
