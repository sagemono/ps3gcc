// { dg-do compile }
// { dg-options "-O2" }
// Even though this is not valid C++, this is a QOI case where
// we should inline the value of the notmes.
// { dg-final { scan-assembler-not "notme" } }
struct a
{
  static const int notme1 = 2;
  static const int notme2 = 2;
};


int f(int c)
{
  return c ? a::notme2 : a::notme1;
}

int main(void)
{
  return f(10);
}
