/* { dg-do compile } */
/* Test to make sure error message mentions qualifiers when const
   is cast away using static_cast. */
class a{};

void sc ()
{
  a b1;
  const a *c = &b1;
  const a *c1 = static_cast<a*>(c); /* { dg-error "casts away qualifiers" } */
}
