#! /bin/csh
# SCE CONFIDENTIAL
# Copyright(C) 2009 Sony Computer Entertainment Inc.
# All Rights Reserved.

# This script builds the following toolchain for SDK2.8.0-GCC411-Jul29.
#  ppu-lv2, spu-lv2
# See each build script for details.
$0:h/build-ppu-lv2.sh || exit 1
$0:h/build-spu-lv2.sh || exit 1
