/* When generating o32 MIPS PIC, main's $gp save slot was out of range
   of a single load instruction.  */
#ifdef STACK_SIZE
#define N (STACK_SIZE/16)
#else
#define N sizeof (int) >= 4 && sizeof (void *) >= 4 ? 0x4000 : 4
#endif
struct big { int i[N]; };
struct big gb;
int foo (struct big b, int x) { return b.i[x]; }
int main (void) { return foo (gb, 0) + foo (gb, 1); }
