/* { dg-do compile { target "ppu-*-*" } } */
/* { dg-options "-mcpu=cell -O2 -mgen-microcode -mwarn-microcode" } */
/* Testing to check that the load in this case does not cause a microcode warning */
unsigned long long f(unsigned char *a)
{
  return *a;
} /* { dg-bogus "microcode" } */
