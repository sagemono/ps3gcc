
/* (C) Copyright
   Sony Computer Entertainment, Inc.,
   Toshiba Corporation,
   International Business Machines Corporation,
   2005.

   This file is free software; you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free
   Software Foundation; either version 2 of the License, or (at your option) 
   any later version.

   This file is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
   for more details.

   You should have received a copy of the GNU General Public License
   along with this file; see the file COPYING.  If not, write to the Free
   Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
   02110-1301, USA.  */

/* This tests a compilation problem that crashed spu-gcc */

typedef union {
    int		i[4];
    double	d[2];
    vector signed int vi;
} v128_t;

typedef struct
{
  float		v_1;
  v128_t	v_2;
  v128_t	v_3;
} struct_0_t;

typedef struct
{
  char		v_4;
  struct_0_t	v_5;
} struct_1_t;

typedef struct
{
  char		v_6;
  struct_1_t	v_7;
  double	v_8;
} struct_2_t;

struct_2_t 
func_0 ()
{
  struct_2_t	v_9 = { 3, 
                        {0, {1.93429e+38F, 
                         {{1724191798, 571422052, 2011841350, 1750563769}}, 
                         {{776471604, 912102327, 1216727942, 2142289105}}}}, 
                        1.32 };
  struct_2_t     *v_10 = &v_9; 

  return (*v_10);
}

int
main(int argc, char **argv)
{
    struct_2_t  v_11;   

    v_11 = func_0();
    return (v_11.v_6 != 3);
}
