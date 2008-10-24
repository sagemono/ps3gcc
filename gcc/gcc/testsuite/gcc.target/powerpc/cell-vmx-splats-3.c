/* { dg-do compile } */
/* { dg-options "-maltivec -O2 -mcpu=cell" } */
#include <altivec.h>

vector unsigned char g(void)
{
  return vec_splats((unsigned char)54);
}

/* For vec_splats of a constant we should not store out the const before 
   loading it into the vector register for the splat.  */
/* { dg-final { scan-assembler-not "stb" } } */
/* { dg-final { scan-assembler "lvlx" } } */
