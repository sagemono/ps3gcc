
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

