// { dg-do run  }
// { dg-xfail-if "eh not supported on spu" { "spu-*-*" } "*" "" }
struct T { ~T() {}; };

int g ()
{
 foo:
  T t;
  int f(int);
 bar:
  T t2;
  int f(double);
  return f(3);
}


int f(int)
{
  return 0;
}


int f(double)
{
  return 1;
}


int main()
{
  return g();
}

