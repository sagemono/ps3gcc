/* { dg-do compile { target "ppu-*-*"} } */
/* { dg-options "-O2 -S -mcpu=cell -mgen-microcode -mwarn-microcode" } */
/* Testing that nor. emits a microcode warning in this case where the comparison is followed by a negation */
int p (long long x)
{
 long long y = ~x,z;
 if ( y > 0 )
  z = ~y;
 if (y > 0)
  return z;
 else
  return 1;
} /* { dg-warning "emitting microcode" } */
