/* { dg-do link } */
/* { dg-options { -W } } */
/* Test PPU intrinsics from <ppu_intrinsics.h>.  */

#ifdef __PPU__
#include <ppu_intrinsics.h>

int main ()
{
  double d, d1, d2;
  float f, f1, f2;
  double dstore;
  unsigned long long ull, a, b;
  long long ll;
  int i;

  ull = __rldcl (a, b, 3);
  ull = __rldcr (a, b, 3);
  ull = __rldic (a, 3, 4);
  ull = __rldicl (a, 4, 5);
  ull = __rldicr (a, 2, 3);
  ull = __rldimi (a, b, 4, 6);
  ull = __rlwimi (a, b, 6, 9, 12);
  ull = __rlwnm (a, b, 3, 5);
  d = __fmul (d1, d2);
  f = __fmuls (f1, f2);
  f = __frsp (f);
  d = __fcfid (ll);
  d = __frsqrte (d1);
  ll = __fctid (d);
  ll = __fctidz (d);
  i = __fctiw (d);
  i = __fctiwz (d);

  return 0;
}

#endif
