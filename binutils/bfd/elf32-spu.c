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

#include "bfd.h"
#include "sysdep.h"
#include "bfdlink.h"
#include "libbfd.h"
#include "elf-bfd.h"
#include "elf/spu.h"

void spu_elf_info_to_howto			PARAMS ((bfd *, arelent *, Elf_Internal_Rela *));
void spu_elf_info_to_howto_rel			PARAMS ((bfd *, arelent *, Elf_Internal_Rela *));
reloc_howto_type *spu_elf_reloc_type_lookup	PARAMS ((bfd *, bfd_reloc_code_real_type));
static void spu_elf_final_write_processing	PARAMS ((bfd *, bfd_boolean));
static bfd_boolean spu_elf_relocate_section	PARAMS ((bfd *, struct bfd_link_info *, bfd *,
							asection *, bfd_byte *, Elf_Internal_Rela *,
							Elf_Internal_Sym *, asection **));
static asection * spu_elf_gc_mark_hook		PARAMS ((asection *, struct bfd_link_info *,
							Elf_Internal_Rela *, struct elf_link_hash_entry *,
							Elf_Internal_Sym *));
static bfd_boolean spu_elf_gc_sweep_hook	PARAMS ((bfd *, struct bfd_link_info *, asection *,
							const Elf_Internal_Rela *));

#if defined(BPA)
static void spu_elf_post_process_headers	PARAMS ((bfd *, struct bfd_link_info *));
static bfd_boolean spu_elf_section_processing	PARAMS ((bfd *, Elf_Internal_Shdr *));
static bfd_boolean spu_elf_always_size_sections	PARAMS ((bfd *, struct bfd_link_info *));
#endif

/*  When USE_REL is not defined bfd uses reloc entry addends
 *  instead of inserting the addend into the instruction.
 *  #define USE_REL 0
 */


/* Values of type 'enum elf_spu_reloc_type' are used to index this
 * array, so it must be declared in the order of that type. */
static reloc_howto_type elf_howto_table[] = {
  HOWTO(R_SPU_NONE,	0, 0, 0, FALSE,  0, complain_overflow_dont,     bfd_elf_generic_reloc, "SPU_NONE",	FALSE, 0x00000000, 0x00000000, FALSE),
  HOWTO(R_SPU_ADDR10,	4, 2,10, FALSE, 14, complain_overflow_bitfield, bfd_elf_generic_reloc, "SPU_ADDR10",	FALSE, 0x00ffc000, 0x00ffc000, FALSE),
  HOWTO(R_SPU_ADDR16,	2, 2,16, FALSE,  7, complain_overflow_bitfield, bfd_elf_generic_reloc, "SPU_ADDR16",	FALSE, 0x007fff80, 0x007fff80, FALSE),
  HOWTO(R_SPU_ADDR16_HI,16,2,16, FALSE,  7, complain_overflow_bitfield, bfd_elf_generic_reloc, "SPU_ADDR16_HI",	FALSE, 0x007fff80, 0x007fff80, FALSE),
  HOWTO(R_SPU_ADDR16_LO,0, 2,16, FALSE,  7, complain_overflow_dont,     bfd_elf_generic_reloc, "SPU_ADDR16_LO",	FALSE, 0x007fff80, 0x007fff80, FALSE),
  HOWTO(R_SPU_ADDR18,	0, 2,18, FALSE,  7, complain_overflow_bitfield, bfd_elf_generic_reloc, "SPU_ADDR18",	FALSE, 0x01ffff80, 0x01ffff80, FALSE),
  HOWTO(R_SPU_GLOB_DAT,	0, 2,32, FALSE,  0, complain_overflow_bitfield, bfd_elf_generic_reloc, "SPU_GLOB_DAT",	FALSE, 0xffffffff, 0xffffffff, FALSE),
  HOWTO(R_SPU_REL16,	2, 2,16,  TRUE,  7, complain_overflow_bitfield, bfd_elf_generic_reloc, "SPU_REL16",	FALSE, 0x007fff80, 0x007fff80, TRUE),
  HOWTO(R_SPU_ADDR7,	0, 2, 7, FALSE, 14, complain_overflow_dont,     bfd_elf_generic_reloc, "SPU_ADDR7",	FALSE, 0x001fc000, 0x001fc000, FALSE),
  HOWTO(R_SPU_REL9,     2, 2, 9,  TRUE,  0, complain_overflow_signed,   bfd_elf_generic_reloc, "SPU_REL9",	FALSE, 0x0180007f, 0x0180007f, TRUE),
  HOWTO(R_SPU_REL9I,    2, 2, 9,  TRUE,  0, complain_overflow_signed,   bfd_elf_generic_reloc, "SPU_REL9I",	FALSE, 0x0000c07f, 0x0000c07f, TRUE),
  HOWTO(R_SPU_ADDR10I,	0, 2,10, FALSE, 14, complain_overflow_signed,   bfd_elf_generic_reloc, "SPU_ADDR10I",	FALSE, 0x00ffc000, 0x00ffc000, FALSE),
  HOWTO(R_SPU_ADDR16I,	0, 2,16, FALSE,  7, complain_overflow_signed,   bfd_elf_generic_reloc, "SPU_ADDR16I",	FALSE, 0x007fff80, 0x007fff80, FALSE),
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
    case BFD_RELOC_32:
      return R_SPU_GLOB_DAT;
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
    }
}

void
spu_elf_info_to_howto (bfd * abfd ATTRIBUTE_UNUSED,
			 arelent * cache_ptr ATTRIBUTE_UNUSED,
			 Elf_Internal_Rela * dst ATTRIBUTE_UNUSED)
{
  enum elf_spu_reloc_type r_type;

  r_type = (enum elf_spu_reloc_type) ELF32_R_TYPE (dst->r_info);
  BFD_ASSERT (r_type < R_SPU_max);
  cache_ptr->howto = &elf_howto_table[(int) r_type];
}

void
spu_elf_info_to_howto_rel (bfd * abfd ATTRIBUTE_UNUSED,
			     arelent * cache_ptr, Elf_Internal_Rela * dst)
{
  enum elf_spu_reloc_type type;

  type = (enum elf_spu_reloc_type) ELF32_R_TYPE (dst->r_info);
  BFD_ASSERT (type < R_SPU_max);
  cache_ptr->howto = &elf_howto_table[(int) type];
}

reloc_howto_type *
spu_elf_reloc_type_lookup (bfd * abfd ATTRIBUTE_UNUSED,
			     bfd_reloc_code_real_type code)
{
  return elf_howto_table + spu_elf_bfd_to_reloc_type (code);
}

/* Look through the relocs for a section during the first phase and
   make any required dynamic sections. 

   We iterate over the relocations three times:

     spu_elf_check_relocs     
       This creates any needed dynamic sections as we first read all the
       input objects.  We need to do create the sections now so they get
       mapped to the correct output sections.  At this points we don't
       know which symbols are resolved from dynamic objects.

     allocate_dynrelocs
       This computes sizes of the sections.  Now we do know which
       symbols come from where, so we can determine the correct amount
       of space to allocate.  Some sections will require no space and
       are stripped by spu_elf_size_dynamic_sections.

     spu_elf_relocate_section
       This finally creates the relocations in the correct section.
 */
static bfd_boolean
spu_elf_check_relocs (bfd *abfd, struct bfd_link_info *info,
		      asection *sec, const Elf_Internal_Rela *relocs)
{
  Elf_Internal_Shdr *symtab_hdr;
  struct elf_link_hash_entry **sym_hashes, **sym_hashes_end;
  const Elf_Internal_Rela *rel;
  const Elf_Internal_Rela *rel_end;
  asection *sreloc;
  bfd *dynobj;

  if (info->relocatable)
    return TRUE;

  /* Don't do anything special with non-loaded, non-alloced sections.
     In particular, there's not much point in propagating relocs to
     shared libs that the dynamic linker won't relocate.  */
  if ((sec->flags & SEC_ALLOC) == 0)
    return TRUE;

  dynobj = elf_hash_table (info)->dynobj;

  symtab_hdr = &elf_tdata (abfd)->symtab_hdr;

  sym_hashes = elf_sym_hashes (abfd);
  sym_hashes_end = (sym_hashes
		    + symtab_hdr->sh_size / sizeof (Elf32_External_Sym)
		    - symtab_hdr->sh_info);

  sreloc = NULL;

  rel_end = relocs + sec->reloc_count;
  for (rel = relocs; rel < rel_end; rel++)
    {
      unsigned long r_symndx;
      struct elf_link_hash_entry *h;
      enum elf_spu_reloc_type r_type;

      r_symndx = ELF32_R_SYM (rel->r_info);
      if (r_symndx < symtab_hdr->sh_info)
	h = NULL;
      else
	h = sym_hashes[r_symndx - symtab_hdr->sh_info];

      r_type = ELF32_R_TYPE (rel->r_info);
      switch (r_type)
	{
	case R_SPU_ADDR10:
	case R_SPU_ADDR16:
	case R_SPU_ADDR16_HI:
	case R_SPU_ADDR16_LO:
	case R_SPU_ADDR18:
	case R_SPU_GLOB_DAT:
	case R_SPU_REL16:
	case R_SPU_ADDR7:
	case R_SPU_ADDR10I:
	case R_SPU_ADDR16I:
	  if (h != NULL
	      && (!h->def_regular
		  || h->root.type == bfd_link_hash_defweak
		  || (info->shared && ! info->symbolic)))
	    {
	      /* We might need to copy these reloc types into the output file.
		 Create a reloc section in dynobj.  */
	      if (sreloc == NULL)
		{
		  const char *name;

		  name = (bfd_elf_string_from_elf_section
			  (abfd,
			   elf_elfheader (abfd)->e_shstrndx,
			   elf_section_data (sec)->rel_hdr.sh_name));
		  if (name == NULL)
		    return FALSE;

		  if (strncmp (name, ".rela", 5) != 0
		      || strcmp (bfd_get_section_name (abfd, sec),
				 name + 5) != 0)
		    {
		      (*_bfd_error_handler)
			(_("%B: bad relocation section name `%s\'"),
			 abfd, name);
		      bfd_set_error (bfd_error_bad_value);
		    }

		  if (dynobj == NULL)
		    dynobj = elf_hash_table (info)->dynobj = abfd;

		  sreloc = bfd_get_section_by_name (dynobj, name);
		  if (sreloc == NULL)
		    {
		      flagword flags;

		      sreloc = bfd_make_section (dynobj, name);
		      flags = (SEC_HAS_CONTENTS | SEC_READONLY
			       | SEC_IN_MEMORY | SEC_LINKER_CREATED);
		      if ((sec->flags & SEC_ALLOC) != 0)
			flags |= SEC_ALLOC | SEC_LOAD;
		      if (sreloc == NULL
			  || ! bfd_set_section_flags (dynobj, sreloc, flags)
			  || ! bfd_set_section_alignment (dynobj, sreloc, 3))
			return FALSE;
		    }
		  elf_section_data (sec)->sreloc = sreloc;
		}
	    }
	  break;

	default:
	  break;
	}
    }
  return TRUE;
}


static bfd_boolean
spu_elf_relocate_section (bfd * output_bfd ATTRIBUTE_UNUSED,
			    struct bfd_link_info *info,
			    bfd * input_bfd,
			    asection * input_section,
			    bfd_byte * contents,
			    Elf_Internal_Rela * relocs,
			    Elf_Internal_Sym * local_syms,
			    asection ** local_sections)
{
  Elf_Internal_Shdr *symtab_hdr;
  struct elf_link_hash_entry **sym_hashes;
  Elf_Internal_Rela *rel, *relend;
  bfd_boolean ret = TRUE;

  if (info->relocatable)
    return TRUE;

  symtab_hdr = &elf_tdata (input_bfd)->symtab_hdr;
  sym_hashes = (struct elf_link_hash_entry **) (elf_sym_hashes (input_bfd));

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
      bfd_reloc_status_type r;
      bfd_boolean unresolved_reloc;
      bfd_boolean warned;

      r_symndx = ELF32_R_SYM (rel->r_info);
      r_type = ELF32_R_TYPE (rel->r_info);
      howto = elf_howto_table + r_type;
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

      switch (r_type)
	{
	  /* Relocations that always need to be propagated if this is a shared
	     object.  */
	case R_SPU_ADDR10:
	case R_SPU_ADDR16:
	case R_SPU_ADDR16_HI:
	case R_SPU_ADDR16_LO:
	case R_SPU_ADDR18:
	case R_SPU_GLOB_DAT:
	case R_SPU_REL16:
	case R_SPU_ADDR7:
	case R_SPU_ADDR10I:
	case R_SPU_ADDR16I:
	  /* r_symndx will be zero only for relocs against symbols
	     from removed linkonce sections, or sections discarded by
	     a linker script.  */
	  if (r_symndx == 0)
	    break;
	  /* Fall thru.  */

	  if ((info->shared
	       && (h == NULL
		   || ELF_ST_VISIBILITY (h->other) == STV_DEFAULT
		   || h->root.type != bfd_link_hash_undefweak)
	       && (!SYMBOL_CALLS_LOCAL (info, h)))
	      || (!info->shared
		  && h != NULL
		  && h->dynindx != -1
		  && h->def_dynamic
		  && !h->def_regular))
	    {
	      Elf_Internal_Rela outrel;
	      bfd_boolean skip, relocate;
	      asection *sreloc;
	      bfd_byte *loc;
	      bfd_vma out_off;

	      /* When generating a dynamic object, these relocations
		 are copied into the output file to be resolved at run
		 time.  */

	      skip = FALSE;
	      relocate = FALSE;

	      out_off = _bfd_elf_section_offset (output_bfd, info,
						 input_section, rel->r_offset);
	      if (out_off == (bfd_vma) -1)
		skip = TRUE;
	      else if (out_off == (bfd_vma) -2)
		skip = TRUE, relocate = TRUE;
	      out_off += (input_section->output_section->vma
			  + input_section->output_offset);
	      outrel.r_offset = out_off;
	      outrel.r_addend = rel->r_addend;

	      if (skip)
		memset (&outrel, 0, sizeof outrel);
	      else if (!SYMBOL_REFERENCES_LOCAL (info, h))
		outrel.r_info = ELF32_R_INFO (h->dynindx, r_type);
	      else
		{
		  /* This symbol is local, or marked to become local. */
		  outrel.r_addend += relocation;
		  if (r_type == R_SPU_GLOB_DAT)
		    {
		      outrel.r_info = ELF32_R_INFO (0, R_SPU_GLOB_DAT);

		      /* Prelink also wants simple and consistent rules
			 for relocs.  This make all RELATIVE relocs have
			 *r_offset equal to r_addend.  */
		      relocate = TRUE;
		    }
		  else
		    {
		      long indx = 0;

		      if (bfd_is_abs_section (sec))
			;
		      else if (sec == NULL || sec->owner == NULL)
			{
			  bfd_set_error (bfd_error_bad_value);
			  return FALSE;
			}
		      else
			{
			  asection *osec;

			  osec = sec->output_section;
			  indx = elf_section_data (osec)->dynindx;

			  /* We are turning this relocation into one
			     against a section symbol, so subtract out
			     the output section's address but not the
			     offset of the input section in the output
			     section.  */
			  outrel.r_addend -= osec->vma;
			}

		      outrel.r_info = ELF32_R_INFO (indx, r_type);
		    }
		}

	      sreloc = elf_section_data (input_section)->sreloc;
	      if (sreloc == NULL)
		abort ();

	      loc = sreloc->contents;
	      loc += sreloc->reloc_count++ * sizeof (Elf32_External_Rela);
	      bfd_elf32_swap_reloca_out (output_bfd, &outrel, loc);

	      /* If this reloc is against an external symbol, it will
		 be computed at runtime, so there's no need to do
		 anything now.  However, for the sake of prelink ensure
		 that the section contents are a known value.  */
	      if (! relocate)
		{
		  unresolved_reloc = FALSE;
		  /* The value chosen here is quite arbitrary as ld.so
		     ignores section contents except for the special
		     case of .opd where the contents might be accessed
		     before relocation.  Choose zero, as that won't
		     cause reloc overflow.  */
		  relocation = 0;
		  rel->r_addend = 0;
		  /* Adjust pc_relative relocs to have zero in *r_offset.  */
		  if (howto->pc_relative)
		    rel->r_addend = (input_section->output_section->vma
				  + input_section->output_offset
				  + rel->r_offset);
		}
	    }
	  break;
	}

      if (unresolved_reloc
	  && !((input_section->flags & SEC_DEBUGGING) != 0
	       && h->def_dynamic))
	{
	  (*_bfd_error_handler)
	    (_("%B(%s+0x%lx): unresolvable %s relocation against symbol `%s'"),
	     input_bfd,
	     bfd_get_section_name (input_bfd, input_section),
	     (long) rel->r_offset,
	     howto->name,
	     sym_name);
	  ret = FALSE;
	}

      r = _bfd_final_link_relocate (howto,
				    input_bfd,
				    input_section,
				    contents,
				    rel->r_offset, relocation, rel->r_addend);

      if (r != bfd_reloc_ok)
	{
	  const char *name;
	  const char *msg = (const char *) 0;

	  if (h != NULL)
	    name = h->root.root.string;
	  else
	    {
	      name = (bfd_elf_string_from_elf_section
		      (input_bfd, symtab_hdr->sh_link, sym->st_name));
	      if (name == NULL || *name == '\0')
		name = bfd_section_name (input_bfd, sec);
	    }

	  switch (r)
	    {
	    case bfd_reloc_overflow:
	      if (!((*info->callbacks->reloc_overflow)
		    (info, (h ? &h->root : NULL), name, howto->name, 
		     (bfd_vma) 0, input_bfd, input_section, rel->r_offset)))
		return FALSE;
	      break;

	    case bfd_reloc_undefined:
	      if (!((*info->callbacks->undefined_symbol)
		    (info, name, input_bfd, input_section,
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
		    (info, msg, name, input_bfd, input_section,
		     rel->r_offset)))
		return FALSE;
	      break;
	    }
	}
    }

  return ret;
}
                                                                                                  
static asection *
spu_elf_gc_mark_hook (asection *sec,
                     struct bfd_link_info *info ATTRIBUTE_UNUSED,
                     Elf_Internal_Rela *rel ATTRIBUTE_UNUSED, struct elf_link_hash_entry *h,
                     Elf_Internal_Sym *sym)
{
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
                                                                                                  
static bfd_boolean
spu_elf_gc_sweep_hook (bfd *abfd ATTRIBUTE_UNUSED, struct bfd_link_info *info ATTRIBUTE_UNUSED,
                      asection *sec ATTRIBUTE_UNUSED, const Elf_Internal_Rela *relocs ATTRIBUTE_UNUSED)
{                                                                                                 
  return TRUE;
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
  BFD_ASSERT (elf_elfheader (abfd)->e_flags == 0);

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


#if defined(BPA)
static void
spu_elf_post_process_headers (bfd * abfd, struct bfd_link_info *link_info)
{
  Elf_Internal_Ehdr *i_ehdrp;	/* Elf file header, internal form */

  /* e_type is set if -plugin assigned. */

  i_ehdrp = elf_elfheader (abfd);

  if (link_info != NULL)
    {
      if (link_info->spuplugin)
	{
	  i_ehdrp->e_type = ET_DYN;
	}
    }
}

#endif


#if defined(BPA)
static bfd_boolean
spu_elf_section_processing (bfd * abfd, Elf_Internal_Shdr * i_shdrp)
{
  /* Content of PT_NOTE segment for SPU plugin is set here.
     Because it doesn't have SEC_ALLOC attribute,
     it is not written in file as usual process.

     If this routine is not used, some special writing process has to be done somewhere.
     (e.g., special function would be needed as string table writing process.) */

  asection *sec;

  sec = i_shdrp->bfd_section;

  if ((sec != NULL) &&
      (sec->name != NULL)
      && (strcmp (sec->name, SPU_PTNOTE_SPUNAME) == 0)
      && (bfd_usrdata(abfd) == NULL))
    {

      SPUPLUGIN_INFO *spuplugin_info;
      spuplugin_info = bfd_alloc (abfd, sizeof (SPUPLUGIN_INFO));

      bfd_put_32(abfd, (bfd_vma) SPU_PLUGIN_NAMESZ, &spuplugin_info->namesz) ;
      bfd_put_32(abfd, (bfd_vma) SPU_PLUGIN_LOOKUPNAMESZ, &spuplugin_info->descsz) ;
      bfd_put_32(abfd, (bfd_vma) 1,  &spuplugin_info->type) ;
      (void)strncpy( spuplugin_info->name, SPU_PLUGIN_NAME, SPU_PLUGIN_NAMESZ);
      (void)strncpy( spuplugin_info->lookupname, bfd_get_filename(abfd), SPU_PLUGIN_LOOKUPNAMESZ);

      i_shdrp->contents = (unsigned char*)spuplugin_info ;
    }

  /* begin sce local, bugzilla #2878 */
  /* If the section will loaded, we compute the SPU GUID from the section. */
  if (sec != NULL
      && ((sec->flags & (SEC_HAS_CONTENTS | SEC_LOAD | SEC_ALLOC))
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
#endif

#if defined(BPA)
/*
 * Make SPU_PTNOTE_SPUNAME section 
 * */
static bfd_boolean
spu_elf_always_size_sections (bfd * abfd, struct bfd_link_info *link_info ATTRIBUTE_UNUSED)
{

  register asection *s;
  char *sname = SPU_PTNOTE_SPUNAME;

  s = bfd_make_section_anyway_with_flags (abfd, sname,
					  SEC_LOAD | SEC_IN_MEMORY | SEC_HAS_CONTENTS
					  | SEC_LINKER_CREATED | SEC_READONLY);

  if (s == NULL
      || ! bfd_set_section_alignment (abfd, s, 2))
    return FALSE;

  if (!bfd_set_section_size (abfd, s, sizeof (SPUPLUGIN_INFO)))
    return FALSE;

  return TRUE;
}
#endif

static bfd_boolean
spu_elf_finish_dynamic_sections (bfd *output_bfd ATTRIBUTE_UNUSED,
				 struct bfd_link_info *info ATTRIBUTE_UNUSED)
{
  return TRUE;
}
static bfd_boolean
spu_elf_finish_dynamic_symbol (bfd *output_bfd ATTRIBUTE_UNUSED,
				      struct bfd_link_info *info ATTRIBUTE_UNUSED,
				      struct elf_link_hash_entry *h ATTRIBUTE_UNUSED,
				      Elf_Internal_Sym *sym ATTRIBUTE_UNUSED)
{
  return TRUE;
}

/* compute the sizes of the required dynamic relocatios */
static bfd_boolean
allocate_dynrelocs (bfd *abfd, struct bfd_link_info *info,
		      asection *sec, const Elf_Internal_Rela *relocs)
{
  Elf_Internal_Shdr *symtab_hdr;
  struct elf_link_hash_entry **sym_hashes, **sym_hashes_end;
  const Elf_Internal_Rela *rel;
  const Elf_Internal_Rela *rel_end;
  asection *sreloc;
  bfd *dynobj;

  if (info->relocatable)
    return TRUE;

  /* Don't do anything special with non-loaded, non-alloced sections.
     In particular, there's not much point in propagating relocs to
     shared libs that the dynamic linker won't relocate.  */
  if ((sec->flags & SEC_ALLOC) == 0)
    return TRUE;

  /* spu_elf_check_relocs will have set sreloc for the sections we
     need to check. */
  sreloc = elf_section_data (sec)->sreloc;
  if (sreloc == NULL)
    return TRUE;

  dynobj = elf_hash_table (info)->dynobj;
  if (dynobj == NULL)
    abort();

  symtab_hdr = &elf_tdata (abfd)->symtab_hdr;

  sym_hashes = elf_sym_hashes (abfd);
  sym_hashes_end = (sym_hashes
		    + symtab_hdr->sh_size / sizeof (Elf32_External_Sym)
		    - symtab_hdr->sh_info);


  rel_end = relocs + sec->reloc_count;
  for (rel = relocs; rel < rel_end; rel++)
    {
      unsigned long r_symndx;
      struct elf_link_hash_entry *h;
      enum elf_spu_reloc_type r_type;

      r_symndx = ELF32_R_SYM (rel->r_info);
      if (r_symndx < symtab_hdr->sh_info)
	h = NULL;
      else
	h = sym_hashes[r_symndx - symtab_hdr->sh_info];

      r_type = ELF32_R_TYPE (rel->r_info);
      switch (r_type)
	{
	case R_SPU_ADDR10:
	case R_SPU_ADDR16:
	case R_SPU_ADDR16_HI:
	case R_SPU_ADDR16_LO:
	case R_SPU_ADDR18:
	case R_SPU_GLOB_DAT:
	case R_SPU_REL16:
	case R_SPU_ADDR7:
	case R_SPU_ADDR10I:
	case R_SPU_ADDR16I:
	  if ((info->shared
	       && (h == NULL
		   || ELF_ST_VISIBILITY (h->other) == STV_DEFAULT
		   || h->root.type != bfd_link_hash_undefweak)
	       && (!SYMBOL_CALLS_LOCAL (info, h)))
	      || (!info->shared
		  && h != NULL
		  && h->dynindx != -1
		  && h->def_dynamic
		  && !h->def_regular))
	    {
	      /* We must copy these reloc types into the output file.
		 Increase the size of the reloc section.  */
	      sreloc->rawsize += sizeof (Elf32_External_Rela);
	    }
	  break;

	default:
	  break;
	}
    }
  return TRUE;
}

/* Set the sizes of the dynamic sections.  */

static bfd_boolean
spu_elf_size_dynamic_sections (bfd * output_bfd ATTRIBUTE_UNUSED, struct bfd_link_info * info)
{
  bfd * dynobj;
  asection * s;
  bfd_boolean relocs;
  bfd_boolean reltext;
  asection *o;
  bfd *inputobj;

  /* Check all the relocations of all input objects to determine
     the size of dynamic sections. */
  for (inputobj = info->input_bfds;
       inputobj;
       inputobj = inputobj->link_next)
    {
      for (o = inputobj->sections; o != NULL; o = o->next)
	{
	  Elf_Internal_Rela *internal_relocs;
	  bfd_boolean ok;

	  if ((o->flags & SEC_RELOC) == 0
	      || o->reloc_count == 0
	      || ((info->strip == strip_all || info->strip == strip_debugger)
		  && (o->flags & SEC_DEBUGGING) != 0)
	      || bfd_is_abs_section (o->output_section))
	    continue;

	  internal_relocs = _bfd_elf_link_read_relocs (inputobj, o, NULL, NULL,
						       info->keep_memory);
	  if (internal_relocs == NULL)
	    return FALSE;

	  ok = allocate_dynrelocs (inputobj, info, o, internal_relocs);

	  if (elf_section_data (o)->relocs != internal_relocs)
	    free (internal_relocs);

	  if (! ok)
	    return FALSE;
	}
    }

  dynobj = elf_hash_table (info)->dynobj;
  BFD_ASSERT (dynobj != NULL);

  /* The code above has determined the sizes of the various dynamic
     sections.  Allocate memory for them.  */
  relocs = FALSE;
  reltext = FALSE;
  for (s = dynobj->sections; s != NULL; s = s->next)
    {
      const char * name;
      bfd_boolean strip;

      if ((s->flags & SEC_LINKER_CREATED) == 0)
	continue;

      if (s->contents != NULL)
	continue;

      /* It's OK to base decisions on the section name, because none
	 of the dynobj section names depend upon the input files.  */
      name = bfd_get_section_name (dynobj, s);

      strip = FALSE;

      if (strncmp (name, ".rela", 5) == 0)
	{
	  if (s->rawsize == 0)
	    {
	      /* If we don't need this section, strip it from the output
	         file.  */
	      strip = TRUE;
	    }
	  else
	    {
	      /* We use the reloc_count field as a counter if we need
		 to copy relocs into the output file.  */
	      s->reloc_count = 0;
	    }
	}
      else 
	/* It's not one of our sections, so don't allocate space.  */
	continue;

      if (strip)
	{
	  s->flags |= SEC_EXCLUDE;
	  continue;
	}

      /* Allocate memory for the section contents.  We use bfd_zalloc
	 here in case unused entries are not reclaimed before the
	 section's contents are written out.  This should not happen,
	 but this way if it does, we get a R_SPU_NONE reloc instead of
	 garbage.  */
      s->contents = (bfd_byte *) bfd_zalloc (dynobj, s->rawsize);
      if (s->contents == NULL && s->rawsize != 0)
	return FALSE;
    }

  return TRUE;
}

/* Create dynamic sections when linking against a dynamic object.  */

static bfd_boolean
spu_elf_create_dynamic_sections (bfd *abfd, struct bfd_link_info *info)
{
  flagword flags;
  asection *s;
  const struct elf_backend_data *bed = get_elf_backend_data (abfd);

  /* We need to create .dynbss, and .rel[a].bss sections.  */

  flags = (SEC_ALLOC | SEC_LOAD | SEC_HAS_CONTENTS | SEC_IN_MEMORY
	   | SEC_LINKER_CREATED);

  if (bed->want_dynbss)
    {
      /* The .dynbss section is a place to put symbols which are defined
	 by dynamic objects, are referenced by regular objects, and are
	 not functions.  We must allocate space for them in the process
	 image and use a R_*_COPY reloc to tell the dynamic linker to
	 initialize them at run time.  The linker script puts the .dynbss
	 section into the .bss section of the final image.  */
      s = bfd_make_section (abfd, ".dynbss");
      if (s == NULL
	  || ! bfd_set_section_flags (abfd, s, SEC_ALLOC | SEC_LINKER_CREATED))
	return FALSE;

      /* The .rel[a].bss section holds copy relocs.  This section is not
     normally needed.  We need to create it here, though, so that the
     linker will map it to an output section.  We can't just create it
     only if we need it, because we will not know whether we need it
     until we have seen all the input files, and the first time the
     main linker code calls BFD after examining all the input files
     (size_dynamic_sections) the input sections have already been
     mapped to the output sections.  If the section turns out not to
     be needed, we can discard it later.  We will never need this
     section when generating a shared object, since they do not use
     copy relocs.  */
      if (! info->shared)
	{
	  s = bfd_make_section (abfd,
				(bed->default_use_rela_p
				 ? ".rela.bss" : ".rel.bss"));
	  if (s == NULL
	      || ! bfd_set_section_flags (abfd, s, flags | SEC_READONLY)
	      || ! bfd_set_section_alignment (abfd, s, bed->s->log_file_align))
	    return FALSE;
	}
    }

  return TRUE;
}

static bfd_boolean
spu_elf_adjust_dynamic_symbol (struct bfd_link_info *info ATTRIBUTE_UNUSED,
			       struct elf_link_hash_entry *h ATTRIBUTE_UNUSED)
{
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

#define elf_backend_can_gc_sections     1
#define elf_backend_rela_normal         1


#define bfd_elf32_bfd_reloc_type_lookup		spu_elf_reloc_type_lookup
#define bfd_elf32_bfd_get_relocated_section_contents _bfd_elf_spu_get_relocated_section_contents

#define elf_info_to_howto			spu_elf_info_to_howto
#define elf_info_to_howto_rel			spu_elf_info_to_howto_rel
#define elf_backend_relocate_section		spu_elf_relocate_section
#define elf_backend_final_write_processing	spu_elf_final_write_processing
#define elf_backend_gc_mark_hook		spu_elf_gc_mark_hook
#define elf_backend_gc_sweep_hook		spu_elf_gc_sweep_hook
#define elf_backend_adjust_dynamic_symbol	spu_elf_adjust_dynamic_symbol
#define elf_backend_check_relocs		spu_elf_check_relocs
                                                                                        
#define elf_backend_create_dynamic_sections	spu_elf_create_dynamic_sections
#define elf_backend_finish_dynamic_sections	spu_elf_finish_dynamic_sections
#define elf_backend_finish_dynamic_symbol	spu_elf_finish_dynamic_symbol
#define elf_backend_size_dynamic_sections	spu_elf_size_dynamic_sections

#define TARGET_BIG_SYM		bfd_elf32_spu_vec
#define TARGET_BIG_NAME		"elf32-spu"
#define ELF_ARCH		bfd_arch_spu
#define ELF_MACHINE_CODE	EM_SPU
#define ELF_MAXPAGESIZE  	0x80	/* This matches the alignment need for DMA. */

#if defined(BPA)
#define elf_backend_post_process_headers        spu_elf_post_process_headers
#define elf_backend_section_processing          spu_elf_section_processing
#define elf_backend_always_size_sections        spu_elf_always_size_sections
#define elf_backend_special_sections	      	spu_elf_special_sections
#endif

#include "elf32-target.h"
