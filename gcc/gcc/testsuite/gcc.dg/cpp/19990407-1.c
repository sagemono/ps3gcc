/* Regression test for a cpplib macro-expansion bug where
   `@' becomes `@@' when stringified.  */

/* { dg-do run } */
/* CELL LOCAL Begin */
/* spu_internals.h declares si_sp as a file-scope variable with the register
 * keyword, but "-ansi -pedantic-errors" specified in dg.exp doesn't like it.
 */
/* { dg-options "" { target spu-*-* } } */
/* CELL LOCAL End */

#include <string.h>
#include <stdlib.h>

#define STR(x) #x

char *a = STR(@foo), *b = "@foo";

int
main(void)
{
  if (strcmp (a, b))
    abort ();
  return 0;
}
