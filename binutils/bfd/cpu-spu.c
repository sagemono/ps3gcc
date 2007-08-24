
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

#include "bfd.h"
#include "sysdep.h"
#include "libbfd.h"


static const bfd_arch_info_type *spu_compatible
  PARAMS ((const bfd_arch_info_type *, const bfd_arch_info_type *));

static const bfd_arch_info_type *
spu_compatible (a,b)
     const bfd_arch_info_type *a;
     const bfd_arch_info_type *b;
{
  BFD_ASSERT (a->arch == bfd_arch_spu);
  switch (b->arch)
    {
    default:
      return NULL;
    case bfd_arch_spu:
      return bfd_default_compatible (a, b);
    }
  /*NOTREACHED*/
}

const bfd_arch_info_type bfd_spu_arch[] =
{
  {
    32,	/* 32 bits in a word */
    32,	/* 32 bits in an address */
    8,	/* 8 bits in a byte */
    bfd_arch_spu,	/* architecture */
    bfd_mach_spu,	/* machine */
    "spu",		/* architecture name */
    "spu:256K",		/* printable name */
    3,			/* aligned power */
    TRUE, 		/* the default machine for the architecture */
    spu_compatible,	/* the spu is only compatible with itself, see above */
    bfd_default_scan,
    0,			/* next -- there are none! */
  }
};
