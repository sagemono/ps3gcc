/* { dg-do compile } */
/* { dg-options "-O2 -ftree-store-sink -fdump-tree-sink-details" } */

#define TMP 1000

int ptr1[100];

void
foo (int m, int n, int tmp1, int tmp2)
{
  int i, j;

  for (i = 0; i < n; ++i)
    {
      for (j = 0; j < m; ++j)
	{
	  if (tmp1 > tmp2)
	    ptr1[j] = tmp2 = 0;
	  else
	    ptr1[j] = tmp2 = tmp1 - TMP;

	}

    }

}
/* { dg-final { scan-tree-dump-times "going to perform sinking of this set of stores" 1 "sink" } } */
/* { dg-final { cleanup-tree-dump "sink" } } */


