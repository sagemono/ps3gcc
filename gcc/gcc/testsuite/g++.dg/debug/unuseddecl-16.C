// { dg-do compile }
// { dg-options "-dA" }
// { dg-skip-if "-g1 does not emit variable info" { *-*-* } "-gdwarf-21" "" }

namespace nsnot
{
  typedef int notmetype;
  void notmedecl(notmetype);
}

using nsnot::notmedecl;
using nsnot::notmetype;

void f(notmetype a)
{
  notmedecl(a);
}


// notmedecl is used via the function call in f so the debugging info should be emitted.
// debugging info for notmetype should also be emitted as it is used via f except at -g1,
//   local variable debugging is not emitted so force it via a global variable b so we cannot
//   test that it emits that info.
// { dg-final { scan-assembler "notmedecl" } } 
