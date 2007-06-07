// PR c++/17155
// { dg-do link }
/* { dg-xfail-if "eh not supported on spu" { "spu-*-*" } "*" "" } */

struct A {
  virtual ~A() {}
};


void tsk_tsk(void)
{
  struct B : public A {
    B(int) {}
  };
}

int main () {}
