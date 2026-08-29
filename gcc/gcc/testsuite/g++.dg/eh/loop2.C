// Test that breaking out of a handler works.
// { dg-do run }
/* { dg-xfail-if "eh not supported on spu" { "spu-*-*" } "*" "" } */

int main ()
{
  while (1)
    {
      try { throw 1; }
      catch (...) { break; }
    }
}
