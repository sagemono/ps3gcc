#   Copyright (C) 2001 Free Software Foundation, Inc.
#   Written By Nick Clifton
# 
# This file is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the
# Free Software Foundation; either version 2, or (at your option) any
# later version.
# 
# In addition to the permissions in the GNU General Public License, the
# Free Software Foundation gives you unlimited permission to link the
# compiled version of this file with other programs, and to distribute
# those programs without any restriction coming from the use of this
# file.  (The General Public License restrictions do apply in other
# respects; for example, they cover modification of the file, and
# distribution when not linked into another program.)
# 
# This file is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; see the file COPYING.  If not, write to
# the Free Software Foundation, 59 Temple Place - Suite 330,
# Boston, MA 02111-1307, USA.
# 
#    As a special exception, if you link this library with files
#    compiled with GCC to produce an executable, this does not cause
#    the resulting executable to be covered by the GNU General Public License.
#    This exception does not however invalidate any other reasons why
#    the executable file might be covered by the GNU General Public License.
# 

# This file contains the epilogue part of the special __init() and
# __fini() functions.  .init and .fini sections written by a user must
# not use non-volatile registers that aren't saved here, and they must
# not clobber the stack space allocated here.  This file must be the
# last thing linked into any executable.

	# Note - this macro is complimented by the FUNC_START macro
	# in crti.asm.  If you change this macro you must also change
	# that macro match.
	#
	# Note - we do not try any fancy optimisations of the return
	# sequences here, it is just not worth it.  Instead keep things
	# simple.  Restore all the save resgisters, including the link
	# register and then perform the correct function return instruction.
.macro FUNC_END
	lqd	$lr, 96($sp)
	lqd	$80, 32($sp)
	lqd	$81, 48($sp)
	lqd	$126, 64($sp)
	ai	$sp, $sp, 80
	bi	$lr
.endm
		
	
	.file		"crtn.asm"

	.section	".init.tail","ax"
	;;
	FUNC_END
	
	.section	".fini.tail","ax"
	;;
	FUNC_END
	
# end of crtn.asm
