/* Global parts of the pgo spec.
   Used by gcc as well as spusim.
   Copyright (C) 2006 Sony Computer Entertainment, Inc.,

   PGO is free software; you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free
   Software Foundation; either version 2, or (at your option) any later
   version.

   PGO is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
   for more details.

   You should have received a copy of the GNU General Public License
   along with PGO; see the file COPYING.  If not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA.  */


#ifndef PGO_PGO_H
#define PGO_PGO_H

/* Version number of the contents of the .pgo_info section.  */
#define PGO_FILE_VERSION_MAJOR 0
#define PGO_FILE_VERSION_MINOR 0

/* ??? Should probably move pgo_rec_e contents to a .def file, but
   the contents aren't as complex as modes.  */

enum pgo_rec_e
{
    /* Make "unknown" have value zero.  There's typically lots of zeros in
       the files and we want to catch mismatches asap. */
    PGO_REC_UNKNOWN = 0,

    /* counter records */
    PGO_REC_COUNTER_MIN,
    PGO_REC_EDGE = PGO_REC_COUNTER_MIN,
    PGO_REC_INTERVAL,
    PGO_REC_POW2,
    PGO_REC_SINGLE,
    PGO_REC_DELTA,
    PGO_REC_COUNTER_MAX,

    /* not a record, delimits start of non-counter records */
    PGO_REC_NON_COUNTER_BEGIN = 64,
    PGO_REC_BEGIN_FILE = PGO_REC_NON_COUNTER_BEGIN,
    /* begin a collection of counter records */
    PGO_REC_BEGIN_COLLECTION,
    /* end a collection of counter records */
    PGO_REC_END_COLLECTION,
    PGO_REC_GCOV_INFO,
    PGO_REC_GCDA_FILE,
    PGO_REC_FN_INFO,
    PGO_REC_NON_COUNTER_MAX
};

#ifdef IN_PGO /* don't use these directly */

#define PGO_COUNTER_REC_NAMES \
    "PGO_REC_EDGE", \
    "PGO_REC_INTERVAL", \
    "PGO_REC_POW2", \
    "PGO_REC_SINGLE", \
    "PGO_REC_DELTA"

#define PGO_NON_COUNTER_REC_NAMES \
    "PGO_REC_BEGIN_FILE", \
    "PGO_REC_BEGIN_COLLECTION", \
    "PGO_REC_END_COLLECTION", \
    "PGO_REC_GCOV_INFO", \
    "PGO_REC_GCDA_FILE", \
    "PGO_REC_FN_INFO"

#endif

#define PGO_MODE(SYMBOL, NAME, KIND, BITS) SYMBOL,

enum pgo_mode_e
{
#include "pgo/pgo-modes.def"
    PGO_MODE_MAX
};

enum pgo_mode_kind_e
{
    PGO_MKIND_OTHER = 0, /* e.g. VOID */
    PGO_MKIND_INT,
    PGO_MKIND_MAX
};

#endif /* PGO_PGO_H */
