#! /bin/csh
# SCE CONFIDENTIAL
# Copyright(C) 2010 Sony Computer Entertainment Inc.
# All Rights Reserved.

# This script builds and installs mingw->ppu-lv2 toolchain for SDK3.6.0-GCC411.  The
# existing toolchain directory is backed up as "host-win32/ppu.~N~" where
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

# mingw settings

setenv TOOLDIR $cwd/tool
setenv BUILD i686-pc-linux-gnu
setenv XHOST i386-pc-mingw32msvc

# ppu-lv2 settings

set path = ($TOOLDIR/lm/bin $CELLDIR/host-linux/ppu/bin $path)
setenv TOOLCHAIN_PREFIX $CELLDIR/host-win32/ppu
setenv TOOLCHAIN_SYSROOT $CELLSDK/target/ppu

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

mkdir -p mp-lv2/binutils || exit 1
cd mp-lv2/binutils || exit 1
$SRC/binutils/configure --target=ppu-lv2 --prefix=$TOOLCHAIN_PREFIX \
	--host=$XHOST --build=$BUILD --with-sysroot=$TOOLCHAIN_SYSROOT || exit 1
make || exit 1
make install || exit 1
cd ../..

mkdir -p mp-lv2/gcc || exit 1
cd mp-lv2/gcc || exit 1
# We need to tell configure to use the linux version of ppu-lv2/bin/as so
# the configure tests get the correct answer.
setenv LD $CELLDIR/host-linux/ppu/ppu-lv2/bin/ld
setenv AS $CELLDIR/host-linux/ppu/ppu-lv2/bin/as
$SRC/gcc/configure --target=ppu-lv2 --prefix=$TOOLCHAIN_PREFIX \
	--host=$XHOST --build=$BUILD --with-sysroot=$TOOLCHAIN_SYSROOT \
	--disable-threads --disable-shared --disable-hosted-libstdcxx \
	--enable-languages="c,c++" || exit 1
unsetenv LD
unsetenv AS
make || exit 1
make install || exit 1
# Remove the headers that conflict with dinkumware
set incs = `ppu-lv2-gcc -print-file-name=include`
set incd = `echo $incs | sed -e "s:/host-linux/:/host-win32/:"`
foreach h (float.h spe.h stdbool.h varargs.h iso646.h \
	stddef.h syslimits.h limits.h stdarg.h)
  rm -f $incd/$h || exit 1
end
cd ../..

cp -af $SRC/gcc/COPYING{,.LIB} $TOOLCHAIN_PREFIX/ || exit 1
