
;; (C) Copyright
;; Sony Computer Entertainment, Inc.,
;; Toshiba Corporation,
;; International Business Machines Corporation,
;; 2001,2002,2003,2004,2005.

;; This file is free software; you can redistribute it and/or modify it under
;; the terms of the GNU General Public License as published by the Free
;; Software Foundation; either version 2 of the License, or (at your option) 
;; any later version.

;; This file is distributed in the hope that it will be useful, but WITHOUT
;; ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
;; FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
;; for more details.

;; You should have received a copy of the GNU General Public License
;; along with this file; see the file COPYING.  If not, write to the Free
;; Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
;; 02110-1301, USA.

;;- See file "rtl.def" for documentation on define_insn, match_*, et. al.


;; Define an insn type attribute.  This is used in function unit delay
;; computations.
;; multi0 is a multiple insn rtl whose first insn is in pipe0
;; multi1 is a multiple insn rtl whose first insn is in pipe1
(define_attr "type" "fx2,shuf,fx3,load,store,br,spr,lnop,nop,fxb,fp6,fp7,fpd,iprefetch,multi0,multi1,hbr,convert"
  (const_string "fx2"))

;; Length (in bytes).
; '(pc)' in the following doesn't include the instruction itself; it is 
; calculated as if the instruction had zero size.
(define_attr "length" ""
		(const_int 4))

;; Processor type -- this attribute must exactly match the processor_type
;; enumeration in spu.h.

(define_attr "cpu" "spu"
  (const (symbol_ref "spu_cpu_attr")))

; (define_function_unit NAME MULTIPLICITY SIMULTANEITY
;			TEST READY-DELAY ISSUE-DELAY [CONFLICT-LIST])

(define_cpu_unit "pipe0,pipe1,fp,ls")

(define_insn_reservation "NOP" 1 (eq_attr "type" "nop")
    "pipe0")

(define_insn_reservation "FX2" 2 (eq_attr "type" "fx2")
    "pipe0, nothing")

(define_insn_reservation "FX3" 4 (eq_attr "type" "fx3,fxb")
    "pipe0, nothing*3")

(define_insn_reservation "FP6" 6 (eq_attr "type" "fp6")
    "pipe0 + fp, nothing*5")

(define_insn_reservation "FP7" 7 (eq_attr "type" "fp7")
    "pipe0, fp, nothing*5")

;; The behaviour of the double precision is that both pipes stall
;; for 6 cycles and the the rest of the operation pipelines for
;; 7 cycles.  The simplest way to model this is to simply ignore
;; the 6 cyle stall.
(define_insn_reservation "FPD" 7 (eq_attr "type" "fpd")
    "pipe0 + pipe1, fp, nothing*5")

(define_insn_reservation "LNOP" 1 (eq_attr "type" "lnop")
    "pipe1")

(define_insn_reservation "STORE" 1 (eq_attr "type" "store")
    "pipe1 + ls")

(define_insn_reservation "IPREFETCH" 1 (eq_attr "type" "iprefetch")
    "pipe1 + ls")

(define_insn_reservation "SHUF" 4 (eq_attr "type" "shuf,br,spr")
    "pipe1, nothing*3")

(define_insn_reservation "LOAD" 6 (eq_attr "type" "load")
    "pipe1 + ls, nothing*5")

(define_insn_reservation "HBR" 18 (eq_attr "type" "hbr")
    "pipe1, nothing*15")

(define_insn_reservation "MULTI0" 4 (eq_attr "type" "multi0")
    "pipe0, nothing*3")

(define_insn_reservation "MULTI1" 4 (eq_attr "type" "multi1")
    "pipe1, nothing*3")

(define_insn_reservation "CONVERT" 0 (eq_attr "type" "convert")
    "nothing")


(define_constants [
 (UNSPEC_BLOCKAGE	0)
 (UNSPEC_IPREFETCH	1)
 (UNSPEC_FREST		2)
 (UNSPEC_FRSQEST	3)
 (UNSPEC_FI		4)
 (UNSPEC_EXTEND_CMP	5)
 (UNSPEC_CG		6)
 (UNSPEC_ADDX		7)
 (UNSPEC_SPU_CBX	8)
 (UNSPEC_SPU_CHX	9)
 (UNSPEC_SPU_CWX	10)
 (UNSPEC_SPU_CDX	11)
 (UNSPEC_SPU_ADDX	12)
 (UNSPEC_SPU_CG		13)
 (UNSPEC_SPU_CGX	14)
 (UNSPEC_SPU_SFX	15)
 (UNSPEC_SPU_BG		16)
 (UNSPEC_SPU_BGX	17)
 (UNSPEC_SPU_CLZ	18)
 (UNSPEC_SPU_CNTB	19)
 (UNSPEC_SPU_FSMB	20)
 (UNSPEC_SPU_FSMH	21)
 (UNSPEC_SPU_FSM	22)
 (UNSPEC_SPU_GBB	23)
 (UNSPEC_SPU_GBH	24)
 (UNSPEC_SPU_GB		25)
 (UNSPEC_SPU_AVGB	26)
 (UNSPEC_SPU_ABSDB	27)
 (UNSPEC_SPU_SUMB	28)
 (UNSPEC_SPU_SHUFB	29)
 (UNSPEC_SPU_SHLQBI	30)
 (UNSPEC_SPU_SHLQBY	31)
 (UNSPEC_SPU_SHLQBYBI	32)
 (UNSPEC_SPU_ROTQBY	33)
 (UNSPEC_SPU_ROTQBYBI	34)
 (UNSPEC_SPU_ROTQBI	35)
 (UNSPEC_SPU_ROTQMBY	36)
 (UNSPEC_SPU_ROTQMBYBI	37)
 (UNSPEC_SPU_ROTQMBI	38)
 (UNSPEC_SPU_HEQ	39)
 (UNSPEC_SPU_HGT	40)
 (UNSPEC_SPU_HLGT	41)
 (UNSPEC_SPU_HBR	42)
 (UNSPEC_SPU_HBRR	43)
 (UNSPEC_SPU_FREST	44)
 (UNSPEC_SPU_FRSQEST	45)
 (UNSPEC_SPU_FI		46)
 (UNSPEC_SPU_CSFLT	47)
 (UNSPEC_SPU_CFLTS	48)
 (UNSPEC_SPU_CUFLT	49)
 (UNSPEC_SPU_CFLTU	50)
 (UNSPEC_SPU_STOP	51)
 (UNSPEC_SPU_STOPD	52)
 (UNSPEC_SPU_LNOP	53)
 (UNSPEC_SPU_NOP	54)
 (UNSPEC_SPU_SYNC	55)
 (UNSPEC_SPU_SYNCC	56)
 (UNSPEC_SPU_DSYNC	57)
 (UNSPEC_SPU_FSCRRD	58)
 (UNSPEC_SPU_FSCRWR	59)
 (UNSPEC_SPU_MFSPR	60)
 (UNSPEC_SPU_MTSPR	61)
 (UNSPEC_SPU_RDCH	62)
 (UNSPEC_SPU_RCHCNT	63)
 (UNSPEC_SPU_WRCH	64)
 (UNSPEC_SPU_ALIGN_HINT	65)
 (UNSPEC_SPU_SHLH	66)
 (UNSPEC_SPU_SHL	67)
 (UNSPEC_SPU_ROTHM	68)
 (UNSPEC_SPU_ROTM	69)
 (UNSPEC_SPU_ROTMAH	70)
 (UNSPEC_SPU_ROTMA	71)
 (UNSPEC_SPU_IDISABLE	72)
 (UNSPEC_SPU_IENABLE	73)
 (UNSPEC_SPU_ORX	74)
 (UNSPEC_SPU_CONVERT	75)
])


;; move

(define_expand "movqi"
  [(set (match_operand:QI 0 "nonimmediate_operand" "=r,r,r,m")
        (match_operand:QI 1 "general_operand"       "r,i,m,r"))]
  ""
  "{ if (spu_expand_mov(operands, QImode))
  	DONE;
  }")


(define_expand "movhi"
  [(set (match_operand:HI 0 "nonimmediate_operand" "=r,r,r,m")
        (match_operand:HI 1 "general_operand"       "r,i,m,r"))]
  ""
  "{ if (spu_expand_mov(operands, HImode))
  	DONE;
  }")

(define_expand "movsi"
  [(set (match_operand:SI 0 "nonimmediate_operand" "=r,r,r,m")
        (match_operand:SI 1 "general_operand"       "r,i,m,r"))]
  ""
  "{
     if (TARGET_LARGE_MEM
     && GET_CODE(operands[0]) == REG
     && (GET_CODE(operands[1]) == CONST
	 || GET_CODE(operands[1]) == SYMBOL_REF
	 || GET_CODE(operands[1]) == LABEL_REF))
       {
	 enum machine_mode mode = GET_MODE (operands[0]);
	 rtx tem = ((reload_in_progress | reload_completed)
		    ? operands[0] : gen_reg_rtx (mode));

	 emit_insn (gen_high (tem, operands[1]));
	 emit_insn (gen_low (operands[0], tem, operands[1]));
	 DONE;
       }

    /* At least one of the operands needs to be a register. */
    if (spu_expand_mov(operands, SImode))
  	DONE;
  }")

(define_expand "movdi"
  [(set (match_operand:DI 0 "nonimmediate_operand" "=r,r,r,m")
        (match_operand:DI 1 "general_operand"       "r,i,m,r"))]
  ""
  "{ if (spu_expand_mov(operands, DImode))
  	DONE;
  }")


(define_expand "movsf"
  [(set (match_operand:SF 0 "nonimmediate_operand" "=r,r,r,m")
        (match_operand:SF 1 "general_operand"       "r,F,m,r"))]
  ""
  "{ if (spu_expand_mov(operands, SFmode))
  	DONE;
  }")

(define_expand "movdf"
  [(set (match_operand:DF 0 "nonimmediate_operand" "=r,r,m,r")
        (match_operand:DF 1 "general_operand"       "r,m,r,i"))]
  ""
  "{ if (spu_expand_mov(operands, DFmode))
  	DONE;
  }")

(define_insn "high"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(high:SI (match_operand:SI 1 "immediate_operand" "i")))]
  ""
  "ilhu\t%0,%1@h")

(define_insn "low"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(lo_sum:SI (match_operand:SI 1 "register_operand" "0")
		   (match_operand:SI 2 "immediate_operand" "i")))]
  ""
  "iohl\t%0,%2@l")

(define_insn "high_v4si"
  [(set (match_operand:V4SI 0 "register_operand" "=r")
	(high:V4SI (match_operand:V4SI 1 "immediate_operand" "i")))]
  "TARGET_LARGE_MEM"
  "ilhu\t%0,%1@h")

(define_insn "low_v4si"
  [(set (match_operand:V4SI 0 "register_operand" "=r")
	(lo_sum:V4SI (match_operand:V4SI 1 "register_operand" "0")
		     (match_operand:V4SI 2 "immediate_operand" "i")))]
  "TARGET_LARGE_MEM"
  "iohl\t%0,%2@l")

(define_insn "pic"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(match_operand:SI 1 "pic_address_operand" "s"))
   (use (const_int 0))]
  "flag_pic"
  "ila\t%0,%%pic(%1)")

;; Whenever a function generates the 'pic' pattern above we need to
;; load the pic_offset_table register.
;; GCC doesn't deal well with labels in the middle of a block so we
;; hardcode the offsets in the asm here.
(define_insn "load_pic_offset"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(unspec:SI [(const_int 0)] 0))
   (set (match_operand:SI 1 "register_operand" "=r")
	(unspec:SI [(const_int 0)] 0))]
  "flag_pic"
  "ila\t%1,.+8\;brsl\t%0,4"
  [(set_attr "length" "8")
   (set_attr "type" "multi0")])


(define_insn "movqi_internal"
  [(set (match_operand:QI 0 "nonimmediate_operand"  "=r,r,r,m")
        (match_operand:QI 1 "general_operand"       " r,i,m,r"))]
  "spu_valid_move(operands)"
  "* return spu_emit_move_asm(operands, QImode);"
  [(set_attr "type" "fx2,fx2,load,store")])

(define_insn "movhi_internal"
  [(set (match_operand:HI 0 "nonimmediate_operand"  "=r,r,r,m")
        (match_operand:HI 1 "general_operand"       " r,i,m,r"))]
  "spu_valid_move(operands)"
  "* return spu_emit_move_asm(operands, HImode);"
  [(set_attr "type" "fx2,fx2,load,store")])

(define_insn_and_split "movsi_internal"
  [(set (match_operand:SI 0 "nonimmediate_operand" "=r,r,r,m")
        (match_operand:SI 1 "move_operand"         " r,i,m,r"))]
  "!TARGET_LARGE_MEM && spu_valid_move(operands)"
  "* return spu_emit_move_asm(operands, SImode);"
  "reload_completed && !TARGET_DONT_SPLIT && !TARGET_LARGE_MEM"
  [(set (match_dup:SI 0)
        (match_dup:SI 1))]
  "if (spu_split_move(operands, SImode))
      DONE;"
  [(set_attr "type" "fx2,fx2,load,store")])

(define_insn_and_split "movsi_internal2"
  [(set (match_operand:SI 0 "nonimmediate_operand" "=r,r,r,m")
        (match_operand:SI 1 "move_operand"         " r,n,m,r"))]
  "TARGET_LARGE_MEM && spu_valid_move(operands)"
  "* return spu_emit_move_asm(operands, SImode);"
  "reload_completed && !TARGET_DONT_SPLIT && TARGET_LARGE_MEM"
  [(set (match_dup:SI 0)
        (match_dup:SI 1))]
  "if (spu_split_move(operands, SImode))
      DONE;"
  [(set_attr "type" "fx2,fx2,load,store")])

(define_insn_and_split "movdi_internal"
  [(set (match_operand:DI 0 "nonimmediate_operand"  "=r,r,r,m")
        (match_operand:DI 1 "general_operand"       " r,i,m,r"))]
  "spu_valid_move(operands)"
  "* return spu_emit_move_asm(operands, DImode);"
  "reload_completed && !TARGET_DONT_SPLIT"
  [(set (match_dup:DI 0)
        (match_dup:DI 1))]
  "if (spu_split_move(operands, DImode))
      DONE;"
  [(set_attr "type" "fx2,fx2,load,store")])

(define_insn_and_split "movsf_internal"
  [(set (match_operand:SF 0 "nonimmediate_operand"  "=r,r,r,m")
        (match_operand:SF 1 "general_operand"       " r,F,m,r"))]
  "spu_valid_move(operands)"
  "* return spu_emit_move_asm(operands, SFmode);"
  "reload_completed && !TARGET_DONT_SPLIT"
  [(set (match_dup:SF 0)
        (match_dup:SF 1))]
  "if (spu_split_move(operands, SFmode))
      DONE;"
  [(set_attr "type" "fx2,fx2,load,store")])

(define_insn "movdf_internal"
  [(set (match_operand:DF 0 "nonimmediate_operand" "=r,r,m,r")
        (match_operand:DF 1 "general_operand"      " r,m,r,i"))]
  "spu_valid_move(operands)"
  "* return spu_emit_move_asm(operands, DFmode);"
  [(set_attr "type" "fx2,load,store,fx2")])

(define_insn_and_split "load"
  [(set (match_operand 0 "register_operand" "=r")
        (match_operand 1 "memory_operand"    "m"))
   (clobber (match_operand:V16QI 2 "register_operand" "=&r"))
   (clobber (match_operand:SI    3 "register_operand" "=&r"))]
  "GET_MODE(operands[0]) == GET_MODE(operands[1])"
  "#"
  ""
  [(set (match_dup 0)
        (match_dup 1))]
  "{ spu_split_load(operands);
     DONE;
   }")

;; General case for loading from an unaligned address.
;;	lqd	$2,0($1)
;;	lqd	$3,16($1)
;;	andi	$4,$1,15
;;	sf	$5,$4,16
;;	shlqby	$2,$2,$4
;;	rotqmby	$3,$3,$5
;;	or	$0,$2,$3
(define_insn_and_split "uload"
  [(set (match_operand 0 "register_operand" "=r")
        (match_operand 1 "memory_operand"    "m"))
   (clobber (match_operand:V16QI 2 "register_operand" "=&r"))
   (clobber (match_operand:V16QI 3 "register_operand" "=&r"))
   (clobber (match_operand:SI    4 "register_operand" "=&r"))
   (clobber (match_operand:SI    5 "register_operand" "=&r"))
   (clobber (match_operand:SI    6 "register_operand" "=&r"))]
  "GET_MODE(operands[0]) == GET_MODE(operands[1])"
  "#"
  ""
  [(set (match_dup 0)
        (match_dup 1))]
  "{ spu_split_load(operands);
     DONE;
   }")

(define_insn_and_split "store"
  [(set (match_operand 0 "memory_operand"   "=m")
        (match_operand 1 "register_operand"  "r"))
   (clobber (match_operand:V16QI 2 "register_operand" "=&r"))
   (clobber (match_operand:V16QI 3 "register_operand" "=&r"))]
  "GET_MODE(operands[0]) == GET_MODE(operands[1])"
  "#"
  ""
  [(set (match_dup 0)
        (match_dup 1))]
  "{ spu_split_store(operands);
     DONE;
   }")

;; General case for store to an unaligned address.
;;	andi	$8,$0,15
;;	sf	$9,$8,16
;;	sf	$10,$8,0
;;	rotqby	$2,$1,$9
;;	lqd	$3,0($0)
;;	lqd	$4,16($0)
;;	fsmbi	$5,0xf000  -- mask depends on size of store
;;	rotqmby	$6,$5,$10
;;	shlqby	$7,$5,$9
;;	selb	$3,$3,$2,$6
;;	selb	$4,$4,$2,$7
;;	stqd	$3,0($0)
;;	stqd	$4,16($0)
(define_insn_and_split "ustore"
  [(set (match_operand 0 "memory_operand"   "=m")
        (match_operand 1 "register_operand"  "r"))
   (clobber (match_operand:V16QI 2 "register_operand" "=&r"))
   (clobber (match_operand:V16QI 3 "register_operand" "=&r"))
   (clobber (match_operand:V16QI 4 "register_operand" "=&r"))
   (clobber (match_operand:V16QI 5 "register_operand" "=&r"))
   (clobber (match_operand:V16QI 6 "register_operand" "=&r"))
   (clobber (match_operand:V16QI 7 "register_operand" "=&r"))
   (clobber (match_operand:SI    8 "register_operand" "=&r"))
   (clobber (match_operand:SI    9 "register_operand" "=&r"))
   (clobber (match_operand:SI    10 "register_operand" "=&r"))
   (clobber (match_operand:SI    11 "register_operand" "=&r"))]
  "GET_MODE(operands[0]) == GET_MODE(operands[1])"
  "#"
  ""
  [(set (match_dup 0)
        (match_dup 1))]
  "{ spu_split_store(operands);
     DONE;
   }")

;; integer conversions

(define_insn "extendqihi2"
  [(set (match_operand:HI 0 "register_operand"  "=r")
	(sign_extend:HI (match_operand:QI 1 "register_operand"   "r")))]
  ""
  "xsbh\t%0,%1")

(define_expand "extendqisi2"
  [(set (match_dup 2)
	(sign_extend:HI (match_operand:QI 1 "register_operand"   "r")))
   (set (match_operand:SI 0 "register_operand"  "=r")
	(sign_extend:SI (match_dup 2)))]
  ""
  "operands[2] = gen_reg_rtx(HImode);")

(define_insn "extendhisi2"
  [(set (match_operand:SI 0 "register_operand"  "=r")
	(sign_extend:SI (match_operand:HI 1 "register_operand"   "r")))]
  ""
  "xshw\t%0,%1")

(define_expand "extendqidi2"
  [(set (match_operand:DI 0 "register_operand"  "=r")
	(sign_extend:DI (match_operand:QI 1 "register_operand"   "r")))]
  ""
  "spu_expand_sign_extend(operands);
   DONE;")

(define_expand "extendhidi2"
  [(set (match_operand:DI 0 "register_operand"  "=r")
	(sign_extend:DI (match_operand:HI 1 "register_operand"   "r")))]
  ""
  "spu_expand_sign_extend(operands);
   DONE;")

(define_insn_and_split "extendsidi2"
  [(set (match_operand:DI 0 "register_operand"  "=r")
	(sign_extend:DI (match_operand:SI 1 "register_operand"   "r")))]
  ""
  "rotqby\t%0,%1,-4\;xswd\t%0,%0"
  "reload_completed && !TARGET_DONT_SPLIT"
  [(set (match_dup:DI 0)
	(sign_extend:DI (match_dup:SI 1)))]
  "{ rtx to = gen_rtx_REG(V16QImode, REGNO(operands[0]));
     rtx to_v4 = gen_rtx_REG(V4SImode, REGNO(operands[0]));
     rtx to_v2 = gen_rtx_REG(V2DImode, REGNO(operands[0]));
     rtx from = gen_rtx_REG(V16QImode, REGNO(operands[1]));
     spu_emit_insn(gen_spu_rotqby(to, from, GEN_INT(-4)));
     spu_emit_insn(gen_spu_xswd(to_v2, to_v4));
     DONE;
   }"
  [(set_attr "length" "8")])

(define_expand "extendqiti2"
  [(set (match_operand:TI 0 "register_operand"  "=r")
	(sign_extend:TI (match_operand:QI 1 "register_operand"   "r")))]
  ""
  "spu_expand_sign_extend(operands);
   DONE;")

(define_expand "extendhiti2"
  [(set (match_operand:TI 0 "register_operand"  "=r")
	(sign_extend:TI (match_operand:HI 1 "register_operand"   "r")))]
  ""
  "spu_expand_sign_extend(operands);
   DONE;")

(define_expand "extendsiti2"
  [(set (match_operand:TI 0 "register_operand"  "=r")
	(sign_extend:TI (match_operand:SI 1 "register_operand"   "r")))]
  ""
  "spu_expand_sign_extend(operands);
   DONE;")

(define_expand "extendditi2"
  [(set (match_operand:TI 0 "register_operand"  "=r")
	(sign_extend:TI (match_operand:DI 1 "register_operand"   "r")))]
  ""
  "spu_expand_sign_extend(operands);
   DONE;")

(define_insn "sign_extend_di"
  [(set (match_operand:DI 0 "register_operand"  "=r")
	(sign_extend:DI (match_operand 1 "register_operand"   "r")))
   (use (match_operand       2 "register_operand" "r"))
   (use (match_operand:V16QI 3 "register_operand" "r"))]
  ""
  "shufb\t%0,%1,%2,%3"
  [(set_attr "type" "shuf")])

(define_insn "sign_extend_ti"
  [(set (match_operand:TI 0 "register_operand"  "=r")
	(sign_extend:TI (match_operand 1 "register_operand"   "r")))
   (use (match_operand       2 "register_operand" "r"))
   (use (match_operand:V16QI 3 "register_operand" "r"))]
  ""
  "shufb\t%0,%1,%2,%3"
  [(set_attr "type" "shuf")])

(define_insn "zero_extendqihi2"
  [(set (match_operand:HI 0 "register_operand"  "=r")
	(zero_extend:HI (match_operand:QI 1 "register_operand"   "r")))]
  ""
  "andi\t%0,%1,0x00ff")

(define_insn "zero_extendqisi2"
  [(set (match_operand:SI 0 "register_operand"  "=r")
	(zero_extend:SI (match_operand:QI 1 "register_operand"   "r")))]
  ""
  "andi\t%0,%1,0x00ff")

(define_expand "zero_extendqidi2"
  [(set (match_dup 2)
	(zero_extend:SI (match_operand:QI 1 "register_operand"   "r")))
   (set (match_operand:DI 0 "register_operand"  "=r")
	(zero_extend:DI (match_dup 2)))]
  ""
  "operands[2] = gen_reg_rtx(SImode);")

(define_expand "zero_extendhisi2"
  [(set (match_operand:SI 0 "register_operand"  "=r")
	(zero_extend:SI (match_operand:HI 1 "register_operand"   "r")))]
  ""
  "{
     rtx mask = gen_reg_rtx(SImode);
     emit_insn(gen_movsi(mask, GEN_INT(0xffff)));
     emit_insn(gen_zero_extendhisi2_internal(operands[0], operands[1], mask));
     DONE;
   }")

(define_insn "zero_extendhisi2_internal"
  [(parallel
    [(set (match_operand:SI 0 "register_operand"  "=r")
	  (zero_extend:SI (match_operand:HI 1 "register_operand"   "r")))
     (use (match_operand:SI 2 "register_operand"  "r"))])]
  ""
  "and\t%0,%1,%2")

(define_expand "zero_extendhidi2"
  [(set (match_dup 2)
	(zero_extend:SI (match_operand:HI 1 "register_operand"   "r")))
   (set (match_operand:DI 0 "register_operand"  "=r")
	(zero_extend:DI (match_dup 2)))]
  ""
  "operands[2] = gen_reg_rtx(SImode);
   emit_insn(gen_zero_extendhisi2(operands[2], operands[1]));
   emit_insn(gen_zero_extendsidi2(operands[0], operands[2]));
   DONE;")

(define_insn "zero_extendsidi2"
  [(set (match_operand:DI 0 "register_operand"  "=r")
	(zero_extend:DI (match_operand:SI 1 "register_operand"   "r")))]
  ""
  "rotqmbyi\t%0,%1,-4"
  [(set_attr "type" "shuf")])


(define_insn "zero_extendqiti2"
  [(set (match_operand:TI 0 "register_operand"  "=r")
	(zero_extend:TI (match_operand:QI 1 "register_operand"   "r")))]
  ""
  "andi\t%0,%1,0xff\;rotqmbyi\t%0,%0,-12"
  [(set_attr "type" "multi0")
   (set_attr "length" "8")])

(define_insn "zero_extendhiti2"
  [(set (match_operand:TI 0 "register_operand"  "=r")
	(zero_extend:TI (match_operand:HI 1 "register_operand"   "r")))]
  ""
  "shlqbyi\t%0,%1,2\;rotqmbyi\t%0,%0,-14"
  [(set_attr "type" "multi1")
   (set_attr "length" "8")])

(define_insn "zero_extendsiti2"
  [(set (match_operand:TI 0 "register_operand"  "=r")
	(zero_extend:TI (match_operand:SI 1 "register_operand"   "r")))]
  ""
  "rotqmbyi\t%0,%1,-12"
  [(set_attr "type" "shuf")])

(define_insn "zero_extendditi2"
  [(set (match_operand:TI 0 "register_operand"  "=r")
	(zero_extend:TI (match_operand:DI 1 "register_operand"   "r")))]
  ""
  "rotqmbyi\t%0,%1,-8"
  [(set_attr "type" "shuf")])


;; (define_insn "trunchiqi2"
;;   [(set (match_operand:QI 0 "register_operand"  "=r")
;; 	(truncate:QI (match_operand:HI 1 "register_operand"   "r")))]
;;   ""
;;   "xsbh\t%0,%1"
;;   [(set_attr "type" "shuf")])
;; 
;; (define_insn "truncsiqi2"
;;   [(set (match_operand:QI 0 "register_operand"  "=r")
;; 	(truncate:QI (match_operand:SI 1 "register_operand"   "r")))]
;;   ""
;;   "xsbh\t%0,%1"
;;   [(set_attr "type" "shuf")])
;; 
;; (define_insn "truncsihi2"
;;   [(set (match_operand:HI 0 "register_operand"  "=r")
;; 	(truncate:HI (match_operand:SI 1 "register_operand"   "r")))]
;;   ""
;;   "xshw\t%0,%1"
;;   [(set_attr "type" "shuf")])

(define_insn "truncdiqi2"
  [(set (match_operand:QI 0 "register_operand"  "=r")
	(truncate:QI (match_operand:DI 1 "register_operand"   "r")))]
  ""
  "shlqbyi\t%0,%1,4"
  [(set_attr "type" "shuf")])

(define_insn "truncdihi2"
  [(set (match_operand:HI 0 "register_operand"  "=r")
	(truncate:HI (match_operand:DI 1 "register_operand"   "r")))]
  ""
  "shlqbyi\t%0,%1,4"
  [(set_attr "type" "shuf")])

(define_insn "truncdisi2"
  [(set (match_operand:SI 0 "register_operand"  "=r")
	(truncate:SI (match_operand:DI 1 "register_operand"   "r")))]
  ""
  "shlqbyi\t%0,%1,4"
  [(set_attr "type" "shuf")])

(define_insn "trunctiqi2"
  [(set (match_operand:QI 0 "register_operand"  "=r")
	(truncate:QI (match_operand:TI 1 "register_operand"   "r")))]
  ""
  "shlqbyi\t%0,%1,12"
  [(set_attr "type" "shuf")])

(define_insn "trunctihi2"
  [(set (match_operand:HI 0 "register_operand"  "=r")
	(truncate:HI (match_operand:TI 1 "register_operand"   "r")))]
  ""
  "shlqbyi\t%0,%1,12"
  [(set_attr "type" "shuf")])

(define_insn "trunctisi2"
  [(set (match_operand:SI 0 "register_operand"  "=r")
	(truncate:SI (match_operand:TI 1 "register_operand"   "r")))]
  ""
  "shlqbyi\t%0,%1,12"
  [(set_attr "type" "shuf")])

(define_insn "trunctidi2"
  [(set (match_operand:DI 0 "register_operand"  "=r")
	(truncate:DI (match_operand:TI 1 "register_operand"   "r")))]
  ""
  "shlqbyi\t%0,%1,8"
  [(set_attr "type" "shuf")])

(define_insn ""
  [(set (match_operand:SI 0 "register_operand"  "=r")
	(truncate:SI (ashiftrt:DI (match_operand:DI 1 "register_operand"   "r")
                                  (const_int 32))))]
  ""
  "ori\t%0,%1,0")

(define_insn ""
  [(set (match_operand:SI 0 "register_operand"  "=r")
	(truncate:SI (lshiftrt:DI (match_operand:DI 1 "register_operand"   "r")
                                  (const_int 32))))]
  ""
  "ori\t%0,%1,0")

;; Catch all the patterns that spu_extract generates in order to
;; avoid any right shifts of a TImode.
(define_insn_and_split ""
  [(set (match_operand:SI 0 "register_operand"  "=r")
	(truncate:SI (ashiftrt:DI (match_operand:DI 1 "register_operand"   "r")
                                  (match_operand:SI 2 "const_int_operand"  "i"))))]
  "operands"
  "#"
  "reload_completed"
  [(use (const_int 0))]
  "{ spu_split_trunc_shift_asm(operands, 0, 0);
     DONE;
   }")

(define_insn_and_split ""
  [(set (match_operand:SI 0 "register_operand"  "=r")
	(truncate:SI (lshiftrt:DI (match_operand:DI 1 "register_operand"   "r")
                                  (match_operand:SI 2 "const_int_operand"  "i"))))]
  "operands"
  "#"
  "reload_completed"
  [(use (const_int 0))]
  "{ spu_split_trunc_shift_asm(operands, 1, 0);
     DONE;
   }")

(define_insn_and_split ""
  [(set (match_operand:SI 0 "register_operand"  "=r")
	(truncate:SI (ashiftrt:TI (match_operand:TI 1 "register_operand"   "r")
                                  (match_operand:SI 2 "const_int_operand"  "i"))))]
  "operands"
  "#"
  "reload_completed"
  [(use (const_int 0))]
  "{ spu_split_trunc_shift_asm(operands, 0, 0);
     DONE;
   }")

(define_insn_and_split ""
  [(set (match_operand:SI 0 "register_operand"  "=r")
	(truncate:SI (lshiftrt:TI (match_operand:TI 1 "register_operand"   "r")
                                  (match_operand:SI 2 "const_int_operand"  "i"))))]
  "operands"
  "#"
  "reload_completed"
  [(use (const_int 0))]
  "{ spu_split_trunc_shift_asm(operands, 1, 0);
     DONE;
   }")

(define_insn_and_split ""
  [(set (match_operand 0 "register_operand"  "=r")
	(truncate (ashiftrt (ashift (match_operand    1 "register_operand"   "r")
                                    (match_operand:SI 2 "const_int_operand"  "i"))
			    (match_operand:SI 3 "const_int_operand"  "i"))))]
  "GET_MODE_BITSIZE(GET_MODE(operands[0])) <= (GET_MODE_BITSIZE(GET_MODE(operands[1])) - INTVAL(operands[3]))"
  "#"
  "reload_completed"
  [(use (const_int 0))]
  "{ spu_split_trunc_shift_asm(operands, 0, 1);
     DONE;
   }")

(define_insn_and_split ""
  [(set (match_operand 0 "register_operand"  "=r")
	(truncate (lshiftrt (ashift (match_operand    1 "register_operand"   "r")
                                    (match_operand:SI 2 "const_int_operand"  "i"))
			    (match_operand:SI 3 "const_int_operand"  "i"))))]
  "GET_MODE_BITSIZE(GET_MODE(operands[0])) <= (GET_MODE_BITSIZE(GET_MODE(operands[1])) - INTVAL(operands[3]))"
  "#"
  "reload_completed"
  [(use (const_int 0))]
  "{ spu_split_trunc_shift_asm(operands, 1, 1);
     DONE;
   }")


;; float conversions

(define_insn "floatsisf2"
  [(set (match_operand:SF 0 "register_operand"  "=r")
	(float:SF (match_operand:SI 1 "register_operand"   "r")))]
  ""
  "csflt\t%0,%1,0"
  [(set_attr "type" "fp7")])

(define_insn "fix_truncsfsi2"
  [(set (match_operand:SI 0 "register_operand"  "=r")
	(fix:SI (match_operand:SF 1 "register_operand"   "r")))]
  ""
  "cflts\t%0,%1,0"
  [(set_attr "type" "fp7")])

(define_insn "floatunssisf2"
  [(set (match_operand:SF 0 "register_operand"  "=r")
	(unsigned_float:SF (match_operand:SI 1 "register_operand"   "r")))]
  ""
  "cuflt\t%0,%1,0"
  [(set_attr "type" "fp7")])

(define_insn "fixuns_truncsfsi2"
  [(set (match_operand:SI 0 "register_operand"  "=r")
	(unsigned_fix:SI (match_operand:SF 1 "register_operand"   "r")))]
  ""
  "cfltu\t%0,%1,0"
  [(set_attr "type" "fp7")])

;;
;; Note that we use -0x80000000LL instead of 0x80000000 to make it fit
;; in the range of SImode. We do not care the upper bits
;;
(define_expand "floatsidf2"
  [(set (match_operand:DF 0 "register_operand"  "=r")
	(float:DF (match_operand:SI 1 "register_operand"   "r")))]
  ""
  "{
    rtx c1 = spu_float_const(\"2147483648\", DFmode);
    rtx r0 = gen_reg_rtx(SImode);
    rtx r1 = gen_reg_rtx(DFmode);
    rtx r2 = gen_reg_rtx(DFmode);
    emit_move_insn(r0, GEN_INT(-0x80000000LL));
    emit_move_insn(r1, c1);
    emit_insn(gen_xorsi3(r0,operands[1], r0));
    emit_insn(gen_floatunssidf2(r2,r0));
    emit_insn(gen_subdf3(operands[0], r2, r1));
    DONE;
  }")

(define_expand "floatunssidf2"
  [(set (match_operand:DF 0 "register_operand"  "=r")
	(unsigned_float:DF (match_operand:SI 1 "register_operand"   "r")))]
  ""
  "{
    rtx c0 = spu_const_from_ints(V16QImode, 0x02031011, 0x12138080, 0x06071415, 0x16178080);
    rtx r0 = gen_reg_rtx(V16QImode);
    emit_move_insn(r0, c0);
    emit_insn(gen_floatunssidf2_internal(operands[0], operands[1], r0));
    DONE;
  }")

(define_insn_and_split "floatunssidf2_internal"
  [(set (match_operand:DF 0 "register_operand"  "=r")
	(unsigned_float:DF (match_operand:SI 1 "register_operand"   "r")))
   (use (match_operand:V16QI 2 "register_operand" "r"))
   (clobber (match_scratch:V4SI 3 "=&r"))
   (clobber (match_scratch:V4SI 4 "=&r"))
   (clobber (match_scratch:V4SI 5 "=&r"))
   (clobber (match_scratch:V4SI 6 "=&r"))]
  ""
  "clz\t%3,%1\;il\t%6,1023+31\;shl\t%4,%1,%3\;ceqi\t%5,%3,32\;sf\t%6,%3,%6\;a\t%4,%4,%4\;andc\t%6,%6,%5\;shufb\t%6,%6,%4,%2\;shlqbii\t%0,%6,4"
  "reload_completed && !TARGET_DONT_SPLIT"
  [(set (match_dup:DF 0)
	(unsigned_float:DF (match_dup:SI 1)))]
 "{
    rtx *ops = operands;
    rtx op1_v4si = gen_rtx_REG(V4SImode, REGNO(ops[1]));
    rtx op0_v16qi = gen_rtx_REG(V16QImode, REGNO(ops[0]));
    rtx op4_v16qi = gen_rtx_REG(V16QImode, REGNO(ops[4]));
    rtx op6_v16qi = gen_rtx_REG(V16QImode, REGNO(ops[6]));
    spu_emit_insn(gen_spu_clz	(ops[3],op1_v4si));
    spu_emit_insn(gen_spu_il	(ops[6],GEN_INT(1023+31)));
    spu_emit_insn(gen_spu_shl	(ops[4],op1_v4si,ops[3]));
    spu_emit_insn(gen_spu_ceqi	(ops[5],ops[3],GEN_INT(32)));
    spu_emit_insn(gen_spu_sf	(ops[6],ops[6],ops[3]));
    spu_emit_insn(gen_spu_a 	(ops[4],ops[4],ops[4]));
    spu_emit_insn(gen_spu_andc	(ops[6],ops[6],ops[5]));
    spu_emit_insn(gen_spu_shufb	(op6_v16qi,op6_v16qi,op4_v16qi,ops[2]));
    spu_emit_insn(gen_spu_shlqbi	(op0_v16qi,op6_v16qi,GEN_INT(4)));
    DONE;
    }"
  [(set_attr "length" "32")])


;; Construct two exact doubles representing the high and low words,
;; then add them.
(define_expand "floatdidf2"
  [(set (match_operand:DF 0 "register_operand"  "=r")
	(float:DF (match_operand:DI 1 "register_operand"   "r")))]
  ""
  "{
    rtx c0 = GEN_INT(0x8000000000000000ull);
    rtx c1 = spu_float_const(\"9223372036854775808\", DFmode);
    rtx c2 = spu_const_from_ints(V16QImode, 0x02031011, 0x12138080, 0x06071415, 0x16178080);
    rtx c3 = spu_const_from_ints(V4SImode, 1023+63, 1023+31, 0, 0);
    rtx r0 = gen_reg_rtx(DImode);
    rtx r1 = gen_reg_rtx(DFmode);
    rtx r2 = gen_reg_rtx(V16QImode);
    rtx r3 = gen_reg_rtx(V4SImode);
    emit_move_insn(r0, c0);
    emit_move_insn(r1, c1);
    emit_move_insn(r2, c2);
    emit_move_insn(r3, c3);
    emit_insn(gen_xordi3(r0,operands[1], r0));
    emit_insn(gen_floatdidf2_internal(operands[0],r0,r1,r2,r3));
    DONE;
  }")

(define_insn_and_split "floatdidf2_internal"
  [(set (match_operand:DF 0 "register_operand"  "=r")
	(float:DF (match_operand:DI 1 "register_operand"   "r")))
   (use (match_operand:DF 2 "register_operand" "r"))
   (use (match_operand:V16QI 3 "register_operand" "r"))
   (use (match_operand:V4SI 4 "register_operand" "r"))
   (clobber (match_scratch:V4SI 5 "=&r"))
   (clobber (match_scratch:V4SI 6 "=&r"))
   (clobber (match_scratch:V4SI 7 "=&r"))]
  ""
  "clz\t%5,%1\;shl\t%6,%1,%5\;ceqi\t%7,%5,32\;sf\t%5,%5,%4\;a\t%6,%6,%6\;andc\t%5,%5,%7\;shufb\t%5,%5,%6,%3\;shlqbii\t%5,%5,4\;shlqbyi\t%6,%5,8\;dfs\t%5,%5,%2\;dfa\t%0,%5,%6"
  "reload_completed && !TARGET_DONT_SPLIT"
  [(set (match_operand:DF 0 "register_operand"  "=r")
	(float:DF (match_operand:DI 1 "register_operand"   "r")))]
  "{
    rtx *ops = operands;
    rtx op1_v4si = gen_rtx_REG(V4SImode, REGNO(ops[1]));
    rtx op5_v16qi = gen_rtx_REG(V16QImode, REGNO(ops[5]));
    rtx op6_v16qi = gen_rtx_REG(V16QImode, REGNO(ops[6]));
    rtx op5_df = gen_rtx_REG(DFmode, REGNO(ops[5]));
    rtx op6_df = gen_rtx_REG(DFmode, REGNO(ops[6]));
    spu_emit_insn(gen_spu_clz	(ops[5],op1_v4si));
    spu_emit_insn(gen_spu_shl	(ops[6],op1_v4si,ops[5]));
    spu_emit_insn(gen_spu_ceqi	(ops[7],ops[5],GEN_INT(32)));
    spu_emit_insn(gen_spu_sf	(ops[5],ops[4],ops[5]));
    spu_emit_insn(gen_spu_a 	(ops[6],ops[6],ops[6]));
    spu_emit_insn(gen_spu_andc	(ops[5],ops[5],ops[7]));
    spu_emit_insn(gen_spu_shufb	(op5_v16qi,op5_v16qi,op6_v16qi,ops[3]));
    spu_emit_insn(gen_spu_shlqbi	(op5_v16qi,op5_v16qi,GEN_INT(4)));
    spu_emit_insn(gen_spu_shlqby	(op6_v16qi,op5_v16qi,GEN_INT(8)));
    emit_insn(gen_subdf3	(op5_df,op5_df,ops[2]));
    emit_insn(gen_adddf3	(ops[0],op5_df,op6_df));
    DONE;
}"
  [(set_attr "length" "40")])

(define_expand "floatunsdidf2"
  [(set (match_operand:DF 0 "register_operand"  "=r")
	(unsigned_float:DF (match_operand:DI 1 "register_operand"   "r")))]
  ""
  "{
    rtx c0 = spu_const_from_ints(V16QImode, 0x02031011, 0x12138080, 0x06071415, 0x16178080);
    rtx c1 = spu_const_from_ints(V4SImode, 1023+63, 1023+31, 0, 0);
    rtx r0 = gen_reg_rtx(V16QImode);
    rtx r1 = gen_reg_rtx(V4SImode);
    emit_move_insn(r1, c1);
    emit_move_insn(r0, c0);
    emit_insn(gen_floatunsdidf2_internal(operands[0], operands[1], r0, r1));
    DONE;
  }")

(define_insn_and_split "floatunsdidf2_internal"
  [(set (match_operand:DF 0 "register_operand"  "=r")
	(unsigned_float:DF (match_operand:DI 1 "register_operand"   "r")))
   (use (match_operand:V16QI 2 "register_operand" "r"))
   (use (match_operand:V4SI 3 "register_operand" "r"))
   (clobber (match_scratch:V4SI 4 "=&r"))
   (clobber (match_scratch:V4SI 5 "=&r"))
   (clobber (match_scratch:V4SI 6 "=&r"))]
  ""
  "clz\t%4,%1\;shl\t%5,%1,%4\;ceqi\t%6,%4,32\;sf\t%4,%4,%3\;a\t%5,%5,%5\;andc\t%4,%4,%6\;shufb\t%4,%4,%5,%2\;shlqbii\t%4,%4,4\;shlqbyi\t%5,%4,8\;dfa\t%0,%4,%5"
  "reload_completed && !TARGET_DONT_SPLIT"
  [(set (match_operand:DF 0 "register_operand"  "=r")
	(unsigned_float:DF (match_operand:DI 1 "register_operand"   "r")))]
  "{
    rtx *ops = operands;
    rtx op1_v4si = gen_rtx_REG(V4SImode, REGNO(ops[1]));
    rtx op4_v16qi = gen_rtx_REG(V16QImode, REGNO(ops[4]));
    rtx op5_v16qi = gen_rtx_REG(V16QImode, REGNO(ops[5]));
    rtx op4_df = gen_rtx_REG(DFmode, REGNO(ops[4]));
    rtx op5_df = gen_rtx_REG(DFmode, REGNO(ops[5]));
    spu_emit_insn(gen_spu_clz	(ops[4],op1_v4si));
    spu_emit_insn(gen_spu_shl	(ops[5],op1_v4si,ops[4]));
    spu_emit_insn(gen_spu_ceqi	(ops[6],ops[4],GEN_INT(32)));
    spu_emit_insn(gen_spu_sf	(ops[4],ops[3],ops[4]));
    spu_emit_insn(gen_spu_a 	(ops[5],ops[5],ops[5]));
    spu_emit_insn(gen_spu_andc	(ops[4],ops[4],ops[6]));
    spu_emit_insn(gen_spu_shufb	(op4_v16qi,op4_v16qi,op5_v16qi,ops[2]));
    spu_emit_insn(gen_spu_shlqbi	(op4_v16qi,op4_v16qi,GEN_INT(4)));
    spu_emit_insn(gen_spu_shlqby	(op5_v16qi,op4_v16qi,GEN_INT(8)));
    emit_insn(gen_adddf3	(ops[0],op4_df,op5_df));
    DONE;
  }"
  [(set_attr "length" "40")])

(define_insn "extendsfdf2"
  [(set (match_operand:DF 0 "register_operand"  "=r")
        (float_extend:DF (match_operand:SF 1 "register_operand"   "r")))]
  ""
  "fesd\t%0,%1"
  [(set_attr "type" "fpd")])


(define_expand "spu_extendsfdf"
  [(set (match_operand:DF 0 "register_operand"  "=r")
        (float_extend:DF (match_operand:SF 1 "register_operand"   "r")))]
  ""
  "{
     spu_extendsfdf2(operands);
     DONE;
   }")


(define_insn "truncdfsf2"
  [(set (match_operand:SF 0 "register_operand"  "=r")
	(float_truncate:SF (match_operand:DF 1 "register_operand"   "r")))]
  ""
  "frds\t%0,%1"
  [(set_attr "type" "fpd")])

(define_expand "spu_truncdfsf"
  [(set (match_operand:SF 0 "register_operand"  "=r")
        (float_truncate:SF (match_operand:DF 1 "register_operand"   "r")))]
  ""
  "{
     spu_truncdfsf2(operands);
     DONE;
   }")


;; add/sub/neg

(define_insn "addhi3"
  [(set (match_operand:HI 0 "register_operand"  "=r,r")
	(plus:HI (match_operand:HI 1 "register_operand"  "r,r")
	         (match_operand:HI 2 "rK_operand"  "r,K")))]
  ""
  "@
  ah\t%0,%1,%2
  ahi\t%0,%1,%2")

(define_insn "subhi3"
  [(set (match_operand:HI 0 "register_operand"  "=r,r,r")
	(minus:HI (match_operand:HI 1 "rK_operand"  "r,K,r")
	          (match_operand:HI 2 "rK_operand"  "r,r,K")))]
  ""
  "@
  sfh\t%0,%2,%1
  sfhi\t%0,%2,%1
  ahi\t%0,%1,%n2")

(define_insn "neghi2"
  [(set (match_operand:HI 0 "register_operand"  "=r")
	(neg:HI (match_operand:HI 1 "register_operand"  "r")))]
  ""
  "sfhi\t%0,%1,0")

(define_insn "addsi3"
  [(set (match_operand:SI 0 "register_operand"  "=r,r")
	(plus:SI (match_operand:SI 1 "register_operand"  "r,r")
	         (match_operand:SI 2 "rK_operand"  "r,K")))]
  ""
  {
      return which_alternative==0 ? "a\t%0,%1,%2" : "ai\t%0,%1,%2";
  })

(define_insn "subsi3"
  [(set (match_operand:SI 0 "register_operand"  "=r,r")
	(minus:SI (match_operand:SI 1 "rK_operand"  "r,K")
	          (match_operand:SI 2 "register_operand"  "r,r")))]
  ""
  "@
  sf\t%0,%2,%1
  sfi\t%0,%2,%1")

(define_insn "negsi2"
  [(set (match_operand:SI 0 "register_operand"  "=r")
	(neg:SI (match_operand:SI 1 "register_operand"  "r")))]
  ""
  "sfi\t%0,%1,0")

;; operand 2 is a nonmemory because the compiler requires it.
;; See ../../regmove.c:~197
;; Also, because we can't tell the difference between an SImode
;; and a DImode const_int.
;;  $t = $a + $b
;;  cg    $t,$a,$b
;;  shufb $x,$x,$x,$p
;;  addx  $t,$t,$x
(define_expand "adddi3"
  [(set (match_operand:DI          0 "register_operand" "=&r")
	(plus:DI (match_operand:DI 1 "register_operand"  "r")
		 (match_operand:DI 2 "register_operand"  "r")))]
  ""
  "{
    rtx c0 = spu_const_from_ints(V16QImode, 0x04050607, 0x80808080, 0x0c0d0e0f, 0x80808080);
    rtx op3 = gen_reg_rtx(V16QImode);
    rtx op4 = gen_reg_rtx(V4SImode);
    emit_move_insn(op3, c0);
    emit_insn(gen_adddi3_internal(operands[0],
                                  operands[1],
                                  operands[2],
                                  op3,
                                  op4));
    DONE;
  }")

(define_insn_and_split "adddi3_internal"
  [(set (match_operand:DI          0 "register_operand" "=&r")
	(plus:DI (match_operand:DI 1 "register_operand"  "r")
	         (match_operand:DI 2 "register_operand"  "r")))
   (use (match_operand:V16QI 3 "register_operand" "r"))
   (clobber (match_operand:V4SI 4 "register_operand" "=&r"))]
  ""
  "#"
  "reload_completed"
  [(set (match_dup:DI           0)
	(minus:DI (match_dup:DI 1)
	          (match_dup:DI 2)))]
  "{
     rtx to = gen_rtx_REG(V4SImode, REGNO(operands[0]));
     rtx l = gen_rtx_REG(V4SImode, REGNO(operands[1]));
     rtx r = gen_rtx_REG(V4SImode, REGNO(operands[2]));
     rtx pat = operands[3];
     rtx cmp = operands[4];
     spu_emit_insn(gen_spu_cg(cmp, r, l));
     spu_emit_insn(gen_spu_shufb(to, cmp, cmp, pat));
     spu_emit_insn(gen_spu_addx(to, r, l, to));
     DONE;
   }"
  [(set_attr "length" "12")])

;;  $t = $b - $a
;;  clgt $x,$a,$b
;;  sf   $t,$b,$a
;;  shufb $x,$x,$x,$p
;;  a    $t,$t,$x
(define_expand "subdi3"
  [(set (match_operand:DI           0 "register_operand" "=&r")
	(minus:DI (match_operand:DI 1 "register_operand"  "r")
		  (match_operand:DI 2 "register_operand"  "r")))]
  ""
  "{
    rtx c0 = spu_const_from_ints(V16QImode, 0x04050607, 0x80808080, 0x0c0d0e0f, 0x80808080);
    rtx op3 = gen_reg_rtx(V16QImode);
    rtx op4 = gen_reg_rtx(V4SImode);
    emit_move_insn(op3, c0);
    emit_insn(gen_subdi3_internal(operands[0],
                                  operands[1],
                                  operands[2],
                                  op3,
                                  op4));
    DONE;
  }")

(define_insn_and_split "subdi3_internal"
  [(set (match_operand:DI           0 "register_operand" "=&r")
	(minus:DI (match_operand:DI 1 "register_operand"  "r")
	          (match_operand:DI 2 "register_operand"  "r")))
   (use (match_operand:V16QI 3 "register_operand" "r"))
   (clobber (match_operand:V4SI 4 "register_operand" "=&r"))]
  ""
  "#"
  "reload_completed"
  [(set (match_dup:DI           0)
	(minus:DI (match_dup:DI 1)
	          (match_dup:DI 2)))]
  "{
     rtx to = gen_rtx_REG(V4SImode, REGNO(operands[0]));
     rtx l = gen_rtx_REG(V4SImode, REGNO(operands[1]));
     rtx r = gen_rtx_REG(V4SImode, REGNO(operands[2]));
     rtx pat = operands[3];
     rtx cmp = operands[4];
     spu_emit_insn(gen_spu_clgt(cmp, r, l));
     spu_emit_insn(gen_spu_sf(to, l, r));
     spu_emit_insn(gen_spu_shufb(cmp, cmp, cmp, pat));
     spu_emit_insn(gen_spu_a(to, to, cmp));
     DONE;
   }"
  [(set_attr "length" "16")])

(define_expand "negdi2"
  [(set (match_operand:DI         0 "register_operand" "=r")
	(neg:DI (match_operand:DI 1 "register_operand"  "r")))]
  ""
  "{
    rtx zero = gen_reg_rtx(DImode);
    emit_move_insn(zero, GEN_INT(0));
    emit_insn(gen_subdi3(operands[0], zero, operands[1]));
    DONE;
  }")

;;
;; This is not the most efficient implementation of addti3.
;; We include this here because 1) the compiler needs it to be
;; defined as the word size is 128-bit and 2) sometimes gcc
;; substitutes an add for a constant left-shift. 2) is unlikely
;; because we also give addti3 a high cost. In case gcc does
;; generate TImode add, here is the code to do it.
;;
(define_insn "addti3"
  [(set (match_operand:TI 0 "register_operand"  "=r")
	(plus:TI (match_operand:TI 1 "register_operand"   "r")
	         (match_operand:TI 2 "nonmemory_operand"  "r")))
   (clobber (match_scratch:TI 3 "=&r"))
   (clobber (match_scratch:TI 4 "=&r"))
   (clobber (match_scratch:TI 5 "=&r"))]
  ""
  "cg %3,%1,%2\n\\
   a %4,%1,%2\n\\
   shlqbyi %5,%3,4\n\\
   cg %3,%4,%5\n\\
   a %4,%4,%5\n\\
   shlqbyi %5,%3,4\n\\
   cg %3,%4,%5\n\\
   shlqbyi %0,%3,4\n\\
   addx %0,%4,%5"
  [(set_attr "type" "multi0")
   (set_attr "length" "36")])

(define_insn "addsf3"
  [(set (match_operand:SF 0 "register_operand" "=r")
        (plus:SF (match_operand:SF 1 "register_operand" "r")
                 (match_operand:SF 2 "register_operand" "r")))]
  ""
  "fa\t%0,%1,%2"
  [(set_attr "type" "fp6")])

(define_insn "subsf3"
  [(set (match_operand:SF 0 "register_operand" "=r")
        (minus:SF (match_operand:SF 1 "register_operand" "r")
                  (match_operand:SF 2 "register_operand" "r")))]
  ""
  "fs\t%0,%1,%2"
  [(set_attr "type" "fp6")])

(define_insn_and_split "negsf2"
  [(set (match_operand:SF 0 "register_operand" "=r")
        (neg:SF (match_operand:SF 1 "register_operand" "r")))
   (clobber (match_scratch:SI 2 "=&r"))]
  ""
  "ilhu\t%2,0x8000\;xor\t%0,%1,%2"
  "reload_completed && !TARGET_DONT_SPLIT"
  [(set (match_dup:SF 0)
	(neg:SF (match_dup:SF 1)))
   (clobber (match_dup:SI 2))]
  "{
     rtx to = gen_rtx_REG(SImode, REGNO(operands[0]));
     rtx from = gen_rtx_REG(SImode, REGNO(operands[1]));
     emit_insn(gen_movsi(operands[2], GEN_INT(-1<<31)));
     emit_insn(gen_xorsi3(to, from, operands[2]));
     DONE;
   }"
  [(set_attr "length" "8")])

(define_insn_and_split "abssf2"
  [(set (match_operand:SF 0 "register_operand" "=r")
        (abs:SF (match_operand:SF 1 "register_operand" "r")))
   (clobber (match_scratch:SI 2 "=&r"))]
  ""
  "ilhu\t%2,0x8000\;andc\t%0,%1,%2"
  "reload_completed && !TARGET_DONT_SPLIT"
  [(set (match_dup:SF 0)
	(abs:SF (match_dup:SF 1)))
   (clobber (match_dup:SI 2))]
  "{
     rtx to = gen_rtx_REG(SImode, REGNO(operands[0]));
     rtx from = gen_rtx_REG(SImode, REGNO(operands[1]));
     emit_insn(gen_movsi(operands[2], GEN_INT(-1<<31)));
     spu_emit_insn(gen_spu_andc(to, from, operands[2]));
     DONE;
   }"
  [(set_attr "length" "8")])

(define_insn "adddf3"
  [(set (match_operand:DF 0 "register_operand" "=r")
        (plus:DF (match_operand:DF 1 "register_operand" "r")
                 (match_operand:DF 2 "register_operand" "r")))]
  ""
  "dfa\t%0,%1,%2"
  [(set_attr "type" "fpd")])

(define_insn "subdf3"
  [(set (match_operand:DF 0 "register_operand" "=r")
        (minus:DF (match_operand:DF 1 "register_operand" "r")
                  (match_operand:DF 2 "register_operand" "r")))]
  ""
  "dfs\t%0,%1,%2"
  [(set_attr "type" "fpd")])

(define_insn_and_split "negdf2"
  [(set (match_operand:DF 0 "register_operand" "=r")
        (neg:DF (match_operand:DF 1 "register_operand" "r")))
   (clobber (match_scratch:V16QI 2 "=&r"))]
  ""
  "fsmbi\t%2,0x8080\;andbi\t%2,%2,0x80\;xor\t%0,%1,%2"
  "reload_completed && !TARGET_DONT_SPLIT"
  [(set (match_dup:DF 0)
	(neg:DF (match_dup:DF 1)))
   (clobber (match_dup:V16QI 2))]
  "{
     rtx to = gen_rtx_REG(V16QImode, REGNO(operands[0]));
     rtx from = gen_rtx_REG(V16QImode, REGNO(operands[1]));
     spu_emit_insn(gen_spu_fsmb(operands[2], GEN_INT(0x8080)));
     spu_emit_insn(gen_spu_and(operands[2], operands[2], spu_const_vector(V16QImode, GEN_INT(0x80))));
     spu_emit_insn(gen_spu_xor(to, from, operands[2]));
     DONE;
   }"
  [(set_attr "length" "12")])


;; mul/div

(define_insn "mulhi3"
  [(set (match_operand:HI 0 "register_operand"  "=r,r")
	(mult:HI (match_operand:HI 1 "register_operand"  "r,r")
	         (match_operand:HI 2 "rK_operand"  "r,K")))]
  ""
  "@
  mpy\t%0,%1,%2
  mpyi\t%0,%1,%2"
  [(set_attr "type" "fp7")])

(define_expand "mulsi3"
  [(parallel
    [(set (match_operand:SI 0 "register_operand"  "=&r")
	  (mult:SI (match_operand:SI 1 "register_operand"  "r")
		   (match_operand:SI 2 "register_operand"  "r")))
     (clobber (match_operand:SI 3 "register_operand" "=&r"))
     (clobber (match_operand:SI 4 "register_operand" "=&r"))
     (clobber (match_operand:SI 5 "register_operand" "=&r"))
     (clobber (match_operand:SI 6 "register_operand" "=&r"))])]
 ""
 "{ operands[3] = gen_reg_rtx(SImode);
    operands[4] = gen_reg_rtx(SImode);
    operands[5] = gen_reg_rtx(SImode);
    operands[6] = gen_reg_rtx(SImode);
  }")

(define_insn_and_split "mulsi3_internal"
  [(set (match_operand:SI 0 "register_operand"  "=r")
	(mult:SI (match_operand:SI 1 "register_operand"  "r")
	         (match_operand:SI 2 "nonmemory_operand"  "ri")))
   (clobber (match_operand:SI 3 "register_operand" "=&r"))
   (clobber (match_operand:SI 4 "register_operand" "=&r"))
   (clobber (match_operand:SI 5 "register_operand" "=&r"))
   (clobber (match_operand:SI 6 "register_operand" "=&r"))]
  ""
  "#"
  ""
  [(set (match_dup:SI 0)
	(mult:SI (match_dup:SI 1)
	         (match_dup:SI 2)))]
  "{
	HOST_WIDE_INT val = 0;
	rtx a = operands[3];
	rtx b = operands[4];
	rtx c = operands[5];
	rtx d = operands[6];
	if (GET_CODE(operands[2]) == CONST_INT)
	  {
	    val = INTVAL(operands[2]);
	    emit_move_insn(d, operands[2]);
	    operands[2] = d;
	  }
	if (val && (val & 0xffff) == 0)
	  {
	    spu_emit_insn(gen_spu_mpyh_si(operands[0], operands[2], operands[1]));
	  }
	else if (val > 0 && val < 0x10000)
	  {
	    rtx cst = CONST_OK_FOR_LETTER_P(val,'K') ? GEN_INT(val) : d;
	    spu_emit_insn(gen_spu_mpyh_si(a, operands[1], operands[2]));
	    spu_emit_insn(gen_spu_mpyu_si(c, operands[1], cst));
	    emit_insn(gen_addsi3(operands[0], a, c));
	  }
	else
	  {
	    spu_emit_insn(gen_spu_mpyh_si(a, operands[1], operands[2]));
	    spu_emit_insn(gen_spu_mpyh_si(b, operands[2], operands[1]));
	    spu_emit_insn(gen_spu_mpyu_si(c, operands[1], operands[2]));
	    emit_insn(gen_addsi3(d, a, b));
	    emit_insn(gen_addsi3(operands[0], d, c));
	  }
	DONE;
   }")

(define_insn "mulhisi3"
  [(set (match_operand:SI 0 "register_operand"  "=r")
	(mult:SI (sign_extend:SI (match_operand:HI 1 "register_operand"  "r"))
	         (sign_extend:SI (match_operand:HI 2 "register_operand"  "r"))))]
  ""
  "mpy\t%0,%1,%2"
  [(set_attr "type" "fp7")])

(define_insn "mulhisi3_const"
  [(set (match_operand:SI 0 "register_operand"  "=r")
	(mult:SI (sign_extend:SI (match_operand:HI 1 "register_operand"  "r"))
	         (match_operand:SI 2 "immediate_operand_K"  "K")))]
  ""
  "mpyi\t%0,%1,%2"
  [(set_attr "type" "fp7")])

(define_insn "umulhisi3"
  [(set (match_operand:SI 0 "register_operand"  "=r")
	(mult:SI (zero_extend:SI (match_operand:HI 1 "register_operand" "r"))
	         (zero_extend:SI (match_operand:HI 2 "register_operand" "r"))))]
  ""
  "mpyu\t%0,%1,%2"
  [(set_attr "type" "fp7")])

(define_insn "umulhisi3_const"
  [(set (match_operand:SI 0 "register_operand"  "=r")
	(mult:SI (zero_extend:SI (match_operand:HI 1 "register_operand" "r"))
	         (match_operand:SI 2 "immediate_operand_K" "K")))]
  ""
  "mpyui\t%0,%1,%2"
  [(set_attr "type" "fp7")])

(define_expand "smulsi3_highpart"
  [(set (match_operand:SI 0 "register_operand"  "=r")
	(truncate:SI
	  (ashiftrt:DI
	    (mult:DI (sign_extend:DI (match_operand:SI 1 "register_operand" "r"))
	             (sign_extend:DI (match_operand:SI 2 "register_operand" "r")))
	    (const_int 32))))]
  ""
  "{
    rtx t0 = gen_reg_rtx(SImode);
    rtx t1 = gen_reg_rtx(SImode);
    rtx t2 = gen_reg_rtx(SImode);
    rtx t3 = gen_reg_rtx(SImode);
    rtx t4 = gen_reg_rtx(SImode);
    rtx t5 = gen_reg_rtx(SImode);
    rtx t6 = gen_reg_rtx(SImode);
    rtx t7 =  gen_reg_rtx(SImode);
    rtx t8 =  gen_reg_rtx(SImode);
    rtx t9 =  gen_reg_rtx(SImode);
    rtx t11 = gen_reg_rtx(SImode);
    rtx t12 = gen_reg_rtx(SImode);
    rtx t14 = gen_reg_rtx(SImode);
    rtx t15 = gen_reg_rtx(HImode);
    rtx t16 = gen_reg_rtx(HImode);
    rtx t17 = gen_reg_rtx(HImode);
    rtx t18 = gen_reg_rtx(HImode);
    rtx t19 = gen_reg_rtx(SImode);
    rtx t20 = gen_reg_rtx(SImode);
    rtx t21 = gen_reg_rtx(SImode);
    rtx op1_hi = gen_rtx_SUBREG(HImode, operands[1], 2);
    rtx op2_hi = gen_rtx_SUBREG(HImode, operands[2], 2);
    rtx t0_hi = gen_rtx_SUBREG(HImode, t0, 2);
    rtx t1_hi = gen_rtx_SUBREG(HImode, t1, 2);
  
    emit_insn(gen_lshrsi3(t0, operands[1], GEN_INT(16)));
    emit_insn(gen_lshrsi3(t1, operands[2], GEN_INT(16)));
    emit_insn(gen_umulhisi3(t2, op1_hi, op2_hi));
    spu_emit_insn(gen_spu_mpyh_si(t3, operands[1], operands[2]));
    spu_emit_insn(gen_spu_mpyh_si(t4, operands[2], operands[1]));
    spu_emit_insn(gen_spu_mpyhh_si(t5, operands[1], operands[2]));
    spu_emit_insn(gen_spu_mpys_si(t6, t0_hi, op2_hi));
    spu_emit_insn(gen_spu_mpys_si(t7, t1_hi, op1_hi));
  
    /* Gen carry bits (in t9 and t11). */
    emit_insn(gen_addsi3(t8, t2, t3));
    spu_emit_insn(gen_spu_cg_si(t9, t2, t3));
    spu_emit_insn(gen_spu_cg_si(t11, t8, t4));
    
    /* Gen high 32 bits in operand[0].  Correct for mpys. */
    spu_emit_insn(gen_spu_addx_si(t12, t5, t6, t9));
    spu_emit_insn(gen_spu_addx_si(t14, t12, t7, t11));
    
    /* mpys treats both operands as signed when we really want it to treat
     * the first operand as signed and the second operand as unsigned.
     * The code below corrects for that difference.  */
    emit_insn(gen_cgt_hi(t15, op1_hi, GEN_INT(-1)));
    emit_insn(gen_cgt_hi(t16, op2_hi, GEN_INT(-1)));
    spu_emit_insn(gen_spu_andc(t17, t1_hi, t15));
    spu_emit_insn(gen_spu_andc(t18, t0_hi, t16));
    emit_insn(gen_extendhisi2(t19, t17));
    emit_insn(gen_extendhisi2(t20, t18));
    emit_insn(gen_addsi3(t21, t19, t20));
    emit_insn(gen_addsi3(operands[0], t14, t21));
    DONE; }")

(define_expand "umulsi3_highpart"
  [(set (match_operand:SI 0 "register_operand"  "=r")
	(truncate:SI
	  (ashiftrt:DI
	    (mult:DI (zero_extend:DI (match_operand:SI 1 "register_operand" "r"))
	             (zero_extend:DI (match_operand:SI 2 "register_operand" "r")))
	    (const_int 32))))]
  ""
  "{
    rtx t0 = gen_reg_rtx(SImode);
    rtx t1 = gen_reg_rtx(SImode);
    rtx t2 = gen_reg_rtx(SImode);
    rtx t3 = gen_reg_rtx(SImode);
    rtx t4 = gen_reg_rtx(SImode);
    rtx t5 = gen_reg_rtx(SImode);
    rtx t6 = gen_reg_rtx(SImode);
    rtx t7 =  gen_reg_rtx(SImode);
    rtx t8 =  gen_reg_rtx(SImode);
    rtx t9 =  gen_reg_rtx(SImode);
    rtx t10 = gen_reg_rtx(SImode);
    rtx t12 = gen_reg_rtx(SImode);
    rtx t13 = gen_reg_rtx(SImode);
    rtx t14 = gen_reg_rtx(SImode);
    rtx op1_hi = gen_rtx_SUBREG(HImode, operands[1], 2);
    rtx op2_hi = gen_rtx_SUBREG(HImode, operands[2], 2);
    rtx t0_hi = gen_rtx_SUBREG(HImode, t0, 2);

    emit_insn(gen_rotlsi3(t0, operands[2], GEN_INT(16)));
    emit_insn(gen_umulhisi3(t1, op1_hi, op2_hi));
    emit_insn(gen_umulhisi3(t2, op1_hi, t0_hi));
    spu_emit_insn(gen_spu_mpyhhu_si(t3, operands[1], t0));
    spu_emit_insn(gen_spu_mpyhhu_si(t4, operands[1], operands[2]));
    emit_insn(gen_ashlsi3(t5, t2, GEN_INT(16)));
    emit_insn(gen_ashlsi3(t6, t3, GEN_INT(16)));
    emit_insn(gen_lshrsi3(t7, t2, GEN_INT(16)));
    emit_insn(gen_lshrsi3(t8, t3, GEN_INT(16)));

    /* Gen carry bits (in t10 and t12). */
    emit_insn(gen_addsi3(t9, t1, t5));
    spu_emit_insn(gen_spu_cg_si(t10, t1, t5));
    spu_emit_insn(gen_spu_cg_si(t12, t9, t6));

    /* Gen high 32 bits in operand[0]. */
    spu_emit_insn(gen_spu_addx_si(t13, t4, t7, t10));
    spu_emit_insn(gen_spu_addx_si(t14, t13, t8, t12));
    emit_insn(gen_movsi(operands[0], t14));

    DONE; }")

;; (define_insn "muldi3"
;;   [(set (match_operand:DI 0 "register_operand"  "=r")
;; 	(mult:DI (match_operand:DI 1 "register_operand"  "r")
;; 	         (match_operand:DI 2 "nonmemory_operand"  "r")))]
;;   ""
;;   "
;; il	$s0,0
;; il	$s1,0x1617
;; shufb	%3,$s0,%1,$s1
;; rotmi	%5,%1,-16
;; mpyu	%3,%3,%1
;; il	$s1,0x1617
;; shufb	%4,$s0,%1,$s1
;; mpyu	%4,%4,%5
;; il	%4,0x1415
;; shufb	%4,$s0,%1,$s1
;; mpyu	%3,%3,%1
;;    ")

;; Not necessarily the best implementation of divide but faster then
;; the default that gcc provides because this is inlined and it uses
;; clz.
(define_insn "divmodsi4"
      [(set (match_operand:SI 0 "register_operand"  "=&r")
            (div:SI (match_operand:SI 1 "register_operand"  "r")
                    (match_operand:SI 2 "register_operand"  "r")))
       (set (match_operand:SI 3 "register_operand"  "=&r")
            (mod:SI (match_dup 1)
                    (match_dup 2)))
       (clobber (match_scratch:SI 4 "=&r"))
       (clobber (match_scratch:SI 5 "=&r"))
       (clobber (match_scratch:SI 6 "=&r"))
       (clobber (match_scratch:SI 7 "=&r"))
       (clobber (match_scratch:SI 8 "=&r"))
       (clobber (match_scratch:SI 9 "=&r"))
       (clobber (match_scratch:SI 10 "=&r"))
       (clobber (match_scratch:SI 11 "=&r"))
       (clobber (match_scratch:SI 12 "=&r"))
       (clobber (reg:SI 130))]
  ""
  "heqi	%2,0\\n\\
	hbrr	3f,1f\\n\\
	sfi	%8,%1,0\\n\\
	sfi	%9,%2,0\\n\\
	cgti	%10,%1,-1\\n\\
	cgti	%11,%2,-1\\n\\
	selb	%8,%8,%1,%10\\n\\
	selb	%9,%9,%2,%11\\n\\
	clz	%4,%8\\n\\
	clz	%7,%9\\n\\
	il	%5,1\\n\\
	fsmbi	%0,0\\n\\
	sf	%7,%4,%7\\n\\
	shlqbyi	%3,%8,0\\n\\
	xor	%11,%10,%11\\n\\
	shl	%5,%5,%7\\n\\
	shl	%4,%9,%7\\n\\
	lnop	\\n\\
1:	or	%12,%0,%5\\n\\
	rotqmbii	%5,%5,-1\\n\\
	clgt	%6,%4,%3\\n\\
	lnop	\\n\\
	sf	%7,%4,%3\\n\\
	rotqmbii	%4,%4,-1\\n\\
	selb	%0,%12,%0,%6\\n\\
	lnop	\\n\\
	selb	%3,%7,%3,%6\\n\\
3:	brnz	%5,1b\\n\\
2:	sfi	%8,%3,0\\n\\
	sfi	%9,%0,0\\n\\
	selb	%3,%8,%3,%10\\n\\
	selb	%0,%0,%9,%11"
  [(set_attr "type" "multi0")
   (set_attr "length" "128")])

(define_insn "udivmodsi4"
      [(set (match_operand:SI 0 "register_operand"  "=&r")
            (udiv:SI (match_operand:SI 1 "register_operand"  "r")
                     (match_operand:SI 2 "register_operand"  "r")))
       (set (match_operand:SI 3 "register_operand"  "=&r")
            (umod:SI (match_dup 1)
                     (match_dup 2)))
       (clobber (match_scratch:SI 4 "=&r"))
       (clobber (match_scratch:SI 5 "=&r"))
       (clobber (match_scratch:SI 6 "=&r"))
       (clobber (match_scratch:SI 7 "=&r"))
       (clobber (match_scratch:SI 8 "=&r"))
       (clobber (reg:SI 130))]
  ""
  "heqi	%2,0\\n\\
	hbrr	3f,1f\\n\\
	clz	%7,%2\\n\\
	clz	%4,%1\\n\\
	il	%5,1\\n\\
	fsmbi	%0,0\\n\\
	sf	%7,%4,%7\\n\\
	ori	%3,%1,0\\n\\
	shl	%5,%5,%7\\n\\
	shl	%4,%2,%7\\n\\
1:	or	%8,%0,%5\\n\\
	rotqmbii	%5,%5,-1\\n\\
	clgt	%6,%4,%3\\n\\
	lnop	\\n\\
	sf	%7,%4,%3\\n\\
	rotqmbii	%4,%4,-1\\n\\
	selb	%0,%8,%0,%6\\n\\
	lnop	\\n\\
	selb	%3,%7,%3,%6\\n\\
3:	brnz	%5,1b\\n\\
2:"
  [(set_attr "type" "multi0")
   (set_attr "length" "80")])

(define_insn "mulsf3"
  [(set (match_operand:SF 0 "register_operand" "=r")
        (mult:SF (match_operand:SF 1 "register_operand" "r")
                 (match_operand:SF 2 "register_operand" "r")))]
  ""
  "fm\t%0,%1,%2"
  [(set_attr "type" "fp6")])

(define_insn "fma"
  [(set (match_operand:SF 0 "register_operand" "=r")
	(plus:SF (mult:SF (match_operand:SF 1 "register_operand" "r")
			  (match_operand:SF 2 "register_operand" "r"))
		 (match_operand:SF 3 "register_operand" "r")))]
  ""
  "fma\t%0,%1,%2,%3"
  [(set_attr "type"	"fp6")])

(define_insn "fnms"
  [(set (match_operand:SF 0 "register_operand" "=r")
        (minus:SF (match_operand:SF 3 "register_operand" "r")
                  (mult:SF (match_operand:SF 1 "register_operand" "r")
                           (match_operand:SF 2 "register_operand" "r"))))]
  ""
  "fnms\t%0,%1,%2,%3"
  [(set_attr "type" "fp6")])

(define_insn "fms"
  [(set (match_operand:SF 0 "register_operand" "=r")
        (minus:SF (mult:SF (match_operand:SF 1 "register_operand" "r")
                           (match_operand:SF 2 "register_operand" "r"))
                  (match_operand:SF 3 "register_operand" "r")))]
  ""
  "fms\t%0,%1,%2,%3"
  [(set_attr "type" "fp6")])


(define_insn_and_split "divsf3"
  [(set (match_operand:SF 0 "register_operand" "=r")
        (div:SF (match_operand:SF 1 "register_operand" "r")
                (match_operand:SF 2 "register_operand" "r")))
   (clobber (match_scratch:SF 3 "=&r"))
   (clobber (match_scratch:SF 4 "=&r"))]
  "spu_float_acc != SPU_FP_ACCURATE"
  "frest\t%3,%2\;fi\t%3,%2,%3\;fm\t%4,%1,%3\;fnms\t%0,%4,%2,%1\;fma\t%0,%0,%3,%4"
  "reload_completed"
  [(set (match_dup:SF 0)
        (div:SF (match_dup:SF 1)
                (match_dup:SF 2)))
   (clobber (match_dup:SF 3))
   (clobber (match_dup:SF 4))]
  "{
    emit_insn(gen_frest(operands[3], operands[2]));
    emit_insn(gen_fi(operands[3], operands[2], operands[3]));
    emit_insn(gen_mulsf3(operands[4], operands[1], operands[3]));
    emit_insn(gen_fnms(operands[0], operands[4], operands[2], operands[1]));
    emit_insn(gen_fma(operands[0], operands[0], operands[3], operands[4]));
    DONE;
  }")

(define_insn "muldf3"
  [(set (match_operand:DF 0 "register_operand" "=r")
        (mult:DF (match_operand:DF 1 "register_operand" "r")
                 (match_operand:DF 2 "register_operand" "r")))]
  ""
  "dfm\t%0,%1,%2"
  [(set_attr "type" "fpd")])

(define_insn "dfma"
  [(set (match_operand:DF 0 "register_operand" "=r")
	(plus:DF (mult:DF (match_operand:DF 1 "register_operand" "r")
		          (match_operand:DF 2 "register_operand" "r"))
		 (match_operand:DF 3 "register_operand" "0")))]
  ""
  "dfma\t%0,%1,%2"
  [(set_attr "type"	"fpd")])

(define_insn "dfnma"
  [(set (match_operand:DF 0 "register_operand" "=r")
	(neg:DF (plus:DF (mult:DF (match_operand:DF 1 "register_operand" "r")
			          (match_operand:DF 2 "register_operand" "r"))
		         (match_operand:DF 3 "register_operand" "0"))))]
  ""
  "dfnma\t%0,%1,%2"
  [(set_attr "type"	"fpd")])

(define_insn "dfnms"
  [(set (match_operand:DF 0 "register_operand" "=r")
        (minus:DF (match_operand:DF 3 "register_operand" "0")
                  (mult:DF (match_operand:DF 1 "register_operand" "r")
                           (match_operand:DF 2 "register_operand" "r"))))]
  ""
  "dfnms\t%0,%1,%2"
  [(set_attr "type" "fpd")])

(define_insn "dfms"
  [(set (match_operand:DF 0 "register_operand" "=r")
        (minus:DF (mult:DF (match_operand:DF 1 "register_operand" "r")
                           (match_operand:DF 2 "register_operand" "r"))
                  (match_operand:DF 3 "register_operand" "0")))]
  ""
  "dfms\t%0,%1,%2"
  [(set_attr "type" "fpd")])

;; Taken from STI's gcc
;; Does not correctly handle INF or NAN.
(define_expand "divdf3"
  [(set (match_operand:DF 0 "register_operand" "=r")
        (div:DF (match_operand:DF 1 "register_operand" "r")
                (match_operand:DF 2 "register_operand" "r")))]
  "(spu_double_acc == SPU_FP_COMPAT && flag_unsafe_math_optimizations) 
   || spu_double_acc == SPU_FP_FAST"
  "{
    /*
    double
    divdf3 (double x, double y)
    {
        float x0;
        float y_f = (float) y;
        double x1, x2;

        x0 = spu_extract(spu_re(spu_promote(y_f, 0)), 0);
        x1 = (double)(x0 * (2.0f - y_f * x0)); 
        x2 = x1 * (2.0 - y * x1);
        return (x * x2 * (2.0 - y * x2));
    }
    */
  
    rtx dst = operands[0];
    rtx x   = operands[1];
    rtx y   = operands[2];
    rtx y_f = gen_reg_rtx(SFmode);
    rtx x0_f = gen_reg_rtx(SFmode);
    rtx x1_f = gen_reg_rtx(SFmode);
    rtx x1 = gen_reg_rtx(DFmode);
    rtx x2 = gen_reg_rtx(DFmode);
    rtx t1_f = gen_reg_rtx(SFmode);
    rtx t1 = gen_reg_rtx(DFmode);
    rtx two = gen_reg_rtx(DFmode); 
    rtx two_f = gen_reg_rtx(SFmode);
  
    emit_insn (gen_truncdfsf2 (y_f, y));
    emit_insn (gen_frest (x0_f, y_f));
    emit_insn (gen_fi (x0_f, y_f, x0_f));
    emit_insn (gen_movsf (two_f, spu_float_const(\"2.0\",SFmode)));
    emit_insn (gen_fnms (t1_f, y_f, x0_f, two_f));
    emit_insn (gen_mulsf3 (x1_f, t1_f, x0_f));
    emit_insn (gen_extendsfdf2 (x1, x1_f));
    emit_insn (gen_extendsfdf2 (two, two_f));
    /* emit_insn (gen_shlqbyi_df (two, two_f, GEN_INT(12))); */
    /* emit_insn (gen_movdf (two, spu_float_const(\"2.0\",DFmode))); */
    emit_insn (gen_movdf (t1, two));
    emit_insn (gen_dfnms (t1, y, x1, t1));
    emit_insn (gen_muldf3 (x2, x1, t1));
    emit_insn (gen_dfnms (two, y, x2, two));
    emit_insn (gen_muldf3 (dst, x2, two));
    emit_insn (gen_muldf3 (dst, dst, x));
    DONE;
}")

;; sqrt
(define_expand "sqrtsf2"
  [(set (match_operand:SF 0 "register_operand" "=r")
        (sqrt:SF (match_operand:SF 1 "register_operand" "r")))]
  ""
  "{ /* The fast version is better for now.  The accurate version is completely
        wrong in some boundary cases. 
       if (spu_float_acc == SPU_FP_ACCURATE)
       {
	 rtx value, insns;
	 start_sequence ();
	 value = emit_library_call_value (sqrt_optab->handlers[SFmode].libfunc, NULL_RTX, LCT_NORMAL,
					 SFmode, 1, operands[1], SFmode);
	 insns = get_insns ();
	 end_sequence ();
	 emit_libcall_block (insns, operands[0], value,
			     gen_rtx_SQRT (SFmode, operands[1]));
       }
     else */
       emit_insn(gen_sqrtsf2_int(operands[0], operands[1]));
     DONE;
   }")

(define_insn_and_split "sqrtsf2_int"
  [(set (match_operand:SF 0 "register_operand" "=r")
        (sqrt:SF (match_operand:SF 1 "register_operand" "r")))
   (clobber (match_scratch:SF 2 "=&r"))
   (clobber (match_scratch:SF 3 "=&r"))
   (clobber (match_scratch:SF 4 "=&r"))
   (clobber (match_scratch:SF 5 "=&r"))]
  "spu_float_acc != SPU_FP_ACCURATE"
  "frsqest\t%2,%1\;ilhu\t%3,0x3f00\;ilhu\t%4,0x4040\;fm\t%3,%3,%1\;fi\t%2,%1,%2\;fm\t%5,%2,%2\;fm\t%2,%2,%3\;fnms\t%0,%1,%5,%4\;fm\t%0,%0,%2"
  "reload_completed"
  [(set (match_dup:SF 0)
        (sqrt:SF (match_dup:SF 1)))
   (clobber (match_dup:SF 2))
   (clobber (match_dup:SF 3))
   (clobber (match_dup:SF 4))
   (clobber (match_dup:SF 5))]
  "{
    if (spu_float_acc != SPU_FP_COMPAT)
      {
	emit_insn(gen_movsf(operands[3],spu_float_const(\"0.5\",SFmode)));
	emit_insn(gen_movsf(operands[4],spu_float_const(\"1.00000011920928955078125\",SFmode)));
	emit_insn(gen_frsqest(operands[2],operands[1]));
	emit_insn(gen_fi(operands[2],operands[1],operands[2]));
	emit_insn(gen_mulsf3(operands[5],operands[2],operands[1]));
	emit_insn(gen_mulsf3(operands[3],operands[5],operands[3]));
	emit_insn(gen_fnms(operands[4],operands[2],operands[5],operands[4]));
	emit_insn(gen_fma(operands[0],operands[4],operands[3],operands[5]));
      }
    else
      {
	emit_insn(gen_movsf(operands[3],spu_float_const(\"0.5\",SFmode)));
	emit_insn(gen_movsf(operands[4],spu_float_const(\"3.0\",SFmode)));
	emit_insn(gen_frsqest(operands[2],operands[1]));
	emit_insn(gen_fi(operands[2],operands[1],operands[2]));
	emit_insn(gen_mulsf3(operands[3],operands[3],operands[1]));
	emit_insn(gen_mulsf3(operands[5],operands[2],operands[2]));
	emit_insn(gen_mulsf3(operands[2],operands[2],operands[3]));
	emit_insn(gen_fnms(operands[0],operands[1],operands[5],operands[4]));
	emit_insn(gen_mulsf3(operands[0],operands[0],operands[2]));
      }
    DONE;
  }")

(define_insn "frest"
  [(set (match_operand:SF 0 "register_operand" "=r")
        (unspec:SF [(match_operand:SF 1 "register_operand" "r")] UNSPEC_FREST))]
  ""
  "frest\t%0,%1"
  [(set_attr "type" "shuf")])

(define_insn "frsqest"
  [(set (match_operand:SF 0 "register_operand" "=r")
        (unspec:SF [(match_operand:SF 1 "register_operand" "r")] UNSPEC_FRSQEST))]
  ""
  "frsqest\t%0,%1"
  [(set_attr "type" "shuf")])

(define_insn "fi"
  [(set (match_operand:SF 0 "register_operand" "=r")
        (unspec:SF [(match_operand:SF 1 "register_operand" "r")
	            (match_operand:SF 2 "register_operand" "r")] UNSPEC_FI))]
  ""
  "fi\t%0,%1,%2"
  [(set_attr "type" "fp7")])


;; logical and/or/xor/comp
(define_insn "andqi3"
  [(set (match_operand:QI 0 "register_operand"  "=r,r")
	(and:QI (match_operand:QI 1 "register_operand"   "r,r")
	        (match_operand:QI 2 "rK_operand"  "r,K")))]
  ""
  "@
  and\t%0,%1,%2
  andbi\t%0,%1,%2")

(define_insn "iorqi3"
  [(set (match_operand:QI 0 "register_operand"  "=r,r")
	(ior:QI (match_operand:QI 1 "register_operand"   "r,r")
	        (match_operand:QI 2 "rK_operand"  "r,K")))]
  ""
  "@
  or\t%0,%1,%2
  orbi\t%0,%1,%2")

(define_insn "xorqi3"
  [(set (match_operand:QI 0 "register_operand"  "=r,r")
	(xor:QI (match_operand:QI 1 "register_operand"   "r,r")
	        (match_operand:QI 2 "rK_operand"  "r,K")))]
  ""
  "@
  xor\t%0,%1,%2
  xorbi\t%0,%1,%2")

(define_insn "one_cmplqi2"
  [(set (match_operand:QI 0 "register_operand"  "=r")
	(not:QI (match_operand:QI 1 "register_operand"   "r")))]
  ""
  "nor\t%0,%1,%1")

(define_insn "andhi3"
  [(set (match_operand:HI 0 "register_operand"  "=r,r")
	(and:HI (match_operand:HI 1 "register_operand"   "r,r")
	        (match_operand:HI 2 "rK_operand"  "r,K")))]
  ""
  "@
  and\t%0,%1,%2
  andhi\t%0,%1,%2")

(define_insn "iorhi3"
  [(set (match_operand:HI 0 "register_operand"  "=r,r,r")
	(ior:HI (match_operand:HI 1 "register_operand"   "r,r,0")
	        (match_operand:HI 2 "rKN_operand"  "r,K,N")))]
  ""
  "@
  or\t%0,%1,%2
  orhi\t%0,%1,%2
  iohl\t%0,%2")

(define_insn "xorhi3"
  [(set (match_operand:HI 0 "register_operand"  "=r,r")
	(xor:HI (match_operand:HI 1 "register_operand"   "r,r")
	        (match_operand:HI 2 "rK_operand"  "r,K")))]
  ""
  "@
  xor\t%0,%1,%2
  xorhi\t%0,%1,%2")

(define_insn "one_cmplhi2"
  [(set (match_operand:HI 0 "register_operand"  "=r")
	(not:HI (match_operand:HI 1 "register_operand"   "r")))]
  ""
  "nor\t%0,%1,%1")

(define_insn "andsi3"
  [(set (match_operand:SI 0 "register_operand"  "=r,r")
	(and:SI (match_operand:SI 1 "register_operand"   "r,r")
	        (match_operand:SI 2 "rK_operand"  "r,K")))]
  ""
  "@
  and\t%0,%1,%2
  andi\t%0,%1,%2")

(define_insn "iorsi3"
  [(set (match_operand:SI 0 "register_operand"  "=r,r,r")
	(ior:SI (match_operand:SI 1 "register_operand"   "r,r,0")
	        (match_operand:SI 2 "rKN_operand"  "r,K,N")))]
  ""
  "@
  or\t%0,%1,%2
  ori\t%0,%1,%2
  iohl\t%0,%2")

(define_insn "xorsi3"
  [(set (match_operand:SI 0 "register_operand"  "=r,r")
	(xor:SI (match_operand:SI 1 "register_operand"   "r,r")
	        (match_operand:SI 2 "rK_operand"  "r,K")))]
  ""
  "@
  xor\t%0,%1,%2
  xori\t%0,%1,%2")

(define_insn "one_cmplsi2"
  [(set (match_operand:SI 0 "register_operand"  "=r")
	(not:SI (match_operand:SI 1 "register_operand"   "r")))]
  ""
  "nor\t%0,%1,%1")

(define_insn "anddi3"
  [(set (match_operand:DI 0 "register_operand"  "=r")
	(and:DI (match_operand:DI 1 "register_operand"  "r")
	        (match_operand:DI 2 "register_operand"  "r")))]
  ""
  "and\t%0,%1,%2")

(define_insn "iordi3"
  [(set (match_operand:DI 0 "register_operand"  "=r")
	(ior:DI (match_operand:DI 1 "register_operand"  "r")
	        (match_operand:DI 2 "register_operand"  "r")))]
  ""
  "or\t%0,%1,%2")

(define_insn "xordi3"
  [(set (match_operand:DI 0 "register_operand"  "=r")
	(xor:DI (match_operand:DI 1 "register_operand"  "r")
	        (match_operand:DI 2 "register_operand"  "r")))]
  ""
  "xor\t%0,%1,%2")

(define_insn "one_cmpldi2"
  [(set (match_operand:DI 0 "register_operand"  "=r")
	(not:DI (match_operand:DI 1 "register_operand"   "r")))]
  ""
  "nor\t%0,%1,%1")

(define_insn "andti3"
  [(set (match_operand:TI 0 "register_operand"  "=r")
	(and:TI (match_operand:TI 1 "register_operand"   "r")
	        (match_operand:TI 2 "register_operand"  "r")))]
  ""
  "and\t%0,%1,%2")

(define_insn "iorti3"
  [(set (match_operand:TI 0 "register_operand"  "=r")
	(ior:TI (match_operand:TI 1 "register_operand"   "r")
	        (match_operand:TI 2 "register_operand"  "r")))]
  ""
  "or\t%0,%1,%2")

(define_insn "xorti3"
  [(set (match_operand:TI 0 "register_operand"  "=r")
	(xor:TI (match_operand:TI 1 "register_operand"   "r")
	        (match_operand:TI 2 "register_operand"  "r")))]
  ""
  "xor\t%0,%1,%2")

(define_insn "one_cmplti2"
  [(set (match_operand:TI 0 "register_operand"  "=r")
	(not:TI (match_operand:TI 1 "register_operand"   "r")))]
  ""
  "nor\t%0,%1,%1")


;; shift & rotate insns

(define_insn "ashlhi3"
  [(set (match_operand:HI 0 "register_operand"  "=r,r")
	(ashift:HI (match_operand:HI 1 "register_operand"   "r,r")
	           (match_operand:HI 2 "nonmemory_operand"  "r,IJ")))]
  ""
  "@
  shlh\t%0,%1,%2
  shlhi\t%0,%1,%2"
  [(set_attr "type" "fx3")])

(define_insn "lshrhi3"
  [(set (match_operand:HI              0 "register_operand"  "=&r,r")
        (lshiftrt:HI (match_operand:HI 1 "register_operand"   "r,r")
                     (match_operand:HI 2 "nonmemory_operand"  "r,IJ")))]
  ""
  "@
   sfhi\t%0,%2,0\;rothm\t%0,%1,%0
   rothmi\t%0,%1,%n2"
  [(set_attr "type" "fx3")])

(define_split ;; "lshrhi3"
  [(set (match_operand:HI              0 "register_operand"  "")
        (lshiftrt:HI (match_operand:HI 1 "register_operand"  "")
                     (match_operand:HI 2 "register_operand"  "")))]
  "reload_completed && !TARGET_DONT_SPLIT"
  [(set (match_dup:HI 0)
        (lshiftrt:HI (match_dup:HI 1)
                     (match_dup:HI 2)))]
  "{
     rtx to = gen_rtx_REG(V8HImode, REGNO(operands[0]));
     rtx l = gen_rtx_REG(V8HImode, REGNO(operands[1]));
     rtx r = gen_rtx_REG(V8HImode, REGNO(operands[2]));
     spu_emit_insn(gen_spu_sfh(to, CONST0_RTX(V8HImode), r));
     spu_emit_insn(gen_spu_rotm(to, l, to));
     DONE;
   }")

(define_insn "ashrhi3"
  [(set (match_operand:HI              0 "register_operand"  "=&r,r")
        (ashiftrt:HI (match_operand:HI 1 "register_operand"   "r,r")
                     (match_operand:HI 2 "nonmemory_operand"  "r,IJ")))]
  ""
  "@
   sfhi\t%0,%2,0\;rotmah\t%0,%1,%0
   rotmahi\t%0,%1,%n2"
  [(set_attr "type" "fx3")])

(define_split ;; "ashrhi3"
  [(set (match_operand:HI              0 "register_operand"  "")
        (ashiftrt:HI (match_operand:HI 1 "register_operand"  "")
                     (match_operand:HI 2 "register_operand"  "")))]
  "reload_completed && !TARGET_DONT_SPLIT"
  [(set (match_dup:HI 0)
        (ashiftrt:HI (match_dup:HI 1)
                     (match_dup:HI 2)))]
  "{
     rtx to = gen_rtx_REG(V8HImode, REGNO(operands[0]));
     rtx l = gen_rtx_REG(V8HImode, REGNO(operands[1]));
     rtx r = gen_rtx_REG(V8HImode, REGNO(operands[2]));
     spu_emit_insn(gen_spu_sfh(to, CONST0_RTX(V8HImode), r));
     spu_emit_insn(gen_spu_rotmah(to, l, to));
     DONE;
   }")

(define_insn "ashlsi3"
  [(set (match_operand:SI 0 "register_operand"  "=r,r")
	(ashift:SI (match_operand:SI 1 "register_operand"   "r,r")
	           (match_operand:SI 2 "nonmemory_operand"  "r,IJ")))]
  ""
  "@
  shl\t%0,%1,%2
  shli\t%0,%1,%2"
  [(set_attr "type" "fx3")])

(define_insn "lshrsi3"
  [(set (match_operand:SI              0 "register_operand"  "=&r,r")
        (lshiftrt:SI (match_operand:SI 1 "register_operand"   "r,r")
                     (match_operand:SI 2 "nonmemory_operand"  "r,IJ")))]
  ""
  "@
   sfi\t%0,%2,0\;rotm\t%0,%1,%0
   rotmi\t%0,%1,%n2"
  [(set_attr "type" "fx3")])

(define_split ;; "lshrsi3"
  [(set (match_operand:SI              0 "register_operand"  "")
        (lshiftrt:SI (match_operand:SI 1 "register_operand"  "")
                     (match_operand:SI 2 "register_operand"  "")))]
  "reload_completed && !TARGET_DONT_SPLIT"
  [(set (match_dup:SI 0)
        (lshiftrt:SI (match_dup:SI 1)
                     (match_dup:SI 2)))]
  "{
     rtx to = gen_rtx_REG(V4SImode, REGNO(operands[0]));
     rtx l = gen_rtx_REG(V4SImode, REGNO(operands[1]));
     rtx r = gen_rtx_REG(V4SImode, REGNO(operands[2]));
     spu_emit_insn(gen_spu_sf(to, CONST0_RTX(V4SImode), r));
     spu_emit_insn(gen_spu_rotm(to, l, to));
     DONE;
   }")

(define_insn "ashrsi3"
  [(set (match_operand:SI              0 "register_operand"  "=&r,r")
        (ashiftrt:SI (match_operand:SI 1 "register_operand"   "r,r")
                     (match_operand:SI 2 "nonmemory_operand"  "r,IJ")))]
  ""
  "@
   sfi\t%0,%2,0\;rotma\t%0,%1,%0
   rotmai\t%0,%1,%n2"
  [(set_attr "type" "fx3")])

(define_split ;; "ashrsi3"
  [(set (match_operand:SI              0 "register_operand"  "")
        (ashiftrt:SI (match_operand:SI 1 "register_operand"  "")
                     (match_operand:SI 2 "register_operand"  "")))]
  "reload_completed && !TARGET_DONT_SPLIT"
  [(set (match_dup:SI 0)
        (ashiftrt:SI (match_dup:SI 1)
                     (match_dup:SI 2)))]
  "{
     rtx to = gen_rtx_REG(V4SImode, REGNO(operands[0]));
     rtx l = gen_rtx_REG(V4SImode, REGNO(operands[1]));
     rtx r = gen_rtx_REG(V4SImode, REGNO(operands[2]));
     spu_emit_insn(gen_spu_sf(to, CONST0_RTX(V4SImode), r));
     spu_emit_insn(gen_spu_rotma(to, l, to));
     DONE;
   }")

(define_insn_and_split "ashldi3"
  [(set (match_operand:DI            0 "register_operand" "=&r,&r")
	(ashift:DI (match_operand:DI 1 "register_operand"   "r,r")
	           (match_operand:SI 2 "nonmemory_operand"  "r,IJ")))]
  ""
  "@
  rotqbyi\t%0,%1,8\;shlqbyi\t%0,%0,8\;shlqbybi\t%0,%0,%2\;shlqbi\t%0,%0,%2
  rotqbyi\t%0,%1,8\;shlqbyi\t%0,%0,(%2)/8+8\;shlqbii\t%0,%0,(%2)%%8"
  "reload_completed && !TARGET_DONT_SPLIT"
  [(set (match_dup:DI            0)
	(ashift:DI (match_dup:DI 1)
	           (match_dup:SI 2)))]
  "{
     rtx to = gen_rtx_REG(V16QImode, REGNO(operands[0]));
     rtx l = gen_rtx_REG(V16QImode, REGNO(operands[1]));
     spu_emit_insn(gen_spu_rotqby(to, l, GEN_INT(8)));
     if (GET_CODE(operands[2]) == REG)
     {
       spu_emit_insn(gen_spu_shlqby(to, to, GEN_INT(8)));
       spu_emit_insn(gen_spu_shlqbybi(to, to, operands[2]));
       spu_emit_insn(gen_spu_shlqbi(to, to, operands[2]));
     }
     else
     {
       HOST_WIDE_INT val = INTVAL(operands[2]);
       spu_emit_insn(gen_spu_shlqby(to, to, GEN_INT(val/8+8)));
       if (val%8)
	 spu_emit_insn(gen_spu_shlqbi(to, to, GEN_INT(val%8)));
     }
     DONE;
  }"
  [(set_attr "length" "16,12")])

(define_insn_and_split "lshrdi3"
  [(set (match_operand:DI              0 "register_operand"  "=r,r")
        (lshiftrt:DI (match_operand:DI 1 "register_operand"   "r,r")
                     (match_operand:SI 2 "nonmemory_operand"  "r,IJ")))
   (clobber (match_scratch:SI 3 "=&r,X"))
   (clobber (match_scratch:SI 4 "=&r,X"))]
  ""
  "@
  sfi\t%3,%2,0\;sfi\t%4,%2,7\;rotqmbi\t%0,%1,%3\;rotqmbybi\t%0,%0,%4
  rotqmbyi\t%0,%1,(%n2)/8\;rotqmbii\t%0,%0,(%n2)%%8"
  "reload_completed && !TARGET_DONT_SPLIT"
  [(set (match_dup:DI              0)
        (lshiftrt:DI (match_dup:DI 1)
                     (match_dup:SI 2)))]
  "{
     rtx s0 = operands[3];
     rtx s1 = operands[4];
     rtx to = gen_rtx_REG(V16QImode, REGNO(operands[0]));
     rtx l = gen_rtx_REG(V16QImode, REGNO(operands[1]));
     if (GET_CODE(operands[2]) == REG)
     {
       emit_insn(gen_subsi3(s0, GEN_INT(0), operands[2]));
       emit_insn(gen_subsi3(s1, GEN_INT(7), operands[2]));
       spu_emit_insn(gen_spu_rotqmbi(to, l, s0));
       spu_emit_insn(gen_spu_rotqmbybi(to, to, s1));
     }
     else
     {
       HOST_WIDE_INT val = -INTVAL(operands[2]);
       spu_emit_insn(gen_spu_rotqmby(to, l, GEN_INT(val/8)));
       spu_emit_insn(gen_spu_rotqmbi(to, to, GEN_INT(val%8)));
     }
     DONE;
  }"
  [(set_attr "length" "16,8")])

(define_insn_and_split "ashrdi3"
  [(set (match_operand:DI              0 "register_operand" "=&r,&r")
        (ashiftrt:DI (match_operand:DI 1 "register_operand"   "r,r")
                     (match_operand:SI 2 "nonmemory_operand"  "r,IJ")))
   (clobber (match_scratch:V16QI 3 "=&r,&r"))
   (clobber (match_scratch:V16QI 4 "=&r,&r"))]
  ""
  "@
  rotmai\t%3,%1,-31\;fsmbi\t%4,0xff00\;fsm\t%3,%3\;selb\t%0,%3,%1,%4\;sfi\t%3,%2,0\;rotqbybi\t%0,%0,%3\;rotqbi\t%0,%0,%3
  rotmai\t%3,%1,-31\;fsmbi\t%4,0xff00\;fsm\t%3,%3\;selb\t%0,%3,%1,%4\;rotqbyi\t%0,%0,(%n2-7)/8\;rotqbii\t%0,%0,(%n2)%%8"
  "reload_completed && !TARGET_DONT_SPLIT"
  [(set (match_dup:DI              0)
        (ashiftrt:DI (match_dup:DI 1)
                     (match_dup:SI 2)))]
  "{
     rtx s0_si = gen_rtx_REG(SImode, REGNO(operands[3]));
     rtx s0_v4 = gen_rtx_REG(V4SImode, REGNO(operands[3]));
     rtx s0_v16 = operands[3];
     rtx s1_v16 = operands[4];
     rtx op1_si = gen_rtx_REG(SImode, REGNO(operands[1]));
     rtx op0_v16 = gen_rtx_REG(V16QImode, REGNO(operands[0]));
     rtx op1_v16 = gen_rtx_REG(V16QImode, REGNO(operands[1]));
     emit_insn(gen_ashrsi3(s0_si, op1_si, GEN_INT(31)));
     spu_emit_insn(gen_spu_fsmb(s1_v16, GEN_INT(0xff00)));
     spu_emit_insn(gen_spu_fsm(s0_v4, s0_si));
     spu_emit_insn(gen_spu_selb(op0_v16, s0_v16, op1_v16, s1_v16));
     if (GET_CODE(operands[2]) == REG)
     {
       emit_insn(gen_subsi3(s0_si, GEN_INT(0), operands[2]));
       spu_emit_insn(gen_spu_rotqbybi(op0_v16, op0_v16, s0_si));
       spu_emit_insn(gen_spu_rotqbi(op0_v16, op0_v16, s0_si));
     }
     else
     {
       HOST_WIDE_INT val = -INTVAL(operands[2]);
       spu_emit_insn(gen_spu_rotqby(op0_v16, op0_v16, GEN_INT((val-7)/8)));
       spu_emit_insn(gen_spu_rotqbi(op0_v16, op0_v16, GEN_INT(val%8)));
     }
     DONE;
  }"
  [(set_attr "length" "28,24")])

(define_insn_and_split "ashlti3"
  [(set (match_operand:TI 0 "register_operand"   "=&r,r")
        (ashift:TI (match_operand:TI 1 "register_operand"  "r,r")
                   (match_operand:SI 2 "nonmemory_operand" "r,i")))]
  ""
  "@
  shlqbybi\t%0,%1,%2\;shlqbi\t%0,%0,%2
  shlqbyi\t%0,%1,%B2\;shlqbii\t%0,%0,%b2"
  "reload_completed && !TARGET_DONT_SPLIT"
  [(set (match_operand:TI 0 "register_operand"   "=&r,r")
        (ashift:TI (match_operand:TI 1 "register_operand"  "r,r")
                   (match_operand:SI 2 "nonmemory_operand" "r,i")))]
  "{
     rtx to_v16 = gen_rtx_REG(V16QImode, REGNO(operands[0]));
     rtx l_v16 = gen_rtx_REG(V16QImode, REGNO(operands[1]));
     if (GET_CODE(operands[2]) == REG)
     {
       spu_emit_insn(gen_spu_shlqbybi(to_v16,l_v16,operands[2]));
       spu_emit_insn(gen_spu_shlqbi(to_v16,to_v16,operands[2]));
     }
     else
     {
       HOST_WIDE_INT shift = INTVAL(operands[2]);
       if ((shift>>3)&0xf)
	 {
	   spu_emit_insn(gen_spu_shlqby(to_v16,l_v16,GEN_INT((shift>>3)&0xf)));
	   l_v16 = to_v16;
	 }
       if (shift&7)
	 spu_emit_insn(gen_spu_shlqbi(to_v16,l_v16,GEN_INT(shift&7)));
     }
     DONE;
   }"
  [(set_attr "length" "8,8")])

(define_insn_and_split "lshrti3"
  [(set (match_operand:TI 0 "register_operand"   "=r")
        (lshiftrt:TI (match_operand:TI 1 "register_operand"  "r")
                     (match_operand:SI 2 "const_int_operand" "i")))]
  ""
  "rotqmbyi\t%0,%1,%R2\;rotqmbii\t%0,%0,%r2"
  "reload_completed && !TARGET_DONT_SPLIT"
  [(set (match_operand:TI 0 "register_operand"   "=r")
        (lshiftrt:TI (match_operand:TI 1 "register_operand"  "r")
                     (match_operand:SI 2 "const_int_operand" "i")))]
  "{
     rtx to_v16 = gen_rtx_REG(V16QImode, REGNO(operands[0]));
     rtx l_v16 = gen_rtx_REG(V16QImode, REGNO(operands[1]));
     HOST_WIDE_INT shift = INTVAL(operands[2]);
     HOST_WIDE_INT byte_shift = (shift >> 3) & 0x1f;
     HOST_WIDE_INT bit_shift = shift & 0x7;

     if (byte_shift)
       {
         spu_emit_insn (gen_spu_rotqmby (to_v16, l_v16,
                        GEN_INT (-byte_shift)));
         l_v16 = to_v16;
       }
     if (bit_shift)
       spu_emit_insn (gen_spu_rotqmbi (to_v16, l_v16, GEN_INT (-bit_shift)));
     DONE;
   }"
  [(set_attr "length" "8")])

(define_insn_and_split "ashrti3"
  [(set (match_operand:TI 0 "register_operand"   "=r")
        (ashiftrt:TI (match_operand:TI 1 "register_operand"  "r")
                     (match_operand:SI 2 "const_int_operand" "i")))
   (clobber (match_scratch:SI 3 "=&r"))]
  ""
  "rotmai\t%3,%1,-31\;fsmb\t%3,%3\;shlqbyi\t%3,%3,%S2\;shlqbii\t%3,%3,%s2\;rotqmbyi\t%0,%1,%R2\;rotqmbii\t%0,%0,%r2\;or\t%0,%0,%3"
  "reload_completed && !TARGET_DONT_SPLIT"
  [(set (match_operand:TI 0 "register_operand"   "=r")
        (ashiftrt:TI (match_operand:TI 1 "register_operand"  "r")
                     (match_operand:SI 2 "const_int_operand" "i")))]
  "{
     rtx s0 = operands[3];
     rtx s0_v4 = gen_rtx_REG(V4SImode, REGNO(operands[3]));
     rtx s0_v16 = gen_rtx_REG(V16QImode, REGNO(operands[3]));
     rtx to_v4 = gen_rtx_REG(V4SImode, REGNO(operands[0]));
     rtx to_v16 = gen_rtx_REG(V16QImode, REGNO(operands[0]));
     rtx l_v4 = gen_rtx_REG(V4SImode, REGNO(operands[1]));
     rtx l_v16 = gen_rtx_REG(V16QImode, REGNO(operands[1]));
     HOST_WIDE_INT shift = -INTVAL(operands[2]);
     spu_emit_insn(gen_spu_rotma(s0_v4,l_v4,spu_const_vector(V4SImode, GEN_INT(-31))));
     spu_emit_insn(gen_spu_fsm(s0_v4,s0));
     if ((shift>>3)&0xf)
       spu_emit_insn(gen_spu_shlqby(s0_v16,s0_v16,GEN_INT((shift>>3)&0xf)));
     if (shift&7)
       spu_emit_insn(gen_spu_shlqbi(s0_v16,s0_v16,GEN_INT(shift&7)));
     if (((7-shift)>>3)&0x1f)
     {
       spu_emit_insn(gen_spu_rotqmby( to_v16,l_v16,GEN_INT(((shift+7)<<56)>>59)));
       l_v16 = to_v16;
     }
     if (shift&7)
       spu_emit_insn(gen_spu_rotqmbi( to_v16,l_v16,GEN_INT((shift<<61)>>61)));
     spu_emit_insn(gen_spu_or(to_v4,to_v4,s0_v4));
     DONE;
   }"
  [(set_attr "length" "28")])

(define_insn "rotlhi3"
  [(set (match_operand:HI 0 "register_operand"  "=r,r")
	(rotate:HI (match_operand:HI 1 "register_operand"   "r,r")
	           (match_operand:HI 2 "nonmemory_operand"  "r,IJ")))]
  ""
  "@
  roth\t%0,%1,%2
  rothi\t%0,%1,%2"
  [(set_attr "type" "fx3")])

(define_insn "rotlsi3"
  [(set (match_operand:SI 0 "register_operand"  "=r,r")
	(rotate:SI (match_operand:SI 1 "register_operand"   "r,r")
	           (match_operand:SI 2 "nonmemory_operand"  "r,IJ")))]
  ""
  "@
  rot\t%0,%1,%2
  roti\t%0,%1,%2"
  [(set_attr "type" "fx3")])

(define_insn "rotlti3"
  [(set (match_operand:TI 0 "register_operand"   "=&r,r")
        (rotate:TI (match_operand:TI 1 "register_operand"  "r,r")
                   (match_operand:SI 2 "nonmemory_operand" "r,i")))]
  ""
  "@
  rotqbybi\t%0,%1,%2\;rotqbi\t%0,%0,%2
  rotqbyi\t%0,%1,%B2\;rotqbii\t%0,%0,%b2"
  [(set_attr "length" "8,8")])


;; struct extract/insert
(define_expand "extv"
  [(set (match_operand:TI 0 "register_operand"   "=r")
        (sign_extract:TI (match_operand:TI 1 "register_operand"  "r")
                         (match_operand:SI 2 "const_int_operand" "i")
                         (match_operand:SI 3 "const_int_operand" "i")))]
  ""
  "{ spu_extract(operands, 0); DONE; }")

(define_expand "extzv"
  [(set (match_operand:TI 0 "register_operand"   "=r")
        (zero_extract:TI (match_operand:TI 1 "register_operand"  "r")
                         (match_operand:SI 2 "const_int_operand" "i")
                         (match_operand:SI 3 "const_int_operand" "i")))]
  ""
  "{ spu_extract(operands, 1); DONE; }")

(define_expand "insv"
  [(set (zero_extract:TI (match_operand:TI 0 "register_operand" "+r")
			 (match_operand:SI 1 "const_int_operand" "i")
			 (match_operand:SI 2 "const_int_operand" "i"))
	(match_operand:TI 3 "nonmemory_operand" "ri"))]
  ""
  "{ spu_insert(operands); DONE; }")


;; String/block move insn.
;; Argument 0 is the destination
;; Argument 1 is the source
;; Argument 2 is the length
;; Argument 3 is the alignment

(define_expand "movstrsi"
  [(parallel [(set (match_operand:BLK 0 "" "")
		   (match_operand:BLK 1 "" ""))
	      (use (match_operand:SI 2 "" ""))
	      (use (match_operand:SI 3 "" ""))])]
  ""
  "
{
  if (spu_expand_block_move (operands))
    DONE;
  else
    FAIL;
}")

(define_insn "indirect_jump"
  [(set (pc) (match_operand:SI 0 "register_operand" "r"))]
  ""
  "bi\t%0"
  [(set_attr "type" "br")])

(define_insn "jump"
  [(set (pc)
	(label_ref (match_operand 0 "" "")))]
  ""
  "br\t%0"
  [(set_attr "type" "br")])

;; it will be used for leaf functions, that don't save any regs and
;; don't have locals on stack, maybe... that is for functions that
;; don't change $sp and don't need to save $lr. right now it's never used.

(define_expand "return"
    [(return)]
  "direct_return()"
  "")

;; used in spu_expand_epilogue to generate return from a function and
;; explicitly set use of $lr.

(define_insn "return_internal"
  [(return)]
  ""
  "bi\t$lr"
  [(set_attr "type" "br")])


(define_insn "nop"
  [(const_int 0)]
  ""
  "nop"
  [(set_attr "type" "nop")])


;; Test & branch instructions

(define_insn "ceq_qi"
  [(set (match_operand:QI 0 "register_operand" "=r,r")
        (eq:QI (match_operand:QI 1 "register_operand" "r,r")
	       (match_operand:QI 2 "rK_operand" "r,K")))]
  ""
  "@
  ceqb\t%0,%1,%2
  ceqbi\t%0,%1,%2")

(define_insn "cgt_qi"
  [(set (match_operand:QI 0 "register_operand" "=r,r")
        (gt:QI (match_operand:QI 1 "register_operand" "r,r")
	       (match_operand:QI 2 "rK_operand" "r,K")))]
  ""
  "@
  cgtb\t%0,%1,%2
  cgtbi\t%0,%1,%2")

(define_insn "clgt_qi"
  [(set (match_operand:QI 0 "register_operand" "=r,r")
        (gtu:QI (match_operand:QI 1 "register_operand" "r,r")
	        (match_operand:QI 2 "rK_operand" "r,K")))]
  ""
  "@
  clgtb\t%0,%1,%2
  clgtbi\t%0,%1,%2")

(define_insn "ceq_hi"
  [(set (match_operand:HI 0 "register_operand" "=r,r")
        (eq:HI (match_operand:HI 1 "register_operand" "r,r")
	       (match_operand:HI 2 "rK_operand" "r,K")))]
  ""
  "@
  ceqh\t%0,%1,%2
  ceqhi\t%0,%1,%2")

(define_insn "cgt_hi"
  [(set (match_operand:HI 0 "register_operand" "=r,r")
        (gt:HI (match_operand:HI 1 "register_operand" "r,r")
	       (match_operand:HI 2 "rK_operand" "r,K")))]
  ""
  "@
  cgth\t%0,%1,%2
  cgthi\t%0,%1,%2")

(define_insn "clgt_hi"
  [(set (match_operand:HI 0 "register_operand" "=r,r")
        (gtu:HI (match_operand:HI 1 "register_operand" "r,r")
	        (match_operand:HI 2 "rK_operand" "r,K")))]
  ""
  "@
  clgth\t%0,%1,%2
  clgthi\t%0,%1,%2")

(define_insn "ceq_si"
  [(set (match_operand:SI 0 "register_operand" "=r,r")
        (eq:SI (match_operand:SI 1 "register_operand" "r,r")
	       (match_operand:SI 2 "rK_operand" "r,K")))]
  ""
  "@
  ceq\t%0,%1,%2
  ceqi\t%0,%1,%2")

(define_insn "cgt_si"
  [(set (match_operand:SI 0 "register_operand" "=r,r")
        (gt:SI (match_operand:SI 1 "register_operand" "r,r")
	       (match_operand:SI 2 "rK_operand" "r,K")))]
  ""
  "@
  cgt\t%0,%1,%2
  cgti\t%0,%1,%2")

(define_insn "clgt_si"
  [(set (match_operand:SI 0 "register_operand" "=r,r")
        (gtu:SI (match_operand:SI 1 "register_operand" "r,r")
	        (match_operand:SI 2 "rK_operand" "r,K")))]
  ""
  "@
  clgt\t%0,%1,%2
  clgti\t%0,%1,%2")

(define_insn_and_split "ceq_di"
  [(set (match_operand:SI 0 "register_operand" "=r")
        (eq:SI (match_operand:DI 1 "register_operand" "r")
	       (match_operand:DI 2 "register_operand" "r")))]
  ""
  "ceq\t%0,%1,%2\;gb\t%0,%0\;cgti\t%0,%0,11"
  "reload_completed && !TARGET_DONT_SPLIT"
  [(set (match_dup:SI 0)
        (eq:SI (match_dup:DI 1)
	       (match_dup:DI 2)))]
  "{
     rtx to_v4 = gen_rtx_REG(V4SImode, REGNO(operands[0]));
     rtx l_v4 = gen_rtx_REG(V4SImode, REGNO(operands[1]));
     rtx r_v4 = gen_rtx_REG(V4SImode, REGNO(operands[2]));
     spu_emit_insn(gen_spu_ceq(to_v4, l_v4, r_v4));
     spu_emit_insn(gen_spu_gb(to_v4, to_v4));
     emit_insn(gen_cgt_si(operands[0], operands[0], GEN_INT(11)));
     DONE;
   }"
  [(set_attr "length" "12")])

(define_insn_and_split "cgt_di" 
  [(set (match_operand:SI 0 "register_operand" "=&r")
        (gt:SI (match_operand:DI 1 "register_operand" "r")
	       (match_operand:DI 2 "register_operand" "r")))
   (clobber (match_scratch:V16QI 3 "=&r"))]
  ""
  "clgt\t%3,%1,%2\;ceq\t%0,%1,%2\;rotqbyi\t%3,%3,4\;and\t%0,%0,%3\;cgt\t%3,%1,%2\;or\t%0,%0,%3"
  "reload_completed && !TARGET_DONT_SPLIT"
  [(set (match_dup:SI 0)
        (gt:SI (match_dup:DI 1)
	       (match_dup:DI 2)))]
  "{
     rtx s0_v16 = operands[3];
     rtx s0_v4 = gen_rtx_REG(V4SImode, REGNO(operands[3]));
     rtx to_v4 = gen_rtx_REG(V4SImode, REGNO(operands[0]));
     rtx l_v4 = gen_rtx_REG(V4SImode, REGNO(operands[1]));
     rtx r_v4 = gen_rtx_REG(V4SImode, REGNO(operands[2]));
     spu_emit_insn(gen_spu_clgt(s0_v4, l_v4, r_v4));
     spu_emit_insn(gen_spu_ceq(to_v4, l_v4, r_v4));
     spu_emit_insn(gen_spu_rotqby(s0_v16, s0_v16, GEN_INT(4)));
     spu_emit_insn(gen_spu_and(to_v4, to_v4, s0_v4));
     spu_emit_insn(gen_spu_cgt(s0_v4, l_v4, r_v4));
     spu_emit_insn(gen_spu_or(to_v4, to_v4, s0_v4));
     DONE;
   }"
  [(set_attr "length" "24")])

(define_insn_and_split "clgt_di"
  [(set (match_operand:SI 0 "register_operand" "=&r")
        (gtu:SI (match_operand:DI 1 "register_operand" "r")
	        (match_operand:DI 2 "register_operand" "r")))
   (clobber (match_scratch:V16QI 3 "=&r"))]
  ""
  "clgt\t%3,%1,%2\;ceq\t%0,%1,%2\;rotqbyi\t%3,%3,4\;and\t%0,%0,%3\;clgt\t%3,%1,%2\;or\t%0,%0,%3"
  "reload_completed && !TARGET_DONT_SPLIT"
  [(set (match_dup:SI 0)
        (gtu:SI (match_dup:DI 1)
	        (match_dup:DI 2)))]
  "{
     rtx s0_v16 = operands[3];
     rtx s0_v4 = gen_rtx_REG(V4SImode, REGNO(operands[3]));
     rtx to_v4 = gen_rtx_REG(V4SImode, REGNO(operands[0]));
     rtx l_v4 = gen_rtx_REG(V4SImode, REGNO(operands[1]));
     rtx r_v4 = gen_rtx_REG(V4SImode, REGNO(operands[2]));
     spu_emit_insn(gen_spu_clgt(s0_v4, l_v4, r_v4));
     spu_emit_insn(gen_spu_ceq(to_v4, l_v4, r_v4));
     spu_emit_insn(gen_spu_rotqby(s0_v16, s0_v16, GEN_INT(4)));
     spu_emit_insn(gen_spu_and(to_v4, to_v4, s0_v4));
     spu_emit_insn(gen_spu_clgt(s0_v4, l_v4, r_v4));
     spu_emit_insn(gen_spu_or(to_v4, to_v4, s0_v4));
     DONE;
   }"
  [(set_attr "length" "24")])

(define_insn "ceq_sf"
  [(set (match_operand:SI 0 "register_operand" "=r")
        (eq:SI (match_operand:SF 1 "register_operand" "r")
	       (match_operand:SF 2 "register_operand" "r")))]
  ""
  "fceq\t%0,%1,%2")

(define_insn "cmeq_sf"
  [(set (match_operand:SI 0 "register_operand" "=r")
        (eq:SI (abs:SF (match_operand:SF 1 "register_operand" "r"))
	       (abs:SF (match_operand:SF 2 "register_operand" "r"))))]
  ""
  "fcmeq\t%0,%1,%2")

(define_insn "cgt_sf"
  [(set (match_operand:SI 0 "register_operand" "=r")
        (gt:SI (match_operand:SF 1 "register_operand" "r")
	       (match_operand:SF 2 "register_operand" "r")))]
  ""
  "fcgt\t%0,%1,%2")

(define_insn "cmgt_sf"
  [(set (match_operand:SI 0 "register_operand" "=r")
        (gt:SI (abs:SF (match_operand:SF 1 "register_operand" "r"))
	       (abs:SF (match_operand:SF 2 "register_operand" "r"))))]
  ""
  "fcmgt\t%0,%1,%2")

;; Add these patterns to catch some optimizations the compiler generates.
(define_insn "clt_sf"
  [(set (match_operand:SI 0 "register_operand" "=r")
        (lt:SI (match_operand:SF 1 "register_operand" "r")
	       (match_operand:SF 2 "register_operand" "r")))]
  ""
  "fcgt\t%0,%2,%1")

(define_insn "cmlt_sf"
  [(set (match_operand:SI 0 "register_operand" "=r")
        (lt:SI (abs:SF (match_operand:SF 1 "register_operand" "r"))
	       (abs:SF (match_operand:SF 2 "register_operand" "r"))))]
  ""
  "fcmgt\t%0,%2,%1")

;; These implementations of ceq_df and cgt_df do not correctly handle
;; NAN or INF.  We will also get incorrect results when the result
;; of the double subtract is too small.
(define_insn_and_split "ceq_df"
  [(set (match_operand:SI 0 "register_operand" "=r")
        (eq:SI (match_operand:DF 1 "register_operand" "r")
	       (match_operand:DF 2 "const_zero_operand" "i")))
   (clobber (match_scratch:V4SI 3 "=&r"))
   (clobber (match_scratch:V4SI 4 "=&r"))]
  "flag_unsafe_math_optimizations"
  "ilhu\t%4,0x8000\;ceqi\t%3,%1,0\;ceq\t%4,%1,%4\;shlqbyi\t%0,%3,4\;or\t%4,%3,%4\;and\t%0,%0,%4"
  "reload_completed && !TARGET_DONT_SPLIT"
  [(set (match_dup:SI 0)
        (eq:SI (match_dup:DF 1)
	       (match_dup:DF 2)))]
  "{
     rtx s0_v4 = operands[3];
     rtx s1_v4 = operands[4];
     rtx s0_v16 = gen_rtx_REG(V16QImode, REGNO(operands[3]));
     rtx to_v16 = gen_rtx_REG(V16QImode, REGNO(operands[0]));
     rtx to_v4 = gen_rtx_REG(V4SImode, REGNO(operands[0]));
     rtx l_v4 = gen_rtx_REG(V4SImode, REGNO(operands[1]));
     spu_emit_insn(gen_spu_ilhu(s1_v4, GEN_INT(0x8000)));
     spu_emit_insn(gen_spu_ceqi(s0_v4, l_v4, const0_rtx));
     spu_emit_insn(gen_spu_ceq(s1_v4, l_v4, s1_v4));
     spu_emit_insn(gen_spu_rotqby(to_v16, s0_v16, GEN_INT(4)));
     spu_emit_insn(gen_spu_or(s1_v4, s0_v4, s1_v4));
     spu_emit_insn(gen_spu_and(to_v4, to_v4, s1_v4));
     DONE;
   }"
  [(set_attr "length" "24")])

(define_insn_and_split "cgt_df"
  [(set (match_operand:SI 0 "register_operand" "=r")
        (gt:SI (match_operand:DF 1 "register_operand" "r")
	       (match_operand:DF 2 "const_zero_operand" "i")))
   (clobber (match_scratch:V4SI 3 "=&r"))
   (clobber (match_scratch:V4SI 4 "=&r"))]
  "flag_unsafe_math_optimizations"
  "ceqi\t%3,%1,0\;cgti\t%4,%1,0\;shlqbyi\t%0,%3,4\;andc\t%0,%3,%0\;or\t%0,%0,%4"
  "reload_completed && !TARGET_DONT_SPLIT"
  [(set (match_dup:SI 0)
        (gt:SI (match_dup:DF 1)
	       (match_dup:DF 2)))]
  "{
     rtx s0_v4 = operands[3];
     rtx s1_v4 = operands[4];
     rtx s0_v16 = gen_rtx_REG(V16QImode, REGNO(operands[3]));
     rtx to_v16 = gen_rtx_REG(V16QImode, REGNO(operands[0]));
     rtx to_v4 = gen_rtx_REG(V4SImode, REGNO(operands[0]));
     rtx l_v4 = gen_rtx_REG(V4SImode, REGNO(operands[1]));
     spu_emit_insn(gen_spu_ceqi(s0_v4, l_v4, const0_rtx));
     spu_emit_insn(gen_spu_cgti(s1_v4, l_v4, const0_rtx));
     spu_emit_insn(gen_spu_rotqby(to_v16, s0_v16, GEN_INT(4)));
     spu_emit_insn(gen_spu_andc(to_v4, s0_v4, to_v4));
     spu_emit_insn(gen_spu_or(to_v4, to_v4, s1_v4));
     DONE;
   }"
  [(set_attr "length" "20")])

(define_insn "ceq_ti"
  [(set (match_operand:SI 0 "register_operand" "=r")
        (eq:SI (match_operand:TI 1 "register_operand" "r")
	       (match_operand:TI 2 "register_operand" "r")))]
  ""
  "ceq\t%0,%1,%2\;gb\t%0,%0\;ceqi\t%0,%0,15"
  [(set_attr "type" "multi0")
   (set_attr "length" "12")])

(define_insn ""
  [(set (pc)
	(if_then_else (match_operator 1 "branch_comparison_operator"
				      [(match_operand 2
						      "register_operand" "r")
				       (const_int 0)])
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  "br%b2%b1z\t%2,%0"
  [(set_attr "type" "br")])

(define_insn ""
  [(set (pc)
	(if_then_else (match_operator 0 "branch_comparison_operator"
				      [(match_operand 1
						      "register_operand" "r")
				       (const_int 0)])
		      (return)
		      (pc)))]
  "direct_return () && find_reg_note (insn, REG_BR_HINT, 0) == 0"
  "bi%b1%b0z\t%1,$lr"
  [(set_attr "type" "br")])

(define_insn ""
  [(set (pc)
	(if_then_else (match_operator 1 "branch_comparison_operator"
				      [(match_operand 2
						      "register_operand" "r")
				       (const_int 0)])
		      (pc)
		      (label_ref (match_operand 0 "" ""))))]
  ""
  "br%b2%b1z\t%2,%0"
  [(set_attr "type" "br")])

(define_insn ""
  [(set (pc)
	(if_then_else (match_operator 0 "branch_comparison_operator"
				      [(match_operand 1
						      "register_operand" "r")
				       (const_int 0)])
		      (pc)
		      (return)))]
  "direct_return () && find_reg_note (insn, REG_BR_HINT, 0) == 0"
  "bi%b1%b0z\t%1,$lr"
  [(set_attr "type" "br")])


;; Compare insns are next.  Note that the spu has two types of compares,
;; signed & unsigned, and one type of branch.
;;
;; Start with the DEFINE_EXPANDs to generate the rtl for compares, scc
;; insns, and branches.  We store the operands of compares until we see
;; how it is used.
(define_expand "cmpqi"
  [(set (cc0)
        (compare (match_operand:QI 0 "register_operand" "")
  		 (match_operand:QI 1 "nonmemory_operand" "")))]
  ""
  "{
  spu_compare_op0 = operands[0];
  spu_compare_op1 = operands[1];
  DONE;
}")

(define_expand "cmphi"
  [(set (cc0)
        (compare (match_operand:HI 0 "register_operand" "")
  		 (match_operand:HI 1 "nonmemory_operand" "")))]
  ""
  "{
  spu_compare_op0 = operands[0];
  spu_compare_op1 = operands[1];
  DONE;
}")

(define_expand "cmpsi"
  [(set (cc0)
        (compare (match_operand:SI 0 "register_operand" "")
  		 (match_operand:SI 1 "nonmemory_operand" "")))]
  ""
  "{
  spu_compare_op0 = operands[0];
  spu_compare_op1 = operands[1];
  DONE;
}")

(define_expand "cmpdi"
  [(set (cc0)
        (compare (match_operand:DI 0 "register_operand" "")
  		 (match_operand:DI 1 "register_operand" "")))]
  ""
  "{
  spu_compare_op0 = operands[0];
  spu_compare_op1 = operands[1];
  DONE;
}")

(define_expand "cmpti"
  [(set (cc0)
        (compare (match_operand:TI 0 "register_operand" "")
  		 (match_operand:TI 1 "register_operand" "")))]
  ""
  "{
  spu_compare_op0 = operands[0];
  spu_compare_op1 = operands[1];
  DONE;
}")

(define_expand "cmpsf"
  [(set (cc0)
        (compare (match_operand:SF 0 "register_operand" "")
  		 (match_operand:SF 1 "register_operand" "")))]
  ""
  "{
  spu_compare_op0 = operands[0];
  spu_compare_op1 = operands[1];
  DONE;
}")

(define_expand "cmpdf"
  [(set (cc0)
        (compare (match_operand:DF 0 "register_operand" "")
  		 (match_operand:DF 1 "register_operand" "")))]
  "flag_unsafe_math_optimizations"
  "{
  spu_compare_op0 = operands[0];
  spu_compare_op1 = operands[1];
  DONE;
}")



;; branch on condition
(define_expand "beq"
  [(use (match_operand 0 "" ""))]
  ""
  "{ spu_emit_branch_or_set (0, EQ, operands); DONE; }")

(define_expand "bne"
  [(use (match_operand 0 "" ""))]
  ""
  "{ spu_emit_branch_or_set (0, NE, operands); DONE; }")

(define_expand "bge"
  [(use (match_operand 0 "" ""))]
  ""
  "{ spu_emit_branch_or_set (0, GE, operands); DONE; }")

(define_expand "bgt"
  [(use (match_operand 0 "" ""))]
  ""
  "{ spu_emit_branch_or_set (0, GT, operands); DONE; }")

(define_expand "ble"
  [(use (match_operand 0 "" ""))]
  ""
  "{ spu_emit_branch_or_set (0, LE, operands); DONE; }")

(define_expand "blt"
  [(use (match_operand 0 "" ""))]
  ""
  "{ spu_emit_branch_or_set (0, LT, operands); DONE; }")

(define_expand "bgeu"
  [(use (match_operand 0 "" ""))]
  ""
  "{ spu_emit_branch_or_set (0, GEU, operands); DONE; }")

(define_expand "bgtu"
  [(use (match_operand 0 "" ""))]
  ""
  "{ spu_emit_branch_or_set (0, GTU, operands); DONE; }")

(define_expand "bleu"
  [(use (match_operand 0 "" ""))]
  ""
  "{ spu_emit_branch_or_set (0, LEU, operands); DONE; }")

(define_expand "bltu"
  [(use (match_operand 0 "" ""))]
  ""
  "{ spu_emit_branch_or_set (0, LTU, operands); DONE; }")


;; set on condition
(define_expand "seq"
  [(clobber (match_operand:SI 0 "register_operand" ""))]
  ""
  "{ spu_emit_branch_or_set (1, EQ, operands); DONE; }")

(define_expand "sne"
  [(clobber (match_operand:SI 0 "register_operand" ""))]
  ""
  "{ spu_emit_branch_or_set (1, NE, operands); DONE; }")

(define_expand "sgt"
  [(clobber (match_operand:SI 0 "register_operand" ""))]
  ""
  "{ spu_emit_branch_or_set (1, GT, operands); DONE; }")

(define_expand "slt"
  [(clobber (match_operand:SI 0 "register_operand" ""))]
  ""
  "{ spu_emit_branch_or_set (1, LT, operands); DONE; }")

(define_expand "sge"
  [(clobber (match_operand:SI 0 "register_operand" ""))]
  ""
  "{ spu_emit_branch_or_set (1, GE, operands); DONE; }")

(define_expand "sle"
  [(clobber (match_operand:SI 0 "register_operand" ""))]
  ""
  "{ spu_emit_branch_or_set (1, LE, operands); DONE; }")

(define_expand "sgtu"
  [(clobber (match_operand:SI 0 "register_operand" ""))]
  ""
  "{ spu_emit_branch_or_set (1, GTU, operands); DONE; }")

(define_expand "sltu"
  [(clobber (match_operand:SI 0 "register_operand" ""))]
  ""
  "{ spu_emit_branch_or_set (1, LTU, operands); DONE; }")

(define_expand "sgeu"
  [(clobber (match_operand:SI 0 "register_operand" ""))]
  ""
  "{ spu_emit_branch_or_set (1, GEU, operands); DONE; }")

(define_expand "sleu"
  [(clobber (match_operand:SI 0 "register_operand" ""))]
  ""
  "{ spu_emit_branch_or_set (1, LEU, operands); DONE; }")

(define_insn_and_split ""
  [(set (match_operand:SI 0 "register_operand" "=r,r")
        (neg:SI (eq:SI (match_operand:SI 1 "register_operand" "r,r")
	               (match_operand:SI 2 "rK_operand" "r,K"))))]
  "0"
  "#"
  ""
  [(set (match_dup 0)
        (eq:SI (match_dup 1) (match_dup 2)))
   (set (match_dup 0)
        (and:SI (match_dup 0) (match_dup 3)))]
  "operands[3] = CONST1_RTX(SImode);")

;; conditional move
;; Define this first one so HAVE_conditional_move is defined.
(define_insn "movcc_dummy"
  [(set (match_operand 0 "" "")
       (if_then_else (match_operand 1 "" "")
                     (match_operand 2 "" "")
                     (match_operand 3 "" "")))]
  "!operands[0]"
  "")

(define_expand "movqicc"
  [(set (match_operand:QI 0 "register_operand" "")
	(if_then_else:QI (match_operand 1 "comparison_operator" "")
		      (match_operand:QI 2 "register_operand" "")
		      (match_operand:QI 3 "register_operand" "")))]
  ""
  "{
      spu_emit_branch_or_set(2, GET_CODE(operands[1]), operands);
      DONE;
  }")

(define_expand "movhicc"
  [(set (match_operand:HI 0 "register_operand" "")
	(if_then_else:HI (match_operand 1 "comparison_operator" "")
		      (match_operand:HI 2 "register_operand" "")
		      (match_operand:HI 3 "register_operand" "")))]
  ""
  "{
      spu_emit_branch_or_set(2, GET_CODE(operands[1]), operands);
      DONE;
  }")

(define_expand "movsicc"
  [(set (match_operand:SI 0 "register_operand" "")
	(if_then_else:SI (match_operand 1 "comparison_operator" "")
		      (match_operand:SI 2 "register_operand" "")
		      (match_operand:SI 3 "register_operand" "")))]
  ""
  "{
      spu_emit_branch_or_set(2, GET_CODE(operands[1]), operands);
      DONE;
  }")

(define_expand "movdicc"
  [(set (match_operand:DI 0 "register_operand" "")
	(if_then_else:DI (match_operand 1 "comparison_operator" "")
		      (match_operand:DI 2 "register_operand" "")
		      (match_operand:DI 3 "register_operand" "")))]
  ""
  "{
      spu_emit_branch_or_set(2, GET_CODE(operands[1]), operands);
      DONE;
  }")

(define_expand "movsfcc"
  [(set (match_operand:SF 0 "register_operand" "")
	(if_then_else:SF (match_operand 1 "comparison_operator" "")
		      (match_operand:SF 2 "register_operand" "")
		      (match_operand:SF 3 "register_operand" "")))]
  ""
  "{
      spu_emit_branch_or_set(2, GET_CODE(operands[1]), operands);
      DONE;
  }")

(define_expand "movdfcc"
  [(set (match_operand:DF 0 "register_operand" "")
	(if_then_else:DF (match_operand 1 "comparison_operator" "")
		      (match_operand:DF 2 "register_operand" "")
		      (match_operand:DF 3 "register_operand" "")))]
  ""
  "{
      spu_emit_branch_or_set(2, GET_CODE(operands[1]), operands);
      DONE;
  }")

(define_expand "movv16qicc"
  [(set (match_operand:V16QI 0 "register_operand" "")
	(if_then_else:QI (match_operand 1 "comparison_operator" "")
		      (match_operand:V16QI 2 "register_operand" "")
		      (match_operand:V16QI 3 "register_operand" "")))]
  ""
  "{
      spu_emit_branch_or_set(2, GET_CODE(operands[1]), operands);
      DONE;
  }")

(define_expand "movv8hicc"
  [(set (match_operand:V8HI 0 "register_operand" "")
	(if_then_else:HI (match_operand 1 "comparison_operator" "")
		      (match_operand:V8HI 2 "register_operand" "")
		      (match_operand:V8HI 3 "register_operand" "")))]
  ""
  "{
      spu_emit_branch_or_set(2, GET_CODE(operands[1]), operands);
      DONE;
  }")

(define_expand "movv4sicc"
  [(set (match_operand:V4SI 0 "register_operand" "")
	(if_then_else:SI (match_operand 1 "comparison_operator" "")
		      (match_operand:V4SI 2 "register_operand" "")
		      (match_operand:V4SI 3 "register_operand" "")))]
  ""
  "{
      spu_emit_branch_or_set(2, GET_CODE(operands[1]), operands);
      DONE;
  }")

(define_expand "movv4sfcc"
  [(set (match_operand:V4SF 0 "register_operand" "")
	(if_then_else:SF (match_operand 1 "comparison_operator" "")
		      (match_operand:V4SF 2 "register_operand" "")
		      (match_operand:V4SF 3 "register_operand" "")))]
  ""
  "{
      spu_emit_branch_or_set(2, GET_CODE(operands[1]), operands);
      DONE;
  }")

(define_expand "movv2dfcc"
  [(set (match_operand:V2DF 0 "register_operand" "")
	(if_then_else:DF (match_operand 1 "comparison_operator" "")
		      (match_operand:V2DF 2 "register_operand" "")
		      (match_operand:V2DF 3 "register_operand" "")))]
  ""
  "{
      spu_emit_branch_or_set(2, GET_CODE(operands[1]), operands);
      DONE;
  }")

;; This pattern is used when the result of a compare is not large
;; enough to use in a selb when expanding conditional moves.
(define_insn "extend_compare"
  [(set (match_operand 0 "register_operand"  "=r")
        (unspec [(match_operand  1 "register_operand"  "r")] UNSPEC_EXTEND_CMP))]
  "operands"
  "fsm\t%0,%1")

;; operand 0 is index
;; operand 1 is the minimum bound
;; operand 2 is the maximum bound - minimum bound + 1
;; operand 3 is CODE_LABEL for the table;
;; operand 4 is the CODE_LABEL to go to if index out of range.
(define_expand "casesi"
  [(match_operand:SI 0 "register_operand" "")
   (match_operand:SI 1 "immediate_operand" "")
   (match_operand:SI 2 "immediate_operand" "")
   (match_operand 3 "" "")
   (match_operand 4 "" "")]
  ""
  "
{
  rtx table = gen_reg_rtx (SImode);
  rtx index = gen_reg_rtx (SImode);
  rtx sindex = gen_reg_rtx (SImode);
  rtx addr = gen_reg_rtx (Pmode);
  rtx note, insn, expected;

  insn = get_last_insn_anywhere();

  emit_move_insn (table, gen_rtx_LABEL_REF (SImode, operands[3]));
  mark_reg_pointer (table, 128);

  emit_insn (gen_subsi3(index, operands[0], force_reg(SImode, operands[1])));
  emit_move_insn (sindex, gen_rtx_ASHIFT (SImode, index, GEN_INT (2)));
  emit_move_insn (addr, gen_rtx_MEM (SImode,
				     gen_rtx_PLUS (SImode, table, sindex)));
  if (flag_pic)
    emit_move_insn (addr, gen_rtx_PLUS (SImode, addr, table));

  expected = const0_rtx;
  if (insn
      && NOTE_P (insn)
      && NOTE_LINE_NUMBER (insn) == NOTE_INSN_EXPECTED_VALUE
      && XEXP (NOTE_EXPECTED_VALUE (insn), 0) == operands[0]
      && GET_CODE (XEXP (NOTE_EXPECTED_VALUE (insn), 1)) == CONST_INT)
    expected = GEN_INT (INTVAL (XEXP (NOTE_EXPECTED_VALUE (insn), 1)) - INTVAL (operands[1]));

  /* Predict a value for the next cmp_and_jump based on a previous
     NOTE_INSN_EXPECTED_VALUE otherwise predict it will not be taken. */
  note = emit_note (NOTE_INSN_EXPECTED_VALUE);
  NOTE_EXPECTED_VALUE (note) = gen_rtx_EQ (VOIDmode, index, expected); 

  emit_cmp_and_jump_insns (index, operands[2], GTU, NULL_RTX, SImode, 1, operands[4]);

#if 0
  {
    /* With conditional move, instead of the cmp_and_jump, there is the
       potential to hint the out of bounds case too, but in general the
       the cmp_and_jump is faster because that jump is pipelined and we
       usually expect it not to get taken.  */
    rtx outaddr = gen_reg_rtx (Pmode);
    emit_move_insn (outaddr, gen_rtx_LABEL_REF (SImode, operands[4]));
    addr = emit_conditional_move (addr, 
		  GTU, index, operands[2],
		  SImode, outaddr, addr, Pmode,
		  1);
  }
#endif

  emit_jump_insn (gen_tablejump (addr, operands[3]));
  DONE;
}")

(define_insn "tablejump"
  [(set (pc) (match_operand:SI 0 "register_operand" "r"))
   (use (label_ref (match_operand 1 "" "")))]
  ""
  {
      return "bi\t%0";
  }
  [(set_attr "type" "br")])

;; Note that operand 1 is total size of args, in bytes,
;; and what the call insn wants is the number of words.
(define_expand "sibcall"
  [(parallel
    [(call (match_operand:QI 0 "sibcall_operand" "")
	   (match_operand:QI 1 "" ""))
     (use (reg:SI 0))])]
  ""
  "{
      if (! sibcall_operand (operands[0], QImode))
        XEXP (operands[0], 0) = copy_to_mode_reg (Pmode, XEXP (operands[0], 0));
      /* this function will check whether the callee has
         dont_save_regs attribute attached and will issue clobber
         insns for every register in the range, which was encoded in
         the callee's symbol prefix by the attribute handler.  The
         prefix is stripped when the final assembly is being output.
         The clobber insns make the compiler to issue save/restore
         insns for every clobbered register in the caller.  */
      /* spu_clobber_dont_save_regs (operands[0]); */
   }")

(define_insn "sibcall_internal"
  [(parallel
    [(call (match_operand:QI 0 "sibcall_operand" "T,S")
	   (match_operand:QI 1 "" "i,i"))
     (use (reg:SI 0))])]
  "SIBLING_CALL_P(insn)"
  {
      return which_alternative==0 ? "bi\t%i0" : "br\t%0";
  }
   [(set_attr "type" "br")])

(define_expand "sibcall_value"
  [(parallel
    [(set (match_operand 0 "" "")
	  (call (match_operand:QI 1 "sibcall_operand" "")
		(match_operand:QI 2 "" "")))
     (use (reg:SI 0))])]
  ""
  "{
      if (! call_operand (operands[1], QImode))
        XEXP (operands[1], 0) = copy_to_mode_reg (Pmode, XEXP (operands[1], 0));
      /* same as above in \"call\".  */
      /* spu_clobber_dont_save_regs (operands[1]); */
   }")

(define_insn "sibcall_value_internal"
  [(parallel
    [(set (match_operand 0 "" "")
	  (call (match_operand:QI 1 "sibcall_operand" "T,S")
		(match_operand:QI 2 "" "i,i")))
     (use (reg:SI 0))])]
  "SIBLING_CALL_P(insn)"
  {
      return which_alternative==0 ? "bi\t%i1" : "br\t%1";
  }
   [(set_attr "type" "br")])

;; Note that operand 1 is total size of args, in bytes,
;; and what the call insn wants is the number of words.
(define_expand "call"
  [(parallel
    [(call (match_operand:QI 0 "call_operand" "")
	   (match_operand:QI 1 "" ""))
     (clobber (reg:SI 0))
     (clobber (reg:SI 130))])]
  ""
  "{
      if (! call_operand (operands[0], QImode))
        XEXP (operands[0], 0) = copy_to_mode_reg (Pmode, XEXP (operands[0], 0));
      /* this function will check whether the callee has
         dont_save_regs attribute attached and will issue clobber
         insns for every register in the range, which was encoded in
         the callee's symbol prefix by the attribute handler.  The
         prefix is stripped when the final assembly is being output.
         The clobber insns make the compiler to issue save/restore
         insns for every clobbered register in the caller.  */
      /* spu_clobber_dont_save_regs (operands[0]); */
   }")

(define_insn "call_internal"
  [(parallel
    [(call (match_operand:QI 0 "call_operand" "T,S")
	   (match_operand:QI 1 "" "i,i"))
     (clobber (reg:SI 0))
     (clobber (reg:SI 130))])]
  ""
  {
      if (GET_CODE (XEXP (operands[0], 0)) == REG)
	return \"bisl\t$lr,%i0\";
      if (GET_CODE (XEXP (operands[0], 0)) == CONST_INT)
	return \"brasl\t$lr,%0\";
      return \"brsl\t$lr,%0\";
  }
   [(set_attr "type" "br")])

(define_expand "call_value"
  [(parallel
    [(set (match_operand 0 "" "")
	  (call (match_operand:QI 1 "call_operand" "")
		(match_operand:QI 2 "" "")))
     (clobber (reg:SI 0))
     (clobber (reg:SI 130))])]
  ""
  "{
      if (! call_operand (operands[1], QImode))
        XEXP (operands[1], 0) = copy_to_mode_reg (Pmode, XEXP (operands[1], 0));
      /* same as above in \"call\".  */
      /* spu_clobber_dont_save_regs (operands[1]); */
   }")

(define_insn "call_value_internal"
  [(parallel
    [(set (match_operand 0 "" "")
	  (call (match_operand:QI 1 "call_operand" "T,S")
		(match_operand:QI 2 "" "i,i")))
     (clobber (reg:SI 0))
     (clobber (reg:SI 130))])]
  ""
  {
      if (GET_CODE (XEXP (operands[1], 0)) == REG)
	return \"bisl\t$lr,%i1\";
      if (GET_CODE (XEXP (operands[1], 0)) == CONST_INT)
	return \"brasl\t$lr,%1\";
      return \"brsl\t$lr,%1\";
  }
   [(set_attr "type" "br")])

(define_expand "untyped_call"
  [(parallel [(call (match_operand 0 "" "")
		    (const_int 0))
	      (match_operand 1 "" "")
	      (match_operand 2 "" "")])]
  ""
{
  int i;
  rtx reg = gen_rtx_REG (TImode, 3);

  /* We need to use call_value so the return value registers don't get
   * clobbered. */
  emit_call_insn (gen_call_value (reg, operands[0], const0_rtx));

  for (i = 0; i < XVECLEN (operands[2], 0); i++)
    {
      rtx set = XVECEXP (operands[2], 0, i);
      emit_move_insn (SET_DEST (set), SET_SRC (set));
    }

  /* The optimizer does not know that the call sets the function value
     registers we stored in the result block.  We avoid problems by
     claiming that all hard registers are used and clobbered at this
     point.  */
  emit_insn (gen_blockage ());

  DONE;
})


;; Patterns used for splitting and combining.

(define_insn "spu_ilh_si"
  [(set (match_operand:SI 0 "register_operand"   "=r")
        (ior:SI (ashift:SI (match_operand:SI 1 "immediate_operand" "K")
                           (const_int 16))
                 (match_dup 1)))]
   ""
   "ilh\t%0,%1")

(define_insn "spu_mpyu_si"
  [(set (match_operand:SI 0 "register_operand"   "=r,r")
        (mult:SI (and:SI (match_operand:SI 1 "register_operand"  "r,r")
                         (const_int 65535))
                 (and:SI (match_operand:SI 2 "rK_operand" "r,K")
                         (const_int 65535))))]
  ""
  "@
   mpyu\t%0,%1,%2
   mpyui\t%0,%1,%2"
  [(set_attr "type" "fp7")])

;; This isn't always profitable to use.  Consider r = a * b + c * d.
;; It's faster to do  the multplies in parallel then add them.  If we
;; merge a multply and add it prevents the multplies from happening in 
;; parallel.
(define_insn "spu_mpya_si"
  [(set (match_operand:SI 0 "register_operand"   "=r")
        (plus:SI (mult:SI (sign_extend:SI (match_operand:HI 1 "register_operand"  "r"))
                          (sign_extend:SI (match_operand:HI 2 "register_operand"  "r")))
                 (match_operand:SI 3 "register_operand"  "r")))]
  "0"
  "mpya\t%0,%1,%2,%3"
  [(set_attr "type" "fp7")])

;;
;; Note the unusual order of numbering of match operands. GCC cannonicalizes
;; RTL of commutative expressions and prefers first operands to be a
;; commutative subexpression. If we chose the lshiftrt subexpression as the
;; first operand, this instruction would be unrecognized after GCC
;; cannonicalized the mult expression.
;;
(define_insn "spu_mpyh_si"
  [(set (match_operand:SI 0 "register_operand"   "=r")
            (ashift:SI (mult:SI (and:SI (match_operand:SI 2 "register_operand"  "r")
				        (const_int 65535))
				(lshiftrt:SI (match_operand:SI 1 "register_operand"  "r")
					     (const_int 16)))
                       (const_int 16)))]
  ""
  "mpyh\t%0,%1,%2"
  [(set_attr "type" "fp7")])

(define_insn "spu_mpys_si"
  [(set (match_operand:SI 0 "register_operand"   "=r")
        (ashiftrt:SI
            (mult:SI (sign_extend:SI (match_operand:HI 1 "register_operand"  "r"))
                     (sign_extend:SI (match_operand:HI 2 "register_operand"  "r")))
            (const_int 16)))]
  ""
  "mpys\t%0,%1,%2"
  [(set_attr "type" "fp7")])

(define_insn "spu_mpyhh_si"
  [(set (match_operand:SI 0 "register_operand"   "=r")
        (mult:SI (ashiftrt:SI (match_operand:SI 1 "register_operand"  "r")
                              (const_int 16))
                 (ashiftrt:SI (match_operand:SI 2 "register_operand"  "r")
                              (const_int 16))))]
  ""
  "mpyhh\t%0,%1,%2"
  [(set_attr "type" "fp7")])

(define_insn "spu_mpyhhu_si"
  [(set (match_operand:SI 0 "register_operand"   "=r")
        (mult:SI (lshiftrt:SI (match_operand:SI 1 "register_operand"  "r")
                              (const_int 16))
                 (lshiftrt:SI (match_operand:SI 2 "register_operand"  "r")
                              (const_int 16))))]
  ""
  "mpyhhu %0,%1,%2"
  [(set_attr "type" "fp7")])

(define_insn "spu_mpyhha_si" 
  [(set (match_operand:SI 0 "register_operand"   "=r")
        (plus:SI (mult:SI (ashiftrt:SI (match_operand:SI 1 "register_operand"  "r")
                                       (const_int 16))
                          (ashiftrt:SI (match_operand:SI 2 "register_operand"  "r")
                                       (const_int 16)))
                 (match_operand:SI 3 "register_operand" "0")))]
  "0"
  "mpyhha\t%0,%1,%2"
  [(set_attr "type" "fp7")])

(define_insn "spu_cg_si"
  [(set (match_operand:SI 0 "register_operand"   "=r")
        (unspec:SI [(match_operand:SI 1 "register_operand"  "r")
                    (match_operand:SI 2 "register_operand"  "r")] UNSPEC_CG))]
  ""
  "cg %0,%1,%2")

(define_insn "spu_addx_si"
  [(set (match_operand:SI 0 "register_operand"   "=r")
        (unspec:SI [(match_operand:SI 1 "register_operand"  "r")
                    (match_operand:SI 2 "register_operand"  "r")
		    (match_operand:SI 3 "register_operand"  "0")] UNSPEC_ADDX))]
  ""
  "addx %0,%1,%2")


;; Vector mode mov patterns

(define_expand "movti"
  [(set (match_operand:TI 0 "nonimmediate_operand" "=r,r,r,m")
        (match_operand:TI 1 "general_operand"       "r,i,m,r"))]
  ""
  "{ if (spu_expand_mov(operands, TImode))
  	DONE;
  }")

(define_expand "movv16qi"
  [(set (match_operand:V16QI 0 "nonimmediate_operand" "=r,r,m,r")
        (match_operand:V16QI 1 "vector_operand"        "r,m,r,i"))]
  ""
  "{ if (spu_expand_mov(operands, V16QImode))
  	DONE;
  }")
  
(define_expand "movv8hi"
  [(set (match_operand:V8HI 0 "nonimmediate_operand" "=r,r,m,r")
        (match_operand:V8HI 1 "vector_operand"        "r,m,r,i"))]
  ""
  "{ if (spu_expand_mov(operands, V8HImode))
  	DONE;
  }")

(define_expand "movv4si"
  [(set (match_operand:V4SI 0 "nonimmediate_operand" "=r,r,m,r")
        (match_operand:V4SI 1 "vector_operand"        "r,m,r,i"))]
  ""
  "{ if (spu_expand_mov(operands, V4SImode))
  	DONE;
  }")

(define_expand "movv2di"
  [(set (match_operand:V2DI 0 "nonimmediate_operand" "=r,r,m,r")
        (match_operand:V2DI 1 "vector_operand"        "r,m,r,i"))]
  ""
  "{ if (spu_expand_mov(operands, V2DImode))
  	DONE;
  }")
  
(define_expand "movv4sf"
  [(set (match_operand:V4SF 0 "nonimmediate_operand" "=r,r,m,r")
        (match_operand:V4SF 1 "vector_operand"        "r,m,r,i"))]
  ""
  "{ if (spu_expand_mov(operands, V4SFmode))
  	DONE;
  }")
  
(define_expand "movv2df"
  [(set (match_operand:V2DF 0 "nonimmediate_operand" "=r,r,m,r")
        (match_operand:V2DF 1 "vector_operand"        "r,m,r,i"))]
  ""
  "{ if (spu_expand_mov(operands, V2DFmode))
  	DONE;
  }")

;; (define_expand "movv2si"
;;   [(set (match_operand:V2SI 0 "nonimmediate_operand" "=r,r,m,r")
;;         (match_operand:V2SI 1 "vector_operand"        "r,m,r,i"))]
;;   ""
;;   "{ if (spu_expand_mov(operands, V2SImode))
;;   	DONE;
;;   }")

(define_insn_and_split "movti_internal"
  [(set (match_operand:TI 0 "nonimmediate_operand" "=r,r,r,m")
        (match_operand:TI 1 "general_operand"      " r,iA,m,r"))]
  "spu_valid_move(operands)"
  "* return spu_emit_move_asm(operands, TImode);"
  "reload_completed && !TARGET_DONT_SPLIT"
  [(set (match_dup:TI 0)
        (match_dup:TI 1))]
  "if (spu_split_move(operands, TImode))
      DONE;"
  [(set_attr "type" "fx2,fx2,load,store")])

(define_insn_and_split "movv16qi_internal"
  [(set (match_operand:V16QI 0 "nonimmediate_operand" "=r,r,m,r,r")
        (match_operand:V16QI 1 "vector_operand"        "r,m,r,U,W"))]
  "spu_valid_move(operands)"
  "* return spu_emit_move_asm(operands, V16QImode); "
  "reload_completed && !TARGET_DONT_SPLIT"
  [(set (match_dup:V16QI 0)
        (match_dup:V16QI 1))]
  "if (spu_split_move(operands, V16QImode))
      DONE;"
  [(set_attr "type" "fx2,load,store,shuf,fx2")])
  
(define_insn_and_split "movv8hi_internal"
  [(set (match_operand:V8HI 0 "nonimmediate_operand" "=r,r,m,r,r")
        (match_operand:V8HI 1 "vector_operand"        "r,m,r,U,W"))]
  "spu_valid_move(operands)"
  "* return spu_emit_move_asm(operands, V8HImode); "
  "reload_completed && !TARGET_DONT_SPLIT"
  [(set (match_dup:V8HI 0)
        (match_dup:V8HI 1))]
  "if (spu_split_move(operands, V8HImode))
      DONE;"
  [(set_attr "type" "fx2,load,store,shuf,fx2")])

(define_insn_and_split "movv4si_internal"
  [(set (match_operand:V4SI 0 "nonimmediate_operand" "=r,r,m,r,r")
        (match_operand:V4SI 1 "vector_operand"        "r,m,r,U,W"))]
  "spu_valid_move(operands)"
  "* return spu_emit_move_asm(operands, V4SImode); "
  "reload_completed && !TARGET_DONT_SPLIT"
  [(set (match_dup:V4SI 0)
        (match_dup:V4SI 1))]
  "if (spu_split_move(operands, V4SImode))
      DONE;"
  [(set_attr "type" "fx2,load,store,shuf,fx2")])

(define_insn_and_split "movv2di_internal"
  [(set (match_operand:V2DI 0 "nonimmediate_operand" "=r,r,m,r,r")
        (match_operand:V2DI 1 "vector_operand"        "r,m,r,U,W"))]
  "spu_valid_move(operands)"
  "* return spu_emit_move_asm(operands, V2DImode); "
  "reload_completed && !TARGET_DONT_SPLIT"
  [(set (match_dup:V2DI 0)
        (match_dup:V2DI 1))]
  "if (spu_split_move(operands, V2DImode))
      DONE;"
  [(set_attr "type" "fx2,load,store,shuf,fx2")])
  
(define_insn_and_split "movv4sf_internal"
  [(set (match_operand:V4SF 0 "nonimmediate_operand" "=r,r,m,r,r")
        (match_operand:V4SF 1 "vector_operand"        "r,m,r,U,W"))]
  "spu_valid_move(operands)"
  "* return spu_emit_move_asm(operands, V4SFmode); "
  "reload_completed && !TARGET_DONT_SPLIT"
  [(set (match_dup:V4SF 0)
        (match_dup:V4SF 1))]
  "if (spu_split_move(operands, V4SFmode))
      DONE;"
  [(set_attr "type" "fx2,load,store,shuf,fx2")])
  
(define_insn_and_split "movv2df_internal"
  [(set (match_operand:V2DF 0 "nonimmediate_operand" "=r,r,m,r,r")
        (match_operand:V2DF 1 "vector_operand"        "r,m,r,U,W"))]
  "spu_valid_move(operands)"
  "* return spu_emit_move_asm(operands, V2DFmode); "
  "reload_completed && !TARGET_DONT_SPLIT"
  [(set (match_dup:V2DI 0)
        (match_dup:V2DI 1))]
  "if (spu_split_move(operands, V2DImode))
      DONE;"
  [(set_attr "type" "fx2,load,store,shuf,fx2")])

;; (define_insn "movv2si_internal"
;;   [(set (match_operand:V2SI 0 "nonimmediate_operand" "=r,r,m")
;;         (match_operand:V2SI 1 "vector_operand"        "r,m,r"))]
;;   "spu_valid_move(operands)"
;;   "* return spu_emit_move_asm(operands, V2SImode); "
;;   [(set_attr "type" "fx2,load,store")])


;; Support for infix operators on vector types.  
;; In most cases these expand to one of the "spu_" patterns following
;; this section.   We only do the easy ones and let GCC generate slow
;; code for the others, e.g., for addv16qi we could generate better
;; code then what GCC does on it's own.
;; GCC supports the following operators on vector types:
;;  + - * / (unary -) & | ~ == !=

(define_expand "addv8hi3"
  [(set (match_operand:V8HI 0 "register_operand"   "")
        (plus:V8HI (match_operand:V8HI 1 "register_operand"   "")
                   (match_operand:V8HI 2 "register_operand" "")))]
  ""
  "")

(define_expand "addv4si3"
  [(set (match_operand:V4SI 0 "register_operand"   "")
        (plus:V4SI (match_operand:V4SI 1 "register_operand" "")
                   (match_operand:V4SI 2 "register_operand" "")))]
  ""
  "")

(define_expand "addv4sf3"
  [(set (match_operand:V4SF 0 "register_operand"   "")
        (plus:V4SF (match_operand:V4SF 1 "register_operand" "")
                   (match_operand:V4SF 2 "register_operand" "")))]
  ""
  "")

(define_expand "addv2df3"
  [(set (match_operand:V2DF 0 "register_operand"   "")
        (plus:V2DF (match_operand:V2DF 1 "register_operand" "")
                   (match_operand:V2DF 2 "register_operand" "")))]
  ""
  "")

(define_expand "subv8hi3"
  [(set (match_operand:V8HI 0 "register_operand"   "")
        (minus:V8HI (match_operand:V8HI 1 "register_operand"   "")
                    (match_operand:V8HI 2 "register_operand" "")))]
  ""
  "")

(define_expand "subv4si3"
  [(set (match_operand:V4SI 0 "register_operand"   "")
        (minus:V4SI (match_operand:V4SI 1 "register_operand" "")
                    (match_operand:V4SI 2 "register_operand" "")))]
  ""
  "")

(define_expand "subv4sf3"
  [(set (match_operand:V4SF 0 "register_operand"   "")
        (minus:V4SF (match_operand:V4SF 1 "register_operand" "")
                    (match_operand:V4SF 2 "register_operand" "")))]
  ""
  "")

(define_expand "subv2df3"
  [(set (match_operand:V2DF 0 "register_operand"   "")
        (minus:V2DF (match_operand:V2DF 1 "register_operand" "")
                    (match_operand:V2DF 2 "register_operand" "")))]
  ""
  "")

(define_insn "negv8hi2"
  [(set (match_operand:V8HI 0 "register_operand"   "=r")
        (neg:V8HI (match_operand:V8HI 1 "register_operand"   "r")))]
  ""
  "sfhi\t%0,%1,0")

(define_insn "negv4si2"
  [(set (match_operand:V4SI 0 "register_operand"   "=r")
        (neg:V4SI (match_operand:V4SI 1 "register_operand" "r")))]
  ""
  "sfi\t%0,%1,0")

(define_insn_and_split "negv4sf2"
  [(set (match_operand:V4SF 0 "register_operand" "=r")
        (neg:V4SF (match_operand:V4SF 1 "register_operand" "r")))
   (clobber (match_scratch:V4SI 2 "=&r"))]
  ""
  "ilhu\t%2,0x8000\;xor\t%0,%1,%2"
  "reload_completed && !TARGET_DONT_SPLIT"
  [(set (match_dup:V4SF 0)
	(neg:V4SF (match_dup:V4SF 1)))
   (clobber (match_dup:V4SI 2))]
  "{
     rtx to = gen_rtx_REG(V4SImode, REGNO(operands[0]));
     rtx from = gen_rtx_REG(V4SImode, REGNO(operands[1]));
     emit_insn(gen_movv4si(operands[2], spu_const_vector(V4SImode, GEN_INT(-1<<31))));
     spu_emit_insn(gen_spu_xor(to, from, operands[2]));
     DONE;
   }"
  [(set_attr "length" "8")])

(define_insn_and_split "negv2df2"
  [(set (match_operand:V2DF 0 "register_operand" "=r")
        (neg:V2DF (match_operand:V2DF 1 "register_operand" "r")))
   (clobber (match_scratch:V16QI 2 "=&r"))]
  ""
  "fsmbi\t%2,0x8080\;andbi\t%2,%2,0x80\;xor\t%0,%1,%2"
  "reload_completed && !TARGET_DONT_SPLIT"
  [(set (match_dup:DF 0)
	(neg:DF (match_dup:DF 1)))
   (clobber (match_dup:V16QI 2))]
  "{
     rtx to = gen_rtx_REG(V16QImode, REGNO(operands[0]));
     rtx from = gen_rtx_REG(V16QImode, REGNO(operands[1]));
     spu_emit_insn(gen_spu_fsmb(operands[2], GEN_INT(0x8080)));
     spu_emit_insn(gen_spu_and(operands[2], operands[2], spu_const_vector(V16QImode, GEN_INT(0x80))));
     spu_emit_insn(gen_spu_xor(to, from, operands[2]));
     DONE;
   }"
  [(set_attr "length" "12")])


(define_expand "mulv4sf3"
  [(set (match_operand:V4SF 0 "register_operand"   "")
        (mult:V4SF (match_operand:V4SF 1 "register_operand" "")
                   (match_operand:V4SF 2 "register_operand" "")))]
  ""
  "")

(define_expand "mulv2df3"
  [(set (match_operand:V2DF 0 "register_operand"   "")
        (mult:V2DF (match_operand:V2DF 1 "register_operand" "")
                   (match_operand:V2DF 2 "register_operand" "")))]
  ""
  "")

(define_expand "iorv16qi3"
  [(set (match_operand:V16QI 0 "register_operand"   "")
        (ior:V16QI (match_operand:V16QI 1 "register_operand"   "")
                   (match_operand:V16QI 2 "register_operand" "")))]
  ""
  "")

(define_expand "iorv8hi3"
  [(set (match_operand:V8HI 0 "register_operand"   "")
        (ior:V8HI (match_operand:V8HI 1 "register_operand"   "")
                  (match_operand:V8HI 2 "register_operand" "")))]
  ""
  "")

(define_expand "iorv4si3"
  [(set (match_operand:V4SI 0 "register_operand"   "")
        (ior:V4SI (match_operand:V4SI 1 "register_operand" "")
                  (match_operand:V4SI 2 "register_operand" "")))]
  ""
  "")

(define_expand "iorv2di3"
  [(set (match_operand:V2DI 0 "register_operand"   "")
        (ior:V2DI (match_operand:V2DI 1 "register_operand" "")
                  (match_operand:V2DI 2 "register_operand" "")))]
  ""
  "")

(define_expand "andv16qi3"
  [(set (match_operand:V16QI 0 "register_operand"   "")
        (and:V16QI (match_operand:V16QI 1 "register_operand"   "")
                   (match_operand:V16QI 2 "register_operand" "")))]
  ""
  "")

(define_expand "andv8hi3"
  [(set (match_operand:V8HI 0 "register_operand"   "")
        (and:V8HI (match_operand:V8HI 1 "register_operand"   "")
                  (match_operand:V8HI 2 "register_operand" "")))]
  ""
  "")

(define_expand "andv4si3"
  [(set (match_operand:V4SI 0 "register_operand"   "")
        (and:V4SI (match_operand:V4SI 1 "register_operand" "")
                  (match_operand:V4SI 2 "register_operand" "")))]
  ""
  "")

(define_expand "andv2di3"
  [(set (match_operand:V2DI 0 "register_operand"   "")
        (and:V2DI (match_operand:V2DI 1 "register_operand" "")
                  (match_operand:V2DI 2 "register_operand" "")))]
  ""
  "")

(define_expand "xorv16qi3"
  [(set (match_operand:V16QI 0 "register_operand"   "")
        (xor:V16QI (match_operand:V16QI 1 "register_operand"   "")
                   (match_operand:V16QI 2 "register_operand" "")))]
  ""
  "")

(define_expand "xorv8hi3"
  [(set (match_operand:V8HI 0 "register_operand"   "")
        (xor:V8HI (match_operand:V8HI 1 "register_operand"   "")
                  (match_operand:V8HI 2 "register_operand" "")))]
  ""
  "")

(define_expand "xorv4si3"
  [(set (match_operand:V4SI 0 "register_operand"   "")
        (xor:V4SI (match_operand:V4SI 1 "register_operand" "")
                  (match_operand:V4SI 2 "register_operand" "")))]
  ""
  "")

(define_expand "xorv2di3"
  [(set (match_operand:V2DI 0 "register_operand"   "")
        (xor:V2DI (match_operand:V2DI 1 "register_operand" "")
                  (match_operand:V2DI 2 "register_operand" "")))]
  ""
  "")

(define_insn "one_cmplv16qi2"
  [(set (match_operand:V16QI 0 "register_operand"   "=r")
        (not:V16QI (match_operand:V16QI 1 "register_operand"   "r")))]
  ""
  "nor\t%0,%1,%1")

(define_insn "one_cmplv8hi2"
  [(set (match_operand:V8HI 0 "register_operand"   "=r")
        (not:V8HI (match_operand:V8HI 1 "register_operand"   "r")))]
  ""
  "nor\t%0,%1,%1")

(define_insn "one_cmplv4si2"
  [(set (match_operand:V4SI 0 "register_operand"   "=r")
        (not:V4SI (match_operand:V4SI 1 "register_operand" "r")))]
  ""
  "nor\t%0,%1,%1")

(define_insn "one_cmplv2di2"
  [(set (match_operand:V2DI 0 "register_operand"   "=r")
        (not:V2DI (match_operand:V2DI 1 "register_operand" "r")))]
  ""
  "nor\t%0,%1,%1")

(define_expand "cmpv16qi"
  [(set (cc0)
        (compare (match_operand:V16QI 0 "register_operand"   "")
                 (match_operand:V16QI 1 "register_operand" "")))]
  ""
  "{
    spu_compare_op0 = operands[0];
    spu_compare_op1 = operands[1];
    DONE;
   }")

(define_expand "cmpv8hi"
  [(set (cc0)
        (compare (match_operand:V8HI 0 "register_operand"   "")
                 (match_operand:V8HI 1 "register_operand" "")))]
  ""
  "{
    spu_compare_op0 = operands[0];
    spu_compare_op1 = operands[1];
    DONE;
   }")

(define_expand "cmpv4si"
  [(set (cc0)
        (compare (match_operand:V4SI 0 "register_operand" "")
                 (match_operand:V4SI 1 "register_operand" "")))]
  ""
  "{
    spu_compare_op0 = operands[0];
    spu_compare_op1 = operands[1];
    DONE;
   }")

(define_expand "cmpv2di"
  [(set (cc0)
        (compare (match_operand:V2DI 0 "register_operand" "")
                 (match_operand:V2DI 1 "register_operand" "")))]
  ""
  "{
    spu_compare_op0 = operands[0];
    spu_compare_op1 = operands[1];
    DONE;
   }")

(define_expand "cmpv4sf"
  [(set (cc0)
        (compare (match_operand:V4SF 0 "register_operand" "")
                 (match_operand:V4SF 1 "register_operand" "")))]
  ""
  "{
    spu_compare_op0 = operands[0];
    spu_compare_op1 = operands[1];
    DONE;
   }")

(define_expand "cmpv2df"
  [(set (cc0)
        (compare (match_operand:V2DF 0 "register_operand" "")
                 (match_operand:V2DF 1 "register_operand" "")))]
  ""
  "{
    spu_compare_op0 = operands[0];
    spu_compare_op1 = operands[1];
    DONE;
   }")

(define_insn_and_split "ceq_vec"
  [(set (match_operand:SI 0 "register_operand" "=r")
        (eq:SI (match_operand 1 "register_operand" "r")
	       (match_operand 2 "register_operand" "r")))]
  "VECTOR_MODE_P(GET_MODE(operands[1]))
   && GET_MODE(operands[1]) == GET_MODE(operands[2])"
  "ceq\t%0,%1,%2\;gb\t%0,%0\;ceqi\t%0,%0,15"
  "&& reload_completed && !TARGET_DONT_SPLIT"
  [(set (match_dup:SI 0)
        (eq:SI (match_dup 1)
	       (match_dup 2)))]
  "{
     rtx to_v4 = gen_rtx_REG(V4SImode, REGNO(operands[0]));
     rtx l_v4 = gen_rtx_REG(V4SImode, REGNO(operands[1]));
     rtx r_v4 = gen_rtx_REG(V4SImode, REGNO(operands[2]));
     rtx to_si = gen_rtx_REG(SImode, REGNO(operands[0]));
     spu_emit_insn(gen_spu_ceq(to_v4, l_v4, r_v4));
     spu_emit_insn(gen_spu_gb(to_v4, to_v4));
     emit_insn(gen_ceq_si(to_si, to_si, GEN_INT(15)));
     DONE;
   }"
  [(set_attr "length" "12")])


;; Following are all the instructions which have an intrinsic. 

;; load/store

(define_expand "spu_lqd"
  [(set (match_operand:V16QI 0 "register_operand"  "=r,r")
        (mem:V16QI (and:SI (plus:SI (match_operand:SI  1 "register_operand"  "r,r")
                                    (match_operand:SI  2 "nonmemory_operand" "L,r"))
                           (const_int -16))))]
  ""
  "{  int warn_bits = 0;
      if (GET_CODE(operands[2]) == CONST_INT
          && (INTVAL(operands[2]) & 15) != 0)
	{
	  warn_bits = 1;
	  operands[2] = GEN_INT(INTVAL(operands[2]) & -16);
	}
      if (GET_CODE(operands[2]) != CONST_INT)
	{
	  rtx op2 = operands[2];
	  operands[2] = force_reg (Pmode, operands[2]);
	  if (! ALIGNED_SYMBOL_REF_P (op2))
	    {
	      warn_bits = 1;
	      emit_insn (gen_andsi3(operands[2],operands[2],GEN_INT(-16)));
	    }
	}
      if (warn_bits)
        warning(\"si_lqd: 4 least significant bits of second operand set to 0\n\");
   }")

(define_expand "spu_lqx"
  [(set (match_operand:V16QI 0 "register_operand"  "=r")
        (mem:V16QI (and:SI (plus:SI (match_operand:SI  1 "register_operand" "r")
                                    (match_operand:SI  2 "register_operand" "r"))
                           (const_int -16))))]
  ""
  "")

(define_insn "spu_lqa"
  [(set (match_operand:V16QI 0 "register_operand"  "=r")
        (mem:V16QI (and:SI (match_operand:SI  1 "address_operand" "p")
                           (const_int -16))))]
  ""
  {
      return "lq%f1\t%0,%a1";
  }
  [(set_attr "type" "load")])

(define_insn "spu_lqr"
  [(parallel
    [(set (match_operand:V16QI 0 "register_operand"  "=r")
	  (mem:V16QI (and:SI (match_operand:SI  1 "pic_address_operand" "s")
			     (const_int -16))))
     (use (const_int 0))])]
  ""
  {
      return "lqr\t%0,%a1";
  }
  [(set_attr "type" "load")])

(define_expand "spu_stqd"
  [(set (mem:V16QI (and:SI (plus:SI (match_operand:SI  1 "register_operand"  "r,r")
                                    (match_operand:SI  2 "nonmemory_operand" "L,r"))
                           (const_int -16)))
        (match_operand:V16QI 0 "register_operand"  "r,r"))]
  ""
  "{  int warn_bits = 0;
      if (GET_CODE(operands[2]) == CONST_INT
          && (INTVAL(operands[2]) & 15) != 0)
	{
	  warn_bits = 1;
	  operands[2] = GEN_INT(INTVAL(operands[2]) & -16);
	}
      if (GET_CODE(operands[2]) != CONST_INT)
	{
	  rtx op2 = operands[2];
	  operands[2] = force_reg (Pmode, operands[2]);
	  if (! ALIGNED_SYMBOL_REF_P (op2))
	    {
	      warn_bits = 1;
	      emit_insn (gen_andsi3(operands[2],operands[2],GEN_INT(-16)));
	    }
	}
      if (warn_bits)
        warning(\"si_stqd: 4 least significant bits of third operand set to 0\n\");
   }")

(define_expand "spu_stqx"
  [(set (mem:V16QI (and:SI (plus:SI (match_operand:SI  1 "register_operand" "r")
                                    (match_operand:SI  2 "register_operand" "r"))
                           (const_int -16)))
        (match_operand:V16QI 0 "register_operand"  "r"))]
  ""
  "")

(define_insn "spu_stqa"
  [(set (mem:V16QI (and:SI (match_operand:SI  1 "address_operand" "p")
                           (const_int -16)))
        (match_operand:V16QI 0 "register_operand"  "r"))]
  ""
  {
      return "stq%f1\t%0,%a1";
  }
  [(set_attr "type" "store")])

(define_insn "spu_stqr"
  [(parallel
    [(set (mem:V16QI (and:SI (match_operand:SI  1 "pic_address_operand" "s")
			     (const_int -16)))
	  (match_operand:V16QI 0 "register_operand"  "r"))
    (use (const_int 0))])]
  ""
  {
      return "stqr\t%0,%a1";
  }
  [(set_attr "type" "store")])

;; generate control word

;; Using spu_cbx for spu_cbd

(define_insn "spu_cbx"
  [(set (match_operand 0 "register_operand"  "=r,r")
        (unspec [(match_operand:SI  1 "register_operand"  "r,r")
                 (match_operand:SI  2 "nonmemory_operand"  "Ls,r")] UNSPEC_SPU_CBX))]
  ""
  "@
   cbd\t%0,%2(%1)
   cbx\t%0,%1,%2"
  [(set_attr "type" "shuf")])

;; Using spu_chx for spu_chd

(define_insn "spu_chx"
  [(set (match_operand 0 "register_operand"  "=r,r")
        (unspec [(match_operand:SI  1 "register_operand"  "r,r")
                 (match_operand:SI  2 "nonmemory_operand"  "Ls,r")] UNSPEC_SPU_CHX))]
  ""
  "@
   chd\t%0,%2(%1)
   chx\t%0,%1,%2"
  [(set_attr "type" "shuf")])

;; Using spu_cwx for spu_cwd

(define_insn "spu_cwx"
  [(set (match_operand 0 "register_operand"  "=r,r")
        (unspec [(match_operand:SI  1 "register_operand"  "r,r")
                 (match_operand:SI  2 "nonmemory_operand"  "Ls,r")] UNSPEC_SPU_CWX))]
  ""
  "@
   cwd\t%0,%2(%1)
   cwx\t%0,%1,%2"
  [(set_attr "type" "shuf")])

;; Using spu_cdx for spu_cdd

(define_insn "spu_cdx"
  [(set (match_operand 0 "register_operand"  "=r,r")
        (unspec [(match_operand:SI  1 "register_operand"  "r,r")
                 (match_operand:SI  2 "nonmemory_operand"  "Ls,r")] UNSPEC_SPU_CDX))]
  ""
  "@
   cdd\t%0,%2(%1)
   cdx\t%0,%1,%2"
  [(set_attr "type" "shuf")])

;; Constant formation

(define_expand "spu_ilh"
  [(set (match_operand:V8HI 0 "register_operand"  "=")
        (const_vector:V8HI [(match_operand:HI  1 "immediate_operand"  "")]))]
  ""
  "{ emit_insn(gen_movv8hi(operands[0], spu_const_vector(V8HImode, operands[1])));
     DONE;
   }")

(define_expand "spu_ilhu"
  [(set (match_operand:V4SI 0 "register_operand"  "=")
        (const_vector:V4SI [(match_operand:SI  1 "immediate_operand"  "")]))]
  ""
  "{ emit_insn(gen_movv4si(operands[0], spu_const_vector(V4SImode, GEN_INT(INTVAL(operands[1]) << 16))));
     DONE;
   }")

(define_expand "spu_il"
  [(set (match_operand:V4SI 0 "register_operand"  "=")
        (const_vector:V4SI [(match_operand:SI  1 "immediate_operand"  "")]))]
  ""
  "{ emit_insn(gen_movv4si(operands[0], spu_const_vector(V4SImode, operands[1])));
     DONE;
   }")

;; Using spu_il for spu_ila

(define_expand "spu_iohl"
  [(set (match_operand:V4SI 0 "register_operand"  "=")
        (ior:V4SI (match_operand:V4SI 1 "register_operand"  "")
                  (const_vector:V4SI [(match_operand:SI  2 "immediate_operand"  "")])))]
  ""
  "{ spu_emit_insn(gen_spu_iohl_internal(operands[0], operands[1], spu_const_vector(V4SImode, operands[2])));
     DONE;
   }")

(define_insn "spu_iohl_internal"
  [(set (match_operand:V4SI 0 "register_operand" "=r")
	(ior:V4SI (match_operand:V4SI 1 "register_operand" "0")
	          (match_operand:V4SI 2 "vec_iohl_operand" "W")))]
  ""
  "iohl\t%0,%2")

;; Using spu_fsmb for spu_fsmbi.

;; integer add
(define_insn "spu_ah"
  [(set (match_operand:V8HI 0 "register_operand"   "=r,r")
        (plus:V8HI (match_operand:V8HI 1 "register_operand"   "r,r")
                   (match_operand:V8HI 2 "vec_regimm_operand" "r,Y")))]
  ""
  "@
   ah\t%0,%1,%2
   ahi\t%0,%1,%2")

(define_expand "spu_ahi"
  [(set (match_operand:V8HI 0 "register_operand"   "=")
        (plus:V8HI (match_operand:V8HI 1 "register_operand"  "")
                   (const_vector:V8HI [(match_operand:HI  2 "immediate_operand"  "")])))]
  ""
  "{ spu_emit_insn(gen_spu_ah(operands[0], operands[1], spu_const_vector(V8HImode, operands[2])));
     DONE;
   }")

(define_insn "spu_a"
  [(set (match_operand:V4SI 0 "register_operand"   "=r,r")
        (plus:V4SI (match_operand:V4SI 1 "register_operand"   "r,r")
                   (match_operand:V4SI 2 "vec_regimm_operand" "r,Y")))]
  ""
  "@
   a\t%0,%1,%2
   ai\t%0,%1,%2")

(define_expand "spu_ai"
  [(set (match_operand:V4SI 0 "register_operand"   "=")
        (plus:V4SI (match_operand:V4SI 1 "register_operand"  "")
                   (const_vector:V4SI [(match_operand:SI 2 "immediate_operand"  "")])))]
  ""
  "{ spu_emit_insn(gen_spu_a(operands[0], operands[1], spu_const_vector(V4SImode, operands[2])));
     DONE;
   }")

(define_insn "spu_addx"
  [(set (match_operand 0 "register_operand"   "=r")
        (unspec [(match_operand 1 "register_operand"  "r")
                 (match_operand 2 "register_operand"  "r")
		 (match_operand 3 "register_operand"  "0")] UNSPEC_SPU_ADDX))]
  ""
  "addx\t%0,%1,%2")

(define_insn "spu_cg"
  [(set (match_operand 0 "register_operand"   "=r")
        (unspec [(match_operand 1 "register_operand"  "r")
                 (match_operand 2 "register_operand"  "r")] UNSPEC_SPU_CG))]
  ""
  "cg\t%0,%1,%2")

(define_insn "spu_cgx"
  [(set (match_operand 0 "register_operand"   "=r")
        (unspec [(match_operand 1 "register_operand"  "r")
                 (match_operand 2 "register_operand"  "r")
		 (match_operand 3 "register_operand"  "0")] UNSPEC_SPU_CGX))]
  ""
  "cgx\t%0,%1,%2")

;; integer subtract
(define_insn "spu_sfh"
  [(set (match_operand:V8HI 0 "register_operand"   "=r,r")
        (minus:V8HI (match_operand:V8HI 1 "vec_regimm_operand" "r,Y")
                    (match_operand:V8HI 2 "register_operand"   "r,r")))]
  ""
  "@
   sfh\t%0,%2,%1
   sfhi\t%0,%2,%1")

(define_expand "spu_sfhi"
  [(set (match_operand:V8HI 0 "register_operand"   "=")
        (minus:V8HI (const_vector:V8HI [(match_operand:HI 1 "immediate_operand"  "")])
                    (match_operand:V8HI 2 "register_operand"  "")))]
  ""
  "{ spu_emit_insn(gen_spu_sfh(operands[0], spu_const_vector(V8HImode, operands[1]), operands[2]));
     DONE;
   }")

(define_insn "spu_sf"
  [(set (match_operand:V4SI 0 "register_operand"   "=r,r")
        (minus:V4SI (match_operand:V4SI 1 "vec_regimm_operand" "r,Y")
                    (match_operand:V4SI 2 "register_operand"   "r,r")))]
  ""
  "@
  sf\t%0,%2,%1
  sfi\t%0,%2,%1")

(define_expand "spu_sfi"
  [(set (match_operand:V4SI 0 "register_operand"   "=")
        (minus:V4SI (const_vector:V4SI [(match_operand:SI 1 "immediate_operand"  "")])
                    (match_operand:V4SI 2 "register_operand"  "")))]
  ""
  "{ spu_emit_insn(gen_spu_sf(operands[0], spu_const_vector(V4SImode, operands[1]), operands[2]));
     DONE;
   }")

(define_insn "spu_sfx"
  [(set (match_operand 0 "register_operand"   "=r")
        (unspec [(match_operand 1 "register_operand"  "r")
                 (match_operand 2 "register_operand"  "r")
		 (match_operand 3 "register_operand"  "0")] UNSPEC_SPU_SFX))]
  ""
  "sfx\t%0,%2,%1")

(define_insn "spu_bg"
  [(set (match_operand 0 "register_operand"   "=r")
        (unspec [(match_operand 1 "register_operand"  "r")
                 (match_operand 2 "register_operand"  "r")] UNSPEC_SPU_BG))]
  ""
  "bg\t%0,%2,%1")

(define_insn "spu_bgx"
  [(set (match_operand 0 "register_operand"   "=r")
        (unspec [(match_operand 1 "register_operand"  "r")
                 (match_operand 2 "register_operand"  "r")
		 (match_operand 3 "register_operand"  "0")] UNSPEC_SPU_BGX))]
  ""
  "bgx\t%0,%2,%1")

;; integer multiply
(define_insn "spu_mpy"
  [(set (match_operand:V4SI 0 "register_operand"   "=r,r")
        (mult:V4SI
	  (sign_extend:V4SI
	    (vec_select:V4HI
	      (match_operand:V8HI 1 "register_operand" "r,r")
	      (parallel [(const_int 1)(const_int 3)(const_int 5)(const_int 7)])))
          (sign_extend:V4SI
	    (vec_select:V4HI
	      (match_operand:V8HI 2 "vec_mpy_operand"  "r,Z")
	      (parallel [(const_int 1)(const_int 3)(const_int 5)(const_int 7)])))))]
  ""
  "@
   mpy\t%0,%1,%2
   mpyi\t%0,%1,%O2"
  [(set_attr "type" "fp7")])

(define_insn "spu_mpyu"
  [(set (match_operand:V4SI 0 "register_operand"   "=r,r")
        (mult:V4SI
	  (zero_extend:V4SI
	    (vec_select:V4HI
	      (match_operand:V8HI 1 "register_operand" "r,r")
	      (parallel [(const_int 1)(const_int 3)(const_int 5)(const_int 7)])))
          (zero_extend:V4SI
	    (vec_select:V4HI
	      (match_operand:V8HI 2 "vec_mpy_operand"  "r,Z")
	      (parallel [(const_int 1)(const_int 3)(const_int 5)(const_int 7)])))))]
  ""
  "@
   mpyu\t%0,%1,%2
   mpyui\t%0,%1,%O2"
  [(set_attr "type" "fp7")])

(define_expand "spu_mpyi"
  [(clobber (match_operand:V4SI 0 "register_operand"   "="))
   (use (match_operand:V8HI 1 "register_operand" ""))
   (use (match_operand:HI 2 "immediate_operand" ""))]
  ""
  "{ spu_emit_insn(gen_spu_mpy(operands[0], operands[1], spu_const_vector(V8HImode, operands[2])));
     DONE;
   }")

(define_expand "spu_mpyui"
  [(clobber (match_operand:V4SI 0 "register_operand"   "="))
   (use (match_operand:V8HI 1 "register_operand"  ""))
   (use (match_operand:HI 2 "immediate_operand" ""))]
  ""
  "{ spu_emit_insn(gen_spu_mpyu(operands[0], operands[1], spu_const_vector(V8HImode, operands[2])));
     DONE;
   }")

(define_insn "spu_mpya"
  [(set (match_operand:V4SI 0 "register_operand"   "=r")
        (plus:V4SI
	  (mult:V4SI
	    (sign_extend:V4SI
	      (vec_select:V4HI
		(match_operand:V8HI 1 "register_operand" "r")
		(parallel [(const_int 1)(const_int 3)(const_int 5)(const_int 7)])))
	    (sign_extend:V4SI
	      (vec_select:V4HI
		(match_operand:V8HI 2 "register_operand" "r")
		(parallel [(const_int 1)(const_int 3)(const_int 5)(const_int 7)]))))
	(match_operand:V4SI 3 "register_operand"  "r")))]
  ""
  "mpya\t%0,%1,%2,%3"
  [(set_attr "type" "fp7")])

(define_insn "spu_mpyh"
  [(set (match_operand:V4SI 0 "register_operand"   "=r")
        (ashift:V4SI
	  (mult:V4SI
	    (sign_extend:V4SI
	      (vec_select:V4HI
		(match_operand:V8HI 1 "register_operand" "r")
		(parallel [(const_int 0)(const_int 2)(const_int 4)(const_int 6)])))
	    (sign_extend:V4SI
	      (vec_select:V4HI
		(match_operand:V8HI 2 "register_operand" "r")
		(parallel [(const_int 1)(const_int 3)(const_int 5)(const_int 7)]))))
	  (const_vector:V4SI [(const_int 16)(const_int 16)(const_int 16)(const_int 16)])))]
  ""
  "mpyh\t%0,%1,%2"
  [(set_attr "type" "fp7")])

(define_insn "spu_mpys"
  [(set (match_operand:V4SI 0 "register_operand"   "=r")
        (ashiftrt:V4SI
	  (mult:V4SI
	    (sign_extend:V4SI
	      (vec_select:V4HI
		(match_operand:V8HI 1 "register_operand" "r")
		(parallel [(const_int 1)(const_int 3)(const_int 5)(const_int 7)])))
	    (sign_extend:V4SI
	      (vec_select:V4HI
		(match_operand:V8HI 2 "register_operand" "r")
		(parallel [(const_int 1)(const_int 3)(const_int 5)(const_int 7)]))))
	  (const_vector:V4SI [(const_int 16)(const_int 16)(const_int 16)(const_int 16)])))]
  ""
  "mpys\t%0,%1,%2"
  [(set_attr "type" "fp7")])

(define_insn "spu_mpyhhu"
  [(set (match_operand:V4SI 0 "register_operand"   "=r")
	(mult:V4SI
	  (zero_extend:V4SI
	    (vec_select:V4HI
	      (match_operand:V8HI 1 "register_operand" "r")
	      (parallel [(const_int 0)(const_int 2)(const_int 4)(const_int 6)])))
	  (zero_extend:V4SI
	    (vec_select:V4HI
	      (match_operand:V8HI 2 "register_operand" "r")
	      (parallel [(const_int 0)(const_int 2)(const_int 4)(const_int 6)])))))]
  ""
  "mpyhhu\t%0,%1,%2"
  [(set_attr "type" "fp7")])

(define_insn "spu_mpyhh"
  [(set (match_operand:V4SI 0 "register_operand"   "=r")
	(mult:V4SI
	  (sign_extend:V4SI
	    (vec_select:V4HI
	      (match_operand:V8HI 1 "register_operand" "r")
	      (parallel [(const_int 0)(const_int 2)(const_int 4)(const_int 6)])))
	  (sign_extend:V4SI
	    (vec_select:V4HI
	      (match_operand:V8HI 2 "register_operand" "r")
	      (parallel [(const_int 0)(const_int 2)(const_int 4)(const_int 6)])))))]
  ""
  "mpyhh\t%0,%1,%2"
  [(set_attr "type" "fp7")])

(define_insn "spu_mpyhhau"
  [(set (match_operand:V4SI 0 "register_operand"   "=r")
        (plus:V4SI
	  (mult:V4SI
	    (zero_extend:V4SI
	      (vec_select:V4HI
		(match_operand:V8HI 1 "register_operand" "r")
		(parallel [(const_int 0)(const_int 2)(const_int 4)(const_int 6)])))
	    (zero_extend:V4SI
	      (vec_select:V4HI
		(match_operand:V8HI 2 "register_operand" "r")
		(parallel [(const_int 0)(const_int 2)(const_int 4)(const_int 6)]))))
	  (match_operand:V4SI 3 "register_operand" "0")))]
  ""
  "mpyhhau\t%0,%1,%2"
  [(set_attr "type" "fp7")])

(define_insn "spu_mpyhha"
  [(set (match_operand:V4SI 0 "register_operand"   "=r")
        (plus:V4SI
	  (mult:V4SI
	    (sign_extend:V4SI
	      (vec_select:V4HI
		(match_operand:V8HI 1 "register_operand" "r")
		(parallel [(const_int 0)(const_int 2)(const_int 4)(const_int 6)])))
	    (sign_extend:V4SI
	      (vec_select:V4HI
		(match_operand:V8HI 2 "register_operand" "r")
		(parallel [(const_int 0)(const_int 2)(const_int 4)(const_int 6)]))))
	  (match_operand:V4SI 3 "register_operand" "0")))]
  ""
  "mpyhha\t%0,%1,%2"
  [(set_attr "type" "fp7")])

;; count zeros/bits
(define_insn "spu_clz"
  [(set (match_operand 0 "register_operand"   "=r")
        (unspec [(match_operand 1 "register_operand" "r")] UNSPEC_SPU_CLZ))]
  ""
  "clz\t%0,%1")

(define_insn "spu_cntb"
  [(set (match_operand 0 "register_operand"   "=r")
        (unspec [(match_operand 1 "register_operand" "r")] UNSPEC_SPU_CNTB))]
  ""
  "cntb\t%0,%1"
  [(set_attr "type" "fxb")])

;; form select mask
(define_insn "spu_fsmb"
  [(set (match_operand 0 "register_operand"   "=r,r")
        (unspec [(match_operand:SI 1 "nonmemory_operand"  "r,MN")] UNSPEC_SPU_FSMB))]
  ""
  "@
  fsmb\t%0,%1
  fsmbi\t%0,%1"
  [(set_attr "type" "shuf")])

(define_insn "spu_fsmh"
  [(set (match_operand 0 "register_operand"   "=r")
        (unspec [(match_operand:SI 1 "register_operand" "r")] UNSPEC_SPU_FSMH))]
  ""
  "fsmh\t%0,%1"
  [(set_attr "type" "shuf")])

(define_insn "spu_fsm"
  [(set (match_operand 0 "register_operand"   "=r")
        (unspec [(match_operand:SI 1 "register_operand" "r")] UNSPEC_SPU_FSM))]
  ""
  "fsm\t%0,%1"
  [(set_attr "type" "shuf")])


;; gather bits
(define_insn "spu_gbb"
  [(set (match_operand 0 "register_operand"   "=r")
        (unspec [(match_operand 1 "register_operand" "r")] UNSPEC_SPU_GBB))]
  ""
  "gbb\t%0,%1"
  [(set_attr "type" "shuf")])

(define_insn "spu_gbh"
  [(set (match_operand 0 "register_operand"   "=r")
        (unspec [(match_operand 1 "register_operand" "r")] UNSPEC_SPU_GBH))]
  ""
  "gbh\t%0,%1"
  [(set_attr "type" "shuf")])

(define_insn "spu_gb"
  [(set (match_operand 0 "register_operand"   "=r")
        (unspec [(match_operand 1 "register_operand" "r")] UNSPEC_SPU_GB))]
  ""
  "gb\t%0,%1"
  [(set_attr "type" "shuf")])

;; misc byte operations
(define_insn "spu_avgb"
  [(set (match_operand 0 "register_operand"   "=r")
        (unspec [(match_operand 1 "register_operand" "r")
                 (match_operand 2 "register_operand" "r")] UNSPEC_SPU_AVGB))]
  ""
  "avgb\t%0,%1,%2"
  [(set_attr "type" "fxb")])

(define_insn "spu_absdb"
  [(set (match_operand 0 "register_operand"   "=r")
        (unspec [(match_operand 1 "register_operand" "r")
                 (match_operand 2 "register_operand" "r")] UNSPEC_SPU_ABSDB))]
  ""
  "absdb\t%0,%1,%2"
  [(set_attr "type" "fxb")])

(define_insn "spu_sumb"
  [(set (match_operand 0 "register_operand"   "=r")
        (unspec [(match_operand 1 "register_operand" "r")
                 (match_operand 2 "register_operand" "r")] UNSPEC_SPU_SUMB))]
  ""
  "sumb\t%0,%1,%2"
  [(set_attr "type" "fxb")])

;; sign extend
(define_insn "spu_xsbh"
  [(set (match_operand:V8HI 0 "register_operand"   "=r")
        (sign_extend:V8HI
	  (vec_select:V8QI
	    (match_operand:V16QI 1 "register_operand" "r")
	    (parallel [(const_int 1)(const_int 3)(const_int 5)(const_int 7)
	               (const_int 9)(const_int 11)(const_int 13)(const_int 15)]))))]
  ""
  "xsbh\t%0,%1")

(define_insn "spu_xshw"
  [(set (match_operand:V4SI 0 "register_operand"   "=r")
        (sign_extend:V4SI
	  (vec_select:V4HI
	    (match_operand:V8HI 1 "register_operand" "r")
	    (parallel [(const_int 1)(const_int 3)(const_int 5)(const_int 7)]))))]
  ""
  "xshw\t%0,%1")

(define_insn "spu_xswd"
  [(set (match_operand:V2DI 0 "register_operand"   "=r")
        (sign_extend:V2DI
	  (vec_select:V2SI
	    (match_operand:V4SI 1 "register_operand" "r")
	    (parallel [(const_int 1)(const_int 3)]))))]
  ""
  "xswd\t%0,%1")

;; logical operations

(define_insn "spu_and"
  [(set (match_operand 0 "register_operand" "=r")
	(and (match_operand 1 "register_operand" "r")
	     (match_operand 2 "vec_regimm_operand" "rY")))]
  "SAME_MODES3(operands)"
  "and%I2\t%0,%1,%2")

(define_expand "spu_andc"
  [(set (match_operand 0 "register_operand"  "=r")
	(and (not (match_operand 2 "register_operand"  "r"))
	     (match_operand 1 "register_operand"   "r")))]
  ""
  "{  enum machine_mode mode = GET_MODE(operands[0]);
      emit_insn(gen_rtx_SET(VOIDmode,
                            operands[0],
                            gen_rtx_AND(mode,
                                        gen_rtx_NOT(mode,
                                                    operands[2]),
					operands[1])));
      DONE;
  }")

(define_insn "spu_andc_internal"
  [(set (match_operand 0 "register_operand"  "=r")
	(and (not (match_operand 2 "register_operand"  "r"))
	     (match_operand 1 "register_operand"   "r")))]
  "SAME_MODES3(operands)"
  "andc\t%0,%1,%2")

(define_expand "spu_andbi"
  [(set (match_operand:V16QI 0 "register_operand"   "=")
        (and:V16QI (match_operand:V16QI 1 "register_operand"  "")
                   (const_vector:V16QI [(match_operand:SI 2 "immediate_operand"  "")])))]
  ""
  "{ spu_emit_insn(gen_spu_and(operands[0], operands[1], spu_const_vector(V16QImode, operands[2])));
     DONE;
   }")

(define_expand "spu_andhi"
  [(set (match_operand:V8HI 0 "register_operand"   "=")
        (and:V8HI (match_operand:V8HI 1 "register_operand"  "")
                  (const_vector:V8HI [(match_operand:HI 2 "immediate_operand"  "")])))]
  ""
  "{ spu_emit_insn(gen_spu_and(operands[0], operands[1], spu_const_vector(V8HImode, operands[2])));
     DONE;
   }")

(define_expand "spu_andi"
  [(set (match_operand:V4SI 0 "register_operand"   "=")
        (and:V4SI (match_operand:V4SI 1 "register_operand"  "")
                  (const_vector:V4SI [(match_operand:SI 2 "immediate_operand"  "")])))]
  ""
  "{ spu_emit_insn(gen_spu_and(operands[0], operands[1], spu_const_vector(V4SImode, operands[2])));
     DONE;
   }")

(define_insn "spu_or"
  [(set (match_operand 0 "register_operand" "=r,r")
	(ior (match_operand 1 "register_operand"   "r,0")
	     (match_operand 2 "vec_regimm_operand" "r,Y")))]
  "SAME_MODES3(operands)"
  "or%I2\t%0,%1,%2")

(define_expand "spu_orc"
  [(set (match_operand 0 "register_operand"  "=r")
	(ior (not (match_operand 2 "register_operand"  "r"))
	     (match_operand 1 "register_operand"   "r")))]
  ""
  "{  enum machine_mode mode = GET_MODE(operands[0]);
      emit_insn(gen_rtx_SET(VOIDmode,
                            operands[0],
                            gen_rtx_IOR(mode,
                                        gen_rtx_NOT(mode,
                                                    operands[2]),
					operands[1])));
      DONE;
  }")

(define_insn "spu_orc_internal"
  [(set (match_operand 0 "register_operand"  "=r")
	(ior (not (match_operand 2 "register_operand"  "r"))
	     (match_operand 1 "register_operand"   "r")))]
  "SAME_MODES3(operands)"
  "orc\t%0,%1,%2")

(define_expand "spu_orbi"
  [(set (match_operand:V16QI 0 "register_operand"   "=")
        (ior:V16QI (match_operand:V16QI 1 "register_operand"  "")
                   (const_vector:V16QI [(match_operand:SI 2 "immediate_operand"  "")])))]
  ""
  "{ spu_emit_insn(gen_spu_or(operands[0], operands[1], spu_const_vector(V16QImode, operands[2])));
     DONE;
   }")

(define_expand "spu_orhi"
  [(set (match_operand:V8HI 0 "register_operand"   "=")
        (ior:V8HI (match_operand:V8HI 1 "register_operand"  "")
                  (const_vector:V8HI [(match_operand:HI 2 "immediate_operand"  "")])))]
  ""
  "{ spu_emit_insn(gen_spu_or(operands[0], operands[1], spu_const_vector(V8HImode, operands[2])));
     DONE;
   }")

(define_expand "spu_ori"
  [(set (match_operand:V4SI 0 "register_operand"   "=")
        (ior:V4SI (match_operand:V4SI 1 "register_operand"  "")
                  (const_vector:V4SI [(match_operand:SI 2 "immediate_operand"  "")])))]
  ""
  "{ spu_emit_insn(gen_spu_or(operands[0], operands[1], spu_const_vector(V4SImode, operands[2])));
     DONE;
   }")

(define_insn "spu_orx"
  [(set (match_operand 0 "register_operand"  "=r")
	(unspec [(match_operand 1 "register_operand"  "r")] UNSPEC_SPU_ORX))]
  "SAME_MODES(operands)"
  "orx\t%0,%1")

(define_insn "spu_xor"
  [(set (match_operand 0 "register_operand" "=r")
	(xor (match_operand 1 "register_operand" "r")
	     (match_operand 2 "vec_regimm_operand" "rY")))]
  "SAME_MODES3(operands)"
  "xor%I2\t%0,%1,%2")

(define_expand "spu_xorbi"
  [(set (match_operand:V16QI 0 "register_operand"   "=")
        (xor:V16QI (match_operand:V16QI 1 "register_operand"  "")
                   (const_vector:V16QI [(match_operand:SI 2 "immediate_operand"  "")])))]
  ""
  "{ spu_emit_insn(gen_spu_xor(operands[0], operands[1], spu_const_vector(V16QImode, operands[2])));
     DONE;
   }")

(define_expand "spu_xorhi"
  [(set (match_operand:V8HI 0 "register_operand"   "=")
        (xor:V8HI (match_operand:V8HI 1 "register_operand"  "")
                  (const_vector:V8HI [(match_operand:HI 2 "immediate_operand"  "")])))]
  ""
  "{ spu_emit_insn(gen_spu_xor(operands[0], operands[1], spu_const_vector(V8HImode, operands[2])));
     DONE;
   }")

(define_expand "spu_xori"
  [(set (match_operand:V4SI 0 "register_operand"   "=")
        (xor:V4SI (match_operand:V4SI 1 "register_operand"  "")
                  (const_vector:V4SI [(match_operand:SI 2 "immediate_operand"  "")])))]
  ""
  "{ spu_emit_insn(gen_spu_xor(operands[0], operands[1], spu_const_vector(V4SImode, operands[2])));
     DONE;
   }")

(define_expand "spu_nand"
  [(set (match_operand 0 "register_operand"  "=r")
	(not (and (match_operand 1 "register_operand"  "r")
	          (match_operand 2 "register_operand"  "r"))))]
  ""
  "{  enum machine_mode mode = GET_MODE(operands[0]);
      emit_insn(gen_rtx_SET(VOIDmode,
                            operands[0],
                            gen_rtx_NOT(mode,
                                        gen_rtx_AND(mode,
                                                    operands[1],
                                                    operands[2]))));
      DONE;
  }")

(define_insn "spu_nand_internal"
  [(set (match_operand 0 "register_operand"  "=r")
	(not (and (match_operand 1 "register_operand"  "r")
	          (match_operand 2 "register_operand"  "r"))))]
  "SAME_MODES3(operands)"
  "nand\t%0,%1,%2")

(define_expand "spu_nor"
  [(set (match_operand 0 "register_operand"  "=r")
	(not (ior (match_operand 1 "register_operand"  "r")
	          (match_operand 2 "register_operand"  "r"))))]
  ""
  "{  enum machine_mode mode = GET_MODE(operands[0]);
      emit_insn(gen_rtx_SET(VOIDmode,
                            operands[0],
                            gen_rtx_NOT(mode,
                                        gen_rtx_IOR(mode,
                                                    operands[1],
                                                    operands[2]))));
      DONE;
  }")

(define_insn "spu_nor_internal"
  [(set (match_operand 0 "register_operand"  "=r")
	(not (ior (match_operand 1 "register_operand"  "r")
	          (match_operand 2 "register_operand"  "r"))))]
  "SAME_MODES3(operands)"
  "nor\t%0,%1,%2")


(define_expand "spu_eqv"
  [(set (match_operand 0 "register_operand"  "=r")
	(not (xor (match_operand 1 "register_operand"  "r")
	          (match_operand 2 "register_operand"  "r"))))]
  ""
  "{  enum machine_mode mode = GET_MODE(operands[0]);
      emit_insn(gen_rtx_SET(VOIDmode,
                            operands[0],
                            gen_rtx_NOT(mode,
                                        gen_rtx_XOR(mode,
                                                    operands[1],
                                                    operands[2]))));
      DONE;
  }")

(define_insn "spu_eqv_internal"
  [(set (match_operand 0 "register_operand"  "=r")
	(not (xor (match_operand 1 "register_operand"  "r")
	          (match_operand 2 "register_operand"  "r"))))]
  "SAME_MODES3(operands)"
  "eqv\t%0,%1,%2")

(define_insn "spu_selb"
  [(set (match_operand 0 "register_operand"   "=r")
  	(bit_merge (match_operand 1 "register_operand"   "r")
                   (match_operand 2 "register_operand"   "r")
                   (match_operand 3 "register_operand"   "r")))]
  "SAME_MODES3(operands)"
  "selb\t%0,%1,%2,%3")

(define_insn "spu_shufb"
  [(set (match_operand 0 "register_operand"   "=r")
        (unspec [(match_operand 1 "register_operand"   "r")
                 (match_operand 2 "register_operand"   "r")
                 (match_operand:V16QI 3 "register_operand"  "r")] UNSPEC_SPU_SHUFB))]
  "operands"
  "shufb\t%0,%1,%2,%3"
  [(set_attr "type" "shuf")])

;; shift
(define_insn "spu_shlh"
  [(set (match_operand 0 "register_operand"   "=r,r")
        (unspec [(match_operand 1 "register_operand"   "r,r")
                 (match_operand 2 "vec_regimm_operand" "r,W")] UNSPEC_SPU_SHLH))]
  ""
  "@
   shlh\t%0,%1,%2
   shlhi\t%0,%1,%S2"
  [(set_attr "type" "fx3")])

(define_expand "spu_shlhi"
  [(set (match_operand:V8HI 0 "register_operand"   "=")
        (ashift:V8HI (match_operand:V8HI 1 "register_operand"  "")
                     (const_vector:V8HI [(match_operand:HI 2 "immediate_operand"  "")])))]
  ""
  "{ spu_emit_insn(gen_spu_shlh(operands[0], operands[1], spu_const_vector(V8HImode, operands[2])));
     DONE;
   }")

(define_insn "spu_shl"
  [(set (match_operand 0 "register_operand"   "=r,r")
        (unspec [(match_operand 1 "register_operand"   "r,r")
                 (match_operand 2 "vec_regimm_operand" "r,W")] UNSPEC_SPU_SHL))]
  ""
  "@
   shl\t%0,%1,%2
   shli\t%0,%1,%S2"
  [(set_attr "type" "fx3")])

(define_expand "spu_shli"
  [(set (match_operand:V4SI 0 "register_operand"   "=")
        (ashift:V4SI (match_operand:V4SI 1 "register_operand"  "")
                     (const_vector:V4SI [(match_operand:SI 2 "immediate_operand"  "")])))]
  ""
  "{ spu_emit_insn(gen_spu_shl(operands[0], operands[1], spu_const_vector(V4SImode, operands[2])));
     DONE;
   }")

(define_insn "spu_shlqbi"
  [(set (match_operand 0 "register_operand"   "=r,r")
        (unspec [(match_operand 1 "register_operand"  "r,r")
                 (match_operand:SI 2 "nonmemory_operand" "r,I")] UNSPEC_SPU_SHLQBI))]
  ""
  "@
  shlqbi\t%0,%1,%2
  shlqbii\t%0,%1,%2"
  [(set_attr "type" "shuf")])

;; Using spu_shlqbi for spu_shlqbii

(define_insn "spu_shlqby"
  [(set (match_operand 0 "register_operand"   "=r,r")
        (unspec [(match_operand 1 "register_operand"  "r,r")
                 (match_operand:SI 2 "nonmemory_operand" "r,I")] UNSPEC_SPU_SHLQBY))]
  ""
  "@
  shlqby\t%0,%1,%2
  shlqbyi\t%0,%1,%2"
  [(set_attr "type" "shuf")])

;; Using spu_shlqby for spu_shlqbyi

(define_insn "spu_shlqbybi"
  [(set (match_operand 0 "register_operand"   "=r")
        (unspec [(match_operand 1 "register_operand"  "r")
                 (match_operand:SI 2 "nonmemory_operand" "r")] UNSPEC_SPU_SHLQBYBI))]
  ""
  "shlqbybi\t%0,%1,%2"
  [(set_attr "type" "shuf")])

;; rotate
(define_insn "spu_roth"
  [(set (match_operand:V8HI 0 "register_operand"   "=r,r")
        (rotate:V8HI (match_operand:V8HI 1 "register_operand"   "r,r")
                     (match_operand:V8HI 2 "vec_regimm_operand" "r,W")))]
  ""
  "@
   roth\t%0,%1,%2
   rothi\t%0,%1,%2"
  [(set_attr "type" "fx3")])

(define_expand "spu_rothi"
  [(set (match_operand:V8HI 0 "register_operand"   "=")
        (rotate:V8HI (match_operand:V8HI 1 "register_operand"  "")
                     (const_vector:V8HI [(match_operand:HI 2 "immediate_operand"  "")])))]
  ""
  "{ spu_emit_insn(gen_spu_roth(operands[0], operands[1], spu_const_vector(V8HImode, operands[2])));
     DONE;
   }")

(define_insn "spu_rot"
  [(set (match_operand:V4SI 0 "register_operand"   "=r,r")
        (rotate:V4SI (match_operand:V4SI 1 "register_operand"   "r,r")
                     (match_operand:V4SI 2 "vec_regimm_operand" "r,W")))]
  ""
  "@
   rot\t%0,%1,%2
   roti\t%0,%1,%2"
  [(set_attr "type" "fx3")])

(define_expand "spu_roti"
  [(set (match_operand:V4SI 0 "register_operand"   "=")
        (rotate:V4SI (match_operand:V4SI 1 "register_operand"  "")
                     (const_vector:V4SI [(match_operand:SI 2 "immediate_operand"  "")])))]
  ""
  "{ spu_emit_insn(gen_spu_rot(operands[0], operands[1], spu_const_vector(V4SImode, operands[2])));
     DONE;
   }")

(define_insn "spu_rotqby"
  [(set (match_operand 0 "register_operand"   "=r,r")
        (unspec [(match_operand 1 "register_operand"  "r,r")
                 (match_operand:SI 2 "nonmemory_operand" "r,Is")] UNSPEC_SPU_ROTQBY))]
  ""
  "@
  rotqby\t%0,%1,%2
  rotqbyi\t%0,%1,%2"
  [(set_attr "type" "shuf")])


;; Using spu_rotqby for spu_rotqbyi

(define_insn "spu_rotqbybi"
  [(set (match_operand 0 "register_operand"   "=r")
        (unspec [(match_operand 1 "register_operand"  "r")
                 (match_operand:SI 2 "register_operand" "r")] UNSPEC_SPU_ROTQBYBI))]
  ""
  "rotqbybi\t%0,%1,%2"
  [(set_attr "type" "shuf")])

(define_insn "spu_rotqbi"
  [(set (match_operand 0 "register_operand"   "=r,r")
        (unspec [(match_operand 1 "register_operand"    "r,r")
                 (match_operand:SI 2 "nonmemory_operand" "r,I")] UNSPEC_SPU_ROTQBI))]
  ""
  "@
  rotqbi\t%0,%1,%2
  rotqbii\t%0,%1,%2"
  [(set_attr "type" "shuf")])

;; Using spu_rotqbi for spu_rotqbii

;; rotate and mask, i.e. shift right
(define_insn "spu_rothm"
  [(set (match_operand 0 "register_operand"   "=r,r")
        (unspec [(match_operand 1 "register_operand"   "r,r")
                 (match_operand 2 "vec_regimm_operand" "r,W")] UNSPEC_SPU_ROTHM))]
  ""
  "@
   rothm\t%0,%1,%2
   rothmi\t%0,%1,%R2"
  [(set_attr "type" "fx3")])

(define_expand "spu_rothmi"
  [(set (match_operand:V8HI 0 "register_operand"   "=")
        (lshiftrt:V8HI (match_operand:V8HI 1 "register_operand"  "")
                   (const_vector:V8HI [(match_operand:HI 2 "immediate_operand"  "")])))]
  ""
  "{ spu_emit_insn(gen_spu_rothm(operands[0], operands[1], spu_const_vector(V8HImode, operands[2])));
     DONE;
   }")

(define_insn "spu_rotm"
  [(set (match_operand 0 "register_operand"   "=r,r")
        (unspec [(match_operand 1 "register_operand"   "r,r")
                 (match_operand 2 "vec_regimm_operand" "r,W")] UNSPEC_SPU_ROTM))]
  ""
  "@
   rotm\t%0,%1,%2
   rotmi\t%0,%1,%R2"
  [(set_attr "type" "fx3")])

(define_expand "spu_rotmi"
  [(set (match_operand:V4SI 0 "register_operand"   "=")
        (lshiftrt:V4SI (match_operand:V4SI 1 "register_operand"  "")
                       (const_vector:V4SI [(match_operand:SI 2 "immediate_operand"  "")])))]
  ""
  "{ spu_emit_insn(gen_spu_rotm(operands[0], operands[1], spu_const_vector(V4SImode, operands[2])));
     DONE;
   }")

(define_insn "spu_rotqmby"
  [(set (match_operand 0 "register_operand"   "=r,r")
        (unspec [(match_operand 1 "register_operand"  "r,r")
                 (match_operand:SI 2 "nonmemory_operand" "r,I")] UNSPEC_SPU_ROTQMBY))]
  ""
  "@
  rotqmby\t%0,%1,%2
  rotqmbyi\t%0,%1,%2"
  [(set_attr "type" "shuf")])

(define_insn "spu_rotqmbybi"
  [(set (match_operand 0 "register_operand"   "=r")
        (unspec [(match_operand 1 "register_operand"  "r")
                 (match_operand:SI 2 "register_operand" "r")] UNSPEC_SPU_ROTQMBYBI))]
  ""
  "rotqmbybi\t%0,%1,%2"
  [(set_attr "type" "shuf")])

;; Using spu_rotqmby for spu_rotqmbyi

(define_insn "spu_rotqmbi"
  [(set (match_operand 0 "register_operand"   "=r,r")
        (unspec [(match_operand 1 "register_operand"  "r,r")
                 (match_operand:SI 2 "nonmemory_operand" "r,I")] UNSPEC_SPU_ROTQMBI))]
  ""
  "@
   rotqmbi\t%0,%1,%2
   rotqmbii\t%0,%1,%2"
  [(set_attr "type" "shuf")])

;; Using spu_rotqmbi for spu_rotqmbii

(define_insn "spu_rotmah"
  [(set (match_operand 0 "register_operand"   "=r,r")
        (unspec [(match_operand 1 "register_operand"   "r,r")
                 (match_operand 2 "vec_regimm_operand" "r,W")] UNSPEC_SPU_ROTMAH))]
  ""
  "@
   rotmah\t%0,%1,%2
   rotmahi\t%0,%1,%R2"
  [(set_attr "type" "fx3")])

(define_expand "spu_rotmahi"
  [(set (match_operand:V8HI 0 "register_operand"   "=")
        (ashiftrt:V8HI (match_operand:V8HI 1 "register_operand"  "")
                       (const_vector:V8HI [(match_operand:HI 2 "immediate_operand"  "")])))]
  ""
  "{ spu_emit_insn(gen_spu_rotmah(operands[0], operands[1], spu_const_vector(V8HImode, operands[2])));
     DONE;
   }")

(define_insn "spu_rotma"
  [(set (match_operand 0 "register_operand"   "=r,r")
        (unspec [(match_operand 1 "register_operand"   "r,r")
                 (match_operand 2 "vec_regimm_operand" "r,W")] UNSPEC_SPU_ROTMA))]
  ""
  "@
   rotma\t%0,%1,%2
   rotmai\t%0,%1,%R2"
  [(set_attr "type" "fx3")])

(define_expand "spu_rotmai"
  [(set (match_operand:V4SI 0 "register_operand"   "=r")
        (ashiftrt:V4SI (match_operand:V4SI 1 "register_operand"  "r")
                       (const_vector:V4SI [(match_operand:SI 2 "immediate_operand"  "I")])))]
  ""
  "{ spu_emit_insn(gen_spu_rotma(operands[0], operands[1], spu_const_vector(V4SImode, operands[2])));
     DONE;
   }")

;; compare & halt
(define_insn "spu_heq"
  [(unspec_volatile [(match_operand:SI 0 "register_operand" "r,r")
	             (match_operand:SI 1 "rK_operand" "r,K")] UNSPEC_SPU_HEQ)]
  ""
  "@
  heq\t%0,%1
  heqi\t%0,%1")

;; Using spu_heq for spu_heqi

(define_insn "spu_hgt"
  [(unspec_volatile [(match_operand:SI 0 "register_operand" "r,r")
	             (match_operand:SI 1 "rK_operand" "r,K")] UNSPEC_SPU_HGT)]
  ""
  "@
  hgt\t%0,%1
  hgti\t%0,%1")

;; Using spu_hgt for spu_hgti

(define_insn "spu_hlgt"
  [(unspec_volatile [(match_operand:SI 0 "register_operand" "r,r")
	             (match_operand:SI 1 "rK_operand" "r,K")] UNSPEC_SPU_HLGT)]
  ""
  "@
  hlgt\t%0,%1
  hlgti\t%0,%1")

;; Using spu_hlgt for spu_hlgti

;; compare
(define_insn "spu_ceqb"
  [(set (match_operand:V16QI 0 "register_operand" "=r,r")
        (eq:V16QI (match_operand:V16QI 1 "register_operand"   "r,r")
                  (match_operand:V16QI 2 "vec_regimm_operand" "r,Y")))]
  ""
  "@
   ceqb\t%0,%1,%2
   ceqbi\t%0,%1,%2")

(define_expand "spu_ceqbi"
  [(set (match_operand:V16QI 0 "register_operand" "=")
        (eq:V16QI (match_operand:V16QI 1 "register_operand" "")
                  (const_vector:V16QI [(match_operand:SI 2 "immediate_operand"  "")])))]
  ""
  "{ spu_emit_insn(gen_spu_ceqb(operands[0], operands[1], spu_const_vector(V16QImode, operands[2])));
     DONE;
   }")

(define_insn "spu_ceqh"
  [(set (match_operand:V8HI 0 "register_operand" "=r,r")
        (eq:V8HI (match_operand:V8HI 1 "register_operand"   "r,r")
	         (match_operand:V8HI 2 "vec_regimm_operand" "r,Y")))]
  ""
  "@
   ceqh\t%0,%1,%2
   ceqhi\t%0,%1,%2")

(define_expand "spu_ceqhi"
  [(set (match_operand:V8HI 0 "register_operand" "=")
        (eq:V8HI (match_operand:V8HI 1 "register_operand" "")
                 (const_vector:V8HI [(match_operand:HI 2 "immediate_operand"  "")])))]
  ""
  "{ spu_emit_insn(gen_spu_ceqh(operands[0], operands[1], spu_const_vector(V8HImode, operands[2])));
     DONE;
   }")

(define_insn "spu_ceq"
  [(set (match_operand:V4SI 0 "register_operand" "=r,r")
        (eq:V4SI (match_operand:V4SI 1 "register_operand"   "r,r")
	         (match_operand:V4SI 2 "vec_regimm_operand" "r,Y")))]
  ""
  "@
   ceq\t%0,%1,%2
   ceqi\t%0,%1,%2")

(define_expand "spu_ceqi"
  [(set (match_operand:V4SI 0 "register_operand" "=")
        (eq:V4SI (match_operand:V4SI 1 "register_operand" "")
                 (const_vector:V4SI [(match_operand:SI 2 "immediate_operand"  "")])))]
  ""
  "{ spu_emit_insn(gen_spu_ceq(operands[0], operands[1], spu_const_vector(V4SImode, operands[2])));
     DONE;
   }")

(define_insn "spu_cgtb"
  [(set (match_operand:V16QI 0 "register_operand" "=r,r")
        (gt:V16QI (match_operand:V16QI 1 "register_operand"   "r,r")
                  (match_operand:V16QI 2 "vec_regimm_operand" "r,Y")))]
  ""
  "@
   cgtb\t%0,%1,%2
   cgtbi\t%0,%1,%2")

(define_expand "spu_cgtbi"
  [(set (match_operand:V16QI 0 "register_operand" "=")
        (gt:V16QI (match_operand:V16QI 1 "register_operand" "")
                  (const_vector:V16QI [(match_operand:SI 2 "immediate_operand"  "")])))]
  ""
  "{ spu_emit_insn(gen_spu_cgtb(operands[0], operands[1], spu_const_vector(V16QImode, operands[2])));
     DONE;
   }")

(define_insn "spu_cgth"
  [(set (match_operand:V8HI 0 "register_operand" "=r,r")
        (gt:V8HI (match_operand:V8HI 1 "register_operand"   "r,r")
	         (match_operand:V8HI 2 "vec_regimm_operand" "r,Y")))]
  ""
  "@
   cgth\t%0,%1,%2
   cgthi\t%0,%1,%2")

(define_expand "spu_cgthi"
  [(set (match_operand:V8HI 0 "register_operand" "=")
        (gt:V8HI (match_operand:V8HI 1 "register_operand" "")
                 (const_vector:V8HI [(match_operand:HI 2 "immediate_operand"  "")])))]
  ""
  "{ spu_emit_insn(gen_spu_cgth(operands[0], operands[1], spu_const_vector(V8HImode, operands[2])));
     DONE;
   }")

(define_insn "spu_cgt"
  [(set (match_operand:V4SI 0 "register_operand" "=r,r")
        (gt:V4SI (match_operand:V4SI 1 "register_operand"   "r,r")
	         (match_operand:V4SI 2 "vec_regimm_operand" "r,Y")))]
  ""
  "@
   cgt\t%0,%1,%2
   cgti\t%0,%1,%2")

(define_expand "spu_cgti"
  [(set (match_operand:V4SI 0 "register_operand" "=")
        (gt:V4SI (match_operand:V4SI 1 "register_operand" "")
                 (const_vector:V4SI [(match_operand:SI 2 "immediate_operand"  "")])))]
  ""
  "{ spu_emit_insn(gen_spu_cgt(operands[0], operands[1], spu_const_vector(V4SImode, operands[2])));
     DONE;
   }")

(define_insn "spu_clgtb"
  [(set (match_operand:V16QI 0 "register_operand" "=r,r")
        (gtu:V16QI (match_operand:V16QI 1 "register_operand"   "r,r")
                   (match_operand:V16QI 2 "vec_regimm_operand" "r,Y")))]
  ""
  "@
   clgtb\t%0,%1,%2
   clgtbi\t%0,%1,%2")

(define_expand "spu_clgtbi"
  [(set (match_operand:V16QI 0 "register_operand" "=")
        (gtu:V16QI (match_operand:V16QI 1 "register_operand" "")
                   (const_vector:V16QI [(match_operand:SI 2 "immediate_operand"  "")])))]
  ""
  "{ spu_emit_insn(gen_spu_clgtb(operands[0], operands[1], spu_const_vector(V16QImode, operands[2])));
     DONE;
   }")

(define_insn "spu_clgth"
  [(set (match_operand:V8HI 0 "register_operand" "=r,r")
        (gtu:V8HI (match_operand:V8HI 1 "register_operand"   "r,r")
	          (match_operand:V8HI 2 "vec_regimm_operand" "r,Y")))]
  ""
  "@
   clgth\t%0,%1,%2
   clgthi\t%0,%1,%2")

(define_expand "spu_clgthi"
  [(set (match_operand:V8HI 0 "register_operand" "=")
        (gtu:V8HI (match_operand:V8HI 1 "register_operand" "")
                  (const_vector:V8HI [(match_operand:HI 2 "immediate_operand"  "")])))]
  ""
  "{ spu_emit_insn(gen_spu_clgth(operands[0], operands[1], spu_const_vector(V8HImode, operands[2])));
     DONE;
   }")

(define_insn "spu_clgt"
  [(set (match_operand:V4SI 0 "register_operand" "=r,r")
        (gtu:V4SI (match_operand:V4SI 1 "register_operand"   "r,r")
	          (match_operand:V4SI 2 "vec_regimm_operand" "r,Y")))]
  ""
  "@
   clgt\t%0,%1,%2
   clgti\t%0,%1,%2")

(define_expand "spu_clgti"
  [(set (match_operand:V4SI 0 "register_operand" "=")
        (gtu:V4SI (match_operand:V4SI 1 "register_operand" "")
                  (const_vector:V4SI [(match_operand:SI 2 "immediate_operand"  "")])))]
  ""
  "{ spu_emit_insn(gen_spu_clgt(operands[0], operands[1], spu_const_vector(V4SImode, operands[2])));
     DONE;
   }")

;; branches

(define_insn "spu_br"
  [(set (pc) (match_operand 0 "address_operand" "s,r"))
   (use (match_operand:SI 1 "address_operand" "s,s"))]
  ""
  "@
   bra\t%0
   bi\t%0"
  [(set_attr "type" "br")])

(define_expand "spu_bra"
  [(set (pc) (match_operand 0 "address_operand" ""))
   (use (match_operand:SI 1 "address_operand" ""))]
  ""
  "")

(define_insn "spu_brsl"
  [(parallel
    [(set (pc)
	  (match_operand 1 "address_operand" "s,r"))
     (set (match_operand 0 "register_operand" "=r,r")
          (plus (pc) (const_int 4)))
     (use (match_operand:SI 2 "address_operand" "s,s"))])]
  ""
  "@
  brsl\t%0,%1
  bisl\t%0,%1"
  [(set_attr "type" "br")])

(define_expand "spu_brasl"
  [(parallel
    [(set (pc)
	  (match_operand 1 "address_operand" ""))
     (set (match_operand 0 "register_operand" "")
          (plus (pc) (const_int 4)))
     (use (match_operand:SI 2 "address_operand" ""))])]
  ""
  "")

(define_expand "spu_bi"
  [(set (pc) (match_operand:SI 0 "register_operand" ""))
   (use (match_operand:SI 1 "address_operand" ""))]
  ""
  "")

;; The description below hides the fact that bisled conditionally
;; executes the call depending on the value in channel 0.  This was 
;; done so that the description would conform to the format of a call 
;; insn.  Otherwise (if this were not part of call insn), the link 
;; register, $lr, would not be saved/restored in the prologue/epilogue.

(define_insn "spu_bisled"
  [(parallel
    [(call (mem:QI (match_operand:SI 0 "register_operand" "r"))
            (const_int 0))
     (clobber (reg:SI 0))
     (clobber (reg:SI 130))
     (use (match_operand:SI 1 "address_operand" ""))
     (use (const_int 0))])]
  ""
  "bisled\t$lr,%0"
  [(set_attr "type" "br")])

(define_insn "spu_bisledd"
  [(parallel
    [(call (mem:QI (match_operand:SI 0 "register_operand" "r"))
            (const_int 0))
     (clobber (reg:SI 0))
     (clobber (reg:SI 130))
     (use (match_operand:SI 1 "address_operand" ""))
     (use (const_int 1))])]
  ""
  "bisledd\t$lr,%0"
  [(set_attr "type" "br")])

(define_insn "spu_bislede"
  [(parallel
    [(call (mem:QI (match_operand:SI 0 "register_operand" "r"))
            (const_int 0))
     (clobber (reg:SI 0))
     (clobber (reg:SI 130))
     (use (match_operand:SI 1 "address_operand" ""))
     (use (const_int 2))])]
  ""
  "bislede\t$lr,%0"
  [(set_attr "type" "br")])

(define_expand "spu_bisl"
    [(parallel
     [(call (mem:QI (match_operand:SI 0 "register_operand" "r"))
            (const_int 0))
      (clobber (reg:SI 0))
      (clobber (reg:SI 130))])
     (use (match_operand:SI 1 "address_operand" "s"))]
  ""
  "")

(define_insn "spu_brnz"
  [(set (pc)
	(if_then_else (ne (match_operand:V4SI 0 "register_operand" "r,r")
			  (const_int 0))
		      (match_operand:SI 1 "address_operand" "r,s")
		      (pc)))
   (use (match_operand:SI 2 "address_operand" "s,s"))]
  ""
  "@
   binz\t%0,%1
   brnz\t%0,%1"
  [(set_attr "type" "br")])

(define_insn "spu_brz"
  [(set (pc)
	(if_then_else (eq (match_operand:V4SI 0 "register_operand" "r,r")
			  (const_int 0))
		      (match_operand:SI 1 "address_operand" "r,s")
		      (pc)))
   (use (match_operand:SI 2 "address_operand" "s,s"))]
  ""
  "@
   biz\t%0,%1
   brz\t%0,%1"
  [(set_attr "type" "br")])

(define_insn "spu_brhnz"
  [(set (pc)
	(if_then_else (ne (match_operand:V8HI 0 "register_operand" "r,r")
			  (const_int 0))
		      (match_operand:SI 1 "address_operand" "r,s")
		      (pc)))
   (use (match_operand:SI 2 "address_operand" "s,s"))]
  ""
  "@
   bihnz\t%0,%1
   brhnz\t%0,%1"
  [(set_attr "type" "br")])

(define_insn "spu_brhz"
  [(set (pc)
	(if_then_else (eq (match_operand:V8HI 0 "register_operand" "r,r")
			  (const_int 0))
		      (match_operand:SI 1 "address_operand" "r,s")
		      (pc)))
   (use (match_operand:SI 2 "address_operand" "s,s"))]
  ""
  "@
   bihz\t%0,%1
   brhz\t%0,%1"
  [(set_attr "type" "br")])


;; The second unspec is to prevent GCC from deleting this insn.  Didn't
;; use unspec_volatile because it effects scheduling.  Didn't add a use
;; of $hbr to all the jumps because it could create false dependencies,
;; again effecting scheduling.
(define_insn "spu_hbr"
  [(set (reg:SI 130)
        (unspec [(match_operand:SI 0 "address_operand" "sK,sK")
	         (match_operand:SI 1 "address_operand" "r,si")] UNSPEC_SPU_HBR))
   (unspec [(const_int 0)] UNSPEC_SPU_HBR)]
  ""
  "@
   hbr\t%0,%1
   hbra\t%0,%1"
  [(set_attr "type" "hbr")])

;; Using spu_hbr for spu_hbra

(define_insn "spu_hbrr"
  [(set (reg:SI 130)
        (unspec [(match_operand:SI 0 "address_operand" "sK")
	         (match_operand:SI 1 "address_operand" "s")] UNSPEC_SPU_HBRR))
   (unspec [(const_int 0)] UNSPEC_SPU_HBRR)]
  ""
  "hbrr\t%0,%1"
  [(set_attr "type" "hbr")])

;; floating point
(define_insn "spu_fa"
  [(set (match_operand:V4SF 0 "register_operand" "=r")
        (plus:V4SF (match_operand:V4SF 1 "register_operand" "r")
                   (match_operand:V4SF 2 "register_operand" "r")))]
  ""
  "fa\t%0,%1,%2"
  [(set_attr "type" "fp6")])

(define_insn "spu_dfa"
  [(set (match_operand:V2DF 0 "register_operand" "=r")
        (plus:V2DF (match_operand:V2DF 1 "register_operand" "r")
                   (match_operand:V2DF 2 "register_operand" "r")))]
  ""
  "dfa\t%0,%1,%2"
  [(set_attr "type" "fpd")])

(define_insn "spu_fs"
  [(set (match_operand:V4SF 0 "register_operand" "=r")
        (minus:V4SF (match_operand:V4SF 1 "register_operand" "r")
                    (match_operand:V4SF 2 "register_operand" "r")))]
  ""
  "fs\t%0,%1,%2"
  [(set_attr "type" "fp6")])

(define_insn "spu_dfs"
  [(set (match_operand:V2DF 0 "register_operand" "=r")
        (minus:V2DF (match_operand:V2DF 1 "register_operand" "r")
                    (match_operand:V2DF 2 "register_operand" "r")))]
  ""
  "dfs\t%0,%1,%2"
  [(set_attr "type" "fpd")])

(define_insn "spu_fm"
  [(set (match_operand:V4SF 0 "register_operand" "=r")
        (mult:V4SF (match_operand:V4SF 1 "register_operand" "r")
                   (match_operand:V4SF 2 "register_operand" "r")))]
  ""
  "fm\t%0,%1,%2"
  [(set_attr "type" "fp6")])

(define_insn "spu_dfm"
  [(set (match_operand:V2DF 0 "register_operand" "=r")
        (mult:V2DF (match_operand:V2DF 1 "register_operand" "r")
                   (match_operand:V2DF 2 "register_operand" "r")))]
  ""
  "dfm\t%0,%1,%2"
  [(set_attr "type" "fpd")])

(define_insn "spu_fma"
  [(set (match_operand:V4SF 0 "register_operand" "=r")
        (plus:V4SF (mult:V4SF (match_operand:V4SF 1 "register_operand" "r")
                              (match_operand:V4SF 2 "register_operand" "r"))
                   (match_operand:V4SF 3 "register_operand" "r")))]
  ""
  "fma\t%0,%1,%2,%3"
  [(set_attr "type" "fp6")])

(define_insn "spu_dfma"
  [(set (match_operand:V2DF 0 "register_operand" "=r")
        (plus:V2DF (mult:V2DF (match_operand:V2DF 1 "register_operand" "r")
                              (match_operand:V2DF 2 "register_operand" "r"))
                   (match_operand:V2DF 3 "register_operand" "0")))]
  ""
  "dfma\t%0,%1,%2"
  [(set_attr "type" "fpd")])

(define_insn "spu_dfnma"
  [(set (match_operand:V2DF 0 "register_operand" "=r")
	(neg:V2DF (plus:V2DF (mult:V2DF (match_operand:V2DF 1 "register_operand" "r")
			                 (match_operand:V2DF 2 "register_operand" "r"))
		             (match_operand:V2DF 3 "register_operand" "0"))))]
  ""
  "dfnma\t%0,%1,%2"
  [(set_attr "type"	"fpd")])

(define_insn "spu_fnms"
  [(set (match_operand:V4SF 0 "register_operand" "=r")
        (minus:V4SF (match_operand:V4SF 3 "register_operand" "r")
                    (mult:V4SF (match_operand:V4SF 1 "register_operand" "r")
                               (match_operand:V4SF 2 "register_operand" "r"))))]
  ""
  "fnms\t%0,%1,%2,%3"
  [(set_attr "type" "fp6")])

(define_insn "spu_dfnms"
  [(set (match_operand:V2DF 0 "register_operand" "=r")
        (minus:V2DF (match_operand:V2DF 3 "register_operand" "0")
                    (mult:V2DF (match_operand:V2DF 1 "register_operand" "r")
                               (match_operand:V2DF 2 "register_operand" "r"))))]
  ""
  "dfnms\t%0,%1,%2"
  [(set_attr "type" "fpd")])

(define_insn "spu_fms"
  [(set (match_operand:V4SF 0 "register_operand" "=r")
        (minus:V4SF (mult:V4SF (match_operand:V4SF 1 "register_operand" "r")
                               (match_operand:V4SF 2 "register_operand" "r"))
                    (match_operand:V4SF 3 "register_operand" "r")))]
  ""
  "fms\t%0,%1,%2,%3"
  [(set_attr "type" "fp6")])

(define_insn "spu_dfms"
  [(set (match_operand:V2DF 0 "register_operand" "=r")
        (minus:V2DF (mult:V2DF (match_operand:V2DF 1 "register_operand" "r")
                               (match_operand:V2DF 2 "register_operand" "r"))
                    (match_operand:V2DF 3 "register_operand" "0")))]
  ""
  "dfms\t%0,%1,%2"
  [(set_attr "type" "fpd")])

;; float div/sqrt estimate and interpolate
(define_insn "spu_frest"
  [(set (match_operand 0 "register_operand" "=r")
        (unspec [(match_operand 1 "register_operand" "r")] UNSPEC_SPU_FREST))]
  ""
  "frest\t%0,%1"
  [(set_attr "type" "shuf")])

(define_insn "spu_frsqest"
  [(set (match_operand 0 "register_operand" "=r")
        (unspec [(match_operand 1 "register_operand" "r")] UNSPEC_SPU_FRSQEST))]
  ""
  "frsqest\t%0,%1"
  [(set_attr "type" "shuf")])

(define_insn "spu_fi"
  [(set (match_operand 0 "register_operand" "=r")
        (unspec [(match_operand 1 "register_operand" "r")
                 (match_operand:V4SF 2 "register_operand" "r")] UNSPEC_SPU_FI))]
  ""
  "fi\t%0,%1,%2"
  [(set_attr "type" "fp7")])

;; float convert
(define_insn "spu_csflt"
  [(set (match_operand 0 "register_operand"  "=r")
	(unspec [(match_operand 1 "register_operand"   "r")
                 (match_operand:SI   2 "immediate_operand"  "K")] UNSPEC_SPU_CSFLT ))]
  ""
  "csflt\t%0,%1,%2"
  [(set_attr "type" "fp7")])

(define_insn "spu_cflts"
  [(set (match_operand 0 "register_operand"  "=r")
	(unspec [(match_operand 1 "register_operand"   "r")
                 (match_operand:SI   2 "immediate_operand"  "J")] UNSPEC_SPU_CFLTS ))]
  ""
  "cflts\t%0,%1,%2"
  [(set_attr "type" "fp7")])

(define_insn "spu_cuflt"
  [(set (match_operand 0 "register_operand"  "=r")
	(unspec [(match_operand 1 "register_operand"   "r")
                 (match_operand:SI   2 "immediate_operand"  "K")] UNSPEC_SPU_CUFLT ))]
  ""
  "cuflt\t%0,%1,%2"
  [(set_attr "type" "fp7")])

(define_insn "spu_cfltu"
  [(set (match_operand 0 "register_operand"  "=r")
	(unspec [(match_operand 1 "register_operand"   "r")
                 (match_operand:SI   2 "immediate_operand"  "J")] UNSPEC_SPU_CFLTU ))]
  ""
  "cfltu\t%0,%1,%2"
  [(set_attr "type" "fp7")])

(define_expand "spu_frds"
   [(set (match_operand:V4SF 0 "register_operand"   "=r")
	 (float_truncate:V4SF (match_operand:V2DF 1 "register_operand" "r")))]
  ""
  "{ spu_emit_insn(gen_spu_frds_internal(operands[0], operands[1], spu_const_vector(V2SFmode, CONST0_RTX(SFmode))));
     DONE;
   }")

(define_insn "spu_frds_internal"
   [(set (match_operand:V4SF 0 "register_operand"   "=r")
        (vec_select:V4SF
	  (vec_concat:V4SF
	    (float_truncate:V2SF (match_operand:V2DF 1 "register_operand" "r"))
	    (match_operand:V2SF 2 "immediate_operand" "Y"))
	  (parallel [(const_int 0)(const_int 2)(const_int 1)(const_int 3)])))]
  ""
  "frds\t%0,%1"
  [(set_attr "type" "fpd")])

(define_insn "spu_fesd"
  [(set (match_operand:V2DF 0 "register_operand"   "=r")
        (float_extend:V2DF
	  (vec_select:V2SF
	    (match_operand:V4SF 1 "register_operand" "r")
	      (parallel [(const_int 0)(const_int 2)]))))]
  ""
  "fesd\t%0,%1"
  [(set_attr "type" "fpd")])

;; float compare
(define_insn "spu_fceq"
  [(set (match_operand:V4SI 0 "register_operand" "=r")
        (eq:V4SI (match_operand:V4SF 1 "register_operand" "r")
	         (match_operand:V4SF 2 "register_operand" "r")))]
  ""
  "fceq\t%0,%1,%2")

(define_insn "spu_fcmeq"
  [(set (match_operand:V4SI 0 "register_operand" "=r")
        (eq:V4SI (abs:V4SF (match_operand:V4SF 1 "register_operand" "r"))
	         (abs:V4SF (match_operand:V4SF 2 "register_operand" "r"))))]
  ""
  "fcmeq\t%0,%1,%2")

(define_insn "spu_fcgt"
  [(set (match_operand:V4SI 0 "register_operand" "=r")
        (gt:V4SI (match_operand:V4SF 1 "register_operand" "r")
	         (match_operand:V4SF 2 "register_operand" "r")))]
  ""
  "fcgt\t%0,%1,%2")

(define_insn "spu_fcmgt"
  [(set (match_operand:V4SI 0 "register_operand" "=r")
        (gt:V4SI (abs:V4SF (match_operand:V4SF 1 "register_operand" "r"))
	         (abs:V4SF (match_operand:V4SF 2 "register_operand" "r"))))]
  ""
  "fcmgt\t%0,%1,%2")

;; control
(define_insn "spu_stop"
  [(unspec_volatile [(match_operand:SI 0 "immediate_operand" "M")] UNSPEC_SPU_STOP)]
  ""
  "stop\t%0"
  [(set_attr "type" "br")])

(define_insn "spu_stopd"
  [(unspec_volatile [(match_operand:SI 0 "register_operand" "r")
		     (match_operand:SI 1 "register_operand" "r")
		     (match_operand:SI 2 "register_operand" "r")] UNSPEC_SPU_STOPD)]
  ""
  "stopd\t%0,%1,%2"
  [(set_attr "type" "br")])


(define_insn "spu_lnop"
  [(unspec_volatile [(const_int 2)] UNSPEC_SPU_LNOP)]
  ""
  "lnop"
  [(set_attr "type" "lnop")])

(define_insn "spu_nop"
  [(unspec_volatile [(const_int 0)] UNSPEC_SPU_NOP)]
  ""
  "nop"
  [(set_attr "type" "nop")])

(define_insn "spu_nopn"
  [(unspec_volatile [(match_operand:SI 0 "immediate_operand" "J")] UNSPEC_SPU_NOP)]
  ""
  "nop\t$%0"
  [(set_attr "type" "nop")])

(define_insn "spu_sync"
  [(unspec_volatile [(const_int 3)] UNSPEC_SPU_SYNC)]
  ""
  "sync"
  [(set_attr "type" "br")])

(define_insn "spu_syncc"
  [(unspec_volatile [(const_int 4)] UNSPEC_SPU_SYNCC)]
  ""
  "syncc"
  [(set_attr "type" "br")])

(define_insn "spu_dsync"
  [(unspec_volatile [(const_int 5)] UNSPEC_SPU_DSYNC)]
  ""
  "dsync"
  [(set_attr "type" "br")])

;; interrupt disable/enable
(define_expand "spu_idisable"
  [(unspec_volatile [(const_int 6)] UNSPEC_SPU_IDISABLE)]
  ""
  "{ 
      if (flag_pic)
        emit_insn(gen_spu_idisable_internal(gen_reg_rtx(SImode)));
      else
        emit_insn(gen_spu_idisable_internal_pic(gen_reg_rtx(SImode)));
       DONE;
   } ")

(define_insn "spu_idisable_internal"
  [(parallel [(unspec_volatile [(const_int 6)] UNSPEC_SPU_IDISABLE)
   (clobber (match_operand:SI 0 "register_operand" "=&r"))])]
  "! flag_pic"
  "ila\t%0,.+8\;bid\t%0"
  [(set_attr "length" "8")
   (set_attr "type" "multi0")])

(define_insn "*spu_idisable_internal_cond_eq"
  [(cond_exec (eq:SI (match_operand:SI 1 "register_operand" "r") (const_int 0))
              (parallel [(unspec_volatile [(const_int 6)] UNSPEC_SPU_IDISABLE)
                         (clobber (match_operand:SI 0 "register_operand" "=&r"))]))]
  "! flag_pic"
  "ila\t%0,.+8\;bizd\t%1,%0"
  [(set_attr "length" "8")
   (set_attr "type" "multi0")])

(define_insn "*spu_idisable_internal_cond_ne"
  [(cond_exec (ne:SI (match_operand:SI 1 "register_operand" "r") (const_int 0))
              (parallel [(unspec_volatile [(const_int 6)] UNSPEC_SPU_IDISABLE)
                         (clobber (match_operand:SI 0 "register_operand" "=&r"))]))]
  "! flag_pic"
  "ila\t%0,.+8\;binzd\t%1,%0"
  [(set_attr "length" "8")
   (set_attr "type" "multi0")])

(define_insn "*spu_idisable_internal_cond_eq_h"
  [(cond_exec (eq:HI (match_operand:HI 1 "register_operand" "r") (const_int 0))
              (parallel [(unspec_volatile [(const_int 6)] UNSPEC_SPU_IDISABLE)
                         (clobber (match_operand:SI 0 "register_operand" "=&r"))]))]
  "! flag_pic"
  "ila\t%0,.+8\;bihzd\t%1,%0"
  [(set_attr "length" "8")
   (set_attr "type" "multi0")])

(define_insn "*spu_idisable_internal_cond_ne_h"
  [(cond_exec (ne:HI (match_operand:HI 1 "register_operand" "r") (const_int 0))
              (parallel [(unspec_volatile [(const_int 6)] UNSPEC_SPU_IDISABLE)
                         (clobber (match_operand:SI 0 "register_operand" "=&r"))]))]
  "! flag_pic"
  "ila\t%0,.+8\;bihnzd\t%1,%0"
  [(set_attr "length" "8")
   (set_attr "type" "multi0")])

(define_insn "spu_idisable_internal_pic"
  [(unspec_volatile [(const_int 6)] UNSPEC_SPU_IDISABLE)
   (clobber (match_operand:SI 0 "register_operand" "=&r"))]
  "flag_pic"
  "brsl\t%0,.+4\;ai\t%0,%0,8\;bid\t%0"
  [(set_attr "length" "12")
   (set_attr "type" "multi0")])

(define_expand "spu_ienable"
  [(unspec_volatile [(const_int 7)] UNSPEC_SPU_IENABLE)]
  ""
  "{ 
      if (flag_pic)
        emit_insn(gen_spu_ienable_internal(gen_reg_rtx(SImode)));
      else
        emit_insn(gen_spu_ienable_internal_pic(gen_reg_rtx (SImode)));
      DONE;
   } ")

(define_insn "spu_ienable_internal"
  [(parallel [(unspec_volatile [(const_int 7)] UNSPEC_SPU_IENABLE)
   (clobber (match_operand:SI 0 "register_operand" "=&r"))])]
  "! flag_pic"
  "ila\t%0,.+8\;bie\t%0"
  [(set_attr "length" "8")
   (set_attr "type" "multi0")])

(define_insn "*spu_ienable_internal_cond_eq"
  [(cond_exec (eq:SI (match_operand:SI 1 "register_operand" "r") (const_int 0))
              (parallel [(unspec_volatile [(const_int 7)] UNSPEC_SPU_IENABLE)
                         (clobber (match_operand:SI 0 "register_operand" "=&r"))]))]
  "! flag_pic"
  "ila\t%0,.+8\;bize\t%1,%0"
  [(set_attr "length" "8")
   (set_attr "type" "multi0")])

(define_insn "*spu_ienable_internal_cond_ne"
  [(cond_exec (ne:SI (match_operand:SI 1 "register_operand" "r") (const_int 0))
              (parallel [(unspec_volatile [(const_int 7)] UNSPEC_SPU_IENABLE)
                         (clobber (match_operand:SI 0 "register_operand" "=&r"))]))]
  "! flag_pic"
  "ila\t%0,.+8\;binze\t%1,%0"
  [(set_attr "length" "8")
   (set_attr "type" "multi0")])

(define_insn "*spu_ienable_internal_cond_eq_h"
  [(cond_exec (eq:HI (match_operand:HI 1 "register_operand" "r") (const_int 0))
              (parallel [(unspec_volatile [(const_int 7)] UNSPEC_SPU_IENABLE)
                         (clobber (match_operand:SI 0 "register_operand" "=&r"))]))]
  "! flag_pic"
  "ila\t%0,.+8\;bihze\t%1,%0"
  [(set_attr "length" "8")
   (set_attr "type" "multi0")])

(define_insn "*spu_ienable_internal_cond_ne_h"
  [(cond_exec (ne:HI (match_operand:HI 1 "register_operand" "r") (const_int 0))
              (parallel [(unspec_volatile [(const_int 7)] UNSPEC_SPU_IENABLE)
                         (clobber (match_operand:SI 0 "register_operand" "=&r"))]))]
  "! flag_pic"
  "ila\t%0,.+8\;bihnze\t%1,%0"
  [(set_attr "length" "8")
   (set_attr "type" "multi0")])

(define_insn "spu_ienable_internal_pic"
  [(unspec_volatile [(const_int 7)] UNSPEC_SPU_IENABLE)
   (clobber (match_operand:SI 0 "register_operand" "=&r"))]
  "flag_pic"
  "brsl\t%0,.+4\;ai\t%0,%0,8\;bie\t%0"
  [(set_attr "length" "12")
   (set_attr "type" "multi0")])


;; special purpose registers
(define_insn "spu_fscrrd"
  [(set (match_operand:V4SI 0 "register_operand" "=r")
        (unspec_volatile:V4SI [(const_int 6)] UNSPEC_SPU_FSCRRD))]
  ""
  "fscrrd\t%0"
  [(set_attr "type" "spr")])

(define_insn "spu_fscrwr"
  [(unspec_volatile [(match_operand:V4SI 0 "register_operand" "r")] UNSPEC_SPU_FSCRWR)]
  ""
  "fscrwr\t$0,%0"
  [(set_attr "type" "spr")])

(define_insn "spu_mfspr"
  [(set (match_operand:SI 0 "register_operand" "=r")
        (unspec_volatile:SI [(match_operand:SI 1 "immediate_operand" "J")] UNSPEC_SPU_MFSPR))]
  ""
  "mfspr\t%0,$sp%1"
  [(set_attr "type" "spr")])

(define_insn "spu_mtspr"
  [(unspec_volatile [(match_operand:SI 0 "immediate_operand" "J")
	             (match_operand:SI 1 "register_operand"  "r")] UNSPEC_SPU_MTSPR)]
  ""
  "mtspr\t$sp%0,%1"
  [(set_attr "type" "spr")])

;; channels
(define_expand "spu_rdch"
  [(set (match_operand:V4SI 0 "register_operand" "=r")
        (unspec_volatile:V4SI [(match_operand:SI 1 "immediate_operand" "J")] UNSPEC_SPU_RDCH))]
  ""
  "{
    if (spu_safe_dma (INTVAL (operands[1])))
      {
        spu_emit_insn (gen_spu_rdch_clobber (operands[0], operands[1]));
        DONE;
      }
   }")

(define_expand "spu_rchcnt"
  [(set (match_operand:SI 0 "register_operand" "=r")
        (unspec_volatile:SI [(match_operand:SI 1 "immediate_operand" "J")] UNSPEC_SPU_RCHCNT))]
  ""
  "{
    if (spu_safe_dma (INTVAL (operands[1])))
      {
        spu_emit_insn (gen_spu_rchcnt_clobber (operands[0], operands[1]));
        DONE;
      }
   }")

(define_expand "spu_wrch"
   [(unspec_volatile [(match_operand:SI 0 "immediate_operand"  "J")
 	              (match_operand:V4SI 1 "register_operand" "r")] UNSPEC_SPU_WRCH)]
   ""
  "{
    if (spu_safe_dma (INTVAL (operands[0])))
      {
        spu_emit_insn (gen_spu_wrch_clobber (operands[0], operands[1]));
        DONE;
      }
   }")

(define_insn "spu_rdch_noclobber"
  [(set (match_operand:V4SI 0 "register_operand" "=r")
        (unspec_volatile:V4SI [(match_operand:SI 1 "immediate_operand" "J")] UNSPEC_SPU_RDCH))]
  ""
  "rdch\t%0,$ch%1"
  [(set_attr "type" "spr")])

(define_insn "spu_rchcnt_noclobber"
  [(set (match_operand:SI 0 "register_operand" "=r")
        (unspec_volatile:SI [(match_operand:SI 1 "immediate_operand" "J")] UNSPEC_SPU_RCHCNT))]
  ""
  "rchcnt\t%0,$ch%1"
  [(set_attr "type" "spr")])

(define_insn "spu_wrch_noclobber"
   [(unspec_volatile [(match_operand:SI 0 "immediate_operand"  "J")
 	              (match_operand:V4SI 1 "register_operand" "r")] UNSPEC_SPU_WRCH)]
   ""
   "wrch\t$ch%0,%1"
   [(set_attr "type" "spr")])

(define_insn "spu_rdch_clobber"
  [(set (match_operand:V4SI 0 "register_operand" "=r")
        (unspec_volatile:V4SI [(match_operand:SI 1 "immediate_operand" "J")] UNSPEC_SPU_RDCH))
    (clobber (mem:BLK (scratch)))]
  ""
  "rdch\t%0,$ch%1"
  [(set_attr "type" "spr")])

(define_insn "spu_rchcnt_clobber"
  [(set (match_operand:SI 0 "register_operand" "=r")
        (unspec_volatile:SI [(match_operand:SI 1 "immediate_operand" "J")] UNSPEC_SPU_RCHCNT))
    (clobber (mem:BLK (scratch)))]
  ""
  "rchcnt\t%0,$ch%1"
  [(set_attr "type" "spr")])

(define_insn "spu_wrch_clobber"
   [(unspec_volatile [(match_operand:SI 0 "immediate_operand"  "J")
 	              (match_operand:V4SI 1 "register_operand" "r")] UNSPEC_SPU_WRCH)
    (clobber (mem:BLK (scratch)))]
   ""
   "wrch\t$ch%0,%1"
   [(set_attr "type" "spr")])

(define_expand "spu_splats" 
  [(set (match_operand 0 "register_operand"  "")
        (vec_duplicate (match_operand 1 "nonmemory_operand"  "")))]
  ""
  "{ spu_builtin_splats(operands);
     DONE; }")

(define_expand "spu_extract"
 [(set (match_operand 0 "register_operand"   "=r")
       (vec_select (match_operand 1 "register_operand"   "r")
                   (parallel [(match_operand:SI 2 "nonmemory_operand" "rJ")])))]
  ""
  "{ rtx ops[4];
     ops[0] = operands[0];
     ops[1] = operands[1];
     ops[2] = operands[2];
     ops[3] = gen_reg_rtx(SImode); 
     spu_builtin_extract(ops);
     DONE;
    }")

(define_expand "spu_insert"
  [(set (match_operand 0 "register_operand"   "=r")
        (unspec [(match_operand 1 "register_operand"   "r")
                 (match_operand 2 "register_operand"   "r")
                 (match_operand:SI 3 "nonmemory_operand" "J")] 0))] 
  ""
  "{ spu_builtin_insert(operands); DONE;
   }")

(define_expand "spu_promote"
  [(set (match_operand 0 "register_operand"   "=r")
        (unspec [(match_operand 1 "register_operand"   "r")
                 (match_operand:SI 2 "immediate_operand" "J")] 0))] 
  ""
  "{ spu_builtin_promote(operands); DONE;
   }")

;; Function prologue and epilogue.

(define_expand "prologue"
  [(const_int 1)]
  ""
  "{
     spu_expand_prologue ();
     DONE;
   }"
)

;; "blockage" is only emited in epilogue.  This is what it took to
;; make "basic block reordering" work with the insns sequence
;; generated by the spu_expand_epilogue (taken from mips.md)

(define_insn "blockage"
  [(unspec_volatile [(const_int 0)] UNSPEC_BLOCKAGE)]
  ""
  ""
  [(set_attr "length" "0")])

(define_expand "epilogue"
  [(const_int 2)]
  ""
  "{
     spu_expand_epilogue (false);
     DONE;
  }"
)

(define_expand "sibcall_epilogue"
  [(const_int 2)]
  ""
  "{
     spu_expand_epilogue (true);
     DONE;
  }"
)

;; FIXME: For the present time, just ignore this guy.

(define_expand "spu_align_hint"
  [(unspec [(match_operand:SI 0 "address_operand" "o")
            (match_operand:SI 1 "immediate_operand" "")
            (match_operand:SI 2 "immediate_operand" "")] UNSPEC_SPU_ALIGN_HINT)]
  ""
  "{
     DONE;
  }"
)

;; We add a use of $0 so the insn isn't scheduled across calls.
(define_expand "branch_hint"
 [(parallel
  [(set (reg:SI 130)
        (unspec [(match_operand:SI 0 "address_operand" "s")
	         (match_operand:SI 1 "register_operand" "r")] UNSPEC_SPU_HBR))
   (unspec [(const_int 0)] UNSPEC_SPU_HBR)])]
  ""
  "")

(define_insn "iprefetch"
  [(unspec [(const_int 1)] UNSPEC_IPREFETCH)]
  ""
  "hbrp"
  [(set_attr "type" "iprefetch")
   (set_attr "length" "4")])

(define_insn "spu_convert"
  [(set (match_operand 0 "register_operand" "=r")
        (unspec [(match_operand 1 "register_operand" "0")] UNSPEC_SPU_CONVERT))]
  ""
  ""
  [(set_attr "type" "convert")
   (set_attr "length" "0")])

;; An insn to allocate new stack space for dynamic use (e.g., alloca).
;; We move the back-chain and decrement the stack pointer.
(define_expand "allocate_stack"
  [(set (match_operand 0 "register_operand" "=r")
	(minus (reg 1) (match_operand 1 "nonmemory_operand" "")))
   (set (reg 1)
	(minus (reg 1) (match_dup 1)))]
  ""
  "spu_allocate_stack (operands[0], operands[1]); DONE;")

;; These patterns say how to save and restore the stack pointer.  We need not
;; save the stack pointer at function or block level since we are careful to
;; preserve the backchain.  Doing nothing at block level means the stack space
;; is allocated until the end of the function.  This is currently safe to do
;; because gcc uses the frame pointer for the whole function, so the worst that
;; happens is wasted stack space.  That could be bad if a VLA is declared in a
;; loop, because new space will be allocated every iteration, but users can
;; work around that case.  Ideally we could detect when we are in a loop and
;; generate the more complicated code in that case.
;;
;; For nonlocal gotos, we must save both the stack pointer and its
;; backchain and restore both.  Note that in the nonlocal case, the
;; save area is a memory location.

(define_expand "save_stack_function"
  [(match_operand 0 "general_operand" "")
   (match_operand 1 "general_operand" "")]
  ""
  "DONE;")

(define_expand "restore_stack_function"
  [(match_operand 0 "general_operand" "")
   (match_operand 1 "general_operand" "")]
  ""
  "DONE;")

(define_expand "save_stack_block"
  [(match_operand 0 "general_operand" "")
   (match_operand 1 "general_operand" "")]
  ""
  "DONE; ")

(define_expand "restore_stack_block"
  [(use (match_operand 0 "register_operand" ""))
   (set (match_dup 2) (match_dup 3))
   (set (match_dup 0) (match_operand 1 "register_operand" ""))
   (set (match_dup 3) (match_dup 2))]
  ""
  "DONE;")

(define_expand "save_stack_nonlocal"
  [(match_operand 0 "memory_operand" "")
   (match_operand 1 "register_operand" "")]
  ""
  "
{
  rtx temp = gen_reg_rtx (Pmode);

  /* Copy the backchain to the first word, sp to the second.  We need to
     save the back chain because __builtin_apply appears to clobber it. */
  emit_move_insn (temp, gen_rtx_MEM (Pmode, operands[1]));
  emit_move_insn (adjust_address_nv (operands[0], SImode, 0), temp);
  emit_move_insn (adjust_address_nv (operands[0], SImode, 4), operands[1]);
  DONE;
}")

(define_expand "restore_stack_nonlocal"
  [(match_operand 0 "register_operand" "")
   (match_operand 1 "memory_operand" "")]
  ""
  "
{
  spu_restore_stack_nonlocal(operands[0], operands[1]);
  DONE;
}")

