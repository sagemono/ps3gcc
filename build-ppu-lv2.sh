#! /bin/csh
# SCE CONFIDENTIAL
# Copyright(C) 2009 Sony Computer Entertainment Inc.
# All Rights Reserved.

# This script builds and installs ppu-lv2 toolchain for SDK3.0.0-GCC411.  The
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

cp -af $SRC/gcc/COPYING{,.LIB} $TOOLCHAIN_PREFIX/ || exit 1
