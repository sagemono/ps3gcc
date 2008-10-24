/* { dg-do compile { target "ppu-*-*" } } */
/* { dg-options "-Os -mcpu=cell -mgen-microcode -mwarn-microcode" } */
/* Testing that addze. does not emit a microcode warning */
typedef long long it;

it f(void);
it g(void);
it div (it a)
{
 it b = a/8;
 if (b < 0)
   return b;
 return a;
} /* { dg-bogus "emitting microcode" } */


