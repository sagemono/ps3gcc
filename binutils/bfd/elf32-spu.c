/* SPU specific support for 32-bit ELF */

/* (C) Copyright
   Sony Computer Entertainment, Inc.,
   Toshiba Corporation,
   International Business Machines Corporation,
   2001,2002,2003,2004,2005,2006.

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

#include "sysdep.h"
#include "bfd.h"
#include "bfdlink.h"
#include "libbfd.h"
#include "elf-bfd.h"
#include "elf/spu.h"
#include "elf32-spu.h"

static asection * spu_elf_gc_mark_hook		PARAMS ((asection *, struct bfd_link_info *,
							Elf_Internal_Rela *, struct elf_link_hash_entry *,
							Elf_Internal_Sym *));

/* We use RELA style relocs.  Don't define USE_REL.  */

static bfd_reloc_status_type spu_elf_rel9 (bfd *, arelent *, asymbol *,
					   void *, asection *,
					   bfd *, char **);

/* Values of type 'enum elf_spu_reloc_type' are used to index this
   array, so it must be declared in the order of that type.  */

static reloc_howto_type elf_howto_table[] = {
  HOWTO (R_SPU_NONE,       0, 0,  0, FALSE,  0, complain_overflow_dont,
	 bfd_elf_generic_reloc, "SPU_NONE",
	 FALSE, 0, 0x00000000, FALSE),

  /* lqd, stqd */
  HOWTO (R_SPU_ADDR10,     4, 2, 10, FALSE, 14, complain_overflow_signed,
	 bfd_elf_generic_reloc, "SPU_ADDR10",
	 FALSE, 0, 0x00ffc000, FALSE),

  /* bra, brasl, lqa, hbra, stqa */
  HOWTO (R_SPU_ADDR16,     2, 2, 16, FALSE,  7, complain_overflow_bitfield,
	 bfd_elf_generic_reloc, "SPU_ADDR16",
	 FALSE, 0, 0x007fff80, FALSE),

  /* ilhu */
  HOWTO (R_SPU_ADDR16_HI, 16, 2, 16, FALSE,  7, complain_overflow_bitfield,
	 bfd_elf_generic_reloc, "SPU_ADDR16_HI",
	 FALSE, 0, 0x007fff80, FALSE),

  /* iohl */
  HOWTO (R_SPU_ADDR16_LO,  0, 2, 16, FALSE,  7, complain_overflow_dont,
	 bfd_elf_generic_reloc, "SPU_ADDR16_LO",
	 FALSE, 0, 0x007fff80, FALSE),

  /* ila */
  HOWTO (R_SPU_ADDR18,     0, 2, 18, FALSE,  7, complain_overflow_bitfield,
	 bfd_elf_generic_reloc, "SPU_ADDR18",
	 FALSE, 0, 0x01ffff80, FALSE),

  /* 32-bit address in data */
  HOWTO (R_SPU_GLOB_DAT,     0, 2, 32, FALSE,  0, complain_overflow_dont,
	 bfd_elf_generic_reloc, "SPU_GLOB_DAT",
	 FALSE, 0, 0xffffffff, FALSE),

  /* br, brsl, lqr, hbrr, brz, brnz, brhz, brhnz, stqr */
  HOWTO (R_SPU_REL16,      2, 2, 16,  TRUE,  7, complain_overflow_bitfield,
	 bfd_elf_generic_reloc, "SPU_REL16",
	 FALSE, 0, 0x007fff80, TRUE),

  /* c[bhwd]d, rot*i, shl*i */
  HOWTO (R_SPU_ADDR7,      0, 2,  7, FALSE, 14, complain_overflow_dont,
	 bfd_elf_generic_reloc, "SPU_ADDR7",
	 FALSE, 0, 0x001fc000, FALSE),

  /* hbra, hbrr (location of branch instruction) */
  HOWTO (R_SPU_REL9,       2, 2,  9,  TRUE,  0, complain_overflow_signed,
	 spu_elf_rel9,          "SPU_REL9",
	 FALSE, 0, 0x0180007f, TRUE),

  /* hbr (location of branch instruction */
  HOWTO (R_SPU_REL9I,      2, 2,  9,  TRUE,  0, complain_overflow_signed,
	 spu_elf_rel9,          "SPU_REL9I",
	 FALSE, 0, 0x0000c07f, TRUE),

  /* and*i, or*i, xor*i, a*i, sf*i, cgt*i, clgt*i, ceq*i, hgt*i, hlgt*i,
   * heq*i, mpy*i */
  HOWTO (R_SPU_ADDR10I,    0, 2, 10, FALSE, 14, complain_overflow_signed,
	 bfd_elf_generic_reloc, "SPU_ADDR10I",
	 FALSE, 0, 0x00ffc000, FALSE),

  /* il, fsmbi, ilh, ilhu, iohl. (allows range of [-2^15..2^15-1]) */
  HOWTO (R_SPU_ADDR16I,    0, 2, 16, FALSE,  7, complain_overflow_signed,
	 bfd_elf_generic_reloc, "SPU_ADDR16I",
	 FALSE, 0, 0x007fff80, FALSE),

  /* 32-bit relative address, used in debug information */
  HOWTO (R_SPU_REL32,      0, 2, 32, TRUE,  0, complain_overflow_dont,
	 bfd_elf_generic_reloc, "SPU_REL32",
	 FALSE, 0, 0xffffffff, TRUE),

  /* Obsolete?  It seems that nothing generates this.  It is the same as
   * R_SPU_ADDR16I, except with a different complain_overflow_*. */
  HOWTO (R_SPU_ADDR16X,    0, 2, 16, FALSE,  7, complain_overflow_bitfield,
	 bfd_elf_generic_reloc, "SPU_ADDR16X",
	 FALSE, 0, 0x007fff80, FALSE),

  /* 32-bit address in main memory */
  HOWTO (R_SPU_PPU32,      0, 2, 32, FALSE,  0, complain_overflow_dont,
	 bfd_elf_generic_reloc, "SPU_PPU32",
	 FALSE, 0, 0xffffffff, FALSE),

  /* 64-bit address in main memory */
  HOWTO (R_SPU_PPU64,      0, 4, 64, FALSE,  0, complain_overflow_dont,
	 bfd_elf_generic_reloc, "SPU_PPU64",
	 FALSE, 0, -1, FALSE),

  /* Mark the instruction that adds the PIC offset */
  HOWTO (R_SPU_ADD_PIC,      0, 2, 32, FALSE,  0, complain_overflow_dont,
	 bfd_elf_generic_reloc, "SPU_ADD_PIC",
	 FALSE, 0, 0xffffc000, FALSE),

};

static struct bfd_elf_special_section const spu_elf_special_sections[]=
{
  { ".toe", 4, 0, SHT_PROGBITS, SHF_ALLOC },
  { ".SpuGUID", 8, 0, SHT_PROGBITS, SHF_ALLOC + SHF_EXECINSTR }, /* sce local bugzilla #2878 */
  { NULL, 0, 0, 0, 0 },
};

static enum elf_spu_reloc_type
spu_elf_bfd_to_reloc_type (bfd_reloc_code_real_type code)
{
  switch (code)
    {
    default:
      return R_SPU_NONE;
    case BFD_RELOC_SPU_IMM10W:
      return R_SPU_ADDR10;
    case BFD_RELOC_SPU_IMM16W:
      return R_SPU_ADDR16;
    case BFD_RELOC_SPU_LO16:
      return R_SPU_ADDR16_LO;
    case BFD_RELOC_SPU_HI16:
      return R_SPU_ADDR16_HI;
    case BFD_RELOC_SPU_IMM18:
      return R_SPU_ADDR18;
    case BFD_RELOC_SPU_PCREL16:
      return R_SPU_REL16;
    case BFD_RELOC_SPU_IMM7:
      return R_SPU_ADDR7;
    case BFD_RELOC_SPU_IMM8:
      return R_SPU_NONE;
    case BFD_RELOC_SPU_PCREL9a:
      return R_SPU_REL9;
    case BFD_RELOC_SPU_PCREL9b:
      return R_SPU_REL9I;
    case BFD_RELOC_SPU_IMM10:
      return R_SPU_ADDR10I;
    case BFD_RELOC_SPU_IMM16:
      return R_SPU_ADDR16I;
    case BFD_RELOC_32:
      return R_SPU_GLOB_DAT;
    case BFD_RELOC_32_PCREL:
      return R_SPU_REL32;
    case BFD_RELOC_SPU_PPU32:
      return R_SPU_PPU32;
    case BFD_RELOC_SPU_PPU64:
      return R_SPU_PPU64;
    case BFD_RELOC_SPU_ADD_PIC:
      return R_SPU_ADD_PIC;
    }
}

static void
spu_elf_info_to_howto (bfd *abfd ATTRIBUTE_UNUSED,
		       arelent *cache_ptr,
		       Elf_Internal_Rela *dst)
{
  enum elf_spu_reloc_type r_type;

  r_type = (enum elf_spu_reloc_type) ELF32_R_TYPE (dst->r_info);
  BFD_ASSERT (r_type < R_SPU_max);
  cache_ptr->howto = &elf_howto_table[(int) r_type];
}

static reloc_howto_type *
spu_elf_reloc_type_lookup (bfd *abfd ATTRIBUTE_UNUSED,
			   bfd_reloc_code_real_type code)
{
  enum elf_spu_reloc_type r_type = spu_elf_bfd_to_reloc_type (code);

  if (r_type == R_SPU_NONE)
    return NULL;

  return elf_howto_table + r_type;
}

/* Apply R_SPU_REL9 and R_SPU_REL9I relocs.  */

static bfd_reloc_status_type
spu_elf_rel9 (bfd *abfd, arelent *reloc_entry, asymbol *symbol,
	      void *data, asection *input_section,
	      bfd *output_bfd, char **error_message)
{
  bfd_size_type octets;
  bfd_vma val;
  long insn;

  /* If this is a relocatable link (output_bfd test tells us), just
     call the generic function.  Any adjustment will be done at final
     link time.  */
  if (output_bfd != NULL)
    return bfd_elf_generic_reloc (abfd, reloc_entry, symbol, data,
				  input_section, output_bfd, error_message);

  if (reloc_entry->address > bfd_get_section_limit (abfd, input_section))
    return bfd_reloc_outofrange;
  octets = reloc_entry->address * bfd_octets_per_byte (abfd);

  /* Get symbol value.  */
  val = 0;
  if (!bfd_is_com_section (symbol->section))
    val = symbol->value;
  if (symbol->section->output_section)
    val += symbol->section->output_section->vma;

  val += reloc_entry->addend;

  /* Make it pc-relative.  */
  val -= input_section->output_section->vma + input_section->output_offset;

  val >>= 2;
  if (val + 256 >= 512)
    return bfd_reloc_overflow;

  insn = bfd_get_32 (abfd, (bfd_byte *) data + octets);

  /* Move two high bits of value to REL9I and REL9 position.
     The mask will take care of selecting the right field.  */
  val = (val & 0x7f) | ((val & 0x180) << 7) | ((val & 0x180) << 16);
  insn &= ~reloc_entry->howto->dst_mask;
  insn |= val & reloc_entry->howto->dst_mask;
  bfd_put_32 (abfd, insn, (bfd_byte *) data + octets);
  return bfd_reloc_ok;
}

static bfd_boolean
spu_elf_new_section_hook (bfd *abfd, asection *sec)
{
  if (!sec->used_by_bfd)
    {
      struct _spu_elf_section_data *sdata;

      sdata = bfd_zalloc (abfd, sizeof (*sdata));
      if (sdata == NULL)
	return FALSE;
      sec->used_by_bfd = sdata;
    }

  return _bfd_elf_new_section_hook (abfd, sec);
}

/* Keep track of the number of relocs that are copied as dynamic relocs
   in check_relocs for each symbol, so we can later discard them if they
   are found to be unnecessary.  */
struct spu_elf_dyn_relocs
{
  struct spu_elf_dyn_relocs *next;

  /* The input section of the reloc.  */
  asection *sec;

  /* Total number of relocs copied for the input section.  */
  bfd_size_type count;
  /* Number of relocs when generating a PIE and the symbol is not dynamic. */
  bfd_size_type pie_count;
};

/* SPU ELF linker hash entry.  */

struct spu_elf_link_hash_entry
{
  struct elf_link_hash_entry elf;

  /* Track dynamic relocs copied for this symbol.  */
  struct spu_elf_dyn_relocs *dyn_relocs;
};

#define spu_elf_hash_entry(ent) ((struct spu_elf_link_hash_entry *) (ent))

/* SPU ELF fixup entry.  */

struct spu_elf_fixup
{
  /* Record the symbol */
  struct elf_link_hash_entry *h;

  /* And the offset */
  bfd_vma r_offset;
};

#define FIXUP_RECORD_SIZE 4

/* SPU ELF linker hash table.  */

struct spu_link_hash_table
{
  struct elf_link_hash_table elf;

  /* Small local sym to section mapping cache.  */
  struct sym_sec_cache sym_sec;

  /* Pointer to the fixup section */
  asection *sfixup;

  asection *init_ctors, *fini_dtors;

  /* Set if stack size analysis should be done.  */
  unsigned int stack_analysis : 1;

  /* Set if __stack_* syms will be emitted.  */
  unsigned int emit_stack_syms : 1;

  /* Set when we want to warn about non-PIC references.
   *  0 - don't warn
   *  1 - warn about references in code
   *  2 - warn about references in code and data */
  unsigned int warn_pic : 2;

  /* Set when we want to save R_SPU_GLOB_DAT relocations in section
   * .fixup when creating an executable.  */   
  unsigned int emit_fixups : 1;

  /* Set when we want to strip unneeded sections from the standard crt
   * files. */
  unsigned int strip_crt : 1;
};

#define spu_hash_table(p) \
  ((struct spu_link_hash_table *) ((p)->hash))

/* Create an entry in a SPU ELF linker hash table.  */

static struct bfd_hash_entry *
spu_elf_link_hash_newfunc (struct bfd_hash_entry *entry,
			   struct bfd_hash_table *table,
			   const char *string)
{
  /* Allocate the structure if it has not already been allocated by a
     subclass.  */
  if (entry == NULL)
    {
      entry = bfd_hash_allocate (table,
				 sizeof (struct spu_elf_link_hash_entry));
      if (entry == NULL)
	return entry;
    }

  /* Call the allocation method of the superclass.  */
  entry = _bfd_elf_link_hash_newfunc (entry, table, string);
  if (entry != NULL)
    {
      spu_elf_hash_entry (entry)->dyn_relocs = NULL;
    }

  return entry;
}

/* Create a spu ELF linker hash table.  */

static struct bfd_link_hash_table *
spu_elf_link_hash_table_create (bfd *abfd)
{
  struct spu_link_hash_table *htab;

  htab = bfd_zmalloc (sizeof (*htab));
  if (htab == NULL)
    return NULL;

  if (!_bfd_elf_link_hash_table_init (&htab->elf, abfd,
				      spu_elf_link_hash_newfunc,
				      sizeof (struct spu_elf_link_hash_entry)))
    {
      free (htab);
      return NULL;
    }

  return &htab->elf.root;
}

/* Free the derived linker hash table.  */

static void
spu_elf_link_hash_table_free (struct bfd_link_hash_table *hash)
{
  _bfd_generic_link_hash_table_free (hash);
}

/* Find the symbol for the given R_SYMNDX in IBFD and set *HP and *SYMP
   to (hash, NULL) for global symbols, and (NULL, sym) for locals.  Set
   *SYMSECP to the symbol's section.  *LOCSYMSP caches local syms.  */

static bfd_boolean
get_sym_h (struct elf_link_hash_entry **hp,
	   Elf_Internal_Sym **symp,
	   asection **symsecp,
	   Elf_Internal_Sym **locsymsp,
	   unsigned long r_symndx,
	   bfd *ibfd)
{
  Elf_Internal_Shdr *symtab_hdr = &elf_tdata (ibfd)->symtab_hdr;

  if (r_symndx >= symtab_hdr->sh_info)
    {
      struct elf_link_hash_entry **sym_hashes = elf_sym_hashes (ibfd);
      struct elf_link_hash_entry *h;

      h = sym_hashes[r_symndx - symtab_hdr->sh_info];
      while (h->root.type == bfd_link_hash_indirect
	     || h->root.type == bfd_link_hash_warning)
	h = (struct elf_link_hash_entry *) h->root.u.i.link;

      if (hp != NULL)
	*hp = h;

      if (symp != NULL)
	*symp = NULL;

      if (symsecp != NULL)
	{
	  asection *symsec = NULL;
	  if (h->root.type == bfd_link_hash_defined
	      || h->root.type == bfd_link_hash_defweak)
	    symsec = h->root.u.def.section;
	  *symsecp = symsec;
	}
    }
  else
    {
      Elf_Internal_Sym *sym;
      Elf_Internal_Sym *locsyms = *locsymsp;

      if (locsyms == NULL)
	{
	  locsyms = (Elf_Internal_Sym *) symtab_hdr->contents;
	  if (locsyms == NULL)
	    {
	      size_t symcount = symtab_hdr->sh_info;

	      /* If we are reading symbols into the contents, then
		 read the global syms too.  This is done to cache
		 syms for later stack analysis.  */
	      if ((unsigned char **) locsymsp == &symtab_hdr->contents)
		symcount = symtab_hdr->sh_size / symtab_hdr->sh_entsize;
	      locsyms = bfd_elf_get_elf_syms (ibfd, symtab_hdr, symcount, 0,
					      NULL, NULL, NULL);
	    }
	  if (locsyms == NULL)
	    return FALSE;
	  *locsymsp = locsyms;
	}
      sym = locsyms + r_symndx;

      if (hp != NULL)
	*hp = NULL;

      if (symp != NULL)
	*symp = sym;

      if (symsecp != NULL)
	{
	  asection *symsec = NULL;
	  if ((sym->st_shndx != SHN_UNDEF
	       && sym->st_shndx < SHN_LORESERVE)
	      || sym->st_shndx > SHN_HIRESERVE)
	    symsec = bfd_section_from_elf_index (ibfd, sym->st_shndx);
	  *symsecp = symsec;
	}
    }

  return TRUE;
}

/* A convenient place for capturing cammand line options */
void
spu_elf_set_link_options (struct bfd_link_info *info,
			  int stack_analysis,
			  int emit_stack_syms,
			  int flag_warn_pic,
			  int emit_fixups,
			  int strip_crt)
{
  struct spu_link_hash_table *htab = spu_hash_table (info);

  /* Stash some options away where we can get at them later.  */
  htab->stack_analysis = stack_analysis;
  htab->emit_stack_syms = emit_stack_syms;
  htab->warn_pic = flag_warn_pic;
  htab->emit_fixups = emit_fixups;
  htab->strip_crt = strip_crt;
}

/* Create the note section if not already present.  This is done early so
   that the linker maps the sections to the right place in the output.  */

bfd_boolean
spu_elf_create_sections (bfd *output_bfd ATTRIBUTE_UNUSED,
			 struct bfd_link_info *info)
{
  struct spu_link_hash_table *htab = spu_hash_table (info);
  bfd *ibfd;
  asection *s;
  flagword flags;

#ifdef DEBUG
  info->callbacks->info ( _("spu_elf_create_sections called\n"));
#endif

  for (ibfd = info->input_bfds; ibfd != NULL; ibfd = ibfd->link_next)
    if (bfd_get_section_by_name (ibfd, SPU_PTNOTE_SPUNAME) != NULL)
      break;

  if (ibfd == NULL)
    {
      /* Make SPU_PTNOTE_SPUNAME section.  */
      size_t name_len;
      size_t name_size;
      size_t size;
      bfd_byte *data;

      ibfd = info->input_bfds;
      flags = SEC_LOAD | SEC_READONLY | SEC_HAS_CONTENTS | SEC_IN_MEMORY;
      s = bfd_make_section_anyway_with_flags (ibfd, SPU_PTNOTE_SPUNAME, flags);
      if (s == NULL
	  || !bfd_set_section_alignment (ibfd, s, 4))
	return FALSE;

      /* The size of the name stored in the note should be variable
         (i.e., name_size = name_len) but lv2 uses a hard coded value
         of 32, so we do that here too. */
      name_len = strlen (bfd_get_filename (output_bfd)) + 1;
      name_size = 32;
      size = 12 + ((sizeof (SPU_PLUGIN_NAME) + 3) & -4);
      size += (name_size + 3) & -4;

      if (!bfd_set_section_size (ibfd, s, size))
	return FALSE;

      data = bfd_zalloc (ibfd, size);
      if (data == NULL)
	return FALSE;

      bfd_put_32 (ibfd, sizeof (SPU_PLUGIN_NAME), data + 0);
      bfd_put_32 (ibfd, 32, data + 4);
      bfd_put_32 (ibfd, 1, data + 8);
      memcpy (data + 12, SPU_PLUGIN_NAME, sizeof (SPU_PLUGIN_NAME));
      memcpy (data + 12 + ((sizeof (SPU_PLUGIN_NAME) + 3) & -4),
	      bfd_get_filename (output_bfd),
	      name_len < name_size ? name_len : name_size);
      s->contents = data;
    }

  if (htab->emit_fixups)
    {
      ibfd = info->input_bfds;
      flags = SEC_LOAD | SEC_ALLOC | SEC_READONLY | SEC_HAS_CONTENTS
	      | SEC_IN_MEMORY | SEC_LINKER_CREATED;
      s = bfd_make_section_with_flags (ibfd, ".fixup", flags);
      if (s == NULL || !bfd_set_section_alignment (ibfd, s, 2))
	return FALSE;
      htab->sfixup = s;
    }

  return TRUE;
}

/* Return true for all relative, absolute and indirect branch instructions.
   bra    00110000 0..
   brasl  00110001 0..
   br     00110010 0..
   brsl   00110011 0..
   brz    00100000 0..
   brnz   00100001 0..
   brhz   00100010 0..
   brhnz  00100011 0..  
   bi     00110101 000
   bisl   00110101 001
   iret   00110101 010
   bisled 00110101 011
   biz    00100101 000
   binz   00100101 001
   bihz   00100101 010
   bihnz  00100101 011
   */

static bfd_boolean
is_branch (const unsigned char *insn)
{
  return ((insn[0] & 0xec) == 0x20 || (insn[0] & 0xef) == 0x25)
	 && (insn[1] & 0x80) == 0;
}

/* OFFSET in SEC (presumably) is the beginning of a function prologue.
   Search for stack adjusting insns, and return the sp delta.  */

static int
find_function_stack_adjust (asection *sec, bfd_vma offset)
{
  int unrecog;
  int reg[128];

  memset (reg, 0, sizeof (reg));
  for (unrecog = 0; offset + 4 <= sec->size && unrecog < 32; offset += 4)
    {
      unsigned char buf[4];
      int rt, ra;
      int imm;

      /* Assume no relocs on stack adjusing insns.  */
      if (!bfd_get_section_contents (sec->owner, sec, buf, offset, 4))
	break;

      if (buf[0] == 0x24 /* stqd */)
	continue;

      rt = buf[3] & 0x7f;
      ra = ((buf[2] & 0x3f) << 1) | (buf[3] >> 7);
      /* Partly decoded immediate field.  */
      imm = (buf[1] << 9) | (buf[2] << 1) | (buf[3] >> 7);

      if (buf[0] == 0x1c /* ai */)
	{
	  imm >>= 7;
	  imm = (imm ^ 0x200) - 0x200;
	  reg[rt] = reg[ra] + imm;

	  if (rt == 1 /* sp */)
	    {
	      if (imm > 0)
		break;
	      return reg[rt];
	    }
	}
      else if (buf[0] == 0x18 && (buf[1] & 0xe0) == 0 /* a */)
	{
	  int rb = ((buf[1] & 0x1f) << 2) | ((buf[2] & 0xc0) >> 6);

	  reg[rt] = reg[ra] + reg[rb];
	  if (rt == 1)
	    return reg[rt];
	}
      else if ((buf[0] & 0xfc) == 0x40 /* il, ilh, ilhu, ila */)
	{
	  if (buf[0] >= 0x42 /* ila */)
	    imm |= (buf[0] & 1) << 17;
	  else
	    {
	      imm &= 0xffff;

	      if (buf[0] == 0x40 /* il */)
		{
		  if ((buf[1] & 0x80) == 0)
		    goto unknown_insn;
		  imm = (imm ^ 0x8000) - 0x8000;
		}
	      else if ((buf[1] & 0x80) == 0 /* ilhu */)
		imm <<= 16;
	    }
	  reg[rt] = imm;
	  continue;
	}
      else if (buf[0] == 0x60 && (buf[1] & 0x80) != 0 /* iohl */)
	{
	  reg[rt] |= imm & 0xffff;
	  continue;
	}
      else if (buf[0] == 0x04 /* ori */)
	{
	  imm >>= 7;
	  imm = (imm ^ 0x200) - 0x200;
	  reg[rt] = reg[ra] | imm;
	  continue;
	}
      else if ((buf[0] == 0x33 && imm == 1 /* brsl .+4 */)
	       || (buf[0] == 0x08 && (buf[1] & 0xe0) == 0 /* sf */))
	{
	  /* Used in pic reg load.  Say rt is trashed.  */
	  reg[rt] = 0;
	  continue;
	}
      else if (is_branch (buf))
	/* If we hit a branch then we must be out of the prologue.  */
	break;
    unknown_insn:
      ++unrecog;
    }

  return 0;
}

/* qsort predicate to sort symbols by section and value.  */

static Elf_Internal_Sym *sort_syms_syms;
static asection **sort_syms_psecs;

static int
sort_syms (const void *a, const void *b)
{
  Elf_Internal_Sym *const *s1 = a;
  Elf_Internal_Sym *const *s2 = b;
  asection *sec1,*sec2;
  bfd_signed_vma delta;

  sec1 = sort_syms_psecs[*s1 - sort_syms_syms];
  sec2 = sort_syms_psecs[*s2 - sort_syms_syms];

  if (sec1 != sec2)
    return sec1->index - sec2->index;

  delta = (*s1)->st_value - (*s2)->st_value;
  if (delta != 0)
    return delta < 0 ? -1 : 1;

  delta = (*s2)->st_size - (*s1)->st_size;
  if (delta != 0)
    return delta < 0 ? -1 : 1;

  return *s1 < *s2 ? -1 : 1;
}

struct call_info
{
  struct function_info *fun;
  struct call_info *next;
  int is_tail;
};

struct function_info
{
  /* List of functions called.  Also branches to hot/cold part of
     function.  */
  struct call_info *call_list;
  /* For hot/cold part of function, point to owner.  */
  struct function_info *start;
  /* Symbol at start of function.  */
  union {
    Elf_Internal_Sym *sym;
    struct elf_link_hash_entry *h;
  } u;
  /* Function section.  */
  asection *sec;
  /* Address range of (this part of) function.  */
  bfd_vma lo, hi;
  /* Stack usage.  */
  int stack;
  /* Set if global symbol.  */
  unsigned int global : 1;
  /* Set if known to be start of function (as distinct from a hunk
     in hot/cold section.  */
  unsigned int is_func : 1;
  /* Flags used during call tree traversal.  */
  unsigned int visit1 : 1;
  unsigned int non_root : 1;
  unsigned int visit2 : 1;
  unsigned int marking : 1;
  unsigned int visit3 : 1;
};

struct spu_elf_stack_info
{
  int num_fun;
  int max_fun;
  /* Variable size array describing functions, one per contiguous
     address range belonging to a function.  */
  struct function_info fun[1];
};

/* Allocate a struct spu_elf_stack_info with MAX_FUN struct function_info
   entries for section SEC.  */

static struct spu_elf_stack_info *
alloc_stack_info (asection *sec, int max_fun)
{
  struct _spu_elf_section_data *sec_data = spu_elf_section_data (sec);
  bfd_size_type amt;

  amt = sizeof (struct spu_elf_stack_info);
  amt += (max_fun - 1) * sizeof (struct function_info);
  sec_data->stack_info = bfd_zmalloc (amt);
  if (sec_data->stack_info != NULL)
    sec_data->stack_info->max_fun = max_fun;
  return sec_data->stack_info;
}

/* Add a new struct function_info describing a (part of a) function
   starting at SYM_H.  Keep the array sorted by address.  */

static struct function_info *
maybe_insert_function (asection *sec,
		       void *sym_h,
		       bfd_boolean global,
		       bfd_boolean is_func)
{
  struct _spu_elf_section_data *sec_data = spu_elf_section_data (sec);
  struct spu_elf_stack_info *sinfo = sec_data->stack_info;
  int i;
  bfd_vma off, size;

  if (sinfo == NULL)
    {
      sinfo = alloc_stack_info (sec, 20);
      if (sinfo == NULL)
	return NULL;
    }

  if (!global)
    {
      Elf_Internal_Sym *sym = sym_h;
      off = sym->st_value;
      size = sym->st_size;
    }
  else
    {
      struct elf_link_hash_entry *h = sym_h;
      off = h->root.u.def.value;
      size = h->size;
    }

  for (i = sinfo->num_fun; --i >= 0; )
    if (sinfo->fun[i].lo <= off)
      break;

  if (i >= 0)
    {
      /* Don't add another entry for an alias, but do update some
	 info.  */
      if (sinfo->fun[i].lo == off)
	{
	  /* Prefer globals over local syms.  */
	  if (global && !sinfo->fun[i].global)
	    {
	      sinfo->fun[i].global = TRUE;
	      sinfo->fun[i].u.h = sym_h;
	    }
	  if (is_func)
	    sinfo->fun[i].is_func = TRUE;
	  return &sinfo->fun[i];
	}
      /* Ignore a zero-size symbol inside an existing function.  */
      else if (sinfo->fun[i].hi > off && size == 0)
	return &sinfo->fun[i];
    }

  if (++i < sinfo->num_fun)
    memmove (&sinfo->fun[i + 1], &sinfo->fun[i],
	     (sinfo->num_fun - i) * sizeof (sinfo->fun[i]));
  else if (i >= sinfo->max_fun)
    {
      bfd_size_type amt = sizeof (struct spu_elf_stack_info);
      bfd_size_type old = amt;

      old += (sinfo->max_fun - 1) * sizeof (struct function_info);
      sinfo->max_fun += 20 + (sinfo->max_fun >> 1);
      amt += (sinfo->max_fun - 1) * sizeof (struct function_info);
      sinfo = bfd_realloc (sinfo, amt);
      if (sinfo == NULL)
	return NULL;
      memset ((char *) sinfo + old, 0, amt - old);
      sec_data->stack_info = sinfo;
    }
  sinfo->fun[i].is_func = is_func;
  sinfo->fun[i].global = global;
  sinfo->fun[i].sec = sec;
  if (global)
    sinfo->fun[i].u.h = sym_h;
  else
    sinfo->fun[i].u.sym = sym_h;
  sinfo->fun[i].lo = off;
  sinfo->fun[i].hi = off + size;
  sinfo->fun[i].stack = -find_function_stack_adjust (sec, off);
  sinfo->num_fun += 1;
  return &sinfo->fun[i];
}

/* Return the name of FUN.  */

static const char *
func_name (struct function_info *fun)
{
  asection *sec;
  bfd *ibfd;
  Elf_Internal_Shdr *symtab_hdr;

  while (fun->start != NULL)
    fun = fun->start;

  if (fun->global)
    return fun->u.h->root.root.string;

  sec = fun->sec;
  if (fun->u.sym->st_name == 0)
    {
      size_t len = strlen (sec->name);
      char *name = bfd_malloc (len + 10);
      if (name == NULL)
	return "(null)";
      sprintf (name, "%s+%lx", sec->name,
	       (unsigned long) fun->u.sym->st_value & 0xffffffff);
      return name;
    }
  ibfd = sec->owner;
  symtab_hdr = &elf_tdata (ibfd)->symtab_hdr;
  return bfd_elf_sym_name (ibfd, symtab_hdr, fun->u.sym, sec);
}

/* Read the instruction at OFF in SEC.  Return true iff the instruction
   is a nop, lnop, or stop 0 (all zero insn).  */

static bfd_boolean
is_nop (asection *sec, bfd_vma off)
{
  unsigned char insn[4];

  if (off + 4 > sec->size
      || !bfd_get_section_contents (sec->owner, sec, insn, off, 4))
    return FALSE;
  if ((insn[0] & 0xbf) == 0 && (insn[1] & 0xe0) == 0x20)
    return TRUE;
  if (insn[0] == 0 && insn[1] == 0 && insn[2] == 0 && insn[3] == 0)
    return TRUE;
  return FALSE;
}

/* Extend the range of FUN to cover nop padding up to LIMIT.
   Return TRUE iff some instruction other than a NOP was found.  */

static bfd_boolean
insns_at_end (struct function_info *fun, bfd_vma limit)
{
  bfd_vma off = (fun->hi + 3) & -4;

  while (off < limit && is_nop (fun->sec, off))
    off += 4;
  if (off < limit)
    {
      fun->hi = off;
      return TRUE;
    }
  fun->hi = limit;
  return FALSE;
}

/* Check and fix overlapping function ranges.  Return TRUE iff there
   are gaps in the current info we have about functions in SEC.  */

static bfd_boolean
check_function_ranges (asection *sec, struct bfd_link_info *info)
{
  struct _spu_elf_section_data *sec_data = spu_elf_section_data (sec);
  struct spu_elf_stack_info *sinfo = sec_data->stack_info;
  int i;
  bfd_boolean gaps = FALSE;

  if (sinfo == NULL)
    return FALSE;

  for (i = 1; i < sinfo->num_fun; i++)
    if (sinfo->fun[i - 1].hi > sinfo->fun[i].lo)
      {
	/* Fix overlapping symbols.  */
	const char *f1 = func_name (&sinfo->fun[i - 1]);
	const char *f2 = func_name (&sinfo->fun[i]);

	info->callbacks->einfo (_("warning: %s overlaps %s\n"), f1, f2);
	sinfo->fun[i - 1].hi = sinfo->fun[i].lo;
      }
    else if (insns_at_end (&sinfo->fun[i - 1], sinfo->fun[i].lo))
      gaps = TRUE;

  if (sinfo->num_fun == 0)
    gaps = TRUE;
  else
    {
      if (sinfo->fun[0].lo != 0)
	gaps = TRUE;
      if (sinfo->fun[sinfo->num_fun - 1].hi > sec->size)
	{
	  const char *f1 = func_name (&sinfo->fun[sinfo->num_fun - 1]);

	  info->callbacks->einfo (_("warning: %s exceeds section size\n"), f1);
	  sinfo->fun[sinfo->num_fun - 1].hi = sec->size;
	}
      else if (insns_at_end (&sinfo->fun[sinfo->num_fun - 1], sec->size))
	gaps = TRUE;
    }
  return gaps;
}

/* Search current function info for a function that contains address
   OFFSET in section SEC.  */

static struct function_info *
find_function (asection *sec, bfd_vma offset, struct bfd_link_info *info)
{
  struct _spu_elf_section_data *sec_data = spu_elf_section_data (sec);
  struct spu_elf_stack_info *sinfo = sec_data->stack_info;
  int lo, hi, mid;

  lo = 0;
  hi = sinfo->num_fun;
  while (lo < hi)
    {
      mid = (lo + hi) / 2;
      if (offset < sinfo->fun[mid].lo)
	hi = mid;
      else if (offset >= sinfo->fun[mid].hi)
	lo = mid + 1;
      else
	return &sinfo->fun[mid];
    }
  info->callbacks->einfo (_("%A:0x%v not found in function table\n"),
			  sec, offset);
  return NULL;
}

/* Add CALLEE to CALLER call list if not already present.  */

static bfd_boolean
insert_callee (struct function_info *caller, struct call_info *callee)
{
  struct call_info *p;
  for (p = caller->call_list; p != NULL; p = p->next)
    if (p->fun == callee->fun)
      {
	/* Tail calls use less stack than normal calls.  Retain entry
	   for normal call over one for tail call.  */
	if (p->is_tail > callee->is_tail)
	  p->is_tail = callee->is_tail;
	return FALSE;
      }
  callee->next = caller->call_list;
  caller->call_list = callee;
  return TRUE;
}

/* Rummage through the relocs for SEC, looking for function calls.
   If CALL_TREE is true, fill in call graph.  If CALL_TREE is false,
   mark destination symbols on calls as being functions.  Also
   look at branches, which may be tail calls or go to hot/cold
   section part of same function.  */

static bfd_boolean
mark_functions_via_relocs (asection *sec,
			   struct bfd_link_info *info,
			   int call_tree)
{
  Elf_Internal_Rela *internal_relocs, *irelaend, *irela;
  Elf_Internal_Shdr *symtab_hdr = &elf_tdata (sec->owner)->symtab_hdr;
  Elf_Internal_Sym *syms;
  void *psyms;
  static bfd_boolean warned;

  internal_relocs = _bfd_elf_link_read_relocs (sec->owner, sec, NULL, NULL,
					       info->keep_memory);
  if (internal_relocs == NULL)
    return FALSE;

  symtab_hdr = &elf_tdata (sec->owner)->symtab_hdr;
  psyms = &symtab_hdr->contents;
  syms = *(Elf_Internal_Sym **) psyms;
  irela = internal_relocs;
  irelaend = irela + sec->reloc_count;
  for (; irela < irelaend; irela++)
    {
      enum elf_spu_reloc_type r_type;
      unsigned int r_indx;
      asection *sym_sec;
      Elf_Internal_Sym *sym;
      struct elf_link_hash_entry *h;
      bfd_vma val;
      unsigned char insn[4];
      bfd_boolean is_call;
      struct function_info *caller;
      struct call_info *callee;

      r_type = ELF32_R_TYPE (irela->r_info);
      if (r_type != R_SPU_REL16
	  && r_type != R_SPU_ADDR16)
	continue;

      r_indx = ELF32_R_SYM (irela->r_info);
      if (!get_sym_h (&h, &sym, &sym_sec, psyms, r_indx, sec->owner))
	return FALSE;

      if (sym_sec == NULL
	  || sym_sec->output_section == NULL
	  || sym_sec->output_section->owner != sec->output_section->owner)
	continue;

      if (!bfd_get_section_contents (sec->owner, sec, insn,
				     irela->r_offset, 4))
	return FALSE;
      if (!is_branch (insn))
	continue;

      if ((sym_sec->flags & (SEC_ALLOC | SEC_LOAD | SEC_CODE))
	  != (SEC_ALLOC | SEC_LOAD | SEC_CODE))
	{
	  if (!call_tree)
	    warned = TRUE;
	  if (!call_tree || !warned)
	    info->callbacks->einfo (_("%B(%A+0x%v): call to non-code section"
				      " %B(%A), stack analysis incomplete\n"),
				    sec->owner, sec, irela->r_offset,
				    sym_sec->owner, sym_sec);
	  continue;
	}

      is_call = (insn[0] & 0xfd) == 0x31;

      if (h)
	val = h->root.u.def.value;
      else
	val = sym->st_value;
      val += irela->r_addend;

      if (!call_tree)
	{
	  struct function_info *fun;

	  if (irela->r_addend != 0)
	    {
	      Elf_Internal_Sym *fake = bfd_zmalloc (sizeof (*fake));
	      if (fake == NULL)
		return FALSE;
	      fake->st_value = val;
	      fake->st_shndx
		= _bfd_elf_section_from_bfd_section (sym_sec->owner, sym_sec);
	      sym = fake;
	    }
	  if (sym)
	    fun = maybe_insert_function (sym_sec, sym, FALSE, is_call);
	  else
	    fun = maybe_insert_function (sym_sec, h, TRUE, is_call);
	  if (fun == NULL)
	    return FALSE;
	  if (irela->r_addend != 0
	      && fun->u.sym != sym)
	    free (sym);
	  continue;
	}

      caller = find_function (sec, irela->r_offset, info);
      if (caller == NULL)
	return FALSE;
      callee = bfd_malloc (sizeof *callee);
      if (callee == NULL)
	return FALSE;

      callee->fun = find_function (sym_sec, val, info);
      if (callee->fun == NULL)
	return FALSE;
      callee->is_tail = !is_call;
      if (!insert_callee (caller, callee))
	free (callee);
      else if (!is_call
	       && !callee->fun->is_func
	       && callee->fun->stack == 0)
	{
	  /* This is either a tail call or a branch from one part of
	     the function to another, ie. hot/cold section.  If the
	     destination has been called by some other function then
	     it is a separate function.  We also assume that functions
	     are not split across input files.  */
	  if (callee->fun->start != NULL
	      || sec->owner != sym_sec->owner)
	    {
	      callee->fun->start = NULL;
	      callee->fun->is_func = TRUE;
	    }
	  else
	    callee->fun->start = caller;
	}
    }

  return TRUE;
}

/* Handle something like .init or .fini, which has a piece of a function.
   These sections are pasted together to form a single function.  */

static bfd_boolean
pasted_function (asection *sec, struct bfd_link_info *info)
{
  struct bfd_link_order *l;
  struct _spu_elf_section_data *sec_data;
  struct spu_elf_stack_info *sinfo;
  Elf_Internal_Sym *fake;
  struct function_info *fun, *fun_start;

  fake = bfd_zmalloc (sizeof (*fake));
  if (fake == NULL)
    return FALSE;
  fake->st_value = 0;
  fake->st_size = sec->size;
  fake->st_shndx
    = _bfd_elf_section_from_bfd_section (sec->owner, sec);
  fun = maybe_insert_function (sec, fake, FALSE, FALSE);
  if (!fun)
    return FALSE;

  /* Find a function immediately preceding this section.  */
  fun_start = NULL;
  for (l = sec->output_section->map_head.link_order; l != NULL; l = l->next)
    {
      if (l->u.indirect.section == sec)
	{
	  if (fun_start != NULL)
	    {
	      if (fun_start->start)
		fun_start = fun_start->start;
	      fun->start = fun_start;
	    }
	  return TRUE;
	}
      if (l->type == bfd_indirect_link_order
	  && (sec_data = spu_elf_section_data (l->u.indirect.section)) != NULL
	  && (sinfo = sec_data->stack_info) != NULL
	  && sinfo->num_fun != 0)
	fun_start = &sinfo->fun[sinfo->num_fun - 1];
    }

  info->callbacks->einfo (_("%A link_order not found\n"), sec);
  return FALSE;
}

/* We're only interested in code sections.  */

static bfd_boolean
interesting_section (asection *s, bfd *obfd,
		     struct spu_link_hash_table *htab ATTRIBUTE_UNUSED)
{
  return (s->output_section != NULL
	  && s->output_section->owner == obfd
	  && ((s->flags & (SEC_ALLOC | SEC_LOAD | SEC_CODE))
	      == (SEC_ALLOC | SEC_LOAD | SEC_CODE))
	  && s->size != 0);
}

/* Map address ranges in code sections to functions.  */

static bfd_boolean
discover_functions (bfd *output_bfd, struct bfd_link_info *info)
{
  struct spu_link_hash_table *htab = spu_hash_table (info);
  bfd *ibfd;
  int bfd_idx;
  Elf_Internal_Sym ***psym_arr;
  asection ***sec_arr;
  bfd_boolean gaps = FALSE;

  bfd_idx = 0;
  for (ibfd = info->input_bfds; ibfd != NULL; ibfd = ibfd->link_next)
    bfd_idx++;

  psym_arr = bfd_zmalloc (bfd_idx * sizeof (*psym_arr));
  if (psym_arr == NULL)
    return FALSE;
  sec_arr = bfd_zmalloc (bfd_idx * sizeof (*sec_arr));
  if (sec_arr == NULL)
    return FALSE;

  
  for (ibfd = info->input_bfds, bfd_idx = 0;
       ibfd != NULL;
       ibfd = ibfd->link_next, bfd_idx++)
    {
      extern const bfd_target bfd_elf32_spu_vec;
      Elf_Internal_Shdr *symtab_hdr;
      asection *sec;
      size_t symcount;
      Elf_Internal_Sym *syms, *sy, **psyms, **psy;
      asection **psecs, **p;

      if (ibfd->xvec != &bfd_elf32_spu_vec)
	continue;

      /* Read all the symbols.  */
      symtab_hdr = &elf_tdata (ibfd)->symtab_hdr;
      symcount = symtab_hdr->sh_size / symtab_hdr->sh_entsize;
      if (symcount == 0)
	continue;

      syms = (Elf_Internal_Sym *) symtab_hdr->contents;
      if (syms == NULL)
	{
	  syms = bfd_elf_get_elf_syms (ibfd, symtab_hdr, symcount, 0,
				       NULL, NULL, NULL);
	  symtab_hdr->contents = (void *) syms;
	  if (syms == NULL)
	    return FALSE;
	}

      /* Select defined function symbols that are going to be output.  */
      psyms = bfd_malloc ((symcount + 1) * sizeof (*psyms));
      if (psyms == NULL)
	return FALSE;
      psym_arr[bfd_idx] = psyms;
      psecs = bfd_malloc (symcount * sizeof (*psecs));
      if (psecs == NULL)
	return FALSE;
      sec_arr[bfd_idx] = psecs;
      for (psy = psyms, p = psecs, sy = syms; sy < syms + symcount; ++p, ++sy)
	if (ELF_ST_TYPE (sy->st_info) == STT_NOTYPE
	    || ELF_ST_TYPE (sy->st_info) == STT_FUNC)
	  {
	    asection *s;

	    *p = s = bfd_section_from_elf_index (ibfd, sy->st_shndx);
	    if (s != NULL && interesting_section (s, output_bfd, htab))
	      *psy++ = sy;
	  }
      symcount = psy - psyms;
      *psy = NULL;

      /* Sort them by section and offset within section.  */
      sort_syms_syms = syms;
      sort_syms_psecs = psecs;
      qsort (psyms, symcount, sizeof (*psyms), sort_syms);

      /* Now inspect the function symbols.  */
      for (psy = psyms; psy < psyms + symcount; )
	{
	  asection *s = psecs[*psy - syms];
	  Elf_Internal_Sym **psy2;

	  for (psy2 = psy; ++psy2 < psyms + symcount; )
	    if (psecs[*psy2 - syms] != s)
	      break;

	  if (!alloc_stack_info (s, psy2 - psy))
	    return FALSE;
	  psy = psy2;
	}

      /* First install info about properly typed and sized functions.
	 In an ideal world this will cover all code sections, except
	 when partitioning functions into hot and cold sections,
	 and the horrible pasted together .init and .fini functions.  */
      for (psy = psyms; psy < psyms + symcount; ++psy)
	{
	  sy = *psy;
	  if (ELF_ST_TYPE (sy->st_info) == STT_FUNC)
	    {
	      asection *s = psecs[sy - syms];
	      if (!maybe_insert_function (s, sy, FALSE, TRUE))
		return FALSE;
	    }
	}

      for (sec = ibfd->sections; sec != NULL && !gaps; sec = sec->next)
	if (interesting_section (sec, output_bfd, htab))
	  gaps |= check_function_ranges (sec, info);
    }

  if (gaps)
    {
      /* See if we can discover more function symbols by looking at
	 relocations.  */
      for (ibfd = info->input_bfds, bfd_idx = 0;
	   ibfd != NULL;
	   ibfd = ibfd->link_next, bfd_idx++)
	{
	  asection *sec;

	  if (psym_arr[bfd_idx] == NULL)
	    continue;

	  for (sec = ibfd->sections; sec != NULL; sec = sec->next)
	    if (interesting_section (sec, output_bfd, htab)
		&& sec->reloc_count != 0)
	      {
		if (!mark_functions_via_relocs (sec, info, FALSE))
		  return FALSE;
	      }
	}

      for (ibfd = info->input_bfds, bfd_idx = 0;
	   ibfd != NULL;
	   ibfd = ibfd->link_next, bfd_idx++)
	{
	  Elf_Internal_Shdr *symtab_hdr;
	  asection *sec;
	  Elf_Internal_Sym *syms, *sy, **psyms, **psy;
	  asection **psecs;

	  if ((psyms = psym_arr[bfd_idx]) == NULL)
	    continue;

	  psecs = sec_arr[bfd_idx];

	  symtab_hdr = &elf_tdata (ibfd)->symtab_hdr;
	  syms = (Elf_Internal_Sym *) symtab_hdr->contents;

	  gaps = FALSE;
	  for (sec = ibfd->sections; sec != NULL && !gaps; sec = sec->next)
	    if (interesting_section (sec, output_bfd, htab))
	      gaps |= check_function_ranges (sec, info);
	  if (!gaps)
	    continue;

	  /* Finally, install all globals.  */
	  for (psy = psyms; (sy = *psy) != NULL; ++psy)
	    {
	      asection *s;

	      s = psecs[sy - syms];

	      /* Global syms might be improperly typed functions.  */
	      if (ELF_ST_TYPE (sy->st_info) != STT_FUNC
		  && ELF_ST_BIND (sy->st_info) == STB_GLOBAL)
		{
		  if (!maybe_insert_function (s, sy, FALSE, FALSE))
		    return FALSE;
		}
	    }

	  /* Some of the symbols we've installed as marking the
	     beginning of functions may have a size of zero.  Extend
	     the range of such functions to the beginning of the
	     next symbol of interest.  */
	  for (sec = ibfd->sections; sec != NULL; sec = sec->next)
	    if (interesting_section (sec, output_bfd, htab))
	      {
		struct _spu_elf_section_data *sec_data;
		struct spu_elf_stack_info *sinfo;

		sec_data = spu_elf_section_data (sec);
		sinfo = sec_data->stack_info;
		if (sinfo != NULL)
		  {
		    int fun_idx;
		    bfd_vma hi = sec->size;

		    for (fun_idx = sinfo->num_fun; --fun_idx >= 0; )
		      {
			sinfo->fun[fun_idx].hi = hi;
			hi = sinfo->fun[fun_idx].lo;
		      }
		  }
		/* No symbols in this section.  Must be .init or .fini
		   or something similar.  */
		else if (!pasted_function (sec, info))
		  return FALSE;
	      }
	}
    }

  for (ibfd = info->input_bfds, bfd_idx = 0;
       ibfd != NULL;
       ibfd = ibfd->link_next, bfd_idx++)
    {
      if (psym_arr[bfd_idx] == NULL)
	continue;

      free (psym_arr[bfd_idx]);
      free (sec_arr[bfd_idx]);
    }

  free (psym_arr);
  free (sec_arr);

  return TRUE;
}

/* Mark nodes in the call graph that are called by some other node.  */

static void
mark_non_root (struct function_info *fun)
{
  struct call_info *call;

  fun->visit1 = TRUE;
  for (call = fun->call_list; call; call = call->next)
    {
      call->fun->non_root = TRUE;
      if (!call->fun->visit1)
	mark_non_root (call->fun);
    }
}

/* Remove cycles from the call graph.  */

static void
call_graph_traverse (struct function_info *fun, struct bfd_link_info *info)
{
  struct call_info **callp, *call;

  fun->visit2 = TRUE;
  fun->marking = TRUE;

  callp = &fun->call_list;
  while ((call = *callp) != NULL)
    {
      if (!call->fun->visit2)
	call_graph_traverse (call->fun, info);
      else if (call->fun->marking)
	{
	  const char *f1 = func_name (fun);
	  const char *f2 = func_name (call->fun);

	  info->callbacks->info (_("Stack analysis will ignore the call "
				   "from %s to %s\n"),
				 f1, f2);
	  *callp = call->next;
	  continue;
	}
      callp = &call->next;
    }
  fun->marking = FALSE;
}

/* Populate call_list for each function.  */

static bfd_boolean
build_call_tree (bfd *output_bfd, struct bfd_link_info *info)
{
  struct spu_link_hash_table *htab = spu_hash_table (info);
  bfd *ibfd;

  for (ibfd = info->input_bfds; ibfd != NULL; ibfd = ibfd->link_next)
    {
      extern const bfd_target bfd_elf32_spu_vec;
      asection *sec;

      if (ibfd->xvec != &bfd_elf32_spu_vec)
	continue;

      for (sec = ibfd->sections; sec != NULL; sec = sec->next)
	{
	  if (!interesting_section (sec, output_bfd, htab)
	      || sec->reloc_count == 0)
	    continue;

	  if (!mark_functions_via_relocs (sec, info, TRUE))
	    return FALSE;
	}

      /* Transfer call info from hot/cold section part of function
	 to main entry.  */
      for (sec = ibfd->sections; sec != NULL; sec = sec->next)
	{
	  struct _spu_elf_section_data *sec_data;
	  struct spu_elf_stack_info *sinfo;

	  if ((sec_data = spu_elf_section_data (sec)) != NULL
	      && (sinfo = sec_data->stack_info) != NULL)
	    {
	      int i;
	      for (i = 0; i < sinfo->num_fun; ++i)
		{
		  if (sinfo->fun[i].start != NULL)
		    {
		      struct call_info *call = sinfo->fun[i].call_list;

		      while (call != NULL)
			{
			  struct call_info *call_next = call->next;
			  if (!insert_callee (sinfo->fun[i].start, call))
			    free (call);
			  call = call_next;
			}
		      sinfo->fun[i].call_list = NULL;
		      sinfo->fun[i].non_root = TRUE;
		    }
		}
	    }
	}
    }

  /* Find the call graph root(s).  */
  for (ibfd = info->input_bfds; ibfd != NULL; ibfd = ibfd->link_next)
    {
      extern const bfd_target bfd_elf32_spu_vec;
      asection *sec;

      if (ibfd->xvec != &bfd_elf32_spu_vec)
	continue;

      for (sec = ibfd->sections; sec != NULL; sec = sec->next)
	{
	  struct _spu_elf_section_data *sec_data;
	  struct spu_elf_stack_info *sinfo;

	  if ((sec_data = spu_elf_section_data (sec)) != NULL
	      && (sinfo = sec_data->stack_info) != NULL)
	    {
	      int i;
	      for (i = 0; i < sinfo->num_fun; ++i)
		if (!sinfo->fun[i].visit1)
		  mark_non_root (&sinfo->fun[i]);
	    }
	}
    }

  /* Remove cycles from the call graph.  We start from the root node(s)
     so that we break cycles in a reasonable place.  */
  for (ibfd = info->input_bfds; ibfd != NULL; ibfd = ibfd->link_next)
    {
      extern const bfd_target bfd_elf32_spu_vec;
      asection *sec;

      if (ibfd->xvec != &bfd_elf32_spu_vec)
	continue;

      for (sec = ibfd->sections; sec != NULL; sec = sec->next)
	{
	  struct _spu_elf_section_data *sec_data;
	  struct spu_elf_stack_info *sinfo;

	  if ((sec_data = spu_elf_section_data (sec)) != NULL
	      && (sinfo = sec_data->stack_info) != NULL)
	    {
	      int i;
	      for (i = 0; i < sinfo->num_fun; ++i)
		if (!sinfo->fun[i].non_root)
		  call_graph_traverse (&sinfo->fun[i], info);
	    }
	}
    }

  return TRUE;
}

/* Descend the call graph for FUN, accumulating total stack required.  */

static bfd_vma
sum_stack (struct function_info *fun,
	   struct bfd_link_info *info,
	   int emit_stack_syms)
{
  struct call_info *call;
  struct function_info *max = NULL;
  bfd_vma max_stack = fun->stack;
  bfd_vma stack;
  const char *f1;

  if (fun->visit3)
    return max_stack;

  for (call = fun->call_list; call; call = call->next)
    {
      stack = sum_stack (call->fun, info, emit_stack_syms);
      /* Include caller stack for normal calls, don't do so for
	 tail calls.  fun->stack here is local stack usage for
	 this function.  */
      if (!call->is_tail)
	stack += fun->stack;
      if (max_stack < stack)
	{
	  max_stack = stack;
	  max = call->fun;
	}
    }

  f1 = func_name (fun);
  info->callbacks->minfo (_("%s: 0x%v 0x%v\n"), f1, (bfd_vma)fun->stack, max_stack);

  if (fun->call_list)
    {
      info->callbacks->minfo (_("  calls:\n"));
      for (call = fun->call_list; call; call = call->next)
	{
	  const char *f2 = func_name (call->fun);
	  const char *ann1 = call->fun == max ? "*" : " ";
	  const char *ann2 = call->is_tail ? "t" : " ";

	  info->callbacks->minfo (_("   %s%s %s\n"), ann1, ann2, f2);
	}
    }

  /* Now fun->stack holds cumulative stack.  */
  fun->stack = max_stack;
  fun->visit3 = TRUE;

  if (emit_stack_syms)
    {
      struct spu_link_hash_table *htab = spu_hash_table (info);
      char *name = bfd_malloc (18 + strlen (f1));
      struct elf_link_hash_entry *h;

      if (name != NULL)
	{
	  if (fun->global || ELF_ST_BIND (fun->u.sym->st_info) == STB_GLOBAL)
	    sprintf (name, "__stack_%s", f1);
	  else
	    sprintf (name, "__stack_%x_%s", fun->sec->id & 0xffffffff, f1);

	  h = elf_link_hash_lookup (&htab->elf, name, TRUE, TRUE, FALSE);
	  free (name);
	  if (h != NULL
	      && (h->root.type == bfd_link_hash_new
		  || h->root.type == bfd_link_hash_undefined
		  || h->root.type == bfd_link_hash_undefweak))
	    {
	      h->root.type = bfd_link_hash_defined;
	      h->root.u.def.section = bfd_abs_section_ptr;
	      h->root.u.def.value = max_stack;
	      h->size = 0;
	      h->type = 0;
	      h->ref_regular = 1;
	      h->def_regular = 1;
	      h->ref_regular_nonweak = 1;
	      h->forced_local = 1;
	      h->non_elf = 0;
	    }
	}
    }

  return max_stack;
}

/* Provide an estimate of total stack required.  */

static bfd_boolean
spu_elf_stack_analysis (bfd *output_bfd,
			struct bfd_link_info *info,
			int emit_stack_syms)
{
  bfd *ibfd;
  bfd_vma max_stack = 0;

  if (!discover_functions (output_bfd, info))
    return FALSE;

  if (!build_call_tree (output_bfd, info))
    return FALSE;

  info->callbacks->info (_("Stack size for call graph root nodes.\n"));
  info->callbacks->minfo (_("\nStack size for functions.  "
			    "Annotations: '*' max stack, 't' tail call\n"));
  for (ibfd = info->input_bfds; ibfd != NULL; ibfd = ibfd->link_next)
    {
      extern const bfd_target bfd_elf32_spu_vec;
      asection *sec;

      if (ibfd->xvec != &bfd_elf32_spu_vec)
	continue;

      for (sec = ibfd->sections; sec != NULL; sec = sec->next)
	{
	  struct _spu_elf_section_data *sec_data;
	  struct spu_elf_stack_info *sinfo;

	  if ((sec_data = spu_elf_section_data (sec)) != NULL
	      && (sinfo = sec_data->stack_info) != NULL)
	    {
	      int i;
	      for (i = 0; i < sinfo->num_fun; ++i)
		{
		  if (!sinfo->fun[i].non_root)
		    {
		      bfd_vma stack;
		      const char *f1;

		      stack = sum_stack (&sinfo->fun[i], info,
					 emit_stack_syms);
		      f1 = func_name (&sinfo->fun[i]);
		      info->callbacks->info (_("  %s: 0x%v\n"),
					      f1, stack);
		      if (max_stack < stack)
			max_stack = stack;
		    }
		}
	    }
	}
    }

  info->callbacks->info (_("Lower bound of maximum stack required is 0x%v (%u bytes)\n"), max_stack, (unsigned int)max_stack);
  return TRUE;
}

/* Perform a final link.  */

static bfd_boolean
spu_elf_final_link (bfd *output_bfd, struct bfd_link_info *info)
{
  struct spu_link_hash_table *htab = spu_hash_table (info);

  if (htab->stack_analysis
      && !spu_elf_stack_analysis (output_bfd, info, htab->emit_stack_syms))
    info->callbacks->einfo ("%X%P: stack analysis error: %E\n");

  return bfd_elf_final_link (output_bfd, info);
}

/* Called when not normally emitting relocs, ie. !info->relocatable
   and !info->emitrelocations.  Returns a count of special relocs
   that need to be emitted.  */

static unsigned int
spu_elf_count_relocs (asection *sec, Elf_Internal_Rela *relocs)
{
  unsigned int count = 0;
  Elf_Internal_Rela *relend = relocs + sec->reloc_count;

  if (relocs == NULL)
    return 0;

  for (; relocs < relend; relocs++)
    {
      int r_type = ELF32_R_TYPE (relocs->r_info);
      if (r_type == R_SPU_PPU32 || r_type == R_SPU_PPU64)
	++count;
    }

  return count;
}

/* Given the location of 2 instructions, determine if they are the
 * special sequence that computes the base pic register: 
 *     ila $a,label
 *     ...
 *     brsl $b,4
 *  label:
 *     ...
 */
static bfd_boolean
is_picreg_insns (bfd *inbfd, bfd_byte *insn1, bfd_byte *insn2)
{
  bfd_vma x = bfd_get_32 (inbfd, insn1);
  if ((x & 0xfe000000) != 0x42000000) /* ila $r,target */
    return FALSE;
  x = bfd_get_32 (inbfd, insn2);
  if ((x & 0xffffff80) != 0x33000080) /* brsl $r,4 */
    return FALSE;
  return TRUE;
}

/* Functions for adding fixup records to .fixup */

#define FIXUP_PUT(output_bfd,htab,index,addr) \
	  bfd_put_32 (output_bfd, addr, \
		      htab->sfixup->contents + FIXUP_RECORD_SIZE * (index))
#define FIXUP_GET(output_bfd,htab,index) \
	  bfd_get_32 (output_bfd, \
		      htab->sfixup->contents + FIXUP_RECORD_SIZE * (index))

/* Store OFFSET in .fixup.  This assumes it will be called with an
 * increasing OFFSET.  When this OFFSET fits with the last base offset,
 * it just sets a bit, otherwise it adds a new fixup record.  */
static void
spu_elf_emit_fixup (bfd *output_bfd, struct bfd_link_info *info,
                    bfd_vma offset)
{
  struct spu_link_hash_table *htab = spu_hash_table (info);
  asection *sfixup = htab->sfixup;
  bfd_vma qaddr = offset & ~(bfd_vma)15;
  bfd_vma bit = ((bfd_vma)8) >> ((offset & 15) >> 2);
  if (sfixup->reloc_count == 0)
    {
      FIXUP_PUT (output_bfd, htab, 0, qaddr | bit);
      sfixup->reloc_count++;
    }
  else
    {
      bfd_vma base = FIXUP_GET (output_bfd, htab, sfixup->reloc_count - 1);
      if (qaddr != (base & ~(bfd_vma)15))
	{
	  if ((sfixup->reloc_count + 1) * FIXUP_RECORD_SIZE > sfixup->size)
	    (*_bfd_error_handler) (_("fatal error while creating .fixup"));
	  FIXUP_PUT (output_bfd, htab, sfixup->reloc_count, qaddr | bit);
	  sfixup->reloc_count++;
	}
      else 
	FIXUP_PUT (output_bfd, htab, sfixup->reloc_count - 1, base | bit);
    }
}

/* This is called from check_relocs, allocate_dynrelocs, and
 * relocate_section.  STRICT is TRUE from the latter 2.
 */
static bfd_boolean
needs_dynreloc (struct bfd_link_info *info, struct elf_link_hash_entry *h, int strict)
{
  struct spu_link_hash_table *htab = spu_hash_table (info);
  if ((info->shared || htab->elf.is_relocatable_executable)
       && (strict
	   ? (h == NULL
	      || ELF_ST_VISIBILITY (h->other) == STV_DEFAULT
	      || h->root.type != bfd_link_hash_undefweak)
	     && !SYMBOL_CALLS_LOCAL (info, h)
	   : (h != NULL
	      && (!info->symbolic
		  || h->root.type == bfd_link_hash_defweak
		  || !h->def_regular))))
    return TRUE;

  if (h != NULL
      && (strict
	  ? (h->dynindx != -1
	     && ((h->def_dynamic && !h->def_regular)
		 || h->root.type == bfd_link_hash_undefweak
		 || h->root.type == bfd_link_hash_undefined))
	  : (h->root.type == bfd_link_hash_defweak
	     || !h->def_regular)))
    return TRUE;
  return FALSE;
}

/* Apply RELOCS to CONTENTS of INPUT_SECTION from INPUT_BFD.  */

static bfd_boolean
spu_elf_relocate_section (bfd *output_bfd,
			  struct bfd_link_info *info,
			  bfd *input_bfd,
			  asection *input_section,
			  bfd_byte *contents,
			  Elf_Internal_Rela *relocs,
			  Elf_Internal_Sym *local_syms,
			  asection **local_sections)
{
  Elf_Internal_Shdr *symtab_hdr;
  struct elf_link_hash_entry **sym_hashes;
  Elf_Internal_Rela *rel, *relend;
  struct spu_link_hash_table *htab;
  bfd_boolean ret = TRUE;
  bfd_boolean emit_these_relocs = FALSE;
  bfd_boolean is_nonpic_object = FALSE;
  bfd_boolean computes_pic_base = FALSE;
  asection *sreloc = NULL;
  Elf_Internal_Rela outrel;
  bfd_byte *loc;

#ifdef DEBUG
      info->callbacks->info ( _("spu_elf_relocate_section called for %B section %A, "
		      "%ld relocations%s\n"),
		      input_bfd, input_section,
		      (long) input_section->reloc_count,
		      (info->relocatable) ? " (relocatable)" : "");
#endif

  htab = spu_hash_table (info);
  symtab_hdr = &elf_tdata (input_bfd)->symtab_hdr;
  sym_hashes = (struct elf_link_hash_entry **) (elf_sym_hashes (input_bfd));

  if ((info->shared || htab->warn_pic || htab->emit_fixups)
      && input_section->flags & SEC_LOAD)
    {
      rel = relocs;
      relend = relocs + input_section->reloc_count;
      for (; rel < relend; rel++)
	{
	  int r_type;
	  unsigned long r_symndx;
	  asection *sec;
	  struct elf_link_hash_entry *h;
	  bfd_vma relocation;

	  r_symndx = ELF32_R_SYM (rel->r_info);
	  r_type = ELF32_R_TYPE (rel->r_info);
	  if (r_type != R_SPU_ADDR18 && r_type != R_SPU_REL16)
	    continue;

	  sec = NULL;
	  relocation = 0;
	  if (r_symndx < symtab_hdr->sh_info)
	    {
	      sec = local_sections[r_symndx];
	    }
	  else
	    {
	      if (sym_hashes == NULL)
		return FALSE;
	      h = sym_hashes[r_symndx - symtab_hdr->sh_info];
	      while (h->root.type == bfd_link_hash_indirect
		     || h->root.type == bfd_link_hash_warning)
		h = (struct elf_link_hash_entry *) h->root.u.i.link;
	      if (h->root.type == bfd_link_hash_defined
		  || h->root.type == bfd_link_hash_defweak)
		sec = h->root.u.def.section;
	      relocation = h->root.u.def.value;
	    }

	  if (sec != input_section)
	    continue;

	  bfd_byte *insn1 = contents + rel->r_offset;
	  bfd_byte *insn2 = contents + rel->r_addend - 4 + relocation;

	  /* Determine if this section contains a code sequence that
	   * loads the PIC base offset into a register.   This is true
	   * when we see a brsl whose target is the next instruction. */
	  if ((r_type == R_SPU_ADDR18
	       && is_picreg_insns (input_bfd, insn1, insn2))
	      || (r_type == R_SPU_REL16
	          && insn1 == insn2
		  && (bfd_get_32 (input_bfd, insn1) & 0xff800000) == 0x33000000)) /* brsl */
	    {
	      computes_pic_base = TRUE;
	      break;
	    }
	}
    }

  rel = relocs;
  relend = relocs + input_section->reloc_count;
  for (; rel < relend; rel++)
    {
      int r_type;
      reloc_howto_type *howto;
      unsigned long r_symndx;
      Elf_Internal_Sym *sym;
      asection *sec;
      struct elf_link_hash_entry *h;
      const char *sym_name;
      bfd_vma relocation;
      bfd_vma addend;
      bfd_reloc_status_type r;
      bfd_boolean unresolved_reloc;
      bfd_boolean is_nonpic_reloc;
      bfd_boolean warned;

      r_symndx = ELF32_R_SYM (rel->r_info);
      r_type = ELF32_R_TYPE (rel->r_info);
      if (r_type == R_SPU_PPU32 || r_type == R_SPU_PPU64)
	{
	  emit_these_relocs = TRUE;
	  continue;
	}

      howto = elf_howto_table + r_type;
      is_nonpic_reloc = FALSE;
      unresolved_reloc = FALSE;
      warned = FALSE;
      h = NULL;
      sym = NULL;
      sec = NULL;
      if (r_symndx < symtab_hdr->sh_info)
	{
	  sym = local_syms + r_symndx;
	  sec = local_sections[r_symndx];
	  sym_name = bfd_elf_sym_name (input_bfd, symtab_hdr, sym, sec);
	  relocation = _bfd_elf_rela_local_sym (output_bfd, sym, &sec, rel);
	}
      else
	{
	  RELOC_FOR_GLOBAL_SYMBOL (info, input_bfd, input_section, rel,
				   r_symndx, symtab_hdr, sym_hashes,
				   h, sec, relocation,
				   unresolved_reloc, warned);
	  sym_name = h->root.root.string;
	}

      if (sec != NULL && elf_discarded_section (sec))
	{
	  /* For relocs against symbols from removed linkonce sections,
	     or sections discarded by a linker script, we just want the
	     section contents zeroed.  Avoid any special processing.  */
	  _bfd_clear_contents (howto, input_bfd, contents + rel->r_offset);
	  rel->r_info = 0;
	  rel->r_addend = 0;
	  continue;
	}

      if (info->relocatable)
	continue;

      /*  Determine if this section is contains non-PIC relocations */
      if ((info->shared || htab->warn_pic > 0 || htab->emit_fixups)
	  && (input_section->flags & SEC_LOAD)
	  && (sym_name == 0 || sec == 0
	      || !bfd_is_abs_section (sec)
	      || strncmp (sym_name, "__ABS__", 7) != 0))
	{
	  /* Most absolute address relocations means this object is
	   * non-PIC.  The exception is R_SPU_ADDR18 which is used when
	   * computing an address in PIC mode. 
	   *
	   * When computing addresses for PIC code the compiler always
	   * inserts the sequence
	   *     ila $a,.+8
	   *     brsl $b,.+4
	   *     sf   $picreg,$a,$b
	   * in a function when it computes the actual address with
	   *     ila  $c,symbol
	   *     a    $d,$picreg,$c
	   * Both of the ila instructions will have a relocation of type
	   * R_SPU_ADDR18.  When the first R_SPU_ADDR18 is this special 
	   * sequence we assume all other R_SPU_ADDR18 relocations are
	   * safe for PIC code.  */
	  switch (r_type)
	    {
	    case R_SPU_ADDR10:    /* lqd/stqd */
	    case R_SPU_ADDR16:    /* bra* */
	    case R_SPU_ADDR7:     /* rot*i/shl*i */
	    case R_SPU_ADDR10I:   /* ai/sfi/etc... */
	    case R_SPU_ADDR16I:   /* ilh/iohl */
	      is_nonpic_reloc = is_nonpic_object = TRUE;
	      break;
	    case R_SPU_GLOB_DAT:  /* initializing global data */
	      if (htab->warn_pic == 2)
		is_nonpic_object = TRUE;
	      break;
	    case R_SPU_ADDR16_LO: /* iohl */
	    case R_SPU_ADDR16_HI: /* ilhu */
	    case R_SPU_ADDR18:    /* ila/lqa/stqa */
	      if (!computes_pic_base)
		is_nonpic_reloc = is_nonpic_object = TRUE;
	      break;
	    }
	}

      addend = rel->r_addend;

      switch (r_type)
	{
	case R_SPU_NONE:
	case R_SPU_max:
	  break;

	  /* These are handled above */
	case R_SPU_PPU32:
	case R_SPU_PPU64:
	  break;

	  /* These are always local */
	case R_SPU_REL9:
	case R_SPU_REL9I:
	case R_SPU_REL32:
	  break;

	  /* These may be generated by PIC, but are adjusted at run-time
	   * by an add instruction in the code.  When we propagate the
	   * relocation we need to account for the existing adjustment.  
	   * When they are in a non-PIC object, we must not account for
	   * any adjustment. */
	case R_SPU_ADDR18:
	case R_SPU_ADDR16_LO:
	case R_SPU_ADDR16_HI:

	  /* This marks the add instruction that adjusts the above
	   * relocations.  When dynamically linking, this instruction
	   * should be patched to "ai $dst,$src,0" */
	case R_SPU_ADD_PIC:

	  /* This is for branches, loads and stores.  It should never
	   * exist because R_SPU_REL16 should always get generated
	   * instead. */
	case R_SPU_ADDR16:

	  /* These shouldn't exist in PIC, but can exist in a exe which
	   * needs to be dynamically linked. */
	case R_SPU_ADDR10:
	case R_SPU_ADDR7:
	case R_SPU_ADDR16I:
	case R_SPU_ADDR10I:
	case R_SPU_ADDR16X:

	  /* Relocations that always need to be propagated if this is a shared
	     object.  */
	case R_SPU_REL16:
	case R_SPU_GLOB_DAT:
	  /* r_symndx will be zero only for relocs against symbols
	     from removed linkonce sections, or sections discarded by
	     a linker script.  */
	  if (r_symndx == 0)
	    break;
	  /* Fall thru.  */

	  if ((input_section->flags & SEC_ALLOC) == 0)
	    break;
	  /* Fall thru.  */

	  if (needs_dynreloc (info, h, 1))
	    {
	      int skip;

#ifdef DEBUG
	      info->callbacks->info (_("spu_elf_relocate_section needs to "
		       "create relocation for %s\n"),
		       (h && h->root.root.string
			? h->root.root.string : "<unknown>"));
#endif

	      /* When generating a shared object, these relocations
		 are copied into the output file to be resolved at run
		 time.  */
	      skip = 0;

	      outrel.r_offset =
		_bfd_elf_section_offset (output_bfd, info, input_section,
					 rel->r_offset);
	      if (outrel.r_offset == (bfd_vma) -1
		  || outrel.r_offset == (bfd_vma) -2)
		skip = (int) outrel.r_offset;
	      else
		outrel.r_offset += (input_section->output_section->vma
				    + input_section->output_offset);

	      if (skip)
		memset (&outrel, 0, sizeof outrel);
	      else if (r_type == R_SPU_ADD_PIC)
		{
		  /* This symbol has a dynamic relocation, so this add
		     should be changed to a reg copy. */
		  relocation = 0x1c000000;
		  addend = 0;
		  break;
		}
	      else if (!SYMBOL_REFERENCES_LOCAL (info, h))
		{
		  unresolved_reloc = FALSE;
		  outrel.r_info = ELF32_R_INFO (h->dynindx, r_type);
		  outrel.r_addend = rel->r_addend;
		}
	      else
		{
		  long indx;
		  outrel.r_addend = relocation + rel->r_addend;

		  if (sec && bfd_is_abs_section (sec))
		    indx = 0;
		  else if (sec == NULL || sec->owner == NULL)
		    {
		      bfd_set_error (bfd_error_bad_value);
		      return FALSE;
		    }
		  else
		    {
		      asection *osec;

		      /* We are turning this relocation into one
			 against a section symbol.  It would be
			 proper to subtract the symbol's value,
			 osec->vma, from the emitted reloc addend,
			 but ld.so expects buggy relocs.  */
		      osec = sec->output_section;
		      indx = elf_section_data (osec)->dynindx;
		      BFD_ASSERT (indx > 0);
#ifdef DEBUG
		      if (indx <= 0)
			info->callbacks->info (_("indx=%ld section=%s flags=%ld name=%s\n"),
				indx, osec->name, osec->flags,
				(h && h->root.root.string
				 ? h->root.root.string : "<unknown>"));
#endif
		    }

		  outrel.r_info = ELF32_R_INFO (indx, r_type);
		}

	      if (sreloc == NULL)
		{
		  const char *name;

		  name = (bfd_elf_string_from_elf_section
			  (input_bfd,
			   elf_elfheader (input_bfd)->e_shstrndx,
			   elf_section_data (input_section)->rel_hdr.sh_name));
		  if (name == NULL)
		    return FALSE;

		  BFD_ASSERT (strncmp (name, ".rela", 5) == 0
			      && strcmp (bfd_get_section_name (input_bfd,
							       input_section),
					 name + 5) == 0);

		  sreloc = bfd_get_section_by_name (htab->elf.dynobj, name);
		  BFD_ASSERT (sreloc != NULL);
		}

	      if (sreloc->contents == NULL)
		return FALSE;

	      loc = sreloc->contents;
	      loc += sreloc->reloc_count++ * sizeof (Elf32_External_Rela);
	      bfd_elf32_swap_reloca_out (output_bfd, &outrel, loc);

	      if (skip == -1)
		continue;

	      /* This reloc will be computed at runtime.  We clear the memory
		 so that it contains predictable value.  */
	      if (! skip
		  && (input_section->flags & SEC_ALLOC) != 0)
		{
		  relocation = howto->pc_relative ? outrel.r_offset : 0;
		  addend = 0;
		  break;
		}
	    }
	  else if (htab->emit_fixups && r_type == R_SPU_GLOB_DAT
	           && strncmp (sym_name, "__ABS__", 7) != 0)
	    {
	      bfd_vma offset;
	      
	      BFD_ASSERT(outrel.r_offset != (bfd_vma) -1 && outrel.r_offset != (bfd_vma) -2);

	      offset = rel->r_offset + input_section->output_section->vma
				  + input_section->output_offset;
	      spu_elf_emit_fixup (output_bfd, info, offset);
#if defined DEBUG
	      info->callbacks->info (_("  fixup for name=%s section=%s offset=%v count=%d\n"),
		      (h && h->root.root.string ? h->root.root.string : "<unknown>"),
		      sec->name, 
		      offset,
		      htab->sfixup->reloc_count);
#endif
	    }
	  /* When it is not a dynamic symbol, leave the add instruction. */
	  else if (r_type == R_SPU_ADD_PIC)
	    continue;
	  break;
	}

#ifdef DEBUG
      info->callbacks->info (_( "\ttype = %s (%d), name = %s, symbol index = %ld, "
	       "offset = %ld, value = %ld addend = %ld\n"),
	       howto->name,
	       (int) r_type,
	       sym_name,
	       r_symndx,
	       (long) rel->r_offset,
	       (long) relocation,
	       (long) addend);
#endif
	if (((info->shared || htab->emit_fixups) && is_nonpic_reloc)
	    || (unresolved_reloc && !h->def_dynamic))
	  {
	    (*_bfd_error_handler)
	      (_("%B(%s+0x%lx):%s%s %s relocation against symbol `%s'"),
	       input_bfd,
	       bfd_get_section_name (input_bfd, input_section),
	       (long) rel->r_offset,
	       unresolved_reloc ? " unresolvable" : "",
	       is_nonpic_reloc ? " non-PIC" : "",
	       howto->name,
	       sym_name);
	    ret = FALSE;
	  }

	/* When an absolute symbol is being used in a PC-relative
	   context, we sometimes want the absolute value to be used as
	   is.  Perhaps all symbols in the ABS section should behave
	   this way, for now we limit it to symbols that start with
	   __ABS__. */
	if (sec && bfd_is_abs_section (sec) 
	    && howto->pc_relative
	    && strncmp (sym_name, "__ABS__", 7) == 0)
	  relocation += input_section->output_section->vma 
			+ input_section->output_offset
			+ rel->r_offset - rel->r_addend;

	r = _bfd_final_link_relocate (howto,
				      input_bfd,
				      input_section,
				      contents,
				      rel->r_offset, relocation, addend);

	if (r != bfd_reloc_ok)
	  {
	    const char *msg = (const char *) 0;

	    switch (r)
	      {
	      case bfd_reloc_overflow:
		if (!((*info->callbacks->reloc_overflow)
		      (info, (h ? &h->root : NULL), sym_name, howto->name,
		       (bfd_vma) 0, input_bfd, input_section, rel->r_offset)))
		  return FALSE;
		break;

	      case bfd_reloc_undefined:
		if (!((*info->callbacks->undefined_symbol)
		      (info, sym_name, input_bfd, input_section,
		       rel->r_offset, TRUE)))
		  return FALSE;
		break;

	      case bfd_reloc_outofrange:
		msg = _("internal error: out of range error");
		goto common_error;

	      case bfd_reloc_notsupported:
		msg = _("internal error: unsupported relocation error");
		goto common_error;

	      case bfd_reloc_dangerous:
		msg = _("internal error: dangerous error");
		goto common_error;

	      default:
		msg = _("internal error: unknown error");
		/* fall through */

	      common_error:
		if (!((*info->callbacks->warning)
		      (info, msg, sym_name, input_bfd, input_section,
		       rel->r_offset)))
		  return FALSE;
		break;
	      }
	  }
      }

  if (htab->warn_pic && is_nonpic_object)
    {
      info->callbacks->einfo (_("warning: section '%s' in '%B' is non-PIC\n"),
	 bfd_get_section_name (input_bfd, input_section),
	 input_bfd);
    }

  if (ret
      && emit_these_relocs
      && !info->relocatable
      && !info->emitrelocations)
    {
      Elf_Internal_Rela *wrel;
      Elf_Internal_Shdr *rel_hdr;

      wrel = rel = relocs;
      relend = relocs + input_section->reloc_count;
      for (; rel < relend; rel++)
	{
	  int r_type;

	  r_type = ELF32_R_TYPE (rel->r_info);
	  if (r_type == R_SPU_PPU32 || r_type == R_SPU_PPU64)
	    *wrel++ = *rel;
	}
      input_section->reloc_count = wrel - relocs;
      /* Backflips for _bfd_elf_link_output_relocs.  */
      rel_hdr = &elf_section_data (input_section)->rel_hdr;
      rel_hdr->sh_size = input_section->reloc_count * rel_hdr->sh_entsize;
      ret = 2;
    }

  return ret;
}

static asection *
spu_elf_gc_mark_hook (asection *sec,
                     struct bfd_link_info *info ATTRIBUTE_UNUSED,
                     Elf_Internal_Rela *rel ATTRIBUTE_UNUSED, struct elf_link_hash_entry *h,
                     Elf_Internal_Sym *sym)
{
  struct spu_link_hash_table *htab = spu_hash_table (info);
  if (htab->init_ctors && !htab->init_ctors->gc_mark
      && strncmp (sec->name, ".ctors", 6) == 0
      && (sec->name[6] == 0 || sec->name[6] == '.'))
    _bfd_elf_gc_mark (info, htab->init_ctors, spu_elf_gc_mark_hook);

  if (htab->fini_dtors && !htab->fini_dtors->gc_mark
      && strncmp (sec->name, ".dtors", 6) == 0
      && (sec->name[6] == 0 || sec->name[6] == '.'))
      _bfd_elf_gc_mark (info, htab->fini_dtors, spu_elf_gc_mark_hook);

  if (h != NULL)
    {
      switch (h->root.type)
        {
        case bfd_link_hash_defined:
        case bfd_link_hash_defweak:
          return h->root.u.def.section;
                                                                                                  
        case bfd_link_hash_common:
          return h->root.u.c.p->section;
                                                                                                  
        default:
          break;
        }
    }
  else
    return bfd_section_from_elf_index (sec->owner, sym->st_shndx);
                                                                                                  
  return NULL;
}
                                                                                                  
static unsigned long long spu_guid = 0x0;

static void
spu_elf_final_write_processing (bfd * abfd, bfd_boolean linker)
{
  asection *guid_sec;

  elf_elfheader (abfd)->e_machine = EM_SPU;

  /* We do these verifications here because this function is called only
   * once.  There may be a better place to do it, I didn't look. */

  BFD_ASSERT (elf_elfheader (abfd)->e_ident[EI_CLASS] == ELFCLASS32);
  BFD_ASSERT (elf_elfheader (abfd)->e_ident[EI_DATA] == ELFDATA2MSB);
  BFD_ASSERT ((elf_elfheader (abfd)->e_flags & ~EF_SPU_MASK) == 0);

  /* Verify that elf_howto_table is in the correct order. */
  {
    unsigned int i;
    for (i = 0; i < sizeof (elf_howto_table) / sizeof (*elf_howto_table); i++)
      {
	BFD_ASSERT (elf_howto_table[i].type == i);
      }
  }

  /* begin sce local, bugzilla 2878 */
  /* Embedding the spu_guid to .SpuGUID section.
     The spu_guid is calculated in spu_elf_section_processing() over 
     sections that are loadable and allocatable. */
  guid_sec = bfd_get_section_by_name (abfd, ".SpuGUID");
  if (linker && guid_sec != NULL)
    {
      unsigned int ui;
      bfd_boolean stat;
      bfd_size_type guid_sec_size, written_bytes;
      bfd_byte *guid_contents;

      guid_sec_size = bfd_get_section_size (guid_sec);

      guid_contents = bfd_malloc (guid_sec_size);
      BFD_ASSERT (guid_contents != NULL);      
	  
      /* Embedd the hash id into .SpuGUID section */
      for (ui = 0; ui < 4; ++ui)
	{
	  /* SPU GUID has following format.
	   * +---------------------------------------------------+
	   * |<GUID harf0>|<GUID half1>|<GUID half2>|<GUID half3>|
	   * +---------------------------------------------------+
	   *  bit 0     15 16        31 32        47 48        63
	   *
	   * __SPU_GUID:
	   *        ila  $2, <GUID half0> | 0b'00
	   *        ila  $2, <GUID half1> | 0b'01
	   *        ila  $2, <GUID half2> | 0b'10
	   *        ila  $2, <GUID half3> | 0b'11
	   * # '|' means a bit concatination
	   */
	  unsigned int ila_insn = 0x42000002; /* opcode of ILA instruction and RT operand*/
	  unsigned int imm18 = (unsigned int)((spu_guid >> (16 * (3 - ui))) & 0xffff);
	  imm18 = (imm18 << 2) | ui;
	  imm18 <<= 7;
	  ila_insn |= imm18;

	  bfd_put_32 (abfd, ila_insn, &guid_contents[ui * 4]);
	}
      
      stat = bfd_seek (abfd, guid_sec->filepos, SEEK_SET);
      BFD_ASSERT (stat == 0);	/* bfd_seek return 0 for success. */
      
      written_bytes = bfd_bwrite (guid_contents, guid_sec_size, abfd);
      BFD_ASSERT (written_bytes == guid_sec_size);

      free (guid_contents);
    }
  /* end sce local */
}

static int spu_plugin = 0;

void
spu_elf_plugin (int val)
{
  spu_plugin = val;
}

/* Set ELF header e_type for plugins.  */

static void
spu_elf_post_process_headers (bfd *abfd,
			      struct bfd_link_info *info ATTRIBUTE_UNUSED)
{
  if (spu_plugin)
    {
      Elf_Internal_Ehdr *i_ehdrp = elf_elfheader (abfd);

      i_ehdrp->e_type = ET_DYN;
    }
}

/* We may add an extra PT_LOAD segment for .toe.  */

static int
spu_elf_additional_program_headers (bfd *abfd)
{
  int extra = 0;
  asection *sec;

  sec = bfd_get_section_by_name (abfd, ".toe");
  if (sec != NULL && (sec->flags & SEC_LOAD) != 0)
    ++extra;

  return extra;
}

/* Remove .toe section from other PT_LOAD segments and put it in
   a segment of its own.  */

static bfd_boolean
spu_elf_modify_segment_map (bfd *abfd, struct bfd_link_info *info)
{
  asection *toe, *s;
  struct elf_segment_map *m;
  unsigned int i;

  if (info == NULL)
    return TRUE;

  toe = bfd_get_section_by_name (abfd, ".toe");
  for (m = elf_tdata (abfd)->segment_map; m != NULL; m = m->next)
    if (m->p_type == PT_LOAD && m->count > 1)
      for (i = 0; i < m->count; i++)
	if ((s = m->sections[i]) == toe)
	  {
	    struct elf_segment_map *m2;
	    bfd_vma amt;

	    if (i + 1 < m->count)
	      {
		amt = sizeof (struct elf_segment_map);
		amt += (m->count - (i + 2)) * sizeof (m->sections[0]);
		m2 = bfd_zalloc (abfd, amt);
		if (m2 == NULL)
		  return FALSE;
		m2->count = m->count - (i + 1);
		memcpy (m2->sections, m->sections + i + 1,
			m2->count * sizeof (m->sections[0]));
		m2->p_type = PT_LOAD;
		m2->next = m->next;
		m->next = m2;
	      }
	    m->count = 1;
	    if (i != 0)
	      {
		m->count = i;
		amt = sizeof (struct elf_segment_map);
		m2 = bfd_zalloc (abfd, amt);
		if (m2 == NULL)
		  return FALSE;
		m2->p_type = PT_LOAD;
		m2->count = 1;
		m2->sections[0] = s;
		m2->next = m->next;
		m->next = m2;
	      }
	    break;
	  }

  return TRUE;
}

/* Check that all loadable section VMAs lie in the range
   LO .. HI inclusive.  */

asection *
spu_elf_check_vma (bfd *abfd, bfd_vma lo, bfd_vma hi)
{
  struct elf_segment_map *m;
  unsigned int i;

  for (m = elf_tdata (abfd)->segment_map; m != NULL; m = m->next)
    if (m->p_type == PT_LOAD)
      for (i = 0; i < m->count; i++)
	if (m->sections[i]->size != 0
	    && (m->sections[i]->vma < lo
		|| m->sections[i]->vma > hi
		|| m->sections[i]->vma + m->sections[i]->size - 1 > hi))
	  return m->sections[i];

  return NULL;
}

/* Tweak the section type of .note.spu_name.  */

static bfd_boolean
spu_elf_fake_sections (bfd *obfd ATTRIBUTE_UNUSED,
		       Elf_Internal_Shdr *hdr,
		       asection *sec)
{
  if (strcmp (sec->name, SPU_PTNOTE_SPUNAME) == 0)
    hdr->sh_type = SHT_NOTE;
  return TRUE;
}

/* Tweak phdrs before writing them out.  */

static int
spu_elf_modify_program_headers (bfd *abfd, struct bfd_link_info *info)
{
  const struct elf_backend_data *bed;
  struct elf_obj_tdata *tdata;
  Elf_Internal_Phdr *phdr, *last;
  unsigned int count;
  unsigned int i;

  if (info == NULL)
    return TRUE;

  bed = get_elf_backend_data (abfd);
  tdata = elf_tdata (abfd);
  phdr = tdata->phdr;
  count = tdata->program_header_size / bed->s->sizeof_phdr;

  /* Round up p_filesz and p_memsz of PT_LOAD segments to multiples
     of 16.  This should always be possible when using the standard
     linker scripts, but don't create overlapping segments if
     someone is playing games with linker scripts.  */
  last = NULL;
  for (i = count; i-- != 0; )
    if (phdr[i].p_type == PT_LOAD)
      {
	unsigned adjust;

	adjust = -phdr[i].p_filesz & 15;
	if (adjust != 0
	    && last != NULL
	    && phdr[i].p_offset + phdr[i].p_filesz > last->p_offset - adjust)
	  break;

	adjust = -phdr[i].p_memsz & 15;
	if (adjust != 0
	    && last != NULL
	    && phdr[i].p_filesz != 0
	    && phdr[i].p_vaddr + phdr[i].p_memsz > last->p_vaddr - adjust
	    && phdr[i].p_vaddr + phdr[i].p_memsz <= last->p_vaddr)
	  break;

	if (phdr[i].p_filesz != 0)
	  last = &phdr[i];
      }

  if (i == (unsigned int) -1)
    for (i = count; i-- != 0; )
      if (phdr[i].p_type == PT_LOAD)
	{
	unsigned adjust;

	adjust = -phdr[i].p_filesz & 15;
	phdr[i].p_filesz += adjust;

	adjust = -phdr[i].p_memsz & 15;
	phdr[i].p_memsz += adjust;
      }

  return TRUE;
}


static bfd_boolean
spu_elf_section_processing (bfd * abfd, Elf_Internal_Shdr * i_shdrp)
{
  asection *sec = i_shdrp->bfd_section;

  /* begin sce local, bugzilla #2878 */
  /* If the section will loaded, we compute the SPU GUID from the section. */
  if (sec != NULL
      && ((sec->flags & (SEC_LINKER_CREATED | SEC_HAS_CONTENTS | SEC_LOAD | SEC_ALLOC))
	  == (SEC_HAS_CONTENTS | SEC_LOAD | SEC_ALLOC))
      && strcmp (sec->name, ".SpuGUID") != 0)
    {
      bfd_size_type sec_size, cnt;
      bfd_byte *sec_contents;

      sec_size = bfd_get_section_size (i_shdrp->bfd_section);

      sec_contents = bfd_malloc (sec_size);
      if (sec_contents == NULL)
	return FALSE;

      /** to avoid read after write immediately (bugzilla #21381) **/
      real_fseek (bfd_cache_lookup (abfd, CACHE_NORMAL), 0, SEEK_CUR);

      if (bfd_get_section_contents (abfd, sec, sec_contents, 0, sec_size) == FALSE)
        {
          free (sec_contents);
          return FALSE;
        }

      /** to avoid write after read immediately (bugzilla #21381) **/
      real_fseek (bfd_cache_lookup (abfd, CACHE_NORMAL), 0, SEEK_CUR);

      /* Cumulate hash value from the section contents. */
      /* FIXME: Does this hash function behave well? */
      for (cnt = 0; cnt < sec_size; ++cnt)
	spu_guid = spu_guid * 173 + (spu_guid >> 19) + sec_contents[cnt];

      free (sec_contents);
    }
  /* end sce local */
  return TRUE;
}

/*
 * Added at SCE to avoid gdb problems when the -q linker switch is used as is done by SPURS. We should not be 
 * "relocating" non-allocated debugger sections.  Its questionable why the linker creates .rela sections for these 
 * in the first place but we'll leave that possible fix for this for another day.
 */
static bfd_byte *
_bfd_elf_spu_get_relocated_section_contents (bfd *abfd, struct bfd_link_info *link_info,
				struct bfd_link_order *link_order, bfd_byte *data, bfd_boolean relocatable,
				asymbol **symbols)
{
  asection *section = link_order->u.indirect.section;

  /** to avoid read after write immediately (bugzilla #21381) **/
  real_fseek (bfd_cache_lookup (abfd, CACHE_NORMAL), 0, SEEK_CUR);

  if ((section->flags & (SEC_DEBUGGING | SEC_ALLOC)) != SEC_DEBUGGING)
    return bfd_generic_get_relocated_section_contents(abfd, link_info, link_order, data, relocatable, symbols);

  bfd_get_section_contents (abfd, section, data, 0, section->rawsize ? section->rawsize : section->size);

  /** to avoid write after read immediately (bugzilla #21381) **/
  real_fseek (bfd_cache_lookup (abfd, CACHE_NORMAL), 0, SEEK_CUR);

  return data;
}

static bfd_boolean
spu_elf_create_dynamic_sections (bfd *abfd, struct bfd_link_info *info)
{
#ifdef DEBUG
  info->callbacks->info ( _("spu_elf_create_dynamic_sections called for %B\n"), abfd);
#endif

  if (info->executable)
    {
      /* We don't need the .interp section, or PHDR and INTERP segments. */
      asection *sec = bfd_get_section_by_name (abfd, ".interp");
      if (sec)
	sec->flags = SEC_EXCLUDE;
    }

 /* Normally call _bfd_elf_create_dynamic_sections but we don't want the
  * basic sections (.plt, .got, etc.) for SPU. */
  return TRUE;
}

/* Copy the extra info we tack onto an elf_link_hash_entry.  */

static void
spu_elf_copy_indirect_symbol (struct bfd_link_info *info,
			      struct elf_link_hash_entry *dir,
			      struct elf_link_hash_entry *ind)
{
  struct spu_elf_link_hash_entry *edir, *eind;

  edir = (struct spu_elf_link_hash_entry *) dir;
  eind = (struct spu_elf_link_hash_entry *) ind;

  if (eind->dyn_relocs != NULL)
    {
      if (edir->dyn_relocs != NULL)
	{
	  struct spu_elf_dyn_relocs **pp;
	  struct spu_elf_dyn_relocs *p;

	  /* Add reloc counts against the indirect sym to the direct sym
	     list.  Merge any entries against the same section.  */
	  for (pp = &eind->dyn_relocs; (p = *pp) != NULL; )
	    {
	      struct spu_elf_dyn_relocs *q;

	      for (q = edir->dyn_relocs; q != NULL; q = q->next)
		if (q->sec == p->sec)
		  {
		    q->count += p->count;
		    q->pie_count += p->pie_count;
		    *pp = p->next;
		    break;
		  }
	      if (q == NULL)
		pp = &p->next;
	    }
	  *pp = edir->dyn_relocs;
	}

      edir->dyn_relocs = eind->dyn_relocs;
      eind->dyn_relocs = NULL;
    }

   _bfd_elf_link_hash_copy_indirect (info, &edir->elf, &eind->elf);
}

/* Look through the relocs for a section during the first phase, and
   allocate space in the global offset table or procedure linkage
   table.  */

static bfd_boolean
spu_elf_check_relocs (bfd *abfd,
		      struct bfd_link_info *info,
		      asection *sec,
		      const Elf_Internal_Rela *relocs)
{
  struct spu_link_hash_table *htab = spu_hash_table (info);
  Elf_Internal_Shdr *symtab_hdr;
  struct elf_link_hash_entry **sym_hashes;
  const Elf_Internal_Rela *rel;
  const Elf_Internal_Rela *rel_end;
  asection *sreloc;
  int num_glob_dats;
  struct spu_elf_fixup* fixups = 0;

  if (htab->init_ctors == NULL
      && strcmp (sec->name, ".init.ctors") == 0)
    htab->init_ctors = sec;
  if (htab->fini_dtors == NULL
      && strcmp (sec->name, ".fini.dtors") == 0)
    htab->fini_dtors = sec;

  if (info->relocatable)
    return TRUE;

  /* Create dynamic sections in an executable with --export-dynamic */
  if (info->executable && info->export_dynamic
      && ! htab->elf.dynamic_sections_created)
    {
      if (! _bfd_elf_link_create_dynamic_sections (abfd, info))
	return FALSE;
    }

  /* There's not much point in propagating relocs to shared libs that
     the dynamic linker won't relocate.  */
  if ((sec->flags & SEC_ALLOC) == 0)
    return TRUE;

#ifdef DEBUG
  info->callbacks->info ( _("spu_elf_check_relocs called for section %A in %B\n"),
		      sec, abfd);
#endif

  symtab_hdr = &elf_tdata (abfd)->symtab_hdr;
  sym_hashes = elf_sym_hashes (abfd);
  sreloc = NULL;

  /* Count the number of R_SPU_GLOB_DAT relocations and allocate space
   * to record their offsets.  This is used to size .fixup. */
  num_glob_dats = 0;
  rel_end = relocs + sec->reloc_count;
  for (rel = relocs; rel < rel_end; rel++)
    if (ELF32_R_TYPE (rel->r_info) == R_SPU_GLOB_DAT)
      num_glob_dats++;
  if (num_glob_dats > 0)
    fixups = elf_section_data (sec)->local_dynrel = bfd_zalloc (abfd, (num_glob_dats+1) * sizeof(struct spu_elf_fixup));

  rel_end = relocs + sec->reloc_count;
  for (rel = relocs; rel < rel_end; rel++)
    {
      unsigned long r_symndx;
      enum elf_spu_reloc_type r_type;
      struct elf_link_hash_entry *h;

      r_symndx = ELF32_R_SYM (rel->r_info);
      if (r_symndx < symtab_hdr->sh_info)
	h = NULL;
      else
	{
	  h = sym_hashes[r_symndx - symtab_hdr->sh_info];
	  while (h->root.type == bfd_link_hash_indirect
		 || h->root.type == bfd_link_hash_warning)
	    h = (struct elf_link_hash_entry *) h->root.u.i.link;
	}

      r_type = ELF32_R_TYPE (rel->r_info);
      switch (r_type)
	{
	case R_SPU_PPU32:
	case R_SPU_PPU64:
	  sec->flags |= SEC_KEEP;
	  break;

	  /* The following relocations don't need to propagate the
	     relocation if linking a shared object since they are
	     section relative.  */
	case R_SPU_REL9:
	case R_SPU_REL9I:
	  break;

	  /* These are just markers.  */
	case R_SPU_NONE:
	case R_SPU_max:
	  break;

	case R_SPU_ADD_PIC:
	  break;

	case R_SPU_REL32:
	case R_SPU_REL16:
	case R_SPU_GLOB_DAT:
	case R_SPU_ADDR18:
	case R_SPU_ADDR16:
	case R_SPU_ADDR16_LO:
	case R_SPU_ADDR16_HI:
	case R_SPU_ADDR10:
	case R_SPU_ADDR7:
	case R_SPU_ADDR10I:
	case R_SPU_ADDR16I:
	case R_SPU_ADDR16X:

	  /* If we are creating a shared library, and this is a reloc
	     against a global symbol, or a non PC relative reloc
	     against a local symbol, then we need to copy the reloc
	     into the shared library.  However, if we are linking with
	     -Bsymbolic, we do not need to copy a reloc against a
	     global symbol which is defined in an object we are
	     including in the link (i.e., DEF_REGULAR is set).  At
	     this point we have not seen all the input files, so it is
	     possible that DEF_REGULAR is not set now but will be set
	     later (it is never cleared).  In case of a weak definition,
	     DEF_REGULAR may be cleared later by a strong definition in
	     a shared library.  We account for that possibility below by
	     storing information in the dyn_relocs field of the hash
	     table entry.  A similar situation occurs when creating
	     shared libraries and symbol visibility changes render the
	     symbol local.

	     If on the other hand, we are creating an executable, we
	     may need to keep relocations for symbols satisfied by a
	     dynamic library if we manage to avoid copy relocs for the
	     symbol.  */
	  if (needs_dynreloc (info, h, 0))
	    {
	      struct spu_elf_dyn_relocs *p;
	      struct spu_elf_dyn_relocs **head;

#ifdef DEBUG
	      info->callbacks->info (
		       _("spu_elf_check_relocs needs to "
		       "create relocation for %s\n"),
		       (h && h->root.root.string
			? h->root.root.string : "<unknown>"));
#endif
	      if (sreloc == NULL)
		{
		  const char *name;

		  name = (bfd_elf_string_from_elf_section
			  (abfd,
			   elf_elfheader (abfd)->e_shstrndx,
			   elf_section_data (sec)->rel_hdr.sh_name));
		  if (name == NULL)
		    return FALSE;

		  BFD_ASSERT (strncmp (name, ".rela", 5) == 0
			      && strcmp (bfd_get_section_name (abfd, sec),
					 name + 5) == 0);

		  if (htab->elf.dynobj == NULL)
		    {
		      htab->elf.dynobj = abfd;
		    }
		  sreloc = bfd_get_section_by_name (htab->elf.dynobj, name);
		  if (sreloc == NULL)
		    {
		      flagword flags;

		      flags = (SEC_HAS_CONTENTS | SEC_READONLY
			       | SEC_IN_MEMORY | SEC_LINKER_CREATED
			       | SEC_ALLOC | SEC_LOAD);
		      sreloc = bfd_make_section_with_flags (htab->elf.dynobj,
							    name,
							    flags);
		      if (sreloc == NULL
			  || ! bfd_set_section_alignment (htab->elf.dynobj,
							  sreloc, 2))
			return FALSE;
		    }
		  elf_section_data (sec)->sreloc = sreloc;
		}

	      /* Count the number of relocations we need for this
	       * symbol.  */
	      if (h)
		{
		  head = &spu_elf_hash_entry (h)->dyn_relocs;
		  p = *head;
		  if (p == NULL || p->sec != sec)
		    {
		      p = bfd_alloc (htab->elf.dynobj, sizeof *p);
		      if (p == NULL)
			return FALSE;
		      p->next = *head;
		      *head = p;
		      p->sec = sec;
		      p->count = 0;
		      p->pie_count = 0;
		    }
		  p->count += 1;
		  if (r_type == R_SPU_GLOB_DAT)
		    p->pie_count += 1;
		}

	    }
	  /* Record all R_SPU_GLOB_DAT relocations as a possible fixup.
	     We record a reference to the symbol.  If it ends up being a
	     dynamic symbol we will not create a fixup for it. */
	  if (htab->emit_fixups && r_type == R_SPU_GLOB_DAT)
	    {
	      fixups->h = h ? h : (struct elf_link_hash_entry *)-1;
	      fixups->r_offset = rel->r_offset;
	      fixups++;
	    }

	  break;
	}
#ifdef DEBUG
      {
      unsigned long r_symndx = ELF32_R_SYM (rel->r_info);
      reloc_howto_type *howto = elf_howto_table + r_type;
      info->callbacks->info (_( "\ttype = %s (%d), name = %s, symbol index = %ld, "
	       "offset = %ld, addend = %ld\n"),
	       howto->name,
	       (int) r_type,
	       (h && h->root.root.string ? h->root.root.string : "<unknown>"),
	       r_symndx,
	       (long) rel->r_offset,
	       (long) rel->r_addend);
      }
#endif
    }

  return TRUE;
}

static bfd_boolean
spu_elf_finish_dynamic_sections (bfd *abfd, struct bfd_link_info *info)
{
  (void)abfd;
  (void)info;
#ifdef DEBUG
  info->callbacks->info (_("spu_elf_finish_dynamic_sections called\n"));
#endif
  return TRUE;
}

static bfd_boolean
spu_elf_finish_dynamic_symbol (
     bfd *output_bfd,
     struct bfd_link_info *info,
     struct elf_link_hash_entry *h,
     Elf_Internal_Sym *sym)
{
  (void)output_bfd;
  (void)info;
  (void)h;
  (void)sym;
#ifdef DEBUG
  info->callbacks->info (_("spu_elf_finish_dynamic_symbol called for %s vis %ld\n"),
	   h->root.root.string, ELF_ST_VISIBILITY (h->other));
#endif
  return TRUE;
}

/* Adjust a symbol defined by a dynamic object and referenced by a
   regular object.  The current definition is in some section of the
   dynamic object, but we're not including those sections.  We have to
   change the definition to something the rest of the link can
   understand.  */

static bfd_boolean
spu_elf_adjust_dynamic_symbol (struct bfd_link_info *info,
			       struct elf_link_hash_entry *h)
{
  (void)info;
  (void)h;

#ifdef DEBUG
  info->callbacks->info (_("spu_elf_adjust_dynamic_symbol called for %s\n"),
	   h->root.root.string);
#endif

  return TRUE;
}
/* Allocate space in associated reloc sections for dynamic relocs.  */

static bfd_boolean
allocate_dynrelocs (struct elf_link_hash_entry *h, void *inf)
{
  struct bfd_link_info *info = inf;
  struct spu_elf_link_hash_entry *eh;
  struct spu_link_hash_table *htab;
  struct spu_elf_dyn_relocs *p;

  if (h->root.type == bfd_link_hash_indirect)
    return TRUE;

  if (h->root.type == bfd_link_hash_warning)
    /* When warning symbols are created, they **replace** the "real"
       entry in the hash table, thus we never get to see the real
       symbol in a hash traversal.  So look at it now.  */
    h = (struct elf_link_hash_entry *) h->root.u.i.link;

  htab = spu_hash_table (info);

  eh = (struct spu_elf_link_hash_entry *) h;
  eh->elf.got.offset = (bfd_vma) -1;

  if (eh->dyn_relocs == NULL)
    return TRUE;

  /* In the shared -Bsymbolic case, discard space allocated for
     dynamic pc-relative relocs against symbols which turn out to be
     defined in regular objects.  For the normal shared case, discard
     space for relocs that have become local due to symbol visibility
     changes.  

     Also discard relocs on undefined weak syms with non-default
     visibility.  */

  if (needs_dynreloc (info, h, 1))
    {
      if (! bfd_elf_link_record_dynamic_symbol (info, h))
	return FALSE;

      /* Finally, allocate space.  */
      for (p = eh->dyn_relocs; p != NULL; p = p->next)
	{
	  asection *sreloc = elf_section_data (p->sec)->sreloc;
	  sreloc->size += p->count * sizeof (Elf32_External_Rela);
	}
    }
  else
    eh->dyn_relocs = NULL;

  return TRUE;
}

/* Find any dynamic relocs that apply to read-only sections.  */

static bfd_boolean
readonly_dynrelocs (struct elf_link_hash_entry *h, void *info)
{
  struct spu_elf_dyn_relocs *p;

  if (h->root.type == bfd_link_hash_indirect)
    return TRUE;

  if (h->root.type == bfd_link_hash_warning)
    h = (struct elf_link_hash_entry *) h->root.u.i.link;

  for (p = spu_elf_hash_entry (h)->dyn_relocs; p != NULL; p = p->next)
    {
      asection *s = p->sec->output_section;

      if (p->count 
	  && s != NULL
	  && ((s->flags & (SEC_READONLY | SEC_ALLOC))
	      == (SEC_READONLY | SEC_ALLOC)))
	{
	  ((struct bfd_link_info *) info)->flags |= DF_TEXTREL;

	  /* Not an error, just cut short the traversal.  */
	  return FALSE;
	}
    }
  return TRUE;
}

struct section_size_data {
  const char *name;
  int len;
  int count;
};
static void
count_sections (bfd *abfd ATTRIBUTE_UNUSED,
		asection *section, void *data)
{
  struct section_size_data *s = data;
  if ((section->flags & SEC_EXCLUDE)
      || strncmp (section->name, s->name, s->len) != 0)
    return;
  if (section->name[s->len] == 0 || section->name[s->len] == '.')
    s->count++;
}

static void
exclude_section (bfd *abfd ATTRIBUTE_UNUSED,
		 asection *section, void *data)
{
  char *name = data;
  if (strcmp (section->name, name) == 0)
    {
      section->flags |= SEC_EXCLUDE;
      section->output_section = bfd_abs_section_ptr;
    }
}

/* Count the number of input sections that start with NAME, not
 * including sections that end with "_head" or "_tail".  When the count
 * is 0, exclude the input sections in EXCLUDE0 and EXCLUDE1. */
static void
strip_crt_sections (struct bfd_link_info *info, const char *name,
		    int len, const char *exclude0, const char *exclude1)
{
  struct section_size_data ssd;
  bfd *ibfd;
  ssd.name = name;
  ssd.len = len;
  ssd.count = 0;
  for (ibfd = info->input_bfds; ibfd != NULL; ibfd = ibfd->link_next)
    bfd_map_over_sections (ibfd, count_sections, &ssd);
  if (ssd.count == 0)
    {
      for (ibfd = info->input_bfds; ibfd != NULL; ibfd = ibfd->link_next)
	bfd_map_over_sections (ibfd, exclude_section, (void *)exclude0);
      if (exclude1)
	for (ibfd = info->input_bfds; ibfd != NULL; ibfd = ibfd->link_next)
	  bfd_map_over_sections (ibfd, exclude_section, (void *)exclude1);
    }
}

bfd_boolean
spu_elf_size_sections (bfd * output_bfd ATTRIBUTE_UNUSED,
		       struct bfd_link_info *info)
{
  struct spu_link_hash_table *htab;
#ifdef DEBUG
  info->callbacks->info (_("spu_elf_size_sections called\n"));
#endif
  htab = spu_hash_table (info);
  if (htab->emit_fixups)
    {
      asection *sfixup = htab->sfixup;
      asection *s;
      struct spu_elf_fixup *p;
      int fixup_count = 0;
      int i;
      bfd *ibfd;
      bfd_vma base_end;

      for (ibfd = info->input_bfds; ibfd != NULL; ibfd = ibfd->link_next)
	{

	  if (bfd_get_flavour (ibfd) != bfd_target_elf_flavour)
	    continue;

	  /* Count an upper bound for the size of .fixup.   It is an
	   * upper bound because this will not merge fixups of different
	   * sections into a single fixup record.  */
	  for (s = ibfd->sections; s != NULL; s = s->next)
	    if ((p = elf_section_data (s)->local_dynrel) != 0)
	      for (base_end = 0, i = 0; p[i].h; i++)
		if ((p[i].h == (struct elf_link_hash_entry *) -1
		     || (!spu_elf_hash_entry (p[i].h)->dyn_relocs
			 && strncmp (p[i].h->root.root.string, "__ABS__", 7)))
		    && p[i].r_offset >= base_end)
		  {
		    base_end = (p[i].r_offset & ~(bfd_vma) 15) + 16;
		    fixup_count++;
#ifdef DEBUG
		    info->callbacks->info (_("  count fixup for %s, sec %A in %B\n"),
					   (p[i].h
					    && p[i].h != (struct elf_link_hash_entry *) -1
					    && p[i].h->root.root.string
					    ? p[i].h->root.root.string
					    : "<unknown>"),
					   s, ibfd);
#endif
		  }
	}

      if (fixup_count == 0)
	{
	  /* Remove the .init.fixups section when there are no fixups. */
	  for (ibfd = info->input_bfds; ibfd != NULL; ibfd = ibfd->link_next)
	    {
	      bfd_map_over_sections (ibfd, exclude_section, ".init.fixups");
	      bfd_map_over_sections (ibfd, exclude_section, ".fixup_head");
	    }
	  sfixup->flags |= SEC_EXCLUDE;
	}
      else
	{
	  /* We always have a NULL fixup as a sentinel */
	  sfixup->size = (fixup_count + 1) * FIXUP_RECORD_SIZE;
	  sfixup->contents =
	    (bfd_byte *) bfd_zalloc (info->input_bfds, sfixup->size);
	  if (sfixup->contents == NULL)
	    return FALSE;
	}
    }

  if (htab->strip_crt)
    {
      strip_crt_sections (info, ".ctors", 6, ".init.ctors", NULL);
      strip_crt_sections (info, ".dtors", 6, ".fini.dtors", NULL);
    }
  return TRUE;
}

static bfd_boolean
spu_elf_size_dynamic_sections (bfd *output_bfd,
			       struct bfd_link_info *info)
{
  struct spu_link_hash_table *htab;
  bfd *dynobj;
  asection *sec;
  bfd_boolean relocs;

#ifdef DEBUG
  info->callbacks->info (_("spu_elf_size_dynamic_sections called\n"));
#endif

  htab = spu_hash_table (info);
  dynobj = htab->elf.dynobj;
  if (dynobj == NULL)
    abort ();

  /* Allocate space for global sym dynamic relocs.  */
  elf_link_hash_traverse (&htab->elf, allocate_dynrelocs, info);

  /* The check_relocs and adjust_dynamic_symbol entry points have
     determined the sizes of the various dynamic sections.  Allocate
     memory for them.  */
  relocs = FALSE;
  for (sec = dynobj->sections; sec != NULL; sec = sec->next)
    {
      if ((sec->flags & SEC_LINKER_CREATED) == 0)
	continue;

      if (strncmp (bfd_get_section_name (dynobj, sec), ".rela", 5) == 0)
	{
	  if (sec->size != 0)
	    {
	      /* Remember whether there are any reloc sections.  */
	      relocs = TRUE;

	      /* We use the reloc_count field as a counter if we need
		 to copy relocs into the output file.  */
	      sec->reloc_count = 0;
	    }
	}
      else
	{
	  /* It's not one of our sections, so don't allocate space.  */
	  continue;
	}

      if (sec->size == 0)
	{
	  /* If we don't need this section, strip it from the
	     output file.  This is mostly to handle .rela.bss and
	     .rela.plt.  We must create both sections in
	     create_dynamic_sections, because they must be created
	     before the linker maps input sections to output
	     sections.  The linker does that before
	     adjust_dynamic_symbol is called, and it is that
	     function which decides whether anything needs to go
	     into these sections.  */
	  sec->flags |= SEC_EXCLUDE;
	  continue;
	}

      if ((sec->flags & SEC_HAS_CONTENTS) == 0)
	continue;

      /* Allocate memory for the section contents.  Zero it, because
	 we may not fill in all the reloc sections.  */
      sec->contents = bfd_zalloc (dynobj, sec->size);
      if (sec->contents == NULL)
	return FALSE;
    }

  if (htab->elf.dynamic_sections_created)
    {
#define add_dynamic_entry(TAG, VAL) \
  _bfd_elf_add_dynamic_entry (info, TAG, VAL)

      if (relocs)
	{
	  if (!add_dynamic_entry (DT_RELA, 0)
	      || !add_dynamic_entry (DT_RELASZ, 0)
	      || !add_dynamic_entry (DT_RELAENT, sizeof (Elf32_External_Rela)))
	    return FALSE;

	  /* If any dynamic relocs apply to a read-only section,
	     then we need a DT_TEXTREL entry.  */
	  if ((info->flags & DF_TEXTREL) == 0)
	    elf_link_hash_traverse (&htab->elf, readonly_dynrelocs, info);

	  if ((info->flags & DF_TEXTREL) != 0)
	    {
	      if (!add_dynamic_entry (DT_TEXTREL, 0))
		return FALSE;
	    }
	}
      /* Align all the dynamic sections to 16 bytes */
      sec = bfd_get_section_by_name (output_bfd, ".dynamic");
      if (sec)
	  bfd_set_section_alignment (output_bfd, sec, 4);
      sec = bfd_get_section_by_name (output_bfd, ".hash");
      if (sec)
	  bfd_set_section_alignment (output_bfd, sec, 4);
      sec = bfd_get_section_by_name (output_bfd, ".dynsym");
      if (sec)
	  bfd_set_section_alignment (output_bfd, sec, 4);
      sec = bfd_get_section_by_name (output_bfd, ".dynstr");
      if (sec)
	  bfd_set_section_alignment (output_bfd, sec, 4);
      sec = bfd_get_section_by_name (output_bfd, ".rela.dyn");
      if (sec)
	  bfd_set_section_alignment (output_bfd, sec, 4);
    }
#undef add_dynamic_entry

  return TRUE;
}

#define TARGET_BIG_SYM		bfd_elf32_spu_vec
#define TARGET_BIG_NAME		"elf32-spu"
#define ELF_ARCH		bfd_arch_spu
#define ELF_MACHINE_CODE	EM_SPU
/* This matches the alignment need for DMA.  */
#define ELF_MAXPAGESIZE		0x80
#define elf_backend_rela_normal         1
#define elf_backend_can_gc_sections	1

#define bfd_elf32_bfd_reloc_type_lookup		spu_elf_reloc_type_lookup
#define elf_info_to_howto			spu_elf_info_to_howto
#define elf_backend_count_relocs		spu_elf_count_relocs
#define elf_backend_relocate_section		spu_elf_relocate_section
#define elf_backend_final_write_processing      spu_elf_final_write_processing
#define elf_backend_gc_mark_hook		spu_elf_gc_mark_hook
#define bfd_elf32_new_section_hook		spu_elf_new_section_hook
#define bfd_elf32_bfd_link_hash_table_create	spu_elf_link_hash_table_create
#define bfd_elf32_bfd_link_hash_table_free	spu_elf_link_hash_table_free

#define elf_backend_additional_program_headers	spu_elf_additional_program_headers
#define elf_backend_modify_segment_map		spu_elf_modify_segment_map
#define elf_backend_modify_program_headers	spu_elf_modify_program_headers
#define elf_backend_post_process_headers        spu_elf_post_process_headers
#define elf_backend_fake_sections		spu_elf_fake_sections
#define elf_backend_special_sections		spu_elf_special_sections
#define bfd_elf32_bfd_final_link		spu_elf_final_link
#define elf_backend_section_processing          spu_elf_section_processing
#define bfd_elf32_bfd_get_relocated_section_contents _bfd_elf_spu_get_relocated_section_contents

#define elf_backend_create_dynamic_sections	spu_elf_create_dynamic_sections
#define elf_backend_finish_dynamic_symbol	spu_elf_finish_dynamic_symbol
#define elf_backend_finish_dynamic_sections	spu_elf_finish_dynamic_sections
#define elf_backend_check_relocs		spu_elf_check_relocs
#define elf_backend_adjust_dynamic_symbol	spu_elf_adjust_dynamic_symbol
#define elf_backend_size_dynamic_sections	spu_elf_size_dynamic_sections
#define elf_backend_copy_indirect_symbol	spu_elf_copy_indirect_symbol

#include "elf32-target.h"
