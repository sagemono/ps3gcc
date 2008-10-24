SCRIPT_NAME=elf
TEMPLATE_NAME=elf32
GENERATE_SHLIB_SCRIPT=yes
GENERATE_PIE_SCRIPT=yes
EXTRA_EM_FILE=spu32elf
OUTPUT_FORMAT="elf32-spu"
ARCH=spu
MACHINE=
TEXT_START_ADDR=0x0000080
ALIGNMENT=16
OTHER_END_SYMBOLS='PROVIDE (__stack = 0x3fff0);'
DISCARD_LTO="/DISCARD/ : { *(.lto*) }"
NO_SMALL_DATA=true
EMBEDDED=true
MAXPAGESIZE=0x80
DATA_ADDR="ALIGN(${MAXPAGESIZE})"
SHLIB_DATA_ADDR="ALIGN(${MAXPAGESIZE})"
OTHER_SECTIONS=".note.spu_name 0 : { KEEP(*(.note.spu_name)) }"
GENERATE_SHLIB_SCRIPT=yes
OTHER_GOT_SECTIONS=".toe ALIGN(128) : { *(.toe) } = 0"

# SCE LOCAL, Bugzilla #2878
# SPURS team requets us to embedded ID at the top of .text section.
# To put the special residential section at the top of .text
# I modified scriptstempl/elf.sc so that it refers to the shell variable
# SPECIAL_SECTION_BEFORE_TEXT_SECTION.
#
# bugzilla 30757
# Add new section .before_text.
INITIAL_READONLY_SECTIONS="
  .SpuGUID ALIGN(128) : ONLY_IF_SPUGUID
  {
        PROVIDE (__SPU_GUID = .);
	LONG(0x0)
	LONG(0x0)
	LONG(0x0)
	LONG(0x0)
  }
  .before_text : { *(.before_text) }
"

# sce local bugzilla 39745
# SPURS team requested us to provide the pair of symbols
# to indentify where is the readonly segment.
SYMBOL_READONLY_SEGMENT_START="  PROVIDE (__ro_segment_start = .); "
SYMBOL_READONLY_SEGMENT_END="  PROVIDE (__ro_segment_end = .); "

TEXT_DYNAMIC=
OTHER_READONLY_SECTIONS="
  .fixup ${RELOCATING-0} : {
    ${RELOCATING+__fixup_start = .;}
    *(.fixup)
  }"
