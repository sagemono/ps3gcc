// Class for doing i/o on pgo data.
/* Copyright (C) 2006 Sony Computer Entertainment, Inc.,

   PGO is free software; you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free
   Software Foundation; either version 2, or (at your option) any later
   version.

   PGO is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
   for more details.

   You should have received a copy of the GNU General Public License
   along with PGO; see the file COPYING.  If not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA.  */



#ifndef PGO_PGO_IO_H
#define PGO_PGO_IO_H

#ident "$Id: pgo-io.h,v 1.7 2006/09/27 21:03:01 dje Exp $"

#include <stdint.h>
#include <string>
#include "libtm/targ-vals.h"
#include "libtm/elf-tools.h"
#include "pgo/pgo.h"

// decls from gcov
struct gcov_info;
struct gcov_fn_info;

using std::string;

// ??? Seems like the better way to go is to have two cooperating classes,
// one that knows how to read the file and one to process the contents
// instead of using subclassing and virtual methods, but I'm not sure how
// to adequately write them.

class pgo_io_c
{
  public:

    // {_target_options} is a ;-delimited list of options.
    // Current options are: 32, 64, big, little.

    pgo_io_c (const string& _elf_file, const string& _target_options);
    pgo_io_c (const uint8_t* _raw_data, uint32_t _size,
	      const string& _target_options);
    virtual ~pgo_io_c ();

    typedef uint64_t addr_t;

    unsigned get_min_record_size (unsigned rec) const;
    const char* get_record_name (unsigned rec) const;

    static const char* get_mode_name (pgo_mode_e mode);
    static pgo_mode_kind_e get_mode_kind (pgo_mode_e mode);
    static unsigned get_mode_bitsize (pgo_mode_e mode);

    unsigned get_offset () const { return curr - raw_data; }

    virtual bool read_file ();

    bool read_rec_edge (addr_t& addr, uint32_t& bin_nr);
    bool read_rec_interval (addr_t& addr, uint32_t& bin_nr,
			    pgo_mode_e& mode, uint32_t& reg_nr,
			    // signed int because that's what gcc uses
			    int32_t& interval_start, int32_t& steps);
    bool read_rec_pow2 (addr_t& addr, uint32_t& bin_nr,
			pgo_mode_e& mode, uint32_t& reg_nr);
    bool read_rec_single (addr_t& addr, uint32_t& bin_nr,
			  pgo_mode_e& mode, uint32_t& reg_nr);
    bool read_rec_delta (addr_t& addr, uint32_t& bin_nr,
			 pgo_mode_e& mode, uint32_t& reg_nr);
    bool read_rec_begin_file (uint8_t& major, uint8_t& minor,
			      uint8_t& insn_ptr_size, uint8_t& data_ptr_size,
			      uint32_t& flags, uint32_t& gcov_offset);
    bool read_rec_begin_collection (uint32_t& gcov_offset);
    bool read_rec_end_collection (uint32_t& gcov_offset);
    bool read_rec_gcov_info (gcov_info*& g_info);
    bool read_rec_gcda_file (char*& file);
    bool read_rec_fn_info (unsigned& nr_fns, unsigned& nr_ctrs,
			   gcov_fn_info*& fn_info);

    bool error_p () { return have_error; }
    const string& get_error () { return error_message; }

  protected:

    const string elf_file;
    const string target_options;

    const uint8_t* raw_data;
    const uint8_t* raw_end;
    unsigned size;

    elf_obj_t* elf_obj;
    bool is_big;
    unsigned elf_size; // 32 or 64
    unsigned iptr_size; // insn ptr size, 32 or 64
    unsigned dptr_size; // data ptr size, 32 or 64

    // convert from target struct to host version
    target_struct_c gcov_info_conv;
    target_struct_c fn_info_conv;
    target_struct_c ctr_info_conv;

    bool have_error;
    string error_message;
    void set_error (const string& msg);

    // Used while reading the data.
    const uint8_t* curr;
    char* current_gcda_file;
    unsigned current_gcda_file_offset; // for sanity check
    gcov_fn_info* current_fn_info ;
    unsigned current_fn_info_offset; // for sanity check

    uint32_t get_uint32 (const uint8_t* p);
    uint64_t get_uint64 (const uint8_t* p);

    bool process_options ();
    bool init ();
    void reinit_specs ();

    static unsigned count_bits (unsigned mask);

    // handlers for each kind of record in the file
    virtual void do_rec_edge (unsigned offset, addr_t addr,
			      uint32_t bin_nr) = 0;
    virtual void do_rec_interval (unsigned offset, addr_t addr,
				  uint32_t bin_nr,
				  pgo_mode_e mode, uint32_t reg_nr,
				  // signed int because that's what gcc uses
				  int32_t interval_start, int32_t steps) = 0;
    virtual void do_rec_pow2 (unsigned offset, addr_t addr,
			      uint32_t bin_nr,
			      pgo_mode_e mode, uint32_t reg_nr) = 0;
    virtual void do_rec_single (unsigned offset, addr_t addr,
				uint32_t bin_nr,
				pgo_mode_e mode, uint32_t reg_nr) = 0;
    virtual void do_rec_delta (unsigned offset, addr_t addr,
			       uint32_t bin_nr,
			       pgo_mode_e mode, uint32_t reg_nr) = 0;
    virtual void do_rec_begin_file (unsigned offset,
				    uint8_t major, uint8_t minor,
				    uint8_t insn_ptr_size, uint8_t data_ptr_size,
				    uint32_t flags, uint32_t gcov_offset) = 0;
    virtual void do_rec_begin_collection (unsigned offset,
					  uint32_t gcov_offset) = 0;
    virtual void do_rec_end_collection (unsigned offset,
					uint32_t gcov_offset) = 0;
    virtual void do_rec_gcov_info (unsigned offset,
				   /*const?*/ gcov_info* g_info) = 0;
    virtual void do_rec_gcda_file (unsigned offset,
				   /*const?*/ char* file) = 0;
    virtual void do_rec_fn_info (unsigned offset,
				 unsigned nr_fns, unsigned nr_ctrs,
				 /*const?*/ gcov_fn_info* fn_info) = 0;
};

#endif // PGO_PGO_IO_H
