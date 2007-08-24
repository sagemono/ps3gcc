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

#ifndef LTO_ASSERT_H
#define LTO_ASSERT_H

#include <assert.h>

#undef assert
#define assert(b) \
	((void) ((b) || (lto_failure( #b , __FILE__, __LINE__), 0)))

void lto_failure( const char *s, const char *file, unsigned line );

#endif /* LTO_ASSERT_H */
