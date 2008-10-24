/* { dg-do compile } */
/* { dg-options "-maltivec -O2 -mcpu=cell" } */
#include <altivec.h>

vector short g(void)
{
  return vec_splats((short)129);
}

/* For vec_splats of a constant we should not store out the const before 
   loading it into the vector register for the splat.  */
/* { dg-final { scan-assembler-not "sth" } } */
/* { dg-final { scan-assembler "lvlx"  } } */
