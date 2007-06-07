/* { dg-do compile } */
/* { dg-options "-fdump-tree-generic" } */
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
/* { dg-final { scan-tree-dump-times "ABS_EXPR" 1 "generic" { xfail spu-*-* } } } */
