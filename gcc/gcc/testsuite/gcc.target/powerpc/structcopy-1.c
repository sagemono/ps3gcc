/* { dg-do compile } */
/* { dg-options "-O2 -fno-tree-sra" } */
typedef struct _SceSoaVector4
{
  __vector float x, y, z;
  char a;
} SceSoaVector4;

SceSoaVector4
test_ref( __vector float x, __vector float y, __vector float z)
{
  SceSoaVector4 a = {x, y, z, 2};
  SceSoaVector4 b = a;
  return b;
}

/* We should be able to do a field by field copy for the SceSoaVector4 struct copy.
   Which means there should be no lvx to load the 2 from b.  */
/* { dg-final { scan-assembler-not "lvx" } } */
