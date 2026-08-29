/* { dg-do link } */
/* { dg-options "-W -O2 -Wall" } */
/* Test PPU intrinsics from <ppu_intrinsics.h>.  */

#ifdef __PPU__
#include <ppu_intrinsics.h>

void f(float a);
int main(void)
{
  f(1);
  return 0;
}

void f(float a)
{
  __dcbf (&a);
  __dcbz (&a);
  __dcbst (&a);
  __icbi (&a);
}

#endif
