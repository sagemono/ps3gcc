/* { dg-xfail-if "eh not supported on spu" { "spu-*-*" } "*" "" } */

#include "nrv1.h"

A f()
{
  A nrv;
  throw 42;
  return nrv;
}
