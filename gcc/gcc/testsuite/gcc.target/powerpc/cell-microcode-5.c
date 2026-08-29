/* { dg-do compile { target "ppu-*-*"} } */
/* { dg-options "-O2 -S -mcpu=cell -mgen-microcode -mwarn-microcode" } */
/* To test that nor. instruction causes a microcode warning in this case where the comparison follows a negation */
int p (long long a)
{
 long long c = ~a;
 if (c > 0)
   return c;
 else
   return 0;
} /* { dg-warning "emitting microcode" } */
