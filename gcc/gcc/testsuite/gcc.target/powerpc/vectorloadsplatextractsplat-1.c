/* { dg-do compile } */
/* { dg-options "-O2 -maltivec" } */
#include <altivec.h>

vector float f(vector float *a)
{
  vector float b = *a;
  vector float c = vec_splat (b, 1);
  float d = *(float*)&c;
  vector float e = {d, d, d, d};
  return e;
}

vector signed char f1(vector signed char *a)
{
  vector signed char b = *a;
  vector signed char c = vec_splat (b, 1);
  signed char d = *(signed char*)&c;
  vector signed char e = {d, d, d, d, d, d, d, d, d, d, d, d, d, d, d, d};
  return e;
}

vector signed short f2(vector signed short *a)
{
  vector signed short b = *a;
  vector signed short c = vec_splat (b, 1);
  signed short d = *(signed short*)&c;
  vector signed short e = {d, d, d, d, d, d, d, d};
  return e;
}


vector signed int f3(vector signed int *a)
{
  vector signed int b = *a;
  vector signed int c = vec_splat (b, 1);
  signed int d = *(signed int*)&c;
  vector signed int e = {d, d, d, d};
  return e;
}

/* This be optimized to no stores or vector element loads.
   4 vector splats and exactly 4 vector loads.  */
/* { dg-final { scan-assembler-not "st" } } */
/* { dg-final { scan-assembler-not "lve" } } */
/* { dg-final { scan-assembler-times "vsplt" 4 } } */
/* { dg-final { scan-assembler-times "lvx" 4 } } */
