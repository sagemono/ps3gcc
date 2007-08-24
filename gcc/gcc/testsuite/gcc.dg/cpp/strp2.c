/* { dg-do run } */

/* Test for odd corner cases in stringizing/pasting.
   Taken more or less verbatim from C99 section 6.10.3.3.  */

#include <stdlib.h>
/* CELL LOCAL Begin */
/* spu_internals.h declares si_sp as a file-scope variable with the register
 * keyword, but "-ansi -pedantic-errors" specified in dg.exp doesn't like it.
 */
/* { dg-options "" { target spu-*-* } } */
/* CELL LOCAL End */
#include <string.h>

#define hash_hash # ## #
#define mkstr(a) # a
#define in_between(a) mkstr(a)
#define join(c, d) in_between(c hash_hash d)

const char p[] = join(x, y);
const char q[] = "x ## y";

int
main (void)
{
  if (strcmp (p, q))
    abort ();
  return 0;
}
