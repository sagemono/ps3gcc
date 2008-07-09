// { dg-do compile { target ppu-*-* } }
// { dg-options "-O0" }

#include <altivec.h>

typedef vector bool int d;
void f(d a, const vector unsigned int b)
{
  a = b; // { dg-error "__vector __bool int" }
}
