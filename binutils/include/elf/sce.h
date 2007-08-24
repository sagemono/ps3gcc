/* SCE specal segment definitions for BFD .
   Copyright 2006 Free Software Foundation, Inc.

   This file is part of BFD, the Binary File Descriptor library.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

#ifndef _ELF_SCE_H
#define _ELF_SCE_H

/* Segment attributes  */

#define PF_SPU_X  (0x00100000) /* SPU executable defined, but unused.*/
#define PF_SPU_W  (0x00200000) /* SPU writable  */
#define PF_SPU_R  (0x00400000) /* SPU readable  */
#define PF_RSX_X  (0x01000000) /* RSX executable defined, but unused. */
#define PF_RSX_W  (0x02000000) /* RSX writable */
#define PF_RSX_R  (0x04000000) /* RSX readable */

#endif /* _ELF_SCE_H */
