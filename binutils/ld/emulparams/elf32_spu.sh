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

SYMBOL_DATA_SEGMENT_START="PROVIDE (__data_start = .);"

# Symbols used to initialze bss using the following assembly code:
#	ila	$6,__bss_start
# 	ai	$6,$6,__ABS__bss_init_offset\n"
# 	hbrr	2f,__ABS__bss_init_hint\n"
# 	ila	$5,__ABS__bss_init_iters\n"
# 	fsmbi	$3,0\n"
# 	ori	$4,$6,0\n"
# 	br	__ABS__bss_init_jump\n"
# 1:	ai	$5,$5,-1\n"
# 	stqd	$3,-(16*8)($6)\n"
# 	stqd	$3,-(16*7)($4)\n"
# 	stqd	$3,-(16*6)($4)\n"
# 	stqd	$3,-(16*5)($4)\n"
# 	stqd	$3,-(16*4)($4)\n"
# 	stqd	$3,-(16*3)($4)\n"
# 	stqd	$3,-(16*2)($4)\n"
# 	ai	$6,$4,16*8\n"
# 	stqd	$3,-(16*1)($4)\n"
# 	ai	$4,$4,16*8\n"
# 2:	brnz	$5,1b"
OTHER_BSS_END_SYMBOLS="
  PROVIDE_HIDDEN (__ABS__bss_init_iters = ((. - __bss_start) / 16) / 8);
  PROVIDE_HIDDEN (__ABS__bss_init_offset = ((. - __bss_start) + 16 * 7) % (16 * 8) + 16);
  PROVIDE_HIDDEN (__ABS__bss_init_hint = 4 * ((. - __bss_start) / 16 > 8 ? 5 : 17));
  PROVIDE_HIDDEN (__ABS__bss_init_jump = 4 * ((. - __bss_start) == 0
					? 13
					: ((. - __bss_start) / 16) % 8 == 0
					  ? 1
					  : 10 - ((. - __bss_start) / 16) % 8));"

TEXT_DYNAMIC=
OTHER_READONLY_SECTIONS="
  .fixup ${RELOCATING-0} : {
    *(.fixup_head)
    *(.fixup)
  }"

INIT_START='KEEP (*(.init.head)) KEEP(*(.init.fixups)) *(.init.zerobss) *(.init.ctors)'
INIT_END='KEEP (*(.init.tail));'
FINI_START='KEEP (*(.fini.head));'
FINI_END='*(.fini.dtors); KEEP (*(.fini.tail));'

CTOR_START='KEEP (*(.ctors_head));'
CTOR_END='KEEP (*(.ctors_tail));'
DTOR_START='KEEP (*(.dtors_head));'
DTOR_END='KEEP (*(.dtors_tail));'
