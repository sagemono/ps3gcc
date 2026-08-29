/* { dg-do compile } */
/* { dg-options "-O2" } */
void f(float *a)
{
  *a = 0.0f;
}
/* GCC should use the integer register to load "0.0f" into a register
   if it is only storing it out to memory.. */
/* { dg-final { scan-assembler "stw" } } */
