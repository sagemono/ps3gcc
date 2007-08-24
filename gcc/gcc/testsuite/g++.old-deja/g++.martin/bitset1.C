// { dg-do run  }
// Origin: Jeff Donner <jdonner@schedsys.com>
// { dg-xfail-if "eh not supported on spu" { "spu-*-*" } "*" "" }
#include <bitset>

int main()
{
  std::bitset<sizeof(int) * 8> bufWord;

  bufWord[3] = 0;
}
