#include <string.h>
#include <stdlib.h>

float t(void)
{
  float a = -0.0;
  float b = 0.0;
  return a*b;
}

int main(void)
{
  float x = t();
  float x1 = 0.0;
  if (memcmp (&x, &x1, sizeof(float)))
    abort ();
}
