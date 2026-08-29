/* { dg-do compile } */
// Test that error message mentions qualifiers when casting away volatile
// using reinterpret_cast.
void r()
{
  volatile short *Foo;
  int** Bar = reinterpret_cast<int**>(&Foo); /* { dg-error "casts away qualifiers" } */
}

