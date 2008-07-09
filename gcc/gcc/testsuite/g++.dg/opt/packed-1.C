// { dg-options "-O2" }
// { dg-do run }

#pragma pack(1)
struct structa
{
  int *fielda;
};
#pragma pack()
struct structb
{
  void method1();
  unsigned long long m_uStartTime;
  int* fieldb;
};
structa *a;
void structb:: method1()
{
  a->fielda = fieldb;
}
int ad;
int main(void)
{
  structb b = {0, &ad};
  structa c;
  a = &c;
  b.method1();
  if (c.fielda != &ad)
    __builtin_abort ();
  return 0;
}
