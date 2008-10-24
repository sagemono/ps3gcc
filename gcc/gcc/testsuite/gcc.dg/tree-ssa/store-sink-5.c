/* { dg-do compile } */
/* { dg-options "-O2 -ftree-store-sink -fdump-tree-sink-details" } */

#define TMP 1000

int *ptr1;
int *ptr2;

void
foo (int m, int n, int tmp1, int tmp2)
{
  int i, j;

  for (i = 0; i < n; ++i)
    {
      for (j = 0; j < m; ++j)
	{
	  if (tmp2 > tmp1)

	    *ptr1++ = TMP;

	  else
	    {
	      if (tmp1 > tmp2)

		*ptr1++ = tmp1 = 0;

	      else

		*ptr1++ = tmp2 = tmp1 - TMP;

	    }
	}
    }
}

/* { dg-final { scan-tree-dump-times "going to perform sinking of this set of stores" 1 "sink" { xfail *-*-* } } } */
/* { dg-final { cleanup-tree-dump "sink" } } */


