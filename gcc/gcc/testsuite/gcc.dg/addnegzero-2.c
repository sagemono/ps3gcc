/* { dg-do compile { target { powerpc-*-* ppu-*-* } } } */
/* { dg-options "-O1 -fno-tree-fre -fno-tree-pre -fno-tree-dominator-opts " } */


/* Test to make sure that we can remove the addition dealing with a + -0.0
   and constants are foldded back on the RTL level. */

double f;
double g(double a, int i)
{
  f = -0.0;
  f = a + f;
}

/* { dg-final { scan-assembler-not "fadd" } } */

