#define vector __attribute__((vector_size(16) ))
vector float g(float t)
{
  return (vector float){1.0, 1.0, t, 1.0};
}

