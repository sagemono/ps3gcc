/* { dg-do compile } */
/* { dg-options "-Wno-nonPOD-varargs" } */
/* Test to check that no warning is emitted on passing a nonPOD type to
 * a variable arguments function because option -Wno-nonPOD-varargs is
 * enabled */
#include <stdarg.h>
#include <stdio.h>
class A 
{
 int i;
};
 
int foo(char*, ...);
int bar() 
{
 A a;
 foo("hello",a); /* { dg-bogus "" } */
 return 0;
}

