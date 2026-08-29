/* Execute a program and read stdout.  Generic Unix version.
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

#include <stdio.h>
#include <errno.h>
#ifdef NEED_DECLARATION_ERRNO
extern int errno;
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#if HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_SYS_WAIT_H
#include <sys/wait.h>
#endif

#ifndef HAVE_WAITPID
#define waitpid(pid, status, flags) wait(status)
#endif

#ifdef vfork /* Autoconf may define this to fork for us. */
# define VFORK_STRING "fork"
#else
# define VFORK_STRING "vfork"
#endif
#ifdef HAVE_VFORK_H
#include <vfork.h>
#endif
#ifdef VMS
#define vfork() (decc$$alloc_vfork_blocks() >= 0 ? \
               lib$get_current_invo_context(decc$$get_vfork_jmpbuf()) : -1)
#endif /* VMS */

/* A safe version of popen.  See pex-unix.c for notes on vfork.  */

FILE *
pex_read (program, argv, this_pname, temp_base, errmsg_fmt, errmsg_arg,
	  flagsarg, pidptr)
     const char *program;
     char * const *argv;
     const char *this_pname;
     const char *temp_base ATTRIBUTE_UNUSED;
     char **errmsg_fmt;
     char **errmsg_arg;
     int flagsarg;
     int *pidptr;
{
  int flags;
  int pid;
  int pdes[2];
  int in;
  int out;
  FILE *ret;
  /* We declare these to be volatile to avoid warnings from gcc about
     them being clobbered by vfork.  */
  volatile int retries;
  volatile int sleep_interval;

  flags = flagsarg;

  if (pipe (pdes) < 0)
    {
      *errmsg_fmt = "pipe";
      *errmsg_arg = NULL;
      return NULL;
    }

  in = pdes[READ_PORT];
  out = pdes[WRITE_PORT];

  sleep_interval = 1;
  pid = -1;
  for (retries = 0; retries < 4; retries++)
    {
      pid = vfork ();
      if (pid >= 0)
	break;
      sleep (sleep_interval);
      sleep_interval *= 2;
    }

  switch (pid)
    {
    case -1:
      *errmsg_fmt = "fork";
      *errmsg_arg = NULL;
      return NULL;

    case 0:
      if (out != STDOUT_FILE_NO)
	{
	  close (STDOUT_FILE_NO);
	  dup (out);
	  close (out);
	}
      close (in);

      if (flags & PEXECUTE_SEARCH)
	execvp (program, argv);
      else
	execv (program, argv);

      /* We don't want to call fprintf after vfork.  */
#define writeerr(s) write (STDERR_FILE_NO, s, strlen (s))
      writeerr (this_pname);
      writeerr (": ");
      writeerr ("installation problem, cannot exec '");
      writeerr (program);
      writeerr ("': ");
      writeerr (xstrerror (errno));
      writeerr ("\n");
      _exit (-1);
      /* NOTREACHED */
      return NULL;

    default:
      close (out);
      ret = fdopen (in, "r");
      if (ret == NULL)
	{
	  *errmsg_fmt = "fdopen";
	  *errmsg_arg = NULL;
	  return NULL;
	}
      *pidptr = pid;
      return ret;
    }
  /* NOTREACHED */
}

int
pex_read_close (pid, file, status, flags)
     int pid;
     FILE *file;
     int *status;
     int flags ATTRIBUTE_UNUSED;
{
  /* ??? Canonicalize STATUS value?  */
  fclose (file);
  return waitpid (pid, status, 0);
}
