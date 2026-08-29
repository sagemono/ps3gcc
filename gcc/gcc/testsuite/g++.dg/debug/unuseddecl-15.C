// { dg-do compile }
// { dg-options "-dA" }

namespace nsnot
{
  typedef struct __FILE {
  } *FILE;
  void notmedecl(FILE);
}

using nsnot::notmedecl;
using nsnot::FILE;

void f(FILE a)
{
  notmedecl(a);
}

// notmedecl is used via the function call in f so the debugging info should be emitted.
// debugging info for FILE should also be emitted as it is used via f.
// { dg-final { scan-assembler "notmedecl" } } 
// { dg-final { scan-assembler "FILE" } } 
