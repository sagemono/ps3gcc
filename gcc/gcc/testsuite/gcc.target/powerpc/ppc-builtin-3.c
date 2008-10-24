/* { dg-do compile } */
/* { dg-options "-O2 -mcpu=cell" } */
/* Test to check that mfcr is not emitted for the following testcase */
int f1 (float a, float b)
{
	return __builtin_isunordered ( a,b);
}
/* { dg-final { scan-assembler-not "mfcr" } } */
