/* { dg-do compile } */
/* { dg-options "-O2" } */
int g(void);
int h(void);

unsigned long long f(unsigned long long a, unsigned long long b)
{
  unsigned int c = -2;
  if (a & c)
  return a;
  else return b;
}

/* The above and can be done with rlwinm so we
   should not load up the constant (-2) from memory.  */
/* { dg-final { scan-assembler-not "ld|lwz " } } */
