/* Test for format checking of conditional expressions for wide
   character strings.  */
/* Origin: Joseph Myers <jsm28@cam.ac.uk> */
/* { dg-do compile } */
/* { dg-options "-std=gnu99 -Wformat" } */

#include "format.h"

void
foo (long l, int nfoo)
{
  wprintf ((nfoo > 1) ? L"%d foos" : L"%d foo", nfoo);
  wprintf ((l > 1) ? L"%d foos" : L"%d foo", l); /* { dg-warning "int" "wrong type in conditional expr" } */
  wprintf ((l > 1) ? L"%ld foos" : L"%d foo", l); /* { dg-warning "int" "wrong type in conditional expr" } */
  wprintf ((l > 1) ? L"%d foos" : L"%ld foo", l); /* { dg-warning "int" "wrong type in conditional expr" } */
  /* Should allow one case to have extra arguments.  */
  wprintf ((nfoo > 1) ? L"%d foos" : L"1 foo", nfoo);
  wprintf ((nfoo > 1) ? L"many foos" : L"1 foo", nfoo); /* { dg-warning "too many" "too many args in all branches" } */
  wprintf ((nfoo > 1) ? L"%d foos" : L"", nfoo);
  wprintf ((nfoo > 1) ? L"%d foos" : ((nfoo > 0) ? L"1 foo" : L"no foos"), nfoo);
  wprintf ((nfoo > 1) ? L"%d foos" : ((nfoo > 0) ? L"%d foo" : L"%d foos"), nfoo);
  wprintf ((nfoo > 1) ? L"%d foos" : ((nfoo > 0) ? L"%d foo" : L"%ld foos"), nfoo); /* { dg-warning "long int" "wrong type" } */
  wprintf ((nfoo > 1) ? L"%ld foos" : ((nfoo > 0) ? L"%d foo" : L"%d foos"), nfoo); /* { dg-warning "long int" "wrong type" } */
  wprintf ((nfoo > 1) ? L"%d foos" : ((nfoo > 0) ? L"%ld foo" : L"%d foos"), nfoo); /* { dg-warning "long int" "wrong type" } */
  /* Extra arguments to NULL should be complained about.  */
  wprintf (NULL, L"foo"); /* { dg-warning "too many" "NULL extra args" } */
  /* { dg-warning "null" "null format arg" { target *-*-* } 26 } */
}
