
/* (C) Copyright
   Sony Computer Entertainment, Inc.,
   Toshiba Corporation,
   International Business Machines Corporation,
   2001,2002,2003,2004,2005.

   This file is free software; you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free
   Software Foundation; either version 2 of the License, or (at your option) 
   any later version.

   This file is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
   for more details.

   You should have received a copy of the GNU General Public License
   along with this file; see the file COPYING.  If not, write to the Free
   Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
   02110-1301, USA.  */

/* As a special exception, if you link this library with files compiled with 
   GCC to produce an executable, this does not cause the resulting executable 
   to be covered by the GNU General Public License.  The exception does not 
   however invalidate any other reasons why the executable file might be covered 
   by the GNU General Public License. */

/* We provide _exit() and abort() to make it obvious what is happening
 * when you run in mambo standalone mode and also to print out an exit
 * value for processing by a script.  The implementations in glibc lead
 * to unsupported syscalls. */

#define SimWriteConsoleCode 0
#define SimExitCode 31

void _exit (int x);
void abort (void);

__inline__ static int callthru0 (int command);
__inline__ static int callthru3 (int command, unsigned long arg1, unsigned long arg2, unsigned long arg3);
__inline__ static int MamboWriteConsoleN (const char *string, int n);
__inline__ static void MamboStopSimulation (void) __attribute__ ((noreturn));

__inline__ static int
callthru0 (int command)
{
  register int c asm ("r3") = command;
  asm volatile (".long 0x000EAEB0":"=r" (c):"r" (c));
  return ((c));
}
__inline__ static int
callthru3 (int command, unsigned long arg1, unsigned long arg2,
	   unsigned long arg3)
{
  register int c asm ("r3") = command;
  register unsigned long a1 asm ("r4") = arg1;
  register unsigned long a2 asm ("r5") = arg2;
  register unsigned long a3 asm ("r6") = arg3;
  asm volatile (".long 0x000EAEB0":"=r" (c):"r" (c), "r" (a1), "r" (a2),
		"r" (a3):"memory");
  return ((c));
}

#define CAST(t,e) ((t)(e))
__inline__ static int
MamboWriteConsoleN (const char *string, int n)
{
  return (callthru3
	  (SimWriteConsoleCode, CAST (unsigned long, string),
	   CAST (unsigned long, n), CAST (unsigned long, 1)));
}
__inline__ static void
MamboStopSimulation (void)
{
  (void)(callthru0 (SimExitCode));
}

void
_exit (int x)
{
  static char buf[] = "exit status = +0000000000\n";
  unsigned int n = (x < 0 ? -x : x);
  int i, j;
  fflush(0);
  i = sizeof (buf) - 2;
  do
    {
      buf[--i] = '0' + (n % 10);
      n /= 10;
    }
  while (n != 0);
  if (x < 0)
    buf[--i] = '-';
  for (j = 14; i < (int)sizeof (buf); j++, i++)
    buf[j] = buf[i];
  MamboWriteConsoleN (buf, sizeof (buf) - 1);
  MamboStopSimulation ();
}
void
abort (void)
{
  static char buf[] = "abort\n";
  fflush(0);
  MamboWriteConsoleN (buf, sizeof (buf) - 1);
  _exit (-1);
}

