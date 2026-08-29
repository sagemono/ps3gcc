/* { dg-do compile } */
/* { dg-options "-O2" } */
/* Test that no more than 4 stvx, 1 stw and 2 stb instructions are generated
 * during the initialization of the two dimensional array */
#include <stdio.h>

int main()
{
 char str[2][34] = {"a","b"};
 printf("%s\n", str[0]);
 return 0;
}
/* { dg-final { scan-assembler-not "sth " } } */
/* { dg-final { scan-assembler-times "stvx" 4 } } */
/* { dg-final { scan-assembler-times "stw" 1 } } */
/* { dg-final { scan-assembler-times "stb" 2 } } */

