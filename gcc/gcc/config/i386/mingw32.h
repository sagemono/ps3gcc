/* Operating system specific defines to be used when targeting GCC for
   hosting on Windows32, using GNU tools and the Windows32 API Library.
   Copyright (C) 1997, 1998, 1999, 2000, 2001, 2002, 2003, 2004
   Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to
the Free Software Foundation, 51 Franklin Street, Fifth Floor,
Boston, MA 02110-1301, USA.  */

#undef TARGET_VERSION
#define TARGET_VERSION fprintf (stderr, " (x86 MinGW)"); 

/* See i386/crtdll.h for an alternative definition.  */
#define EXTRA_OS_CPP_BUILTINS()					\
  do								\
    {								\
      builtin_define ("__MSVCRT__");				\
      builtin_define ("__MINGW32__");			   	\
      builtin_define ("_WIN32");				\
      builtin_define_std ("WIN32");				\
      builtin_define_std ("WINNT");				\
    }								\
  while (0)

/* Override the standard choice of /usr/include as the default prefix
   to try when searching for header files.  */
#undef STANDARD_INCLUDE_DIR
#define STANDARD_INCLUDE_DIR "/mingw/include"
#undef STANDARD_INCLUDE_COMPONENT
#define STANDARD_INCLUDE_COMPONENT "MINGW"

#undef CPP_SPEC
#define CPP_SPEC "%{posix:-D_POSIX_SOURCE} %{mthreads:-D_MT}"

/* For Windows applications, include more libraries, but always include
   kernel32.  */
#undef LIB_SPEC
#define LIB_SPEC "%{pg:-lgmon} %{mwindows:-lgdi32 -lcomdlg32} \
                  -luser32 -lkernel32 -ladvapi32 -lshell32 -lmsvcp60"

/* Include in the mingw32 libraries with libgcc */
#undef LINK_SPEC
#define LINK_SPEC "%{mwindows:--subsystem windows} \
  %{mconsole:--subsystem console} \
  %{shared: %{mdll: %eshared and mdll are not compatible}} \
  %{shared: --shared} %{mdll:--dll} \
  %{static:-Bstatic} %{!static:-Bdynamic} \
  %{shared|mdll: -e _DllMainCRTStartup@12}"

/* Include in the mingw32 libraries with libgcc */
#undef LIBGCC_SPEC
#define LIBGCC_SPEC \
  "%{mthreads:-lmingwthrd} -lmingw32 -lgcc -lmoldname -lmingwex -lmsvcrt"

#undef STARTFILE_SPEC
#define STARTFILE_SPEC "%{shared|mdll:dllcrt2%O%s} \
  %{!shared:%{!mdll:crt2%O%s}} %{pg:gcrt2%O%s}  \
  %{!fno-exceptions:crtbegin%O%s}"

#undef ENDFILE_SPEC
#define ENDFILE_SPEC "%{!fno-exceptions:crtend%O%s}"

/* Override startfile prefix defaults.  */
#ifndef STANDARD_STARTFILE_PREFIX_1
#define STANDARD_STARTFILE_PREFIX_1 "/mingw/lib/"
#endif
#ifndef STANDARD_STARTFILE_PREFIX_2
#define STANDARD_STARTFILE_PREFIX_2 ""
#endif

/* Output STRING, a string representing a filename, to FILE.
   We canonicalize it to be in Unix format (backslashes are replaced
   forward slashes.  */
#undef OUTPUT_QUOTED_STRING
#define OUTPUT_QUOTED_STRING(FILE, STRING)               \
do {						         \
  char c;					         \
						         \
  putc ('\"', asm_file);			         \
						         \
  while ((c = *string++) != 0)			         \
    {						         \
      if (c == '\\')				         \
	c = '/';				         \
						         \
      if (ISPRINT (c))                                   \
        {                                                \
          if (c == '\"')			         \
	    putc ('\\', asm_file);		         \
          putc (c, asm_file);			         \
        }                                                \
      else                                               \
        fprintf (asm_file, "\\%03o", (unsigned char) c); \
    }						         \
						         \
  putc ('\"', asm_file);			         \
} while (0)

/* Define as short unsigned for compatibility with MS runtime.  */
#undef WINT_TYPE
#define WINT_TYPE "short unsigned int"

/* MSVCRT does not support the "I" flag provided by GLIBC.  */
#undef TARGET_EXTRA_PRINTF_FLAG_CHARS
#define TARGET_EXTRA_PRINTF_FLAG_CHARS ""
#undef TARGET_EXTRA_SCANF_FLAG_CHARS
#define TARGET_EXTRA_SCANF_FLAG_CHARS ""

/* MSVCRT supports additional length specifiers for "printf".  (In
   fact, it does not support some of the C99 specifiers, like
   "ll".  However, we do not presently have a mechanism for disabling
   a specifiers.)  */
#define TARGET_EXTRA_PRINTF_LENGTH_SPECIFIERS   \
   /* 32-bit integer */                         \
   { "I32", FMT_LEN_l, STD_EXT, NULL, 0, 0 },   \
   /* 64-bit integer */                         \
   { "I64", FMT_LEN_ll, STD_EXT, NULL, 0, 0 },  \
   /* size_t or ptrdiff_t */                    \
   { "I", FMT_LEN_t, STD_EXT, NULL, 0, 0 }

#define TARGET_EXTRA_SCANF_LENGTH_SPECIFIERS    \
   TARGET_EXTRA_PRINTF_LENGTH_SPECIFIERS

