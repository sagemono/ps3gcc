// PR c++/31742

int Alpha() __attribute__ ((warn_unused_result));

class C1 {
 public:
  C1();
  ~C1();

 public:
  int value_;
  int Beta() __attribute__ ((warn_unused_result)) {
    return value_;
  }
  C1 Gamma() __attribute__ ((warn_unused_result)) {
    return *this;
  }
};

extern C1 Delta() __attribute__ ((warn_unused_result));

int Foo() {
  C1 c1;
  Alpha(); // { dg-warning "ignoring" }
  c1.Beta(); // { dg-warning "ignoring" }
  c1.Gamma(); // { dg-warning "ignoring" }
  Delta(); // { dg-warning "ignoring" }
  return 0;
}

int Foo2() {
  C1 c1;
  int a = Alpha();
  int b = c1.Beta();
  C1 g = c1.Gamma();
  C1 d = Delta();
  return 0;
}
