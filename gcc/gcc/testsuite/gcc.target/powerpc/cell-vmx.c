/* { dg-do compile } */
/* { dg-options "-mcpu=cell -O2" } */
#include <altivec.h>

vector float a(float *b)
{
  return (vector float){*b, *b, *b, *b};
}

vector float a1(float *b)
{
  return vec_splats(*b);
}

vector float a2 (float *b)
{
  return vec_promote(*b, 1);
}

/* For both vector initialization and vec_splats, GCC should output lvlx
   followed by a vspltw.  This is because lvlx will put the correct address
   in the first element of the vector.   We should not have any vperm as we
   don't need to move around the element.  */
/*{ dg-final { scan-assembler "lvlx" } }  */
/*{ dg-final { scan-assembler "vspltw" } }  */
/*{ dg-final { scan-assembler-not "vperm" } }  */
