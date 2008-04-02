#define vector __attribute__((vector_size(16) ))

vector float a(float *b)
{
  float c = b[1];
  return (vector float){c,c,c,c};
}

vector float b(float *b, float *e)
{
  float c = b[2];
  float d = e[2];
  return (vector float){c,c,c,c} + (vector float){d,d,d,d};
}
