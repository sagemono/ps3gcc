/* A Bison parser, made by GNU Bison 1.875d.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     INT = 258,
     NAME = 259,
     LNAME = 260,
     OREQ = 261,
     ANDEQ = 262,
     RSHIFTEQ = 263,
     LSHIFTEQ = 264,
     DIVEQ = 265,
     MULTEQ = 266,
     MINUSEQ = 267,
     PLUSEQ = 268,
     OROR = 269,
     ANDAND = 270,
     NE = 271,
     EQ = 272,
     GE = 273,
     LE = 274,
     RSHIFT = 275,
     LSHIFT = 276,
     UNARY = 277,
     END = 278,
     ALIGN_K = 279,
     BLOCK = 280,
     BIND = 281,
     QUAD = 282,
     SQUAD = 283,
     LONG = 284,
     SHORT = 285,
     BYTE = 286,
     SECTIONS = 287,
     PHDRS = 288,
     DATA_SEGMENT_ALIGN = 289,
     DATA_SEGMENT_RELRO_END = 290,
     DATA_SEGMENT_END = 291,
     SORT_BY_NAME = 292,
     SORT_BY_ALIGNMENT = 293,
     SIZEOF_HEADERS = 294,
     OUTPUT_FORMAT = 295,
     FORCE_COMMON_ALLOCATION = 296,
     OUTPUT_ARCH = 297,
     INHIBIT_COMMON_ALLOCATION = 298,
     SEGMENT_START = 299,
     INCLUDE = 300,
     MEMORY = 301,
     DEFSYMEND = 302,
     NOLOAD = 303,
     DSECT = 304,
     COPY = 305,
     INFO = 306,
     OVERLAY = 307,
     DEFINED = 308,
     TARGET_K = 309,
     SEARCH_DIR = 310,
     MAP = 311,
     ENTRY = 312,
     NEXT = 313,
     SIZEOF = 314,
     ADDR = 315,
     LOADADDR = 316,
     MAX_K = 317,
     MIN_K = 318,
     STARTUP = 319,
     HLL = 320,
     SYSLIB = 321,
     FLOAT = 322,
     NOFLOAT = 323,
     NOCROSSREFS = 324,
     ORIGIN = 325,
     FILL = 326,
     LENGTH = 327,
     CREATE_OBJECT_SYMBOLS = 328,
     INPUT = 329,
     GROUP = 330,
     OUTPUT = 331,
     CONSTRUCTORS = 332,
     ALIGNMOD = 333,
     AT = 334,
     SUBALIGN = 335,
     PROVIDE = 336,
     PROVIDE_HIDDEN = 337,
     AS_NEEDED = 338,
     CHIP = 339,
     LIST = 340,
     SECT = 341,
     ABSOLUTE = 342,
     LOAD = 343,
     NEWLINE = 344,
     ENDWORD = 345,
     ORDER = 346,
     NAMEWORD = 347,
     ASSERT_K = 348,
     FORMAT = 349,
     PUBLIC = 350,
     BASE = 351,
     ALIAS = 352,
     TRUNCATE = 353,
     REL = 354,
     INPUT_SCRIPT = 355,
     INPUT_MRI_SCRIPT = 356,
     INPUT_DEFSYM = 357,
     CASE = 358,
     EXTERN = 359,
     START = 360,
     VERS_TAG = 361,
     VERS_IDENTIFIER = 362,
     GLOBAL = 363,
     LOCAL = 364,
     VERSIONK = 365,
     INPUT_VERSION_SCRIPT = 366,
     KEEP = 367,
     ONLY_IF_RO = 368,
     ONLY_IF_RW = 369,
     SPECIAL = 370,
     ONLY_IF_SPUGUID = 371,
     EXCLUDE_FILE = 372
   };
#endif
#define INT 258
#define NAME 259
#define LNAME 260
#define OREQ 261
#define ANDEQ 262
#define RSHIFTEQ 263
#define LSHIFTEQ 264
#define DIVEQ 265
#define MULTEQ 266
#define MINUSEQ 267
#define PLUSEQ 268
#define OROR 269
#define ANDAND 270
#define NE 271
#define EQ 272
#define GE 273
#define LE 274
#define RSHIFT 275
#define LSHIFT 276
#define UNARY 277
#define END 278
#define ALIGN_K 279
#define BLOCK 280
#define BIND 281
#define QUAD 282
#define SQUAD 283
#define LONG 284
#define SHORT 285
#define BYTE 286
#define SECTIONS 287
#define PHDRS 288
#define DATA_SEGMENT_ALIGN 289
#define DATA_SEGMENT_RELRO_END 290
#define DATA_SEGMENT_END 291
#define SORT_BY_NAME 292
#define SORT_BY_ALIGNMENT 293
#define SIZEOF_HEADERS 294
#define OUTPUT_FORMAT 295
#define FORCE_COMMON_ALLOCATION 296
#define OUTPUT_ARCH 297
#define INHIBIT_COMMON_ALLOCATION 298
#define SEGMENT_START 299
#define INCLUDE 300
#define MEMORY 301
#define DEFSYMEND 302
#define NOLOAD 303
#define DSECT 304
#define COPY 305
#define INFO 306
#define OVERLAY 307
#define DEFINED 308
#define TARGET_K 309
#define SEARCH_DIR 310
#define MAP 311
#define ENTRY 312
#define NEXT 313
#define SIZEOF 314
#define ADDR 315
#define LOADADDR 316
#define MAX_K 317
#define MIN_K 318
#define STARTUP 319
#define HLL 320
#define SYSLIB 321
#define FLOAT 322
#define NOFLOAT 323
#define NOCROSSREFS 324
#define ORIGIN 325
#define FILL 326
#define LENGTH 327
#define CREATE_OBJECT_SYMBOLS 328
#define INPUT 329
#define GROUP 330
#define OUTPUT 331
#define CONSTRUCTORS 332
#define ALIGNMOD 333
#define AT 334
#define SUBALIGN 335
#define PROVIDE 336
#define PROVIDE_HIDDEN 337
#define AS_NEEDED 338
#define CHIP 339
#define LIST 340
#define SECT 341
#define ABSOLUTE 342
#define LOAD 343
#define NEWLINE 344
#define ENDWORD 345
#define ORDER 346
#define NAMEWORD 347
#define ASSERT_K 348
#define FORMAT 349
#define PUBLIC 350
#define BASE 351
#define ALIAS 352
#define TRUNCATE 353
#define REL 354
#define INPUT_SCRIPT 355
#define INPUT_MRI_SCRIPT 356
#define INPUT_DEFSYM 357
#define CASE 358
#define EXTERN 359
#define START 360
#define VERS_TAG 361
#define VERS_IDENTIFIER 362
#define GLOBAL 363
#define LOCAL 364
#define VERSIONK 365
#define INPUT_VERSION_SCRIPT 366
#define KEEP 367
#define ONLY_IF_RO 368
#define ONLY_IF_RW 369
#define SPECIAL 370
#define ONLY_IF_SPUGUID 371
#define EXCLUDE_FILE 372




/* Copy the first part of user declarations.  */
#line 22 "ldgram.y"

/*

 */

#define DONTDECLARE_MALLOC

#include "bfd.h"
#include "sysdep.h"
#include "bfdlink.h"
#include "ld.h"
#include "ldexp.h"
#include "ldver.h"
#include "ldlang.h"
#include "ldfile.h"
#include "ldemul.h"
#include "ldmisc.h"
#include "ldmain.h"
#include "mri.h"
#include "ldctor.h"
#include "ldlex.h"

#ifndef YYDEBUG
#define YYDEBUG 1
#endif

static enum section_type sectype;
static lang_memory_region_type *region;

FILE *saved_script_handle = NULL;
bfd_boolean force_make_executable = FALSE;

bfd_boolean ldgram_in_script = FALSE;
bfd_boolean ldgram_had_equals = FALSE;
bfd_boolean ldgram_had_keep = FALSE;
char *ldgram_vers_current_lang = NULL;

#define ERROR_NAME_MAX 20
static char *error_names[ERROR_NAME_MAX];
static int error_index;
#define PUSH_ERROR(x) if (error_index < ERROR_NAME_MAX) error_names[error_index] = x; error_index++;
#define POP_ERROR()   error_index--;


/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 65 "ldgram.y"
typedef union YYSTYPE {
  bfd_vma integer;
  struct big_int
    {
      bfd_vma integer;
      char *str;
    } bigint;
  fill_type *fill;
  char *name;
  const char *cname;
  struct wildcard_spec wildcard;
  struct wildcard_list *wildcard_list;
  struct name_list *name_list;
  int token;
  union etree_union *etree;
  struct phdr_info
    {
      bfd_boolean filehdr;
      bfd_boolean phdrs;
      union etree_union *at;
      union etree_union *flags;
    } phdr;
  struct lang_nocrossref *nocrossref;
  struct lang_output_section_phdr_list *section_phdr;
  struct bfd_elf_version_deps *deflist;
  struct bfd_elf_version_expr *versyms;
  struct bfd_elf_version_tree *versnode;
} YYSTYPE;
/* Line 191 of yacc.c.  */
#line 383 "ldgram.c"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 214 of yacc.c.  */
#line 395 "ldgram.c"

#if ! defined (yyoverflow) || YYERROR_VERBOSE

# ifndef YYFREE
#  define YYFREE free
# endif
# ifndef YYMALLOC
#  define YYMALLOC malloc
# endif

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   define YYSTACK_ALLOC alloca
#  endif
# else
#  if defined (alloca) || defined (_ALLOCA_H)
#   define YYSTACK_ALLOC alloca
#  else
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (defined (YYSTYPE_IS_TRIVIAL) && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short int yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short int) + sizeof (YYSTYPE))			\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined (__GNUC__) && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T yyi;		\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short int yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  14
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1655

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  141
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  114
/* YYNRULES -- Number of rules. */
#define YYNRULES  334
/* YYNRULES -- Number of states. */
#define YYNSTATES  708

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   372

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   139,     2,     2,     2,    34,    21,     2,
      37,   136,    32,    30,   134,    31,     2,    33,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    16,   135,
      24,     6,    25,    15,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   137,     2,   138,    20,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    53,    19,    54,   140,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     7,     8,     9,    10,    11,    12,    13,    14,    17,
      18,    22,    23,    26,    27,    28,    29,    35,    36,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short int yyprhs[] =
{
       0,     0,     3,     6,     9,    12,    15,    17,    18,    23,
      24,    27,    31,    32,    35,    40,    42,    44,    47,    49,
      54,    59,    63,    66,    71,    75,    80,    85,    90,    95,
     100,   103,   106,   109,   114,   119,   122,   125,   128,   131,
     132,   138,   141,   142,   146,   149,   150,   152,   156,   158,
     162,   163,   165,   169,   171,   174,   178,   179,   182,   185,
     186,   188,   190,   192,   194,   196,   198,   200,   202,   204,
     206,   211,   216,   221,   226,   235,   240,   242,   244,   249,
     250,   256,   261,   262,   268,   273,   278,   280,   284,   287,
     289,   293,   296,   297,   303,   304,   312,   313,   320,   325,
     328,   331,   332,   337,   340,   341,   349,   351,   353,   355,
     357,   363,   368,   373,   381,   389,   397,   405,   414,   417,
     419,   423,   425,   427,   431,   436,   438,   439,   445,   448,
     450,   452,   454,   459,   461,   466,   471,   474,   476,   477,
     479,   481,   483,   485,   487,   489,   491,   494,   495,   497,
     499,   501,   503,   505,   507,   509,   511,   513,   515,   519,
     523,   530,   537,   539,   540,   546,   549,   553,   554,   555,
     563,   567,   571,   572,   576,   578,   581,   583,   586,   591,
     596,   600,   604,   606,   611,   615,   616,   618,   620,   621,
     624,   628,   629,   632,   635,   639,   644,   647,   650,   653,
     657,   661,   665,   669,   673,   677,   681,   685,   689,   693,
     697,   701,   705,   709,   713,   717,   723,   727,   731,   736,
     738,   740,   745,   750,   755,   760,   765,   772,   779,   786,
     791,   798,   803,   805,   812,   819,   826,   831,   836,   840,
     841,   846,   847,   852,   853,   858,   859,   861,   863,   865,
     867,   868,   869,   870,   871,   872,   873,   893,   894,   895,
     896,   897,   898,   917,   918,   919,   927,   929,   931,   933,
     935,   937,   941,   942,   945,   949,   952,   959,   970,   973,
     975,   976,   978,   981,   982,   983,   987,   988,   989,   990,
     991,  1003,  1008,  1009,  1012,  1013,  1014,  1021,  1023,  1024,
    1028,  1034,  1035,  1039,  1040,  1043,  1044,  1050,  1052,  1055,
    1060,  1066,  1073,  1075,  1078,  1079,  1082,  1087,  1092,  1101,
    1103,  1105,  1109,  1113,  1114,  1124,  1125,  1133,  1135,  1139,
    1141,  1145,  1147,  1151,  1152
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const short int yyrhs[] =
{
     142,     0,    -1,   116,   156,    -1,   117,   146,    -1,   127,
     243,    -1,   118,   144,    -1,     4,    -1,    -1,   145,     4,
       6,   205,    -1,    -1,   147,   148,    -1,   148,   149,   105,
      -1,    -1,   100,   205,    -1,   100,   205,   134,   205,    -1,
       4,    -1,   101,    -1,   107,   151,    -1,   106,    -1,   111,
       4,     6,   205,    -1,   111,     4,   134,   205,    -1,   111,
       4,   205,    -1,   110,     4,    -1,   102,     4,   134,   205,
      -1,   102,     4,   205,    -1,   102,     4,     6,   205,    -1,
      38,     4,     6,   205,    -1,    38,     4,   134,   205,    -1,
      94,     4,     6,   205,    -1,    94,     4,   134,   205,    -1,
     103,   153,    -1,   104,   152,    -1,   108,     4,    -1,   113,
       4,   134,     4,    -1,   113,     4,   134,     3,    -1,   112,
     205,    -1,   114,     3,    -1,   119,   154,    -1,   120,   155,
      -1,    -1,    61,   143,   150,   148,    36,    -1,   121,     4,
      -1,    -1,   151,   134,     4,    -1,   151,     4,    -1,    -1,
       4,    -1,   152,   134,     4,    -1,     4,    -1,   153,   134,
       4,    -1,    -1,     4,    -1,   154,   134,     4,    -1,     4,
      -1,   155,     4,    -1,   155,   134,     4,    -1,    -1,   157,
     158,    -1,   158,   159,    -1,    -1,   187,    -1,   166,    -1,
     235,    -1,   196,    -1,   197,    -1,   199,    -1,   201,    -1,
     168,    -1,   245,    -1,   135,    -1,    70,    37,     4,   136,
      -1,    71,    37,   143,   136,    -1,    92,    37,   143,   136,
      -1,    56,    37,     4,   136,    -1,    56,    37,     4,   134,
       4,   134,     4,   136,    -1,    58,    37,     4,   136,    -1,
      57,    -1,    59,    -1,    90,    37,   162,   136,    -1,    -1,
      91,   160,    37,   162,   136,    -1,    72,    37,   143,   136,
      -1,    -1,    61,   143,   161,   158,    36,    -1,    85,    37,
     202,   136,    -1,   120,    37,   155,   136,    -1,     4,    -1,
     162,   134,     4,    -1,   162,     4,    -1,     5,    -1,   162,
     134,     5,    -1,   162,     5,    -1,    -1,    99,    37,   163,
     162,   136,    -1,    -1,   162,   134,    99,    37,   164,   162,
     136,    -1,    -1,   162,    99,    37,   165,   162,   136,    -1,
      46,    53,   167,    54,    -1,   167,   211,    -1,   167,   168,
      -1,    -1,    73,    37,     4,   136,    -1,   185,   184,    -1,
      -1,   109,   169,    37,   205,   134,     4,   136,    -1,     4,
      -1,    32,    -1,    15,    -1,   170,    -1,   133,    37,   172,
     136,   170,    -1,    51,    37,   170,   136,    -1,    52,    37,
     170,   136,    -1,    51,    37,    52,    37,   170,   136,   136,
      -1,    51,    37,    51,    37,   170,   136,   136,    -1,    52,
      37,    51,    37,   170,   136,   136,    -1,    52,    37,    52,
      37,   170,   136,   136,    -1,    51,    37,   133,    37,   172,
     136,   170,   136,    -1,   172,   170,    -1,   170,    -1,   173,
     186,   171,    -1,   171,    -1,     4,    -1,   137,   173,   138,
      -1,   171,    37,   173,   136,    -1,   174,    -1,    -1,   128,
      37,   176,   174,   136,    -1,   185,   184,    -1,    89,    -1,
     135,    -1,    93,    -1,    51,    37,    93,   136,    -1,   175,
      -1,   180,    37,   203,   136,    -1,    87,    37,   181,   136,
      -1,   178,   177,    -1,   177,    -1,    -1,   178,    -1,    41,
      -1,    42,    -1,    43,    -1,    44,    -1,    45,    -1,   203,
      -1,     6,   181,    -1,    -1,    14,    -1,    13,    -1,    12,
      -1,    11,    -1,    10,    -1,     9,    -1,     8,    -1,     7,
      -1,   135,    -1,   134,    -1,     4,     6,   203,    -1,     4,
     183,   203,    -1,    97,    37,     4,     6,   203,   136,    -1,
      98,    37,     4,     6,   203,   136,    -1,   134,    -1,    -1,
      62,    53,   189,   188,    54,    -1,   188,   189,    -1,   188,
     134,   189,    -1,    -1,    -1,     4,   190,   193,    16,   191,
     186,   192,    -1,    86,     6,   203,    -1,    88,     6,   203,
      -1,    -1,    37,   194,   136,    -1,   195,    -1,   194,   195,
      -1,     4,    -1,   139,     4,    -1,    80,    37,   143,   136,
      -1,    81,    37,   198,   136,    -1,    81,    37,   136,    -1,
     198,   186,   143,    -1,   143,    -1,    82,    37,   200,   136,
      -1,   200,   186,   143,    -1,    -1,    83,    -1,    84,    -1,
      -1,     4,   202,    -1,     4,   134,   202,    -1,    -1,   204,
     205,    -1,    31,   205,    -1,    37,   205,   136,    -1,    74,
      37,   205,   136,    -1,   139,   205,    -1,    30,   205,    -1,
     140,   205,    -1,   205,    32,   205,    -1,   205,    33,   205,
      -1,   205,    34,   205,    -1,   205,    30,   205,    -1,   205,
      31,   205,    -1,   205,    29,   205,    -1,   205,    28,   205,
      -1,   205,    23,   205,    -1,   205,    22,   205,    -1,   205,
      27,   205,    -1,   205,    26,   205,    -1,   205,    24,   205,
      -1,   205,    25,   205,    -1,   205,    21,   205,    -1,   205,
      20,   205,    -1,   205,    19,   205,    -1,   205,    15,   205,
      16,   205,    -1,   205,    18,   205,    -1,   205,    17,   205,
      -1,    69,    37,     4,   136,    -1,     3,    -1,    55,    -1,
      75,    37,     4,   136,    -1,    76,    37,     4,   136,    -1,
      77,    37,     4,   136,    -1,   103,    37,   205,   136,    -1,
      38,    37,   205,   136,    -1,    38,    37,   205,   134,   205,
     136,    -1,    48,    37,   205,   134,   205,   136,    -1,    49,
      37,   205,   134,   205,   136,    -1,    50,    37,   205,   136,
      -1,    60,    37,     4,   134,   205,   136,    -1,    39,    37,
     205,   136,    -1,     4,    -1,    78,    37,   205,   134,   205,
     136,    -1,    79,    37,   205,   134,   205,   136,    -1,   109,
      37,   205,   134,     4,   136,    -1,    86,    37,     4,   136,
      -1,    88,    37,     4,   136,    -1,    95,    25,     4,    -1,
      -1,    95,    37,   205,   136,    -1,    -1,    38,    37,   205,
     136,    -1,    -1,    96,    37,   205,   136,    -1,    -1,   129,
      -1,   130,    -1,   132,    -1,   131,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     4,   212,   226,   207,   208,   209,   213,
     210,    53,   214,   179,    54,   215,   229,   206,   230,   182,
     216,   186,    -1,    -1,    -1,    -1,    -1,    -1,    68,   217,
     227,   228,   207,   209,   218,    53,   219,   231,    54,   220,
     229,   206,   230,   182,   221,   186,    -1,    -1,    -1,    91,
     222,   226,   223,    53,   167,    54,    -1,    64,    -1,    65,
      -1,    66,    -1,    67,    -1,    68,    -1,    37,   224,   136,
      -1,    -1,    37,   136,    -1,   205,   225,    16,    -1,   225,
      16,    -1,    40,    37,   205,   136,   225,    16,    -1,    40,
      37,   205,   136,    39,    37,   205,   136,   225,    16,    -1,
     205,    16,    -1,    16,    -1,    -1,    85,    -1,    25,     4,
      -1,    -1,    -1,   230,    16,     4,    -1,    -1,    -1,    -1,
      -1,   231,     4,   232,    53,   179,    54,   233,   230,   182,
     234,   186,    -1,    47,    53,   236,    54,    -1,    -1,   236,
     237,    -1,    -1,    -1,     4,   238,   240,   241,   239,   135,
      -1,   205,    -1,    -1,     4,   242,   241,    -1,    95,    37,
     205,   136,   241,    -1,    -1,    37,   205,   136,    -1,    -1,
     244,   247,    -1,    -1,   246,   126,    53,   247,    54,    -1,
     248,    -1,   247,   248,    -1,    53,   250,    54,   135,    -1,
     122,    53,   250,    54,   135,    -1,   122,    53,   250,    54,
     249,   135,    -1,   122,    -1,   249,   122,    -1,    -1,   251,
     135,    -1,   124,    16,   251,   135,    -1,   125,    16,   251,
     135,    -1,   124,    16,   251,   135,   125,    16,   251,   135,
      -1,   123,    -1,     4,    -1,   251,   135,   123,    -1,   251,
     135,     4,    -1,    -1,   251,   135,   120,     4,    53,   252,
     251,   254,    54,    -1,    -1,   120,     4,    53,   253,   251,
     254,    54,    -1,   124,    -1,   251,   135,   124,    -1,   125,
      -1,   251,   135,   125,    -1,   120,    -1,   251,   135,   120,
      -1,    -1,   135,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short int yyrline[] =
{
       0,   162,   162,   163,   164,   165,   169,   173,   173,   183,
     183,   196,   197,   201,   202,   203,   206,   209,   210,   211,
     213,   215,   217,   219,   221,   223,   225,   227,   229,   231,
     233,   234,   235,   237,   239,   241,   243,   245,   246,   248,
     247,   251,   253,   257,   258,   259,   263,   265,   269,   271,
     276,   277,   278,   282,   284,   286,   291,   291,   302,   303,
     309,   310,   311,   312,   313,   314,   315,   316,   317,   318,
     319,   321,   323,   325,   328,   330,   332,   334,   336,   338,
     337,   341,   344,   343,   347,   351,   355,   358,   361,   364,
     367,   370,   374,   373,   378,   377,   382,   381,   388,   392,
     393,   394,   398,   400,   401,   401,   409,   413,   417,   424,
     430,   436,   442,   448,   454,   460,   466,   472,   481,   490,
     501,   510,   521,   529,   533,   540,   542,   541,   548,   549,
     553,   554,   559,   564,   565,   570,   577,   578,   581,   583,
     587,   589,   591,   593,   595,   600,   607,   609,   613,   615,
     617,   619,   621,   623,   625,   627,   632,   632,   637,   641,
     649,   653,   661,   661,   665,   669,   670,   671,   676,   675,
     683,   691,   699,   700,   704,   705,   709,   711,   716,   721,
     722,   727,   729,   735,   737,   739,   743,   745,   751,   754,
     763,   774,   774,   780,   782,   784,   786,   788,   790,   793,
     795,   797,   799,   801,   803,   805,   807,   809,   811,   813,
     815,   817,   819,   821,   823,   825,   827,   829,   831,   833,
     835,   838,   840,   842,   844,   846,   848,   850,   852,   854,
     856,   865,   867,   869,   871,   873,   875,   877,   883,   884,
     888,   889,   893,   894,   898,   899,   903,   904,   905,   906,
     907,   910,   914,   917,   923,   925,   910,   932,   934,   936,
     941,   943,   931,   953,   955,   953,   963,   964,   965,   966,
     967,   971,   972,   973,   977,   978,   983,   984,   989,   990,
     995,   996,  1001,  1003,  1008,  1011,  1024,  1028,  1033,  1035,
    1026,  1043,  1046,  1048,  1052,  1053,  1052,  1062,  1112,  1115,
    1127,  1136,  1139,  1148,  1148,  1162,  1162,  1172,  1173,  1177,
    1181,  1185,  1192,  1196,  1204,  1207,  1211,  1215,  1219,  1226,
    1230,  1234,  1238,  1243,  1242,  1256,  1255,  1265,  1269,  1273,
    1277,  1281,  1285,  1291,  1293
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INT", "NAME", "LNAME", "'='", "OREQ",
  "ANDEQ", "RSHIFTEQ", "LSHIFTEQ", "DIVEQ", "MULTEQ", "MINUSEQ", "PLUSEQ",
  "'?'", "':'", "OROR", "ANDAND", "'|'", "'^'", "'&'", "NE", "EQ", "'<'",
  "'>'", "GE", "LE", "RSHIFT", "LSHIFT", "'+'", "'-'", "'*'", "'/'", "'%'",
  "UNARY", "END", "'('", "ALIGN_K", "BLOCK", "BIND", "QUAD", "SQUAD",
  "LONG", "SHORT", "BYTE", "SECTIONS", "PHDRS", "DATA_SEGMENT_ALIGN",
  "DATA_SEGMENT_RELRO_END", "DATA_SEGMENT_END", "SORT_BY_NAME",
  "SORT_BY_ALIGNMENT", "'{'", "'}'", "SIZEOF_HEADERS", "OUTPUT_FORMAT",
  "FORCE_COMMON_ALLOCATION", "OUTPUT_ARCH", "INHIBIT_COMMON_ALLOCATION",
  "SEGMENT_START", "INCLUDE", "MEMORY", "DEFSYMEND", "NOLOAD", "DSECT",
  "COPY", "INFO", "OVERLAY", "DEFINED", "TARGET_K", "SEARCH_DIR", "MAP",
  "ENTRY", "NEXT", "SIZEOF", "ADDR", "LOADADDR", "MAX_K", "MIN_K",
  "STARTUP", "HLL", "SYSLIB", "FLOAT", "NOFLOAT", "NOCROSSREFS", "ORIGIN",
  "FILL", "LENGTH", "CREATE_OBJECT_SYMBOLS", "INPUT", "GROUP", "OUTPUT",
  "CONSTRUCTORS", "ALIGNMOD", "AT", "SUBALIGN", "PROVIDE",
  "PROVIDE_HIDDEN", "AS_NEEDED", "CHIP", "LIST", "SECT", "ABSOLUTE",
  "LOAD", "NEWLINE", "ENDWORD", "ORDER", "NAMEWORD", "ASSERT_K", "FORMAT",
  "PUBLIC", "BASE", "ALIAS", "TRUNCATE", "REL", "INPUT_SCRIPT",
  "INPUT_MRI_SCRIPT", "INPUT_DEFSYM", "CASE", "EXTERN", "START",
  "VERS_TAG", "VERS_IDENTIFIER", "GLOBAL", "LOCAL", "VERSIONK",
  "INPUT_VERSION_SCRIPT", "KEEP", "ONLY_IF_RO", "ONLY_IF_RW", "SPECIAL",
  "ONLY_IF_SPUGUID", "EXCLUDE_FILE", "','", "';'", "')'", "'['", "']'",
  "'!'", "'~'", "$accept", "file", "filename", "defsym_expr", "@1",
  "mri_script_file", "@2", "mri_script_lines", "mri_script_command", "@3",
  "ordernamelist", "mri_load_name_list", "mri_abs_name_list",
  "casesymlist", "extern_name_list", "script_file", "@4", "ifile_list",
  "ifile_p1", "@5", "@6", "input_list", "@7", "@8", "@9", "sections",
  "sec_or_group_p1", "statement_anywhere", "@10", "wildcard_name",
  "wildcard_spec", "exclude_name_list", "file_NAME_list",
  "input_section_spec_no_keep", "input_section_spec", "@11", "statement",
  "statement_list", "statement_list_opt", "length", "fill_exp", "fill_opt",
  "assign_op", "end", "assignment", "opt_comma", "memory",
  "memory_spec_list", "memory_spec", "@12", "origin_spec", "length_spec",
  "attributes_opt", "attributes_list", "attributes_string", "startup",
  "high_level_library", "high_level_library_NAME_list",
  "low_level_library", "low_level_library_NAME_list",
  "floating_point_support", "nocrossref_list", "mustbe_exp", "@13", "exp",
  "memspec_at_opt", "opt_at", "opt_align", "opt_subalign",
  "sect_constraint", "section", "@14", "@15", "@16", "@17", "@18", "@19",
  "@20", "@21", "@22", "@23", "@24", "@25", "type", "atype",
  "opt_exp_with_type", "opt_exp_without_type", "opt_nocrossrefs",
  "memspec_opt", "phdr_opt", "overlay_section", "@26", "@27", "@28",
  "phdrs", "phdr_list", "phdr", "@29", "@30", "phdr_type",
  "phdr_qualifiers", "phdr_val", "version_script_file", "@31", "version",
  "@32", "vers_nodes", "vers_node", "verdep", "vers_tag", "vers_defns",
  "@33", "@34", "opt_semicolon", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short int yytoknum[] =
{
       0,   256,   257,   258,   259,   260,    61,   261,   262,   263,
     264,   265,   266,   267,   268,    63,    58,   269,   270,   124,
      94,    38,   271,   272,    60,    62,   273,   274,   275,   276,
      43,    45,    42,    47,    37,   277,   278,    40,   279,   280,
     281,   282,   283,   284,   285,   286,   287,   288,   289,   290,
     291,   292,   293,   123,   125,   294,   295,   296,   297,   298,
     299,   300,   301,   302,   303,   304,   305,   306,   307,   308,
     309,   310,   311,   312,   313,   314,   315,   316,   317,   318,
     319,   320,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   330,   331,   332,   333,   334,   335,   336,   337,   338,
     339,   340,   341,   342,   343,   344,   345,   346,   347,   348,
     349,   350,   351,   352,   353,   354,   355,   356,   357,   358,
     359,   360,   361,   362,   363,   364,   365,   366,   367,   368,
     369,   370,   371,   372,    44,    59,    41,    91,    93,    33,
     126
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,   141,   142,   142,   142,   142,   143,   145,   144,   147,
     146,   148,   148,   149,   149,   149,   149,   149,   149,   149,
     149,   149,   149,   149,   149,   149,   149,   149,   149,   149,
     149,   149,   149,   149,   149,   149,   149,   149,   149,   150,
     149,   149,   149,   151,   151,   151,   152,   152,   153,   153,
     154,   154,   154,   155,   155,   155,   157,   156,   158,   158,
     159,   159,   159,   159,   159,   159,   159,   159,   159,   159,
     159,   159,   159,   159,   159,   159,   159,   159,   159,   160,
     159,   159,   161,   159,   159,   159,   162,   162,   162,   162,
     162,   162,   163,   162,   164,   162,   165,   162,   166,   167,
     167,   167,   168,   168,   169,   168,   170,   170,   170,   171,
     171,   171,   171,   171,   171,   171,   171,   171,   172,   172,
     173,   173,   174,   174,   174,   175,   176,   175,   177,   177,
     177,   177,   177,   177,   177,   177,   178,   178,   179,   179,
     180,   180,   180,   180,   180,   181,   182,   182,   183,   183,
     183,   183,   183,   183,   183,   183,   184,   184,   185,   185,
     185,   185,   186,   186,   187,   188,   188,   188,   190,   189,
     191,   192,   193,   193,   194,   194,   195,   195,   196,   197,
     197,   198,   198,   199,   200,   200,   201,   201,   202,   202,
     202,   204,   203,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   206,   206,
     207,   207,   208,   208,   209,   209,   210,   210,   210,   210,
     210,   212,   213,   214,   215,   216,   211,   217,   218,   219,
     220,   221,   211,   222,   223,   211,   224,   224,   224,   224,
     224,   225,   225,   225,   226,   226,   226,   226,   227,   227,
     228,   228,   229,   229,   230,   230,   231,   232,   233,   234,
     231,   235,   236,   236,   238,   239,   237,   240,   241,   241,
     241,   242,   242,   244,   243,   246,   245,   247,   247,   248,
     248,   248,   249,   249,   250,   250,   250,   250,   250,   251,
     251,   251,   251,   252,   251,   253,   251,   251,   251,   251,
     251,   251,   251,   254,   254
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     2,     2,     2,     2,     1,     0,     4,     0,
       2,     3,     0,     2,     4,     1,     1,     2,     1,     4,
       4,     3,     2,     4,     3,     4,     4,     4,     4,     4,
       2,     2,     2,     4,     4,     2,     2,     2,     2,     0,
       5,     2,     0,     3,     2,     0,     1,     3,     1,     3,
       0,     1,     3,     1,     2,     3,     0,     2,     2,     0,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       4,     4,     4,     4,     8,     4,     1,     1,     4,     0,
       5,     4,     0,     5,     4,     4,     1,     3,     2,     1,
       3,     2,     0,     5,     0,     7,     0,     6,     4,     2,
       2,     0,     4,     2,     0,     7,     1,     1,     1,     1,
       5,     4,     4,     7,     7,     7,     7,     8,     2,     1,
       3,     1,     1,     3,     4,     1,     0,     5,     2,     1,
       1,     1,     4,     1,     4,     4,     2,     1,     0,     1,
       1,     1,     1,     1,     1,     1,     2,     0,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     3,
       6,     6,     1,     0,     5,     2,     3,     0,     0,     7,
       3,     3,     0,     3,     1,     2,     1,     2,     4,     4,
       3,     3,     1,     4,     3,     0,     1,     1,     0,     2,
       3,     0,     2,     2,     3,     4,     2,     2,     2,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     5,     3,     3,     4,     1,
       1,     4,     4,     4,     4,     4,     6,     6,     6,     4,
       6,     4,     1,     6,     6,     6,     4,     4,     3,     0,
       4,     0,     4,     0,     4,     0,     1,     1,     1,     1,
       0,     0,     0,     0,     0,     0,    19,     0,     0,     0,
       0,     0,    18,     0,     0,     7,     1,     1,     1,     1,
       1,     3,     0,     2,     3,     2,     6,    10,     2,     1,
       0,     1,     2,     0,     0,     3,     0,     0,     0,     0,
      11,     4,     0,     2,     0,     0,     6,     1,     0,     3,
       5,     0,     3,     0,     2,     0,     5,     1,     2,     4,
       5,     6,     1,     2,     0,     2,     4,     4,     8,     1,
       1,     3,     3,     0,     9,     0,     7,     1,     3,     1,
       3,     1,     3,     0,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned short int yydefact[] =
{
       0,    56,     9,     7,   303,     0,     2,    59,     3,    12,
       5,     0,     4,     0,     1,    57,    10,     0,   314,     0,
     304,   307,     0,     0,     0,     0,    76,     0,    77,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   186,   187,
       0,     0,    79,     0,     0,     0,   104,     0,    69,    58,
      61,    67,     0,    60,    63,    64,    65,    66,    62,    68,
       0,    15,     0,     0,     0,     0,    16,     0,     0,     0,
      18,    45,     0,     0,     0,     0,     0,     0,    50,     0,
       0,     0,     0,   320,   331,   319,   327,   329,     0,     0,
     314,   308,   191,   155,   154,   153,   152,   151,   150,   149,
     148,   191,   101,   292,     0,     0,     6,    82,     0,     0,
       0,     0,     0,     0,     0,   185,   188,     0,     0,     0,
       0,     0,     0,     0,   157,   156,   103,     0,     0,    39,
       0,   219,   232,     0,     0,     0,     0,     0,     0,     0,
       0,   220,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    13,     0,    48,    30,
      46,    31,    17,    32,    22,     0,    35,     0,    36,    51,
      37,    53,    38,    41,    11,     8,     0,     0,     0,     0,
     315,     0,   158,     0,   159,     0,     0,     0,     0,    59,
     168,   167,     0,     0,     0,     0,     0,   180,   182,   163,
     163,   188,     0,    86,    89,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    12,     0,     0,   197,
     193,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   196,
     198,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    24,     0,     0,    44,     0,     0,     0,
      21,     0,     0,    54,     0,   325,   327,   329,     0,     0,
     309,   322,   332,   321,   328,   330,     0,   192,   251,    98,
     257,   263,   100,    99,   294,   291,   293,     0,    73,    75,
     305,   172,     0,    70,    71,    81,   102,   178,   162,   179,
       0,   183,     0,   188,   189,    84,    92,    88,    91,     0,
       0,    78,     0,    72,   191,   191,     0,    85,     0,    26,
      27,    42,    28,    29,   194,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   217,   216,   214,   213,   212,   207,   206,
     210,   211,   209,   208,   205,   204,   202,   203,   199,   200,
     201,    14,    25,    23,    49,    47,    43,    19,    20,    34,
      33,    52,    55,     0,   316,   317,     0,   312,   310,     0,
     272,     0,   272,     0,     0,    83,     0,     0,   164,     0,
     165,   181,   184,   190,     0,    96,    87,    90,     0,    80,
       0,     0,     0,   306,    40,     0,   225,   231,     0,     0,
     229,     0,   218,   195,   221,   222,   223,     0,     0,   236,
     237,   224,     0,     0,   333,   330,   323,   313,   311,     0,
       0,   272,     0,   241,   279,     0,   280,   264,   297,   298,
       0,   176,     0,     0,   174,     0,   166,     0,     0,    94,
     160,   161,     0,     0,     0,     0,     0,     0,     0,     0,
     215,   334,     0,     0,     0,   266,   267,   268,   269,   270,
     273,     0,     0,     0,     0,   275,     0,   243,   278,   281,
     241,     0,   301,     0,   295,     0,   177,   173,   175,     0,
     163,    93,     0,     0,   105,   226,   227,   228,   230,   233,
     234,   235,   326,     0,   333,   271,     0,   274,     0,     0,
     245,   245,   101,     0,   298,     0,     0,    74,   191,     0,
      97,     0,   318,     0,   272,     0,     0,     0,   252,   258,
       0,     0,   299,     0,   296,   170,     0,   169,    95,   324,
       0,     0,   240,     0,     0,   250,     0,   265,   302,   298,
     191,     0,   276,   242,     0,   246,   247,   249,   248,     0,
     259,   300,   171,     0,   244,   253,   286,   272,   138,     0,
       0,   122,   108,   107,   140,   141,   142,   143,   144,     0,
       0,     0,   129,   131,     0,     0,   130,     0,   109,     0,
     125,   133,   137,   139,     0,     0,     0,   287,   260,   277,
       0,     0,   191,   126,     0,   106,     0,   121,   163,     0,
     136,   254,   191,   128,     0,   283,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   145,     0,   119,     0,     0,
     123,     0,   163,   283,     0,   138,     0,   239,     0,     0,
     132,     0,   111,     0,     0,   112,   135,   106,     0,     0,
     118,   120,   124,   239,   134,     0,   282,     0,   284,     0,
       0,     0,     0,     0,   127,   110,   284,   288,     0,   147,
       0,     0,     0,     0,     0,   147,   284,   238,   191,     0,
     261,   114,   113,     0,   115,   116,   255,   147,   146,   285,
     163,   117,   163,   289,   262,   256,   163,   290
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short int yydefgoto[] =
{
      -1,     5,   107,    10,    11,     8,     9,    16,    81,   216,
     162,   161,   159,   170,   172,     6,     7,    15,    49,   118,
     189,   206,   404,   503,   458,    50,   185,    51,   122,   598,
     599,   638,   618,   600,   601,   636,   602,   603,   604,   605,
     634,   690,   101,   126,    52,   641,    53,   302,   191,   301,
     500,   547,   397,   453,   454,    54,    55,   199,    56,   200,
      57,   202,   635,   183,   221,   668,   487,   520,   538,   569,
     293,   390,   555,   578,   643,   702,   391,   556,   576,   625,
     700,   392,   491,   481,   442,   443,   446,   490,   647,   679,
     579,   624,   686,   706,    58,   186,   296,   393,   526,   449,
     494,   524,    12,    13,    59,    60,    20,    21,   389,    88,
      89,   474,   383,   472
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -643
static const short int yypact[] =
{
     163,  -643,  -643,  -643,  -643,    49,  -643,  -643,  -643,  -643,
    -643,    54,  -643,   -21,  -643,   677,  1429,    70,   111,    28,
     -21,  -643,   830,    43,    74,    95,  -643,   121,  -643,   169,
     156,   173,   206,   213,   223,   233,   234,   251,  -643,  -643,
     255,   256,  -643,   260,   263,   268,  -643,   273,  -643,  -643,
    -643,  -643,    67,  -643,  -643,  -643,  -643,  -643,  -643,  -643,
     198,  -643,   321,   169,   323,   579,  -643,   328,   329,   334,
    -643,  -643,   345,   346,   347,   579,   348,   351,   359,   360,
     363,   250,   579,  -643,   370,  -643,   362,   366,   322,   240,
     111,  -643,  -643,  -643,  -643,  -643,  -643,  -643,  -643,  -643,
    -643,  -643,  -643,  -643,   379,   383,  -643,  -643,   388,   389,
     169,   169,   392,   169,    21,  -643,   399,    78,   367,   169,
     407,   408,   377,   360,  -643,  -643,  -643,   365,     9,  -643,
      40,  -643,  -643,   579,   579,   579,   378,   384,   385,   391,
     393,  -643,   394,   395,   396,   398,   401,   402,   404,   406,
     409,   410,   413,   415,   579,   579,  1256,   331,  -643,   282,
    -643,   292,    10,  -643,  -643,   445,  1621,   295,  -643,  -643,
     310,  -643,    25,  -643,  -643,  1621,   372,   216,   216,   318,
     265,   400,  -643,   579,  -643,   304,    20,    94,   309,  -643,
    -643,  -643,   319,   326,   333,   336,   337,  -643,  -643,   106,
     131,    31,   341,  -643,  -643,   420,    32,    78,   343,   452,
     453,   579,    69,   -21,   579,   579,  -643,   579,   579,  -643,
    -643,   876,   579,   579,   579,   579,   579,   460,   476,   579,
     477,   492,   499,   579,   579,   502,   503,   579,   579,  -643,
    -643,   579,   579,   579,   579,   579,   579,   579,   579,   579,
     579,   579,   579,   579,   579,   579,   579,   579,   579,   579,
     579,   579,   579,  1621,   505,   506,  -643,   507,   579,   579,
    1621,   137,   508,  -643,   509,  -643,  -643,  -643,   382,   397,
    -643,  -643,   514,  -643,  -643,  -643,   -72,  1621,   830,  -643,
    -643,  -643,  -643,  -643,  -643,  -643,  -643,   523,  -643,  -643,
     737,   497,    44,  -643,  -643,  -643,  -643,  -643,  -643,  -643,
     169,  -643,   169,   399,  -643,  -643,  -643,  -643,  -643,   500,
     120,  -643,   115,  -643,  -643,  -643,  1276,  -643,   -31,  1621,
    1621,  1451,  1621,  1621,  -643,   856,   896,  1296,  1316,   916,
     405,   411,   936,   416,   417,   419,  1336,  1374,   421,   423,
     976,  1394,  1581,   953,  1354,  1414,  1450,   667,   834,   834,
     457,   457,   457,   457,   288,   288,   189,   189,  -643,  -643,
    -643,  1621,  1621,  1621,  -643,  -643,  -643,  1621,  1621,  -643,
    -643,  -643,  -643,   216,   274,   265,   491,  -643,  -643,   -47,
      30,   512,    30,   579,   422,  -643,     8,   513,  -643,   388,
    -643,  -643,  -643,  -643,    78,  -643,  -643,  -643,   526,  -643,
     428,   429,   541,  -643,  -643,   579,  -643,  -643,   579,   579,
    -643,   579,  -643,  -643,  -643,  -643,  -643,   579,   579,  -643,
    -643,  -643,   562,   579,   433,   554,  -643,  -643,  -643,   208,
     534,  1558,   557,   479,  -643,  1601,   490,  -643,  1621,    17,
     572,  -643,   573,     6,  -643,   494,  -643,   118,    78,  -643,
    -643,  -643,   442,   996,  1016,  1036,  1056,  1076,  1096,   456,
    1621,   265,   539,   216,   216,  -643,  -643,  -643,  -643,  -643,
    -643,   458,   579,   -26,   580,  -643,   558,   559,  -643,  -643,
     479,   546,   564,   565,  -643,   467,  -643,  -643,  -643,   598,
     471,  -643,   125,    78,  -643,  -643,  -643,  -643,  -643,  -643,
    -643,  -643,  -643,   472,   433,  -643,  1116,  -643,   579,   569,
     515,   515,  -643,   579,    17,   579,   473,  -643,  -643,   524,
    -643,   130,   265,   560,   252,  1136,   579,   576,  -643,  -643,
     369,  1156,  -643,  1176,  -643,  -643,   613,  -643,  -643,  -643,
     583,   607,  -643,  1196,   579,   184,   571,  -643,  -643,    17,
    -643,   579,  -643,  -643,  1216,  -643,  -643,  -643,  -643,   577,
    -643,  -643,  -643,  1236,  -643,  -643,  -643,   588,   618,    48,
     610,   665,  -643,  -643,  -643,  -643,  -643,  -643,  -643,   594,
     595,   599,  -643,  -643,   600,   601,  -643,    85,  -643,   603,
    -643,  -643,  -643,   618,   581,   604,    67,  -643,  -643,  -643,
     291,    62,  -643,  -643,   212,  -643,   605,  -643,   103,    85,
    -643,  -643,  -643,  -643,   590,   619,   608,   609,   511,   612,
     528,   629,   631,   544,   547,  -643,    13,  -643,    12,   294,
    -643,    85,   165,   619,   548,   618,   681,   591,   212,   212,
    -643,   212,  -643,   212,   212,  -643,  -643,   551,   567,   212,
    -643,  -643,  -643,   591,  -643,   650,  -643,   683,  -643,   570,
     574,    15,   578,   584,  -643,  -643,  -643,  -643,   705,    81,
     585,   586,   212,   589,   592,    81,  -643,  -643,  -643,   708,
    -643,  -643,  -643,   593,  -643,  -643,  -643,    81,  -643,  -643,
     471,  -643,   471,  -643,  -643,  -643,   471,  -643
};

/* YYPGOTO[NTERM-NUM].  */
static const short int yypgoto[] =
{
    -643,  -643,   -57,  -643,  -643,  -643,  -643,   501,  -643,  -643,
    -643,  -643,  -643,  -643,   614,  -643,  -643,   537,  -643,  -643,
    -643,  -196,  -643,  -643,  -643,  -643,   191,  -180,  -643,  -113,
    -393,    76,   112,    96,  -643,  -643,   127,  -643,    97,  -643,
      52,  -642,  -643,   138,  -519,  -198,  -643,  -643,  -271,  -643,
    -643,  -643,  -643,  -643,   290,  -643,  -643,  -643,  -643,  -643,
    -643,  -175,   -92,  -643,   -62,    82,   262,  -643,   235,  -643,
    -643,  -643,  -643,  -643,  -643,  -643,  -643,  -643,  -643,  -643,
    -643,  -643,  -643,  -643,  -423,   371,  -643,  -643,   122,  -461,
    -643,  -643,  -643,  -643,  -643,  -643,  -643,  -643,  -643,  -643,
    -473,  -643,  -643,  -643,  -643,  -643,   553,   -16,  -643,   664,
    -170,  -643,  -643,   257
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -306
static const short int yytable[] =
{
     182,   310,   312,   156,    91,   292,   129,   278,   279,   184,
     451,   322,   451,   166,   266,   214,   615,   657,   484,   615,
     175,   492,    18,   413,   294,   106,   314,   582,   582,   273,
     582,   400,    18,   131,   132,   201,   317,   318,   475,   476,
     477,   478,   479,   696,   583,   583,   217,   583,   190,    14,
     387,   542,   607,   193,   194,   703,   196,   198,    17,   606,
     133,   134,   208,   388,   616,   590,   615,   439,   136,   137,
     440,   219,   220,   273,   295,   437,    82,   582,   138,   139,
     140,    90,   203,   204,   606,   141,   571,   688,   438,   615,
     142,    19,   239,   240,   583,   263,   102,   689,   398,   143,
     582,    19,   608,   270,   144,   145,   146,   147,   148,   149,
     480,   551,   493,   631,   632,    83,   150,   583,   151,   317,
     318,   287,   317,   318,   406,   407,   606,   103,   456,   317,
     318,   319,   104,   152,   317,   318,   616,   590,   403,   153,
     379,   380,   497,   215,   267,   452,   595,   452,   659,   326,
     597,   682,   329,   330,   580,   332,   333,   197,   105,   274,
     335,   336,   337,   338,   339,   313,   320,   342,   321,   154,
     155,   346,   347,   106,   218,   350,   351,   205,   399,   352,
     353,   354,   355,   356,   357,   358,   359,   360,   361,   362,
     363,   364,   365,   366,   367,   368,   369,   370,   371,   372,
     373,   124,   125,   274,   617,   327,   377,   378,   457,   108,
     109,   131,   132,   434,   319,   685,   615,   319,   595,   408,
      83,   257,   258,   259,   319,   697,   617,   582,   297,   319,
     298,    84,   410,   411,    85,    86,    87,   308,   133,   134,
     308,   640,   309,   110,   583,   135,   136,   137,   661,   320,
     111,   409,   320,   401,   501,   402,   138,   139,   140,   320,
     112,   530,   502,   141,   320,   308,   548,   311,   142,   281,
     113,   114,   475,   476,   477,   478,   479,   143,   281,     1,
       2,     3,   144,   145,   146,   147,   148,   149,   115,   483,
       4,   550,   116,   117,   150,   615,   151,   119,   615,   308,
     120,   662,   529,   513,   514,   121,   582,   531,   288,   582,
     123,   152,    91,   565,   566,   567,   568,   153,   255,   256,
     257,   258,   259,   583,   127,   128,   583,   130,   441,   445,
     441,   448,   157,   158,   131,   132,    84,   261,   160,    85,
     276,   277,   626,   627,   480,   626,   627,   154,   155,   163,
     164,   165,   167,   463,   168,   174,   464,   465,   289,   466,
     292,   133,   134,   169,   171,   467,   468,   173,   135,   136,
     137,   470,   290,   288,   176,   180,   179,    34,   177,   138,
     139,   140,   178,   187,   628,   282,   141,   188,   283,   284,
     285,   142,   190,   192,   282,   291,   195,   283,   284,   435,
     143,    44,    45,   201,   207,   144,   145,   146,   147,   148,
     149,   209,   210,    46,   211,   222,   264,   150,   213,   151,
     516,   223,   224,   557,   629,   275,   265,   629,   225,   271,
     226,   227,   228,   229,   152,   230,   545,   290,   231,   232,
     153,   233,    34,   234,   272,   299,   235,   236,   131,   132,
     237,   268,   238,   280,   286,   303,   535,   316,   324,   325,
     291,   541,   304,   543,   340,   262,    44,    45,   572,   305,
     154,   155,   306,   307,   553,   133,   134,   315,    46,   323,
     341,   343,   135,   136,   137,   253,   254,   255,   256,   257,
     258,   259,   564,   138,   139,   140,   344,   630,   633,   573,
     141,   637,   704,   345,   705,   142,   348,   349,   707,   374,
     375,   376,   381,   382,   143,   131,   132,   384,   386,   144,
     145,   146,   147,   148,   149,   660,   630,   394,   444,   455,
     644,   150,   385,   151,   396,   669,   670,   405,   637,   421,
     672,   673,   133,   134,   436,   462,   675,   422,   152,   135,
     136,   137,   424,   425,   153,   426,   450,   429,   660,   430,
     138,   139,   140,   459,   460,   461,   469,   141,   471,   693,
     473,   482,   142,   485,   486,   489,   495,   496,   504,   269,
     499,   143,   131,   132,   154,   155,   144,   145,   146,   147,
     148,   149,   511,   512,   515,   518,   517,   519,   150,   522,
     151,   523,   525,   527,   528,   308,   536,   532,   544,   133,
     134,   537,   546,   554,   549,   152,   135,   136,   137,   560,
     561,   153,   581,   562,   570,   483,   609,   138,   139,   140,
     575,   610,   611,   582,   141,   621,   612,   613,   614,   142,
     619,   622,   639,   645,   646,   648,   649,   650,   143,   651,
     583,   154,   155,   144,   145,   146,   147,   148,   149,   584,
     585,   586,   587,   588,   652,   150,   653,   151,   654,   589,
     590,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     655,    22,   152,   656,   664,   666,   667,  -122,   153,   247,
     248,   249,   250,   251,   252,   253,   254,   255,   256,   257,
     258,   259,  -106,   674,   677,   591,   680,   592,   678,   687,
     681,   593,   699,   540,   683,    44,    45,   331,   154,   155,
     684,   691,   692,    23,    24,   694,   300,   671,   695,   701,
     620,   642,   658,    25,    26,    27,    28,   212,    29,    30,
     698,    22,   665,   498,   623,   676,   594,    31,    32,    33,
      34,   595,   521,   596,   181,   597,   539,    35,    36,    37,
      38,    39,    40,   447,     0,   663,   328,    41,    42,    43,
       0,   533,     0,   395,    44,    45,     0,     0,     0,     0,
       0,     0,     0,    23,    24,     0,    46,     0,     0,     0,
       0,     0,     0,    25,    26,    27,    28,    47,    29,    30,
       0,     0,     0,  -305,     0,     0,     0,    31,    32,    33,
      34,     0,    48,     0,     0,     0,     0,    35,    36,    37,
      38,    39,    40,     0,     0,     0,     0,    41,    42,    43,
       0,     0,     0,     0,    44,    45,    92,    93,    94,    95,
      96,    97,    98,    99,   100,     0,    46,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    47,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,     0,
       0,   241,    48,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   241,     0,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   241,     0,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   241,     0,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   241,     0,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   259,     0,     0,
     415,   241,   416,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   241,   334,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   241,   417,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   241,   420,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   241,   423,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   241,     0,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   241,   431,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   241,   505,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   241,   506,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   241,   507,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   241,   508,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   241,   509,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   241,   510,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   241,   534,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   241,   552,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   241,   558,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   241,   559,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   241,   563,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   241,   574,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,     0,   577,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   241,
     260,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   241,
     412,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,     0,
     418,     0,     0,    61,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,     0,
     419,     0,     0,     0,     0,    61,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
     427,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,     0,     0,   414,     0,    62,
      63,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   428,     0,
       0,     0,    63,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    64,     0,     0,     0,     0,   432,    65,
      66,    67,    68,    69,   -42,    70,    71,    72,     0,    73,
      74,    75,    76,    77,     0,    64,     0,     0,    78,    79,
      80,    65,    66,    67,    68,    69,     0,    70,    71,    72,
       0,    73,    74,    75,    76,    77,     0,     0,     0,     0,
      78,    79,    80,   241,     0,   242,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,     0,     0,   483,   241,   433,   242,   243,
     244,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     254,   255,   256,   257,   258,   259,   241,   488,   242,   243,
     244,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     254,   255,   256,   257,   258,   259,   241,     0,   242,   243,
     244,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     254,   255,   256,   257,   258,   259
};

static const short int yycheck[] =
{
      92,   199,   200,    65,    20,   185,    63,   177,   178,   101,
       4,   207,     4,    75,     4,     6,     4,     4,   441,     4,
      82,     4,    53,    54,     4,     4,   201,    15,    15,     4,
      15,   302,    53,     3,     4,     4,     4,     5,    64,    65,
      66,    67,    68,   685,    32,    32,     6,    32,     4,     0,
     122,   524,     4,   110,   111,   697,   113,   114,     4,   578,
      30,    31,   119,   135,    51,    52,     4,    37,    38,    39,
      40,   133,   134,     4,    54,   122,     6,    15,    48,    49,
      50,    53,     4,     5,   603,    55,   559,     6,   135,     4,
      60,   122,   154,   155,    32,   157,    53,    16,    54,    69,
      15,   122,    54,   165,    74,    75,    76,    77,    78,    79,
     136,   534,    95,    51,    52,     4,    86,    32,    88,     4,
       5,   183,     4,     5,     4,     5,   645,    53,   399,     4,
       5,    99,    37,   103,     4,     5,    51,    52,   313,   109,
       3,     4,   136,   134,   134,   139,   133,   139,   136,   211,
     137,   136,   214,   215,   577,   217,   218,   136,    37,   134,
     222,   223,   224,   225,   226,   134,   134,   229,   136,   139,
     140,   233,   234,     4,   134,   237,   238,    99,   134,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,   134,   135,   134,   597,   136,   268,   269,   404,    53,
      37,     3,     4,   383,    99,   676,     4,    99,   133,    99,
       4,    32,    33,    34,    99,   686,   619,    15,   134,    99,
     136,   120,   324,   325,   123,   124,   125,   134,    30,    31,
     134,   138,   136,    37,    32,    37,    38,    39,   641,   134,
      37,   136,   134,   310,   136,   312,    48,    49,    50,   134,
      37,   136,   458,    55,   134,   134,   136,   136,    60,     4,
      37,    37,    64,    65,    66,    67,    68,    69,     4,   116,
     117,   118,    74,    75,    76,    77,    78,    79,    37,    37,
     127,    39,    37,    37,    86,     4,    88,    37,     4,   134,
      37,   136,   500,   473,   474,    37,    15,   503,     4,    15,
      37,   103,   328,   129,   130,   131,   132,   109,    30,    31,
      32,    33,    34,    32,   126,     4,    32,     4,   390,   391,
     392,   393,     4,     4,     3,     4,   120,     6,     4,   123,
     124,   125,    51,    52,   136,    51,    52,   139,   140,     4,
       4,     4,     4,   415,     3,   105,   418,   419,    54,   421,
     540,    30,    31,     4,     4,   427,   428,     4,    37,    38,
      39,   433,    68,     4,     4,   135,    54,    73,    16,    48,
      49,    50,    16,     4,    93,   120,    55,     4,   123,   124,
     125,    60,     4,     4,   120,    91,     4,   123,   124,   125,
      69,    97,    98,     4,    37,    74,    75,    76,    77,    78,
      79,     4,     4,   109,    37,    37,   134,    86,    53,    88,
     482,    37,    37,    54,   133,    53,   134,   133,    37,   134,
      37,    37,    37,    37,   103,    37,   528,    68,    37,    37,
     109,    37,    73,    37,   134,   136,    37,    37,     3,     4,
      37,     6,    37,   135,    54,   136,   518,    37,     6,     6,
      91,   523,   136,   525,     4,   134,    97,    98,   560,   136,
     139,   140,   136,   136,   536,    30,    31,   136,   109,   136,
       4,     4,    37,    38,    39,    28,    29,    30,    31,    32,
      33,    34,   554,    48,    49,    50,     4,   610,   611,   561,
      55,   614,   700,     4,   702,    60,     4,     4,   706,     4,
       4,     4,     4,     4,    69,     3,     4,   135,     4,    74,
      75,    76,    77,    78,    79,   638,   639,     4,    16,    16,
     622,    86,   135,    88,    37,   648,   649,    37,   651,   134,
     653,   654,    30,    31,    53,     4,   659,   136,   103,    37,
      38,    39,   136,   136,   109,   136,   134,   136,   671,   136,
      48,    49,    50,    37,   136,   136,     4,    55,   135,   682,
      16,    37,    60,    16,    95,    85,     4,     4,   136,   134,
      86,    69,     3,     4,   139,   140,    74,    75,    76,    77,
      78,    79,   136,    54,   136,    37,    16,    38,    86,    53,
      88,    37,    37,   136,     6,   134,    37,   135,   135,    30,
      31,    96,    88,    37,    54,   103,    37,    38,    39,     6,
      37,   109,     4,    16,    53,    37,    16,    48,    49,    50,
      53,    37,    37,    15,    55,    54,    37,    37,    37,    60,
      37,    37,    37,    53,    25,    37,    37,   136,    69,    37,
      32,   139,   140,    74,    75,    76,    77,    78,    79,    41,
      42,    43,    44,    45,   136,    86,    37,    88,    37,    51,
      52,     6,     7,     8,     9,    10,    11,    12,    13,    14,
     136,     4,   103,   136,   136,     4,    95,   136,   109,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    37,   136,    54,    87,   136,    89,    25,     4,
     136,    93,     4,   522,   136,    97,    98,   216,   139,   140,
     136,   136,   136,    46,    47,   136,   189,   651,   136,   136,
     603,   619,   636,    56,    57,    58,    59,   123,    61,    62,
     688,     4,   645,   453,   606,   663,   128,    70,    71,    72,
      73,   133,   490,   135,    90,   137,   521,    80,    81,    82,
      83,    84,    85,   392,    -1,   643,   213,    90,    91,    92,
      -1,   514,    -1,    36,    97,    98,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    46,    47,    -1,   109,    -1,    -1,    -1,
      -1,    -1,    -1,    56,    57,    58,    59,   120,    61,    62,
      -1,    -1,    -1,   126,    -1,    -1,    -1,    70,    71,    72,
      73,    -1,   135,    -1,    -1,    -1,    -1,    80,    81,    82,
      83,    84,    85,    -1,    -1,    -1,    -1,    90,    91,    92,
      -1,    -1,    -1,    -1,    97,    98,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    -1,   109,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    -1,
      -1,    15,   135,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    15,    -1,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    15,    -1,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    15,    -1,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    15,    -1,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    -1,    -1,
     134,    15,   136,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    15,   136,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    15,   136,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    15,   136,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    15,   136,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    15,    -1,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    15,   136,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    15,   136,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    15,   136,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    15,   136,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    15,   136,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    15,   136,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    15,   136,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    15,   136,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    15,   136,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    15,   136,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    15,   136,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    15,   136,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    15,   136,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    -1,   136,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    15,
     134,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    15,
     134,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    -1,
     134,    -1,    -1,     4,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    -1,
     134,    -1,    -1,    -1,    -1,     4,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
     134,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    -1,    -1,    36,    -1,    38,
      61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,    -1,
      -1,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,   134,   100,
     101,   102,   103,   104,   105,   106,   107,   108,    -1,   110,
     111,   112,   113,   114,    -1,    94,    -1,    -1,   119,   120,
     121,   100,   101,   102,   103,   104,    -1,   106,   107,   108,
      -1,   110,   111,   112,   113,   114,    -1,    -1,    -1,    -1,
     119,   120,   121,    15,    -1,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    -1,    -1,    37,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    15,    -1,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,   116,   117,   118,   127,   142,   156,   157,   146,   147,
     144,   145,   243,   244,     0,   158,   148,     4,    53,   122,
     247,   248,     4,    46,    47,    56,    57,    58,    59,    61,
      62,    70,    71,    72,    73,    80,    81,    82,    83,    84,
      85,    90,    91,    92,    97,    98,   109,   120,   135,   159,
     166,   168,   185,   187,   196,   197,   199,   201,   235,   245,
     246,     4,    38,    61,    94,   100,   101,   102,   103,   104,
     106,   107,   108,   110,   111,   112,   113,   114,   119,   120,
     121,   149,     6,     4,   120,   123,   124,   125,   250,   251,
      53,   248,     6,     7,     8,     9,    10,    11,    12,    13,
      14,   183,    53,    53,    37,    37,     4,   143,    53,    37,
      37,    37,    37,    37,    37,    37,    37,    37,   160,    37,
      37,    37,   169,    37,   134,   135,   184,   126,     4,   143,
       4,     3,     4,    30,    31,    37,    38,    39,    48,    49,
      50,    55,    60,    69,    74,    75,    76,    77,    78,    79,
      86,    88,   103,   109,   139,   140,   205,     4,     4,   153,
       4,   152,   151,     4,     4,     4,   205,     4,     3,     4,
     154,     4,   155,     4,   105,   205,     4,    16,    16,    54,
     135,   250,   203,   204,   203,   167,   236,     4,     4,   161,
       4,   189,     4,   143,   143,     4,   143,   136,   143,   198,
     200,     4,   202,     4,     5,    99,   162,    37,   143,     4,
       4,    37,   155,    53,     6,   134,   150,     6,   134,   205,
     205,   205,    37,    37,    37,    37,    37,    37,    37,    37,
      37,    37,    37,    37,    37,    37,    37,    37,    37,   205,
     205,    15,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
     134,     6,   134,   205,   134,   134,     4,   134,     6,   134,
     205,   134,   134,     4,   134,    53,   124,   125,   251,   251,
     135,     4,   120,   123,   124,   125,    54,   205,     4,    54,
      68,    91,   168,   211,     4,    54,   237,   134,   136,   136,
     158,   190,   188,   136,   136,   136,   136,   136,   134,   136,
     186,   136,   186,   134,   202,   136,    37,     4,     5,    99,
     134,   136,   162,   136,     6,     6,   205,   136,   247,   205,
     205,   148,   205,   205,   136,   205,   205,   205,   205,   205,
       4,     4,   205,     4,     4,     4,   205,   205,     4,     4,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,     4,     4,     4,   205,   205,     3,
       4,     4,     4,   253,   135,   135,     4,   122,   135,   249,
     212,   217,   222,   238,     4,    36,    37,   193,    54,   134,
     189,   143,   143,   202,   163,    37,     4,     5,    99,   136,
     203,   203,   134,    54,    36,   134,   136,   136,   134,   134,
     136,   134,   136,   136,   136,   136,   136,   134,   134,   136,
     136,   136,   134,    16,   251,   125,    53,   122,   135,    37,
      40,   205,   225,   226,    16,   205,   227,   226,   205,   240,
     134,     4,   139,   194,   195,    16,   189,   162,   165,    37,
     136,   136,     4,   205,   205,   205,   205,   205,   205,     4,
     205,   135,   254,    16,   252,    64,    65,    66,    67,    68,
     136,   224,    37,    37,   225,    16,    95,   207,    16,    85,
     228,   223,     4,    95,   241,     4,     4,   136,   195,    86,
     191,   136,   162,   164,   136,   136,   136,   136,   136,   136,
     136,   136,    54,   251,   251,   136,   205,    16,    37,    38,
     208,   207,    53,    37,   242,    37,   239,   136,     6,   186,
     136,   162,   135,   254,   136,   205,    37,    96,   209,   209,
     167,   205,   241,   205,   135,   203,    88,   192,   136,    54,
      39,   225,   136,   205,    37,   213,   218,    54,   136,   136,
       6,    37,    16,   136,   205,   129,   130,   131,   132,   210,
      53,   241,   203,   205,   136,    53,   219,   136,   214,   231,
     225,     4,    15,    32,    41,    42,    43,    44,    45,    51,
      52,    87,    89,    93,   128,   133,   135,   137,   170,   171,
     174,   175,   177,   178,   179,   180,   185,     4,    54,    16,
      37,    37,    37,    37,    37,     4,    51,   171,   173,    37,
     177,    54,    37,   184,   232,   220,    51,    52,    93,   133,
     170,    51,    52,   170,   181,   203,   176,   170,   172,    37,
     138,   186,   173,   215,   203,    53,    25,   229,    37,    37,
     136,    37,   136,    37,    37,   136,   136,     4,   174,   136,
     170,   171,   136,   229,   136,   179,     4,    95,   206,   170,
     170,   172,   170,   170,   136,   170,   206,    54,    25,   230,
     136,   136,   136,   136,   136,   230,   233,     4,     6,    16,
     182,   136,   136,   170,   136,   136,   182,   230,   181,     4,
     221,   136,   216,   182,   186,   186,   234,   186
};

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)		\
   ((Current).first_line   = (Rhs)[1].first_line,	\
    (Current).first_column = (Rhs)[1].first_column,	\
    (Current).last_line    = (Rhs)[N].last_line,	\
    (Current).last_column  = (Rhs)[N].last_column)
#endif

/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)

# define YYDSYMPRINT(Args)			\
do {						\
  if (yydebug)					\
    yysymprint Args;				\
} while (0)

# define YYDSYMPRINTF(Title, Token, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr, 					\
                  Token, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short int *bottom, short int *top)
#else
static void
yy_stack_print (bottom, top)
    short int *bottom;
    short int *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_reduce_print (int yyrule)
#else
static void
yy_reduce_print (yyrule)
    int yyrule;
#endif
{
  int yyi;
  unsigned int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %u), ",
             yyrule - 1, yylno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname [yyrhs[yyi]]);
  YYFPRINTF (stderr, "-> %s\n", yytname [yyr1[yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YYDSYMPRINT(Args)
# define YYDSYMPRINTF(Title, Token, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if defined (YYMAXDEPTH) && YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  register const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  register char *yyd = yydest;
  register const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

#endif /* !YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (yytype < YYNTOKENS)
    {
      YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
# ifdef YYPRINT
      YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
    }
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yydestruct (int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yytype, yyvaluep)
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  switch (yytype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM)
# else
int yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short int yyssa[YYINITDEPTH];
  short int *yyss = yyssa;
  register short int *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;



#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;


  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short int *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	short int *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YYDSYMPRINTF ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %s, ", yytname[yytoken]));

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 7:
#line 173 "ldgram.y"
    { ldlex_defsym(); }
    break;

  case 8:
#line 175 "ldgram.y"
    {
		  ldlex_popstate();
		  lang_add_assignment(exp_assop(yyvsp[-1].token,yyvsp[-2].name,yyvsp[0].etree));
		}
    break;

  case 9:
#line 183 "ldgram.y"
    {
		  ldlex_mri_script ();
		  PUSH_ERROR (_("MRI style script"));
		}
    break;

  case 10:
#line 188 "ldgram.y"
    {
		  ldlex_popstate ();
		  mri_draw_tree ();
		  POP_ERROR ();
		}
    break;

  case 15:
#line 203 "ldgram.y"
    {
			einfo(_("%P%F: unrecognised keyword in MRI style script '%s'\n"),yyvsp[0].name);
			}
    break;

  case 16:
#line 206 "ldgram.y"
    {
			config.map_filename = "-";
			}
    break;

  case 19:
#line 212 "ldgram.y"
    { mri_public(yyvsp[-2].name, yyvsp[0].etree); }
    break;

  case 20:
#line 214 "ldgram.y"
    { mri_public(yyvsp[-2].name, yyvsp[0].etree); }
    break;

  case 21:
#line 216 "ldgram.y"
    { mri_public(yyvsp[-1].name, yyvsp[0].etree); }
    break;

  case 22:
#line 218 "ldgram.y"
    { mri_format(yyvsp[0].name); }
    break;

  case 23:
#line 220 "ldgram.y"
    { mri_output_section(yyvsp[-2].name, yyvsp[0].etree);}
    break;

  case 24:
#line 222 "ldgram.y"
    { mri_output_section(yyvsp[-1].name, yyvsp[0].etree);}
    break;

  case 25:
#line 224 "ldgram.y"
    { mri_output_section(yyvsp[-2].name, yyvsp[0].etree);}
    break;

  case 26:
#line 226 "ldgram.y"
    { mri_align(yyvsp[-2].name,yyvsp[0].etree); }
    break;

  case 27:
#line 228 "ldgram.y"
    { mri_align(yyvsp[-2].name,yyvsp[0].etree); }
    break;

  case 28:
#line 230 "ldgram.y"
    { mri_alignmod(yyvsp[-2].name,yyvsp[0].etree); }
    break;

  case 29:
#line 232 "ldgram.y"
    { mri_alignmod(yyvsp[-2].name,yyvsp[0].etree); }
    break;

  case 32:
#line 236 "ldgram.y"
    { mri_name(yyvsp[0].name); }
    break;

  case 33:
#line 238 "ldgram.y"
    { mri_alias(yyvsp[-2].name,yyvsp[0].name,0);}
    break;

  case 34:
#line 240 "ldgram.y"
    { mri_alias (yyvsp[-2].name, 0, (int) yyvsp[0].bigint.integer); }
    break;

  case 35:
#line 242 "ldgram.y"
    { mri_base(yyvsp[0].etree); }
    break;

  case 36:
#line 244 "ldgram.y"
    { mri_truncate ((unsigned int) yyvsp[0].bigint.integer); }
    break;

  case 39:
#line 248 "ldgram.y"
    { ldlex_script (); ldfile_open_command_file(yyvsp[0].name); }
    break;

  case 40:
#line 250 "ldgram.y"
    { ldlex_popstate (); }
    break;

  case 41:
#line 252 "ldgram.y"
    { lang_add_entry (yyvsp[0].name, FALSE); }
    break;

  case 43:
#line 257 "ldgram.y"
    { mri_order(yyvsp[0].name); }
    break;

  case 44:
#line 258 "ldgram.y"
    { mri_order(yyvsp[0].name); }
    break;

  case 46:
#line 264 "ldgram.y"
    { mri_load(yyvsp[0].name); }
    break;

  case 47:
#line 265 "ldgram.y"
    { mri_load(yyvsp[0].name); }
    break;

  case 48:
#line 270 "ldgram.y"
    { mri_only_load(yyvsp[0].name); }
    break;

  case 49:
#line 272 "ldgram.y"
    { mri_only_load(yyvsp[0].name); }
    break;

  case 50:
#line 276 "ldgram.y"
    { yyval.name = NULL; }
    break;

  case 53:
#line 283 "ldgram.y"
    { ldlang_add_undef (yyvsp[0].name); }
    break;

  case 54:
#line 285 "ldgram.y"
    { ldlang_add_undef (yyvsp[0].name); }
    break;

  case 55:
#line 287 "ldgram.y"
    { ldlang_add_undef (yyvsp[0].name); }
    break;

  case 56:
#line 291 "ldgram.y"
    {
	 ldlex_both();
	}
    break;

  case 57:
#line 295 "ldgram.y"
    {
	ldlex_popstate();
	}
    break;

  case 70:
#line 320 "ldgram.y"
    { lang_add_target(yyvsp[-1].name); }
    break;

  case 71:
#line 322 "ldgram.y"
    { ldfile_add_library_path (yyvsp[-1].name, FALSE); }
    break;

  case 72:
#line 324 "ldgram.y"
    { lang_add_output(yyvsp[-1].name, 1); }
    break;

  case 73:
#line 326 "ldgram.y"
    { lang_add_output_format (yyvsp[-1].name, (char *) NULL,
					    (char *) NULL, 1); }
    break;

  case 74:
#line 329 "ldgram.y"
    { lang_add_output_format (yyvsp[-5].name, yyvsp[-3].name, yyvsp[-1].name, 1); }
    break;

  case 75:
#line 331 "ldgram.y"
    { ldfile_set_output_arch (yyvsp[-1].name, bfd_arch_unknown); }
    break;

  case 76:
#line 333 "ldgram.y"
    { command_line.force_common_definition = TRUE ; }
    break;

  case 77:
#line 335 "ldgram.y"
    { command_line.inhibit_common_definition = TRUE ; }
    break;

  case 79:
#line 338 "ldgram.y"
    { lang_enter_group (); }
    break;

  case 80:
#line 340 "ldgram.y"
    { lang_leave_group (); }
    break;

  case 81:
#line 342 "ldgram.y"
    { lang_add_map(yyvsp[-1].name); }
    break;

  case 82:
#line 344 "ldgram.y"
    { ldlex_script (); ldfile_open_command_file(yyvsp[0].name); }
    break;

  case 83:
#line 346 "ldgram.y"
    { ldlex_popstate (); }
    break;

  case 84:
#line 348 "ldgram.y"
    {
		  lang_add_nocrossref (yyvsp[-1].nocrossref);
		}
    break;

  case 86:
#line 356 "ldgram.y"
    { lang_add_input_file(yyvsp[0].name,lang_input_file_is_search_file_enum,
				 (char *)NULL); }
    break;

  case 87:
#line 359 "ldgram.y"
    { lang_add_input_file(yyvsp[0].name,lang_input_file_is_search_file_enum,
				 (char *)NULL); }
    break;

  case 88:
#line 362 "ldgram.y"
    { lang_add_input_file(yyvsp[0].name,lang_input_file_is_search_file_enum,
				 (char *)NULL); }
    break;

  case 89:
#line 365 "ldgram.y"
    { lang_add_input_file(yyvsp[0].name,lang_input_file_is_l_enum,
				 (char *)NULL); }
    break;

  case 90:
#line 368 "ldgram.y"
    { lang_add_input_file(yyvsp[0].name,lang_input_file_is_l_enum,
				 (char *)NULL); }
    break;

  case 91:
#line 371 "ldgram.y"
    { lang_add_input_file(yyvsp[0].name,lang_input_file_is_l_enum,
				 (char *)NULL); }
    break;

  case 92:
#line 374 "ldgram.y"
    { yyval.integer = as_needed; as_needed = TRUE; }
    break;

  case 93:
#line 376 "ldgram.y"
    { as_needed = yyvsp[-2].integer; }
    break;

  case 94:
#line 378 "ldgram.y"
    { yyval.integer = as_needed; as_needed = TRUE; }
    break;

  case 95:
#line 380 "ldgram.y"
    { as_needed = yyvsp[-2].integer; }
    break;

  case 96:
#line 382 "ldgram.y"
    { yyval.integer = as_needed; as_needed = TRUE; }
    break;

  case 97:
#line 384 "ldgram.y"
    { as_needed = yyvsp[-2].integer; }
    break;

  case 102:
#line 399 "ldgram.y"
    { lang_add_entry (yyvsp[-1].name, FALSE); }
    break;

  case 104:
#line 401 "ldgram.y"
    {ldlex_expression ();}
    break;

  case 105:
#line 402 "ldgram.y"
    { ldlex_popstate ();
		  lang_add_assignment (exp_assert (yyvsp[-3].etree, yyvsp[-1].name)); }
    break;

  case 106:
#line 410 "ldgram.y"
    {
			  yyval.cname = yyvsp[0].name;
			}
    break;

  case 107:
#line 414 "ldgram.y"
    {
			  yyval.cname = "*";
			}
    break;

  case 108:
#line 418 "ldgram.y"
    {
			  yyval.cname = "?";
			}
    break;

  case 109:
#line 425 "ldgram.y"
    {
			  yyval.wildcard.name = yyvsp[0].cname;
			  yyval.wildcard.sorted = none;
			  yyval.wildcard.exclude_name_list = NULL;
			}
    break;

  case 110:
#line 431 "ldgram.y"
    {
			  yyval.wildcard.name = yyvsp[0].cname;
			  yyval.wildcard.sorted = none;
			  yyval.wildcard.exclude_name_list = yyvsp[-2].name_list;
			}
    break;

  case 111:
#line 437 "ldgram.y"
    {
			  yyval.wildcard.name = yyvsp[-1].cname;
			  yyval.wildcard.sorted = by_name;
			  yyval.wildcard.exclude_name_list = NULL;
			}
    break;

  case 112:
#line 443 "ldgram.y"
    {
			  yyval.wildcard.name = yyvsp[-1].cname;
			  yyval.wildcard.sorted = by_alignment;
			  yyval.wildcard.exclude_name_list = NULL;
			}
    break;

  case 113:
#line 449 "ldgram.y"
    {
			  yyval.wildcard.name = yyvsp[-2].cname;
			  yyval.wildcard.sorted = by_name_alignment;
			  yyval.wildcard.exclude_name_list = NULL;
			}
    break;

  case 114:
#line 455 "ldgram.y"
    {
			  yyval.wildcard.name = yyvsp[-2].cname;
			  yyval.wildcard.sorted = by_name;
			  yyval.wildcard.exclude_name_list = NULL;
			}
    break;

  case 115:
#line 461 "ldgram.y"
    {
			  yyval.wildcard.name = yyvsp[-2].cname;
			  yyval.wildcard.sorted = by_alignment_name;
			  yyval.wildcard.exclude_name_list = NULL;
			}
    break;

  case 116:
#line 467 "ldgram.y"
    {
			  yyval.wildcard.name = yyvsp[-2].cname;
			  yyval.wildcard.sorted = by_alignment;
			  yyval.wildcard.exclude_name_list = NULL;
			}
    break;

  case 117:
#line 473 "ldgram.y"
    {
			  yyval.wildcard.name = yyvsp[-1].cname;
			  yyval.wildcard.sorted = by_name;
			  yyval.wildcard.exclude_name_list = yyvsp[-3].name_list;
			}
    break;

  case 118:
#line 482 "ldgram.y"
    {
			  struct name_list *tmp;
			  tmp = (struct name_list *) xmalloc (sizeof *tmp);
			  tmp->name = yyvsp[0].cname;
			  tmp->next = yyvsp[-1].name_list;
			  yyval.name_list = tmp;
			}
    break;

  case 119:
#line 491 "ldgram.y"
    {
			  struct name_list *tmp;
			  tmp = (struct name_list *) xmalloc (sizeof *tmp);
			  tmp->name = yyvsp[0].cname;
			  tmp->next = NULL;
			  yyval.name_list = tmp;
			}
    break;

  case 120:
#line 502 "ldgram.y"
    {
			  struct wildcard_list *tmp;
			  tmp = (struct wildcard_list *) xmalloc (sizeof *tmp);
			  tmp->next = yyvsp[-2].wildcard_list;
			  tmp->spec = yyvsp[0].wildcard;
			  yyval.wildcard_list = tmp;
			}
    break;

  case 121:
#line 511 "ldgram.y"
    {
			  struct wildcard_list *tmp;
			  tmp = (struct wildcard_list *) xmalloc (sizeof *tmp);
			  tmp->next = NULL;
			  tmp->spec = yyvsp[0].wildcard;
			  yyval.wildcard_list = tmp;
			}
    break;

  case 122:
#line 522 "ldgram.y"
    {
			  struct wildcard_spec tmp;
			  tmp.name = yyvsp[0].name;
			  tmp.exclude_name_list = NULL;
			  tmp.sorted = none;
			  lang_add_wild (&tmp, NULL, ldgram_had_keep);
			}
    break;

  case 123:
#line 530 "ldgram.y"
    {
			  lang_add_wild (NULL, yyvsp[-1].wildcard_list, ldgram_had_keep);
			}
    break;

  case 124:
#line 534 "ldgram.y"
    {
			  lang_add_wild (&yyvsp[-3].wildcard, yyvsp[-1].wildcard_list, ldgram_had_keep);
			}
    break;

  case 126:
#line 542 "ldgram.y"
    { ldgram_had_keep = TRUE; }
    break;

  case 127:
#line 544 "ldgram.y"
    { ldgram_had_keep = FALSE; }
    break;

  case 129:
#line 550 "ldgram.y"
    {
 		lang_add_attribute(lang_object_symbols_statement_enum);
	      	}
    break;

  case 131:
#line 555 "ldgram.y"
    {

		  lang_add_attribute(lang_constructors_statement_enum);
		}
    break;

  case 132:
#line 560 "ldgram.y"
    {
		  constructors_sorted = TRUE;
		  lang_add_attribute (lang_constructors_statement_enum);
		}
    break;

  case 134:
#line 566 "ldgram.y"
    {
			  lang_add_data ((int) yyvsp[-3].integer, yyvsp[-1].etree);
			}
    break;

  case 135:
#line 571 "ldgram.y"
    {
			  lang_add_fill (yyvsp[-1].fill);
			}
    break;

  case 140:
#line 588 "ldgram.y"
    { yyval.integer = yyvsp[0].token; }
    break;

  case 141:
#line 590 "ldgram.y"
    { yyval.integer = yyvsp[0].token; }
    break;

  case 142:
#line 592 "ldgram.y"
    { yyval.integer = yyvsp[0].token; }
    break;

  case 143:
#line 594 "ldgram.y"
    { yyval.integer = yyvsp[0].token; }
    break;

  case 144:
#line 596 "ldgram.y"
    { yyval.integer = yyvsp[0].token; }
    break;

  case 145:
#line 601 "ldgram.y"
    {
		  yyval.fill = exp_get_fill (yyvsp[0].etree, 0, "fill value");
		}
    break;

  case 146:
#line 608 "ldgram.y"
    { yyval.fill = yyvsp[0].fill; }
    break;

  case 147:
#line 609 "ldgram.y"
    { yyval.fill = (fill_type *) 0; }
    break;

  case 148:
#line 614 "ldgram.y"
    { yyval.token = '+'; }
    break;

  case 149:
#line 616 "ldgram.y"
    { yyval.token = '-'; }
    break;

  case 150:
#line 618 "ldgram.y"
    { yyval.token = '*'; }
    break;

  case 151:
#line 620 "ldgram.y"
    { yyval.token = '/'; }
    break;

  case 152:
#line 622 "ldgram.y"
    { yyval.token = LSHIFT; }
    break;

  case 153:
#line 624 "ldgram.y"
    { yyval.token = RSHIFT; }
    break;

  case 154:
#line 626 "ldgram.y"
    { yyval.token = '&'; }
    break;

  case 155:
#line 628 "ldgram.y"
    { yyval.token = '|'; }
    break;

  case 158:
#line 638 "ldgram.y"
    {
		  lang_add_assignment (exp_assop (yyvsp[-1].token, yyvsp[-2].name, yyvsp[0].etree));
		}
    break;

  case 159:
#line 642 "ldgram.y"
    {
		  lang_add_assignment (exp_assop ('=', yyvsp[-2].name,
						  exp_binop (yyvsp[-1].token,
							     exp_nameop (NAME,
									 yyvsp[-2].name),
							     yyvsp[0].etree)));
		}
    break;

  case 160:
#line 650 "ldgram.y"
    {
		  lang_add_assignment (exp_provide (yyvsp[-3].name, yyvsp[-1].etree, FALSE));
		}
    break;

  case 161:
#line 654 "ldgram.y"
    {
		  lang_add_assignment (exp_provide (yyvsp[-3].name, yyvsp[-1].etree, TRUE));
		}
    break;

  case 168:
#line 676 "ldgram.y"
    { region = lang_memory_region_lookup (yyvsp[0].name, TRUE); }
    break;

  case 169:
#line 679 "ldgram.y"
    {}
    break;

  case 170:
#line 684 "ldgram.y"
    {
		  region->origin = exp_get_vma (yyvsp[0].etree, 0, "origin");
		  region->current = region->origin;
		}
    break;

  case 171:
#line 692 "ldgram.y"
    {
		  region->length = exp_get_vma (yyvsp[0].etree, -1, "length");
		}
    break;

  case 172:
#line 699 "ldgram.y"
    { /* dummy action to avoid bison 1.25 error message */ }
    break;

  case 176:
#line 710 "ldgram.y"
    { lang_set_flags (region, yyvsp[0].name, 0); }
    break;

  case 177:
#line 712 "ldgram.y"
    { lang_set_flags (region, yyvsp[0].name, 1); }
    break;

  case 178:
#line 717 "ldgram.y"
    { lang_startup(yyvsp[-1].name); }
    break;

  case 180:
#line 723 "ldgram.y"
    { ldemul_hll((char *)NULL); }
    break;

  case 181:
#line 728 "ldgram.y"
    { ldemul_hll(yyvsp[0].name); }
    break;

  case 182:
#line 730 "ldgram.y"
    { ldemul_hll(yyvsp[0].name); }
    break;

  case 184:
#line 738 "ldgram.y"
    { ldemul_syslib(yyvsp[0].name); }
    break;

  case 186:
#line 744 "ldgram.y"
    { lang_float(TRUE); }
    break;

  case 187:
#line 746 "ldgram.y"
    { lang_float(FALSE); }
    break;

  case 188:
#line 751 "ldgram.y"
    {
		  yyval.nocrossref = NULL;
		}
    break;

  case 189:
#line 755 "ldgram.y"
    {
		  struct lang_nocrossref *n;

		  n = (struct lang_nocrossref *) xmalloc (sizeof *n);
		  n->name = yyvsp[-1].name;
		  n->next = yyvsp[0].nocrossref;
		  yyval.nocrossref = n;
		}
    break;

  case 190:
#line 764 "ldgram.y"
    {
		  struct lang_nocrossref *n;

		  n = (struct lang_nocrossref *) xmalloc (sizeof *n);
		  n->name = yyvsp[-2].name;
		  n->next = yyvsp[0].nocrossref;
		  yyval.nocrossref = n;
		}
    break;

  case 191:
#line 774 "ldgram.y"
    { ldlex_expression (); }
    break;

  case 192:
#line 776 "ldgram.y"
    { ldlex_popstate (); yyval.etree=yyvsp[0].etree;}
    break;

  case 193:
#line 781 "ldgram.y"
    { yyval.etree = exp_unop ('-', yyvsp[0].etree); }
    break;

  case 194:
#line 783 "ldgram.y"
    { yyval.etree = yyvsp[-1].etree; }
    break;

  case 195:
#line 785 "ldgram.y"
    { yyval.etree = exp_unop ((int) yyvsp[-3].integer,yyvsp[-1].etree); }
    break;

  case 196:
#line 787 "ldgram.y"
    { yyval.etree = exp_unop ('!', yyvsp[0].etree); }
    break;

  case 197:
#line 789 "ldgram.y"
    { yyval.etree = yyvsp[0].etree; }
    break;

  case 198:
#line 791 "ldgram.y"
    { yyval.etree = exp_unop ('~', yyvsp[0].etree);}
    break;

  case 199:
#line 794 "ldgram.y"
    { yyval.etree = exp_binop ('*', yyvsp[-2].etree, yyvsp[0].etree); }
    break;

  case 200:
#line 796 "ldgram.y"
    { yyval.etree = exp_binop ('/', yyvsp[-2].etree, yyvsp[0].etree); }
    break;

  case 201:
#line 798 "ldgram.y"
    { yyval.etree = exp_binop ('%', yyvsp[-2].etree, yyvsp[0].etree); }
    break;

  case 202:
#line 800 "ldgram.y"
    { yyval.etree = exp_binop ('+', yyvsp[-2].etree, yyvsp[0].etree); }
    break;

  case 203:
#line 802 "ldgram.y"
    { yyval.etree = exp_binop ('-' , yyvsp[-2].etree, yyvsp[0].etree); }
    break;

  case 204:
#line 804 "ldgram.y"
    { yyval.etree = exp_binop (LSHIFT , yyvsp[-2].etree, yyvsp[0].etree); }
    break;

  case 205:
#line 806 "ldgram.y"
    { yyval.etree = exp_binop (RSHIFT , yyvsp[-2].etree, yyvsp[0].etree); }
    break;

  case 206:
#line 808 "ldgram.y"
    { yyval.etree = exp_binop (EQ , yyvsp[-2].etree, yyvsp[0].etree); }
    break;

  case 207:
#line 810 "ldgram.y"
    { yyval.etree = exp_binop (NE , yyvsp[-2].etree, yyvsp[0].etree); }
    break;

  case 208:
#line 812 "ldgram.y"
    { yyval.etree = exp_binop (LE , yyvsp[-2].etree, yyvsp[0].etree); }
    break;

  case 209:
#line 814 "ldgram.y"
    { yyval.etree = exp_binop (GE , yyvsp[-2].etree, yyvsp[0].etree); }
    break;

  case 210:
#line 816 "ldgram.y"
    { yyval.etree = exp_binop ('<' , yyvsp[-2].etree, yyvsp[0].etree); }
    break;

  case 211:
#line 818 "ldgram.y"
    { yyval.etree = exp_binop ('>' , yyvsp[-2].etree, yyvsp[0].etree); }
    break;

  case 212:
#line 820 "ldgram.y"
    { yyval.etree = exp_binop ('&' , yyvsp[-2].etree, yyvsp[0].etree); }
    break;

  case 213:
#line 822 "ldgram.y"
    { yyval.etree = exp_binop ('^' , yyvsp[-2].etree, yyvsp[0].etree); }
    break;

  case 214:
#line 824 "ldgram.y"
    { yyval.etree = exp_binop ('|' , yyvsp[-2].etree, yyvsp[0].etree); }
    break;

  case 215:
#line 826 "ldgram.y"
    { yyval.etree = exp_trinop ('?' , yyvsp[-4].etree, yyvsp[-2].etree, yyvsp[0].etree); }
    break;

  case 216:
#line 828 "ldgram.y"
    { yyval.etree = exp_binop (ANDAND , yyvsp[-2].etree, yyvsp[0].etree); }
    break;

  case 217:
#line 830 "ldgram.y"
    { yyval.etree = exp_binop (OROR , yyvsp[-2].etree, yyvsp[0].etree); }
    break;

  case 218:
#line 832 "ldgram.y"
    { yyval.etree = exp_nameop (DEFINED, yyvsp[-1].name); }
    break;

  case 219:
#line 834 "ldgram.y"
    { yyval.etree = exp_bigintop (yyvsp[0].bigint.integer, yyvsp[0].bigint.str); }
    break;

  case 220:
#line 836 "ldgram.y"
    { yyval.etree = exp_nameop (SIZEOF_HEADERS,0); }
    break;

  case 221:
#line 839 "ldgram.y"
    { yyval.etree = exp_nameop (SIZEOF,yyvsp[-1].name); }
    break;

  case 222:
#line 841 "ldgram.y"
    { yyval.etree = exp_nameop (ADDR,yyvsp[-1].name); }
    break;

  case 223:
#line 843 "ldgram.y"
    { yyval.etree = exp_nameop (LOADADDR,yyvsp[-1].name); }
    break;

  case 224:
#line 845 "ldgram.y"
    { yyval.etree = exp_unop (ABSOLUTE, yyvsp[-1].etree); }
    break;

  case 225:
#line 847 "ldgram.y"
    { yyval.etree = exp_unop (ALIGN_K,yyvsp[-1].etree); }
    break;

  case 226:
#line 849 "ldgram.y"
    { yyval.etree = exp_binop (ALIGN_K,yyvsp[-3].etree,yyvsp[-1].etree); }
    break;

  case 227:
#line 851 "ldgram.y"
    { yyval.etree = exp_binop (DATA_SEGMENT_ALIGN, yyvsp[-3].etree, yyvsp[-1].etree); }
    break;

  case 228:
#line 853 "ldgram.y"
    { yyval.etree = exp_binop (DATA_SEGMENT_RELRO_END, yyvsp[-1].etree, yyvsp[-3].etree); }
    break;

  case 229:
#line 855 "ldgram.y"
    { yyval.etree = exp_unop (DATA_SEGMENT_END, yyvsp[-1].etree); }
    break;

  case 230:
#line 857 "ldgram.y"
    { /* The operands to the expression node are
			     placed in the opposite order from the way
			     in which they appear in the script as
			     that allows us to reuse more code in
			     fold_binary.  */
			  yyval.etree = exp_binop (SEGMENT_START,
					  yyvsp[-1].etree,
					  exp_nameop (NAME, yyvsp[-3].name)); }
    break;

  case 231:
#line 866 "ldgram.y"
    { yyval.etree = exp_unop (ALIGN_K,yyvsp[-1].etree); }
    break;

  case 232:
#line 868 "ldgram.y"
    { yyval.etree = exp_nameop (NAME,yyvsp[0].name); }
    break;

  case 233:
#line 870 "ldgram.y"
    { yyval.etree = exp_binop (MAX_K, yyvsp[-3].etree, yyvsp[-1].etree ); }
    break;

  case 234:
#line 872 "ldgram.y"
    { yyval.etree = exp_binop (MIN_K, yyvsp[-3].etree, yyvsp[-1].etree ); }
    break;

  case 235:
#line 874 "ldgram.y"
    { yyval.etree = exp_assert (yyvsp[-3].etree, yyvsp[-1].name); }
    break;

  case 236:
#line 876 "ldgram.y"
    { yyval.etree = exp_nameop (ORIGIN, yyvsp[-1].name); }
    break;

  case 237:
#line 878 "ldgram.y"
    { yyval.etree = exp_nameop (LENGTH, yyvsp[-1].name); }
    break;

  case 238:
#line 883 "ldgram.y"
    { yyval.name = yyvsp[0].name; }
    break;

  case 239:
#line 884 "ldgram.y"
    { yyval.name = 0; }
    break;

  case 240:
#line 888 "ldgram.y"
    { yyval.etree = yyvsp[-1].etree; }
    break;

  case 241:
#line 889 "ldgram.y"
    { yyval.etree = 0; }
    break;

  case 242:
#line 893 "ldgram.y"
    { yyval.etree = yyvsp[-1].etree; }
    break;

  case 243:
#line 894 "ldgram.y"
    { yyval.etree = 0; }
    break;

  case 244:
#line 898 "ldgram.y"
    { yyval.etree = yyvsp[-1].etree; }
    break;

  case 245:
#line 899 "ldgram.y"
    { yyval.etree = 0; }
    break;

  case 246:
#line 903 "ldgram.y"
    { yyval.token = ONLY_IF_RO; }
    break;

  case 247:
#line 904 "ldgram.y"
    { yyval.token = ONLY_IF_RW; }
    break;

  case 248:
#line 905 "ldgram.y"
    { yyval.token = ONLY_IF_SPUGUID; }
    break;

  case 249:
#line 906 "ldgram.y"
    { yyval.token = SPECIAL; }
    break;

  case 250:
#line 907 "ldgram.y"
    { yyval.token = 0; }
    break;

  case 251:
#line 910 "ldgram.y"
    { ldlex_expression(); }
    break;

  case 252:
#line 914 "ldgram.y"
    { ldlex_popstate (); ldlex_script (); }
    break;

  case 253:
#line 917 "ldgram.y"
    {
			  lang_enter_output_section_statement(yyvsp[-8].name, yyvsp[-6].etree,
							      sectype,
							      yyvsp[-4].etree, yyvsp[-3].etree, yyvsp[-5].etree, yyvsp[-1].token);
			}
    break;

  case 254:
#line 923 "ldgram.y"
    { ldlex_popstate (); ldlex_expression (); }
    break;

  case 255:
#line 925 "ldgram.y"
    {
		  ldlex_popstate ();
		  lang_leave_output_section_statement (yyvsp[0].fill, yyvsp[-3].name, yyvsp[-1].section_phdr, yyvsp[-2].name);
		}
    break;

  case 256:
#line 930 "ldgram.y"
    {}
    break;

  case 257:
#line 932 "ldgram.y"
    { ldlex_expression (); }
    break;

  case 258:
#line 934 "ldgram.y"
    { ldlex_popstate (); ldlex_script (); }
    break;

  case 259:
#line 936 "ldgram.y"
    {
			  lang_enter_overlay (yyvsp[-5].etree, yyvsp[-2].etree);
			}
    break;

  case 260:
#line 941 "ldgram.y"
    { ldlex_popstate (); ldlex_expression (); }
    break;

  case 261:
#line 943 "ldgram.y"
    {
			  ldlex_popstate ();
			  lang_leave_overlay (yyvsp[-11].etree, (int) yyvsp[-12].integer,
					      yyvsp[0].fill, yyvsp[-3].name, yyvsp[-1].section_phdr, yyvsp[-2].name);
			}
    break;

  case 263:
#line 953 "ldgram.y"
    { ldlex_expression (); }
    break;

  case 264:
#line 955 "ldgram.y"
    {
		  ldlex_popstate ();
		  lang_add_assignment (exp_assop ('=', ".", yyvsp[0].etree));
		}
    break;

  case 266:
#line 963 "ldgram.y"
    { sectype = noload_section; }
    break;

  case 267:
#line 964 "ldgram.y"
    { sectype = dsect_section; }
    break;

  case 268:
#line 965 "ldgram.y"
    { sectype = copy_section; }
    break;

  case 269:
#line 966 "ldgram.y"
    { sectype = info_section; }
    break;

  case 270:
#line 967 "ldgram.y"
    { sectype = overlay_section; }
    break;

  case 272:
#line 972 "ldgram.y"
    { sectype = normal_section; }
    break;

  case 273:
#line 973 "ldgram.y"
    { sectype = normal_section; }
    break;

  case 274:
#line 977 "ldgram.y"
    { yyval.etree = yyvsp[-2].etree; }
    break;

  case 275:
#line 978 "ldgram.y"
    { yyval.etree = (etree_type *)NULL;  }
    break;

  case 276:
#line 983 "ldgram.y"
    { yyval.etree = yyvsp[-3].etree; }
    break;

  case 277:
#line 985 "ldgram.y"
    { yyval.etree = yyvsp[-7].etree; }
    break;

  case 278:
#line 989 "ldgram.y"
    { yyval.etree = yyvsp[-1].etree; }
    break;

  case 279:
#line 990 "ldgram.y"
    { yyval.etree = (etree_type *) NULL;  }
    break;

  case 280:
#line 995 "ldgram.y"
    { yyval.integer = 0; }
    break;

  case 281:
#line 997 "ldgram.y"
    { yyval.integer = 1; }
    break;

  case 282:
#line 1002 "ldgram.y"
    { yyval.name = yyvsp[0].name; }
    break;

  case 283:
#line 1003 "ldgram.y"
    { yyval.name = DEFAULT_MEMORY_REGION; }
    break;

  case 284:
#line 1008 "ldgram.y"
    {
		  yyval.section_phdr = NULL;
		}
    break;

  case 285:
#line 1012 "ldgram.y"
    {
		  struct lang_output_section_phdr_list *n;

		  n = ((struct lang_output_section_phdr_list *)
		       xmalloc (sizeof *n));
		  n->name = yyvsp[0].name;
		  n->used = FALSE;
		  n->next = yyvsp[-2].section_phdr;
		  yyval.section_phdr = n;
		}
    break;

  case 287:
#line 1028 "ldgram.y"
    {
			  ldlex_script ();
			  lang_enter_overlay_section (yyvsp[0].name);
			}
    break;

  case 288:
#line 1033 "ldgram.y"
    { ldlex_popstate (); ldlex_expression (); }
    break;

  case 289:
#line 1035 "ldgram.y"
    {
			  ldlex_popstate ();
			  lang_leave_overlay_section (yyvsp[0].fill, yyvsp[-1].section_phdr);
			}
    break;

  case 294:
#line 1052 "ldgram.y"
    { ldlex_expression (); }
    break;

  case 295:
#line 1053 "ldgram.y"
    { ldlex_popstate (); }
    break;

  case 296:
#line 1055 "ldgram.y"
    {
		  lang_new_phdr (yyvsp[-5].name, yyvsp[-3].etree, yyvsp[-2].phdr.filehdr, yyvsp[-2].phdr.phdrs, yyvsp[-2].phdr.at,
				 yyvsp[-2].phdr.flags);
		}
    break;

  case 297:
#line 1063 "ldgram.y"
    {
		  yyval.etree = yyvsp[0].etree;

		  if (yyvsp[0].etree->type.node_class == etree_name
		      && yyvsp[0].etree->type.node_code == NAME)
		    {
		      const char *s;
		      unsigned int i;
		      static const char * const phdr_types[] =
			{
			  "PT_NULL", "PT_LOAD", "PT_DYNAMIC",
			  "PT_INTERP", "PT_NOTE", "PT_SHLIB",
			  "PT_PHDR", "PT_TLS"
			};

		      s = yyvsp[0].etree->name.name;
		      for (i = 0;
			   i < sizeof phdr_types / sizeof phdr_types[0];
			   i++)
			if (strcmp (s, phdr_types[i]) == 0)
			  {
			    yyval.etree = exp_intop (i);
			    break;
			  }
#if (defined(BPA))
		      if (strcmp(s, "PT_SPU_INFO") == 0)
			yyval.etree = exp_intop(0x70000000);
#endif

		      if (i == sizeof phdr_types / sizeof phdr_types[0])
			{
			  if (strcmp (s, "PT_GNU_EH_FRAME") == 0)
			    yyval.etree = exp_intop (0x6474e550);
			  else if (strcmp (s, "PT_GNU_STACK") == 0)
			    yyval.etree = exp_intop (0x6474e551);
			  else
			    {
			      einfo (_("\
%X%P:%S: unknown phdr type `%s' (try integer literal)\n"),
				     s);
			      yyval.etree = exp_intop (0);
			    }
			}
		    }
		}
    break;

  case 298:
#line 1112 "ldgram.y"
    {
		  memset (&yyval.phdr, 0, sizeof (struct phdr_info));
		}
    break;

  case 299:
#line 1116 "ldgram.y"
    {
		  yyval.phdr = yyvsp[0].phdr;
		  if (strcmp (yyvsp[-2].name, "FILEHDR") == 0 && yyvsp[-1].etree == NULL)
		    yyval.phdr.filehdr = TRUE;
		  else if (strcmp (yyvsp[-2].name, "PHDRS") == 0 && yyvsp[-1].etree == NULL)
		    yyval.phdr.phdrs = TRUE;
		  else if (strcmp (yyvsp[-2].name, "FLAGS") == 0 && yyvsp[-1].etree != NULL)
		    yyval.phdr.flags = yyvsp[-1].etree;
		  else
		    einfo (_("%X%P:%S: PHDRS syntax error at `%s'\n"), yyvsp[-2].name);
		}
    break;

  case 300:
#line 1128 "ldgram.y"
    {
		  yyval.phdr = yyvsp[0].phdr;
		  yyval.phdr.at = yyvsp[-2].etree;
		}
    break;

  case 301:
#line 1136 "ldgram.y"
    {
		  yyval.etree = NULL;
		}
    break;

  case 302:
#line 1140 "ldgram.y"
    {
		  yyval.etree = yyvsp[-1].etree;
		}
    break;

  case 303:
#line 1148 "ldgram.y"
    {
		  ldlex_version_file ();
		  PUSH_ERROR (_("VERSION script"));
		}
    break;

  case 304:
#line 1153 "ldgram.y"
    {
		  ldlex_popstate ();
		  POP_ERROR ();
		}
    break;

  case 305:
#line 1162 "ldgram.y"
    {
		  ldlex_version_script ();
		}
    break;

  case 306:
#line 1166 "ldgram.y"
    {
		  ldlex_popstate ();
		}
    break;

  case 309:
#line 1178 "ldgram.y"
    {
		  lang_register_vers_node (NULL, yyvsp[-2].versnode, NULL);
		}
    break;

  case 310:
#line 1182 "ldgram.y"
    {
		  lang_register_vers_node (yyvsp[-4].name, yyvsp[-2].versnode, NULL);
		}
    break;

  case 311:
#line 1186 "ldgram.y"
    {
		  lang_register_vers_node (yyvsp[-5].name, yyvsp[-3].versnode, yyvsp[-1].deflist);
		}
    break;

  case 312:
#line 1193 "ldgram.y"
    {
		  yyval.deflist = lang_add_vers_depend (NULL, yyvsp[0].name);
		}
    break;

  case 313:
#line 1197 "ldgram.y"
    {
		  yyval.deflist = lang_add_vers_depend (yyvsp[-1].deflist, yyvsp[0].name);
		}
    break;

  case 314:
#line 1204 "ldgram.y"
    {
		  yyval.versnode = lang_new_vers_node (NULL, NULL);
		}
    break;

  case 315:
#line 1208 "ldgram.y"
    {
		  yyval.versnode = lang_new_vers_node (yyvsp[-1].versyms, NULL);
		}
    break;

  case 316:
#line 1212 "ldgram.y"
    {
		  yyval.versnode = lang_new_vers_node (yyvsp[-1].versyms, NULL);
		}
    break;

  case 317:
#line 1216 "ldgram.y"
    {
		  yyval.versnode = lang_new_vers_node (NULL, yyvsp[-1].versyms);
		}
    break;

  case 318:
#line 1220 "ldgram.y"
    {
		  yyval.versnode = lang_new_vers_node (yyvsp[-5].versyms, yyvsp[-1].versyms);
		}
    break;

  case 319:
#line 1227 "ldgram.y"
    {
		  yyval.versyms = lang_new_vers_pattern (NULL, yyvsp[0].name, ldgram_vers_current_lang, FALSE);
		}
    break;

  case 320:
#line 1231 "ldgram.y"
    {
		  yyval.versyms = lang_new_vers_pattern (NULL, yyvsp[0].name, ldgram_vers_current_lang, TRUE);
		}
    break;

  case 321:
#line 1235 "ldgram.y"
    {
		  yyval.versyms = lang_new_vers_pattern (yyvsp[-2].versyms, yyvsp[0].name, ldgram_vers_current_lang, FALSE);
		}
    break;

  case 322:
#line 1239 "ldgram.y"
    {
		  yyval.versyms = lang_new_vers_pattern (yyvsp[-2].versyms, yyvsp[0].name, ldgram_vers_current_lang, TRUE);
		}
    break;

  case 323:
#line 1243 "ldgram.y"
    {
			  yyval.name = ldgram_vers_current_lang;
			  ldgram_vers_current_lang = yyvsp[-1].name;
			}
    break;

  case 324:
#line 1248 "ldgram.y"
    {
			  struct bfd_elf_version_expr *pat;
			  for (pat = yyvsp[-2].versyms; pat->next != NULL; pat = pat->next);
			  pat->next = yyvsp[-8].versyms;
			  yyval.versyms = yyvsp[-2].versyms;
			  ldgram_vers_current_lang = yyvsp[-3].name;
			}
    break;

  case 325:
#line 1256 "ldgram.y"
    {
			  yyval.name = ldgram_vers_current_lang;
			  ldgram_vers_current_lang = yyvsp[-1].name;
			}
    break;

  case 326:
#line 1261 "ldgram.y"
    {
			  yyval.versyms = yyvsp[-2].versyms;
			  ldgram_vers_current_lang = yyvsp[-3].name;
			}
    break;

  case 327:
#line 1266 "ldgram.y"
    {
		  yyval.versyms = lang_new_vers_pattern (NULL, "global", ldgram_vers_current_lang, FALSE);
		}
    break;

  case 328:
#line 1270 "ldgram.y"
    {
		  yyval.versyms = lang_new_vers_pattern (yyvsp[-2].versyms, "global", ldgram_vers_current_lang, FALSE);
		}
    break;

  case 329:
#line 1274 "ldgram.y"
    {
		  yyval.versyms = lang_new_vers_pattern (NULL, "local", ldgram_vers_current_lang, FALSE);
		}
    break;

  case 330:
#line 1278 "ldgram.y"
    {
		  yyval.versyms = lang_new_vers_pattern (yyvsp[-2].versyms, "local", ldgram_vers_current_lang, FALSE);
		}
    break;

  case 331:
#line 1282 "ldgram.y"
    {
		  yyval.versyms = lang_new_vers_pattern (NULL, "extern", ldgram_vers_current_lang, FALSE);
		}
    break;

  case 332:
#line 1286 "ldgram.y"
    {
		  yyval.versyms = lang_new_vers_pattern (yyvsp[-2].versyms, "extern", ldgram_vers_current_lang, FALSE);
		}
    break;


    }

/* Line 1010 of yacc.c.  */
#line 3770 "ldgram.c"

  yyvsp -= yylen;
  yyssp -= yylen;


  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  int yytype = YYTRANSLATE (yychar);
	  const char* yyprefix;
	  char *yymsg;
	  int yyx;

	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  int yyxbegin = yyn < 0 ? -yyn : 0;

	  /* Stay within bounds of both yycheck and yytname.  */
	  int yychecklim = YYLAST - yyn;
	  int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
	  int yycount = 0;

	  yyprefix = ", expecting ";
	  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      {
		yysize += yystrlen (yyprefix) + yystrlen (yytname [yyx]);
		yycount += 1;
		if (yycount == 5)
		  {
		    yysize = 0;
		    break;
		  }
	      }
	  yysize += (sizeof ("syntax error, unexpected ")
		     + yystrlen (yytname[yytype]));
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "syntax error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[yytype]);

	      if (yycount < 5)
		{
		  yyprefix = ", expecting ";
		  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
		    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
		      {
			yyp = yystpcpy (yyp, yyprefix);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yyprefix = " or ";
		      }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror ("syntax error; also virtual memory exhausted");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror ("syntax error");
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* If at end of input, pop the error token,
	     then the rest of the stack, then return failure.  */
	  if (yychar == YYEOF)
	     for (;;)
	       {
		 YYPOPSTACK;
		 if (yyssp == yyss)
		   YYABORT;
		 YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
		 yydestruct (yystos[*yyssp], yyvsp);
	       }
        }
      else
	{
	  YYDSYMPRINTF ("Error: discarding", yytoken, &yylval, &yylloc);
	  yydestruct (yytoken, &yylval);
	  yychar = YYEMPTY;

	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

#ifdef __GNUC__
  /* Pacify GCC when the user code never invokes YYERROR and the label
     yyerrorlab therefore never appears in user code.  */
  if (0)
     goto yyerrorlab;
#endif

  yyvsp -= yylen;
  yyssp -= yylen;
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;

      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
      yydestruct (yystos[yystate], yyvsp);
      YYPOPSTACK;
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;


  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*----------------------------------------------.
| yyoverflowlab -- parser overflow comes here.  |
`----------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}


#line 1296 "ldgram.y"

void
yyerror(arg)
     const char *arg;
{
  if (ldfile_assumed_script)
    einfo (_("%P:%s: file format not recognized; treating as linker script\n"),
	   ldfile_input_filename);
  if (error_index > 0 && error_index < ERROR_NAME_MAX)
     einfo ("%P%F:%S: %s in %s\n", arg, error_names[error_index-1]);
  else
     einfo ("%P%F:%S: %s\n", arg);
}

