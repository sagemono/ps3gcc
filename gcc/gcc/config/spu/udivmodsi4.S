#; (C) Copyright
#; Sony Computer Entertainment, Inc.,
#; 2006.
#;
#; This file is free software; you can redistribute it and/or modify it under
#; the terms of the GNU General Public License as published by the Free
#; Software Foundation; either version 2 of the License, or (at your option)
#; any later version.
#;
#; This file is distributed in the hope that it will be useful, but WITHOUT
#; ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
#; FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
#; for more details.
#;
#; You should have received a copy of the GNU General Public License
#; along with this file; see the file COPYING.  If not, write to the Free
#; Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
#; 02110-1301, USA.  
#;
#; As a special exception, if you link this library with files compiled with
#; GCC to produce an executable, this does not cause the resulting executable
#; to be covered by the GNU General Public License.  The exception does not
#; however invalidate any other reasons why the executable file might be covered
#; by the GNU General Public License. 

#; Not necessarily the best implementation of divide but faster then
#; the default that gcc provides.
.text
        .align  3
        .global __udivmodsi4
        .type   __udivmodsi4, @function
__udivmodsi4:
	heqi	$5,0
	hbrr	2f,1f
	clz	$10,$5
	clz	$7,$4
	il	$8,1
	fsmbi	$3,0
	sf	$10,$7,$10
	ori	$6,$4,0
	shl	$8,$8,$10
	shl	$7,$5,$10
1:	or	$11,$3,$8
	rotqmbii	$8,$8,-1
	clgt	$9,$7,$6
	lnop
	sf	$10,$7,$6
	rotqmbii	$7,$7,-1
	selb	$3,$11,$3,$9
	lnop
	selb	$6,$10,$6,$9
2:	brnz	$8,1b
	bi	$12

