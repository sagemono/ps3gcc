
/* (C) Copyright
   Sony Computer Entertainment, Inc.,
   Toshiba Corporation,
   International Business Machines Corporation,
   2001,2002,2003,2004,2005,2006.

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

#undef CPP_SPEC
#define	CPP_SPEC "-D__SPU__ %{mis:-D__IS__}"

#undef TARGET_OS_CPP_BUILTINS
#define TARGET_OS_CPP_BUILTINS()	\
  do {					\
    if (spu_float_acc == SPU_FP_ACCURATE) \
      builtin_define("__FLOAT_ACCURATE__");	\
    else if (spu_float_acc == SPU_FP_FAST) \
      builtin_define("__FLOAT_FAST__");	\
    if (spu_double_acc == SPU_FP_ACCURATE) \
      builtin_define("__DOUBLE_ACCURATE__");	\
    else if (spu_double_acc == SPU_FP_FAST) \
      builtin_define("__DOUBLE_FAST__");	\
  } while (0)

#undef  STARTFILE_SPEC
#define STARTFILE_SPEC	""

#undef LIB_SPEC
#define LIB_SPEC "--start-group -lc -lgcc -lstdc++ -lsupc++ --end-group"

#undef INCLUDE_DEFAULTS
#define INCLUDE_DEFAULTS		   \
{					   \
  /* This is the dir for fixincludes and for gcc's private headers.  */ \
  { GCC_INCLUDE_DIR, "GCC", 0, 0, 0 },     \
  { "/include", STANDARD_INCLUDE_COMPONENT, 0, 0, 1 }, \
  { 0, 0, 0, 0, 0 }			   \
}

