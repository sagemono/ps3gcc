/* { dg-do compile } */
/* { dg-options "-maltivec -O2 -mcpu=cell" } */
#include <altivec.h>

vector float g(void)
{
  return vec_splats(1.0f);
}

/* The function h should produce a vxor (which is it does now).  */
vector float h(void)
{
  return vec_splats(0.0f);
}

vector int g1(void)
{
  return vec_splats(1289);
}

/* For vec_splats of a constant we should not store out the const before 
   loading it into the vector register for the splat.  */
/* { dg-final { scan-assembler-not "stfs" } } */
/* { dg-final { scan-assembler-not "stw" } } */
/* { dg-final { scan-assembler-not "lvewx" } } */
/* { dg-final { scan-assembler "lvlx" } } */
/* { dg-final { scan-assembler "vxor" } } */
