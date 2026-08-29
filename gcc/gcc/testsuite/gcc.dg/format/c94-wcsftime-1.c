/* Test for wcsftime formats.  Formats using C94 features.  */
/* Origin: Joseph Myers <jsm28@cam.ac.uk> */
/* { dg-do compile } */
/* { dg-options "-std=iso9899:199409 -pedantic -Wformat -Wformat-y2k" } */

#include "format.h"

void
foo (wchar_t *s, size_t m, const struct tm *tp)
{
  /* See ISO/IEC 9899:1990 (E) subclause 7.12.3.5 (pages 174-175).  */
  /* Formats which are Y2K-compliant (no 2-digit years).  */
  wcsftime (s, m, L"%a%A%b%B%d%H%I%j%m%M%p%S%U%w%W%X%Y%Z%%", tp);
  /* Formats with 2-digit years.  */
  wcsftime (s, m, L"%y", tp); /* { dg-warning "only last 2" "2-digit year" } */
  /* Formats with 2-digit years in some locales.  */
  wcsftime (s, m, L"%c", tp); /* { dg-warning "some locales" "2-digit year" } */
  wcsftime (s, m, L"%x", tp); /* { dg-warning "some locales" "2-digit year" } */
}
