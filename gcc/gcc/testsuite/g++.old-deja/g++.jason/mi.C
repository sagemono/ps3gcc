// { dg-do run  }
// { dg-xfail-if "eh not supported on spu" { "spu-*-*" } "*" "" }
int status;

struct A { virtual void foo () { status = 1; } };
struct B { };
struct C : public A, public B { };
struct D { virtual void baz () { } };
struct E : public D, public C { void foo () { status = 0; } };

int main ()
{
  E* ep = new E;

  ep->foo();

  return status;
}
