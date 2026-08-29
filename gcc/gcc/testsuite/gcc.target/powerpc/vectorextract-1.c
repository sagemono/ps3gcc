/* { dg-do compile } */
/* { dg-options "-O2 -maltivec" } */

/* We used to ICE when using vec_extract with a mem that
   has an offset and we need to add another offset to it.  */
#include <altivec.h>

float f(vector float *a)
{
  return vec_extract (a[1], 1);
}

/* We should be able not emit a stvewx here as we are just doing a normal float
   load.  */
/* { dg-final { scan-assembler-not "stvewx" } } */
/* { dg-final { scan-assembler "lfs" } } */
