/* { dg-do run } */
/* { dg-options "-O2" }  */
/* PS3 extension, test that vec_splat accepts a non constant value and
   that the non constant value does not generate a store and actually works.  */
#include <altivec.h>

vector float f(vector float a, int b) __attribute__((noinline));
vector float f(vector float a, int b)
{
  return vec_splat (a, b);
}
vector signed int f1(vector signed int a, int b) __attribute__((noinline));
vector signed int f1(vector signed int a, int b)
{
  return vec_splat (a, b);
}
vector signed char f2(vector signed char a, int b) __attribute__((noinline));
vector signed char f2(vector signed char a, int b)
{
  return vec_splat (a, b);
}
vector signed short f3(vector signed short a, int b) __attribute__((noinline));
vector signed short f3(vector signed short a, int b)
{
  return vec_splat (a, b);
}

vector signed char vsc = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
vector signed short vss = {0, 1, 2, 3, 4, 5, 6, 7};
vector signed int vsi = {0, 1, 2, 3};
vector float vf = {0, 1, 2, 3};

int main(void)
{
  int i;
  for(i = 0;i < 16;i++)
    {
      vector signed char a = f2(vsc, i);
      if (a != vec_splats ((signed char)i))
        __builtin_abort ();
    }
  for(i = 0;i < 8;i++)
    {
      vector signed short a = f3(vss, i);
      if (a != vec_splats ((signed short)i))
        __builtin_abort ();
    }
  for(i = 0;i < 4;i++)
    {
      vector signed int a = f1(vsi, i);
      if (a != vec_splats ((signed int)i))
        __builtin_abort ();
    }
  for(i = 0;i < 4;i++)
    {
      vector float a = f(vf, i);
      if (a != vec_splats ((float)i))
        __builtin_abort ();
    }
  return 0;
}

