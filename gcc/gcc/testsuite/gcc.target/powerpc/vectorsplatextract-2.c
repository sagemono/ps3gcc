/* { dg-do compile } */
/* { dg-options "-O2 -maltivec" } */
/* This tests two things, first that the splat goes away and then second
   that we don't have a LHS with the extraction later on. */

#include <altivec.h>
float f(vector float *a)
{
  vector float b = vec_splat (a[0], 1);
  return b[0];
}

float f1(vector float *a)
{
  return a[0][1];
}
/* We should be able not emit a stvewx here as we are just doing a normal float
   load.  */
/* { dg-final { scan-assembler-not "stvewx" } } */
/* { dg-final { scan-assembler "lfs" } } */
