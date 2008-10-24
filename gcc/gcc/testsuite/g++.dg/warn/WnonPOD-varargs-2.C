#include <stdarg.h>
/* { dg-do compile } */
/* { dg-options "-Werror=-WnonPOD-varargs" } */
/* Test to check that error is emitted when passing nonPOD to varargs
 * with the option to convert warning to error enabled */
class String {
public:
  void SetData(char *NewData) { m_Data = NewData; }
private:
  char *m_Data;
};

int Bar(char *s, va_list ArgList){}


int Foo(char *s, ...)
{
  va_list ArgList;
  va_start(ArgList, s);
  int Result = Bar(s, ArgList);
  va_end(ArgList);
  return Result;
}

int main(int argc, char **argv)
{
  String MyString;
  MyString.SetData("ghi");
  Foo("abc", MyString); /* { dg-error "cannot pass objects of" } */
}
