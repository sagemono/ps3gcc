// Test that vtables are set up properly for constructors and destructors
// of template classes.

// { dg-do run }
// { dg-xfail-if "eh not supported on spu" { "spu-*-*" } "*" "" }

int r;

template <class T>
struct A {
  virtual void f () { }
  A() { f (); }
  ~A() { f (); }
};

struct B : public A<int> {
  virtual void f () { ++r; }
};

int main ()
{
  { B b; }
  return r;
}
