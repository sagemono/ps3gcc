/*
 * Special support for lv2
 *
 *   Copyright (C) 2007
 *   Extended from crtsavres.asm to support LV2 ABI save/restore
 *   functions.
 * 
 *   This file is part of GCC.
 *
 *   GCC is free software; you can redistribute it and/or modify it under
 *   the terms of the GNU General Public License as published by the Free
 *   Software Foundation; either version 2, or (at your option) any later
 *   version.
 *
 *   In addition to the permissions in the GNU General Public License, the
 *   Free Software Foundation gives you unlimited permission to link the
 *   compiled version of this file into combinations with other programs,
 *   and to distribute those combinations without any restriction coming
 *   from the use of this file.  (The General Public License restrictions
 *   do apply in other respects; for example, they cover modification of
 *   the file, and distribution when not linked into a combine
 *   executable.)
 *
 *   GCC is distributed in the hope that it will be useful, but WITHOUT ANY
 *   WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *   for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with GCC; see the file COPYING.  If not, write to the Free
 *   Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
 *   02110-1301, USA.  */

/* Do any initializations needed for the eabi environment */

	.file	"lv2savres.S"
	.section ".text"
	#include "ppc-asm.h"


/* Routines for saving floating point registers, called by the compiler.  */

FUNC_START(_savefpr_14)	stfd	14,-144(1)	/* save fp registers */
FUNC_START(_savefpr_15)	stfd	15,-136(1)
FUNC_START(_savefpr_16)	stfd	16,-128(1)
FUNC_START(_savefpr_17)	stfd	17,-120(1)
FUNC_START(_savefpr_18)	stfd	18,-112(1)
FUNC_START(_savefpr_19)	stfd	19,-104(1)
FUNC_START(_savefpr_20)	stfd	20,-96(1)
FUNC_START(_savefpr_21)	stfd	21,-88(1)
FUNC_START(_savefpr_22)	stfd	22,-80(1)
FUNC_START(_savefpr_23)	stfd	23,-72(1)
FUNC_START(_savefpr_24)	stfd	24,-64(1)
FUNC_START(_savefpr_25)	stfd	25,-56(1)
FUNC_START(_savefpr_26)	stfd	26,-48(1)
FUNC_START(_savefpr_27)	stfd	27,-40(1)
FUNC_START(_savefpr_28)	stfd	28,-32(1)
FUNC_START(_savefpr_29)	stfd	29,-24(1)
FUNC_START(_savefpr_30)	stfd	30,-16(1)
FUNC_START(_savefpr_31)	stfd	31,-8(1)
			std	0,16(1)  
			blr
FUNC_END(_savefpr_31)
FUNC_END(_savefpr_30)
FUNC_END(_savefpr_29)
FUNC_END(_savefpr_28)
FUNC_END(_savefpr_27)
FUNC_END(_savefpr_26)
FUNC_END(_savefpr_25)
FUNC_END(_savefpr_24)
FUNC_END(_savefpr_23)
FUNC_END(_savefpr_22)
FUNC_END(_savefpr_21)
FUNC_END(_savefpr_20)
FUNC_END(_savefpr_19)
FUNC_END(_savefpr_18)
FUNC_END(_savefpr_17)
FUNC_END(_savefpr_16)
FUNC_END(_savefpr_15)
FUNC_END(_savefpr_14)

/* Routines for saving integer registers, called by the compiler.  */

FUNC_START(_savegpr0_14)	std	14,-144(1)	/* save gp registers */
FUNC_START(_savegpr0_15)	std	15,-136(1)
FUNC_START(_savegpr0_16)	std	16,-128(1)
FUNC_START(_savegpr0_17)	std	17,-120(1)
FUNC_START(_savegpr0_18)	std	18,-112(1)
FUNC_START(_savegpr0_19)	std	19,-104(1)
FUNC_START(_savegpr0_20)	std	20,-96(1) 
FUNC_START(_savegpr0_21)	std	21,-88(1) 
FUNC_START(_savegpr0_22)	std	22,-80(1) 
FUNC_START(_savegpr0_23)	std	23,-72(1) 
FUNC_START(_savegpr0_24)	std	24,-64(1) 
FUNC_START(_savegpr0_25)	std	25,-56(1) 
FUNC_START(_savegpr0_26)	std	26,-48(1) 
FUNC_START(_savegpr0_27)	std	27,-40(1) 
FUNC_START(_savegpr0_28)	std	28,-32(1) 
FUNC_START(_savegpr0_29)	std	29,-24(1) 
FUNC_START(_savegpr0_30)	std	30,-16(1) 
FUNC_START(_savegpr0_31)	std	31,-8(1)  
				std	0,16(1)  
				blr
FUNC_END(_savegpr0_31)
FUNC_END(_savegpr0_30)
FUNC_END(_savegpr0_29)
FUNC_END(_savegpr0_28)
FUNC_END(_savegpr0_27)
FUNC_END(_savegpr0_26)
FUNC_END(_savegpr0_25)
FUNC_END(_savegpr0_24)
FUNC_END(_savegpr0_23)
FUNC_END(_savegpr0_22)
FUNC_END(_savegpr0_21)
FUNC_END(_savegpr0_20)
FUNC_END(_savegpr0_19)
FUNC_END(_savegpr0_18)
FUNC_END(_savegpr0_17)
FUNC_END(_savegpr0_16)
FUNC_END(_savegpr0_15)
FUNC_END(_savegpr0_14)

/* Called when we are also saving fprs */
FUNC_START(_savegpr1_14)	std	14,-144(12)	/* save gp registers */
FUNC_START(_savegpr1_15)	std	15,-136(12)
FUNC_START(_savegpr1_16)	std	16,-128(12)
FUNC_START(_savegpr1_17)	std	17,-120(12)
FUNC_START(_savegpr1_18)	std	18,-112(12)
FUNC_START(_savegpr1_19)	std	19,-104(12)
FUNC_START(_savegpr1_20)	std	20,-96(12) 
FUNC_START(_savegpr1_21)	std	21,-88(12) 
FUNC_START(_savegpr1_22)	std	22,-80(12) 
FUNC_START(_savegpr1_23)	std	23,-72(12) 
FUNC_START(_savegpr1_24)	std	24,-64(12) 
FUNC_START(_savegpr1_25)	std	25,-56(12) 
FUNC_START(_savegpr1_26)	std	26,-48(12) 
FUNC_START(_savegpr1_27)	std	27,-40(12) 
FUNC_START(_savegpr1_28)	std	28,-32(12) 
FUNC_START(_savegpr1_29)	std	29,-24(12) 
FUNC_START(_savegpr1_30)	std	30,-16(12) 
FUNC_START(_savegpr1_31)	std	31,-8(12)  
				blr
FUNC_END(_savegpr1_31)
FUNC_END(_savegpr1_30)
FUNC_END(_savegpr1_29)
FUNC_END(_savegpr1_28)
FUNC_END(_savegpr1_27)
FUNC_END(_savegpr1_26)
FUNC_END(_savegpr1_25)
FUNC_END(_savegpr1_24)
FUNC_END(_savegpr1_23)
FUNC_END(_savegpr1_22)
FUNC_END(_savegpr1_21)
FUNC_END(_savegpr1_20)
FUNC_END(_savegpr1_19)
FUNC_END(_savegpr1_18)
FUNC_END(_savegpr1_17)
FUNC_END(_savegpr1_16)
FUNC_END(_savegpr1_15)
FUNC_END(_savegpr1_14)

FUNC_START(_savevr_20)	li	12,-192
			stvx	20,12,0
FUNC_START(_savevr_21)	li	12,-176
			stvx	21,12,0
FUNC_START(_savevr_22)	li	12,-160
			stvx	22,12,0
FUNC_START(_savevr_23)	li	12,-144
			stvx	23,12,0
FUNC_START(_savevr_24)	li	12,-128
			stvx	24,12,0
FUNC_START(_savevr_25)	li	12,-112
			stvx	25,12,0
FUNC_START(_savevr_26)	li	12,-96
			stvx	26,12,0
FUNC_START(_savevr_27)	li	12,-80
			stvx	27,12,0
FUNC_START(_savevr_28)	li	12,-64
			stvx	28,12,0
FUNC_START(_savevr_29)	li	12,-48
			stvx	29,12,0
FUNC_START(_savevr_30)	li	12,-32
			stvx	30,12,0
FUNC_START(_savevr_31)	li	12,-16
			stvx	31,12,0
			blr
FUNC_END(_savevr_31)
FUNC_END(_savevr_30)
FUNC_END(_savevr_29)
FUNC_END(_savevr_28)
FUNC_END(_savevr_27)
FUNC_END(_savevr_26)
FUNC_END(_savevr_25)
FUNC_END(_savevr_24)
FUNC_END(_savevr_23)
FUNC_END(_savevr_22)
FUNC_END(_savevr_21)
FUNC_END(_savevr_20)

/* Routines for restoring floating point registers, called by the compiler.  */

FUNC_START(_restfpr_14)	lfd	14,-144(1)	/* restore fp registers */
FUNC_START(_restfpr_15)	lfd	15,-136(1)
FUNC_START(_restfpr_16)	lfd	16,-128(1)
FUNC_START(_restfpr_17)	lfd	17,-120(1)
FUNC_START(_restfpr_18)	lfd	18,-112(1)
FUNC_START(_restfpr_19)	lfd	19,-104(1)
FUNC_START(_restfpr_20)	lfd	20,-96(1)
FUNC_START(_restfpr_21)	lfd	21,-88(1)
FUNC_START(_restfpr_22)	lfd	22,-80(1)
FUNC_START(_restfpr_23)	lfd	23,-72(1)
FUNC_START(_restfpr_24)	lfd	24,-64(1)
FUNC_START(_restfpr_25)	lfd	25,-56(1)
FUNC_START(_restfpr_26)	lfd	26,-48(1)
FUNC_START(_restfpr_27)	lfd	27,-40(1)
FUNC_START(_restfpr_28)	lfd	28,-32(1)
FUNC_START(_restfpr_29)	ld	0,16(1)  
			lfd	29,-24(1)
			mtlr	0
			lfd	30,-16(1)
			lfd	31,-8(1)
			blr
FUNC_END(_restfpr_29)
FUNC_END(_restfpr_28)
FUNC_END(_restfpr_27)
FUNC_END(_restfpr_26)
FUNC_END(_restfpr_25)
FUNC_END(_restfpr_24)
FUNC_END(_restfpr_23)
FUNC_END(_restfpr_22)
FUNC_END(_restfpr_21)
FUNC_END(_restfpr_20)
FUNC_END(_restfpr_19)
FUNC_END(_restfpr_18)
FUNC_END(_restfpr_17)
FUNC_END(_restfpr_16)
FUNC_END(_restfpr_15)
FUNC_END(_restfpr_14)

FUNC_START(_restfpr_30) lfd	30,-16(1)
FUNC_START(_restfpr_31) ld	0,16(1)
			lfd	31,-8(1)
			mtlr	0
			blr
FUNC_END(_restfpr_31)
FUNC_END(_restfpr_30)

/* Routines for restoring integer registers, called by the compiler.  */

FUNC_START(_restgpr0_14)	ld	14,-144(1)	/* restore gp registers */
FUNC_START(_restgpr0_15)	ld	15,-136(1)
FUNC_START(_restgpr0_16)	ld	16,-128(1)
FUNC_START(_restgpr0_17)	ld	17,-120(1)
FUNC_START(_restgpr0_18)	ld	18,-112(1)
FUNC_START(_restgpr0_19)	ld	19,-104(1)
FUNC_START(_restgpr0_20)	ld	20,-96(1)
FUNC_START(_restgpr0_21)	ld	21,-88(1)
FUNC_START(_restgpr0_22)	ld	22,-80(1)
FUNC_START(_restgpr0_23)	ld	23,-72(1)
FUNC_START(_restgpr0_24)	ld	24,-64(1)
FUNC_START(_restgpr0_25)	ld	25,-56(1)
FUNC_START(_restgpr0_26)	ld	26,-48(1)
FUNC_START(_restgpr0_27)	ld	27,-40(1)
FUNC_START(_restgpr0_28)	ld	28,-32(1)
FUNC_START(_restgpr0_29)	ld	0,16(1)  
				ld	29,-24(1)
				mtlr	0
				ld	30,-16(1)
				ld	31,-8(1)
				blr
FUNC_END(_restgpr0_29)
FUNC_END(_restgpr0_28)
FUNC_END(_restgpr0_27)
FUNC_END(_restgpr0_26)
FUNC_END(_restgpr0_25)
FUNC_END(_restgpr0_24)
FUNC_END(_restgpr0_23)
FUNC_END(_restgpr0_22)
FUNC_END(_restgpr0_21)
FUNC_END(_restgpr0_20)
FUNC_END(_restgpr0_19)
FUNC_END(_restgpr0_18)
FUNC_END(_restgpr0_17)
FUNC_END(_restgpr0_16)
FUNC_END(_restgpr0_15)
FUNC_END(_restgpr0_14)

FUNC_START(_restgpr0_30)	ld	30,-16(1)
FUNC_START(_restgpr0_31)	ld	0,16(1)
				ld	31,-8(1)
				mtlr	0
				blr
FUNC_END(_restgpr0_31)
FUNC_END(_restgpr0_30)

/* Called when we are also restoring fprs */
FUNC_START(_restgpr1_14)	ld	14,-144(12)	/* restore gp registers */
FUNC_START(_restgpr1_15)	ld	15,-136(12)
FUNC_START(_restgpr1_16)	ld	16,-128(12)
FUNC_START(_restgpr1_17)	ld	17,-120(12)
FUNC_START(_restgpr1_18)	ld	18,-112(12)
FUNC_START(_restgpr1_19)	ld	19,-104(12)
FUNC_START(_restgpr1_20)	ld	20,-96(12)
FUNC_START(_restgpr1_21)	ld	21,-88(12)
FUNC_START(_restgpr1_22)	ld	22,-80(12)
FUNC_START(_restgpr1_23)	ld	23,-72(12)
FUNC_START(_restgpr1_24)	ld	24,-64(12)
FUNC_START(_restgpr1_25)	ld	25,-56(12)
FUNC_START(_restgpr1_26)	ld	26,-48(12)
FUNC_START(_restgpr1_27)	ld	27,-40(12)
FUNC_START(_restgpr1_28)	ld	28,-32(12)
FUNC_START(_restgpr1_29)	ld	29,-24(12)
FUNC_START(_restgpr1_30)	ld	30,-16(12)
FUNC_START(_restgpr1_31)	ld	31,-8(12)
				blr
FUNC_END(_restgpr1_31)
FUNC_END(_restgpr1_30)
FUNC_END(_restgpr1_29)
FUNC_END(_restgpr1_28)
FUNC_END(_restgpr1_27)
FUNC_END(_restgpr1_26)
FUNC_END(_restgpr1_25)
FUNC_END(_restgpr1_24)
FUNC_END(_restgpr1_23)
FUNC_END(_restgpr1_22)
FUNC_END(_restgpr1_21)
FUNC_END(_restgpr1_20)
FUNC_END(_restgpr1_19)
FUNC_END(_restgpr1_18)
FUNC_END(_restgpr1_17)
FUNC_END(_restgpr1_16)
FUNC_END(_restgpr1_15)
FUNC_END(_restgpr1_14)

FUNC_START(_restvr_20)	li	12,-192
			lvx	20,12,0
FUNC_START(_restvr_21)	li	12,-176
			lvx	21,12,0
FUNC_START(_restvr_22)	li	12,-160
			lvx	22,12,0
FUNC_START(_restvr_23)	li	12,-144
			lvx	23,12,0
FUNC_START(_restvr_24)	li	12,-128
			lvx	24,12,0
FUNC_START(_restvr_25)	li	12,-112
			lvx	25,12,0
FUNC_START(_restvr_26)	li	12,-96
			lvx	26,12,0
FUNC_START(_restvr_27)	li	12,-80
			lvx	27,12,0
FUNC_START(_restvr_28)	li	12,-64
			lvx	28,12,0
FUNC_START(_restvr_29)	li	12,-48
			lvx	29,12,0
FUNC_START(_restvr_30)	li	12,-32
			lvx	30,12,0
FUNC_START(_restvr_31)	li	12,-16
			lvx	31,12,0
			blr
FUNC_END(_restvr_31)
FUNC_END(_restvr_30)
FUNC_END(_restvr_29)
FUNC_END(_restvr_28)
FUNC_END(_restvr_27)
FUNC_END(_restvr_26)
FUNC_END(_restvr_25)
FUNC_END(_restvr_24)
FUNC_END(_restvr_23)
FUNC_END(_restvr_22)
FUNC_END(_restvr_21)
FUNC_END(_restvr_20)
