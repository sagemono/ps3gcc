/* { dg-do compile } */
/* { dg-options "-O2" } */
union A
{
 float a;
};

void t1(union A);
void t(float a)
{
  union A a1, a2, a3;
  a1.a = a;
  a3 = a1;
  t1(a3);
}
/*  The union A should be passed via integer register so we should store and then load
    the floating point value.  */
/* { dg-final { scan-assembler "stfs" } } */
