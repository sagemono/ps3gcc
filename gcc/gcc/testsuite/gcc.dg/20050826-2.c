/* { dg-do compile } */
/* Even though this might be expected to overflow the stack, it should
   not cause an ICE. */

void test (void)
{
  char test[0x7fffffff];
}

