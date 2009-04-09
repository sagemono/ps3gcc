/* BFD back-end for ppc64_spu objects.
   Copyright (C) 2008 Sony Computer Entertainment, Inc.

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
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.  */

/* This back-end maps an SPU object to a PPC64 object file for reading
 * only.  
 *
 * 1) SEC_ALLOC sections are mapped to a single .data section as if the
 *    user had done
 *      objcopy -O binary -I elf32-spu -B powerpc  <file> tmp.bin
 *      objcopy -O elf64-powerpc -I binary tmp.bin <file>
 * 2) _binary_<file>_start, _binary_<file>_end, and _binary_<file>_size
 *    symbols are generated at the boundaries of this .data section
 * 3) _binary_start, _binary_end, and _binary_size symbols are generated
 *    with local binding at the boundaries of this .data section
 * 4) R_SPU_PPC32 and R_SPU_PPC64 relocations are mapped to the
 *    R_PPC64_ADDR32 and R_PPC64_ADDR64 relocations in .data.rela.
 *
 * TODO: Use program header alignment to set alignment of the .data
 * section.
 *
 * TODO: Include the .bss section as zeroed contents when the
 * __init_zerobss symbol is not defined.  __init_zerobss will be defined
 * when a standard crt file is included that initializes the .bss
 * section to 0.
 *
 * TODO: Some programming models expect the whole SPU ELF file (headers
 * and all) to be in the data section.  Perhaps this can be triggered
 * off of entry point names.  
 *
 * TODO: Allow for an easier way to specify an alternate prefix for the
 * *_start, *_end, and *_size symbols.  The current method is to use
 * objcopy --redefine-sym.
 *
 * TODO:  It is possible to expose all STB_GLOBAL/STV_DEFAULT symbols in
 * the SPU object as _<some_prefix>_<symbol>, but that would bloat the
 * symbol table.  Create a new bind type to export SPU symbols to PPU.
 *
 * TODO: When the SPU object does not have section headers, use program
 * headers to do parts (1) and (2).
 *
 * TODO: What about SPU objects that are DYNAMIC?  For now, it doesn't
 * seem there is any use for handling them here.
 *
 * TODO: Should the SPU Name Note section be included?  It seems it is
 * just for debugging
 *
 * TODO: Create a .gnu_debuglink section too.  Or, map all of the debug
 * sections to .spu.<GUID>.<debug_section>.
 *
 * TODO: When the elf contains sections named .ppu.<section>, copy that
 * to a PPU section named .<section>.  This allows an SPU to define
 * sections to be included in the PPU file.
 */

#include "bfd.h"
#include "sysdep.h"
#include "safe-ctype.h"
#include "libbfd.h"
#include "elf-bfd.h"
#include "elf/ppc64.h"
#include "elf/spu.h"

#define ONES(n) (((bfd_vma) 1 << ((n) - 1) << 1) - 1)

static reloc_howto_type ppc64_spu_howto_raw[] = {
  /* This reloc does nothing.  */
  HOWTO (R_PPC64_NONE,		/* type */
	 0,			/* rightshift */
	 2,			/* size (0 = byte, 1 = short, 2 = long) */
	 32,			/* bitsize */
	 FALSE,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_dont, /* complain_on_overflow */
	 bfd_elf_generic_reloc,	/* special_function */
	 "R_PPC64_NONE",	/* name */
	 FALSE,			/* partial_inplace */
	 0,			/* src_mask */
	 0,			/* dst_mask */
	 FALSE),		/* pcrel_offset */

  /* A standard 32 bit relocation.  */
  HOWTO (R_PPC64_ADDR32,	/* type */
	 0,			/* rightshift */
	 2,			/* size (0 = byte, 1 = short, 2 = long) */
	 32,			/* bitsize */
	 FALSE,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_bitfield, /* complain_on_overflow */
	 bfd_elf_generic_reloc,	/* special_function */
	 "R_PPC64_ADDR32",	/* name */
	 FALSE,			/* partial_inplace */
	 0,			/* src_mask */
	 0xffffffff,		/* dst_mask */
	 FALSE),		/* pcrel_offset */

  /* A standard 64-bit relocation.  */
  HOWTO (R_PPC64_ADDR64,	/* type */
	 0,			/* rightshift */
	 4,			/* size (0=byte, 1=short, 2=long, 4=64 bits) */
	 64,			/* bitsize */
	 FALSE,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_dont, /* complain_on_overflow */
	 bfd_elf_generic_reloc,	/* special_function */
	 "R_PPC64_ADDR64",	/* name */
	 FALSE,			/* partial_inplace */
	 0,			/* src_mask */
	 ONES (64),		/* dst_mask */
	 FALSE),		/* pcrel_offset */

};

static reloc_howto_type *
ppc64_spu_reloc_type_lookup (bfd * abfd ATTRIBUTE_UNUSED,
			     bfd_reloc_code_real_type code)
{
  unsigned int r;

  switch (code)
    {
    default:
      return NULL;
    case BFD_RELOC_NONE:
      r = 0;
      break;
    case BFD_RELOC_32:
      r = 1;
      break;
    case BFD_RELOC_64:
      r = 2;
      break;
    }
  return &ppc64_spu_howto_raw[r];
};

static void
ppc64_spu_info_to_howto (bfd *abfd ATTRIBUTE_UNUSED, arelent *cache_ptr,
			 Elf_Internal_Rela *dst)
{
  unsigned int r;
  unsigned int type;

  type = ELF64_R_TYPE (dst->r_info);
  switch (type)
    {
    default:
      (*_bfd_error_handler) (_("%B: invalid relocation type %d"),
			     abfd, (int) type);
      r = 0;
      break;
    case R_PPC64_NONE:
      r = 0;
      break;
    case R_PPC64_ADDR32:
      r = 1;
      break;
    case R_PPC64_ADDR64:
      r = 2;
      break;
    }
  cache_ptr->howto = &ppc64_spu_howto_raw[r];
}

/* The default syms we create */
#define BIN_SYMS 3

struct ppc64_spu_tdata {
  struct elf_obj_tdata elf;
  bfd *spu_bfd;
  asymbol **spu_sympp;
  Elf_Internal_Shdr *hdrs;
  Elf_Internal_Sym *syms;
  bfd_vma align_power;
  bfd_vma low_lma;
  bfd_vma high_lma;
};

#define DATA_SH_INDEX		1
#define SHSTRTAB_SH_INDEX	2
#define SYMTAB_SH_INDEX		3
#define STRTAB_SH_INDEX		4
#define RELA_SH_INDEX		5

static char sym_prefix[] = "_binary_";

/* Create a ppc64_spu object.  Invoked via bfd_set_format.  */

static bfd_boolean
ppc64_spu_mkobject (bfd *abfd)
{
  abfd->tdata.any = bfd_zalloc (abfd, sizeof (struct ppc64_spu_tdata));
  return abfd->tdata.any != NULL;
}

static bfd_size_type
add_string (bfd *abfd, int strtab, const char *name0, const char *name1)
{
  struct ppc64_spu_tdata *ppc64_spu = abfd->tdata.any;
  Elf_Internal_Shdr *sec = &ppc64_spu->hdrs[strtab];
  int len0 = (name0 ? strlen (name0) : 0);
  int len1 = (name1 ? strlen (name1) : 0);
  bfd_size_type off = sec->sh_size;
  if (len0 + len1 == 0)
    return 0;
  /* When the name is already in the table, return its position. */
  if (sec->contents)
    {
      char *p0, *p1;
      bfd_size_type size = sec->sh_size;
      p0 = (char *)sec->contents;
      while (size > 0)
	{
	  p1 = memchr (p0, 0, size);
	  if (len0 + len1 == p1 - p0
	      && (len0 == 0 || memcmp (p0, name0, len0) == 0)
	      && (len1 == 0 || memcmp (p0 + len0, name1, len1) == 0))
	    return p0 - (char *)sec->contents;
	  size -= p1 - p0 + 1;
	  p0 = p1 + 1;
	}
    }
  if ((off & -256) != ((off + len0 + len1) & -256))
    sec->contents = bfd_realloc (sec->contents, ((off + len0 + len1) & -256) + 256);
  if (len0)
    memcpy (sec->contents + off, name0, len0);
  if (len1)
    memcpy (sec->contents + off + len0, name1, len1);
  sec->contents[off+len0+len1] = 0;
  sec->sh_size += len0 + len1 + 1;
  return off;
}

static int
add_symbol (bfd * abfd, bfd_vma value, bfd_vma size, const char *name0,
	    const char *name1, unsigned char bind, unsigned char type,
	    unsigned char other, unsigned int shndx, int lookup)
{
  struct ppc64_spu_tdata *ppc64_spu = abfd->tdata.any;
  Elf_Internal_Shdr *sec = &ppc64_spu->hdrs[SYMTAB_SH_INDEX];
  Elf_Internal_Sym *sym = ppc64_spu->syms;
  int strtab = type == STT_SECTION ? SHSTRTAB_SH_INDEX : STRTAB_SH_INDEX;
  bfd_size_type name = add_string (abfd, strtab, name0, name1);
  int num_syms = NUM_SHDR_ENTRIES (sec);
  int i = 0; /* ??? */

  /* Look for an existing symbol with the same name */
  if (name && lookup)
    for (i = 0; i < num_syms; i++, sym++)
      if (sym->st_name == name)
	return i;

  /* st_info is the index of the for non-STB_LOCAL symbol.  The first
     symbol of the section is always a dummy entry, so this must be
     greater than 1 if there is a non-local symbol. */
  if (sec->sh_info == 0 && bind != STB_LOCAL)
    sec->sh_info = i;

  if (i % 8 == 0)
    {
      ppc64_spu->syms = bfd_realloc (ppc64_spu->syms, (i + 8) * sizeof (Elf_Internal_Sym));
      sec->contents = (void *)ppc64_spu->syms;
    }
  sec->sh_size += sec->sh_entsize;
  sym = &ppc64_spu->syms[num_syms];
  sym->st_value = value;
  sym->st_size = size;
  sym->st_name = name;
  sym->st_info = ELF_ST_INFO (bind, type);
  sym->st_other = other;
  sym->st_shndx = shndx;
  return i;
}

static void
add_relocation (bfd *abfd, bfd_vma offset, const char *name, unsigned int rtype, bfd_vma addend)
{
  struct ppc64_spu_tdata *ppc64_spu = abfd->tdata.any;
  Elf_Internal_Shdr *sec = &ppc64_spu->hdrs[RELA_SH_INDEX];
  Elf_Internal_Rela *rela;
  int num_relas = NUM_SHDR_ENTRIES (sec);
  int sym;
  sym = add_symbol (abfd, 0, 0, name, 0, STB_GLOBAL, STT_NOTYPE, STV_DEFAULT, SHN_UNDEF, 1);
  if (num_relas % 8 == 0)
    sec->contents = bfd_realloc (sec->contents, (num_relas + 8) * sizeof (Elf_Internal_Rela));
  rela = &((Elf_Internal_Rela *)sec->contents)[num_relas];
  rela->r_offset = offset;
  rela->r_info = ELF64_R_INFO (sym, rtype);
  rela->r_addend = addend;
  sec->sh_size += sec->sh_entsize;
  elf_elfheader(abfd)->e_shnum = 6;
}

static void
alloc_section_bounds (bfd *abfd ATTRIBUTE_UNUSED,
		      asection *section, void *data)
{
  struct ppc64_spu_tdata *ppc64_spu = data;
  bfd_vma low, high;
  if ((section->flags & (SEC_ALLOC | SEC_EXCLUDE | SEC_HAS_CONTENTS | SEC_NEVER_LOAD | SEC_LOAD))
      != (SEC_ALLOC | SEC_HAS_CONTENTS | SEC_LOAD))
    return;
  low = section->lma;
  high = section->lma + section->size;
  if (section->alignment_power > ppc64_spu->align_power)
    ppc64_spu->align_power = section->alignment_power;
  if (ppc64_spu->high_lma == 0)
    {
      ppc64_spu->low_lma = low;
      ppc64_spu->high_lma = high;
    }
  else if (high > ppc64_spu->high_lma)
    ppc64_spu->high_lma = high;
  else if (low < ppc64_spu->low_lma)
    ppc64_spu->low_lma = low;
}

static void
map_relocations (bfd *abfd, asection *section, void *data)
{
  bfd *ppc_bfd = data;
  struct ppc64_spu_tdata *ppc64_spu = ppc_bfd->tdata.any;
  if ((section->flags & (SEC_ALLOC | SEC_EXCLUDE | SEC_HAS_CONTENTS | SEC_NEVER_LOAD | SEC_LOAD))
      != (SEC_ALLOC | SEC_HAS_CONTENTS | SEC_LOAD))
    return;
  if ((section->flags & SEC_RELOC) && section->reloc_count > 0)
    {
      int i;
      arelent **relpp;
      long relsize;
      long relcount;
      relsize = bfd_get_reloc_upper_bound (abfd, section);
      if (relsize < 0
          || (relpp = bfd_malloc (relsize)) == 0
	  || (relcount = bfd_canonicalize_reloc (abfd, section, relpp, ppc64_spu->spu_sympp)) == 0)
	return;
      for (i = 0; i < relcount; i++)
	{
	  unsigned int rtype = relpp[i]->howto->type;
	  bfd_vma offset = relpp[i]->address + section->lma - ppc64_spu->low_lma;
	  bfd_vma addend = relpp[i]->addend;
	  asymbol *sym = relpp[i]->sym_ptr_ptr[0];
	  if (rtype == R_SPU_PPU32)
	    add_relocation (ppc_bfd, offset, sym->name, R_PPC64_ADDR32, addend);
	  else if (rtype == R_SPU_PPU64)
	    add_relocation (ppc_bfd, offset, sym->name, R_PPC64_ADDR64, addend);
	}
      free (relpp);
    }
}

/* Change any non-alphanumeric characters to underscores.  */
static void
mangle_mem (char *p, unsigned int size)
{
  unsigned int i;
  for (i = 0; i < size; i++, p++)
    if (*p && !ISALNUM (*p))
      *p = '_';
}

static const bfd_target *
ppc64_spu_object_p (bfd *abfd)
{
  const bfd_target *right_targ;
  unsigned int i;
  char *name;
  unsigned int len;
  long symsize;
  long symcount;
  struct ppc64_spu_tdata *ppc64_spu;
  Elf_Internal_Shdr *data_sec, *symtab_sec, *strtab_sec, *shstrtab_sec, *rela_sec;
  bfd *nbfd;
  Elf_Internal_Ehdr *i_ehdrp;

  /* This target can only be used for reading. */
  if (bfd_write_p (abfd))
    return 0;

  /* Open the file as an SPU bfd_object */
  nbfd = bfd_openr (abfd->filename, "elf32-spu");
  if (!nbfd)
    return 0;
  nbfd->format = bfd_object;
  right_targ = BFD_SEND_FMT (nbfd, _bfd_check_format, (nbfd));
  if (right_targ != nbfd->xvec)
    {
      nbfd->format = bfd_unknown;
      goto fail_object_p;
    }

  if (!ppc64_spu_mkobject (abfd))
    goto fail_object_p;

  ppc64_spu = abfd->tdata.any;
  ppc64_spu->spu_bfd = nbfd;

  if ((symsize = bfd_get_symtab_upper_bound (nbfd)) < 0
      || (ppc64_spu->spu_sympp = bfd_alloc (abfd, symsize)) == 0
      || (symcount = bfd_canonicalize_symtab (nbfd, ppc64_spu->spu_sympp)) < 0)
    goto fail_object_p;

  bfd_map_over_sections (nbfd, alloc_section_bounds, ppc64_spu);

  /* Set up the ELF header */
  i_ehdrp = elf_elfheader(abfd);
  i_ehdrp->e_ident[EI_MAG0] = ELFMAG0;
  i_ehdrp->e_ident[EI_MAG1] = ELFMAG1;
  i_ehdrp->e_ident[EI_MAG2] = ELFMAG2;
  i_ehdrp->e_ident[EI_MAG3] = ELFMAG3;
  i_ehdrp->e_ident[EI_CLASS] = ELFCLASS64;
  i_ehdrp->e_ident[EI_DATA] = ELFDATA2MSB;
  i_ehdrp->e_ident[EI_VERSION] = EV_CURRENT;
  i_ehdrp->e_ident[EI_OSABI] = ELFOSABI_NONE;
  i_ehdrp->e_ident[EI_ABIVERSION] = 0;
  i_ehdrp->e_flags = EF_PPC64_REL24; /* sce flag */
  i_ehdrp->e_type = ET_REL;
  i_ehdrp->e_machine = EM_PPC64;
  i_ehdrp->e_ehsize = 64;
  i_ehdrp->e_shentsize = 64;
  i_ehdrp->e_shnum = 5;
  i_ehdrp->e_shstrndx = SHSTRTAB_SH_INDEX;
  elf_numsections (abfd) = i_ehdrp->e_shnum;

  /* Create internal section headers. Allocate for the maximum we might
   * need, even though we might not use them all.  */
  ppc64_spu->hdrs = bfd_zalloc (abfd, sizeof (Elf_Internal_Shdr) * 6);

  /* Create .data section */
  data_sec = &ppc64_spu->hdrs[DATA_SH_INDEX];
  data_sec->sh_type = SHT_PROGBITS;
  data_sec->sh_flags = SHF_WRITE | SHF_ALLOC;
  data_sec->sh_size = ppc64_spu->high_lma - ppc64_spu->low_lma;
  data_sec->sh_addralign = 1 << ppc64_spu->align_power;

  /* Create .shstrtab section */
  shstrtab_sec = &ppc64_spu->hdrs[SHSTRTAB_SH_INDEX];
  shstrtab_sec->sh_type = SHT_STRTAB;
  shstrtab_sec->sh_addralign = 1;
  shstrtab_sec->sh_size = 1;
  shstrtab_sec->contents = bfd_malloc (256);
  shstrtab_sec->contents[0] = 0;

  /* Create .symtab section */
  symtab_sec = &ppc64_spu->hdrs[SYMTAB_SH_INDEX];
  symtab_sec->sh_type = SHT_SYMTAB;
  symtab_sec->sh_link = STRTAB_SH_INDEX;
  symtab_sec->sh_info = 0;
  symtab_sec->sh_addralign = 8;
  symtab_sec->sh_entsize = 24;
  symtab_sec->sh_size = 24;
  symtab_sec->contents = bfd_zmalloc (8 * sizeof (Elf_Internal_Sym));
  ppc64_spu->syms = (void *)symtab_sec->contents;

  /* Create .strtab section */
  strtab_sec = &ppc64_spu->hdrs[STRTAB_SH_INDEX];
  strtab_sec->sh_type = SHT_STRTAB;
  strtab_sec->sh_size = 1;
  strtab_sec->contents = bfd_malloc (256);
  strtab_sec->contents[0] = 0;

  /* Create empty .rela.data.rel section.  It will only be seen if
     i_ehdrp->e_shnum is increased.  */
  rela_sec = &ppc64_spu->hdrs[RELA_SH_INDEX];
  rela_sec->sh_type = SHT_RELA;
  rela_sec->sh_link = SYMTAB_SH_INDEX;
  rela_sec->sh_info = DATA_SH_INDEX;
  rela_sec->sh_addralign = 8;
  rela_sec->sh_entsize = 24;
  rela_sec->sh_size = 0;
  rela_sec->contents = 0;

  len = strlen (bfd_get_filename (abfd)) + sizeof (sym_prefix) + 1;
  name = bfd_alloc (abfd, len);
  snprintf (name, len, "%s%s_", sym_prefix, bfd_get_filename (abfd));
  mangle_mem (name, len);

  /* Create internal symbols. The first is a dummy entry of all zeros. */

  /* .data section symbol */
  add_symbol (abfd, 0, 0, 0, 0,
	      STB_LOCAL, STT_SECTION, STV_DEFAULT,
	      DATA_SH_INDEX, 0);
  add_symbol (abfd, 0, 0, "_binary_start", 0,
	      STB_LOCAL, STT_NOTYPE, STV_DEFAULT,
	      DATA_SH_INDEX, 0);
  add_symbol (abfd, data_sec->sh_size, 0, "_binary_end", 0,
	      STB_LOCAL, STT_NOTYPE, STV_DEFAULT,
	      DATA_SH_INDEX, 0);
  add_symbol (abfd, data_sec->sh_size, 0, "_binary_size", 0,
	      STB_LOCAL, STT_NOTYPE, STV_DEFAULT,
	      SHN_ABS, 0);

  /* _binary_<file>_start */
  add_symbol (abfd, 0, 0, name, "start",
	      STB_GLOBAL, STT_NOTYPE, STV_DEFAULT,
	      DATA_SH_INDEX, 0);

  /* _binary_<file>_end */
  add_symbol (abfd, data_sec->sh_size, 0, name, "end",
	      STB_GLOBAL, STT_NOTYPE, STV_DEFAULT,
	      DATA_SH_INDEX, 0);

  /* _binary_<file>_size */
  add_symbol (abfd, data_sec->sh_size, 0, name, "size",
	      STB_GLOBAL, STT_NOTYPE, STV_DEFAULT,
	      SHN_ABS, 0);

  /* Read in SPU relocations and create PPU relocations */
  bfd_map_over_sections (nbfd, map_relocations, abfd);

  /* Now we know all the sections we need, so create names for them */
  symtab_sec->sh_name = add_string (abfd, SHSTRTAB_SH_INDEX, ".symtab", 0);
  strtab_sec->sh_name = add_string (abfd, SHSTRTAB_SH_INDEX, ".strtab", 0);
  shstrtab_sec->sh_name = add_string (abfd, SHSTRTAB_SH_INDEX, ".shstrtab", 0);
  if (i_ehdrp->e_shnum == 6)
    {
      data_sec->sh_name = add_string (abfd, SHSTRTAB_SH_INDEX, ".data.rel", 0);
      rela_sec->sh_name = add_string (abfd, SHSTRTAB_SH_INDEX, ".rela.data.rel", 0);
    }
  else
      data_sec->sh_name = add_string (abfd, SHSTRTAB_SH_INDEX, ".data", 0);

  /* And allocate the section pointer buffer */
  elf_elfsections(abfd) = bfd_zalloc (abfd, sizeof (Elf_Internal_Shdr *) * (i_ehdrp->e_shnum + 1));
  for (i = 0; i < i_ehdrp->e_shnum; i++)
    elf_elfsections(abfd)[i] = &ppc64_spu->hdrs[i];
  elf_elfsections(abfd)[i_ehdrp->e_shnum] = 0;

  if (!bfd_section_from_shdr (abfd, DATA_SH_INDEX)
      || !bfd_section_from_shdr (abfd, SHSTRTAB_SH_INDEX)
      || !bfd_section_from_shdr (abfd, SYMTAB_SH_INDEX)
      || !bfd_section_from_shdr (abfd, STRTAB_SH_INDEX)
      || (i_ehdrp->e_shnum == 6 && !bfd_section_from_shdr (abfd, RELA_SH_INDEX)))
    goto fail_object_p;

  /* Set lma for the .data section */
  data_sec->bfd_section->lma = ppc64_spu->low_lma;

  /* Could call _bfd_elf_setup_sections (abfd), but currently it only
   * deals with SHF_GROUP and SHF_LINK_ORDER, so we don't need it */

  if (bfd_get_arch_info (abfd) != NULL)
    bfd_set_arch_info (abfd, bfd_lookup_arch (bfd_arch_powerpc, bfd_mach_ppc64));

  return abfd->xvec;

fail_object_p:
  bfd_close (nbfd);
  return 0;
}

struct get_section_contents_args {
  void *location;
  bfd_size_type offset;
  bfd_size_type count;
  bfd_boolean success;
};

static void
map_get_section_contents (bfd *abfd, asection *section, void *data)
{
  struct get_section_contents_args *args = data;
  void *l = NULL; /* ??? */
  bfd_size_type o = 0; /* ??? */
  bfd_size_type c = 0;

  if ((section->flags & (SEC_ALLOC | SEC_EXCLUDE | SEC_HAS_CONTENTS | SEC_NEVER_LOAD | SEC_LOAD))
      != (SEC_ALLOC | SEC_HAS_CONTENTS | SEC_LOAD))
    return;

  if (section->lma >= args->offset && section->lma < args->offset + args->count)
    {
      l = args->location + (section->lma - args->offset);
      o = 0;
      if (section->lma + section->size <= args->offset + args->count)
	c = section->size;
      else
	c = (args->offset + args->count) - section->lma;

    }
  else if (section->lma < args->offset && section->lma + section->size > args->offset)
    {
      l = args->location;
      o = args->offset - section->lma;
      if (section->lma + section->size <= args->offset + args->count)
	c = (section->lma + section->size) - args->offset;
      else
	c = args->count;
    }

  if (c > 0)
    if (!bfd_get_section_contents (abfd, section, l, (file_ptr)o, c))
      args->success = FALSE;
}

/* Get contents of the only section.  */

static bfd_boolean
ppc64_spu_get_section_contents (bfd *abfd,
				asection *section,
				void * location,
				file_ptr offset,
				bfd_size_type count)
{
  struct ppc64_spu_tdata *ppc64_spu = abfd->tdata.any;
  struct get_section_contents_args args;

  if (section != elf_elfsections(abfd)[DATA_SH_INDEX]->bfd_section)
    return FALSE;

  // The mapping is based on lma, there could be holes.
  memset (location, 0, count);

  args.location = location;
  args.offset = (bfd_size_type)offset + section->lma;
  args.count = count;
  args.success = TRUE;
  bfd_map_over_sections (ppc64_spu->spu_bfd, map_get_section_contents, &args);
  return args.success;
}


static Elf_Internal_Sym *
ppc64_spu_get_elf_syms (bfd * ibfd,
			Elf_Internal_Shdr * symtab_hdr,
			size_t symcount,
			size_t symoffset,
			Elf_Internal_Sym * intsym_buf,
			void *extsym_buf ATTRIBUTE_UNUSED,
			Elf_External_Sym_Shndx * extshndx_buf ATTRIBUTE_UNUSED)
{
  struct ppc64_spu_tdata *ppc64_spu = ibfd->tdata.any;

  if (symcount == 0)
    return intsym_buf;

  if (symtab_hdr != &elf_tdata (ibfd)->symtab_hdr
      || symcount + symoffset > symtab_hdr->sh_size / symtab_hdr->sh_entsize)
    return NULL;

  if (intsym_buf == NULL)
    {
      intsym_buf = bfd_malloc2 (symcount, sizeof (Elf_Internal_Sym));
      if (intsym_buf == NULL)
	return NULL;
    }
  memcpy (intsym_buf, ppc64_spu->syms + symoffset, symcount * sizeof (Elf_Internal_Sym));
  return intsym_buf;
}

static bfd_boolean
ppc64_spu_close_and_cleanup (bfd * abfd)
{
  struct ppc64_spu_tdata *ppc64_spu = abfd->tdata.any;
  bfd_close (ppc64_spu->spu_bfd);
  return _bfd_elf_close_and_cleanup (abfd);
}

static long
ppc64_spu_canonicalize_reloc (bfd *abfd, sec_ptr section,
			       arelent **relptr, asymbol **symbols)
{
  arelent *tblptr;
  unsigned int i;

  if (section->relocation == NULL)
    {
      struct bfd_elf_section_data * const d = elf_section_data (section);
      Elf_Internal_Shdr *rel_hdr;
      Elf_Internal_Rela *rela;
      bfd_size_type reloc_count;
      arelent *relent;
      unsigned int symcount = bfd_get_symcount (abfd);

      if (d->rel_hdr.sh_size == 0)
	return 0;

      rel_hdr = &d->rel_hdr;
      reloc_count = NUM_SHDR_ENTRIES (rel_hdr);
      section->relocation = bfd_alloc (abfd, reloc_count * sizeof (arelent));

      relent = section->relocation;
      rela = (void *)rel_hdr->contents;
      for (i = 0; i < reloc_count; i++, relent++, rela++)
	{
	  relent->address = rela->r_offset;
	  if (ELF64_R_SYM (rela->r_info) == 0)
	    relent->sym_ptr_ptr = bfd_abs_section_ptr->symbol_ptr_ptr;
	  else if (ELF64_R_SYM (rela->r_info) > symcount)
	    {
	      (*_bfd_error_handler)
		(_("%s(%s): relocation %d has invalid symbol index %ld"),
		 abfd->filename, section->name, i, ELF64_R_SYM (rela->r_info));
	      relent->sym_ptr_ptr = bfd_abs_section.symbol_ptr_ptr;
	    }
	  else
	    relent->sym_ptr_ptr = symbols + ELF64_R_SYM (rela->r_info) - 1;
	  relent->addend = rela->r_addend;
	  ppc64_spu_info_to_howto (abfd, relent, rela);
	}
    }

  tblptr = section->relocation;
  for (i = 0; i < section->reloc_count; i++)
    *relptr++ = tblptr++;
  *relptr = NULL;

  return section->reloc_count;
}

#define TARGET_BIG_SYM		bfd_elf64_powerpc_spu_vec
#define TARGET_BIG_NAME		"elf64-powerpc-spu"
#define ELF_ARCH		bfd_arch_powerpc
#define ELF_MACHINE_CODE	EM_PPC64
#define ELF_MAXPAGESIZE		0x10000

#define elf_info_to_howto	         ppc64_spu_info_to_howto
#define bfd_elf64_bfd_reloc_type_lookup  ppc64_spu_reloc_type_lookup

#define bfd_elf64_object_p		 ppc64_spu_object_p
#define bfd_elf64_mkobject		 ppc64_spu_mkobject
#define bfd_elf64_close_and_cleanup      ppc64_spu_close_and_cleanup

#define bfd_elf64_get_section_contents	 ppc64_spu_get_section_contents
#define elf_backend_get_elf_syms 	 ppc64_spu_get_elf_syms
#define bfd_elf64_canonicalize_reloc 	 ppc64_spu_canonicalize_reloc

#define elf_backend_want_got_sym 0
#define elf_backend_want_plt_sym 0
#define elf_backend_plt_alignment 3
#define elf_backend_plt_not_loaded 1
#define elf_backend_got_header_size 8
#define elf_backend_can_gc_sections 1
#define elf_backend_can_refcount 1
#define elf_backend_rela_normal 1

#include "elf64-target.h"
