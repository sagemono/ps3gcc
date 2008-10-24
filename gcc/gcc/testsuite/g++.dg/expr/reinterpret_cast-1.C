/* { dg-do compile } */
/* Test that error message mentions qualifiers when volatile
   is cast away using reinterpret_cast. */
void r()
{
 volatile short Foo=1;
 int* Bar = reinterpret_cast<int*>(&Foo); /* { dg-error "qualifiers" } */
} 




