// { dg-do compile }
// { dg-options "-dA" }

namespace nsnot
{
  typedef int notmetype;
  static void notmedecl(notmetype a);
  static void notmedecl(notmetype a)
  {
    a = 2;
  }
}

using namespace nsnot;

static void notmedecl2(notmetype a)
{
  notmedecl(a);
}

// Even though to the compiler notmedecl and notmetype ared used via the using,
// there is no need to emit debugging info for it as it not used in
// any emitted functions.
// { dg-final { scan-assembler-not "string \\\"notmetype\\\"" } }
// { dg-final { scan-assembler-not "notmedecl" } }
// { dg-final { scan-assembler-not "notmedecl2" } }
