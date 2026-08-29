#! /bin/csh
# SCE CONFIDENTIAL
# Copyright(C) 2012 Sony Computer Entertainment Inc.
# All Rights Reserved.

# This script builds the following toolchain for SDK4.2.0-GCC411.
#  linux->mingw, mingw->pu-lv2, mingw->spu-lv2
# See each build script for details.
$0:h/build-lm.sh || exit 1
$0:h/build-mp-lv2.sh || exit 1
$0:h/build-ms-lv2.sh || exit 1
