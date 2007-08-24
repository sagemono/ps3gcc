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
#ifndef LTO_INFO_ASM_H
#define LTO_INFO_ASM_H


#include <stdio.h>
#include "lto/lto-info.h"

extern void
lto_asm_fn_start( FILE * file, int no_return, int has_nonlocal_label,
		  const char *linkonce_name );

void
lto_asm_fn_proto( FILE * file,
                  lto_args_kind_t args_kind, long long unsigned args );

extern void
lto_asm_fn_call( FILE * file, int sibcall, int no_return,
		lto_args_kind_t args_kind, long long unsigned args );

extern void
lto_asm_noargs( FILE * file, annotation_kind_t kind );

extern void
lto_asm_noargs_tab( FILE * file, annotation_kind_t kind );

extern void
lto_asm_onearg( FILE * file, annotation_kind_t kind, int arg );

extern void
lto_asm_tablejump( FILE * file, unsigned labelnum );

extern void
lto_asm_jumptable_end( FILE * file, unsigned labelnum );

extern void
lto_asm_alias( FILE * file, int is_volatile, long long int info );

#endif /* LTO_INFO_ASM_H */
