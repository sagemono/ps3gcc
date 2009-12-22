/* { dg-do compile } */
/* { dg-options "-O2 -mno-toc" } */
void direct_call ();
typedef void (*FP)();

void
test (FP indirect_call)
{
  direct_call();
  (*indirect_call)();
}
/* { dg-final { scan-assembler-not "nop" } } */
/* { dg-final { scan-assembler-not "std 2,40\\(1\\)" } } */
/* { dg-final { scan-assembler-not "ld 2,40\\(1\\)" } } */
