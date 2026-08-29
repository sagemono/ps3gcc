/* { dg-do compile } */
/* { dg-options "-O2" } */
int h(float a, int flags)
{
  if (flags & 3)
    return flags;
  if (a <= 0.0f)
    return 1;
  return 0;
}
/* GCC should not use the integer register to load "0.0f" into a register. */
/* { dg-final { scan-assembler-not "stw" } } */
