/* { dg-do compile } */
/* { dg-options "" } */

#define vector __attribute__((vector_size(16) ))

float vf(vector float a)
{
  return 0[a]; /* { dg-error "" } */
}


float fv(vector float a)
{
  return a[0];
}
