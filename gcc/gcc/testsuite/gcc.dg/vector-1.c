/* { dg-do compile } */
/* { dg-options "" } */

#define vector __attribute__((vector_size(16) ))

float vf(vector float a)
{
  return 0[a]; /* { dg-error "subscripted value is not an array, a pointer, or a vector" } */
}


float fv(vector float a)
{
  return a[0];
}
