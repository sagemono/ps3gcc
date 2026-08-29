/* { dg-do compile { target "ppu-*-*"} } */
/* { dg-options "-O2 -S -mcpu=cell -mwarn-microcode" } */
/* Testing that a microcoded nor. is not emitted for this case when compiling with -O2 */
long long f (long long a, long long b)
{
 long long c = ~a;
 if (c>0)
	return a;
 else
	return c;
} /* { dg-final { scan-assembler "nor" } } */
/* { dg-final { scan-assembler-not "nor. " } } */
