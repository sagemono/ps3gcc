// { dg-do compile }
// { dg-options "" }

// Test that the CONSTRUCTOR is considered value dependent.

#define vector __attribute__((vector_size(16) ))

template<unsigned int X>
vector float fSegfault(vector float v)
{
  vector unsigned char perm = (vector unsigned char)
  { X*4, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
}
