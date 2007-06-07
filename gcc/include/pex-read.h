/* Declarations of the pex-read functions for libiberty.
   Copyright 2004 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

#ifndef PEX_READ_H
#define PEX_READ_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif
 
/* Execute a program and read its standard output.  */

extern FILE *pex_read (const char *, char * const *, const char *,
		       const char *, char **, char **, int, int *);

/* Wait for pex_read to finish.  */

extern int pex_read_close (int, FILE *, int *, int);

#ifdef __cplusplus
}
#endif

#endif /* PEX_READ_H */
