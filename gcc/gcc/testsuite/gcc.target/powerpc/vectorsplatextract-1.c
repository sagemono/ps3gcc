/* { dg-do compile } */
/* { dg-options "-O2 -maltivec" } */
#include <altivec.h>

float f(vector float a)
{
  vector float b = vec_splat (a, 1);
  return b[0];
}
int fsi(vector signed int a)
{
  vector signed int b = vec_splat (a, 1);
  return b[0];
}
int fui(vector unsigned int a)
{
  vector unsigned int b = vec_splat (a, 1);
  return b[0];
}
int fss(vector signed short a)
{
  vector signed short b = vec_splat (a, 1);
  return b[0];
}
int fus(vector unsigned short a)
{
  vector unsigned short b = vec_splat (a, 1);
  return b[0];
}
int fsc(vector signed char a)
{
  vector signed char b = vec_splat (a, 1);
  return b[0];
}
int fuc(vector unsigned char a)
{
  vector unsigned char b = vec_splat (a, 1);
  return b[0];
}

/* The splat followed by an extraction should be removed as it is not needed
   as we can extract directly from that element of the vector. */
/* { dg-final { scan-assembler-not "vsplt" } } */
