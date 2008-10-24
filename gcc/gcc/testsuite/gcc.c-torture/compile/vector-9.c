
#define vector __attribute__((vector_size(16) ))

vector float *d;
float f(vector float *a)
{
  a += 1;
  d = a;
  vector float b = *a;
  float d = ((float*)&b)[1];
  return d;
}
