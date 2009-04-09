/* This is the equivalent of rtl-profile.c, rewritten to generate the output
   needed by the PGO project.  */
/* Copyright (C) 2006 Sony Computer Entertainment, Inc.,

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


#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "rtl.h"
#include "flags.h"
#include "output.h"
#include "regs.h"
#include "expr.h"
#include "function.h"
#include "c-common.h"
#include "toplev.h"
#include "coverage.h"
#include "tree.h"
#include "tree-flow.h"
#include "tree-dump.h"
#include "tree-pass.h"
#include "value-prof.h"
#include "ggc.h"
#include "tm_p.h" /* for DBX_REGISTER_NUMBER */

#include "pgo/pgo.h"

static GTY(()) tree pgo_gcov_type_node;

/* Emit a PGO insn with all 3 additional arguments.
   TAG is one of ???.
   BASE is the "base counter number" of the tracked value.  The tracking of
   some values requires multiple counters.
   VALUE_OR_0 is the value to track or zero.
   ARG[123] are extra arguments, dependent on the item being tracked.  */

static tree
build_pgo_insn (int tag, int base, tree value_or_0,
		int arg1, int arg2, int arg3)
{
  tree tree_tag = build_int_cst_type (integer_type_node, tag);
  tree tree_base = build_int_cst_type (integer_type_node, base);
  tree tree_arg1 = build_int_cst_type (integer_type_node, arg1);
  tree tree_arg2 = build_int_cst_type (integer_type_node, arg2);
  tree tree_arg3 = build_int_cst_type (integer_type_node, arg3);
  tree pgo_info = build_stmt (PGO_INFO_EXPR, tree_tag, tree_base, value_or_0,
			      tree_arg1, tree_arg2, tree_arg3);
  return pgo_info;
}

/* Build a PGO insn with 0 additional arguments.
   TAG is one of ???.
   BASE is the "base counter number" of the tracked value.  The tracking of
   some values requires multiple counters.
   VALUE_OR_0 the value to track or zero.  */

static tree
build_pgo0_insn (int tag, int base, tree value_or_0)
{
  return build_pgo_insn (tag, base, value_or_0, 0, 0, 0);
}

/* Do initialization work for the edge profiler.  */

static void
pgo_init_edge_profiler (void)
{
  if (!pgo_gcov_type_node)
    {
      pgo_gcov_type_node = get_gcov_type ();
      switch_to_section (get_section (".pgo_info", SECTION_DEBUG | SECTION_NOTYPE, NULL));
    }
}

/* Output instructions as RTL to increment the edge execution count.  */

static void
pgo_gen_edge_profiler (int edgeno, edge e)
{
  int counterno = pgo_coverage_counter_ref (GCOV_COUNTER_ARCS, edgeno);

  tree pgo_info = build_pgo0_insn (GCOV_COUNTER_ARCS, counterno,
				   build_int_cst (integer_type_node, 0));

  bsi_insert_on_edge (e, pgo_info);
}

/* Emits code to get VALUE to instrument at BSI, and returns the
   variable containing the value.  */

static tree
prepare_instrumented_value (block_stmt_iterator *bsi,
			    histogram_value value)
{
  tree val = value->hvalue.value;
  return force_gimple_operand_bsi (bsi, fold_convert (pgo_gcov_type_node, val),
				   true, NULL_TREE);
}

static void
pgo_gen_value_profiler (histogram_value value, unsigned tag, unsigned base)
{
  int counterno = pgo_coverage_counter_ref (tag, base);
  tree stmt = value->hvalue.stmt;
  block_stmt_iterator bsi = bsi_for_stmt (stmt);
  tree val = prepare_instrumented_value (&bsi, value);
  tree pgo_info;

  switch (value->type)
    {
      case HIST_TYPE_INTERVAL:
      {
	/*t = GCOV_COUNTER_V_INTERVAL; */
	pgo_info = build_pgo_insn (tag, counterno, val,
				   value->hdata.intvl.int_start,
				   value->hdata.intvl.steps,
				   0 /* old flags field */);
	break;
      }
      case HIST_TYPE_POW2:
      {
	/*t = GCOV_COUNTER_V_POW2; */
	pgo_info = build_pgo0_insn (tag, counterno, val);
	break;
      }
      case HIST_TYPE_SINGLE_VALUE:
      {
	/*t = GCOV_COUNTER_V_SINGLE; */
	pgo_info = build_pgo0_insn (tag, counterno, val);
	break;
      }
      case HIST_TYPE_CONST_DELTA:
      {
	/*t = GCOV_COUNTER_V_DELTA; */
	pgo_info = build_pgo0_insn (tag, counterno, val);
	break;
      }
      default:
	abort ();
    }

  bsi_insert_before (&bsi, pgo_info, BSI_SAME_STMT);
}

/* Output instructions as RTL to increment the interval histogram counter.
   VALUE is the expression whose value is profiled.  TAG is the tag of the
   section for counters, BASE is offset of the counter position.  */

static void
pgo_gen_interval_profiler (histogram_value value, unsigned tag, unsigned base)
{
  pgo_gen_value_profiler (value, tag, base);
}

/* Output instructions as RTL to increment the power of two histogram counter.
   VALUE is the expression whose value is profiled.  TAG is the tag of the
   section for counters, BASE is offset of the counter position.  */

static void
pgo_gen_pow2_profiler (histogram_value value, unsigned tag, unsigned base)
{
  pgo_gen_value_profiler (value, tag, base);
}

/* Output instructions as RTL for code to find the most common value.
   VALUE is the expression whose value is profiled.  TAG is the tag of the
   section for counters, BASE is offset of the counter position.  */

static void
pgo_gen_one_value_profiler (histogram_value value, unsigned tag, unsigned base)
{
  pgo_gen_value_profiler (value, tag, base);
}

/* Output instructions as RTL for code to find the most common value of
   a difference between two evaluations of an expression.
   VALUE is the expression whose value is profiled.  TAG is the tag of the
   section for counters, BASE is offset of the counter position.  */

static void
pgo_gen_const_delta_profiler (histogram_value value, unsigned tag, unsigned base)
{
  pgo_gen_value_profiler (value, tag, base);
}

/* Return the file on which profile dump output goes, if any.  */

static FILE *
pgo_profile_dump_file (void)
{
  return dump_file;
}

struct profile_hooks pgo_profile_hooks =
{
  pgo_init_edge_profiler,
  pgo_gen_edge_profiler,
  pgo_gen_interval_profiler,
  pgo_gen_pow2_profiler,
  pgo_gen_one_value_profiler,
  pgo_gen_const_delta_profiler,
  pgo_profile_dump_file
};

/* Generate RTL code for a `pgo_info_expr' statement.  */

void
expand_pgo_info_expr (tree exp)
{
#if defined (HAVE_pgo_info_arc) || defined (HAVE_pgo_info_value) /* FIXME: wip */
  unsigned tag = TREE_INT_CST_LOW (TREE_OPERAND (exp, 0));
  rtx rtx_tag = gen_rtx_CONST_INT (VOIDmode, tag);
  rtx rtx_base = gen_rtx_CONST_INT (VOIDmode, TREE_INT_CST_LOW (TREE_OPERAND (exp, 1)));
  rtx tmp;

  if (tag == GCOV_COUNTER_ARCS)
    {
	rtx arg1 = gen_rtx_CONST_INT (VOIDmode, TREE_INT_CST_LOW (TREE_OPERAND (exp, 2)));
	tmp = gen_pgo_info_arc (rtx_tag, rtx_base, arg1);
    }
  else
    {
	rtx val = expand_expr (TREE_OPERAND (exp, 2), NULL_RTX, VOIDmode, 0);
	rtx arg1 = gen_rtx_CONST_INT (VOIDmode, TREE_INT_CST_LOW (TREE_OPERAND (exp, 3)));
	rtx arg2 = gen_rtx_CONST_INT (VOIDmode, TREE_INT_CST_LOW (TREE_OPERAND (exp, 4)));
	rtx arg3 = gen_rtx_CONST_INT (VOIDmode, TREE_INT_CST_LOW (TREE_OPERAND (exp, 5)));
	val = force_reg (TYPE_MODE (pgo_gcov_type_node), val);
	tmp = gen_pgo_info_value (rtx_tag, rtx_base, val, arg1, arg2, arg3);
    }

  emit_insn (tmp);
#endif
}

/* Utilities for generating .pgo_info.  */

unsigned /*pgo_rec_e*/
get_pgo_rec_nr (unsigned gcov_counter_kind)
{
  switch (gcov_counter_kind)
    {
    case GCOV_COUNTER_ARCS: return PGO_REC_EDGE;
    case GCOV_COUNTER_V_INTERVAL: return PGO_REC_INTERVAL;
    case GCOV_COUNTER_V_POW2: return PGO_REC_POW2;
    case GCOV_COUNTER_V_SINGLE: return PGO_REC_SINGLE;
    case GCOV_COUNTER_V_DELTA: return PGO_REC_DELTA;
    default: gcc_assert (0);
    }
}

unsigned /*pgo_mode_e*/
get_pgo_mode (enum machine_mode mode)
{
#if 0 /* ??? wip */
  static const char* const pgo_mode_names[] = { PGO_MODE_NAMES };
  static const enum pgo_mode_e pgo_modes[] = { PGO_MODES };
  const char* name = GET_MODE_NAME (mode);
  unsigned i;
  unsigned nr_pgo_modes = sizeof (pgo_mode_names) / sizeof (pgo_mode_names[0]);
#endif

  switch (mode)
    {
    case QImode: return PGO_MODE_QI;
    case HImode: return PGO_MODE_HI;
    case SImode: return PGO_MODE_SI;
    case DImode: return PGO_MODE_DI;
#if 0 /* fp not supported yet */
    case SFmode: return PGO_MODE_SF;
    case DFmode: return PGO_MODE_DF;
#endif
    default: break;
    }

#if 0 /* ??? wip */
  /* Not a common mode. */

  for (i = 0; i < nr_pgo_modes; ++i)
    {
      if (strcmp (name, pgo_mode_names[i]) == 0)
	return pgo_modes[i];
    }
#endif

  return PGO_MODE_VOID;
}

/* Convert an internal gcc register number to its external version.
   We use the target's DWARF register numbering scheme so we don't have to
   invent another.  */

unsigned
get_pgo_regno (unsigned regno)
{
  gcc_assert (regno < FIRST_PSEUDO_REGISTER);

  return DBX_REGISTER_NUMBER (regno);
}

#include "gt-pgo-profile.h"
