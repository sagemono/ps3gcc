/* { dg-do compile } */
/* { dg-options "-O2 -mcpu=cell" } */
/* Test to check that mfcr is not emitted for this testcase */
int f(float a, float b)
{
	return __builtin_isgreaterequal (a,b);

}
/* { dg-final { scan-assembler-not "mfcr" } } */
