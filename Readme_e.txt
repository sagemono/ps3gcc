[SCE CONFIDENTIAL DOCUMENT]
                    Copyright(C) 2008 Sony Computer Entertainment Inc.
                                                   All Rights Reserved.
======================================================================
This package includes the source files of Cell OS Lv-2 toolchain.  When
rebuilding the toolchain using this package, please make sure to install
Cell OS Lv-2 toolchain in the default directory (/usr/local/cell)
or the directory specified by CELLSDK environment variable.

Invoking build.sh will rebuild the toolchain for Linux.  Script build.sh
uses build-ppu-lv2.sh and build-spu-lv2.sh.  You can change the toolchain
install directory and the rebuild working directory by setting
environment variables.  See comments in script files for details.
Example:
    mkdir work/src
    unzip toolchain-src-2.5.0-GCC411.zip -d work/src
    mkdir work/build
    cd work/build
    ../src/build.sh
Script build-mingw.sh will rebuild the toolchain for Windows.  The script
runs on Linux environment.  MinGW runtime is required for rebuilding.
Download the following two files, and put them in "mingw" directory.
  mingw-runtime-3.3.tar.gz
    http://sourceforge.net/forum/forum.php?forum_id=372259
  w32api-2.5.tar.gz
    http://sourceforge.net/forum/forum.php?forum_id=352558
The script build-mingw.sh uses build-lm.sh, build-mp-lv2.sh and
build-ms-lv2.sh.  You can change the toolchain install directory and the
rebuild working directory by setting environment variables.  See comments
in script files for details.

Please note that the specifications are preliminary and subject to change 
without prior notice.  
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
Permission and Restrictions on Use
----------------------------------------------------------------------
The permission and restrictions on using this package conform to 
the contract concluded between your company and our company (Sony 
Computer Entertainment Inc).  
----------------------------------------------------------------------
Note on Trademarks
----------------------------------------------------------------------
All other product and company names mentioned herein, with or without 
the registered trademark symbol (R) or trademark symbol (TM), are 
generally registered trademarks and/or trademarks of their respective 
owners. 
