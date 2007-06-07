// { dg-do run  }
// { dg-xfail-if "eh not supported on spu" { "spu-*-*" } "*" "" }
// { dg-options "-w" }
// Another magic NULL problem.

#include <stddef.h>

int main()
{
  try
    {
      throw(NULL);
    }
  catch (...)
    {
    }
}
