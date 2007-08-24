// { dg-do run  }
// { dg-xfail-if "256K size limit" { "spu-*-*" } "*" "" }
#include <iostream>
#include <iterator>
#include <string>

std::ostream_iterator<std::string> oo(std::cout);

int main()
{
    *oo = "Hello, ";
    ++oo;
    *oo = "world!\n";
}

