/* { dg-do compile } */
/* { dg-options "-O2 -mno-toc=1" } */
void direct_call ();
typedef void (*FP)();

test (FP indirect_call)
{
  direct_call();
  (*indirect_call)();
}
/* { dg-final { scan-assembler-not "nop" } } */
/* { dg-final { scan-assembler "std 2,40\\(1\\)" } } */
/* { dg-final { scan-assembler "ld 2,40\\(1\\)" } } */
