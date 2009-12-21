#! /bin/csh
# SCE CONFIDENTIAL
# Copyright(C) 2009 Sony Computer Entertainment Inc.
# All Rights Reserved.

# This script builds and installs mingw->spu-lv2 toolchain for SDK2.8.0-GCC411-Jul29.  The
# existing toolchain directory is backed up as "host-win32/spu.~N~" where
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

# spu-lv2 settings

set path = ($TOOLDIR/lm/bin $CELLDIR/host-linux/spu/bin $path)
setenv TOOLCHAIN_PREFIX $CELLDIR/host-win32/spu
setenv TOOLCHAIN_SYSROOT $CELLSDK/target/spu

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

mkdir -p ms-lv2/binutils || exit 1
cd ms-lv2/binutils || exit 1
setenv CFLAGS "-DBPA -O2 -g"
$SRC/binutils/configure --target=spu-lv2 --prefix=$TOOLCHAIN_PREFIX \
	--host=$XHOST --build=$BUILD --with-sysroot=$TOOLCHAIN_SYSROOT || exit 1
unsetenv CFLAGS
make || exit 1
make install || exit 1
cd ../..

mkdir -p ms-lv2/gcc || exit 1
cd ms-lv2/gcc || exit 1
setenv LD $CELLDIR/host-linux/spu/spu-lv2/bin/ld
setenv AS $CELLDIR/host-linux/spu/spu-lv2/bin/as
$SRC/gcc/configure --target=spu-lv2 --prefix=$TOOLCHAIN_PREFIX \
	--host=$XHOST --build=$BUILD --with-sysroot=$TOOLCHAIN_SYSROOT \
	--disable-threads --disable-shared --disable-hosted-libstdcxx \
	--enable-languages="c,c++" || exit 1
make || exit 1
make install || exit 1
# Remove the headers that conflict with dinkumware
set incs = `spu-lv2-gcc -print-file-name=include`
set incd = `echo $incs | sed -e "s:/host-linux/:/host-win32/:"`
foreach h (float.h stdbool.h varargs.h iso646.h \
	stddef.h syslimits.h limits.h stdarg.h)
  rm -f $incd/$h || exit 1
end
cd ../..

cp -af $SRC/gcc/COPYING{,.LIB} $TOOLCHAIN_PREFIX/ || exit 1
