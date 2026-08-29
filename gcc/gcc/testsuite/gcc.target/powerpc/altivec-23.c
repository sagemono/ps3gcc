/* { dg-do compile } */
/* { dg-xfail-if "" { "powerpc-*-eabispe*" "powerpc-ibm-aix*" } { "*" } { "" } } */
/* { dg-options "-O0 -maltivec" } */

#include <altivec.h>
typedef vector float vec4;
void grow(vec4 * a, float f) {
  vec4 fv = (vector float){f, f, f, f};
  a[1] = vec_sub(a[0], fv);
}
