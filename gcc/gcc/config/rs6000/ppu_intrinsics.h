/*
 * SCE CONFIDENTIAL
 * $PSLibId$
 * Copyright (C) 2006 Sony Computer Entertainment Inc. 
 * All Rights Reserved.
 */
#ifndef __PPU_INTRINSICS_H__
#define __PPU_INTRINSICS_H__

/* SNC defined both __SNC__ and __GNUC__. Need to detect __SNC__ first */
#if defined (__SNC__)
#include <ppu_intrinsics_snc.h>
#elif defined (__GNUC__)
#include <ppu_intrinsics_gcc.h>
#else
#error "Cannot find the immplementation of ppu_intrinsics.h."
#endif

#endif /* __PPU_INTRINSICS_H__ */
