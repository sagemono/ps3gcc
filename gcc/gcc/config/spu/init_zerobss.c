/* (C) Copyright
 * Sony Computer Entertainment, Inc.,
 * 2008.
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

/* Zero out the bss code.  */

#include <spu_intrinsics.h>

/* This is forced to 16 byte alignment by the linker script */
extern char __bss_start[];


static void __init_zerobss (void) __attribute__ ((naked, section(".init.zerobss","ax"), used));
static void
__init_zerobss (void)
{
  __asm__ volatile (
	".align	3\n"
"	ai	$6,%0,__ABS__bss_init_offset\n"
"	hbrr	2f,__ABS__bss_init_hint\n"
"	ila	$5,__ABS__bss_init_iters\n"
"	fsmbi	$3,0\n"
"	ori	$4,$6,0\n"
"	br	__ABS__bss_init_jump\n"
"1:	ai	$5,$5,-1\n"
"	stqd	$3,-(16*8)($6)\n"
"	stqd	$3,-(16*7)($4)\n"
"	stqd	$3,-(16*6)($4)\n"
"	stqd	$3,-(16*5)($4)\n"
"	stqd	$3,-(16*4)($4)\n"
"	stqd	$3,-(16*3)($4)\n"
"	stqd	$3,-(16*2)($4)\n"
"	ai	$6,$4,16*8\n"
"	stqd	$3,-(16*1)($4)\n"
"	ai	$4,$4,16*8\n"
"2:	brnz	$5,1b"
	:
	: "r" (__bss_start)
	: "$3", "$4", "$5", "$6", "memory", "hbr");

}
