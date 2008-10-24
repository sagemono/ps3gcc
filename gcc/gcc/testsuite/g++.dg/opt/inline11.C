// { dg-do compile }
// { dg-options "-O2" }

static inline int *fromSlotB(void)
{
  static int shuf_BZZZ = 1;
  return &shuf_BZZZ;
}

int *f(void)
{
  return fromSlotB();
}

// Even though the static variable in fromSlotB is still referenced,
// the compiler should remove fromSlotB.
// { dg-final { scan-assembler-not "_Z9fromSlotBv:" } }
