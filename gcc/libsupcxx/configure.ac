# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(package-unused, version-unused,, libsupc++)
AC_CONFIG_SRCDIR([eh_alloc.cc])

AM_ENABLE_MULTILIB(, ..)

AC_CANONICAL_SYSTEM
target_alias=${target_alias-$host_alias}
AC_SUBST(target_alias)

GCC_NO_EXECUTABLES

AM_INIT_AUTOMAKE([no-define foreign no-dependencies])

# Checks for programs.
AC_PROG_CXX
AC_PROG_RANLIB


# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stddef.h unistd.h])


# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T
AC_C_VOLATILE
AC_CHECK_TYPES([ptrdiff_t])

gcc_version='`cat ${top_srcdir}/../gcc/BASE-VER`'
AC_SUBST(gcc_version)

toolexecdir='$(libdir)/gcc/$(target_alias)'
toolexeclibdir='${toolexecdir}/${gcc_version}$(MULTISUBDIR)'
AC_SUBST(toolexecdir)
AC_SUBST(toolexeclibdir)

libsupcxx_include_dir='${toolexecdir}/${gcc_version}/include'
AC_SUBST(libsupcxx_include_dir)

if test ${multilib} = yes; then
  multilib_arg="--enable-multilib"
else
  multilib_arg=
fi

case "$target_alias" in
  ppu-lv2)
    am_cxxflags="-mno-altivec -Wall" 
    ;;
  spu-lv2)
    am_cxxflags="-D__cxxabivl=__cxxabiv1 -fPIC -Wall"
    ;;
esac
AC_SUBST(am_cxxflags)

AM_CONDITIONAL(RTTI, test "$with_multisubdir" != "fno-exceptions/fno-rtti")
AM_CONDITIONAL(PPU_LV2, test "$target_alias" = "ppu-lv2")

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

