// { dg-do assemble  }
// CELL LOCAL Begin
// -pedantic-errors doesn't like the vector declarations in bits/cmpeqd2.h
// { dg-xfail-if "ISO C++ forbids compound-literals" { "spu-*-*" } "*" "" }
// CELL LOCAL End
#include <complex>
template<class T>
class Vec {
public:
    Vec() { data = new T; }
    Vec<T> split() { Vec<T> tmp; operator=(tmp); return tmp; }
    void operator=(const Vec<T> &v) { data = new T; }
    T *data;
};
template class Vec<std::complex<double> >;
