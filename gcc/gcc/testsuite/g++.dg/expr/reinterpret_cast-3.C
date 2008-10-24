/* { dg-do compile } */
/* Test to make sure we don't error out with a reinterpret_cast from a volatile
   to another volatile type.  */
void r()
{
  volatile short *Foo;
  volatile int** Bar = reinterpret_cast<volatile int**>(&Foo); /* { dg-bogus "qualifiers" } */
}
