// { dg-do run { xfail sparc64-*-elf arm-*-pe } }
// { dg-xfail-if "eh not supported on spu" { "spu-*-*" } "*" "" }
// { dg-options "-fexceptions" }

class foo {
public:
  class error {};

  void cause_error(void) { throw error(); }
};

int main(void)
{
  foo f;
  try {
    f.cause_error();
  }
  catch (foo::error&) {
    return 0;
  }
  return 1;
}
