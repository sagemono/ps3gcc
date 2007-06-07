// { dg-do run  }
// { dg-xfail-if "eh not supported on spu" { "spu-*-*" } "*" "" }
extern "C" void exit (int);
extern "C" void abort (void);
struct A { int i; };
int main ()
{
  try { throw &A::i; }
  catch (int A::*p)
    {
      if (p == &A::i)
	exit (0);
      else
	abort ();
    }
  abort ();
}
