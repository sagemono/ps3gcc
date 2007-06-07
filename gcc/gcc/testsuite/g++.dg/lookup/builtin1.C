// PR c++/19367
// { dg-do link } 
// { dg-xfail-if "eh not supported on spu" { "spu-*-*" } "*" "" }

void abort (void) { throw 3; }

namespace std { using ::abort; }

int main ()
{
  using std::abort;
  abort();
}
