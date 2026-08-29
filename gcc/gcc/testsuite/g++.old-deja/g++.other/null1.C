// { dg-do link  }

#include <cstddef>

void g(int) {}
void g(long) {}
extern void g(void*);

template <int I>
void h() {}

void k(int) {}

template <class T>
void l(T);

template <>
void l(int) {}

template <>
void l(long) {}

int main()
{
  int i = NULL; // { dg-warning "" "" { xfail "spu-*-*" "ppu-*-lv2" } } converting NULL to non-pointer type
  float z = NULL; // { dg-warning "" "" { xfail "spu-*-*" "ppu-*-lv2" } } converting NULL to non-pointer type
  int a[2];

  i != NULL; // { dg-warning "" "" { xfail "spu-*-*" "ppu-*-lv2" } } NULL used in arithmetic
  NULL != z; // { dg-warning "" "" { xfail "spu-*-*" "ppu-*-lv2" } } NULL used in arithmetic
  k != NULL; // No warning: decay conversion
  NULL != a; // Likewise.
  -NULL;     // { dg-warning "" "" { xfail "spu-*-*" "ppu-*-lv2" } } converting NULL to non-pointer type
  +NULL;     // { dg-warning "" "" { xfail "spu-*-*" "ppu-*-lv2" } } converting NULL to non-pointer type
  ~NULL;     // { dg-warning "" "" { xfail "spu-*-*" "ppu-*-lv2" } } converting NULL to non-pointer type
  a[NULL] = 3; // { dg-warning "" "" { xfail "spu-*-*" "ppu-*-lv2" } } converting NULL to non-pointer-type
  i = NULL;  // { dg-warning "" "" { xfail "spu-*-*" "ppu-*-lv2" } } converting NULL to non-pointer type
  z = NULL;  // { dg-warning "" "" { xfail "spu-*-*" "ppu-*-lv2" } } converting NULL to non-pointer type
  k(NULL);   // { dg-warning "" "" { xfail "spu-*-*" "ppu-*-lv2" } } converting NULL to int
  g(NULL);   // { dg-warning "" "" { xfail "spu-*-*" "ppu-*-lv2" } } converting NULL to int
  h<NULL>(); // No warning: NULL bound to integer template parameter
  l(NULL);   // { dg-warning "" "" { xfail "spu-*-*" "ppu-*-lv2" } } converting NULL to int
  NULL && NULL; // No warning: converting NULL to bool is OK
}
