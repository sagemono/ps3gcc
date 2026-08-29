/* spu-dis.c -- Disassemble Spu instructions */

/* (C) Copyright
   Sony Computer Entertainment, Inc.,
   Toshiba Corporation,
   International Business Machines Corporation,
   2001,2002,2003,2004,2005,2010

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

/* -Mmfc-cmd option patch is contributed by luke@gmail.com,
   Original patches are
      http://sourceware.org/ml/binutils/2010-12/msg00192.html
      http://sourceware.org/ml/binutils/2010-12/msg00195.html */
 
#include <stdio.h>
#include "sysdep.h"
#include "dis-asm.h"
#include "opintl.h"
#include "opcode/spu.h"

/* This file provides a disassembler function which uses
   the disassembler interface defined in dis-asm.h.   */

static void init_spu_disassemble(void);

extern const struct spu_opcode spu_opcodes[];
extern const int spu_num_opcodes;

static const struct spu_opcode *spu_disassemble_table[(1<<11)];

void
print_spu_disassembler_options (FILE *stream)
{
  fprintf (stream,
           _("\n"
             "The following spu specific disassembler options are supported for use\n"
             "with the -M switch (multiple options should be separated by commas):\n"));
  fprintf (stream, _("  mfc-cmd     Comment immediate values that match common mfc commands\n"));
  fprintf (stream, _("  mfc-cmd-all Comment immediate values that match any mfc command\n"));
}

struct spu_options
{
  unsigned int mfc_cmd_comment_level;
};

static void
parse_spu_options (struct spu_options *options, char *string)
{
  char *arg;

  options->mfc_cmd_comment_level = 0;

  arg = string;
  while (arg != NULL)
    {
      char *end = strchr (arg, ',');
      if (end != NULL)
        *end = '\0';

      if (strcmp (arg, "mfc-cmd") == 0)
        options->mfc_cmd_comment_level = 1;
      else if (strcmp (arg, "mfc-cmd-all") == 0)
        options->mfc_cmd_comment_level = 2;
      else
        fprintf (stderr, _("warning: ignoring unknown -M%s option\n"), arg);

      if (end != NULL)
        *end++ = ',';
      arg = end;
    }
}

static void
init_spu_disassemble()
{
    int i;
    /* If two instructions have the same opcode then we prefer the first
     * one.  In most cases it is just an alternate mnemonic. */
    for (i = 0; i < spu_num_opcodes; i++)
    {
        int o = spu_opcodes[i].opcode;
        if (o >= (1 << 11))
            abort();
        if (spu_disassemble_table[o] == 0)
            spu_disassemble_table[o] = &spu_opcodes[i];
    }
}

/* Determine the instruction from the 10 least significant bits. */
static const struct spu_opcode *
get_index_for_opcode(unsigned int insn)
{
   const struct spu_opcode *index;
   unsigned int opcode = insn >> (32-11);

   /* Init the table.  This assumes that element 0/opcode 0 (currently
    * NOP) is always used */
   if (spu_disassemble_table[0] == 0)
       init_spu_disassemble();

   if ((index = spu_disassemble_table[opcode & 0x780]) != 0
       && index->insn_type == RRR)
       return index;

   if ((index = spu_disassemble_table[opcode & 0x7f0]) != 0
   &&  (index->insn_type == RI18 || index->insn_type == LBT))
           return index;

   if ((index = spu_disassemble_table[opcode & 0x7f8]) != 0
       && index->insn_type == RI10)
       return index;

   if ((index = spu_disassemble_table[opcode & 0x7fc]) != 0
       && (index->insn_type == RI16))
       return index;

   if ((index = spu_disassemble_table[opcode & 0x7fe]) != 0
       && (index->insn_type == RI8))
       return index;

   if ((index = spu_disassemble_table[opcode & 0x7ff]) != 0)
   {
       return index;
   }

   return 0;
}

/* Convert immediate value to displayable string to be appended to insn comment. */
static const char *
get_comment_for_immediate (int immed, const struct spu_options *options)
{
  struct value_string_pair
  {
    int val;
    const char *str;
  };
  static const struct value_string_pair mfc_common[] =
  {
    {0x0020, " (put)"},
    {0x0021, " (putb)"},
    {0x0022, " (putf)"},
    {0x0024, " (putl)"},
    {0x0025, " (putlb)"},
    {0x0026, " (putlf)"},
    {0x0040, " (get)"},
    {0x0041, " (getb)"},
    {0x0042, " (getf)"},
    {0x0044, " (getl)"},
    {0x0045, " (getlb)"},
    {0x0046, " (getlf)"},
    {0x00b0, " (putlluc)"},
    {0x00b4, " (putllc)"},
    {0x00b8, " (putqlluc)"},
    {0x00d0, " (getllar)"},
  };
  static const struct value_string_pair mfc_others[] =
  {
    {0x0028, " (puts)"},
    {0x0029, " (putbs)"},
    {0x002a, " (putfs)"},
    {0x0030, " (putr)"},
    {0x0031, " (putrb)"},
    {0x0032, " (putrf)"},
    {0x0034, " (putrl)"},
    {0x0035, " (putrlb)"},
    {0x0036, " (putrlf)"},
    {0x0048, " (gets)"},
    {0x0049, " (getbs)"},
    {0x004a, " (getfs)"},
    {0x0080, " (sdcrt)"},
    {0x0081, " (sdcrtst)"},
    {0x0089, " (sdcrz)"},
    {0x008d, " (sdcrst)"},
    {0x008f, " (sdcrf)"},
    {0x00a0, " (sndsig)"},
    {0x00a1, " (sndsigb)"},
    {0x00a2, " (sndsigf)"},
    {0x00c0, " (barrier)"},
    {0x00c8, " (mfceieio)"},
    {0x00cc, " (mfcsync)"},
  };
  unsigned i;
  if (options->mfc_cmd_comment_level > 0)
    {
      for (i=0; i<sizeof(mfc_common)/sizeof(*mfc_common); ++i)
      {
        if (mfc_common[i].val == immed)
          return mfc_common[i].str;
      }
    }
  if (options->mfc_cmd_comment_level > 1)
    {
      for (i=0; i<sizeof(mfc_others)/sizeof(*mfc_others); ++i)
      {
        if (mfc_others[i].val == immed)
          return mfc_others[i].str;
      }
    }
  return "";
}

/* Print a Spu instruction.  */

int
print_insn_spu (memaddr, info)
     bfd_vma memaddr;
     struct disassemble_info *info;
{
  bfd_byte buffer[4];
  int value;
  int hex_value;
  int status;
  unsigned int insn;
  const struct spu_opcode *index;
  enum spu_insns tag;
  struct spu_options options;

  parse_spu_options (&options, info->disassembler_options);

  status = (*info->read_memory_func) (memaddr, buffer, 4, info);
  if (status != 0)
    {
      (*info->memory_error_func) (status, memaddr, info);
      return -1;
    }

  insn = bfd_getb32 (buffer);

  index = get_index_for_opcode(insn);

  if (index == 0)
    {
      (*info->fprintf_func) (info->stream, ".long 0x%x", insn);
    }
  else
    {
      int i;
      int paren = 0;
      tag = (enum spu_insns)(index - spu_opcodes);
      (*info->fprintf_func) (info->stream, "%s", index->mnemonic);
      if (tag == M_BI || tag == M_BISL || tag == M_IRET || tag == M_BISLED
	  || tag == M_BIHNZ || tag == M_BIHZ || tag == M_BINZ || tag == M_BIZ
          || tag == M_SYNC || tag == M_HBR)
	{
	  int fb = (insn >> (32-18)) & 0x7f;
	  if (fb & 0x40)
	    (*info->fprintf_func) (info->stream, tag == M_SYNC ? "c" : "p");
	  if (fb & 0x20)
	    (*info->fprintf_func) (info->stream, "d");
	  if (fb & 0x10)
	    (*info->fprintf_func) (info->stream, "e");
	}
      if (index->arg[0] != 0)
	(*info->fprintf_func) (info->stream, "\t");
      hex_value = 0;
      for (i = 1;  i <= index->arg[0]; i++)
	{
	  int arg = index->arg[i];
	  if (arg != A_P && !paren && i > 1)
	    (*info->fprintf_func) (info->stream, ",");

	  switch (arg)
	    {
	    case A_T:
	      (*info->fprintf_func) (info->stream, "$%d", DECODE_INSN_RT(insn));
	      break;
	    case A_A:
	      (*info->fprintf_func) (info->stream, "$%d", DECODE_INSN_RA(insn));
	      break;
	    case A_B:
	      (*info->fprintf_func) (info->stream, "$%d", DECODE_INSN_RB(insn));
	      break;
	    case A_C:
	      (*info->fprintf_func) (info->stream, "$%d", DECODE_INSN_RC(insn));
	      break;
	    case A_S:
	      (*info->fprintf_func) (info->stream, "$sp%d", DECODE_INSN_RA(insn));
	      break;
	    case A_H:
	      (*info->fprintf_func) (info->stream, "$ch%d", DECODE_INSN_RA(insn));
	      break;
	    case A_P:
	      paren++;
	      (*info->fprintf_func) (info->stream, "(");
	      break;
	    case A_U7A:
	      (*info->fprintf_func) (info->stream, "%d", 173 - DECODE_INSN_U8(insn));
	      break;
	    case A_U7B:
	      (*info->fprintf_func) (info->stream, "%d", 155 - DECODE_INSN_U8(insn));
	      break;
	    case A_S3:
	    case A_S6:
	    case A_S7:
	    case A_S7N:
	    case A_U3:
	    case A_U5:
	    case A_U6:
	    case A_U7:
	      hex_value = DECODE_INSN_I7(insn);
	      (*info->fprintf_func) (info->stream, "%d", hex_value);
	      break;
	    case A_S11:
	      (*info->print_address_func) (memaddr + DECODE_INSN_I9a(insn) * 4, info);
	      break;
	    case A_S11I:
	      (*info->print_address_func) (memaddr + DECODE_INSN_I9b(insn) * 4, info);
	      break;
	    case A_S10:
	    case A_S10B:
	      hex_value = DECODE_INSN_I10(insn);
	      (*info->fprintf_func) (info->stream, "%d", hex_value);
	      break;
	    case A_S14:
	      hex_value = DECODE_INSN_I10(insn) * 16;
	      (*info->fprintf_func) (info->stream, "%d", hex_value);
	      break;
	    case A_S16:
	      hex_value = DECODE_INSN_I16(insn);
	      (*info->fprintf_func) (info->stream, "%d", hex_value);
	      break;
	    case A_X16:
	      hex_value = DECODE_INSN_U16(insn);
	      (*info->fprintf_func) (info->stream, "%u", hex_value);
	      break;
	    case A_R18:
	      value = DECODE_INSN_I16(insn) * 4;
	      if (value == 0)
		(*info->fprintf_func) (info->stream, "%d", value);
	      else
		{
		  hex_value = (int)(memaddr + value);
		  (*info->print_address_func) (hex_value & 0x3ffff, info);
		}
	      break;
	    case A_S18:
	      value = DECODE_INSN_U16(insn) * 4;
	      if (value == 0)
		(*info->fprintf_func) (info->stream, "%d", value);
	      else
		(*info->print_address_func) (value, info);
	      break;
	    case A_U18:
	      value = DECODE_INSN_U18(insn);
	      if (value == 0 || !(*info->symbol_at_address_func)(0, info))
		{
		  hex_value = value;
		  (*info->fprintf_func) (info->stream, "%u", value);
		}
	      else
		(*info->print_address_func) (value, info);
	      break;
	    case A_U14:
	      hex_value = DECODE_INSN_U14(insn);
	      (*info->fprintf_func) (info->stream, "%u", hex_value);
	      break;
	    }
	  if (arg != A_P && paren)
	    {
	      (*info->fprintf_func) (info->stream, ")");
	      paren--;
	    }
	}
      if (hex_value != 0)
        {
          const char * comment = get_comment_for_immediate (hex_value, &options);
          if (hex_value > 16)
            {
              (*info->fprintf_func) (info->stream, "\t# %x%s", hex_value, comment);
            }
          else if (comment[0] != '\0')
            {
              (*info->fprintf_func) (info->stream, "\t# %s", comment);
            }
        }
    }
  return 4;
}
