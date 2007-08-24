/* { dg-do run } */
/* { dg-options "-maltivec" } */
/* Test the vec_splats and vec_promote VMX intrinsics.  */
#include <altivec.h>
#include "altivec_check.h"

extern void abort (void);

vector int a[] = {{0, 0, 0, 0}, {1,0,0,0}, {1,2,0,0},{1,2,3,0},{1,2,3,4},{5,2,3,4},{5,6,3,4}};
vector int c = {0,6,3,4};
vector int d = {0,0,3,4};
int main1(int t)
{
  int i;
  vector int b = vec_splats(0);
  for(i = 0;i<sizeof(a)/sizeof(a[0])-1;i++)
    {
	if (b!=a[i])
	  abort ();
	b = vec_insert(i+1, b, i); 
    }
  if (b!=a[i])
    abort ();

  b = vec_insert(0, b, 0);
  if (b!=c)
    abort ();
  
  b = vec_insert(0, b, 1);
  if (b!=d)
    abort ();
  
  return 0;
}

int main(void)
{
  altivec_check();  /* Exits if AltiVec not supported */
  return main1 (0);
}
