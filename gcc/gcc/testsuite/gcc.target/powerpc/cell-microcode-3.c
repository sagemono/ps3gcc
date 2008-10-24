/* { dg-do compile { target "ppu-*-*" } } */
/* { dg-options "-mcpu=cell -O2 -mgen-microcode -mwarn-microcode" } */
/* Testing that subfc. does not cause a microcode warning.  */
unsigned long long sub (unsigned long long a, unsigned long long b)
{
        unsigned long long c;
        c=(-(a)+b);
        if(c>0)
                return (c);
        else
                return(a-b);
} /* { dg-bogus "emitting microcode" } */
