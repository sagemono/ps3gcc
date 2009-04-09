/* { dg-do compile } */
/* { dg-options "-O2" } */
union A
{
 float a;
 float a1;
};
float t(float a)
{
  union A a1, a2, a3;
  int i;
  a1.a = a;
  for(i =0;i<100;i++)
  {
  a2 = a1;
  a2.a += a;
  a1 = a2;
  }
  a3 = a1;
  return a3.a;
}
/*  The union A should get the same mode as float as it only can be accessed as SFmode.
    Check this by making sure there is no stw or std in the optimizated assembly.  */
/* { dg-final { scan-assembler-not "std" } } */
/* { dg-final { scan-assembler-not "stw" } } */
