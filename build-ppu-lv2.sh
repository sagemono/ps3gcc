#! /bin/csh
# SCE CONFIDENTIAL
# Copyright(C) 2007 Sony Computer Entertainment Inc.
# All Rights Reserved.

# This script builds and installs ppu-lv2 toolchain for SDK1.9.0-GCC402.  The
# existing toolchain directory is backed up as "host-linux/ppu.~N~" where
# N is a generated number.
# If environment variable CELLSDK is set, it should be the directory where
# SDK is installed.  The default is "/usr/local/cell".
# If environment variable CELLDIR is set, it is used as an installation
# target.  The default is the same directory as CELLSDK.
# If environment variable TOOLCHAIN_BUILDDIR is set, it is used as a working
# directory to build the toolchain.  The default is the current working
# directory.

# common settings

if( ! $?CELLSDK ) setenv CELLSDK /usr/local/cell
if( ! $?CELLDIR ) setenv CELLDIR $CELLSDK
setenv SRC `cd $0:h; echo $cwd`
if( $?TOOLCHAIN_BUILDDIR ) then
  if( -d $TOOLCHAIN_BUILDDIR ) cd $TOOLCHAIN_BUILDDIR
endif

# ppu-lv2 settings

setenv TOOLCHAIN_PREFIX $CELLDIR/host-linux/ppu
setenv TOOLCHAIN_SYSROOT $CELLSDK/target/ppu
set path = ($TOOLCHAIN_PREFIX/bin $path)

# build & install

if( -e $TOOLCHAIN_PREFIX ) then
  set backup_n=1
  while( -e $TOOLCHAIN_PREFIX.~${backup_n}~ )
    @ backup_n++
  end
  set backup =  $TOOLCHAIN_PREFIX.~${backup_n}~
  mkdir $backup || exit 1
  (cd $TOOLCHAIN_PREFIX/ && tar cf - .) | (cd $backup && tar xf -) || exit 1
endif

mkdir -p ppu-lv2/binutils || exit 1
cd ppu-lv2/binutils || exit 1
$SRC/binutils/configure --target=ppu-lv2 --prefix=$TOOLCHAIN_PREFIX \
	--with-sysroot=$TOOLCHAIN_SYSROOT || exit 1
make || exit 1
make install || exit 1
rehash
cd ../..

mkdir -p ppu-lv2/gcc || exit 1
cd ppu-lv2/gcc || exit 1
$SRC/gcc/configure --target=ppu-lv2 --prefix=$TOOLCHAIN_PREFIX \
	--with-sysroot=$TOOLCHAIN_SYSROOT --with-headers \
	--disable-shared --enable-languages="c,c++" || exit 1
make || exit 1
make install || exit 1
rehash
# Remove the headers that conflict with dinkumware
set inc = `ppu-lv2-gcc -print-file-name=include`
foreach h (float.h math.h spe.h stdbool.h stdlib.h varargs.h iso646.h \
	stddef.h syslimits.h limits.h stdarg.h stdio.h)
  rm -f $inc/$h || exit 1
end
cd ../..

mkdir -p ppu-lv2/libsupcxx || exit 1
cd ppu-lv2/libsupcxx || exit 1
ppu-lv2-g++ -c -O2 -g -mno-altivec -I $SRC/libsupcxx \
	$SRC/libsupcxx/*.cc || exit 1
ppu-lv2-ar cr libsupc++.a *.o || exit 1
foreach d (fno-exceptions fno-exceptions/fno-rtti)
  mkdir -p $d || exit 1
  pushd $d || exit 1
  foreach f (eh_alloc.cc eh_aux_runtime.cc eh_catch.cc eh_globals.cc \
	eh_personality.cc eh_terminate.cc eh_throw.cc pure.cc tinfo.cc vec.cc)
    # BUG: -fno-rtti should be added
    ppu-lv2-g++ -fno-exceptions -c -O2 -g -mno-altivec -I $SRC/libsupcxx \
	$SRC/libsupcxx/$f || exit 1
  end
  ppu-lv2-ar cr libsupc++.a *.o || exit 1
  popd
end
set incdir = `ppu-lv2-gcc -print-file-name=include`
cp -af $SRC/libsupcxx/cxxabi.h $incdir/ || exit 1
foreach d (. fno-exceptions fno-exceptions/fno-rtti)
  cp -af $d/libsupc++.a $incdir/../$d || exit 1
end
cd ../..

cp -af $SRC/gcc/COPYING{,.LIB} $TOOLCHAIN_PREFIX/ || exit 1
