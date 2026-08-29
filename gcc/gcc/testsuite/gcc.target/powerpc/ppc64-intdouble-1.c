/* { dg-do compile } */
/* { dg-options "-O2 -mpowerpc64" } */

double f(signed char *a)
{
  return *a;
}

/* There should be only one store and one sign extend.  */
/* { dg-final { scan-assembler-times "std" 1 } } */
/* { dg-final { scan-assembler-times "exts" 1 } } */

