// { dg-do compile }
// { dg-options "-dA" }

namespace nsnot
{
  typedef int notmetype;
  static void notmedecl(notmetype a)
  {
   a = 2;
  }
}

using nsnot::notmetype;
//using nsnot::notmedecl;

static void notmedecl2(notmetype a)
{
  nsnot::notmedecl(a);
}

// Even though to the compiler notmedecl and notmetype ared used via the using,
// there is no need to emit debugging info for it as it not used in
// any emitted functions.
// { dg-final { scan-assembler-not "string \\\"notmetype\\\"" } }
// { dg-final { scan-assembler-not "notmedecl" } }
// { dg-final { scan-assembler-not "notmedecl2" } }
