/* { dg-do compile { target "ppu-*-*" } } */
/* { dg-options "-mcpu=cell -O2 -mgen-microcode -mwarn-microcode" } */
/* Testing that the subf. instruction does not cause a microcode warning */
long long sub (long long a, long long b)
{
        long long c;
        c=(-(a)+b);
        if(c>0)
                return a + 1;
        else

                return b + 1;

} /* { dg-bogus "emitting microcode" } */

