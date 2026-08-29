/* { dg-do compile } */
/* { dg-options "-O1 -fno-trapping-math -fdump-tree-optimized" } */
  
extern void f(int);
extern void link_error ();

extern float x;
extern double y;
extern long double z;

/* CELL LOCAL:
   We want to exclude the single precision floating point part of this test
   on the CELL SPU because it does not support Infinity and Nan for single 
   precision. */
 
int
main ()
{
  double nan = __builtin_nan ("");
#ifndef __SPU__ /* CELL LOCAL */
  float nanf = __builtin_nanf ("");
#endif /* CELL LOCAL */
  long double nanl = __builtin_nanl ("");

  double pinf = __builtin_inf ();
#ifndef __SPU__ /* CELL LOCAL */
  float pinff = __builtin_inff ();
#endif /* CELL LOCAL */
  long double pinfl = __builtin_infl ();

  if (__builtin_finite (pinf))
    link_error ();
#ifndef __SPU__ /* CELL LOCAL */
  if (__builtin_finitef (pinff))
    link_error ();
#endif /* CELL LOCAL */
  if (__builtin_finitel (pinfl))
    link_error ();

  if (__builtin_finite (nan))
    link_error ();
#ifndef __SPU__ /* CELL LOCAL */
  if (__builtin_finitef (nanf))
    link_error ();
#endif /* CELL LOCAL */
  if (__builtin_finitel (nanl))
    link_error ();

  if (!__builtin_finite (4.0))
    link_error ();
#ifndef __SPU__ /* CELL LOCAL */
  if (!__builtin_finitef (4.0))
    link_error ();
#endif /* CELL LOCAL */
  if (!__builtin_finitel (4.0))
    link_error ();
}


/* Check that all instances of link_error were subject to DCE.  */
/* { dg-final { scan-tree-dump-times "link_error" 0 "optimized" } } */
/* { dg-final { cleanup-tree-dump "optimized" } } */
