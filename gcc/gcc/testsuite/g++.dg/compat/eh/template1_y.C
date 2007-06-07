/* { dg-xfail-if "eh not supported on spu" { "spu-*-*" } "*" "" } */

#include "template1.h"

template<class T> void C<T>::f (void) throw (E)
{
  throw E();
}

template class C<int>;
