/* { dg-do compile } */
/* { dg-options "-pedantic-errors -std=c99" } */

#define vector __attribute__((vector_size(16) ))

/* Even though we want to emit an error for compound-literals but we really don't
   vector compound-literals to emit an error as that is the only way to "construct"
   a vector. */
vector signed int x=(vector signed int){1,2,3,4};
