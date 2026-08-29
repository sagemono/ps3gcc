/* { dg-do compile } */
/* { dg-options "-O2 -mcpu=cell" } */
/* Test to check that mfcr is not emitted for following test case */
int f(float a, float b)
{
 return __builtin_isgreaterequal(a,b);
}
int f1 (float a, float b)
{
 return __builtin_isunordered(a,b);
}
/* { dg-final { scan-assembler-not "mfcr" } } */
