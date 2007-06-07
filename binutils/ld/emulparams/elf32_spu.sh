SCRIPT_NAME=elf
TEMPLATE_NAME=elf32
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
GENERATE_SHLIB_SCRIPT=yes
OTHER_GOT_SECTIONS=".toe ALIGN(128) : { *(.toe) } = 0"

# SCE LOCAL, Bugzilla #2878
# SPURS team requets us to embedded ID at the top of .text section.
# To put the special residential section at the top of .text
# I modified scriptstempl/elf.sc so that it refers to the shell variable
# SPECIAL_SECTION_BEFORE_TEXT_SECTION.
SPECIAL_SECTION_BEFORE_TEXT_SECTION="
  .SpuGUID ALIGN(128) : ONLY_IF_SPUGUID
  {
        PROVIDE (__SPU_GUID = .);
	LONG(0x0)
	LONG(0x0)
	LONG(0x0)
	LONG(0x0)
  }
"

