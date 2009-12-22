/* { dg-do compile } */
/* { dg-options "-mno-toc=1" } */
#ifdef CELL_GCM_SNC_NOTOCRESTORE_2
typedef int macro_is_defined_p;
#endif
macro_is_defined_p x;  /* { dg-error "error: expected" "" } */



