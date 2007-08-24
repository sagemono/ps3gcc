/* { dg-do compile } */
typedef float v2sf __attribute__ ((vector_size (8)));
/* Apple AltiVec code supports this kind of cast in build_c_cast() */
v2sf sub (void) { return (v2sf) 0.0; } /* { dg-error "can't convert" "cast expr to vector" { xfail { "spu-*-*" "ppu-*-*" } } } */
