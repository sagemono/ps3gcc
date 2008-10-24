/* { dg-do compile } */
/* { dg-options "-dA -gdwarf-2" } */
/* Test that debugging information for unused declaration notme
 * is not emitted */
typedef int notme2;

extern notme2 notme;

static inline int
notme1()
{
  return notme;
} /* { dg-final { scan-assembler-not "notme" } } */
