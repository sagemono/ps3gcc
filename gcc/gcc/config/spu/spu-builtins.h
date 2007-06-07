
/* (C) Copyright
   Sony Computer Entertainment, Inc.,
   Toshiba Corporation,
   International Business Machines Corporation,
   2001,2002,2003,2004,2005.

   This file is free software; you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free
   Software Foundation; either version 2 of the License, or (at your option) 
   any later version.

   This file is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
   for more details.

   You should have received a copy of the GNU General Public License
   along with this file; see the file COPYING.  If not, write to the Free
   Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
   02110-1301, USA.  */

enum spu_builtin_type {
    B_INSN,
    B_JUMP,
    B_CJUMP,	/* conditional jump */
    B_BISLED,
    B_CALL,
    B_HINT,
    B_OVERLOAD, 
    B_INTERNAL
};


typedef enum {
#define DEF_BUILTIN(fcode, icode, name, type, params) fcode,
#include "spu_builtins.def"
#undef DEF_BUILTIN
   NUM_SPU_BUILTINS
} function_code;

struct spu_builtin_description {
    function_code fcode;
    int icode;
    const char *name;
    enum spu_builtin_type type;

    /* The first element of parm is always the return type.  The rest
     * are a zero terminated list of parameters. */
    int parm[5];

    tree fndecl;
};

