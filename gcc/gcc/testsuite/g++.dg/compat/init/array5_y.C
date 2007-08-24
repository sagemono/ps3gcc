/* { dg-xfail-if "eh not supported on spu" { "spu-*-*" } "*" "" } */

int count;
int num;

struct A
{
  A();
  ~A();
};

A::A()
{
  if (count == num)
    throw "";
  count++;
}

A::~A()
{
  count--;
}
