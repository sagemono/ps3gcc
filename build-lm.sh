#! /bin/csh
# SCE CONFIDENTIAL
# Copyright(C) 2009 Sony Computer Entertainment Inc.
# All Rights Reserved.

# This script builds and installs linux->mingw toolchain for SDK2.7.0-GCC411.
# If environment variable TOOLCHAIN_BUILDDIR is set, it is used as a working
# directory to build the toolchain.  The default is the current working
# directory.
# The toolchain is installed in $TOOLCHAIN_BUILDDIR/tool/lm

# common settings

setenv SRC `cd $0:h; echo $cwd`
if( $?TOOLCHAIN_BUILDDIR ) then
  if( -d $TOOLCHAIN_BUILDDIR ) cd $TOOLCHAIN_BUILDDIR
endif

# mingw settings

setenv TOOLDIR $cwd/tool
setenv XHOST i386-pc-mingw32msvc

# build & install

if(! -f $SRC/mingw/mingw-runtime-3.3.tar.gz || \
   ! -e $SRC/mingw/w32api-2.5.tar.gz) then
  echo "MinGW runtime files are not found in $SRC/mingw.  Please download them."
  exit 1
endif

mkdir -p lm/binutils || exit 1
cd lm/binutils || exit 1
$SRC/binutils/configure --target=$XHOST --prefix=$TOOLDIR/lm || exit 1
make || exit 1
make install || exit 1
cd ../..

mkdir -p $TOOLDIR/lm/$XHOST || exit 1
pushd $TOOLDIR/lm/$XHOST || exit 1
gzip -dc $SRC/mingw/mingw-runtime-3.3.tar.gz | tar xf - || exit 1
gzip -dc $SRC/mingw/w32api-2.5.tar.gz | tar xf - || exit 1
popd

set path = ($TOOLDIR/lm/bin $path)
mkdir -p lm/gcc || exit 1
cd lm/gcc || exit 1
$SRC/gcc/configure --target=$XHOST --prefix=$TOOLDIR/lm \
	--with-headers="$TOOLDIR/lm/$XHOST/include" \
	--with-gnu-as --with-gnu-ld \
	--without-newlib --disable-multilib || exit 1
make || exit 1
make install || exit 1
cd ../..

if( -f $SRC/mingw/libiconv-1.11.tar.gz) then
  mkdir -p lm/iconv || exit 1
  cd lm/iconv || exit 1
  gzip -dc $SRC/mingw/libiconv-1.11.tar.gz | tar xf - || exit 1
  libiconv-1.11/configure --prefix=$TOOLDIR/lm/$XHOST --host=$XHOST \
	--enable-static=yes --enable-shared=no || exit 1
  make || exit 1
  make install || exit 1
  cd ../..
endif
