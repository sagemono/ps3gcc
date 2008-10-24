/* { dg-do compile } */
/* { dg-options "-O2 -ftree-store-sink -fdump-tree-sink-details" } */

#define TMP 1000

int ptr1[100];
int ptr2[100];

void
foo (int m, int n, int tmp1, int tmp2)
{
  int i, j;

  for (i = 0; i < n; ++i)
    {
      for (j = 0; j < m; ++j)
	{
	  if (tmp2 > tmp1)
	    {
	      ptr1[j] = TMP;
	      ptr2[j] = tmp1;
	    }
	  else
	    {
	      if (tmp1 > tmp2)
		{
		  ptr1[j] = tmp1 = 0;
		  ptr2[j] = tmp2;
		}
	      else
		{
		  ptr1[j] = tmp2 = tmp1 - TMP;
		  ptr2[j] = tmp1;
		}
	    }

	}

    }

}
/* { dg-final { scan-tree-dump-times "going to perform sinking of this set of stores" 2 "sink" } } */
/* { dg-final { cleanup-tree-dump "sink" } } */


