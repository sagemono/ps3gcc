#! /bin/csh
# SCE CONFIDENTIAL
# Copyright(C) 2005 Sony Computer Entertainment Inc.
# All Rights Reserved.

# This script build the following toolchain for SDK1.8.0.
#  ppu-lv2, spu-lv2
# See each build script for details.
$0:h/build-ppu-lv2.sh || exit 1
$0:h/build-spu-lv2.sh || exit 1
