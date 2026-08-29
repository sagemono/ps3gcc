// { dg-do compile }
// { dg-options "-dA" }

namespace nsnot
{
  typedef struct __FILE {
  } *FILE;
  void notmedecl(FILE);
}

using nsnot::notmedecl;

// Even though to the compiler notmedecl is used via the using,
// there is no need to emit debugging info for it as it not used in
// any emitted functions.
// { dg-final { scan-assembler-not "notmedecl" } } 
