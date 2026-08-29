/* Execute a program and read stdout.  Generic version.
   Copyright (C) 2004 Free Software Foundation, Inc.

This file is part of the libiberty library.
Libiberty is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public
License as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.

Libiberty is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with libiberty; see the file COPYING.LIB.  If not,
write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

#include "pex-common.h"
#include "pex-read.h"

#include <errno.h>
#ifdef NEED_DECLARATION_ERRNO
extern int errno;
#endif
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

FILE *
pex_read (program, argv, this_pname, temp_base, errmsg_fmt, errmsg_arg, flags,
	  pidptr)
     const char *program;
     char * const *argv;
     const char *this_pname ATTRIBUTE_UNUSED;
     const char *temp_base ATTRIBUTE_UNUSED;
     char **errmsg_fmt;
     char **errmsg_arg;
     int flags ATTRIBUTE_UNUSED;
     int *pidptr ATTRIBUTE_UNUSED;
{
  char *cmd;
  int i;
  FILE *ret;

  cmd = xstrdup (program);
  for (i = 0; argv[i] != NULL; ++i)
    cmd = reconcat (cmd, cmd, " ", argv[i], NULL);

  ret = popen (cmd, "r");
  if (ret == NULL)
    {
      *errmsg_fmt = "popen";
      *errmsg_arg = NULL;
    }

  free (cmd);

  return ret;
}

int
pex_read_close (pid, file, status, flags)
     int pid ATTRIBUTE_UNUSED;
     FILE *file;
     int *status;
     int flags ATTRIBUTE_UNUSED;
{
  *status = pclose (file);
  return 0;
}
