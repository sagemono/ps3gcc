;; Scheduling description for IBM Power4 and PowerPC 970 processors.

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

;; Sources: BE BOOK4

;; BE Architechture (old manual) *DD2*
;; IU, XU, VSU, dipatcher decodes and dispatch 2 insns per cycle in program order
;; XU executes all fixed point insns(3 units, a simple alu, a complex unit, and load/store unit)
;; VSU executes all scalar floating points insn(a float unit), VMX insns(VMX unit, 4 sub units, simple, permute, complex, floating point)
;; Dual issue combination
;;	FXU	LSU	BR	FP 	VMX(sx,cx,sp)	VMX(perm)
;;FXU	X
;;LSU		X               X       X               X	
;;BR			X
;;FP				X
;;VMX(sx,cx,sp)				X
;;VMX(perm)						X

;; Dual issue exceptons: 
;;(1) nop-pipelined FXU instr in slot 0 
;;(2) non-pipelined FPU inst in slot 0
;; CSI instr(contex-synchronizing insn)
;; Microcode insn

;; BE Architechture (new manual) *DD1*
;; single issue
;; XU unit: simple(xu_sim), complex(xu_com=mul+div, hypothetical), LSU(xu_lsu))(fxu include sim, mul,div)
;; BRU unit: bru(none register stall), bru_cr(cr register stall)
;; VSU unit: vus(vmx simple), vup(vmx permute), vuc(vmx complex), vuf(vmx float), fpu(floats). fpu_div is hypthetical, it is for nonpipelined simulation
;; micr insns will stall at least 7 cycles to get the first instr from ROM, micro instructions are not dual issued. 


(define_automaton "cellxu,cellvsu,cellbru")

(define_cpu_unit "fxu_sim_cell,fxu_mul_div_cell,lsu_cell" "cellxu")
(define_cpu_unit "bru_cell,bru_cr_cell" "cellbru")
(define_cpu_unit "vus_cell,vup_cell,fpu_cell" "cellvsu")

(automata_option "v")
(automata_option "progress")
(automata_option "time")

(exclusion_set "fxu_sim_cell" "fxu_mul_div_cell")

(exclusion_set "bru_cell" "bru_cr_cell")
(exclusion_set "vus_cell" "vup_cell")

;; Load/store
;;lmw, lswi, lswx are only generated for optimize for space, MC
(define_insn_reservation "cell-load" 2
  (and (eq_attr "type" "load")
       (eq_attr "cpu" "cellppu"))
  "lsu_cell")

;;ldux,ldu,lbzux,lbzu, hardware breaks it down to two instrs,if with 32bytes alignment,CMC
(define_insn_reservation "cell-load-ux" 2
  (and (eq_attr "type" "load_ux,load_u")
       (eq_attr "cpu" "cellppu"))
  "fxu_sim_cell+lsu_cell")

;;lha,lhax,lhau,lhaux,lwa,lwax,lwaux, MC, latency unknow 11/7, 11/8, 11/12
(define_insn_reservation "cell-load-ext" 2
  (and (eq_attr "type" "load_ext,load_ext_u,load_ext_ux")
       (eq_attr "cpu" "cellppu"))
  "fxu_sim_cell+lsu_cell")

;;lfs,lfsx,lfd,lfdx, 1 cycle
(define_insn_reservation "cell-fpload" 1
  (and (eq_attr "type" "fpload")
       (eq_attr "cpu" "cellppu"))
  "lsu_cell+fpu_cell")

;; lfsu,lfsux,lfdu,lfdux 1cycle(fpr) 2 cycle(gpr)
(define_insn_reservation "cell-fpload-update" 1
  (and (eq_attr "type" "fpload,fpload_u,fpload_ux")
       (eq_attr "cpu" "cellppu"))
  "lsu_cell+fxu_sim_cell")

(define_insn_reservation "cell-vecload" 2
  (and (eq_attr "type" "vecload")
       (eq_attr "cpu" "cellppu"))
  "lsu_cell")

;;st? stw(MC)
(define_insn_reservation "cell-store" 1
  (and (eq_attr "type" "store")
       (eq_attr "cpu" "cellppu"))
  "lsu_cell")

;;stdux, stdu, (hardware breaks into store and add) 2 for update reg
(define_insn_reservation "cell-store-update" 1
  (and (eq_attr "type" "store_ux,store_u")
       (eq_attr "cpu" "cellppu"))
  "fxu_sim_cell+lsu_cell")

(define_insn_reservation "cell-fpstore" 1
  (and (eq_attr "type" "fpstore")
       (eq_attr "cpu" "cellppu"))
  "lsu_cell+fpu_cell")

(define_insn_reservation "cell-fpstore-update" 1
  (and (eq_attr "type" "fpstore_ux,fpstore_u")
       (eq_attr "cpu" "cellppu"))
  "lsu_cell+fxu_sim_cell")

;; manual says n/a
(define_insn_reservation "cell-vecstore" 1
  (and (eq_attr "type" "vecstore")
       (eq_attr "cpu" "cellppu"))
  "lsu_cell")

;; Integer latency is 2 cycles
(define_insn_reservation "cell-integer" 2
  (and (eq_attr "type" "integer")
       (eq_attr "cpu" "cellppu"))
  "fxu_sim_cell")

;; rlwimi, alter cr0  
(define_insn_reservation "cell-insert" 2
  (and (eq_attr "type" "insert_word")
       (eq_attr "cpu" "cellppu"))
 "fxu_sim_cell")

;; cmpi, cmpli, cmpla, add, addo, sub, subo, alter cr0 
(define_insn_reservation "cell-cmp" 1
  (and (eq_attr "type" "cmp,fast_compare")
       (eq_attr "cpu" "cellppu"))
  "fxu_sim_cell")

;; add, addo, sub, subo, alter cr0, rldcli, rlwinm 
(define_insn_reservation "cell-fast-cmp" 2
  (and (eq_attr "type" "compare,fast_compare,delayed_compare")
       (eq_attr "cpu" "cellppu"))
  "fxu_sim_cell")

;; mulld
(define_insn_reservation "cell-lmul-cmp" 15
  (and (eq_attr "type" "lmul,lmul_compare")
       (eq_attr "cpu" "cellppu"))
  "fxu_mul_div_cell,fxu_mul_div_cell*14")

;; mulli, 8 cycles, not simulated
(define_insn_reservation "cell-imul" 10
  (and (eq_attr "type" "imul,imul2,imul3")
       (eq_attr "cpu" "cellppu"))
  "fxu_sim_cell+fxu_mul_div_cell,fxu_mul_div_cell*9")
 
;; divide
(define_insn_reservation "cell-idiv" 32
  (and (eq_attr "type" "idiv")
       (eq_attr "cpu" "cellppu"))
  "fxu_mul_div_cell, fxu_mul_div_cell*31")

(define_insn_reservation "cell-ldiv" 64
  (and (eq_attr "type" "ldiv")
       (eq_attr "cpu" "cellppu"))
  "fxu_mul_div_cell, fxu_mul_div_cell*63")

;;(define_insn_reservation "cell-mtjmpr" 3
;;  (and (eq_attr "type" "mtjmpr,mfjmpr")
;;       (eq_attr "cpu" "cellppu"))
;;  "du1_cell,bpu_cell")

;; Branches
;; b, ba, bl, bla, unconditional branch always predicts correctly n/a latency
;; bcctr, bcctrl, latency 2, but I dont see where these instructions get emitted 
;; jmpreg seems to be 2 instructions then this is incorrect, todo
(define_insn_reservation "cell-branch" 1
  (and (eq_attr "type" "jmpreg,branch")
       (eq_attr "cpu" "cellppu"))
  "bru_cell")

;; cr hazard
;; todo if insn reads CR following a stwcx, pipeline stall till stwcx finish
;; todo fcompare if denormalied number, set flag bit for a specific CR field, then following CR dependent is refeteched
(define_insn_reservation "cell-crlogical" 1
  (and (eq_attr "type" "cr_logical")
       (eq_attr "cpu" "cellppu"))
  "bru_cr_cell")

(define_insn_reservation "cell-mfcr" 8
  (and (eq_attr "type" "mfcr")
       (eq_attr "cpu" "cellppu"))
   "bru_cr_cell,bru_cr_cell*7")

; mtcrf (1 field)
(define_insn_reservation "cell-mtcrf" 1
  (and (eq_attr "type" "mtcr")
       (eq_attr "cpu" "cellppu"))
  "fxu_sim_cell")

;; Things need to add for fpu unit
;; mcrfs(14cycle) mffs, mffs.(11) fsel(10, 1 for cr)
;; fres, frsqrte, fdiv, fdivs,fsqrt,fsqrts, fres. frsqrte. 10 or 1(cr) , thoughtput 1/10
;; frsp. fctid. fctidz. fctiw. fctiwz. fefid. 10, thoughput 1/10

;; mtfsfi mtfsfi. mtfst mtfst. mtfsb0 mtfsb0. mtfcb1 mtfcb1. 12, thoughtput 1/cycle

; Basic FP latency is 10 cycles, thoughput is 1/cycle
(define_insn_reservation "cell-fp" 10
  (and (eq_attr "type" "fp,dmul")
       (eq_attr "cpu" "cellppu"))
  "fpu_cell")

(define_insn_reservation "cell-fpcompare" 1
  (and (eq_attr "type" "fpcompare")
       (eq_attr "cpu" "cellppu"))
  "fpu_cell")

;; sdiv thoughput 1/69, not pipelined, 
(define_insn_reservation "cell-sdiv" 69
  (and (eq_attr "type" "sdiv,ddiv")
       (eq_attr "cpu" "cellppu"))
  "fpu_cell, fpu_cell*68")

;; fsqrt thoughput 1/79, not pipelined
(define_insn_reservation "cell-sqrt" 79
  (and (eq_attr "type" "ssqrt,dsqrt")
       (eq_attr "cpu" "cellppu"))
 "fpu_cell, fpu_cell*78")

; VMX
(define_insn_reservation "cell-vecsimple" 4
  (and (eq_attr "type" "vecsimple")
       (eq_attr "cpu" "cellppu"))
  "vus_cell")

;; mult, div, madd
(define_insn_reservation "cell-veccomplex" 10
  (and (eq_attr "type" "veccomplex")
       (eq_attr "cpu" "cellppu"))
  "vus_cell")

(define_insn_reservation "cell-veccmp" 4
  (and (eq_attr "type" "veccmp")
       (eq_attr "cpu" "cellppu"))
  "vus_cell")

(define_insn_reservation "cell-vecfloat" 12
  (and (eq_attr "type" "vecfloat")
       (eq_attr "cpu" "cellppu"))
  "vus_cell")

(define_insn_reservation "cell-vecperm" 4
  (and (eq_attr "type" "vecperm")
       (eq_attr "cpu" "cellppu"))
  "vup_cell")

;; (define_bypass cycle "out-insns" "in-insns")

;; addi r3, r3, 1
;; lw r4,offset(r3)
;; there are 5 cycle deplay for r3 bypassing
;; there are 5 cycle delay for a dependent load after a load
(define_bypass 5 "cell-integer" "cell-load")
(define_bypass 5 "cell-integer" "cell-load-ext")
(define_bypass 5 "cell-load" "cell-load")

;;(define_bypass 8 "cell-veccomplex" "cell-veccomplex")

;;Things are not simulated:
;;update instruction, update address gpr are not simulated
;;microcode stall at least 7 cycles before dispatch, currently only issue one mc per cycle, delay is not simulated
;;vuc, vuf can only issue every the other cycle due to halfwidth
;;mtspr, mfspr, XER, LR simulated as simple insn
;;vrefp, vrsqrtefp have longer latency(14), instead of(12), not simulated

