/* Threads compatibility routines for libgcc2.  */
/* Compile this one with gcc.  */
/* Copyright (C) 1997, 1999, 2000, 2001, 2002, 2003
   Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 2, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.  */

/* As a special exception, if you link this library with other files,
   some of which are compiled with GCC, to produce an executable,
   this library does not by itself cause the resulting executable
   to be covered by the GNU General Public License.
   This exception does not however invalidate any other reasons why
   the executable file might be covered by the GNU General Public License.  */

#ifndef GCC_GTHR_DINKUMWARE_H
#define GCC_GTHR_DINKUMWARE_H

#define __GTHREADS 1
/* Use the implementation provided by dinkumware. */
#include <sys/synchronization.h>

#ifdef __cplusplus
extern "C" {
#endif

#define __GTHREAD_ONCE_INIT 0
#define __GTHREAD_MUTEX_INIT_FUNCTION(mtx) __gthr_lv2_mutex_init_function(mtx)

typedef int __gthread_key_t;
typedef long __gthread_once_t;
typedef sys_lwmutex_t __gthread_mutex_t;
typedef sys_lwmutex_t __gthread_recursive_mutex_t;
typedef sys_lwmutex_attribute_t __gthread_mutexattr_t;

extern int __gthr_lv2_once(__gthread_once_t *,void (*) (void));
extern int __gthr_lv2_key_create(__gthread_key_t *, void (*) (void *));
extern int __gthr_lv2_key_delete(__gthread_key_t);
extern void *__gthr_lv2_getspecific(__gthread_key_t);
extern int __gthr_lv2_setspecific(__gthread_key_t, const void *);
extern void __gthr_lv2_mutex_init_function(__gthread_mutex_t *);
extern int __gthr_lv2_mutex_destroy(__gthread_mutex_t *);
extern int __gthr_lv2_mutex_lock(__gthread_mutex_t *);
extern int __gthr_lv2_mutex_unlock(__gthread_mutex_t *);

static inline int
__gthread_active_p ()
{
  return 1;
}

static inline int
__gthread_once (__gthread_once_t *once, void (*func) (void))
{
  return __gthr_lv2_once (once, func);
}

static inline int
__gthread_key_create (__gthread_key_t *key, void (*dtor) (void *))
{
  return __gthr_lv2_key_create (key, dtor);
}

static inline int
__gthread_key_delete (__gthread_key_t key)
{
  return __gthr_lv2_key_delete (key);
}

static inline void *
__gthread_getspecific (__gthread_key_t key)
{
  return __gthr_lv2_getspecific (key);
}

static inline int
__gthread_setspecific (__gthread_key_t key, const void *ptr)
{
  return __gthr_lv2_setspecific (key, (void *)ptr);
}

static inline int
__gthread_mutex_destroy (__gthread_mutex_t *mutex)
{
  return __gthr_lv2_mutex_destroy (mutex);
}

static inline int
__gthread_mutex_lock (__gthread_mutex_t *mutex)
{
  return __gthr_lv2_mutex_lock (mutex);
}

static inline int
__gthread_mutex_unlock (__gthread_mutex_t *mutex)
{
  return __gthr_lv2_mutex_unlock (mutex);
}

static inline int
__gthread_mutex_trylock (__gthread_mutex_t *mutex)
{
  return -1;
}

static inline int
__gthread_recursive_mutex_lock (__gthread_recursive_mutex_t *mutex)
{
  return -1;
}

static inline int
__gthread_recursive_mutex_trylock (__gthread_recursive_mutex_t *mutex)
{
  return -1;
}

static inline int
__gthread_recursive_mutex_unlock (__gthread_recursive_mutex_t *mutex)
{
  return -1;
}

#ifdef __cplusplus
}
#endif

#endif /* ! GCC_GTHR_DINKUMWARE_H */

