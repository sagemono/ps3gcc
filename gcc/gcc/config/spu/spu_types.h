
/* Definitions of IBM Synergistic Processing Unit (SPU). */

/* --------------------------------------------------------------  */
/* (C)Copyright                                                    */
/* Sony Computer Entertainment, Inc.,                              */
/* Toshiba Corporation,                                            */
/* International Business Machines Corporation,                    */
/* 2001,2002,2003.                                                 */
/* --------------------------------------------------------------  */

/* As a special exception, if you include this header file into source
files compiled by GCC, this header file does not by itself cause the 
resulting executable to be covered by the GNU General Public License. 
This exception does not however invalidate any other reasons why the 
executable file might be covered by the GNU General Public License. */

#ifndef  _SPU_TYPES_H
#define _SPU_TYPES_H 

#include "tree.h"

/*  The parameters of a builtin are described using a zero terminated
 *  array of values from enum tree_index (defined in tree.h) and enum
 *  spu_tree_index (defined below).  This array is also used when for
 *  parameters that need special handling when expanding the builtin to
 *  rtl.  
 * 
 *  Special handling is done based on spu_builtin_type too.
 * */
enum spu_tree_index {
    P_BLAB = TI_MAX,
    /* Everything after this point is assumed to be of type int.
     * These parameters only accept immediate values of a specific
     * range. */
    SPU_TI_7,
    SPU_TI_S7,
    SPU_TI_U7,
    SPU_TI_S10,
    SPU_TI_S10_4,
    SPU_TI_U14,
    SPU_TI_16,
    SPU_TI_S16,
    SPU_TI_S16_2,
    SPU_TI_U16,
    SPU_TI_U16_2,
    SPU_TI_U18,

    /* integer range */
    SPU_TI_INT_FIRST = SPU_TI_7,
    SPU_TI_INT_LAST = SPU_TI_U18,

    SPU_TI_UV16QI_TYPE,
    SPU_TI_UV8HI_TYPE,
    SPU_TI_UV4SI_TYPE,
    SPU_TI_UV2DI_TYPE,

    SPU_TI_V16QI_TYPE,
    SPU_TI_V8HI_TYPE,
    SPU_TI_V4SI_TYPE,
    SPU_TI_V2DI_TYPE,
    SPU_TI_V4SF_TYPE,
    SPU_TI_V2DF_TYPE,

    /* vector range */
    SPU_TI_VEC_FIRST = SPU_TI_UV16QI_TYPE,
    SPU_TI_VEC_LAST = SPU_TI_V2DF_TYPE,

    /* builtin parm range */
    SPU_TI_FIRST = SPU_TI_INT_FIRST,
    SPU_TI_LAST = SPU_TI_VEC_LAST,

    SPU_TI_MAX
};

static inline bool
spu_tree_index_p(int p)
{
  return p >= SPU_TI_FIRST && p <= SPU_TI_LAST;
}

static inline bool
spu_tree_index_int_p(int p)
{
  return p >= SPU_TI_INT_FIRST && p <= SPU_TI_INT_LAST;
}

static inline bool
spu_tree_index_vector_p(int p)
{
  return p >= SPU_TI_VEC_FIRST && p <= SPU_TI_VEC_LAST;
}

/* type node accesser functions. */
extern tree spu_get_type_node_by_tree_index(int p);
extern void spu_set_type_node_by_tree_index(int p, tree t);

#endif /* _SPUTYPES_H */
