/* { dg-require-effective-target vect_int } */

#include <stdarg.h>
#include "tree-vect.h"

#define N 64

#define DOT 43680

unsigned char X[N] __attribute__ ((__aligned__(16)));
unsigned char Y[N] __attribute__ ((__aligned__(16)));

/* char->int->int dot product. 
   Not detected as a dot-product.
   Also not detected as widening multiplication.
   Only way this can currently be vectorized is using type promotion
   (vector unpacking) nut this is currently supported only when unpakcing
   to a type excetly twice as wide as the input (e.g. char->short).
   Currenty doesn't get vectorized due to presence of type conversions.  */
unsigned int
foo (int len) {
  int i;
  unsigned int result = 0;

  for (i=0; i<len; i++) {
    result += (X[i] * Y[i]);
  }
  return result;
}

int main (void)
{
  unsigned int dot;
  int i;

  check_vect ();

  for (i=0; i<N; i++) {
    X[i] = i;
    Y[i] = 64-i;
  }

  dot = foo (N);
  if (dot != DOT)
    abort ();

  return 0;
}

/* { dg-final { scan-tree-dump-times "vectorized 1 loops" 1 "vect" { xfail *-*-* } } } */

/* { dg-final { cleanup-tree-dump "vect" } } */

