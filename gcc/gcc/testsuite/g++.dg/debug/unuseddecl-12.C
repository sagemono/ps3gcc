// { dg-do compile }
// { dg-options "-dA" }

namespace nsnot
{
  typedef int notmetype;
  struct notmeclass {
  void notmedecl(notmetype a);
  };
}

using nsnot::notmeclass;

static
void notmedecl2(notmeclass a)
{
  a.notmedecl(1);
}

// Even though to the compiler notmedecl and notmetype ared used via the using,
// there is no need to emit debugging info for it as it not used in
// any emitted functions.
// { dg-final { scan-assembler-not "string \\\"notmetype\\\"" } }
// { dg-final { scan-assembler-not "string \\\"notmeclass\\\"" } }
// { dg-final { scan-assembler-not "ascii \\\"notmedecl" } }
// { dg-final { scan-assembler-not "notmedecl2" } }
