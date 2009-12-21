/* SCE CONFIDENTIAL
$PSLibId$
* Copyright (C) 2008 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

/* (C) Copyright
 * Sony Computer Entertainment, Inc.,
 * Toshiba Corporation,
 * International Business Machines Corporation,
 * 2001,2002,2003,2004,2005,2006.
 *
 * This file is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
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

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <spu_printf.h>
#include <spu_intrinsics.h>
#include <spu_mfcio.h>

typedef struct CellSpursJobContext2 CellSpursJobContext2;
typedef struct CellSpursJob256 CellSpursJob256;

void _cellSpursJobCrtAuxInitialize(CellSpursJobContext2 *pContext, CellSpursJob256 *pJob256);
void _cellSpursJobCrtAuxFinalize(CellSpursJobContext2 *pContext, CellSpursJob256 *pJob256);

#define _ALWAYS_INLINE_  __attribute__((always_inline)) static inline

static unsigned int __fixup_start2[0] __attribute__((section(".fixup_head", "a"), aligned(4), used));
register unsigned int si_r126 asm ("$126");


/* ----------------------------------------------------------------------- */

/* FIXME!! dummy _exit to work around libc problem(bug#36457) */
void _exit(void);
void _exit(void) {
	spu_printf("\n_exit() has been called!!\n"
			   "_exit() is not implemented\n");
	  __asm__ volatile("stopd $0,$0,$0\n");
}


static void __fini_unfixups (void)
{
	unsigned int * fixup = __fixup_start2;
	__vector unsigned int sbase = spu_splats (si_r126);
	for ( ; *fixup; fixup++)
    {
		unsigned int info = *fixup;
		__vector unsigned int *addr =  
			(__vector unsigned int *)((si_r126 + info) & -16);
		*addr = *addr - (sbase & spu_maskw (info));
    }
}

/* ----------------------------------------------------------------------- */

void _init(void);
void _fini(void);
int __do_atexit(void);

extern void cellSpursJobMain2(CellSpursJobContext2 *, CellSpursJob256 *);
void __job_start(CellSpursJobContext2 *ctx, CellSpursJob256 *job);
void __job_start(CellSpursJobContext2 *ctx, CellSpursJob256 *job) {
	_cellSpursJobCrtAuxInitialize(ctx, job);
	_init();
	cellSpursJobMain2(ctx, job);

	__do_atexit();
	_fini();
	__fini_unfixups();
	_cellSpursJobCrtAuxFinalize(ctx, job);
}

/* ----------------------------------------------------------------------- */





/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */
