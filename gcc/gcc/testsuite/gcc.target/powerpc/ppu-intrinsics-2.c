#include <ppu_intrinsics.h>

unsigned a(void)
{
   unsigned *ea = (void*)0;
    unsigned old;
    do {
        old = __lwarx(ea);
    } while (0 == __stwcx(ea, old + 1));
    return old;
}


