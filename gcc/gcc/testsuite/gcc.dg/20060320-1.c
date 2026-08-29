/* { dg-do run { target spu-*-* } } */
/* { dg-options "-O2" } */

void verify (vector unsigned int v) __attribute__ ((__noinline__));
void verify (vector unsigned int v)
{
  if (__builtin_spu_extract (v, 1) == 0)
    abort ();
}

void test (unsigned int x) __attribute__ ((__noinline__));
void test (unsigned int x)
{
  vector unsigned int v = { x, 1, 2, 3 };
  verify (v);
}

int main (void)
{
  test (0);
  return 0;
}

