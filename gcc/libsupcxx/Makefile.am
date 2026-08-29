## Makefile for the GNU C++ Support library.
##
## Copyright (C) 2000, 2001, 2002, 2003, 2004 Free Software Foundation, Inc.
##
## Process this file with automake to produce Makefile.in.
##
## This file is part of GCC.
##
## GCC is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2, or (at your option)
## any later version.
##
## GCC is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with GCC; see the file COPYING.  If not, write to
## the Free Software Foundation, 51 Franklin Street, Fifth Floor,
## Boston, MA 02110-1301, USA.

ACLOCAL_AMFLAGS = -I ../config

toolexeclib_LIBRARIES = libsupc++.a

if PPU_LV2
if RTTI
  rtti_source = tinfo2.cc
endif
endif

if PPU_LV2
  ppu_lv2_source = \
	eh_alloc.cc \
	eh_catch.cc \
	eh_globals.cc \
	eh_personality.cc \
	eh_terminate.cc \
	eh_throw.cc \
	guard.cc \
	vec.cc
endif

sources = \
	eh_aux_runtime.cc \
	pure.cc \
	tinfo.cc

libsupc___a_SOURCES = $(sources) $(ppu_lv2_source) $(rtti_source)

libsupcxxinstalldir = $(libsupcxx_include_dir)
libsupcxxinstall_HEADERS = cxxabi.h

install-libsupcxxinstallHEADERS: $(libsupcxxinstall_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(libsupcxxinstalldir)
	@list='$(libsupcxxinstall_HEADERS)'; for p in $$list; do \
	  q=`echo $$p | sed -e 's,.*/,,'`; \
	  if test -f "$$p"; then d= ; else d="$(srcdir)/"; fi; \
	  echo " $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libsupcxxinstalldir)/$$q"; \
	  $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libsupcxxinstalldir)/$$q; \
	done

uninstall-libsupcxxinstallHEADERS:
	@$(NORMAL_UNINSTALL)
	list='$(libsupcxxinstall_HEADERS)'; for p in $$list; do \
	  q=`echo $$p | sed -e 's,.*/,,'`; \
	  rm -f $(DESTDIR)$(libsupcxxinstalldir)/$$q; \
	done

AM_CXXFLAGS = $(am_cxxflags)

AM_MAKEFLAGS = \
	"AR_FLAGS=$(AR_FLAGS)" \
	"CC_FOR_BUILD=$(CC_FOR_BUILD)" \
	"CC_FOR_TARGET=$(CC_FOR_TARGET)" \
	"CFLAGS=$(CFLAGS)" \
	"CXXFLAGS=$(CXXFLAGS)" \
	"CFLAGS_FOR_BUILD=$(CFLAGS_FOR_BUILD)" \
	"CFLAGS_FOR_TARGET=$(CFLAGS_FOR_TARGET)" \
	"INSTALL=$(INSTALL)" \
	"INSTALL_DATA=$(INSTALL_DATA)" \
	"INSTALL_PROGRAM=$(INSTALL_PROGRAM)" \
	"INSTALL_SCRIPT=$(INSTALL_SCRIPT)" \
	"LDFLAGS=$(LDFLAGS)" \
	"LIBCFLAGS=$(LIBCFLAGS)" \
	"LIBCFLAGS_FOR_TARGET=$(LIBCFLAGS_FOR_TARGET)" \
	"MAKE=$(MAKE)" \
	"MAKEINFO=$(MAKEINFO) $(MAKEINFOFLAGS)" \
	"PICFLAG=$(PICFLAG)" \
	"PICFLAG_FOR_TARGET=$(PICFLAG_FOR_TARGET)" \
	"SHELL=$(SHELL)" \
	"RUNTESTFLAGS=$(RUNTESTFLAGS)" \
	"exec_prefix=$(exec_prefix)" \
	"infodir=$(infodir)" \
	"libdir=$(libdir)" \
	"includedir=$(includedir)" \
	"prefix=$(prefix)" \
	"tooldir=$(tooldir)" \
	"AR=$(AR)" \
	"AS=$(AS)" \
	"LD=$(LD)" \
	"RANLIB=$(RANLIB)" \
	"NM=$(NM)" \
	"NM_FOR_BUILD=$(NM_FOR_BUILD)" \
	"NM_FOR_TARGET=$(NM_FOR_TARGET)" \
	"DESTDIR=$(DESTDIR)" \
	"WERROR=$(WERROR)"

