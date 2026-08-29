/* { dg-do compile } */
/* { dg-options "-O2 -mpowerpc64" } */

double f(unsigned char *a)
{
  return *a;
}

/* There should be only one store and no sign extend or zero extend as
   it is already zero extended by the load.  */
/* { dg-final { scan-assembler-times "std" 1 } } */
/* { dg-final { scan-assembler-times "exts" 0 } } */
/* { dg-final { scan-assembler-times "rldicl" 0 } } */

