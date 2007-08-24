/* Copyright (C) 2006 Sony Computer Entertainment, Inc.,

   LTO is free software; you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free
   Software Foundation; either version 2, or (at your option) any later
   version.

   LTO is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
   for more details.

   You should have received a copy of the GNU General Public License
   along with LTO; see the file COPYING.  If not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA.  */


/*
// Import this from gcc!
*/
extern int max_label_num (void);

#include "lto/lto-info-asm.h"
#include <string.h>

/*
// Import this from gcc!
*/
extern int max_label_num (void);

static int lto_init = 0;

static const char *lto_info_names[256] = {NULL};

static annotation_kind_t lto_kind;
static unsigned lto_prev_max_label = 0;
static unsigned lto_max_label;
static unsigned lto_sub_label;
static unsigned lto_uniquen = 0;
static const char *lto_linkonce_name = NULL;

static void
lto_asm_init( void )
{
#define LTO_INFO(enum,code,name,argspec) lto_info_names[enum] = #enum;
#include "lto/lto-info.def"
#undef LTO_INFO
    lto_init = 1;
}

#define LTO_TAB "\t\t\t"

static void
lto_header( const char *pre, FILE * file )
{
    if( !lto_init ) lto_asm_init();
    lto_max_label = max_label_num();
    if( lto_max_label != lto_prev_max_label ) {
        lto_prev_max_label = lto_max_label;
        lto_uniquen = 0;
    }
    lto_sub_label = lto_uniquen++;
    fprintf( file, "%s\t\t.L_lto_%u_%u:\n", pre, lto_max_label, lto_sub_label );
    if( lto_linkonce_name == NULL ) {
	fprintf( file, LTO_TAB ".pushsection .lto_info\n" );
    } else {
	fprintf( file, LTO_TAB ".pushsection .gnu.linkonce.lto.t."
			"%s,\"G\",@progbits,%s,comdat\n",
		lto_linkonce_name, lto_linkonce_name );
    }
}

static void
lto_boilerplate_start( FILE * file, annotation_kind_t kind )
{
    lto_kind = kind;
    fprintf( file, LTO_TAB "/* %s", lto_info_names[kind] );
}

static void
lto_boilerplate_end( FILE * file )
{
    fprintf( file, " */\n"
		LTO_TAB ".byte %u\n"
		LTO_TAB ".int .L_lto_%u_%u\n",
		lto_kind, lto_max_label, lto_sub_label );
}

static void
lto_footer( FILE * file, const char *post )
{
    fprintf( file, LTO_TAB ".popsection\n%s", post );
}

static void
print_regrange( FILE * f, char c, unsigned r, unsigned n )
{
const char *sep;
unsigned i;
    if( n == 0 ) return;
    sep = "";
    if( n <= 2 ) {
	for( i=0; i<n; i++ ) {
	    fprintf( f, "%s%c%u", sep, c, r+i );
	    sep = " ";
	}
    } else {
	fprintf( f, "%s%c%u-%c%u", sep, c, r, c, r+n-1 );
    }
}

static void
print_regmask( FILE * f, char c, unsigned mask )
{
const char *sep;
unsigned r, len;
    if( mask == 0 ) {
	fprintf( f, " ()" );
	return;
    }
    fprintf( f, " (" );
    sep = "";
    r = 0;
    while( mask ) {
	// find next member of set
	while( (mask & 1) == 0 ) {
	    mask >>= 1;
	    r += 1;
	}
	// find length of member range
	len = 0;
	while( mask & 1 ) {
	    mask >>= 1;
	    len += 1;
	}
	fprintf( f, "%s", sep );
	print_regrange( f, c, r, len );
	sep = " ";
	r += len;
    }
    fprintf( f, ")" );
}

static void
print_fn_proto_readably( FILE * file,
		    lto_args_kind_t args_kind, long long unsigned args )
{
lto_spuargs_t spuargs;
lto_ppuargs_t ppuargs;
unsigned n;
char c;
    switch( args_kind ) {
	case LTO_ARGS_SPU:
	    spuargs.u = args;
	    fprintf( file, " spu:" );
	    n = spuargs.f.arg_nregs;
	    fprintf( file, " (" );
	    print_regrange( file, 'r', spuargs.f.arg_firstreg, n );
	    fprintf( file, ")" );
	    n = spuargs.f.result_nregs;
	    if( n != 0 ) {
		fprintf( file, " -> (" );
		print_regrange( file, 'r', spuargs.f.result_firstreg, n );
		fprintf( file, ")" );
	    }
	    break;
	case LTO_ARGS_PPU:
	    ppuargs.u = args;
	    fprintf( file, " ppu:" );
	    print_regmask( file, 'r', ppuargs.f.gpr_argmask );
	    print_regmask( file, 'f', ppuargs.f.fpr_argmask );
	    print_regmask( file, 'v', ppuargs.f.vcr_argmask );
	    n = ppuargs.f.result_nregs;
	    if( n != 0 ) {
		c = " rfv"[ppuargs.f.result_loc];
		fprintf( file, " -> (" );
		print_regrange( file, c, ppuargs.f.result_firstreg, n );
		fprintf( file, ")" );
	    }
	    break;
	default:
	    fprintf( file, " 0x%x: 0x%llx", args_kind, args );
    }
}

void
lto_asm_fn_start( FILE * file, int no_return, int has_nonlocal_label,
		  const char *linkonce_name ) 
{
lto_fstart_flags_t flags;
    lto_linkonce_name = linkonce_name;
    memset( &flags, 0, sizeof(flags) );
    flags.f.no_return = no_return;
    flags.f.lto_version = LTO_CURRENT_VERSION;
    flags.f.has_nonlocal_label = has_nonlocal_label;
    lto_header( "\t", file );
    lto_boilerplate_start( file, LTO_FSTART );
    fprintf( file, " 0x%x", flags.u );
    lto_boilerplate_end( file );
    fprintf( file, LTO_TAB ".int 0x%x\n", flags.u );
    lto_footer( file, "" );
}

void
lto_asm_fn_proto( FILE * file,
		  lto_args_kind_t args_kind, long long unsigned args )
{
unsigned hi = (args >> 32);
unsigned lo = (args & 0xffffffff);
    lto_header( "\t", file );
    lto_boilerplate_start( file, LTO_PROTO );
    print_fn_proto_readably( file, args_kind, args );
    lto_boilerplate_end( file );
    fprintf( file, LTO_TAB ".int 0x%x, 0x%x\n", lo, hi );
    lto_footer( file, "" );
}

static void
print_fn_call_readably( FILE * file, int sibcall, int no_return,
                 lto_args_kind_t args_kind, long long unsigned args )
{
    if( no_return ) fprintf( file, " noret" );
    if( sibcall )   fprintf( file, " sib" );
    print_fn_proto_readably( file, args_kind, args );
}

void
lto_asm_fn_call( FILE * file, int sibcall, int no_return,
		 lto_args_kind_t args_kind, long long unsigned args )
{
lto_call_flags_t flags;
unsigned hi = (args >> 32);
unsigned lo = (args & 0xffffffff);
    memset( &flags, 0, sizeof(flags) );
    flags.f.no_return = no_return;
    flags.f.sibcall = sibcall;
    lto_header( "\t", file );
    lto_boilerplate_start( file, LTO_CALL );
    print_fn_call_readably( file, sibcall, no_return, args_kind, args );
    lto_boilerplate_end( file );
    fprintf( file, LTO_TAB ".int 0x%x\n", flags.u );
    /* NB: We are assuming here that the host is little-endian (which
	   it will be for the Sony SDK).  */
    fprintf( file, LTO_TAB ".int 0x%x, 0x%x\n", lo, hi );
    lto_footer( file, "" );
}

void
lto_asm_noargs( FILE * file, annotation_kind_t kind )
{
    lto_header( "\t", file );
    lto_boilerplate_start( file, kind );
    lto_boilerplate_end( file );
    lto_footer( file, "" );
}

void
lto_asm_noargs_tab( FILE * file, annotation_kind_t kind )
{
    lto_header( "", file );
    lto_boilerplate_start( file, kind );
    lto_boilerplate_end( file );
    lto_footer( file, "\t" );
}

void
lto_asm_onearg( FILE * file, annotation_kind_t kind, int arg )
{
    lto_header( "\t", file );
    lto_boilerplate_start( file, kind );
    fprintf( file, " 0x%x", arg );
    lto_boilerplate_end( file );
    fprintf( file, LTO_TAB ".int 0x%x\n", arg );
    lto_footer( file, "" );
}

static void
lto_end_jumptable_label( FILE * file, unsigned labelnum )
{
    fprintf( file, ".L_lto_jtend_%u", labelnum );
}

void
lto_asm_tablejump( FILE * file, unsigned labelnum )
{
    lto_header( "\t", file );
    lto_boilerplate_start( file, LTO_TBLJMP );
    fprintf( file, " .L%u ", labelnum );
    lto_end_jumptable_label( file, labelnum );
    lto_boilerplate_end( file );
    fprintf( file, LTO_TAB ".int .L%u\n", labelnum );
    fprintf( file, LTO_TAB ".int " );
    lto_end_jumptable_label( file, labelnum );
    fprintf( file, "\n" );
    lto_footer( file, "" );
}

void
lto_asm_jumptable_end( FILE * file, unsigned labelnum )
{
    fprintf( file, LTO_TAB );
    lto_end_jumptable_label( file, labelnum );
    fprintf( file, ":\n" );
}

void
lto_asm_alias( FILE * file, int is_volatile, long long int info )
{
unsigned hi = ((long long unsigned)info)>>32;
unsigned lo = ((long long unsigned)info) & 0xffffffff;
    lto_header( "", file );
    if( hi == 0 ) {
	lto_boilerplate_start( file, LTO_ALIAS32 );
	fprintf( file, " 0x%x", lo );
	lto_boilerplate_end( file );
	fprintf( file, LTO_TAB ".int 0x%x\n", lo );
    } else {
	lto_boilerplate_start( file, LTO_ALIAS64 );
	fprintf( file, " 0x%x 0x%x", hi, lo );
	lto_boilerplate_end( file );
	fprintf( file, LTO_TAB ".int 0x%x, 0x%x\n", hi, lo );
    }
    if( is_volatile ) {
	lto_boilerplate_start( file, LTO_VOLATILE );
	lto_boilerplate_end( file );
    }
    lto_footer( file, "\t" );
}
