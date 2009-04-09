/* This used to ICE in some cases. */
int g(void);
int h(void);
unsigned long long f(unsigned long long a, unsigned long long b)
{
  unsigned int c = -2;
  if (a & c)
  return g();
  else return h();
}
