[SCE CONFIDENTIAL DOCUMENT]
Toolchain src GCC41
                    Copyright(C) 2011 Sony Computer Entertainment Inc.
                                                   All Rights Reserved.
======================================================================
This package contains source codes for GCC 4.1 provided in 
PlayStation(R)3 Programmer Tool Toolchain package.

GCC 4.1 uses the software which applies the GNU General Public License 
Version 3 (GPLv3) and the terms added according to section 7 of GPLv3, 
and this package provides the target source codes.
Please see the following COPYING3 files for the information of license 
and copyright. 

  binutils/COPYING3
  gcc/COPYING3

----------------------------------------------------------------------
Contents of This Package
----------------------------------------------------------------------
Readme_j.txt
Readme_e.txt
build.sh                script for rebuilding the toolchain for Linux
build-ppu-lv2.sh        script for rebuilding Lv-2 PPU toolchain
build-spu-lv2.sh        script for rebuilding Lv-2 SPU toolchain
build-mingw.sh          script for rebuilding the toolchain for Windows
build-lm.sh             script for rebuilding Linux->Windows toolchain
build-mp-lv2.sh         script for rebuilding Lv-2 PPU toolchain for Windows
build-ms-lv2.sh         script for rebuilding Lv-2 SPU toolchain for Windows
diff-from-gcc           differences from GCC 4.1.1 and Binutils 2.17
                        Only the changes made by our company are included.
binutils/               GNU development tools source file
gcc/                    GNU Compiler Collection (GCC) source file
mingw/                  place for downloaded MinGW runtime

----------------------------------------------------------------------
How to rebuild the toolchain
----------------------------------------------------------------------
When rebuilding the toolchain using this package, please make sure 
to install PlayStation(R)3 Programmer toolchain in the default directory 
(/usr/local/cell) or the directory specified by CELLSDK environment variable.

Invoking build.sh will rebuild the toolchain for Linux.  Script build.sh
uses build-ppu-lv2.sh and build-spu-lv2.sh.  You can change the toolchain
install directory and the rebuild working directory by setting
environment variables.  See comments in script files for details.

Example:
    mkdir work/src
    unzip toolchain-src-3.7.0-GCC411.zip -d work/src
    mkdir work/build
    cd work/build
    ../src/build.sh

Script build-mingw.sh will rebuild the toolchain for Windows.  The script
runs on Linux environment.  MinGW runtime is required for rebuilding.
Download the following two files, and put them in "mingw" directory.
  mingw-runtime-3.3.tar.gz
  w32api-2.5.tar.gz
    http://ftp.jaist.ac.jp/pub/sourceforge/m/project/mi/mingw/OldFiles/

The script build-mingw.sh uses build-lm.sh, build-mp-lv2.sh and
build-ms-lv2.sh.  You can change the toolchain install directory and the
rebuild working directory by setting environment variables.  See comments
in script files for details.

Please note that the specifications are preliminary and subject to change 
without prior notice.  

----------------------------------------------------------------------
Changes
----------------------------------------------------------------------
Source files included in the package are modified based on the source 
codes of GCC 4.1 distributed through the following URL.
Differences from the original codes are shown with diff-from-gcc file.

http://gcc.gnu.org/mirrors.html

Any right concerning the modified parts belongs to Sony Computer 
Entertainment Inc. 
The copyrighted work as a whole including SCE's modification shall be 
licensed based on GPLv3, and individual applicable files shall be 
licensed based on the terms added according to section 7 of GPLv3.

----------------------------------------------------------------------
