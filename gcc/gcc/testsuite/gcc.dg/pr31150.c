/* { dg-do compile } */
/* { dg-options "-fdump-tree-gimple" } */

int main()
{
  char str[2][34] = {"a","b"};

  __builtin_puts(str[0]);

  return 0;
}

/* { dg-final { scan-tree-dump-times "97" 1 "gimple" } } */
/* { dg-final { scan-tree-dump-times "98" 1 "gimple" } } */
/* { dg-final { cleanup-tree-dump "gimple" } } */
