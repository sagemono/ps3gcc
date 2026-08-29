// { dg-do run  }
// { dg-xfail-if "256K size limit" { "spu-*-*" } "*" "" }
#include<iostream>

int main() {
  try {
    throw 1;
  } catch(...) {
   try {
     throw;
   } catch(int) {
   }
   try {
     throw;
   } catch(int) {
   }
  }
  return 0;
}


