/* { dg-do compile } */
/* { dg-options "-maltivec -O2 -mcpu=cell" } */
#include <altivec.h>

/* FIXME: this does not work currently because we don't produce a constant
   for VIEW_CONVERT_EXPR<float, int_cst(0)>.  This is done in 4.3.0 though.   */
vector float t(void)
{
  int t1 = 1;
  float t2 = *(float*)&t1;
  return vec_splats(t2);
}

/* For vec_splats of a constant we should not store out the const before 
   loading it into the vector register for the splat.  */
/* { dg-final { scan-assembler-not "stfs" { xfail *-*-* } } } */
/* { dg-final { scan-assembler-not "lfs" { xfail *-*-* } } } */
/* { dg-final { scan-assembler-not "stw" { xfail *-*-* } } } */
/* { dg-final { scan-assembler-not "lvewx" { xfail *-*-* } } } */
/* { dg-final { scan-assembler "lvlx" { xfail *-*-* } } } */
