/* { dg-do compile } */
/* { dg-options "-O2" }  */
/* PS3 extension, test that vec_splat accepts a non constant value and
   that the non constant value does not generate a store.  */
#include <altivec.h>

vector float f(vector float a, int b)
{
  return vec_splat (a, b);
}
vector signed int f1(vector signed int a, int b)
{
  return vec_splat (a, b);
}
vector signed char f2(vector signed char a, int b)
{
  return vec_splat (a, b);
}
vector signed short f3(vector signed short a, int b)
{
  return vec_splat (a, b);
}
vector float ff(vector float a, int b)
{
  return vec_splat (a, 100);
}
vector signed int ff1(vector signed int a, int b)
{
  return vec_splat (a, 100);
}
vector signed char ff2(vector signed char a, int b)
{
  return vec_splat (a, 100);
}
vector signed short ff3(vector signed short a, int b)
{
  return vec_splat (a, 100);
}

/* We should get 8 vsplt[bhw], 4 lvsl and no stores. */
/* { dg-final { scan-assembler-times "vsplt" 8 } } */
/* { dg-final { scan-assembler-times "lvsl" 4 } } */
/* { dg-final { scan-assembler-not "stvx" } } */

