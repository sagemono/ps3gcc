/* Test for multiple declarations and composite types.  Diagnosis of
   incompatible implicit declaration.  */
/* Origin: Joseph Myers <jsm@polyomino.org.uk> */
/* { dg-do compile } */
/* { dg-options "-std=c89" } */

void
f (void)
{
  long z(); /* { dg-error "previous implicit declaration" } */
}

void
g (void)
{
  z(); /* { dg-error "incompatible" } */
  /* CELL LOCAL Begin */
  /* Because of PR 75311, the warning
     incompatible implicit declaration of built-in function 'labs' and 'printf'
     are suppressed in the SCE port */
  /* CELL LOCAL End */
  labs(1); /* { dg-warning "incompatible" "" {xfail spu-*-* ppu-*-* } } */
  printf("x"); /* { dg-warning "incompatible" "" {xfail spu-*-* ppu-*-* } } */
}
