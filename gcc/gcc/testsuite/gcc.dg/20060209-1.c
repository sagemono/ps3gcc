
/* { dg-do run { target spu-*-* } } */
/* { dg-options "-O2 -g" } */

typedef signed char qword __attribute__((__vector_size__(16)));
volatile qword x;

char data[16];

void test (void) __attribute__((__noinline__));
void test (void)
{
  int i;

  x = __builtin_si_from_ptr (data);

  for (i = 0; i < 1; i++)
    x = __builtin_si_from_ptr (data);
}

int main (void)
{
  test ();

  exit (data != (void *)__builtin_si_to_ptr (x));
}

