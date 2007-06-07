// { dg-do run  }
// { dg-xfail-if "256K size limit" { "spu-*-*" } "*" "" }
// { dg-options "-O" }

#include <iostream>
#include <typeinfo>

int main() {
  int *i1, *i2;
  std::cerr << (typeid(i1)==typeid(i2)) << std::endl;
}
