/* { dg-do compile } */
/* { dg-options "-Os -fdump-tree-final_cleanup" } */

float h(void);

static inline float divideadd(float a, float b, float c)
{
  return a / b + c;
}

float g(float a, float b, float c)
{
  return a + b +c + divideadd(b, c, a) + h();
}

float g1(float a, float b, float c)
{
  return a + b +c + divideadd(b, c, a);
}

/* We should have inlined the divideadd function at -Os as a divide should be the same size
   as an normal instruction.   */

/* { dg-final { scan-tree-dump-times "divideadd" 0 "final_cleanup"} } */

/* { dg-final { cleanup-tree-dump "final_cleanup" } } */

