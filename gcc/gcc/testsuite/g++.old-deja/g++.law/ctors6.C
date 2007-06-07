// { dg-do assemble  }
// CELL LOCAL Begin
// -pedantic-errors doesn't like the vector declarations in bits/cmpeqd2.h
// { dg-xfail-if "ISO C++ forbids compound-literals" { "spu-*-*" } "*" "" }
// CELL LOCAL End
// GROUPS passed constructors
#include <complex>

double foo(std::complex<double> *a)
{
  return 0.0;
}


double bar(void)
{
  std::complex<double> v[10];
  return foo(v);
}
