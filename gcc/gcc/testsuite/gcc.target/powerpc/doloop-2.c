/* Make sure the loop is recognized as doloops.
   If so, "bdnz" will be generated on ppc; if not,
   you will get "ble" or "blt" or "bge".  Also since
   this loop is known to iterate enough times, make sure
   the bdnz is marked for static branch prediction.  
   We will mispredict the branch 1 out of 100 times
   which is just as good as the hardware predictor will do.  */
/* { dg-do compile } */
/* { dg-options "-O2" } */
extern int x[];

int f(int l)
{
  int i;
  for(i = 0; i < 100; i++)
    x[i] = i;
}

/* { dg-final { scan-assembler "bdnz\\\+ " } } */
