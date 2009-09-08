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

#include <cell/spurs/job_context.h>


typedef struct InitInfoHeader {
  uint32_t offset;
  uint8_t isCached;
  uint8_t pad[7];
  uint32_t base;
  char magic_str[16];
} __attribute__((aligned(16))) InitInfoHeader;

typedef struct SegmentInfo {
	uint32_t ptr;
	uint32_t size;
} SegmentInfo;

typedef struct RelocInfo {
	uint32_t ptr;
	uint32_t size;
} RelocInfo;

#define MEMORY_CHECK_MARKER 0xdeadbeef
#define MAGIC_STR_CMP "%SPURS JOB INFO%"
#define TERMINATE_WORD 0xffffffff

#define LIKELY(exp)		(__builtin_expect((exp),1))
#define UNLIKELY(exp)	(__builtin_expect((exp),0))
#define spu_testz(v)	(spu_extract(spu_orx((vec_uint4)(v)), 0) == 0)
#define spu_not(v)			spu_xor((v),-1)

typedef void (*func_ptr) (void);

func_ptr __CTOR_LIST__[1]
  __attribute__ ((section(".ctors"), aligned(4)))
  = { (func_ptr) (-1) };

static func_ptr __DTOR_LIST__[1]
  __attribute__((section(".dtors"), aligned(4)))
  = { (func_ptr) (-1) };

__attribute__((always_inline))
static inline
void __spu_memset(qword *s, qword c, size_t n)
{
	if (n == 0) return;

	qword t5, t6, t7;

	__asm__ volatile(
		"/* memset(%[s],%[c],%[n]) t[%[a],%[b],%[t]] */\n\
        .align  3\n\
.__spu_memset%=:\n\
        sfi     %[a],%[n],0x80;         hbrr    .br%=,.do%=;\n\
        ai      %[n],%[n],-0x80;        brsl    %[b],0f;\n\
0:      andi    %[a],%[a],0x70;\n\
        \n\
        rotmi   %[t],%[a],-2;\n\
        sf      %[s],%[a],%[s];\n\
        ai      %[b],%[b],1f-0b;\n\
        a       %[n],%[n],%[a];\n\
        a       %[b],%[b],%[t];\n\
        ori     %[t],%[s],0;\n\
                                        bi  %[b];\n\
/*------+-------+---------------|-------+-------+---------------|*/\n\
.do%=:  ai      %[n],%[n],-0x80;1:      stqd    %[c],0x00(%[t]);\n\
                                        stqd    %[c],0x10(%[s]);\n\
                                        stqd    %[c],0x20(%[s]);\n\
                                        stqd    %[c],0x30(%[s]);\n\
                                        stqd    %[c],0x40(%[s]);\n\
                                        stqd    %[c],0x50(%[s]);\n\
                                        stqd    %[c],0x60(%[s]);\n\
        ai      %[t],%[s],0x80;         stqd    %[c],0x70(%[s]);\n\
        ai      %[s],%[s],0x80; .br%=:  brnz    %[n],.do%=;"
		: [a]"=&r"(t5), [b]"=&r"(t6), [t]"=&r"(t7),
		  [s]"+r"(s), [n]"+r"(n)
		: [c]"r"(c)
		: "memory"
		);
}

static unsigned int get_PIC_base(void) {
  char *a0 ,*a1; 
  __asm__ ("ila\t%0,.+8\n"
           "\tbrsl\t%1,4"
           : "=r" (a0), "=r" (a1));

  return (uintptr_t)a1-(uintptr_t)a0;
}

#define ceil16(val) (((val)+15) & ~15)

extern char __bss_start[];
extern char _end[];
extern func_ptr __CTOR_END__[];
static vec_uint4 *_pMemCheckMarker[8];
static void _init(CellSpursJobContext2 *pContext, CellSpursJob256 *pJob256)
{
	InitInfoHeader *pInitInfoHeader = (InitInfoHeader *)(uintptr_t)_end;
	/* clear bss */
	{
		qword *s = (qword *)(uintptr_t)__bss_start;
		size_t n = (uintptr_t)_end-(uintptr_t)__bss_start;
		__spu_memset(s, (qword)((vec_uint4){0,0,0,0}), n);
	}
	vec_uchar16 magic_str = *((vec_uchar16 *)(uintptr_t)pInitInfoHeader->magic_str);
	unsigned offset = pInitInfoHeader->offset;
	unsigned isCached = pInitInfoHeader->isCached;
	if UNLIKELY(pJob256->header.jobType & CELL_SPURS_JOB_TYPE_MEMORY_CHECK) {
		/* Patch job descriptor to revert buffer size, and insert memory check marker */
		vec_uint4 bufSizes = *((vec_uint4 *)(uintptr_t)&pJob256->header.useInOutBuffer);
		bufSizes = spu_add(bufSizes, spu_and(spu_and(spu_cmpgt(bufSizes, 0u), spu_maskw(0x6)), (unsigned)-16));
		bufSizes = (vec_uint4)spu_add((vec_ushort8)bufSizes, spu_and(spu_and(spu_cmpgt((vec_ushort8)bufSizes, 0), spu_maskh(0x1)), (unsigned short)-1));
		*((vec_uint4 *)(uintptr_t)&pJob256->header.useInOutBuffer) = bufSizes;
		uint64_t *pCacheList = &pJob256->workArea.dmaList[pJob256->header.sizeDmaList/8];
		for(unsigned i = 0; i < pJob256->header.sizeCacheDmaList/8; i++,pCacheList++) {
			vec_uint4 cacheListMask = spu_maskw(1u<<(3-(((uintptr_t)pCacheList>>2)&3)));
			vec_uint4 cacheList = *((vec_uint4 *)(uintptr_t)pCacheList);
			cacheList = spu_sel(cacheList, spu_add(cacheList, -16), cacheListMask);
			*((vec_uint4 *)(uintptr_t)pCacheList) = cacheList;
			_pMemCheckMarker[i] = (vec_uint4 *)((uintptr_t)pContext->cacheBuffer[i] +
												ceil16(spu_extract(spu_rlqwbyte(cacheList,
																				(uintptr_t)pCacheList), 0)));
		}
		if (pJob256->header.sizeInOrInOut) {
			_pMemCheckMarker[4] = (vec_uint4 *)((uintptr_t)pContext->ioBuffer + ceil16(pJob256->header.sizeInOrInOut));
		}
		if (pJob256->header.sizeOut) {
			_pMemCheckMarker[5] = (vec_uint4 *)((uintptr_t)pContext->oBuffer + ceil16(pJob256->header.sizeOut));
		}
		register vec_uint4 sp;
		__asm__ volatile("ori %0,$1,0":"=r"(sp));
		unsigned sizeStack = spu_extract(sp, 1);
		uintptr_t stackPointer = spu_extract(sp, 0);
		_pMemCheckMarker[6] = (vec_uint4 *)(uintptr_t)(stackPointer + 48 - sizeStack - sizeof(vec_uint4));
		for(int i = 0; i < 8; i++) if (_pMemCheckMarker[i]||i==7) *_pMemCheckMarker[i] = spu_splats(MEMORY_CHECK_MARKER);
	}
	if (spu_testz(spu_not(spu_cmpeq(magic_str, *((vec_uchar16 *)(uintptr_t)MAGIC_STR_CMP))))) {
		/* this job binary is generated by spu_elf-to-ppu_obj */
		pInitInfoHeader->isCached = 1;
		uint32_t base = get_PIC_base();
		/* load RW segments */
		SegmentInfo *pSegmentInfo;
		unsigned hasRWsegment = 0;
		for(pSegmentInfo = (SegmentInfo *)(pInitInfoHeader+1);
			UNLIKELY(pSegmentInfo->ptr != TERMINATE_WORD);
			pSegmentInfo++)
		{
			if UNLIKELY(isCached) {
				hasRWsegment = 1;
				unsigned tsize  = 0;
				uintptr_t lsa = (uintptr_t)(base + pSegmentInfo->ptr);
				uint32_t  ea = (pJob256->header.eaBinary&~3) + pSegmentInfo->ptr - offset;
				for( unsigned size = pSegmentInfo->size; size; size -= tsize)
				{
					tsize = (size > 16*1024) ? 16*1024 : size;
					mfc_get((void *)lsa, ea, tsize, pContext->dmaTag, 0, 0);
					lsa += tsize; ea += tsize;
				}
			}
		}
		if UNLIKELY(hasRWsegment) {
			mfc_write_tag_mask(1 << pContext->dmaTag);
			mfc_read_tag_status_all();
		}

		/* fixup relocations */
		RelocInfo *pRelocInfo;
		for(pRelocInfo = (RelocInfo *)(pSegmentInfo+1); pRelocInfo->ptr != TERMINATE_WORD; pRelocInfo++) {
			for(unsigned i = 0; i < pRelocInfo->size/4; i++) {
				((uint32_t *)((uintptr_t)pRelocInfo->ptr+base))[i] += base;
			}
		}
	}

	/* call global constructors */
	{
		func_ptr *p;

		/* The compiler assumes all symbols are 16 byte aligned, which is
		 * not the case for __CTOR_END__.  This inline assembly makes sure
		 * the address is loaded into a register for which the compiler does
		 * not assume anything about alignment. */
		__asm__ ("\n" : "=r" (p) : "0" (__CTOR_END__ - 1));

		for (; *p != (func_ptr) -1; p--) {
			(*p) ();
		}
	}
}

		/* MACROS */
#define NATS	80	/* 64 + extra for fclose, xgetloc, locks, etc. */

		/* TYPES */
typedef void (**Ppvoidfn)(void);

extern void (*_Atfuns[NATS])(void);
extern size_t _Atcount;

static void _finalize(CellSpursJobContext2 *pContext, CellSpursJob256 *pJob256)
{
	if UNLIKELY(pJob256->header.jobType & CELL_SPURS_JOB_TYPE_MEMORY_CHECK) {
		/* buffer overrun check */
		unsigned memoryCheck = 0;
		for(int i = 0; i < 8; i++) {
			if (_pMemCheckMarker[i] || i == 7) {
				memoryCheck |= (spu_testz(spu_not(spu_cmpeq(*_pMemCheckMarker[i],
														   spu_splats(MEMORY_CHECK_MARKER)))) ? 0 : (1<<i));
			}
		}
		if UNLIKELY(memoryCheck) {
			spu_printf("Assert!! buffer overrun occured\n"
					   "\tMemory check mask = %#x\n"
					   "\teaJobDescriptor = %#llx\n", memoryCheck, pContext->eaJobDescriptor);
			__asm__ volatile("stopd $0,$0,$0\n");
		}
	}
	/* FIXME!! this should be moved to dinkum-spu for SPURS jobs */
	/* call static destructors */
	{	/* tidy up and exit to system */
		while (_Atcount < NATS)	/* normal stack */ {
			(_Atfuns[_Atcount++])();
		}
	}

	/* call global destructors */
	{
		static func_ptr *p = 0;
		if (!p)
		{
			/* See comment for __CTOR_END__ above. */
			__asm__ ("" : "=r" (p) : "0" (__DTOR_LIST__ + 1));
			for (; *p; p++)
				(*p) ();
		}
	}
	
}

/* FIXME!! dummy _exit to work around libc problem(bug#36457) */
void _exit(void);
void _exit(void) {
	spu_printf("\n_exit() has been called!!\n"
			   "_exit() is not implemented\n");
	__asm__ volatile("stopd $0,$0,$0\n");
}

extern void cellSpursJobMain2(CellSpursJobContext2 *, CellSpursJob256 *);
void __job_start(CellSpursJobContext2 *ctx, CellSpursJob256 *job);
void __job_start(CellSpursJobContext2 *ctx, CellSpursJob256 *job) {
	_init(ctx, job);
	cellSpursJobMain2(ctx, job);
	_finalize(ctx, job);
}

/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */
