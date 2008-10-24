/* { dg-do compile } */
/* Test that the following code resulting in invalid assembly generates an error */
int f(void)
{
  asm("%r0"::"n"(14));/* { dg-error "invalid 'asm'" } */
}

