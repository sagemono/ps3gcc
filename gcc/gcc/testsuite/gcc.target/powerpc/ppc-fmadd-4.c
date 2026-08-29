/* { dg-do compile } */
/* { dg-options "-O2" } */
float
f (float a, float b)
{
  return 2.0f * a * (1.f + b);
}

/* We should be able do this function without any load from memory and
   with only fadd and fmadd. This originally comes from vpr. */
/* { dg-final { scan-assembler "fadd" } } */
/* { dg-final { scan-assembler "fmadd" } } */
/* { dg-final { scan-assembler-not "lfs" } } */
/* { dg-final { scan-assembler-not "lfd" } } */
