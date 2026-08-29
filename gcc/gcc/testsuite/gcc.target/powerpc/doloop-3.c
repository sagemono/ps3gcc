/* Make sure the loop is recognized as doloops.
   If so, "bdnz" will be generated on ppc; if not,
   you will get "ble" or "blt" or "bge".  Also since
   this loop is known to iterate enough times, make sure
   the bdnz is not marked for static branch prediction.  
   The branch cannot be predicted without knowning how
   the long the loop is (l), so we should not predict it
   for the Cell.  */
/* { dg-do compile } */
/* { dg-options "-O2 -mcpu=cell" } */
extern int x[];

int f(int l)
{
  int i;
  for(i = 0; i < l; i++)
    x[i] = i;
}

/* { dg-final { scan-assembler "bdnz " } } */
