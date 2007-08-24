// { dg-do assemble  }
// CELL LOCAL Begin
// -pedantic-errors doesn't like the vector declarations in bits/cmpeqd2.h
// { dg-xfail-if "ISO C++ forbids compound-literals" { "spu-*-*" } "*" "" }
// CELL LOCAL End
// GROUPS passed old-abort
#include <complex>
typedef std::complex<double> Complex;

Complex ComputeVVself()
{
Complex temp1;
Complex self[3][3];

   self[1][2] = 100.0;
   return self[1][2];

}
