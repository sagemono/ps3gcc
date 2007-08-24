;; Scheduling description for cell processors.

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

;; Sources: BE BOOK4 (/sfs/enc/doc/PPU_BookIV_DD3.0_latest.pdf)

;; BE Architechture *DD3.0 and DD3.1*
;; This file simulate PPU processor unit backend of pipeline, maualP24. 
;; manual P27, stall and flush points
;; IU, XU, VSU, dipatcher decodes and dispatch 2 insns per cycle in program order, the grouped adress are aligned by 8
;; This file only simulate one thread situation
;; XU executes all fixed point insns(3 units, a simple alu, a complex unit, and load/store unit)
;; VSU executes all scalar floating points insn(a float unit), VMX insns(VMX unit, 4 sub units, simple, permute, complex, floating point)

;; Dual issue combination

;;	FXU	LSU	BR 	VMX(sx,cx,vsu_fp,fp_arith)	VMX(perm,vsu_ls,fp_ls)
;;FXU	X

;;LSU		X               	X               	X	

;;BR			X

;;VMX(sx,cx,vsu_fp,fp_arth)		X

;;VMX(perm,vsu_ls, fp_ls)					X

;; Dual issue exceptons: 
;;(1) nop-pipelined FXU instr in slot 0 
;;(2) non-pipelined FPU inst in slot 0
;; CSI instr(contex-synchronizing insn)
;; Microcode insn

;; BRU unit: bru(none register stall), bru_cr(cr register stall)
;; VSU unit: vus(vmx simple), vup(vmx permute), vuc(vmx complex), vuf(vmx float), fpu(floats). fpu_div is hypthetical, it is for nonpipelined simulation
;; micr insns will stall at least 7 cycles to get the first instr from ROM, micro instructions are not dual issued. 

;; slot0 is older than slot1
;; non-pipelined insn need to be in slot1 to avoid 1cycle stall

;; There different stall point
;; IB2, only stall one thread if stall here, so try to stall here as much as we can 
;; condition(1) insert nop, OR and ORI instruction form 
;; condition(2) flush happens, in case of: RAW, WAW, D-ERAT miss, or CR0-access while stdcx, or stwcx
;; IS2 stall ;; Page91 for details
;; VQ8 stall
;; IS2 stall can be activated by VQ8 stall and trying to issue a vsu instr to the vsu issue queue

;;(define_automaton "cellxu")

;;(define_cpu_unit "fxu_sim_cell,fxu_mul_div_cell,lsu_cell,bru_cell,bru_cr_cell,vsu1_cell,vsu2_cell" "cellxu")

;; ndfa
(define_automaton "cellxu,cellvsu,cellbru")

(define_cpu_unit "fxu_sim_cell,fxu_mul_div_cell,lsu_cell" "cellxu")
(define_cpu_unit "bru_cell,bru_cr_cell" "cellbru")
(define_cpu_unit "vsu1_cell,vsu2_cell" "cellvsu")


(automata_option "v")
(automata_option "progress")
(automata_option "time")

(exclusion_set "fxu_sim_cell" "fxu_mul_div_cell")
(exclusion_set "bru_cell" "bru_cr_cell")

(define_reservation "nonpipeline" "fxu_mul_div_cell+lsu_cell+vsu1_cell+vsu2_cell")

;; Load/store
;;lmw, lswi, lswx are only generated for optimize for space, MC, these instr are not simulated
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
  "vsu2_cell+lsu_cell")

;; lfsu,lfsux,lfdu,lfdux 1cycle(fpr) 2 cycle(gpr)
(define_insn_reservation "cell-fpload-update" 1
  (and (eq_attr "type" "fpload,fpload_u,fpload_ux")
       (eq_attr "cpu" "cellppu"))
  "fxu_sim_cell+vsu2_cell+lsu_cell")

(define_insn_reservation "cell-vecload" 2
  (and (eq_attr "type" "vecload")
       (eq_attr "cpu" "cellppu"))
  "vsu2_cell+lsu_cell")

;;st? stw(MC)
;; all stores are issued in both LSU & FXU. The address flows in the
;; LSU pipe while the data in the FXU pipe.
(define_insn_reservation "cell-store" 1
  (and (eq_attr "type" "store")
       (eq_attr "cpu" "cellppu"))
  "lsu_cell+fxu_sim_cell")

;;stdux, stdu, (hardware breaks into store and add) 2 for update reg
(define_insn_reservation "cell-store-update" 1
  (and (eq_attr "type" "store_ux,store_u")
       (eq_attr "cpu" "cellppu"))
  "fxu_sim_cell+lsu_cell")

(define_insn_reservation "cell-fpstore" 1
  (and (eq_attr "type" "fpstore")
       (eq_attr "cpu" "cellppu"))
  "vsu2_cell+lsu_cell")

(define_insn_reservation "cell-fpstore-update" 1
  (and (eq_attr "type" "fpstore_ux,fpstore_u")
       (eq_attr "cpu" "cellppu"))
  "vsu2_cell+fxu_sim_cell+lsu_cell")

(define_insn_reservation "cell-vecstore" 1
  (and (eq_attr "type" "vecstore")
       (eq_attr "cpu" "cellppu"))
  "vsu2_cell+lsu_cell")

;; Integer latency is 2 cycles
(define_insn_reservation "cell-integer" 2
  (and (eq_attr "type" "integer")
       (eq_attr "cpu" "cellppu"))
  "fxu_sim_cell")

;; two and three Integer insturction sequences
;; Note that we cannot schedule other instructions into
;; the middle of these sequences. So they are treated like
;; non-pipelined instructions.
(define_insn_reservation "cell-two" 4
  (and (eq_attr "type" "two")
       (eq_attr "cpu" "cellppu"))
  "fxu_sim_cell,nonpipeline,fxu_sim_cell")

;; rlwimi, alter cr0  
(define_insn_reservation "cell-three" 6
  (and (eq_attr "type" "three")
       (eq_attr "cpu" "cellppu"))
  "fxu_sim_cell,nonpipeline*3,fxu_sim_cell")

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
  "nonpipeline,nonpipeline*14")

;; mulli, 8 cycles, not simulated
(define_insn_reservation "cell-imul" 10
  (and (eq_attr "type" "imul,imul2,imul3")
       (eq_attr "cpu" "cellppu"))
  "nonpipeline,nonpipeline*9")
 
;; divide
(define_insn_reservation "cell-idiv" 32
  (and (eq_attr "type" "idiv")
       (eq_attr "cpu" "cellppu"))
  "nonpipeline,nonpipeline*31")

(define_insn_reservation "cell-ldiv" 64
  (and (eq_attr "type" "ldiv")
       (eq_attr "cpu" "cellppu"))
  "nonpipeline,nonpipeline*63")

;;mflr mfctr other spr are non-pipelined,
(define_insn_reservation "cell-mfjmpr" 2
  (and (eq_attr "type" "mfjmpr")
       (eq_attr "cpu" "cellppu"))
  "bru_cell*2")

;;mtlr, mtctr,
;;mtspr fully pipelined 
(define_insn_reservation "cell-mtjmpr" 1
 (and (eq_attr "type" "mtjmpr")
       (eq_attr "cpu" "cellppu"))
  "bru_cell")

;; Branches
;; b, ba, bl, bla, unconditional branch always predicts correctly n/a latency
;; bcctr, bcctrl, latency 2, actually adjust by be to 4
(define_insn_reservation "cell-branch" 1
  (and (and (eq_attr "type" "branch")
       	    (eq_attr "cpu" "cellppu"))
       (not (eq_attr "size_note" "long_branch")))
  "bru_cell")

;; a long call sequence has a call followed by an NOP, which will
;; be patched by the loader.
(define_insn_reservation "cell-branchreg" 1
  (and (and (eq_attr "type" "jmpreg")
	    (eq_attr "cpu" "cellppu"))
       (not (eq_attr "size_note" "long_jmpreg")))
  "bru_cell")

(define_insn_reservation "cell-long-branch" 1
  (and (and (eq_attr "type" "branch")
       	    (eq_attr "cpu" "cellppu"))
       (eq_attr "size_note" "long_branch"))
  "bru_cell,fxu_sim_cell")

(define_insn_reservation "cell-long-jmpreg" 1
  (and (and (eq_attr "type" "jmpreg")
       	    (eq_attr "cpu" "cellppu"))
       (eq_attr "size_note" "long_jmpreg"))
  "bru_cell,lsu_cell")

;; cr hazard
;; page 90, special cases for CR hazard, only one instr can access cr per cycle
;; if insn reads CR following a stwcx, pipeline stall till stwcx finish
(define_insn_reservation "cell-crlogical" 1
  (and (eq_attr "type" "cr_logical,delayed_cr")
       (eq_attr "cpu" "cellppu"))
  "bru_cr_cell")

(define_insn_reservation "cell-mfcr" 34
  (and (eq_attr "type" "mfcr")
       (eq_attr "cpu" "cellppu"))
   "bru_cr_cell,bru_cr_cell*33")

; mtcrf (1 field)
(define_insn_reservation "cell-mtcrf" 1
  (and (eq_attr "type" "mtcr")
       (eq_attr "cpu" "cellppu"))
  "fxu_sim_cell")


; Basic FP latency is 10 cycles, thoughput is 1/cycle
(define_insn_reservation "cell-fp" 10
  (and (eq_attr "type" "fp,dmul")
       (eq_attr "cpu" "cellppu"))
  "vsu1_cell")

(define_insn_reservation "cell-fpcompare" 1
  (and (eq_attr "type" "fpcompare")
       (eq_attr "cpu" "cellppu"))
  "vsu1_cell")

;; sdiv thoughput 1/69, not pipelined, 
(define_insn_reservation "cell-sdiv" 69
  (and (eq_attr "type" "sdiv,ddiv")
       (eq_attr "cpu" "cellppu"))
  "vsu1_cell, vsu1_cell*68")

;; fsqrt thoughput 1/79, not pipelined
(define_insn_reservation "cell-sqrt" 79
  (and (eq_attr "type" "ssqrt,dsqrt")
       (eq_attr "cpu" "cellppu"))
 "vsu1_cell, vsu1_cell*78")

; VMX
(define_insn_reservation "cell-vecsimple" 4
  (and (eq_attr "type" "vecsimple")
       (eq_attr "cpu" "cellppu"))
  "vsu1_cell")

;; mult, div, madd
(define_insn_reservation "cell-veccomplex" 10
  (and (eq_attr "type" "veccomplex")
       (eq_attr "cpu" "cellppu"))
  "vsu1_cell")

(define_insn_reservation "cell-veccmp" 4
  (and (eq_attr "type" "veccmp")
       (eq_attr "cpu" "cellppu"))
  "vsu1_cell")

(define_insn_reservation "cell-vecfloat" 12
  (and (eq_attr "type" "vecfloat")
       (eq_attr "cpu" "cellppu"))
  "vsu1_cell")

(define_insn_reservation "cell-vecperm" 4
  (and (eq_attr "type" "vecperm")
       (eq_attr "cpu" "cellppu"))
  "vsu2_cell")

;; (define_bypass cycle "out-insns" "in-insns" [guards])
;; number defines when the result generated by the instructions given in string out_insn_names will be ready for the instructions given in string in_insn_names. The instructions in the string are separated by commas.

;; RAW register dependency

;; addi r3, r3, 1
;; lw r4,offset(r3)
;; there are 5 cycle deplay for r3 bypassing
;; there are 5 cycle delay for a dependent load after a load

;; address has a 5 cycle latency
(define_bypass 5 "cell-load,cell-load-ext,cell-integer"
 "cell-load,cell-load-ext,cell-fpload,cell-fpload-update,cell-vecload")
(define_bypass 5 "cell-load,cell-load-ext,cell-integer"
 "cell-store,cell-store-update,cell-fpstore,cell-fpstore-update,cell-vecstore"
 "rs6000_use_address_p")

;; VXU float RAW
(define_bypass 11 "cell-vecfloat" "cell-vecfloat")

;; VXU and FPU
(define_bypass 6 "cell-veccomplex" "cell-vecsimple")
(define_bypass 3 "cell-vecfloat" "cell-veccomplex")
(define_bypass 13 "cell-vecstore" "cell-fpstore")
(define_bypass 7 "cell-fp" "cell-fpload")
;; vsu1 should avoid writing to the same target register as vsu2 insn within 12 cycles. 
 
;; WAW hazard

;; the target of VSU estimate should not be reused within 10 dispatch groups
;; the target of VSU float should not be reused within 8 dispatch groups
;; the target of VSU complex should not be reused within 5 dispatch groups
;; FP LOAD should not reuse an FPU Arithmetic target with 6 dispatch gropus

;; mtctr-bcctr/bcctrl, branch target ctr register shadow update at ex4 stage(10 cycles)
(define_bypass 10 "cell-mtjmpr" "cell-branchreg")

;;Things are not simulated:
;;update instruction, update address gpr are not simulated
;;vrefp, vrsqrtefp have latency(14), currently simluated as 12cycle float insns

