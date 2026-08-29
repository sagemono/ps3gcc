/* { dg-do compile } */
/* { dg-options "-O2" } */
union A
{
 vector float a;
};

struct B
{
  union A a;
};

void t1(struct B);
void t(vector float a)
{
  struct B a1, a2, a3;
  a1.a.a = a;
  a3 = a1;
  t1(a3);
}
/*  The struct B should be passed via integer register so we should store and then load the vector.  */
/* { dg-final { scan-assembler "stvx" } } */
