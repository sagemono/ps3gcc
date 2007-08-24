/* { dg-do compile } */
/* { dg-options "-O1 -fno-trapping-math -fdump-tree-optimized" } */
  
extern void f(int);
extern void link_error ();

extern float x;
extern double y;
extern long double z;

/* CELL LOCAL:
   We want to exclude the single precision parts of this
   test on the SPU because the SPU does not support infinity.
   However, we still want to exercise the double and long double
   parts.
 */

int
main ()
{
  double pinf = __builtin_inf ();
#ifndef __SPU__	/* CELL LOCAL */
  float pinff = __builtin_inff ();
#endif	/* CELL LOCAL */
  long double pinfl = __builtin_infl ();

  if (__builtin_isinf (pinf) != 1)
    link_error ();
#ifndef __SPU__	/* CELL LOCAL */
  if (__builtin_isinf (pinff) != 1)
    link_error ();
  if (__builtin_isinff (pinff) != 1)
    link_error ();
#endif	/* CELL LOCAL */
  if (__builtin_isinf (pinfl) != 1)
    link_error ();
  if (__builtin_isinfl (pinfl) != 1)
    link_error ();

  if (__builtin_isinf (-pinf) != -1)
    link_error ();
#ifndef __SPU__	/* CELL LOCAL */
  if (__builtin_isinf (-pinff) != -1)
    link_error ();
  if (__builtin_isinff (-pinff) != -1)
    link_error ();
#endif	/* CELL LOCAL */
  if (__builtin_isinf (-pinfl) != -1)
    link_error ();
  if (__builtin_isinfl (-pinfl) != -1)
    link_error ();

  if (__builtin_isinf (4.0))
    link_error ();
  if (__builtin_isinf (4.0))
    link_error ();
#ifndef __SPU__	/* CELL LOCAL */
  if (__builtin_isinff (4.0))
    link_error ();
#endif	/* CELL LOCAL */
  if (__builtin_isinf (4.0))
    link_error ();
  if (__builtin_isinfl (4.0))
    link_error ();
}


/* Check that all instances of link_error were subject to DCE.  */
/* { dg-final { scan-tree-dump-times "link_error" 0 "optimized" } } */
/* { dg-final { cleanup-tree-dump "optimized" } } */
