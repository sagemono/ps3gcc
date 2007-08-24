/* A runtime check for AltiVec capability.  */
/* Contributed by Ziemowit Laski  <zlaski@apple.com>  */

#ifdef __CELLOS_LV2__

void altivec_check(void) {
}
#else

#include <signal.h>
extern void exit (int);
extern void abort (void);

void 
sig_ill_handler (int sig)
{
    exit (0);
}

void altivec_check(void) {

  /* Exit on systems without AltiVec.  */
  signal (SIGILL, sig_ill_handler);
#ifdef __MACH__
  asm volatile ("vor v0,v0,v0");
#else
  asm volatile ("vor 0,0,0");
#endif
  signal (SIGILL, SIG_DFL);
}
#endif
