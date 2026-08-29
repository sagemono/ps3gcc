// { dg-do run { xfail sparc64-*-elf arm-*-pe } }
// { dg-xfail-if "eh not supported on spu" { "spu-*-*" } "*" "" }
// { dg-options "-fexceptions" }

int main () {
  try {
    try {
      throw 1;
    } catch ( char * ) {
    }
  } catch ( int ) {
    return 0;
  }
  return 1;
}
