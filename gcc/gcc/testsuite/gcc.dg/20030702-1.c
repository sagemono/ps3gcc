/* This tests whether REG_ALWAYS_RETURN notes are handled
   correctly in combine.  */
/* { dg-do compile } */
/* { dg-options "-O2 -fpic -fprofile-arcs" } */
/* CELL LOCAL */
/* { dg-error "Can't create dynamic relocations" "PIC" { target spu-*-* } 0 } */

void test (void)
{
  fork ();
}

