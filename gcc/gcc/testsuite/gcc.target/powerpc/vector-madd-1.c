/* { dg-do compile } */
/* { dg-options "-O2 -maltivec -ffast-math" } */
#include <altivec.h>

#define vector __vector

vector float f(vector float a, vector float b, vector float c)
{
  return a * b + c;
}


vector float f1(vector float a, vector float b, vector float c)
{
  vector float d = (vector float){0.0f, 0.0f, 0.0f, 0.0f};
  return vec_add (vec_madd (a, b, d), c);
}

/* With a * b + c and vec_add/vec_madd with 0, we should produce only one vmaddfp and no vaddfp
   as we should combine the multiple and addition together.  */
/* { dg-final { scan-assembler "vmaddfp" } } */
/* { dg-final { scan-assembler-not "vaddfp" } } */
