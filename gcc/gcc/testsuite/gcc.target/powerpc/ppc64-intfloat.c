/* { dg-options "-O2 -mpowerpc64" } */
/* { dg-do compile { target ppu-*-* powerpc64-*-* } }*/

/* { dg-final { scan-assembler-not "bge" } } */


void Bad(float* dest, const int* src)
{
  *dest = (float)*src;
}
