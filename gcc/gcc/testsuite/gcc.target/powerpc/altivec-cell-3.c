/* { dg-do run } */
/* { dg-options "-maltivec" } */
/* Test the vec_splats and vec_promote VMX intrinsics.  */
#include <altivec.h>
#include "altivec_check.h"

extern void abort (void);

vector int a = {0, 0, 0, 0};
int main1(int t)
{
  int i;
  vector int b = vec_splats(0);
  if (a != b)
    abort ();

  b = vec_splats(t);
  if (a != b)
    abort ();

  b = vec_promote(0, 1);
  if (vec_extract (b, 1) != 0)
    abort ();

  b = vec_promote(t, t);
  if (vec_extract (b, t) != 0)
    abort ();

  return 0;
}

int main(void)
{
  altivec_check();  /* Exits if AltiVec not supported */
  return main1 (0);
}
