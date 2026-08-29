// { dg-do run  }
// { dg-xfail-if "eh not supported on spu" { "spu-*-*" } "*" "" }
// { dg-options "-Wno-deprecated -fno-exceptions" }
// PRMS Id: 6267

struct A {
  int i;
  A() { i = 2; }
};
 
main()
{
  A *p = new A ();
}
