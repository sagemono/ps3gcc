/* { dg-do compile } */
/* { dg-options "-O2 -fpic" } */
double foo (double x) {
  return x + 1.75;
}
/* { dg-final { scan-assembler "lfs" } } */
