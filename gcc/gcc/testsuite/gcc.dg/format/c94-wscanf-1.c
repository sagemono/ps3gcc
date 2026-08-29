/* Test for wscanf formats.  Formats using C94 features, including cases
   where C94 specifies some aspect of the format to be ignored or where
   the behaviour is undefined.
*/
/* Origin: Joseph Myers <jsm28@cam.ac.uk> */
/* { dg-do compile } */
/* { dg-options "-std=iso9899:199409 -pedantic -Wformat" } */

#include "format.h"

void
foo (int *ip, unsigned int *uip, short int *hp, unsigned short int *uhp,
     long int *lp, unsigned long int *ulp, float *fp, double *dp,
     long double *ldp, char *s, signed char *ss, unsigned char *us,
     void **pp, int *n, llong *llp, ullong *ullp, wchar_t *ls,
     const int *cip, const int *cn, const char *cs, const void **ppc,
     void *const *pcp, short int *hn, long int *ln, void *p, char **sp,
     volatile void *ppv)
{
  /* See ISO/IEC 9899:1990 (E) subclause 7.9.6.2 (pages 134-138).  */
  /* Basic sanity checks for the different components of a format.  */
  wscanf (L"%d", ip);
  wscanf (L"%*d");
  wscanf (L"%3d", ip);
  wscanf (L"%hd", hp);
  wscanf (L"%3ld", lp);
  wscanf (L"%*3d");
  wscanf (L"%d %ld", ip, lp);
  /* Valid and invalid %% constructions.  */
  wscanf (L"%%");
  wscanf (L"%*%"); /* { dg-warning "format" "bogus %%" } */
  wscanf (L"%*%\n"); /* { dg-warning "format" "bogus %%" } */
  wscanf (L"%4%"); /* { dg-warning "format" "bogus %%" } */
  wscanf (L"%4%\n"); /* { dg-warning "format" "bogus %%" } */
  wscanf (L"%h%"); /* { dg-warning "format" "bogus %%" } */
  wscanf (L"%h%\n"); /* { dg-warning "format" "bogus %%" } */
  /* Valid, invalid and silly assignment-suppression constructions.  */
  wscanf (L"%*d%*i%*o%*u%*x%*X%*e%*E%*f%*g%*G%*s%*[abc]%*c%*p");
  wscanf (L"%*2d%*8s%*3c");
  wscanf (L"%*n", n); /* { dg-warning "suppress" "suppression of %n" } */
  wscanf (L"%*hd"); /* { dg-warning "together" "suppression with length" } */
  /* Valid, invalid and silly width constructions.  */
  wscanf (L"%2d%3i%4o%5u%6x%7X%8e%9E%10f%11g%12G%13s%14[abc]%15c%16p",
	  ip, ip, uip, uip, uip, uip, fp, fp, fp, fp, fp, s, s, s, pp);
  wscanf (L"%0d", ip); /* { dg-warning "width" "warning for zero width" } */
  wscanf (L"%3n", n); /* { dg-warning "width" "width with %n" } */
  /* Valid and invalid %h, %l, %L constructions.  */
  wscanf (L"%hd%hi%ho%hu%hx%hX%hn", hp, hp, uhp, uhp, uhp, uhp, hn);
  wscanf (L"%he", fp); /* { dg-warning "length" "bad use of %h" } */
  wscanf (L"%hE", fp); /* { dg-warning "length" "bad use of %h" } */
  wscanf (L"%hf", fp); /* { dg-warning "length" "bad use of %h" } */
  wscanf (L"%hg", fp); /* { dg-warning "length" "bad use of %h" } */
  wscanf (L"%hG", fp); /* { dg-warning "length" "bad use of %h" } */
  wscanf (L"%hs", s); /* { dg-warning "length" "bad use of %h" } */
  wscanf (L"%h[ac]", s); /* { dg-warning "length" "bad use of %h" } */
  wscanf (L"%hc", s); /* { dg-warning "length" "bad use of %h" } */
  wscanf (L"%hp", pp); /* { dg-warning "length" "bad use of %h" } */
  wscanf (L"%h"); /* { dg-warning "conversion lacks type" "bare %h" } */
  wscanf (L"%h."); /* { dg-warning "conversion" "bogus %h" } */
  wscanf (L"%ld%li%lo%lu%lx%lX%ln", lp, lp, ulp, ulp, ulp, ulp, ln);
  wscanf (L"%le%lE%lf%lg%lG", dp, dp, dp, dp, dp);
  wscanf (L"%lp", pp); /* { dg-warning "length" "bad use of %l" } */
  wscanf (L"%lc%ls%l[abc]", ls, ls, ls);
  wscanf (L"%Le%LE%Lf%Lg%LG", ldp, ldp, ldp, ldp, ldp);
  wscanf (L"%Ld", llp); /* { dg-warning "does not support" "bad use of %L" } */
  wscanf (L"%Li", llp); /* { dg-warning "does not support" "bad use of %L" } */
  wscanf (L"%Lo", ullp); /* { dg-warning "does not support" "bad use of %L" } */
  wscanf (L"%Lu", ullp); /* { dg-warning "does not support" "bad use of %L" } */
  wscanf (L"%Lx", ullp); /* { dg-warning "does not support" "bad use of %L" } */
  wscanf (L"%LX", ullp); /* { dg-warning "does not support" "bad use of %L" } */
  wscanf (L"%Ls", s); /* { dg-warning "length" "bad use of %L" } */
  wscanf (L"%L[ac]", s); /* { dg-warning "length" "bad use of %L" } */
  wscanf (L"%Lc", s); /* { dg-warning "length" "bad use of %L" } */
  wscanf (L"%Lp", pp); /* { dg-warning "length" "bad use of %L" } */
  wscanf (L"%Ln", n); /* { dg-warning "length" "bad use of %L" } */
  /* Valid uses of each bare conversion.  */
  wscanf (L"%d%i%o%u%x%X%e%E%f%g%G%s%[abc]%c%p%n%%", ip, ip, uip, uip, uip,
	  uip, fp, fp, fp, fp, fp, s, s, s, pp, n);
  /* Allow other character pointers with %s, %c, %[].  */
  wscanf (L"%2s%3s%4c%5c%6[abc]%7[abc]", ss, us, ss, us, ss, us);
  /* Further tests for %[].  */
  wscanf (L"%[%d]%d", s, ip);
  wscanf (L"%[^%d]%d", s, ip);
  wscanf (L"%[]%d]%d", s, ip);
  wscanf (L"%[^]%d]%d", s, ip);
  wscanf (L"%[%d]%d", s, ip);
  wscanf (L"%[]abcd", s); /* { dg-warning "no closing" "incomplete scanset" } */
  /* Various tests of bad argument types.  Some of these are only pedantic
     warnings.
  */
  wscanf (L"%d", lp); /* { dg-warning "format" "bad argument types" } */
  wscanf (L"%d", uip); /* { dg-warning "format" "bad argument types" } */
  wscanf (L"%d", pp); /* { dg-warning "format" "bad argument types" } */
  wscanf (L"%p", ppc); /* { dg-warning "format" "bad argument types" } */
  wscanf (L"%p", ppv); /* { dg-warning "format" "bad argument types" } */
  wscanf (L"%s", n); /* { dg-warning "format" "bad argument types" } */
  wscanf (L"%s", p); /* { dg-warning "format" "bad argument types" } */
  wscanf (L"%p", p); /* { dg-warning "format" "bad argument types" } */
  wscanf (L"%p", sp); /* { dg-warning "format" "bad argument types" } */
  /* Tests for writing into constant values.  */
  wscanf (L"%d", cip); /* { dg-warning "constant" "%d writing into const" } */
  wscanf (L"%n", cn); /* { dg-warning "constant" "%n writing into const" } */
  wscanf (L"%s", cs); /* { dg-warning "constant" "%s writing into const" } */
  wscanf (L"%p", pcp); /* { dg-warning "constant" "%p writing into const" } */
  /* Wrong number of arguments.  */
  wscanf (L"%d%d", ip); /* { dg-warning "arguments" "wrong number of args" } */
  wscanf (L"%d", ip, ip); /* { dg-warning "arguments" "wrong number of args" } */
  /* Miscellaneous bogus constructions.  */
  wscanf (L""); /* { dg-warning "zero-length" "warning for empty format" } */
  wscanf (L"\0"); /* { dg-warning "embedded" "warning for embedded NUL" } */
  wscanf (L"%d\0", ip); /* { dg-warning "embedded" "warning for embedded NUL" } */
  wscanf (L"%d\0%d", ip, ip); /* { dg-warning "embedded|too many" "warning for embedded NUL" } */
  wscanf (NULL); /* { dg-warning "null" "null format string warning" } */
  wscanf (L"%"); /* { dg-warning "trailing" "trailing % warning" } */
  wscanf (L"%d", (int *)0); /* { dg-warning "null" "writing into NULL" } */
}
