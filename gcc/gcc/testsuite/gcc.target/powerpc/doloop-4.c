/* { dg-do compile } */
/* { dg-options "-O2" } */

int f(int l, int *a)
{
  int i;
  for(i = 0;i < l; i++)
    a[i] = i;
  return l;
}

/* We should be able to do this loop without adding -1 to the l
   to get the number of iterations with this loop still doing a do-loop. */
/* The place where we were getting an extra -1 is when converting from 32bits
   to 64bits as the ctr register is used as 64bits on powerpc64. */
/* { dg-final { scan-assembler-not "-1" } } */
/* { dg-final { scan-assembler "bdnz" } } */
/* { dg-final { scan-assembler-times "mtctr" 1 } } */
