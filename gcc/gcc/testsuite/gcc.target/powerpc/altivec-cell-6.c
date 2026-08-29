/* { dg-do compile } */
/* { dg-options "-maltivec" } */

#include <altivec.h>
vector float f(void)
{
  vector float * a = (void*)16;
  return vec_lvlx (0, a);
}


