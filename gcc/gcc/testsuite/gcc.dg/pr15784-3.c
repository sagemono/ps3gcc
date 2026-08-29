/* { dg-do compile } */
/* SH4 without -mieee defaults to -ffinite-math-only.  */
/* { dg-options "-fdump-tree-gimple -fno-finite-math-only" } */
/* Test for folding abs(x) where appropriate.  */
#define abs(x) x > 0 ? x : -x
extern double fabs (double);

int a (float x) {
	return fabs(x) >= 0.0;
}

/* spu defines TARGET_FLOAT_FORMAT to SPU_FLOAT_FORMAT, which will evaluate
 * '!HONOR_NANS' to true in fold-const.c/fold() so that 'fabs(x) >= 0.0' is 
 * considered true for arbitrary x.
 */
/* { dg-final { scan-tree-dump-times "ABS_EXPR" 1 "gimple" { xfail spu-*-* } } } */
/* { dg-final { cleanup-tree-dump "gimple" } } */
