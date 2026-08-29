// { dg-do compile }
// { dg-options "-dA" }

namespace nsnot
{
  typedef int notmetype;
  void notmedecl(notmetype);
}

using nsnot::notmetype;
using nsnot::notmedecl;


// Even though to the compiler notmedecl and notmetype ared used via the using,
// there is no need to emit debugging info for it as it not used in
// any emitted functions.
// { dg-final { scan-assembler-not "string \\\"notmetype\\\"" } }
// { dg-final { scan-assembler-not "notmedecl" } }
