/* { dg-do compile } */
/* { dg-options "-W -Wall" } */

#define vector __attribute__((vector_size(16) ))


float vf(void)
{
  register vector float a = (vector float){0, 1, 2, 3};
  return a[0]; /* { dg-bogus "register" } */
}

