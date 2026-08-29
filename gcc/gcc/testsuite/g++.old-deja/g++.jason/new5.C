// { dg-do run  }
// { dg-xfail-if "eh not supported on spu" { "spu-*-*" } "*" "" }
int main ()
{
  const int *p = new const int (0);
  delete p;
}
