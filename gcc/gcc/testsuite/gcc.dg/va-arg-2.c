/* <varargs.h> is not supported anymore, but we do install a stub
   file which issues an #error telling the user to convert their code.  */

/* { dg-do compile } */

#include <varargs.h>  /* { dg-bogus "varargs.h" "missing file" { xfail spu-*-lv2 ppu-*-lv2 } } */

/* { dg-error "" "In file included from" { xfail spu-*-lv2 ppu-*-lv2 } 6 } */
/* { dg-error "no longer implements" "#error 1" { xfail spu-*-lv2 ppu-*-lv2 } 4 } */
/* { dg-error "Revise your code" "#error 2" { xfail spu-*-lv2 ppu-*-lv2 } 5 } */

int x;  /* prevent empty-source-file warning */
