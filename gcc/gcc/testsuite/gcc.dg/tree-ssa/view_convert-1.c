/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-final_cleanup" } */

/* We should be able to transform all of the following from address expressions
   to VIEW_CONVERT_EXPRs */

#define vector __attribute__((vector_size(16) ))
struct a
{
  vector float f;
};

#if 0
/* FIXME: we should be able to handle this but currently cannot.  */
struct a g1(vector float f)
{
  struct a t;
  *(vector float*)&t = f;
  return t;
}
#endif

vector signed int g3(vector float f)
{
  return *(vector signed int*)&f;
}

struct a g(vector float f)
{
  return *(struct a*)&f;
}

vector float g31(vector float a)
{
  struct a *b, c;
  vector float *d = &a;
  b = (struct a*)d;
  c = *b;
  return c.f;
}

vector float g2(vector float f)
{
  return ((struct a*)&f)->f;
}

/* { dg-final { scan-tree-dump-times "VIEW_CONVERT_EXPR" 4 "final_cleanup"} } */

/* { dg-final { cleanup-tree-dump "final_cleanup" } } */

