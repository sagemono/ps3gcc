/* { dg-do compile } */
/* { dg-options "-WnonPOD-varargs" } */
/* Test that warning is emitted since a nonPOD is being passed to a
 * variable arguments function */
#include <stdarg.h>

void Write( const char* msg, const char* msg2, ...){}

struct String
{
  String(const char*);
  private:
  int i;
};

int main()
{
String str("World");
Write("Hello","Debug out %s" ,str); /* { dg-warning "cannot pass objects of" } */
return 0;
}
