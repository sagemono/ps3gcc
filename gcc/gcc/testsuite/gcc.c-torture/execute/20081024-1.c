#include <stddef.h>
#include <string.h>

/* Test that using signed char in pointer arthamtic works correctly. */

int *f(int *a, signed char *c)__attribute__((noinline));
int *f(int *a, signed char *c)
{
  return a + *c;
}

int *f1(int *a, signed char *c)__attribute__((noinline));
int *f1(int *a, signed char *c)
{
  int b = *c;
  return a + b;
}

ptrdiff_t g(int *a, signed char c)__attribute__((noinline));
ptrdiff_t g(int *a, signed char c)
{
  return (f(a, &c) - a);
}


ptrdiff_t g1(int *a, signed char c)__attribute__((noinline));
ptrdiff_t g1(int *a, signed char c)
{
  return (f1(a, &c) - a);
}

void h1(int *a, signed char *c)__attribute__((noinline));
void h1(int *a, signed char *c)
{
  int d = *c;
  a[d] = 1;
}


void h(int *a, signed char *c)__attribute__((noinline));
void h(int *a, signed char *c)
{
  a[*c] = 1;
}
int main(void)
{
#define c (signed char){-23}

  int a[256*2];
  /* Set b to half way point of a so we have wiggle room.   */
  int *b = &a[256];

  /* Test that adding add c to b and then subtracting b gives back c.   */
  if (g(b, c) != c)
    __builtin_abort ();

  /* Likewise but this time cast the signed char to an int before adding it.  */
  if (g1(b, c) != c)
    __builtin_abort ();

  memset (a, 0, 256*2*sizeof(int));

  /* Make sure that the memset works, semi unrelated to the testcase. */
  if (a[(256-23)] != 0)
    __builtin_abort ();

  /* Set b[c] to 1 and make sure that we get the correct element set. */
  h(b, &c);
  if (a[(256-23)] != 1)
    __builtin_abort ();

  memset (a, 0, 256*2*sizeof(int));
  /* Make sure that the memset works, semi unrelated to the testcase. */
  if (a[(256-23)] != 0)
    __builtin_abort ();

  /* Set b[(int)c] to 1 and make sure that we get the correct element set. */
  h1(b, &c);
  if (a[(256-23)] != 1)
    __builtin_abort ();


  return 0;
}


