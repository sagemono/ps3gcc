/* { dg-xfail-if "eh not supported on spu" { "spu-*-*" } "*" "" } */

struct One { };
struct Two { };

void
handle_unexpected ()
{
  try
  {
    throw;
  }
  catch (One &)
  {
    throw Two ();
  }
}

void
doit () throw (Two)
{
  throw One ();
}
