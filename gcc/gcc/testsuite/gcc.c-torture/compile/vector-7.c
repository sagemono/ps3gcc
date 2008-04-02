#define vector __attribute__((vector_size(16) ))

vector int f(int a, int b)
{
  int g[100000]={};
  g[a] = b;
  return (vector int){a,a,a,a} + (vector int){g[b], g[b], g[b], g[b]};
}


