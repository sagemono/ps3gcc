// { dg-do run  }
// { dg-xfail-if "256K size limit" { "spu-*-*" } "*" "" }
#include <cassert>
#include <iostream>

int bar ()
{
  throw 100;
}

int main ()
{
  int i = 0;
  try
    {
      i = bar ();
    }
  catch (...)
    {
    }

//  std::cout << "i = " << i << std::endl;
  assert (i == 0) ; 
}




