/* { dg-do compile } */
/* { dg-options "-mpowerpc-gfxopt" } */
/* { dg-final { scan-assembler "stfiwx" } } */

int foo (double x)
{
  return x;
}
