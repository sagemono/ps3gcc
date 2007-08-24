#ifndef LTO_INFO_H
#define LTO_INFO_H
/* Copyright (C) 2006 Sony Computer Entertainment, Inc.,

   LTO is free software; you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free
   Software Foundation; either version 2, or (at your option) any later
   version.

   LTO is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
   for more details.

   You should have received a copy of the GNU General Public License
   along with LTO; see the file COPYING.  If not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA.  */


typedef enum {

    ANN_ARG_NONE,
    ANN_ARG_U32,
    ANN_ARG_ADDR

} annotation_argtype_t;

enum { MAX_ANNOTATION_ARGS = 3 };

typedef enum {

#define LTO_INFO(tag,code,name,args) \
  tag = code,
#include "lto-info.def"
#undef LTO_INFO  

  LTO_COUNT

} annotation_kind_t;

#define LTO_CURRENT_VERSION 2

typedef union {
    unsigned u;
    struct {
	unsigned no_return   : 1;
	unsigned lto_version : 8;
	unsigned has_nonlocal_label : 1;
	unsigned UNUSED      : 22;
    } f;
} lto_fstart_flags_t;

typedef enum {
    LTO_ARGS_SPU = 1,
    LTO_ARGS_PPU = 2
} lto_args_kind_t;

typedef union {
    unsigned u;
    struct {
	unsigned no_return   : 1;
	unsigned sibcall     : 1;
	unsigned UNUSED1     : 6;
        unsigned args_kind   : 8;	/* lto_args_kind_t */
	unsigned UNUSED2     : 16;
    } f;
} lto_call_flags_t;

/*
// The struct lto_spuargs_t or lto_ppuargs_t are the two words after
//  the flags in the CALL annotation.  If the args_kind field of the
//  flags is LTO_ARGS_SPU, then use lto_spuargs_t; if it is LTO_ARGS_PPU,
//  then use lto_ppuargs_t.  In either case, the struct explains which
//  registers are used to pass arguments in the call and which registers
//  are used to return results.
// These structs are enclosed in unions to make it easier to convert
//  them to and from a 64-bit unsigned or a pair of 32-bit unsigneds.
//

//
// On the SPU, there is one register set, and arguments are passed in
//  a compact sequence starting with a register designated in the ABI.
//  Similarly, a result may be returned in one or more registers, again
//  a compact sequence starting with a register designated in the ABI.
//  So this struct specifies the starting register and number of registers
//  used, both for the argument list and for the result.  (In fact, the
//  SPU ABI says that the starting register is always r3 for both, but
//  this representation doesn't assume that.)
*/

typedef union {
    long long unsigned u;
    unsigned pair[2];
    struct {
	unsigned arg_firstreg    : 16;
	unsigned arg_nregs       : 16;
	unsigned result_firstreg : 16;
	unsigned result_nregs    : 16;
    } f;
} lto_spuargs_t;

typedef enum {
    LTO_RESULT_NONE = 0,
    LTO_RESULT_GPR  = 1,
    LTO_RESULT_FPR  = 2,
    LTO_RESULT_VEC  = 3
} lto_resultloc_t;

/*
// On the PPU, there are three register sets: general, floating-point,
//  and vector registers.  Arguments may be passed in any of them, and
//  for calls without prototypes, an argument may be passed in both an
//  fpr and some gprs or in both a vector register and some gprs.  With
//  a prototype, the gpr is allocated unused to an argument even if
//  the argument is passed in an fp or vector register, so the general
//  registers used to pass arguments might not be a compact sequence.
//  Fortunately the number of registers in each set used to pass arguments
//  is small enough that a bitmask can be used to represent it.  The
//  bitmasks are numbered from the least-significant bit, which represents
//  register 0 in that register set.  So, for example, gpr_argmask = 0x68
//  means that gprs r3, r5, and r6 are used to pass arguments, because
//  0x68 = (1<<3) | (1<<5) | (1<<6).
// On the PPU, results are returned only in a compact sequence in one of
//  the three register sets, so we represent this as the starting register
//  and number of registers used, together with a flag that tells which
//  register set is used.
*/

typedef union {
    long long unsigned u;
    unsigned pair[2];
    struct {
	unsigned gpr_argmask     : 16;
	unsigned fpr_argmask     : 16;
	unsigned vcr_argmask     : 16;
        unsigned result_loc      :  2;  /* lto_resultloc_t */
	unsigned result_firstreg :  7;  
	unsigned result_nregs    :  7;  
    } f;
} lto_ppuargs_t;

#endif /* LTO_INFO_H */
