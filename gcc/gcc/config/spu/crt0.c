
/* (C) Copyright
 * Sony Computer Entertainment, Inc.,
 * Toshiba Corporation,
 * International Business Machines Corporation,
 * 2001,2002,2003,2004,2005,2006,2007,2008.
 *
 * This file is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * In addition to the permissions in the GNU General Public License, the
 * Free Software Foundation gives you unlimited permission to link the
 * compiled version of this file with other programs, and to distribute
 * those programs without any restriction coming from the use of this
 * file.  (The General Public License restrictions do apply in other
 * respects; for example, they cover modification of the file, and
 * distribution when not linked into another program.)
 *
 * This file is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this file; see the file COPYING.  If not, write to the Free
 * Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.  */

/* As a special exception, if you link this library with files compiled with
 * GCC to produce an executable, this does not cause the resulting executable
 * to be covered by the GNU General Public License.  The exception does not
 * however invalidate any other reasons why the executable file might be covered
 * by the GNU General Public License. */

/* bugzilla 31752
   Add codes to support SPURS Job model.
   All modifications related with SPURS job are enclosed with #if defined(_SPURS_TASK)
   and #endif. */
#include "spu_intrinsics.h"

#if defined (_RAW_SPU) || defined (_SPU_THREAD)
#include <sys/spu_thread.h>
extern int main(unsigned long long, unsigned long long, unsigned long long, unsigned long long);
void _start(unsigned long long, unsigned long long, unsigned long long, unsigned long long) __attribute__((naked));
#elif defined (_ISO_SPU)
extern int main(unsigned long long arg0,
		unsigned long long arg1,
		unsigned long long arg2,
		unsigned long long arg3,
		unsigned long long arg4,
		unsigned long long arg5,
		unsigned long long arg6,
		unsigned long long arg7,
		unsigned long long arg8,
		unsigned long long arg9,
		unsigned long long arg10,
		unsigned long long arg11,
		unsigned long long arg12,
		unsigned long long arg13,
		unsigned long long arg14,
		unsigned long long arg15,
		unsigned long long arg16,
		unsigned long long arg17,
		unsigned long long arg18,
		unsigned long long arg19);
void _start(unsigned long long arg0,
	    unsigned long long arg1,
	    unsigned long long arg2,
	    unsigned long long arg3,
	    unsigned long long arg4,
	    unsigned long long arg5,
	    unsigned long long arg6,
	    unsigned long long arg7,
	    unsigned long long arg8,
	    unsigned long long arg9,
	    unsigned long long arg10,
	    unsigned long long arg11,
	    unsigned long long arg12,
	    unsigned long long arg13,
	    unsigned long long arg14,
	    unsigned long long arg15,
	    unsigned long long arg16,
	    unsigned long long arg17,
	    unsigned long long arg18,
	    unsigned long long arg19) __attribute__((naked));
#elif defined(_SPURS_TASK)
#include <stdint.h>
void _start (qword arg0, uint64_t arg1) __attribute__ ((naked));
int cellSpursTaskMain(qword, uint64_t);
void cellSpursTaskExit(int) __attribute__ ((noreturn));
#else
#ifdef _STD_MAIN
extern int main(int,char*[]);
#else
extern int main(int, unsigned long long, unsigned long long);
#endif
void _start(int, unsigned long long, unsigned long long) __attribute__((naked));
#endif

extern void _init (void);
extern void _fini (void);

extern void exit(int);

void _exit(int) __attribute__((naked));

typedef void (*func_ptr) (void);

extern qword __stack;

extern char _end[];

#ifdef _STD_MAIN
char argv_buf[256] = "0000000000000000000000000000000000000000"
                     "0000000000000000000000000000000000000000"
                     "0000000000000000000000000000000000000000"
                     "0000000000000000000000000000000000000000"
                     "0000000000000000000000000000000000000000"
                     "0000000000000000000000000000000000000000"
                     "0000000000000000";
 int  argc = 1;
char *argv[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
#endif

#ifdef _STD_MAIN
#define NOT_USED_IN_STD_MAIN	__attribute__((unused))
#else
#define NOT_USED_IN_STD_MAIN
#endif

#if defined (_RAW_SPU) || defined (_SPU_THREAD)
/* According to the LV2 ABI an SPU thread is called with these
 * parameters.  */
void
_start(unsigned long long spu_id, unsigned long long p1, unsigned long long p2, unsigned long long p3)
#elif defined(_ISO_SPU)
void _start(unsigned long long arg0,
	    unsigned long long arg1,
	    unsigned long long arg2,
	    unsigned long long arg3,
	    unsigned long long arg4,
	    unsigned long long arg5,
	    unsigned long long arg6,
	    unsigned long long arg7,
	    unsigned long long arg8,
	    unsigned long long arg9,
	    unsigned long long arg10,
	    unsigned long long arg11,
	    unsigned long long arg12,
	    unsigned long long arg13,
	    unsigned long long arg14,
	    unsigned long long arg15,
	    unsigned long long arg16,
	    unsigned long long arg17,
	    unsigned long long arg18,
	    unsigned long long arg19)
#elif defined(_SPURS_TASK)
void
_start (qword arg0, uint64_t arg1)
#else
/* According to the BE Linux ABI an SPU module is called with these
 * parameters.  Also, $2 is set to the Available Stack Size.  */
void
_start(int spu_id NOT_USED_IN_STD_MAIN,
       unsigned long long param NOT_USED_IN_STD_MAIN,
       unsigned long long env NOT_USED_IN_STD_MAIN)
#endif
{
  register qword si_sp asm("$sp");

  qword stack_size;
  qword end;
  qword chain;

  /* Initialize the stack.  The ABI requires at least 2 slots, the back
   * chain and return pointer.  __stack typically points to the next
   * slot which contains 0. */
  chain = (qword)spu_splats((unsigned int)(&__stack-2));
  si_stqd(si_ai(chain,32), chain, 0);

  /* put 0 into the bottom of stack */
  qword *p = (qword *)&__stack;
  *p = (qword)(0);

  /* Initialize the Available Stack Size word of the Stack Pointer
   * information register.  Default to everything up to _end. */
  end = (qword)spu_splats((unsigned int)(_end));
  stack_size = si_sf(end, chain);
#if !defined (_RAW_SPU) && !defined (_SPU_THREAD) && !defined(_SPURS_TASK)
  /* The BE Linux ABI passes the stack size in $2, or use
     the default if $2 == 0. */
  {
    register qword si_r2 asm("$2");
    qword tmp = si_rotqbyi(si_r2, 12);
    stack_size = si_selb(tmp, stack_size, si_ceqi(tmp, 0));
  }
#endif
  si_sp = si_selb(chain, stack_size, si_fsmbi(0x0f00));


  _init();

#ifdef _STD_MAIN
  argv[0] = argv_buf;
  exit(main(argc, argv));
#elif defined (_RAW_SPU) || defined (_SPU_THREAD)
  exit(main(spu_id, p1, p2, p3));
#elif defined (_ISO_SPU)
  exit(main(arg0,  arg1,  arg2,  arg3,  arg4,
	    arg5,  arg6,  arg7,  arg8,  arg9,
	    arg10, arg11, arg12, arg13, arg14,
	    arg15, arg16, arg17, arg18, arg19));
#elif defined(_SPURS_TASK)
  exit(cellSpursTaskMain(arg0, arg1));
#else
  exit(main(spu_id, param, env));
#endif
  __builtin_si_stop(0);
}

/* C99 requires _Exit */
void _Exit(int) __attribute__((__weak__, __alias__("_exit")));

void
_exit(int rc)
{
  _fini();
#if defined (_SPU_THREAD)
  sys_spu_thread_exit(rc);
#elif defined (_SPURS_TASK)
  cellSpursTaskExit(rc);
#else
  /* Some self modifying code to return 'rc' in the 'stop' insn. */
  asm volatile (
    "	ori     $3, %0,0\n"
    "	lqr     $4, 1f\n"
    "	cbd     $5, 1f+3($sp)\n"
    "	shufb   $0, %0, $4, $5\n"
    "	stqr    $0, 1f\n"
    "	sync\n"
    "1:\n"
    "	stop    0x2000\n"
    : : "r" (rc) );
#endif
}

