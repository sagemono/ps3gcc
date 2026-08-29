/* { dg-do compile } */
/* { dg-options "-O2 -maltivec" } */
/* We should be able to produce all of these comparisions without a store.
   This tests, equals, not equals, less than, greather than, less than or equal, and greater than or equals.
   Types tested: vector float, vector signed int, vector unsigned int, vector signed short, vector unsigned short,
                 vector signed char, and vector unsigned char.   */
/* { dg-final { scan-assembler-not  "stvewx" } }*/

#include <altivec.h>


#define eq(type1, type) \
int eq##type1(type a, type b) \
{ \
  return a == b; \
}

#define lt(type1, type) \
int lt##type1(type a, type b) \
{ \
  return a < b; \
}

#define gt(type1, type) \
int gt##type1(type a, type b) \
{ \
  return a > b; \
}

#define ne(type1, type) \
int ne##type1(type a, type b) \
{ \
  return a != b; \
}

#define le(type1, type) \
int le##type1(type a, type b) \
{ \
  return a <= b; \
}

#define ge(type1, type) \
int ge##type1(type a, type b) \
{ \
  return a >= b; \
}

#define all(type1, type) \
eq(type1, type) \
ne(type1, type) \
lt(type1, type) \
le(type1, type) \
gt(type1, type) \
ge(type1, type)


all(float, vector float)

all(int, vector signed int)
all(uint, vector unsigned int)

all(short, vector signed short)
all(ushort, vector unsigned short)

all(char, vector signed char)
all(uchar, vector unsigned char)

