/* Test for wprintf formats.  Formats using C94 features, including cases
   where C94 specifies some aspect of the format to be ignored or where
   the behaviour is undefined.
*/
/* Origin: Joseph Myers <jsm28@cam.ac.uk> */
/* { dg-do compile } */
/* { dg-options "-std=iso9899:199409 -pedantic -Wformat" } */

#include "format.h"

void
foo (int i, int i1, int i2, unsigned int u, double d, char *s, void *p,
     int *n, short int *hn, long int l, unsigned long int ul,
     long int *ln, long double ld, wint_t lc, wchar_t *ls, llong ll,
     ullong ull, unsigned int *un, const int *cn, signed char *ss,
     unsigned char *us, const signed char *css, unsigned int u1,
     unsigned int u2)
{
  /* See ISO/IEC 9899:1990 (E) subclause 7.9.6.1 (pages 131-134).  */
  /* Basic sanity checks for the different components of a format.  */
  wprintf (L"%d\n", i);
  wprintf (L"%+d\n", i);
  wprintf (L"%3d\n", i);
  wprintf (L"%-3d\n", i);
  wprintf (L"%.7d\n", i);
  wprintf (L"%+9.4d\n", i);
  wprintf (L"%.3ld\n", l);
  wprintf (L"%*d\n", i1, i);
  wprintf (L"%.*d\n", i2, i);
  wprintf (L"%*.*ld\n", i1, i2, l);
  wprintf (L"%d %lu\n", i, ul);
  /* GCC has objected to the next one in the past, but it is a valid way
     of specifying zero precision.
  */
  wprintf (L"%.e\n", d); /* { dg-bogus "precision" "bogus precision warning" } */
  /* Bogus use of width.  */
  wprintf (L"%5n\n", n); /* { dg-warning "width" "width with %n" } */
  /* Erroneous, ignored or pointless constructs with precision.  */
  /* Whether negative values for precision may be included in the format
     string is not entirely clear; presume not, following Clive Feather's
     proposed resolution to DR#220 against C99.  In any case, such a
     construct should be warned about.
  */
  wprintf (L"%.-5d\n", i); /* { dg-warning "format|precision" "negative precision warning" } */
  wprintf (L"%.-*d\n", i); /* { dg-warning "format" "broken %.-*d format" } */
  wprintf (L"%.3c\n", i); /* { dg-warning "precision" "precision with %c" } */
  wprintf (L"%.3p\n", p); /* { dg-warning "precision" "precision with %p" } */
  wprintf (L"%.3n\n", n); /* { dg-warning "precision" "precision with %n" } */
  /* Valid and invalid %% constructions.  Some of the warning messages
     are non-optimal, but they do detect the errorneous nature of the
     format string.
  */
  wprintf (L"%%");
  wprintf (L"%.3%"); /* { dg-warning "format" "bogus %%" } */
  wprintf (L"%-%"); /* { dg-warning "format" "bogus %%" } */
  wprintf (L"%-%\n"); /* { dg-warning "format" "bogus %%" } */
  wprintf (L"%5%\n"); /* { dg-warning "format" "bogus %%" } */
  wprintf (L"%h%\n"); /* { dg-warning "format" "bogus %%" } */
  /* Valid and invalid %h, %l, %L constructions.  */
  wprintf (L"%hd", i);
  wprintf (L"%hi", i);
  /* Strictly, these parameters should be int or unsigned int according to
     what unsigned short promotes to.  However, GCC ignores sign
     differences in format checking here, and this is relied on to get the
     correct checking without print_char_table needing to know whether
     int and short are the same size.
  */
  wprintf (L"%ho%hu%hx%hX", u, u, u, u);
  wprintf (L"%hn", hn);
  wprintf (L"%hf", d); /* { dg-warning "length" "bad use of %h" } */
  wprintf (L"%he", d); /* { dg-warning "length" "bad use of %h" } */
  wprintf (L"%hE", d); /* { dg-warning "length" "bad use of %h" } */
  wprintf (L"%hg", d); /* { dg-warning "length" "bad use of %h" } */
  wprintf (L"%hG", d); /* { dg-warning "length" "bad use of %h" } */
  wprintf (L"%hc", i); /* { dg-warning "length" "bad use of %h" } */
  wprintf (L"%hs", s); /* { dg-warning "length" "bad use of %h" } */
  wprintf (L"%hp", p); /* { dg-warning "length" "bad use of %h" } */
  wprintf (L"%h"); /* { dg-warning "conversion lacks type" "bare %h" } */
  wprintf (L"%h."); /* { dg-warning "conversion" "bogus %h." } */
  wprintf (L"%ld%li%lo%lu%lx%lX", l, l, ul, ul, ul, ul);
  wprintf (L"%ln", ln);
  wprintf (L"%lf", d); /* { dg-warning "length|C" "bad use of %l" } */
  wprintf (L"%le", d); /* { dg-warning "length|C" "bad use of %l" } */
  wprintf (L"%lE", d); /* { dg-warning "length|C" "bad use of %l" } */
  wprintf (L"%lg", d); /* { dg-warning "length|C" "bad use of %l" } */
  wprintf (L"%lG", d); /* { dg-warning "length|C" "bad use of %l" } */
  wprintf (L"%lp", p); /* { dg-warning "length|C" "bad use of %l" } */
  wprintf (L"%lc", lc);
  wprintf (L"%ls", ls);
  /* These uses of %L are legitimate, though GCC has wrongly warned for
     them in the past.
  */
  wprintf (L"%Le%LE%Lf%Lg%LG", ld, ld, ld, ld, ld);
  /* These next six are accepted by GCC as referring to long long,
     but -pedantic correctly warns.
  */
  wprintf (L"%Ld", ll); /* { dg-warning "does not support" "bad use of %L" } */
  wprintf (L"%Li", ll); /* { dg-warning "does not support" "bad use of %L" } */
  wprintf (L"%Lo", ull); /* { dg-warning "does not support" "bad use of %L" } */
  wprintf (L"%Lu", ull); /* { dg-warning "does not support" "bad use of %L" } */
  wprintf (L"%Lx", ull); /* { dg-warning "does not support" "bad use of %L" } */
  wprintf (L"%LX", ull); /* { dg-warning "does not support" "bad use of %L" } */
  wprintf (L"%Lc", i); /* { dg-warning "length" "bad use of %L" } */
  wprintf (L"%Ls", s); /* { dg-warning "length" "bad use of %L" } */
  wprintf (L"%Lp", p); /* { dg-warning "length" "bad use of %L" } */
  wprintf (L"%Ln", n); /* { dg-warning "length" "bad use of %L" } */
  /* Valid uses of each bare conversion.  */
  wprintf (L"%d%i%o%u%x%X%f%e%E%g%G%c%s%p%n%%", i, i, u, u, u, u, d, d, d, d, d,
	   i, s, p, n);
  /* Uses of the - flag (valid on all non-%, non-n conversions).  */
  wprintf (L"%-d%-i%-o%-u%-x%-X%-f%-e%-E%-g%-G%-c%-s%-p", i, i, u, u, u, u,
	   d, d, d, d, d, i, s, p);
  wprintf (L"%-n", n); /* { dg-warning "flag" "bad use of %-n" } */
  /* Uses of the + flag (valid on signed conversions only).  */
  wprintf (L"%+d%+i%+f%+e%+E%+g%+G\n", i, i, d, d, d, d, d);
  wprintf (L"%+o", u); /* { dg-warning "flag" "bad use of + flag" } */
  wprintf (L"%+u", u); /* { dg-warning "flag" "bad use of + flag" } */
  wprintf (L"%+x", u); /* { dg-warning "flag" "bad use of + flag" } */
  wprintf (L"%+X", u); /* { dg-warning "flag" "bad use of + flag" } */
  wprintf (L"%+c", i); /* { dg-warning "flag" "bad use of + flag" } */
  wprintf (L"%+s", s); /* { dg-warning "flag" "bad use of + flag" } */
  wprintf (L"%+p", p); /* { dg-warning "flag" "bad use of + flag" } */
  wprintf (L"%+n", n); /* { dg-warning "flag" "bad use of + flag" } */
  /* Uses of the space flag (valid on signed conversions only, and ignored
     with +).
  */
  wprintf (L"% +d", i); /* { dg-warning "use of both|ignored" "use of space and + flags" } */
  wprintf (L"%+ d", i); /* { dg-warning "use of both|ignored" "use of space and + flags" } */
  wprintf (L"% d% i% f% e% E% g% G\n", i, i, d, d, d, d, d);
  wprintf (L"% o", u); /* { dg-warning "flag" "bad use of space flag" } */
  wprintf (L"% u", u); /* { dg-warning "flag" "bad use of space flag" } */
  wprintf (L"% x", u); /* { dg-warning "flag" "bad use of space flag" } */
  wprintf (L"% X", u); /* { dg-warning "flag" "bad use of space flag" } */
  wprintf (L"% c", i); /* { dg-warning "flag" "bad use of space flag" } */
  wprintf (L"% s", s); /* { dg-warning "flag" "bad use of space flag" } */
  wprintf (L"% p", p); /* { dg-warning "flag" "bad use of space flag" } */
  wprintf (L"% n", n); /* { dg-warning "flag" "bad use of space flag" } */
  /* Uses of the # flag.  */
  wprintf (L"%#o%#x%#X%#e%#E%#f%#g%#G", u, u, u, d, d, d, d, d);
  wprintf (L"%#d", i); /* { dg-warning "flag" "bad use of # flag" } */
  wprintf (L"%#i", i); /* { dg-warning "flag" "bad use of # flag" } */
  wprintf (L"%#u", u); /* { dg-warning "flag" "bad use of # flag" } */
  wprintf (L"%#c", i); /* { dg-warning "flag" "bad use of # flag" } */
  wprintf (L"%#s", s); /* { dg-warning "flag" "bad use of # flag" } */
  wprintf (L"%#p", p); /* { dg-warning "flag" "bad use of # flag" } */
  wprintf (L"%#n", n); /* { dg-warning "flag" "bad use of # flag" } */
  /* Uses of the 0 flag.  */
  wprintf (L"%08d%08i%08o%08u%08x%08X%08e%08E%08f%08g%08G", i, i, u, u, u, u,
	   d, d, d, d, d);
  wprintf (L"%0c", i); /* { dg-warning "flag" "bad use of 0 flag" } */
  wprintf (L"%0s", s); /* { dg-warning "flag" "bad use of 0 flag" } */
  wprintf (L"%0p", p); /* { dg-warning "flag" "bad use of 0 flag" } */
  wprintf (L"%0n", n); /* { dg-warning "flag" "bad use of 0 flag" } */
  /* 0 flag ignored with precision for certain types, not others.  */
  wprintf (L"%08.5d", i); /* { dg-warning "ignored" "0 flag ignored with precision" } */
  wprintf (L"%08.5i", i); /* { dg-warning "ignored" "0 flag ignored with precision" } */
  wprintf (L"%08.5o", u); /* { dg-warning "ignored" "0 flag ignored with precision" } */
  wprintf (L"%08.5u", u); /* { dg-warning "ignored" "0 flag ignored with precision" } */
  wprintf (L"%08.5x", u); /* { dg-warning "ignored" "0 flag ignored with precision" } */
  wprintf (L"%08.5X", u); /* { dg-warning "ignored" "0 flag ignored with precision" } */
  wprintf (L"%08.5f%08.5e%08.5E%08.5g%08.5G", d, d, d, d, d);
  /* 0 flag ignored with - flag.  */
  wprintf (L"%-08d", i); /* { dg-warning "flags|ignored" "0 flag ignored with - flag" } */
  wprintf (L"%-08i", i); /* { dg-warning "flags|ignored" "0 flag ignored with - flag" } */
  wprintf (L"%-08o", u); /* { dg-warning "flags|ignored" "0 flag ignored with - flag" } */
  wprintf (L"%-08u", u); /* { dg-warning "flags|ignored" "0 flag ignored with - flag" } */
  wprintf (L"%-08x", u); /* { dg-warning "flags|ignored" "0 flag ignored with - flag" } */
  wprintf (L"%-08X", u); /* { dg-warning "flags|ignored" "0 flag ignored with - flag" } */
  wprintf (L"%-08e", d); /* { dg-warning "flags|ignored" "0 flag ignored with - flag" } */
  wprintf (L"%-08E", d); /* { dg-warning "flags|ignored" "0 flag ignored with - flag" } */
  wprintf (L"%-08f", d); /* { dg-warning "flags|ignored" "0 flag ignored with - flag" } */
  wprintf (L"%-08g", d); /* { dg-warning "flags|ignored" "0 flag ignored with - flag" } */
  wprintf (L"%-08G", d); /* { dg-warning "flags|ignored" "0 flag ignored with - flag" } */
  /* Various tests of bad argument types.  */
  wprintf (L"%d", l); /* { dg-warning "format" "bad argument types" } */
  wprintf (L"%*.*d", l, i2, i); /* { dg-warning "field" "bad * argument types" } */
  wprintf (L"%*.*d", i1, l, i); /* { dg-warning "field" "bad * argument types" } */
  wprintf (L"%ld", i); /* { dg-warning "format" "bad argument types" } */
  wprintf (L"%s", n); /* { dg-warning "format" "bad argument types" } */
  wprintf (L"%p", i); /* { dg-warning "format" "bad argument types" } */
  wprintf (L"%n", p); /* { dg-warning "format" "bad argument types" } */
  /* With -pedantic, we want some further checks for pointer targets:
     %p should allow only pointers to void (possibly qualified) and
     to character types (possibly qualified), but not function pointers
     or pointers to other types.  (Whether, in fact, character types are
     allowed here is unclear; see thread on comp.std.c, July 2000 for
     discussion of the requirements of rules on identical representation,
     and of the application of the as if rule with the new va_arg
     allowances in C99 to wprintf.)  Likewise, we should warn if
     pointer targets differ in signedness, except in some circumstances
     for character pointers.  (In C99 we should consider warning for
     char * or unsigned char * being passed to %hhn, even if strictly
     legitimate by the standard.)
  */
  wprintf (L"%p", foo); /* { dg-warning "format" "bad argument types" } */
  wprintf (L"%n", un); /* { dg-warning "format" "bad argument types" } */
  wprintf (L"%p", n); /* { dg-warning "format" "bad argument types" } */
  /* Allow character pointers with %p.  */
  wprintf (L"%p%p%p%p", s, ss, us, css);
  /* %s allows any character type.  */
  wprintf (L"%s%s%s%s", s, ss, us, css);
  /* Warning for void * arguments for %s is GCC's historical behaviour,
     and seems useful to keep, even if some standard versions might be
     read to permit it.
  */
  wprintf (L"%s", p); /* { dg-warning "format" "bad argument types" } */
  /* The historical behaviour is to allow signed / unsigned types
     interchangably as arguments.  For values representable in both types,
     such usage may be correct.  For now preserve the behaviour of GCC
     in such cases.
  */
  wprintf (L"%d", u);
  /* Also allow the same for width and precision arguments.  In the past,
     GCC has been inconsistent and allowed unsigned for width but not
     precision.
  */
  wprintf (L"%*.*d", u1, u2, i);
  /* Wrong number of arguments.  */
  wprintf (L"%d%d", i); /* { dg-warning "arguments" "wrong number of args" } */
  wprintf (L"%d", i, i); /* { dg-warning "arguments" "wrong number of args" } */
  /* Miscellaneous bogus constructions.  */
  wprintf (L""); /* { dg-warning "zero-length" "warning for empty format" } */
  wprintf (L"\0"); /* { dg-warning "embedded" "warning for embedded NUL" } */
  wprintf (L"%d\0", i); /* { dg-warning "embedded" "warning for embedded NUL" } */
  wprintf (L"%d\0%d", i, i); /* { dg-warning "embedded|too many" "warning for embedded NUL" } */
  wprintf (NULL); /* { dg-warning "null" "null format string warning" } */
  wprintf (L"%"); /* { dg-warning "trailing" "trailing % warning" } */
  wprintf (L"%++d", i); /* { dg-warning "repeated" "repeated flag warning" } */
  wprintf (L"%n", cn); /* { dg-warning "constant" "%n with const" } */
  wprintf ((const wchar_t *)"foo"); /* { dg-warning "not a wide" "non-wide string" } */
  wprintf (L"%n", (int *)0); /* { dg-warning "null" "%n with NULL" } */
  wprintf (L"%s", (char *)0); /* { dg-warning "null" "%s with NULL" } */
}
