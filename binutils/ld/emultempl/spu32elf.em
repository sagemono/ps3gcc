cat >>e${EMULATION_NAME}.c <<EOF

static void
spu_before_allocation (void)
{
  if (!link_info.relocatable)
    {
      expld.phase = lang_mark_phase_enum;
      expld.dataseg.phase = exp_dataseg_none;
      one_lang_size_sections_pass (NULL, TRUE);
      lang_reset_memory_regions ();
    }

  gld${EMULATION_NAME}_before_allocation ();
}

EOF

LDEMUL_BEFORE_ALLOCATION=spu_before_allocation

