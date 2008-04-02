
# This file is sourced from elf32.em, and defines extra spu specific
# features.
#
cat >>e${EMULATION_NAME}.c <<EOF
#include "ldctor.h"
#include "elf32-spu.h"

/* Non-zero to perform stack space analysis.  */
static int stack_analysis = 0;

/* Whether to emit symbols with stack requirements for each function.  */
static int emit_stack_syms = 0;

/* Whether to emit an error when non-PIC objects are linked */
static int flag_pic;

/* Range of valid addresses for loadable sections.  */
static bfd_vma local_store_lo = 0;
static bfd_vma local_store_hi = 0x3ffff;


static int
is_spu_target (void)
{
  extern const bfd_target bfd_elf32_spu_vec;

  return link_info.hash->creator == &bfd_elf32_spu_vec;
}

/* Create our note section.  */

static void
spu_after_open (void)
{
  if (is_spu_target ()
      && !link_info.relocatable
      && link_info.input_bfds != NULL
      && !spu_elf_create_sections (output_bfd, &link_info,
				   stack_analysis, emit_stack_syms, flag_pic))
    einfo ("%X%P: can not create note section: %E\n");

  gld${EMULATION_NAME}_after_open ();
}

/* Go find if we need to do anything special for overlays.  */

static void
spu_before_allocation (void)
{
  if (is_spu_target ()
      && !link_info.relocatable)
    {
      /* Size the sections.  This is premature, but we need to know the
	 rough layout so that overlays can be found.  */
      expld.phase = lang_mark_phase_enum;
      expld.dataseg.phase = exp_dataseg_none;
      one_lang_size_sections_pass (NULL, TRUE);

      /* We must not cache anything from the preliminary sizing.  */
      lang_reset_memory_regions ();
    }

  gld${EMULATION_NAME}_before_allocation ();
}

/* Final emulation specific call.  */

static void
spu_finish (void)
{
  /* Call the elf32.em routine.  */
  gld${EMULATION_NAME}_finish ();

  if (is_spu_target () && local_store_lo < local_store_hi)
    {
      asection *s;

      s = spu_elf_check_vma (output_bfd, local_store_lo, local_store_hi);
      if (s != NULL)
	einfo ("%X%P: %A exceeds local store range\n", s);
    }
}

EOF

# Define some shell vars to insert bits of code into the standard elf
# parse_args and list_options functions.
#
PARSE_AND_LIST_PROLOGUE='
#define OPTION_SPU_PLUGIN		301
#define OPTION_SPU_LOCAL_STORE		(OPTION_SPU_PLUGIN + 1)
#define OPTION_SPU_STACK_ANALYSIS	(OPTION_SPU_LOCAL_STORE + 1)
#define OPTION_SPU_STACK_SYMS		(OPTION_SPU_STACK_ANALYSIS + 1)
#define OPTION_SPU_WARN_PIC_ALL		(OPTION_SPU_STACK_SYMS + 1)
#define OPTION_SPU_WARN_PIC_CODE	(OPTION_SPU_WARN_PIC_ALL + 1)
'

PARSE_AND_LIST_LONGOPTS='
  { "plugin", no_argument, NULL, OPTION_SPU_PLUGIN },
  { "local-store", required_argument, NULL, OPTION_SPU_LOCAL_STORE },
  { "stack-analysis", no_argument, NULL, OPTION_SPU_STACK_ANALYSIS },
  { "emit-stack-syms", no_argument, NULL, OPTION_SPU_STACK_SYMS },
  { "warn-pic-all", no_argument, NULL, OPTION_SPU_WARN_PIC_ALL },
  { "warn-pic-code", no_argument, NULL, OPTION_SPU_WARN_PIC_CODE },
'

PARSE_AND_LIST_OPTIONS='
  fprintf (file, _("\
  --plugin              Make SPU plugin.\n\
  --local-store=lo:hi   Valid address range.\n\
  --stack-analysis      Estimate maximum stack requirement.\n\
  --emit-stack-syms     Add __stack_func giving stack needed for each func.\n\
  --warn-pic-all        Warn about non-PIC references in code or data.\n\
  --warn-pic-code       Warn about non-PIC references in code.\n"
		   ));
'

PARSE_AND_LIST_ARGS_CASES='
    case OPTION_SPU_PLUGIN:
      spu_elf_plugin (1);
      break;

    case OPTION_SPU_LOCAL_STORE:
      {
	char *end;
	local_store_lo = strtoul (optarg, &end, 0);
	if (*end == '\'':'\'')
	  {
	    local_store_hi = strtoul (end + 1, &end, 0);
	    if (*end == 0)
	      break;
	  }
	einfo (_("%P%F: invalid --local-store address range `%s'\''\n"), optarg);
      }
      break;

    case OPTION_SPU_STACK_ANALYSIS:
      stack_analysis = 1;
      break;

    case OPTION_SPU_STACK_SYMS:
      emit_stack_syms = 1;
      break;

    case OPTION_SPU_WARN_PIC_ALL:
      flag_pic = 2;
      break;

    case OPTION_SPU_WARN_PIC_CODE:
      flag_pic = 1;
      break;
'

LDEMUL_AFTER_OPEN=spu_after_open
LDEMUL_BEFORE_ALLOCATION=spu_before_allocation
LDEMUL_FINISH=spu_finish
