// { dg-do run  }
// { dg-options "-O2" }
// { dg-xfail-if "eh not supported on spu" { "spu-*-*" } "*" "" }
// Test for bad loop optimization of goto fixups.

typedef bool (*ftype) ();

int c, d;
struct A {
  A() { ++c; }
  A(const A&) { ++c; }
  ~A() { ++d; }
};

void f (ftype func)
{
  A a;
  do {
    if ((*func)()) return;
  } while (true);
}

bool test ()
{
  return true;
}

main ()
{
  f (test);
  return (c != d);
}
