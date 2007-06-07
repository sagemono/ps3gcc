// { dg-do run  }
// { dg-xfail-if "eh not supported on spu" { "spu-*-*" } "*" "" }
template <class T>
void f() throw (T)
{
  throw 7;
}


int main()
{
  try {
    f<int>();
  } catch (...) {
    return 0;
  }
}
