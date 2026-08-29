// { dg-do run  }
// { dg-xfail-if "256K size limit" { "spu-*-*" } "*" "" }
// { dg-options "-O2" }

#include <iostream>

std::ostream& foo (char *x, std::ostream &y)
{
  return y << "" << x;
}

int main ()
{
  foo ("", std::cout);
}
