/* PR tree-optimization/33619 */
/* { dg-do run { target ppu-*-* } } */
/* { dg-options "-O2" } */

#ifdef __powerpc__
# define REG1 "3"
# define REG2 "4"
#elif defined __x86_64__
# define REG1 "rdi"
# define REG2 "rsi"
#endif

static inline void
bar (unsigned long x, int y)
{
  register unsigned long p1 __asm__ (REG1) = x;
  register unsigned long p2 __asm__ (REG2) = y;
  __asm__ volatile ("" : "=r" (p1), "=r" (p2) : "0" (p1), "1" (p2) : "memory");
  if (p1 != 0xdeadUL || p2 != 0xbefUL)
    __builtin_abort ();
}

__attribute__((const, noinline)) int
baz (int x)
{
  return x;
}

__attribute__((noinline)) void
foo (unsigned long *x, int y)
{
  unsigned long a = *x;
  bar (a, baz (y));
}

int
main (void)
{
  unsigned long a = 0xdeadUL;
  foo (&a, 0xbefUL);
  return 0;
}
