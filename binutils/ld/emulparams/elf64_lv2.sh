TEMPLATE_NAME=elf32
EXTRA_EM_FILE=ppc64elf
ELFSIZE=64
GENERATE_SHLIB_SCRIPT=yes
GENERATE_PIE_SCRIPT=yes
SCRIPT_NAME=elf64celloslv2  # sce local, Bugzilla #8760
OUTPUT_FORMAT="elf64-powerpc-celloslv2"
TEXT_START_ADDR=0x10000
MAXPAGESIZE=0x10000
COMMONPAGESIZE=0x1000
ARCH=powerpc:common64
MACHINE=
NOP=0x60000000
DISCARD_LTO="/DISCARD/ : { *(.lto*) }"
EXECUTABLE_SYMBOLS='PROVIDE (__stack = 0); PROVIDE (___stack = 0);'
OTHER_BSS_END_SYMBOLS='__end = .;'
CTOR_START='PROVIDE (__CTOR_LIST__ = .); PROVIDE (___CTOR_LIST__ = .);'
CTOR_END='PROVIDE (__CTOR_END__ = .); PROVIDE (___CTOR_END__ = .);'
DTOR_START='PROVIDE (__DTOR_LIST__ = .); PROVIDE (___DTOR_LIST__ = .);'
DTOR_END='PROVIDE (__DTOR_END__ = .); PROVIDE (___DTOR_END__ = .);'
OTHER_TEXT_SECTIONS="*(.sfpr .glink)"
BSS_PLT=
OTHER_BSS_SYMBOLS="
  .tocbss	${RELOCATING-0}${RELOCATING+ALIGN(8)} : { *(.tocbss)}"
OTHER_PLT_RELOC_SECTIONS="
  .rela.tocbss	${RELOCATING-0} : { *(.rela.tocbss) }"

if test x${RELOCATING+set} = xset; then
  GOT="
  .got		ALIGN(8) : { *(.got .toc) }"
else
  GOT="
  .got		0 : { *(.got) }
  .toc		0 : { *(.toc) }"
fi
OTHER_GOT_RELOC_SECTIONS="
  .rela.toc	${RELOCATING-0} : { *(.rela.toc) }"
OTHER_READWRITE_SECTIONS="
  .toc1		${RELOCATING-0}${RELOCATING+ALIGN(8)} : { *(.toc1) }
  .opd		${RELOCATING-0}${RELOCATING+ALIGN(8)} : { KEEP (*(.opd)) }"

# Treat a host that matches the target with the possible exception of "64"
# in the name as if it were native.
if test `echo "$host" | sed -e s/64//` = `echo "$target" | sed -e s/64//`; then
  case " $EMULATION_LIBPATH " in
    *" ${EMULATION_NAME} "*)
      NATIVE=yes
      ;;
  esac
fi

# Bugzilla #8760
# I don't know if linker requires "-z combreloc" option in case of
# linking Lv2 OS application, but all of programs have been linked with
# "-z combreloc" option. So I keep the option specified.
#
# The genscript.sh, which is used to generate eelf64_lv2.c, enables
# combreloc link model only if GENERATE_COMBRELOC_SCRIT is asserted.
# but GENERATE_COMBRELOC_SCRIT is 1 only if #SCRIPT_NAME == "elf.sc".
# So I explicitly assert the flag to support combreloc layout model.
GENERATE_COMBRELOC_SCRIPT=yes
