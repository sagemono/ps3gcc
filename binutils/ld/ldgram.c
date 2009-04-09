/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

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
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

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
     INSERT_K = 289,
     AFTER = 290,
     BEFORE = 291,
     DATA_SEGMENT_ALIGN = 292,
     DATA_SEGMENT_RELRO_END = 293,
     DATA_SEGMENT_END = 294,
     SORT_BY_NAME = 295,
     SORT_BY_ALIGNMENT = 296,
     SIZEOF_HEADERS = 297,
     OUTPUT_FORMAT = 298,
     FORCE_COMMON_ALLOCATION = 299,
     OUTPUT_ARCH = 300,
     INHIBIT_COMMON_ALLOCATION = 301,
     SEGMENT_START = 302,
     INCLUDE = 303,
     MEMORY = 304,
     DEFSYMEND = 305,
     NOLOAD = 306,
     DSECT = 307,
     COPY = 308,
     INFO = 309,
     OVERLAY = 310,
     DEFINED = 311,
     TARGET_K = 312,
     SEARCH_DIR = 313,
     MAP = 314,
     ENTRY = 315,
     NEXT = 316,
     SIZEOF = 317,
     ADDR = 318,
     LOADADDR = 319,
     MAX_K = 320,
     MIN_K = 321,
     STARTUP = 322,
     HLL = 323,
     SYSLIB = 324,
     FLOAT = 325,
     NOFLOAT = 326,
     NOCROSSREFS = 327,
     ORIGIN = 328,
     FILL = 329,
     LENGTH = 330,
     CREATE_OBJECT_SYMBOLS = 331,
     INPUT = 332,
     GROUP = 333,
     OUTPUT = 334,
     CONSTRUCTORS = 335,
     ALIGNMOD = 336,
     AT = 337,
     SUBALIGN = 338,
     PROVIDE = 339,
     PROVIDE_HIDDEN = 340,
     AS_NEEDED = 341,
     CHIP = 342,
     LIST = 343,
     SECT = 344,
     ABSOLUTE = 345,
     LOAD = 346,
     NEWLINE = 347,
     ENDWORD = 348,
     ORDER = 349,
     NAMEWORD = 350,
     ASSERT_K = 351,
     FORMAT = 352,
     PUBLIC = 353,
     BASE = 354,
     ALIAS = 355,
     TRUNCATE = 356,
     REL = 357,
     INPUT_SCRIPT = 358,
     INPUT_MRI_SCRIPT = 359,
     INPUT_DEFSYM = 360,
     CASE = 361,
     EXTERN = 362,
     START = 363,
     VERS_TAG = 364,
     VERS_IDENTIFIER = 365,
     GLOBAL = 366,
     LOCAL = 367,
     VERSIONK = 368,
     INPUT_VERSION_SCRIPT = 369,
     KEEP = 370,
     ONLY_IF_RO = 371,
     ONLY_IF_RW = 372,
     SPECIAL = 373,
     ONLY_IF_SPUGUID = 374,
     EXCLUDE_FILE = 375
   };
#endif
/* Tokens.  */
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
#define INSERT_K 289
#define AFTER 290
#define BEFORE 291
#define DATA_SEGMENT_ALIGN 292
#define DATA_SEGMENT_RELRO_END 293
#define DATA_SEGMENT_END 294
#define SORT_BY_NAME 295
#define SORT_BY_ALIGNMENT 296
#define SIZEOF_HEADERS 297
#define OUTPUT_FORMAT 298
#define FORCE_COMMON_ALLOCATION 299
#define OUTPUT_ARCH 300
#define INHIBIT_COMMON_ALLOCATION 301
#define SEGMENT_START 302
#define INCLUDE 303
#define MEMORY 304
#define DEFSYMEND 305
#define NOLOAD 306
#define DSECT 307
#define COPY 308
#define INFO 309
#define OVERLAY 310
#define DEFINED 311
#define TARGET_K 312
#define SEARCH_DIR 313
#define MAP 314
#define ENTRY 315
#define NEXT 316
#define SIZEOF 317
#define ADDR 318
#define LOADADDR 319
#define MAX_K 320
#define MIN_K 321
#define STARTUP 322
#define HLL 323
#define SYSLIB 324
#define FLOAT 325
#define NOFLOAT 326
#define NOCROSSREFS 327
#define ORIGIN 328
#define FILL 329
#define LENGTH 330
#define CREATE_OBJECT_SYMBOLS 331
#define INPUT 332
#define GROUP 333
#define OUTPUT 334
#define CONSTRUCTORS 335
#define ALIGNMOD 336
#define AT 337
#define SUBALIGN 338
#define PROVIDE 339
#define PROVIDE_HIDDEN 340
#define AS_NEEDED 341
#define CHIP 342
#define LIST 343
#define SECT 344
#define ABSOLUTE 345
#define LOAD 346
#define NEWLINE 347
#define ENDWORD 348
#define ORDER 349
#define NAMEWORD 350
#define ASSERT_K 351
#define FORMAT 352
#define PUBLIC 353
#define BASE 354
#define ALIAS 355
#define TRUNCATE 356
#define REL 357
#define INPUT_SCRIPT 358
#define INPUT_MRI_SCRIPT 359
#define INPUT_DEFSYM 360
#define CASE 361
#define EXTERN 362
#define START 363
#define VERS_TAG 364
#define VERS_IDENTIFIER 365
#define GLOBAL 366
#define LOCAL 367
#define VERSIONK 368
#define INPUT_VERSION_SCRIPT 369
#define KEEP 370
#define ONLY_IF_RO 371
#define ONLY_IF_RW 372
#define SPECIAL 373
#define ONLY_IF_SPUGUID 374
#define EXCLUDE_FILE 375




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

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 60 "ldgram.y"
{
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
}
/* Line 187 of yacc.c.  */
#line 404 "ldgram.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 417 "ldgram.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
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
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  14
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1733

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  144
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  114
/* YYNRULES -- Number of rules.  */
#define YYNRULES  336
/* YYNRULES -- Number of states.  */
#define YYNSTATES  713

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   375

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   142,     2,     2,     2,    34,    21,     2,
      37,   139,    32,    30,   137,    31,     2,    33,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    16,   138,
      24,     6,    25,    15,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   140,     2,   141,    20,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    56,    19,    57,   143,     2,     2,     2,
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
      49,    50,    51,    52,    53,    54,    55,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     6,     9,    12,    15,    17,    18,    23,
      24,    27,    31,    32,    35,    40,    42,    44,    47,    49,
      54,    59,    63,    66,    71,    75,    80,    85,    90,    95,
     100,   103,   106,   109,   114,   119,   122,   125,   128,   131,
     132,   138,   141,   142,   146,   149,   150,   152,   156,   158,
     162,   163,   165,   169,   171,   174,   178,   179,   182,   185,
     186,   188,   190,   192,   194,   196,   198,   200,   202,   204,
     206,   211,   216,   221,   226,   235,   240,   242,   244,   249,
     250,   256,   261,   262,   268,   273,   278,   282,   286,   288,
     292,   295,   297,   301,   304,   305,   311,   312,   320,   321,
     328,   333,   336,   339,   340,   345,   348,   349,   357,   359,
     361,   363,   365,   371,   376,   381,   389,   397,   405,   413,
     422,   425,   427,   431,   433,   435,   439,   444,   446,   447,
     453,   456,   458,   460,   462,   467,   469,   474,   479,   482,
     484,   485,   487,   489,   491,   493,   495,   497,   499,   502,
     503,   505,   507,   509,   511,   513,   515,   517,   519,   521,
     523,   527,   531,   538,   545,   547,   548,   554,   557,   561,
     562,   563,   571,   575,   579,   580,   584,   586,   589,   591,
     594,   599,   604,   608,   612,   614,   619,   623,   624,   626,
     628,   629,   632,   636,   637,   640,   643,   647,   652,   655,
     658,   661,   665,   669,   673,   677,   681,   685,   689,   693,
     697,   701,   705,   709,   713,   717,   721,   725,   731,   735,
     739,   744,   746,   748,   753,   758,   763,   768,   773,   780,
     787,   794,   799,   806,   811,   813,   820,   827,   834,   839,
     844,   848,   849,   854,   855,   860,   861,   866,   867,   869,
     871,   873,   875,   876,   877,   878,   879,   880,   881,   901,
     902,   903,   904,   905,   906,   925,   926,   927,   935,   937,
     939,   941,   943,   945,   949,   950,   953,   957,   960,   967,
     978,   981,   983,   984,   986,   989,   990,   991,   995,   996,
     997,   998,   999,  1011,  1016,  1017,  1020,  1021,  1022,  1029,
    1031,  1032,  1036,  1042,  1043,  1047,  1048,  1051,  1052,  1058,
    1060,  1063,  1068,  1074,  1081,  1083,  1086,  1087,  1090,  1095,
    1100,  1109,  1111,  1113,  1117,  1121,  1122,  1132,  1133,  1141,
    1143,  1147,  1149,  1153,  1155,  1159,  1160
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     145,     0,    -1,   119,   159,    -1,   120,   149,    -1,   130,
     246,    -1,   121,   147,    -1,     4,    -1,    -1,   148,     4,
       6,   208,    -1,    -1,   150,   151,    -1,   151,   152,   108,
      -1,    -1,   103,   208,    -1,   103,   208,   137,   208,    -1,
       4,    -1,   104,    -1,   110,   154,    -1,   109,    -1,   114,
       4,     6,   208,    -1,   114,     4,   137,   208,    -1,   114,
       4,   208,    -1,   113,     4,    -1,   105,     4,   137,   208,
      -1,   105,     4,   208,    -1,   105,     4,     6,   208,    -1,
      38,     4,     6,   208,    -1,    38,     4,   137,   208,    -1,
      97,     4,     6,   208,    -1,    97,     4,   137,   208,    -1,
     106,   156,    -1,   107,   155,    -1,   111,     4,    -1,   116,
       4,   137,     4,    -1,   116,     4,   137,     3,    -1,   115,
     208,    -1,   117,     3,    -1,   122,   157,    -1,   123,   158,
      -1,    -1,    64,   146,   153,   151,    36,    -1,   124,     4,
      -1,    -1,   154,   137,     4,    -1,   154,     4,    -1,    -1,
       4,    -1,   155,   137,     4,    -1,     4,    -1,   156,   137,
       4,    -1,    -1,     4,    -1,   157,   137,     4,    -1,     4,
      -1,   158,     4,    -1,   158,   137,     4,    -1,    -1,   160,
     161,    -1,   161,   162,    -1,    -1,   190,    -1,   169,    -1,
     238,    -1,   199,    -1,   200,    -1,   202,    -1,   204,    -1,
     171,    -1,   248,    -1,   138,    -1,    73,    37,     4,   139,
      -1,    74,    37,   146,   139,    -1,    95,    37,   146,   139,
      -1,    59,    37,     4,   139,    -1,    59,    37,     4,   137,
       4,   137,     4,   139,    -1,    61,    37,     4,   139,    -1,
      60,    -1,    62,    -1,    93,    37,   165,   139,    -1,    -1,
      94,   163,    37,   165,   139,    -1,    75,    37,   146,   139,
      -1,    -1,    64,   146,   164,   161,    36,    -1,    88,    37,
     205,   139,    -1,   123,    37,   158,   139,    -1,    48,    49,
       4,    -1,    48,    50,     4,    -1,     4,    -1,   165,   137,
       4,    -1,   165,     4,    -1,     5,    -1,   165,   137,     5,
      -1,   165,     5,    -1,    -1,   102,    37,   166,   165,   139,
      -1,    -1,   165,   137,   102,    37,   167,   165,   139,    -1,
      -1,   165,   102,    37,   168,   165,   139,    -1,    46,    56,
     170,    57,    -1,   170,   214,    -1,   170,   171,    -1,    -1,
      76,    37,     4,   139,    -1,   188,   187,    -1,    -1,   112,
     172,    37,   208,   137,     4,   139,    -1,     4,    -1,    32,
      -1,    15,    -1,   173,    -1,   136,    37,   175,   139,   173,
      -1,    54,    37,   173,   139,    -1,    55,    37,   173,   139,
      -1,    54,    37,    55,    37,   173,   139,   139,    -1,    54,
      37,    54,    37,   173,   139,   139,    -1,    55,    37,    54,
      37,   173,   139,   139,    -1,    55,    37,    55,    37,   173,
     139,   139,    -1,    54,    37,   136,    37,   175,   139,   173,
     139,    -1,   175,   173,    -1,   173,    -1,   176,   189,   174,
      -1,   174,    -1,     4,    -1,   140,   176,   141,    -1,   174,
      37,   176,   139,    -1,   177,    -1,    -1,   131,    37,   179,
     177,   139,    -1,   188,   187,    -1,    92,    -1,   138,    -1,
      96,    -1,    54,    37,    96,   139,    -1,   178,    -1,   183,
      37,   206,   139,    -1,    90,    37,   184,   139,    -1,   181,
     180,    -1,   180,    -1,    -1,   181,    -1,    41,    -1,    42,
      -1,    43,    -1,    44,    -1,    45,    -1,   206,    -1,     6,
     184,    -1,    -1,    14,    -1,    13,    -1,    12,    -1,    11,
      -1,    10,    -1,     9,    -1,     8,    -1,     7,    -1,   138,
      -1,   137,    -1,     4,     6,   206,    -1,     4,   186,   206,
      -1,   100,    37,     4,     6,   206,   139,    -1,   101,    37,
       4,     6,   206,   139,    -1,   137,    -1,    -1,    65,    56,
     192,   191,    57,    -1,   191,   192,    -1,   191,   137,   192,
      -1,    -1,    -1,     4,   193,   196,    16,   194,   189,   195,
      -1,    89,     6,   206,    -1,    91,     6,   206,    -1,    -1,
      37,   197,   139,    -1,   198,    -1,   197,   198,    -1,     4,
      -1,   142,     4,    -1,    83,    37,   146,   139,    -1,    84,
      37,   201,   139,    -1,    84,    37,   139,    -1,   201,   189,
     146,    -1,   146,    -1,    85,    37,   203,   139,    -1,   203,
     189,   146,    -1,    -1,    86,    -1,    87,    -1,    -1,     4,
     205,    -1,     4,   137,   205,    -1,    -1,   207,   208,    -1,
      31,   208,    -1,    37,   208,   139,    -1,    77,    37,   208,
     139,    -1,   142,   208,    -1,    30,   208,    -1,   143,   208,
      -1,   208,    32,   208,    -1,   208,    33,   208,    -1,   208,
      34,   208,    -1,   208,    30,   208,    -1,   208,    31,   208,
      -1,   208,    29,   208,    -1,   208,    28,   208,    -1,   208,
      23,   208,    -1,   208,    22,   208,    -1,   208,    27,   208,
      -1,   208,    26,   208,    -1,   208,    24,   208,    -1,   208,
      25,   208,    -1,   208,    21,   208,    -1,   208,    20,   208,
      -1,   208,    19,   208,    -1,   208,    15,   208,    16,   208,
      -1,   208,    18,   208,    -1,   208,    17,   208,    -1,    72,
      37,     4,   139,    -1,     3,    -1,    58,    -1,    78,    37,
       4,   139,    -1,    79,    37,     4,   139,    -1,    80,    37,
       4,   139,    -1,   106,    37,   208,   139,    -1,    38,    37,
     208,   139,    -1,    38,    37,   208,   137,   208,   139,    -1,
      51,    37,   208,   137,   208,   139,    -1,    52,    37,   208,
     137,   208,   139,    -1,    53,    37,   208,   139,    -1,    63,
      37,     4,   137,   208,   139,    -1,    39,    37,   208,   139,
      -1,     4,    -1,    81,    37,   208,   137,   208,   139,    -1,
      82,    37,   208,   137,   208,   139,    -1,   112,    37,   208,
     137,     4,   139,    -1,    89,    37,     4,   139,    -1,    91,
      37,     4,   139,    -1,    98,    25,     4,    -1,    -1,    98,
      37,   208,   139,    -1,    -1,    38,    37,   208,   139,    -1,
      -1,    99,    37,   208,   139,    -1,    -1,   132,    -1,   133,
      -1,   135,    -1,   134,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     4,   215,   229,   210,   211,   212,   216,   213,    56,
     217,   182,    57,   218,   232,   209,   233,   185,   219,   189,
      -1,    -1,    -1,    -1,    -1,    -1,    71,   220,   230,   231,
     210,   212,   221,    56,   222,   234,    57,   223,   232,   209,
     233,   185,   224,   189,    -1,    -1,    -1,    94,   225,   229,
     226,    56,   170,    57,    -1,    67,    -1,    68,    -1,    69,
      -1,    70,    -1,    71,    -1,    37,   227,   139,    -1,    -1,
      37,   139,    -1,   208,   228,    16,    -1,   228,    16,    -1,
      40,    37,   208,   139,   228,    16,    -1,    40,    37,   208,
     139,    39,    37,   208,   139,   228,    16,    -1,   208,    16,
      -1,    16,    -1,    -1,    88,    -1,    25,     4,    -1,    -1,
      -1,   233,    16,     4,    -1,    -1,    -1,    -1,    -1,   234,
       4,   235,    56,   182,    57,   236,   233,   185,   237,   189,
      -1,    47,    56,   239,    57,    -1,    -1,   239,   240,    -1,
      -1,    -1,     4,   241,   243,   244,   242,   138,    -1,   208,
      -1,    -1,     4,   245,   244,    -1,    98,    37,   208,   139,
     244,    -1,    -1,    37,   208,   139,    -1,    -1,   247,   250,
      -1,    -1,   249,   129,    56,   250,    57,    -1,   251,    -1,
     250,   251,    -1,    56,   253,    57,   138,    -1,   125,    56,
     253,    57,   138,    -1,   125,    56,   253,    57,   252,   138,
      -1,   125,    -1,   252,   125,    -1,    -1,   254,   138,    -1,
     127,    16,   254,   138,    -1,   128,    16,   254,   138,    -1,
     127,    16,   254,   138,   128,    16,   254,   138,    -1,   126,
      -1,     4,    -1,   254,   138,   126,    -1,   254,   138,     4,
      -1,    -1,   254,   138,   123,     4,    56,   255,   254,   257,
      57,    -1,    -1,   123,     4,    56,   256,   254,   257,    57,
      -1,   127,    -1,   254,   138,   127,    -1,   128,    -1,   254,
     138,   128,    -1,   123,    -1,   254,   138,   123,    -1,    -1,
     138,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   158,   158,   159,   160,   161,   165,   169,   169,   179,
     179,   192,   193,   197,   198,   199,   202,   205,   206,   207,
     209,   211,   213,   215,   217,   219,   221,   223,   225,   227,
     229,   230,   231,   233,   235,   237,   239,   241,   242,   244,
     243,   247,   249,   253,   254,   255,   259,   261,   265,   267,
     272,   273,   274,   278,   280,   282,   287,   287,   298,   299,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   317,   319,   321,   324,   326,   328,   330,   332,   334,
     333,   337,   340,   339,   343,   347,   348,   350,   355,   358,
     361,   364,   367,   370,   374,   373,   378,   377,   382,   381,
     388,   392,   393,   394,   398,   400,   401,   401,   409,   413,
     417,   424,   430,   436,   442,   448,   454,   460,   466,   472,
     481,   490,   501,   510,   521,   529,   533,   540,   542,   541,
     548,   549,   553,   554,   559,   564,   565,   570,   577,   578,
     581,   583,   587,   589,   591,   593,   595,   600,   607,   609,
     613,   615,   617,   619,   621,   623,   625,   627,   632,   632,
     637,   641,   649,   653,   661,   661,   665,   669,   670,   671,
     676,   675,   683,   691,   699,   700,   704,   705,   709,   711,
     716,   721,   722,   727,   729,   735,   737,   739,   743,   745,
     751,   754,   763,   774,   774,   780,   782,   784,   786,   788,
     790,   793,   795,   797,   799,   801,   803,   805,   807,   809,
     811,   813,   815,   817,   819,   821,   823,   825,   827,   829,
     831,   833,   835,   838,   840,   842,   844,   846,   848,   850,
     852,   854,   856,   865,   867,   869,   871,   873,   875,   877,
     883,   884,   888,   889,   893,   894,   898,   899,   903,   904,
     905,   906,   907,   910,   914,   917,   923,   925,   910,   932,
     934,   936,   941,   943,   931,   953,   955,   953,   963,   964,
     965,   966,   967,   971,   972,   973,   977,   978,   983,   984,
     989,   990,   995,   996,  1001,  1003,  1008,  1011,  1024,  1028,
    1033,  1035,  1026,  1043,  1046,  1048,  1052,  1053,  1052,  1062,
    1112,  1115,  1127,  1136,  1139,  1148,  1148,  1162,  1162,  1172,
    1173,  1177,  1181,  1185,  1192,  1196,  1204,  1207,  1211,  1215,
    1219,  1226,  1230,  1234,  1238,  1243,  1242,  1256,  1255,  1265,
    1269,  1273,  1277,  1281,  1285,  1291,  1293
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INT", "NAME", "LNAME", "'='", "OREQ",
  "ANDEQ", "RSHIFTEQ", "LSHIFTEQ", "DIVEQ", "MULTEQ", "MINUSEQ", "PLUSEQ",
  "'?'", "':'", "OROR", "ANDAND", "'|'", "'^'", "'&'", "NE", "EQ", "'<'",
  "'>'", "GE", "LE", "RSHIFT", "LSHIFT", "'+'", "'-'", "'*'", "'/'", "'%'",
  "UNARY", "END", "'('", "ALIGN_K", "BLOCK", "BIND", "QUAD", "SQUAD",
  "LONG", "SHORT", "BYTE", "SECTIONS", "PHDRS", "INSERT_K", "AFTER",
  "BEFORE", "DATA_SEGMENT_ALIGN", "DATA_SEGMENT_RELRO_END",
  "DATA_SEGMENT_END", "SORT_BY_NAME", "SORT_BY_ALIGNMENT", "'{'", "'}'",
  "SIZEOF_HEADERS", "OUTPUT_FORMAT", "FORCE_COMMON_ALLOCATION",
  "OUTPUT_ARCH", "INHIBIT_COMMON_ALLOCATION", "SEGMENT_START", "INCLUDE",
  "MEMORY", "DEFSYMEND", "NOLOAD", "DSECT", "COPY", "INFO", "OVERLAY",
  "DEFINED", "TARGET_K", "SEARCH_DIR", "MAP", "ENTRY", "NEXT", "SIZEOF",
  "ADDR", "LOADADDR", "MAX_K", "MIN_K", "STARTUP", "HLL", "SYSLIB",
  "FLOAT", "NOFLOAT", "NOCROSSREFS", "ORIGIN", "FILL", "LENGTH",
  "CREATE_OBJECT_SYMBOLS", "INPUT", "GROUP", "OUTPUT", "CONSTRUCTORS",
  "ALIGNMOD", "AT", "SUBALIGN", "PROVIDE", "PROVIDE_HIDDEN", "AS_NEEDED",
  "CHIP", "LIST", "SECT", "ABSOLUTE", "LOAD", "NEWLINE", "ENDWORD",
  "ORDER", "NAMEWORD", "ASSERT_K", "FORMAT", "PUBLIC", "BASE", "ALIAS",
  "TRUNCATE", "REL", "INPUT_SCRIPT", "INPUT_MRI_SCRIPT", "INPUT_DEFSYM",
  "CASE", "EXTERN", "START", "VERS_TAG", "VERS_IDENTIFIER", "GLOBAL",
  "LOCAL", "VERSIONK", "INPUT_VERSION_SCRIPT", "KEEP", "ONLY_IF_RO",
  "ONLY_IF_RW", "SPECIAL", "ONLY_IF_SPUGUID", "EXCLUDE_FILE", "','", "';'",
  "')'", "'['", "']'", "'!'", "'~'", "$accept", "file", "filename",
  "defsym_expr", "@1", "mri_script_file", "@2", "mri_script_lines",
  "mri_script_command", "@3", "ordernamelist", "mri_load_name_list",
  "mri_abs_name_list", "casesymlist", "extern_name_list", "script_file",
  "@4", "ifile_list", "ifile_p1", "@5", "@6", "input_list", "@7", "@8",
  "@9", "sections", "sec_or_group_p1", "statement_anywhere", "@10",
  "wildcard_name", "wildcard_spec", "exclude_name_list", "file_NAME_list",
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
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,    61,   261,   262,   263,
     264,   265,   266,   267,   268,    63,    58,   269,   270,   124,
      94,    38,   271,   272,    60,    62,   273,   274,   275,   276,
      43,    45,    42,    47,    37,   277,   278,    40,   279,   280,
     281,   282,   283,   284,   285,   286,   287,   288,   289,   290,
     291,   292,   293,   294,   295,   296,   123,   125,   297,   298,
     299,   300,   301,   302,   303,   304,   305,   306,   307,   308,
     309,   310,   311,   312,   313,   314,   315,   316,   317,   318,
     319,   320,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   330,   331,   332,   333,   334,   335,   336,   337,   338,
     339,   340,   341,   342,   343,   344,   345,   346,   347,   348,
     349,   350,   351,   352,   353,   354,   355,   356,   357,   358,
     359,   360,   361,   362,   363,   364,   365,   366,   367,   368,
     369,   370,   371,   372,   373,   374,   375,    44,    59,    41,
      91,    93,    33,   126
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   144,   145,   145,   145,   145,   146,   148,   147,   150,
     149,   151,   151,   152,   152,   152,   152,   152,   152,   152,
     152,   152,   152,   152,   152,   152,   152,   152,   152,   152,
     152,   152,   152,   152,   152,   152,   152,   152,   152,   153,
     152,   152,   152,   154,   154,   154,   155,   155,   156,   156,
     157,   157,   157,   158,   158,   158,   160,   159,   161,   161,
     162,   162,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   162,   163,
     162,   162,   164,   162,   162,   162,   162,   162,   165,   165,
     165,   165,   165,   165,   166,   165,   167,   165,   168,   165,
     169,   170,   170,   170,   171,   171,   172,   171,   173,   173,
     173,   174,   174,   174,   174,   174,   174,   174,   174,   174,
     175,   175,   176,   176,   177,   177,   177,   178,   179,   178,
     180,   180,   180,   180,   180,   180,   180,   180,   181,   181,
     182,   182,   183,   183,   183,   183,   183,   184,   185,   185,
     186,   186,   186,   186,   186,   186,   186,   186,   187,   187,
     188,   188,   188,   188,   189,   189,   190,   191,   191,   191,
     193,   192,   194,   195,   196,   196,   197,   197,   198,   198,
     199,   200,   200,   201,   201,   202,   203,   203,   204,   204,
     205,   205,   205,   207,   206,   208,   208,   208,   208,   208,
     208,   208,   208,   208,   208,   208,   208,   208,   208,   208,
     208,   208,   208,   208,   208,   208,   208,   208,   208,   208,
     208,   208,   208,   208,   208,   208,   208,   208,   208,   208,
     208,   208,   208,   208,   208,   208,   208,   208,   208,   208,
     209,   209,   210,   210,   211,   211,   212,   212,   213,   213,
     213,   213,   213,   215,   216,   217,   218,   219,   214,   220,
     221,   222,   223,   224,   214,   225,   226,   214,   227,   227,
     227,   227,   227,   228,   228,   228,   229,   229,   229,   229,
     230,   230,   231,   231,   232,   232,   233,   233,   234,   235,
     236,   237,   234,   238,   239,   239,   241,   242,   240,   243,
     244,   244,   244,   245,   245,   247,   246,   249,   248,   250,
     250,   251,   251,   251,   252,   252,   253,   253,   253,   253,
     253,   254,   254,   254,   254,   255,   254,   256,   254,   254,
     254,   254,   254,   254,   254,   257,   257
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     2,     2,     2,     1,     0,     4,     0,
       2,     3,     0,     2,     4,     1,     1,     2,     1,     4,
       4,     3,     2,     4,     3,     4,     4,     4,     4,     4,
       2,     2,     2,     4,     4,     2,     2,     2,     2,     0,
       5,     2,     0,     3,     2,     0,     1,     3,     1,     3,
       0,     1,     3,     1,     2,     3,     0,     2,     2,     0,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       4,     4,     4,     4,     8,     4,     1,     1,     4,     0,
       5,     4,     0,     5,     4,     4,     3,     3,     1,     3,
       2,     1,     3,     2,     0,     5,     0,     7,     0,     6,
       4,     2,     2,     0,     4,     2,     0,     7,     1,     1,
       1,     1,     5,     4,     4,     7,     7,     7,     7,     8,
       2,     1,     3,     1,     1,     3,     4,     1,     0,     5,
       2,     1,     1,     1,     4,     1,     4,     4,     2,     1,
       0,     1,     1,     1,     1,     1,     1,     1,     2,     0,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     3,     6,     6,     1,     0,     5,     2,     3,     0,
       0,     7,     3,     3,     0,     3,     1,     2,     1,     2,
       4,     4,     3,     3,     1,     4,     3,     0,     1,     1,
       0,     2,     3,     0,     2,     2,     3,     4,     2,     2,
       2,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     5,     3,     3,
       4,     1,     1,     4,     4,     4,     4,     4,     6,     6,
       6,     4,     6,     4,     1,     6,     6,     6,     4,     4,
       3,     0,     4,     0,     4,     0,     4,     0,     1,     1,
       1,     1,     0,     0,     0,     0,     0,     0,    19,     0,
       0,     0,     0,     0,    18,     0,     0,     7,     1,     1,
       1,     1,     1,     3,     0,     2,     3,     2,     6,    10,
       2,     1,     0,     1,     2,     0,     0,     3,     0,     0,
       0,     0,    11,     4,     0,     2,     0,     0,     6,     1,
       0,     3,     5,     0,     3,     0,     2,     0,     5,     1,
       2,     4,     5,     6,     1,     2,     0,     2,     4,     4,
       8,     1,     1,     3,     3,     0,     9,     0,     7,     1,
       3,     1,     3,     1,     3,     0,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       0,    56,     9,     7,   305,     0,     2,    59,     3,    12,
       5,     0,     4,     0,     1,    57,    10,     0,   316,     0,
     306,   309,     0,     0,     0,     0,     0,    76,     0,    77,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   188,
     189,     0,     0,    79,     0,     0,     0,   106,     0,    69,
      58,    61,    67,     0,    60,    63,    64,    65,    66,    62,
      68,     0,    15,     0,     0,     0,     0,    16,     0,     0,
       0,    18,    45,     0,     0,     0,     0,     0,     0,    50,
       0,     0,     0,     0,   322,   333,   321,   329,   331,     0,
       0,   316,   310,   193,   157,   156,   155,   154,   153,   152,
     151,   150,   193,   103,   294,     0,     0,     0,     0,     6,
      82,     0,     0,     0,     0,     0,     0,     0,   187,   190,
       0,     0,     0,     0,     0,     0,     0,   159,   158,   105,
       0,     0,    39,     0,   221,   234,     0,     0,     0,     0,
       0,     0,     0,     0,   222,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    13,
       0,    48,    30,    46,    31,    17,    32,    22,     0,    35,
       0,    36,    51,    37,    53,    38,    41,    11,     8,     0,
       0,     0,     0,   317,     0,   160,     0,   161,     0,     0,
      86,    87,     0,     0,    59,   170,   169,     0,     0,     0,
       0,     0,   182,   184,   165,   165,   190,     0,    88,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    12,     0,     0,   199,   195,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   198,   200,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    24,     0,
       0,    44,     0,     0,     0,    21,     0,     0,    54,     0,
     327,   329,   331,     0,     0,   311,   324,   334,   323,   330,
     332,     0,   194,   253,   100,   259,   265,   102,   101,   296,
     293,   295,     0,    73,    75,   307,   174,     0,    70,    71,
      81,   104,   180,   164,   181,     0,   185,     0,   190,   191,
      84,    94,    90,    93,     0,     0,    78,     0,    72,   193,
     193,     0,    85,     0,    26,    27,    42,    28,    29,   196,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   219,   218,
     216,   215,   214,   209,   208,   212,   213,   211,   210,   207,
     206,   204,   205,   201,   202,   203,    14,    25,    23,    49,
      47,    43,    19,    20,    34,    33,    52,    55,     0,   318,
     319,     0,   314,   312,     0,   274,     0,   274,     0,     0,
      83,     0,     0,   166,     0,   167,   183,   186,   192,     0,
      98,    89,    92,     0,    80,     0,     0,     0,   308,    40,
       0,   227,   233,     0,     0,   231,     0,   220,   197,   223,
     224,   225,     0,     0,   238,   239,   226,     0,     0,   335,
     332,   325,   315,   313,     0,     0,   274,     0,   243,   281,
       0,   282,   266,   299,   300,     0,   178,     0,     0,   176,
       0,   168,     0,     0,    96,   162,   163,     0,     0,     0,
       0,     0,     0,     0,     0,   217,   336,     0,     0,     0,
     268,   269,   270,   271,   272,   275,     0,     0,     0,     0,
     277,     0,   245,   280,   283,   243,     0,   303,     0,   297,
       0,   179,   175,   177,     0,   165,    95,     0,     0,   107,
     228,   229,   230,   232,   235,   236,   237,   328,     0,   335,
     273,     0,   276,     0,     0,   247,   247,   103,     0,   300,
       0,     0,    74,   193,     0,    99,     0,   320,     0,   274,
       0,     0,     0,   254,   260,     0,     0,   301,     0,   298,
     172,     0,   171,    97,   326,     0,     0,   242,     0,     0,
     252,     0,   267,   304,   300,   193,     0,   278,   244,     0,
     248,   249,   251,   250,     0,   261,   302,   173,     0,   246,
     255,   288,   274,   140,     0,     0,   124,   110,   109,   142,
     143,   144,   145,   146,     0,     0,     0,   131,   133,     0,
       0,   132,     0,   111,     0,   127,   135,   139,   141,     0,
       0,     0,   289,   262,   279,     0,     0,   193,   128,     0,
     108,     0,   123,   165,     0,   138,   256,   193,   130,     0,
     285,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     147,     0,   121,     0,     0,   125,     0,   165,   285,     0,
     140,     0,   241,     0,     0,   134,     0,   113,     0,     0,
     114,   137,   108,     0,     0,   120,   122,   126,   241,   136,
       0,   284,     0,   286,     0,     0,     0,     0,     0,   129,
     112,   286,   290,     0,   149,     0,     0,     0,     0,     0,
     149,   286,   240,   193,     0,   263,   116,   115,     0,   117,
     118,   257,   149,   148,   287,   165,   119,   165,   291,   264,
     258,   165,   292
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     5,   110,    10,    11,     8,     9,    16,    82,   221,
     165,   164,   162,   173,   175,     6,     7,    15,    50,   121,
     194,   211,   409,   508,   463,    51,   188,    52,   125,   603,
     604,   643,   623,   605,   606,   641,   607,   608,   609,   610,
     639,   695,   102,   129,    53,   646,    54,   307,   196,   306,
     505,   552,   402,   458,   459,    55,    56,   204,    57,   205,
      58,   207,   640,   186,   226,   673,   492,   525,   543,   574,
     298,   395,   560,   583,   648,   707,   396,   561,   581,   630,
     705,   397,   496,   486,   447,   448,   451,   495,   652,   684,
     584,   629,   691,   711,    59,   189,   301,   398,   531,   454,
     499,   529,    12,    13,    60,    61,    20,    21,   394,    89,
      90,   479,   388,   477
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -624
static const yytype_int16 yypact[] =
{
     210,  -624,  -624,  -624,  -624,    39,  -624,  -624,  -624,  -624,
    -624,    64,  -624,   -35,  -624,   732,  1504,    83,    98,    22,
     -35,  -624,   546,    44,    66,    68,    91,  -624,    94,  -624,
     137,    89,   145,   172,   182,   186,   201,   219,   230,  -624,
    -624,   258,   263,  -624,   270,   278,   286,  -624,   287,  -624,
    -624,  -624,  -624,   114,  -624,  -624,  -624,  -624,  -624,  -624,
    -624,   197,  -624,   144,   137,   321,   671,  -624,   324,   332,
     333,  -624,  -624,   338,   339,   343,   671,   344,   349,   351,
     354,   355,   256,   671,  -624,   361,  -624,   358,   360,   309,
     239,    98,  -624,  -624,  -624,  -624,  -624,  -624,  -624,  -624,
    -624,  -624,  -624,  -624,  -624,   374,   378,   379,   380,  -624,
    -624,   381,   382,   137,   137,   387,   137,    23,  -624,   388,
      28,   356,   137,   390,   392,   367,   354,  -624,  -624,  -624,
     341,    40,  -624,    80,  -624,  -624,   671,   671,   671,   368,
     369,   370,   372,   373,  -624,   375,   384,   389,   396,   397,
     398,   399,   400,   401,   405,   406,   407,   671,   671,  1328,
     350,  -624,   288,  -624,   310,    32,  -624,  -624,   460,  1699,
     311,  -624,  -624,   313,  -624,    36,  -624,  -624,  1699,   395,
     223,   223,   273,   245,   366,  -624,   671,  -624,    20,    25,
    -624,  -624,   111,   314,  -624,  -624,  -624,   315,   316,   318,
     319,   320,  -624,  -624,   150,   153,    37,   322,  -624,  -624,
     415,    33,    28,   329,   463,   464,   671,    24,   -35,   671,
     671,  -624,   671,   671,  -624,  -624,   959,   671,   671,   671,
     671,   671,   467,   471,   671,   472,   473,   475,   671,   671,
     476,   477,   671,   671,  -624,  -624,   671,   671,   671,   671,
     671,   671,   671,   671,   671,   671,   671,   671,   671,   671,
     671,   671,   671,   671,   671,   671,   671,   671,  1699,   478,
     481,  -624,   491,   671,   671,  1699,   274,   496,  -624,   497,
    -624,  -624,  -624,   371,   376,  -624,  -624,   501,  -624,  -624,
    -624,   -40,  1699,   546,  -624,  -624,  -624,  -624,  -624,  -624,
    -624,  -624,   503,  -624,  -624,   817,   479,    18,  -624,  -624,
    -624,  -624,  -624,  -624,  -624,   137,  -624,   137,   388,  -624,
    -624,  -624,  -624,  -624,   482,   132,  -624,    76,  -624,  -624,
    -624,  1348,  -624,   -12,  1699,  1699,  1526,  1699,  1699,  -624,
     939,   979,  1368,  1388,   999,   383,   385,  1019,   391,   394,
     404,  1408,  1449,   409,   411,  1039,  1469,  1659,  1551,  1429,
    1489,   742,   919,  1507,  1507,   386,   386,   386,   386,   279,
     279,   196,   196,  -624,  -624,  -624,  1699,  1699,  1699,  -624,
    -624,  -624,  1699,  1699,  -624,  -624,  -624,  -624,   223,   272,
     245,   454,  -624,  -624,   -19,   543,   615,   543,   671,   408,
    -624,     8,   499,  -624,   381,  -624,  -624,  -624,  -624,    28,
    -624,  -624,  -624,   484,  -624,   423,   424,   518,  -624,  -624,
     671,  -624,  -624,   671,   671,  -624,   671,  -624,  -624,  -624,
    -624,  -624,   671,   671,  -624,  -624,  -624,   521,   671,   393,
     510,  -624,  -624,  -624,   202,   490,  1636,   512,   431,  -624,
    1679,   447,  -624,  1699,    10,   532,  -624,   540,     4,  -624,
     480,  -624,    79,    28,  -624,  -624,  -624,   425,  1062,  1082,
    1102,  1122,  1142,  1162,   426,  1699,   245,   511,   223,   223,
    -624,  -624,  -624,  -624,  -624,  -624,   428,   671,   251,   554,
    -624,   534,   537,  -624,  -624,   431,   520,   541,   542,  -624,
     438,  -624,  -624,  -624,   578,   461,  -624,   120,    28,  -624,
    -624,  -624,  -624,  -624,  -624,  -624,  -624,  -624,   462,   393,
    -624,  1185,  -624,   671,   562,   505,   505,  -624,   671,    10,
     671,   469,  -624,  -624,   514,  -624,   129,   245,   551,   257,
    1205,   671,   572,  -624,  -624,   557,  1225,  -624,  1245,  -624,
    -624,   604,  -624,  -624,  -624,   574,   596,  -624,  1265,   671,
     110,   561,  -624,  -624,    10,  -624,   671,  -624,  -624,  1285,
    -624,  -624,  -624,  -624,   570,  -624,  -624,  -624,  1308,  -624,
    -624,  -624,   576,    11,    53,   611,   579,  -624,  -624,  -624,
    -624,  -624,  -624,  -624,   592,   593,   598,  -624,  -624,   599,
     600,  -624,    84,  -624,   601,  -624,  -624,  -624,    11,   582,
     603,   114,  -624,  -624,  -624,   231,   284,  -624,  -624,    27,
    -624,   605,  -624,     3,    84,  -624,  -624,  -624,  -624,   585,
     618,   607,   610,   509,   613,   517,   622,   623,   522,   523,
    -624,    72,  -624,    15,   243,  -624,    84,   164,   618,   524,
      11,   660,   567,    27,    27,  -624,    27,  -624,    27,    27,
    -624,  -624,   531,   533,    27,  -624,  -624,  -624,   567,  -624,
     614,  -624,   651,  -624,   538,   544,    19,   545,   549,  -624,
    -624,  -624,  -624,   675,    42,   550,   552,    27,   559,   560,
      42,  -624,  -624,  -624,   676,  -624,  -624,  -624,   564,  -624,
    -624,  -624,    42,  -624,  -624,   461,  -624,   461,  -624,  -624,
    -624,   461,  -624
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -624,  -624,   -53,  -624,  -624,  -624,  -624,   486,  -624,  -624,
    -624,  -624,  -624,  -624,   555,  -624,  -624,   488,  -624,  -624,
    -624,  -202,  -624,  -624,  -624,  -624,   163,  -183,  -624,  -170,
    -553,    49,    87,    59,  -624,  -624,   104,  -624,    63,  -624,
      21,  -623,  -624,   105,  -558,  -203,  -624,  -624,  -289,  -624,
    -624,  -624,  -624,  -624,   259,  -624,  -624,  -624,  -624,  -624,
    -624,  -189,   -93,  -624,   -63,    47,   224,  -624,   192,  -624,
    -624,  -624,  -624,  -624,  -624,  -624,  -624,  -624,  -624,  -624,
    -624,  -624,  -624,  -624,  -430,   323,  -624,  -624,    77,  -619,
    -624,  -624,  -624,  -624,  -624,  -624,  -624,  -624,  -624,  -624,
    -494,  -624,  -624,  -624,  -624,  -624,   508,   -16,  -624,   637,
    -174,  -624,  -624,   211
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -308
static const yytype_int16 yytable[] =
{
     185,   315,   317,   159,    92,   297,   283,   284,   456,   187,
     327,   132,   456,   169,   497,   586,   489,   319,   405,   620,
     178,    18,   195,   620,   293,   611,   587,   109,   278,   299,
     587,   620,   208,   209,   587,   547,   271,   322,   323,    14,
     278,   206,   587,   588,    18,   418,   219,   588,   693,   622,
     611,   588,   589,   590,   591,   592,   593,   612,   694,   588,
     198,   199,   690,   201,   203,   594,   595,   701,    17,   213,
     576,   622,   702,   224,   225,   403,   662,   294,    91,   708,
     322,   323,   300,   322,   323,   392,   222,   587,   620,    83,
      19,   295,   611,   666,   244,   245,    35,   268,   393,   587,
     103,   596,    84,   597,   588,   275,   442,   598,   498,   556,
     613,    45,    46,    19,   296,   461,   588,   105,   106,   443,
      45,    46,   104,   292,   322,   323,   621,   595,   107,   408,
     210,   108,    47,   322,   323,   324,   411,   412,   621,   595,
     313,   109,   599,   502,   645,   111,   457,   600,   131,   601,
     457,   602,   585,   331,   664,   404,   334,   335,   687,   337,
     338,   279,   202,   332,   340,   341,   342,   343,   344,   272,
     325,   347,   326,   279,   318,   351,   352,   220,   324,   355,
     356,   324,   112,   357,   358,   359,   360,   361,   362,   363,
     364,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,   376,   377,   378,   134,   135,   462,   600,   113,
     382,   383,   602,   325,   439,   414,   325,   223,   506,   114,
     600,    85,   324,   115,    86,    87,    88,    84,   262,   263,
     264,   324,   136,   137,   413,   620,   415,   416,   116,   138,
     139,   140,   570,   571,   572,   573,   587,   620,   302,   286,
     303,   127,   128,   141,   142,   143,   117,   325,   587,   535,
     144,   507,   406,   588,   407,   145,   325,   118,   553,   480,
     481,   482,   483,   484,   146,   588,   286,   384,   385,   147,
     148,   149,   150,   151,   152,   631,   632,   313,   620,   314,
     313,   153,   316,   154,   488,   119,   555,   631,   632,   587,
     120,   313,   534,   667,   518,   519,   536,   122,   155,   260,
     261,   262,   263,   264,   156,   123,   588,    92,   480,   481,
     482,   483,   484,   124,   126,   133,   130,   633,   160,     1,
       2,     3,   446,   450,   446,   453,   161,   163,   636,   637,
       4,   485,   166,   167,   157,   158,    85,   168,   170,    86,
     281,   282,   171,   134,   135,   172,   266,   468,   174,   176,
     469,   470,   297,   471,   177,   179,   182,   634,   287,   472,
     473,   288,   289,   290,   180,   475,   181,   183,   190,   634,
     136,   137,   191,   192,   193,   195,   197,   138,   139,   140,
     485,   200,   206,   212,   214,   287,   215,   218,   288,   289,
     440,   141,   142,   143,   216,   227,   228,   229,   144,   230,
     231,   285,   232,   145,   258,   259,   260,   261,   262,   263,
     264,   233,   146,   291,   521,   269,   234,   147,   148,   149,
     150,   151,   152,   235,   236,   237,   238,   239,   240,   153,
     550,   154,   241,   242,   243,   635,   638,   270,   276,   642,
     277,   280,   321,   304,   308,   309,   155,   310,   311,   312,
     540,   320,   156,   134,   135,   546,   273,   548,   328,   329,
     330,   345,   577,   665,   635,   346,   348,   349,   558,   350,
     353,   354,   379,   674,   675,   380,   642,   267,   677,   678,
     136,   137,   157,   158,   680,   381,   569,   138,   139,   140,
     386,   387,   709,   578,   710,   391,   665,   399,   712,   389,
     441,   141,   142,   143,   390,   460,   401,   698,   144,   410,
     426,   464,   467,   145,   427,   474,   478,   487,   490,   491,
     429,   476,   146,   430,   649,   494,   500,   147,   148,   149,
     150,   151,   152,   431,   501,   455,   134,   135,   434,   153,
     435,   154,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   293,   465,   466,   509,   516,   155,   520,   517,   504,
     522,   523,   156,   136,   137,   524,   527,   532,   528,   530,
     444,   139,   140,   445,   533,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   141,   142,   143,   274,   313,   541,
     537,   144,   157,   158,   542,   551,   145,   549,   554,   559,
     565,   566,   567,   488,   562,   146,  -108,   575,   134,   135,
     147,   148,   149,   150,   151,   152,   580,   614,   295,   615,
     616,   449,   153,    35,   154,   617,   618,   619,   624,   626,
     627,   650,   644,   651,   653,   136,   137,   654,   655,   155,
     656,   296,   138,   139,   140,   156,   657,    45,    46,   658,
     659,   660,   661,   669,   671,   672,   141,   142,   143,    47,
    -124,   682,   679,   144,   134,   135,   683,   685,   145,   692,
     704,   217,   305,   686,   688,   157,   158,   146,   689,   696,
     545,   697,   147,   148,   149,   150,   151,   152,   699,   700,
     663,   136,   137,   706,   153,   676,   154,   336,   138,   139,
     140,   647,   625,   670,   703,   681,   628,   503,   544,   526,
     452,   155,   141,   142,   143,   668,   333,   156,   184,   144,
     538,     0,     0,     0,   145,     0,    22,     0,     0,     0,
       0,     0,     0,   146,     0,     0,     0,     0,   147,   148,
     149,   150,   151,   152,     0,     0,     0,   157,   158,     0,
     153,     0,   154,   251,   252,   253,   254,   255,   256,   257,
     258,   259,   260,   261,   262,   263,   264,   155,    23,    24,
      25,     0,     0,   156,     0,     0,     0,     0,     0,     0,
       0,    26,    27,    28,    29,     0,    30,    31,     0,     0,
       0,     0,     0,     0,     0,    32,    33,    34,    35,     0,
       0,     0,     0,   157,   158,    36,    37,    38,    39,    40,
      41,    22,     0,     0,     0,    42,    43,    44,     0,     0,
       0,     0,    45,    46,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    47,     0,     0,     0,     0,     0,
       0,     0,     0,   400,     0,    48,     0,     0,     0,     0,
       0,  -307,     0,    23,    24,    25,     0,     0,     0,     0,
      49,     0,     0,     0,     0,     0,    26,    27,    28,    29,
       0,    30,    31,     0,     0,     0,     0,     0,     0,     0,
      32,    33,    34,    35,     0,     0,     0,     0,     0,     0,
      36,    37,    38,    39,    40,    41,     0,     0,     0,     0,
      42,    43,    44,     0,     0,     0,     0,    45,    46,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    47,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      48,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   246,    49,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   246,     0,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   246,     0,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   246,     0,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   246,     0,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   246,     0,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,     0,     0,   420,   246,   421,   247,
     248,   249,   250,   251,   252,   253,   254,   255,   256,   257,
     258,   259,   260,   261,   262,   263,   264,   246,   339,   247,
     248,   249,   250,   251,   252,   253,   254,   255,   256,   257,
     258,   259,   260,   261,   262,   263,   264,   246,   422,   247,
     248,   249,   250,   251,   252,   253,   254,   255,   256,   257,
     258,   259,   260,   261,   262,   263,   264,   246,   425,   247,
     248,   249,   250,   251,   252,   253,   254,   255,   256,   257,
     258,   259,   260,   261,   262,   263,   264,   246,   428,   247,
     248,   249,   250,   251,   252,   253,   254,   255,   256,   257,
     258,   259,   260,   261,   262,   263,   264,   246,   436,   247,
     248,   249,   250,   251,   252,   253,   254,   255,   256,   257,
     258,   259,   260,   261,   262,   263,   264,     0,     0,     0,
     246,   510,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     246,   511,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     246,   512,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     246,   513,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     246,   514,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     246,   515,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
       0,     0,     0,   246,   539,   247,   248,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,   263,   264,   246,   557,   247,   248,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,   263,   264,   246,   563,   247,   248,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,   263,   264,   246,   564,   247,   248,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,   263,   264,   246,   568,   247,   248,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,   263,   264,   246,   579,   247,   248,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,   263,   264,     0,     0,     0,     0,   582,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   246,   265,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   246,   417,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,     0,   423,     0,     0,    62,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,     0,   424,     0,     0,     0,     0,
      62,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,   264,    63,     0,     0,   432,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   419,     0,    63,     0,     0,     0,    64,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   262,   263,   264,   433,     0,     0,     0,
      64,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    65,     0,     0,     0,     0,   437,    66,    67,    68,
      69,    70,   -42,    71,    72,    73,     0,    74,    75,    76,
      77,    78,     0,    65,     0,     0,    79,    80,    81,    66,
      67,    68,    69,    70,     0,    71,    72,    73,     0,    74,
      75,    76,    77,    78,     0,     0,     0,     0,    79,    80,
      81,   246,     0,   247,   248,   249,   250,   251,   252,   253,
     254,   255,   256,   257,   258,   259,   260,   261,   262,   263,
     264,     0,     0,   488,   246,   438,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   246,   493,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   246,     0,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264
};

static const yytype_int16 yycheck[] =
{
      93,   204,   205,    66,    20,   188,   180,   181,     4,   102,
     212,    64,     4,    76,     4,     4,   446,   206,   307,     4,
      83,    56,     4,     4,     4,   583,    15,     4,     4,     4,
      15,     4,     4,     5,    15,   529,     4,     4,     5,     0,
       4,     4,    15,    32,    56,    57,     6,    32,     6,   602,
     608,    32,    41,    42,    43,    44,    45,     4,    16,    32,
     113,   114,   681,   116,   117,    54,    55,   690,     4,   122,
     564,   624,   691,   136,   137,    57,     4,    57,    56,   702,
       4,     5,    57,     4,     5,   125,     6,    15,     4,     6,
     125,    71,   650,   646,   157,   158,    76,   160,   138,    15,
      56,    90,     4,    92,    32,   168,   125,    96,    98,   539,
      57,   100,   101,   125,    94,   404,    32,    49,    50,   138,
     100,   101,    56,   186,     4,     5,    54,    55,    37,   318,
     102,    37,   112,     4,     5,   102,     4,     5,    54,    55,
     137,     4,   131,   139,   141,    56,   142,   136,     4,   138,
     142,   140,   582,   216,   139,   137,   219,   220,   139,   222,
     223,   137,   139,   139,   227,   228,   229,   230,   231,   137,
     137,   234,   139,   137,   137,   238,   239,   137,   102,   242,
     243,   102,    37,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,   264,   265,   266,   267,     3,     4,   409,   136,    37,
     273,   274,   140,   137,   388,   139,   137,   137,   139,    37,
     136,   123,   102,    37,   126,   127,   128,     4,    32,    33,
      34,   102,    30,    31,   102,     4,   329,   330,    37,    37,
      38,    39,   132,   133,   134,   135,    15,     4,   137,     4,
     139,   137,   138,    51,    52,    53,    37,   137,    15,   139,
      58,   463,   315,    32,   317,    63,   137,    37,   139,    67,
      68,    69,    70,    71,    72,    32,     4,     3,     4,    77,
      78,    79,    80,    81,    82,    54,    55,   137,     4,   139,
     137,    89,   139,    91,    37,    37,    39,    54,    55,    15,
      37,   137,   505,   139,   478,   479,   508,    37,   106,    30,
      31,    32,    33,    34,   112,    37,    32,   333,    67,    68,
      69,    70,    71,    37,    37,     4,   129,    96,     4,   119,
     120,   121,   395,   396,   397,   398,     4,     4,    54,    55,
     130,   139,     4,     4,   142,   143,   123,     4,     4,   126,
     127,   128,     3,     3,     4,     4,     6,   420,     4,     4,
     423,   424,   545,   426,   108,     4,    57,   136,   123,   432,
     433,   126,   127,   128,    16,   438,    16,   138,     4,   136,
      30,    31,     4,     4,     4,     4,     4,    37,    38,    39,
     139,     4,     4,    37,     4,   123,     4,    56,   126,   127,
     128,    51,    52,    53,    37,    37,    37,    37,    58,    37,
      37,   138,    37,    63,    28,    29,    30,    31,    32,    33,
      34,    37,    72,    57,   487,   137,    37,    77,    78,    79,
      80,    81,    82,    37,    37,    37,    37,    37,    37,    89,
     533,    91,    37,    37,    37,   615,   616,   137,   137,   619,
     137,    56,    37,   139,   139,   139,   106,   139,   139,   139,
     523,   139,   112,     3,     4,   528,     6,   530,   139,     6,
       6,     4,   565,   643,   644,     4,     4,     4,   541,     4,
       4,     4,     4,   653,   654,     4,   656,   137,   658,   659,
      30,    31,   142,   143,   664,     4,   559,    37,    38,    39,
       4,     4,   705,   566,   707,     4,   676,     4,   711,   138,
      56,    51,    52,    53,   138,    16,    37,   687,    58,    37,
     137,    37,     4,    63,   139,     4,    16,    37,    16,    98,
     139,   138,    72,   139,   627,    88,     4,    77,    78,    79,
      80,    81,    82,   139,     4,   137,     3,     4,   139,    89,
     139,    91,     6,     7,     8,     9,    10,    11,    12,    13,
      14,     4,   139,   139,   139,   139,   106,   139,    57,    89,
      16,    37,   112,    30,    31,    38,    56,   139,    37,    37,
      37,    38,    39,    40,     6,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    51,    52,    53,   137,   137,    37,
     138,    58,   142,   143,    99,    91,    63,   138,    57,    37,
       6,    37,    16,    37,    57,    72,    37,    56,     3,     4,
      77,    78,    79,    80,    81,    82,    56,    16,    71,    37,
      37,    16,    89,    76,    91,    37,    37,    37,    37,    57,
      37,    56,    37,    25,    37,    30,    31,    37,   139,   106,
      37,    94,    37,    38,    39,   112,   139,   100,   101,    37,
      37,   139,   139,   139,     4,    98,    51,    52,    53,   112,
     139,    57,   139,    58,     3,     4,    25,   139,    63,     4,
       4,   126,   194,   139,   139,   142,   143,    72,   139,   139,
     527,   139,    77,    78,    79,    80,    81,    82,   139,   139,
     641,    30,    31,   139,    89,   656,    91,   221,    37,    38,
      39,   624,   608,   650,   693,   668,   611,   458,   526,   495,
     397,   106,    51,    52,    53,   648,   218,   112,    91,    58,
     519,    -1,    -1,    -1,    63,    -1,     4,    -1,    -1,    -1,
      -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,    77,    78,
      79,    80,    81,    82,    -1,    -1,    -1,   142,   143,    -1,
      89,    -1,    91,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,   106,    46,    47,
      48,    -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    59,    60,    61,    62,    -1,    64,    65,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    73,    74,    75,    76,    -1,
      -1,    -1,    -1,   142,   143,    83,    84,    85,    86,    87,
      88,     4,    -1,    -1,    -1,    93,    94,    95,    -1,    -1,
      -1,    -1,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    36,    -1,   123,    -1,    -1,    -1,    -1,
      -1,   129,    -1,    46,    47,    48,    -1,    -1,    -1,    -1,
     138,    -1,    -1,    -1,    -1,    -1,    59,    60,    61,    62,
      -1,    64,    65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      73,    74,    75,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      83,    84,    85,    86,    87,    88,    -1,    -1,    -1,    -1,
      93,    94,    95,    -1,    -1,    -1,    -1,   100,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    15,   138,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    15,    -1,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    15,    -1,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    15,    -1,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    15,    -1,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    15,    -1,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    -1,    -1,   137,    15,   139,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    15,   139,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    15,   139,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    15,   139,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    15,   139,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    15,   139,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    -1,    -1,    -1,
      15,   139,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      15,   139,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      15,   139,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      15,   139,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      15,   139,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      15,   139,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      -1,    -1,    -1,    15,   139,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    15,   139,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    15,   139,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    15,   139,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    15,   139,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    15,   139,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    -1,    -1,    -1,    -1,   139,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    15,   137,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    15,   137,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    -1,   137,    -1,    -1,     4,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    -1,   137,    -1,    -1,    -1,    -1,
       4,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    38,    -1,    -1,   137,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    36,    -1,    38,    -1,    -1,    -1,    64,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,   137,    -1,    -1,    -1,
      64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    97,    -1,    -1,    -1,    -1,   137,   103,   104,   105,
     106,   107,   108,   109,   110,   111,    -1,   113,   114,   115,
     116,   117,    -1,    97,    -1,    -1,   122,   123,   124,   103,
     104,   105,   106,   107,    -1,   109,   110,   111,    -1,   113,
     114,   115,   116,   117,    -1,    -1,    -1,    -1,   122,   123,
     124,    15,    -1,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    -1,    -1,    37,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    15,    -1,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   119,   120,   121,   130,   145,   159,   160,   149,   150,
     147,   148,   246,   247,     0,   161,   151,     4,    56,   125,
     250,   251,     4,    46,    47,    48,    59,    60,    61,    62,
      64,    65,    73,    74,    75,    76,    83,    84,    85,    86,
      87,    88,    93,    94,    95,   100,   101,   112,   123,   138,
     162,   169,   171,   188,   190,   199,   200,   202,   204,   238,
     248,   249,     4,    38,    64,    97,   103,   104,   105,   106,
     107,   109,   110,   111,   113,   114,   115,   116,   117,   122,
     123,   124,   152,     6,     4,   123,   126,   127,   128,   253,
     254,    56,   251,     6,     7,     8,     9,    10,    11,    12,
      13,    14,   186,    56,    56,    49,    50,    37,    37,     4,
     146,    56,    37,    37,    37,    37,    37,    37,    37,    37,
      37,   163,    37,    37,    37,   172,    37,   137,   138,   187,
     129,     4,   146,     4,     3,     4,    30,    31,    37,    38,
      39,    51,    52,    53,    58,    63,    72,    77,    78,    79,
      80,    81,    82,    89,    91,   106,   112,   142,   143,   208,
       4,     4,   156,     4,   155,   154,     4,     4,     4,   208,
       4,     3,     4,   157,     4,   158,     4,   108,   208,     4,
      16,    16,    57,   138,   253,   206,   207,   206,   170,   239,
       4,     4,     4,     4,   164,     4,   192,     4,   146,   146,
       4,   146,   139,   146,   201,   203,     4,   205,     4,     5,
     102,   165,    37,   146,     4,     4,    37,   158,    56,     6,
     137,   153,     6,   137,   208,   208,   208,    37,    37,    37,
      37,    37,    37,    37,    37,    37,    37,    37,    37,    37,
      37,    37,    37,    37,   208,   208,    15,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,   137,     6,   137,   208,   137,
     137,     4,   137,     6,   137,   208,   137,   137,     4,   137,
      56,   127,   128,   254,   254,   138,     4,   123,   126,   127,
     128,    57,   208,     4,    57,    71,    94,   171,   214,     4,
      57,   240,   137,   139,   139,   161,   193,   191,   139,   139,
     139,   139,   139,   137,   139,   189,   139,   189,   137,   205,
     139,    37,     4,     5,   102,   137,   139,   165,   139,     6,
       6,   208,   139,   250,   208,   208,   151,   208,   208,   139,
     208,   208,   208,   208,   208,     4,     4,   208,     4,     4,
       4,   208,   208,     4,     4,   208,   208,   208,   208,   208,
     208,   208,   208,   208,   208,   208,   208,   208,   208,   208,
     208,   208,   208,   208,   208,   208,   208,   208,   208,     4,
       4,     4,   208,   208,     3,     4,     4,     4,   256,   138,
     138,     4,   125,   138,   252,   215,   220,   225,   241,     4,
      36,    37,   196,    57,   137,   192,   146,   146,   205,   166,
      37,     4,     5,   102,   139,   206,   206,   137,    57,    36,
     137,   139,   139,   137,   137,   139,   137,   139,   139,   139,
     139,   139,   137,   137,   139,   139,   139,   137,    16,   254,
     128,    56,   125,   138,    37,    40,   208,   228,   229,    16,
     208,   230,   229,   208,   243,   137,     4,   142,   197,   198,
      16,   192,   165,   168,    37,   139,   139,     4,   208,   208,
     208,   208,   208,   208,     4,   208,   138,   257,    16,   255,
      67,    68,    69,    70,    71,   139,   227,    37,    37,   228,
      16,    98,   210,    16,    88,   231,   226,     4,    98,   244,
       4,     4,   139,   198,    89,   194,   139,   165,   167,   139,
     139,   139,   139,   139,   139,   139,   139,    57,   254,   254,
     139,   208,    16,    37,    38,   211,   210,    56,    37,   245,
      37,   242,   139,     6,   189,   139,   165,   138,   257,   139,
     208,    37,    99,   212,   212,   170,   208,   244,   208,   138,
     206,    91,   195,   139,    57,    39,   228,   139,   208,    37,
     216,   221,    57,   139,   139,     6,    37,    16,   139,   208,
     132,   133,   134,   135,   213,    56,   244,   206,   208,   139,
      56,   222,   139,   217,   234,   228,     4,    15,    32,    41,
      42,    43,    44,    45,    54,    55,    90,    92,    96,   131,
     136,   138,   140,   173,   174,   177,   178,   180,   181,   182,
     183,   188,     4,    57,    16,    37,    37,    37,    37,    37,
       4,    54,   174,   176,    37,   180,    57,    37,   187,   235,
     223,    54,    55,    96,   136,   173,    54,    55,   173,   184,
     206,   179,   173,   175,    37,   141,   189,   176,   218,   206,
      56,    25,   232,    37,    37,   139,    37,   139,    37,    37,
     139,   139,     4,   177,   139,   173,   174,   139,   232,   139,
     182,     4,    98,   209,   173,   173,   175,   173,   173,   139,
     173,   209,    57,    25,   233,   139,   139,   139,   139,   139,
     233,   236,     4,     6,    16,   185,   139,   139,   173,   139,
     139,   185,   233,   184,     4,   224,   139,   219,   185,   189,
     189,   237,   189
};

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
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
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
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
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
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

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
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
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

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
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
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
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

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

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
#line 169 "ldgram.y"
    { ldlex_defsym(); }
    break;

  case 8:
#line 171 "ldgram.y"
    {
		  ldlex_popstate();
		  lang_add_assignment(exp_assop((yyvsp[(3) - (4)].token),(yyvsp[(2) - (4)].name),(yyvsp[(4) - (4)].etree)));
		}
    break;

  case 9:
#line 179 "ldgram.y"
    {
		  ldlex_mri_script ();
		  PUSH_ERROR (_("MRI style script"));
		}
    break;

  case 10:
#line 184 "ldgram.y"
    {
		  ldlex_popstate ();
		  mri_draw_tree ();
		  POP_ERROR ();
		}
    break;

  case 15:
#line 199 "ldgram.y"
    {
			einfo(_("%P%F: unrecognised keyword in MRI style script '%s'\n"),(yyvsp[(1) - (1)].name));
			}
    break;

  case 16:
#line 202 "ldgram.y"
    {
			config.map_filename = "-";
			}
    break;

  case 19:
#line 208 "ldgram.y"
    { mri_public((yyvsp[(2) - (4)].name), (yyvsp[(4) - (4)].etree)); }
    break;

  case 20:
#line 210 "ldgram.y"
    { mri_public((yyvsp[(2) - (4)].name), (yyvsp[(4) - (4)].etree)); }
    break;

  case 21:
#line 212 "ldgram.y"
    { mri_public((yyvsp[(2) - (3)].name), (yyvsp[(3) - (3)].etree)); }
    break;

  case 22:
#line 214 "ldgram.y"
    { mri_format((yyvsp[(2) - (2)].name)); }
    break;

  case 23:
#line 216 "ldgram.y"
    { mri_output_section((yyvsp[(2) - (4)].name), (yyvsp[(4) - (4)].etree));}
    break;

  case 24:
#line 218 "ldgram.y"
    { mri_output_section((yyvsp[(2) - (3)].name), (yyvsp[(3) - (3)].etree));}
    break;

  case 25:
#line 220 "ldgram.y"
    { mri_output_section((yyvsp[(2) - (4)].name), (yyvsp[(4) - (4)].etree));}
    break;

  case 26:
#line 222 "ldgram.y"
    { mri_align((yyvsp[(2) - (4)].name),(yyvsp[(4) - (4)].etree)); }
    break;

  case 27:
#line 224 "ldgram.y"
    { mri_align((yyvsp[(2) - (4)].name),(yyvsp[(4) - (4)].etree)); }
    break;

  case 28:
#line 226 "ldgram.y"
    { mri_alignmod((yyvsp[(2) - (4)].name),(yyvsp[(4) - (4)].etree)); }
    break;

  case 29:
#line 228 "ldgram.y"
    { mri_alignmod((yyvsp[(2) - (4)].name),(yyvsp[(4) - (4)].etree)); }
    break;

  case 32:
#line 232 "ldgram.y"
    { mri_name((yyvsp[(2) - (2)].name)); }
    break;

  case 33:
#line 234 "ldgram.y"
    { mri_alias((yyvsp[(2) - (4)].name),(yyvsp[(4) - (4)].name),0);}
    break;

  case 34:
#line 236 "ldgram.y"
    { mri_alias ((yyvsp[(2) - (4)].name), 0, (int) (yyvsp[(4) - (4)].bigint).integer); }
    break;

  case 35:
#line 238 "ldgram.y"
    { mri_base((yyvsp[(2) - (2)].etree)); }
    break;

  case 36:
#line 240 "ldgram.y"
    { mri_truncate ((unsigned int) (yyvsp[(2) - (2)].bigint).integer); }
    break;

  case 39:
#line 244 "ldgram.y"
    { ldlex_script (); ldfile_open_command_file((yyvsp[(2) - (2)].name)); }
    break;

  case 40:
#line 246 "ldgram.y"
    { ldlex_popstate (); }
    break;

  case 41:
#line 248 "ldgram.y"
    { lang_add_entry ((yyvsp[(2) - (2)].name), FALSE); }
    break;

  case 43:
#line 253 "ldgram.y"
    { mri_order((yyvsp[(3) - (3)].name)); }
    break;

  case 44:
#line 254 "ldgram.y"
    { mri_order((yyvsp[(2) - (2)].name)); }
    break;

  case 46:
#line 260 "ldgram.y"
    { mri_load((yyvsp[(1) - (1)].name)); }
    break;

  case 47:
#line 261 "ldgram.y"
    { mri_load((yyvsp[(3) - (3)].name)); }
    break;

  case 48:
#line 266 "ldgram.y"
    { mri_only_load((yyvsp[(1) - (1)].name)); }
    break;

  case 49:
#line 268 "ldgram.y"
    { mri_only_load((yyvsp[(3) - (3)].name)); }
    break;

  case 50:
#line 272 "ldgram.y"
    { (yyval.name) = NULL; }
    break;

  case 53:
#line 279 "ldgram.y"
    { ldlang_add_undef ((yyvsp[(1) - (1)].name)); }
    break;

  case 54:
#line 281 "ldgram.y"
    { ldlang_add_undef ((yyvsp[(2) - (2)].name)); }
    break;

  case 55:
#line 283 "ldgram.y"
    { ldlang_add_undef ((yyvsp[(3) - (3)].name)); }
    break;

  case 56:
#line 287 "ldgram.y"
    {
	 ldlex_both();
	}
    break;

  case 57:
#line 291 "ldgram.y"
    {
	ldlex_popstate();
	}
    break;

  case 70:
#line 316 "ldgram.y"
    { lang_add_target((yyvsp[(3) - (4)].name)); }
    break;

  case 71:
#line 318 "ldgram.y"
    { ldfile_add_library_path ((yyvsp[(3) - (4)].name), FALSE); }
    break;

  case 72:
#line 320 "ldgram.y"
    { lang_add_output((yyvsp[(3) - (4)].name), 1); }
    break;

  case 73:
#line 322 "ldgram.y"
    { lang_add_output_format ((yyvsp[(3) - (4)].name), (char *) NULL,
					    (char *) NULL, 1); }
    break;

  case 74:
#line 325 "ldgram.y"
    { lang_add_output_format ((yyvsp[(3) - (8)].name), (yyvsp[(5) - (8)].name), (yyvsp[(7) - (8)].name), 1); }
    break;

  case 75:
#line 327 "ldgram.y"
    { ldfile_set_output_arch ((yyvsp[(3) - (4)].name), bfd_arch_unknown); }
    break;

  case 76:
#line 329 "ldgram.y"
    { command_line.force_common_definition = TRUE ; }
    break;

  case 77:
#line 331 "ldgram.y"
    { command_line.inhibit_common_definition = TRUE ; }
    break;

  case 79:
#line 334 "ldgram.y"
    { lang_enter_group (); }
    break;

  case 80:
#line 336 "ldgram.y"
    { lang_leave_group (); }
    break;

  case 81:
#line 338 "ldgram.y"
    { lang_add_map((yyvsp[(3) - (4)].name)); }
    break;

  case 82:
#line 340 "ldgram.y"
    { ldlex_script (); ldfile_open_command_file((yyvsp[(2) - (2)].name)); }
    break;

  case 83:
#line 342 "ldgram.y"
    { ldlex_popstate (); }
    break;

  case 84:
#line 344 "ldgram.y"
    {
		  lang_add_nocrossref ((yyvsp[(3) - (4)].nocrossref));
		}
    break;

  case 86:
#line 349 "ldgram.y"
    { lang_add_insert ((yyvsp[(3) - (3)].name), 0); }
    break;

  case 87:
#line 351 "ldgram.y"
    { lang_add_insert ((yyvsp[(3) - (3)].name), 1); }
    break;

  case 88:
#line 356 "ldgram.y"
    { lang_add_input_file((yyvsp[(1) - (1)].name),lang_input_file_is_search_file_enum,
				 (char *)NULL); }
    break;

  case 89:
#line 359 "ldgram.y"
    { lang_add_input_file((yyvsp[(3) - (3)].name),lang_input_file_is_search_file_enum,
				 (char *)NULL); }
    break;

  case 90:
#line 362 "ldgram.y"
    { lang_add_input_file((yyvsp[(2) - (2)].name),lang_input_file_is_search_file_enum,
				 (char *)NULL); }
    break;

  case 91:
#line 365 "ldgram.y"
    { lang_add_input_file((yyvsp[(1) - (1)].name),lang_input_file_is_l_enum,
				 (char *)NULL); }
    break;

  case 92:
#line 368 "ldgram.y"
    { lang_add_input_file((yyvsp[(3) - (3)].name),lang_input_file_is_l_enum,
				 (char *)NULL); }
    break;

  case 93:
#line 371 "ldgram.y"
    { lang_add_input_file((yyvsp[(2) - (2)].name),lang_input_file_is_l_enum,
				 (char *)NULL); }
    break;

  case 94:
#line 374 "ldgram.y"
    { (yyval.integer) = as_needed; as_needed = TRUE; }
    break;

  case 95:
#line 376 "ldgram.y"
    { as_needed = (yyvsp[(3) - (5)].integer); }
    break;

  case 96:
#line 378 "ldgram.y"
    { (yyval.integer) = as_needed; as_needed = TRUE; }
    break;

  case 97:
#line 380 "ldgram.y"
    { as_needed = (yyvsp[(5) - (7)].integer); }
    break;

  case 98:
#line 382 "ldgram.y"
    { (yyval.integer) = as_needed; as_needed = TRUE; }
    break;

  case 99:
#line 384 "ldgram.y"
    { as_needed = (yyvsp[(4) - (6)].integer); }
    break;

  case 104:
#line 399 "ldgram.y"
    { lang_add_entry ((yyvsp[(3) - (4)].name), FALSE); }
    break;

  case 106:
#line 401 "ldgram.y"
    {ldlex_expression ();}
    break;

  case 107:
#line 402 "ldgram.y"
    { ldlex_popstate ();
		  lang_add_assignment (exp_assert ((yyvsp[(4) - (7)].etree), (yyvsp[(6) - (7)].name))); }
    break;

  case 108:
#line 410 "ldgram.y"
    {
			  (yyval.cname) = (yyvsp[(1) - (1)].name);
			}
    break;

  case 109:
#line 414 "ldgram.y"
    {
			  (yyval.cname) = "*";
			}
    break;

  case 110:
#line 418 "ldgram.y"
    {
			  (yyval.cname) = "?";
			}
    break;

  case 111:
#line 425 "ldgram.y"
    {
			  (yyval.wildcard).name = (yyvsp[(1) - (1)].cname);
			  (yyval.wildcard).sorted = none;
			  (yyval.wildcard).exclude_name_list = NULL;
			}
    break;

  case 112:
#line 431 "ldgram.y"
    {
			  (yyval.wildcard).name = (yyvsp[(5) - (5)].cname);
			  (yyval.wildcard).sorted = none;
			  (yyval.wildcard).exclude_name_list = (yyvsp[(3) - (5)].name_list);
			}
    break;

  case 113:
#line 437 "ldgram.y"
    {
			  (yyval.wildcard).name = (yyvsp[(3) - (4)].cname);
			  (yyval.wildcard).sorted = by_name;
			  (yyval.wildcard).exclude_name_list = NULL;
			}
    break;

  case 114:
#line 443 "ldgram.y"
    {
			  (yyval.wildcard).name = (yyvsp[(3) - (4)].cname);
			  (yyval.wildcard).sorted = by_alignment;
			  (yyval.wildcard).exclude_name_list = NULL;
			}
    break;

  case 115:
#line 449 "ldgram.y"
    {
			  (yyval.wildcard).name = (yyvsp[(5) - (7)].cname);
			  (yyval.wildcard).sorted = by_name_alignment;
			  (yyval.wildcard).exclude_name_list = NULL;
			}
    break;

  case 116:
#line 455 "ldgram.y"
    {
			  (yyval.wildcard).name = (yyvsp[(5) - (7)].cname);
			  (yyval.wildcard).sorted = by_name;
			  (yyval.wildcard).exclude_name_list = NULL;
			}
    break;

  case 117:
#line 461 "ldgram.y"
    {
			  (yyval.wildcard).name = (yyvsp[(5) - (7)].cname);
			  (yyval.wildcard).sorted = by_alignment_name;
			  (yyval.wildcard).exclude_name_list = NULL;
			}
    break;

  case 118:
#line 467 "ldgram.y"
    {
			  (yyval.wildcard).name = (yyvsp[(5) - (7)].cname);
			  (yyval.wildcard).sorted = by_alignment;
			  (yyval.wildcard).exclude_name_list = NULL;
			}
    break;

  case 119:
#line 473 "ldgram.y"
    {
			  (yyval.wildcard).name = (yyvsp[(7) - (8)].cname);
			  (yyval.wildcard).sorted = by_name;
			  (yyval.wildcard).exclude_name_list = (yyvsp[(5) - (8)].name_list);
			}
    break;

  case 120:
#line 482 "ldgram.y"
    {
			  struct name_list *tmp;
			  tmp = (struct name_list *) xmalloc (sizeof *tmp);
			  tmp->name = (yyvsp[(2) - (2)].cname);
			  tmp->next = (yyvsp[(1) - (2)].name_list);
			  (yyval.name_list) = tmp;
			}
    break;

  case 121:
#line 491 "ldgram.y"
    {
			  struct name_list *tmp;
			  tmp = (struct name_list *) xmalloc (sizeof *tmp);
			  tmp->name = (yyvsp[(1) - (1)].cname);
			  tmp->next = NULL;
			  (yyval.name_list) = tmp;
			}
    break;

  case 122:
#line 502 "ldgram.y"
    {
			  struct wildcard_list *tmp;
			  tmp = (struct wildcard_list *) xmalloc (sizeof *tmp);
			  tmp->next = (yyvsp[(1) - (3)].wildcard_list);
			  tmp->spec = (yyvsp[(3) - (3)].wildcard);
			  (yyval.wildcard_list) = tmp;
			}
    break;

  case 123:
#line 511 "ldgram.y"
    {
			  struct wildcard_list *tmp;
			  tmp = (struct wildcard_list *) xmalloc (sizeof *tmp);
			  tmp->next = NULL;
			  tmp->spec = (yyvsp[(1) - (1)].wildcard);
			  (yyval.wildcard_list) = tmp;
			}
    break;

  case 124:
#line 522 "ldgram.y"
    {
			  struct wildcard_spec tmp;
			  tmp.name = (yyvsp[(1) - (1)].name);
			  tmp.exclude_name_list = NULL;
			  tmp.sorted = none;
			  lang_add_wild (&tmp, NULL, ldgram_had_keep);
			}
    break;

  case 125:
#line 530 "ldgram.y"
    {
			  lang_add_wild (NULL, (yyvsp[(2) - (3)].wildcard_list), ldgram_had_keep);
			}
    break;

  case 126:
#line 534 "ldgram.y"
    {
			  lang_add_wild (&(yyvsp[(1) - (4)].wildcard), (yyvsp[(3) - (4)].wildcard_list), ldgram_had_keep);
			}
    break;

  case 128:
#line 542 "ldgram.y"
    { ldgram_had_keep = TRUE; }
    break;

  case 129:
#line 544 "ldgram.y"
    { ldgram_had_keep = FALSE; }
    break;

  case 131:
#line 550 "ldgram.y"
    {
 		lang_add_attribute(lang_object_symbols_statement_enum);
	      	}
    break;

  case 133:
#line 555 "ldgram.y"
    {

		  lang_add_attribute(lang_constructors_statement_enum);
		}
    break;

  case 134:
#line 560 "ldgram.y"
    {
		  constructors_sorted = TRUE;
		  lang_add_attribute (lang_constructors_statement_enum);
		}
    break;

  case 136:
#line 566 "ldgram.y"
    {
			  lang_add_data ((int) (yyvsp[(1) - (4)].integer), (yyvsp[(3) - (4)].etree));
			}
    break;

  case 137:
#line 571 "ldgram.y"
    {
			  lang_add_fill ((yyvsp[(3) - (4)].fill));
			}
    break;

  case 142:
#line 588 "ldgram.y"
    { (yyval.integer) = (yyvsp[(1) - (1)].token); }
    break;

  case 143:
#line 590 "ldgram.y"
    { (yyval.integer) = (yyvsp[(1) - (1)].token); }
    break;

  case 144:
#line 592 "ldgram.y"
    { (yyval.integer) = (yyvsp[(1) - (1)].token); }
    break;

  case 145:
#line 594 "ldgram.y"
    { (yyval.integer) = (yyvsp[(1) - (1)].token); }
    break;

  case 146:
#line 596 "ldgram.y"
    { (yyval.integer) = (yyvsp[(1) - (1)].token); }
    break;

  case 147:
#line 601 "ldgram.y"
    {
		  (yyval.fill) = exp_get_fill ((yyvsp[(1) - (1)].etree), 0, "fill value");
		}
    break;

  case 148:
#line 608 "ldgram.y"
    { (yyval.fill) = (yyvsp[(2) - (2)].fill); }
    break;

  case 149:
#line 609 "ldgram.y"
    { (yyval.fill) = (fill_type *) 0; }
    break;

  case 150:
#line 614 "ldgram.y"
    { (yyval.token) = '+'; }
    break;

  case 151:
#line 616 "ldgram.y"
    { (yyval.token) = '-'; }
    break;

  case 152:
#line 618 "ldgram.y"
    { (yyval.token) = '*'; }
    break;

  case 153:
#line 620 "ldgram.y"
    { (yyval.token) = '/'; }
    break;

  case 154:
#line 622 "ldgram.y"
    { (yyval.token) = LSHIFT; }
    break;

  case 155:
#line 624 "ldgram.y"
    { (yyval.token) = RSHIFT; }
    break;

  case 156:
#line 626 "ldgram.y"
    { (yyval.token) = '&'; }
    break;

  case 157:
#line 628 "ldgram.y"
    { (yyval.token) = '|'; }
    break;

  case 160:
#line 638 "ldgram.y"
    {
		  lang_add_assignment (exp_assop ((yyvsp[(2) - (3)].token), (yyvsp[(1) - (3)].name), (yyvsp[(3) - (3)].etree)));
		}
    break;

  case 161:
#line 642 "ldgram.y"
    {
		  lang_add_assignment (exp_assop ('=', (yyvsp[(1) - (3)].name),
						  exp_binop ((yyvsp[(2) - (3)].token),
							     exp_nameop (NAME,
									 (yyvsp[(1) - (3)].name)),
							     (yyvsp[(3) - (3)].etree))));
		}
    break;

  case 162:
#line 650 "ldgram.y"
    {
		  lang_add_assignment (exp_provide ((yyvsp[(3) - (6)].name), (yyvsp[(5) - (6)].etree), FALSE));
		}
    break;

  case 163:
#line 654 "ldgram.y"
    {
		  lang_add_assignment (exp_provide ((yyvsp[(3) - (6)].name), (yyvsp[(5) - (6)].etree), TRUE));
		}
    break;

  case 170:
#line 676 "ldgram.y"
    { region = lang_memory_region_lookup ((yyvsp[(1) - (1)].name), TRUE); }
    break;

  case 171:
#line 679 "ldgram.y"
    {}
    break;

  case 172:
#line 684 "ldgram.y"
    {
		  region->origin = exp_get_vma ((yyvsp[(3) - (3)].etree), 0, "origin");
		  region->current = region->origin;
		}
    break;

  case 173:
#line 692 "ldgram.y"
    {
		  region->length = exp_get_vma ((yyvsp[(3) - (3)].etree), -1, "length");
		}
    break;

  case 174:
#line 699 "ldgram.y"
    { /* dummy action to avoid bison 1.25 error message */ }
    break;

  case 178:
#line 710 "ldgram.y"
    { lang_set_flags (region, (yyvsp[(1) - (1)].name), 0); }
    break;

  case 179:
#line 712 "ldgram.y"
    { lang_set_flags (region, (yyvsp[(2) - (2)].name), 1); }
    break;

  case 180:
#line 717 "ldgram.y"
    { lang_startup((yyvsp[(3) - (4)].name)); }
    break;

  case 182:
#line 723 "ldgram.y"
    { ldemul_hll((char *)NULL); }
    break;

  case 183:
#line 728 "ldgram.y"
    { ldemul_hll((yyvsp[(3) - (3)].name)); }
    break;

  case 184:
#line 730 "ldgram.y"
    { ldemul_hll((yyvsp[(1) - (1)].name)); }
    break;

  case 186:
#line 738 "ldgram.y"
    { ldemul_syslib((yyvsp[(3) - (3)].name)); }
    break;

  case 188:
#line 744 "ldgram.y"
    { lang_float(TRUE); }
    break;

  case 189:
#line 746 "ldgram.y"
    { lang_float(FALSE); }
    break;

  case 190:
#line 751 "ldgram.y"
    {
		  (yyval.nocrossref) = NULL;
		}
    break;

  case 191:
#line 755 "ldgram.y"
    {
		  struct lang_nocrossref *n;

		  n = (struct lang_nocrossref *) xmalloc (sizeof *n);
		  n->name = (yyvsp[(1) - (2)].name);
		  n->next = (yyvsp[(2) - (2)].nocrossref);
		  (yyval.nocrossref) = n;
		}
    break;

  case 192:
#line 764 "ldgram.y"
    {
		  struct lang_nocrossref *n;

		  n = (struct lang_nocrossref *) xmalloc (sizeof *n);
		  n->name = (yyvsp[(1) - (3)].name);
		  n->next = (yyvsp[(3) - (3)].nocrossref);
		  (yyval.nocrossref) = n;
		}
    break;

  case 193:
#line 774 "ldgram.y"
    { ldlex_expression (); }
    break;

  case 194:
#line 776 "ldgram.y"
    { ldlex_popstate (); (yyval.etree)=(yyvsp[(2) - (2)].etree);}
    break;

  case 195:
#line 781 "ldgram.y"
    { (yyval.etree) = exp_unop ('-', (yyvsp[(2) - (2)].etree)); }
    break;

  case 196:
#line 783 "ldgram.y"
    { (yyval.etree) = (yyvsp[(2) - (3)].etree); }
    break;

  case 197:
#line 785 "ldgram.y"
    { (yyval.etree) = exp_unop ((int) (yyvsp[(1) - (4)].integer),(yyvsp[(3) - (4)].etree)); }
    break;

  case 198:
#line 787 "ldgram.y"
    { (yyval.etree) = exp_unop ('!', (yyvsp[(2) - (2)].etree)); }
    break;

  case 199:
#line 789 "ldgram.y"
    { (yyval.etree) = (yyvsp[(2) - (2)].etree); }
    break;

  case 200:
#line 791 "ldgram.y"
    { (yyval.etree) = exp_unop ('~', (yyvsp[(2) - (2)].etree));}
    break;

  case 201:
#line 794 "ldgram.y"
    { (yyval.etree) = exp_binop ('*', (yyvsp[(1) - (3)].etree), (yyvsp[(3) - (3)].etree)); }
    break;

  case 202:
#line 796 "ldgram.y"
    { (yyval.etree) = exp_binop ('/', (yyvsp[(1) - (3)].etree), (yyvsp[(3) - (3)].etree)); }
    break;

  case 203:
#line 798 "ldgram.y"
    { (yyval.etree) = exp_binop ('%', (yyvsp[(1) - (3)].etree), (yyvsp[(3) - (3)].etree)); }
    break;

  case 204:
#line 800 "ldgram.y"
    { (yyval.etree) = exp_binop ('+', (yyvsp[(1) - (3)].etree), (yyvsp[(3) - (3)].etree)); }
    break;

  case 205:
#line 802 "ldgram.y"
    { (yyval.etree) = exp_binop ('-' , (yyvsp[(1) - (3)].etree), (yyvsp[(3) - (3)].etree)); }
    break;

  case 206:
#line 804 "ldgram.y"
    { (yyval.etree) = exp_binop (LSHIFT , (yyvsp[(1) - (3)].etree), (yyvsp[(3) - (3)].etree)); }
    break;

  case 207:
#line 806 "ldgram.y"
    { (yyval.etree) = exp_binop (RSHIFT , (yyvsp[(1) - (3)].etree), (yyvsp[(3) - (3)].etree)); }
    break;

  case 208:
#line 808 "ldgram.y"
    { (yyval.etree) = exp_binop (EQ , (yyvsp[(1) - (3)].etree), (yyvsp[(3) - (3)].etree)); }
    break;

  case 209:
#line 810 "ldgram.y"
    { (yyval.etree) = exp_binop (NE , (yyvsp[(1) - (3)].etree), (yyvsp[(3) - (3)].etree)); }
    break;

  case 210:
#line 812 "ldgram.y"
    { (yyval.etree) = exp_binop (LE , (yyvsp[(1) - (3)].etree), (yyvsp[(3) - (3)].etree)); }
    break;

  case 211:
#line 814 "ldgram.y"
    { (yyval.etree) = exp_binop (GE , (yyvsp[(1) - (3)].etree), (yyvsp[(3) - (3)].etree)); }
    break;

  case 212:
#line 816 "ldgram.y"
    { (yyval.etree) = exp_binop ('<' , (yyvsp[(1) - (3)].etree), (yyvsp[(3) - (3)].etree)); }
    break;

  case 213:
#line 818 "ldgram.y"
    { (yyval.etree) = exp_binop ('>' , (yyvsp[(1) - (3)].etree), (yyvsp[(3) - (3)].etree)); }
    break;

  case 214:
#line 820 "ldgram.y"
    { (yyval.etree) = exp_binop ('&' , (yyvsp[(1) - (3)].etree), (yyvsp[(3) - (3)].etree)); }
    break;

  case 215:
#line 822 "ldgram.y"
    { (yyval.etree) = exp_binop ('^' , (yyvsp[(1) - (3)].etree), (yyvsp[(3) - (3)].etree)); }
    break;

  case 216:
#line 824 "ldgram.y"
    { (yyval.etree) = exp_binop ('|' , (yyvsp[(1) - (3)].etree), (yyvsp[(3) - (3)].etree)); }
    break;

  case 217:
#line 826 "ldgram.y"
    { (yyval.etree) = exp_trinop ('?' , (yyvsp[(1) - (5)].etree), (yyvsp[(3) - (5)].etree), (yyvsp[(5) - (5)].etree)); }
    break;

  case 218:
#line 828 "ldgram.y"
    { (yyval.etree) = exp_binop (ANDAND , (yyvsp[(1) - (3)].etree), (yyvsp[(3) - (3)].etree)); }
    break;

  case 219:
#line 830 "ldgram.y"
    { (yyval.etree) = exp_binop (OROR , (yyvsp[(1) - (3)].etree), (yyvsp[(3) - (3)].etree)); }
    break;

  case 220:
#line 832 "ldgram.y"
    { (yyval.etree) = exp_nameop (DEFINED, (yyvsp[(3) - (4)].name)); }
    break;

  case 221:
#line 834 "ldgram.y"
    { (yyval.etree) = exp_bigintop ((yyvsp[(1) - (1)].bigint).integer, (yyvsp[(1) - (1)].bigint).str); }
    break;

  case 222:
#line 836 "ldgram.y"
    { (yyval.etree) = exp_nameop (SIZEOF_HEADERS,0); }
    break;

  case 223:
#line 839 "ldgram.y"
    { (yyval.etree) = exp_nameop (SIZEOF,(yyvsp[(3) - (4)].name)); }
    break;

  case 224:
#line 841 "ldgram.y"
    { (yyval.etree) = exp_nameop (ADDR,(yyvsp[(3) - (4)].name)); }
    break;

  case 225:
#line 843 "ldgram.y"
    { (yyval.etree) = exp_nameop (LOADADDR,(yyvsp[(3) - (4)].name)); }
    break;

  case 226:
#line 845 "ldgram.y"
    { (yyval.etree) = exp_unop (ABSOLUTE, (yyvsp[(3) - (4)].etree)); }
    break;

  case 227:
#line 847 "ldgram.y"
    { (yyval.etree) = exp_unop (ALIGN_K,(yyvsp[(3) - (4)].etree)); }
    break;

  case 228:
#line 849 "ldgram.y"
    { (yyval.etree) = exp_binop (ALIGN_K,(yyvsp[(3) - (6)].etree),(yyvsp[(5) - (6)].etree)); }
    break;

  case 229:
#line 851 "ldgram.y"
    { (yyval.etree) = exp_binop (DATA_SEGMENT_ALIGN, (yyvsp[(3) - (6)].etree), (yyvsp[(5) - (6)].etree)); }
    break;

  case 230:
#line 853 "ldgram.y"
    { (yyval.etree) = exp_binop (DATA_SEGMENT_RELRO_END, (yyvsp[(5) - (6)].etree), (yyvsp[(3) - (6)].etree)); }
    break;

  case 231:
#line 855 "ldgram.y"
    { (yyval.etree) = exp_unop (DATA_SEGMENT_END, (yyvsp[(3) - (4)].etree)); }
    break;

  case 232:
#line 857 "ldgram.y"
    { /* The operands to the expression node are
			     placed in the opposite order from the way
			     in which they appear in the script as
			     that allows us to reuse more code in
			     fold_binary.  */
			  (yyval.etree) = exp_binop (SEGMENT_START,
					  (yyvsp[(5) - (6)].etree),
					  exp_nameop (NAME, (yyvsp[(3) - (6)].name))); }
    break;

  case 233:
#line 866 "ldgram.y"
    { (yyval.etree) = exp_unop (ALIGN_K,(yyvsp[(3) - (4)].etree)); }
    break;

  case 234:
#line 868 "ldgram.y"
    { (yyval.etree) = exp_nameop (NAME,(yyvsp[(1) - (1)].name)); }
    break;

  case 235:
#line 870 "ldgram.y"
    { (yyval.etree) = exp_binop (MAX_K, (yyvsp[(3) - (6)].etree), (yyvsp[(5) - (6)].etree) ); }
    break;

  case 236:
#line 872 "ldgram.y"
    { (yyval.etree) = exp_binop (MIN_K, (yyvsp[(3) - (6)].etree), (yyvsp[(5) - (6)].etree) ); }
    break;

  case 237:
#line 874 "ldgram.y"
    { (yyval.etree) = exp_assert ((yyvsp[(3) - (6)].etree), (yyvsp[(5) - (6)].name)); }
    break;

  case 238:
#line 876 "ldgram.y"
    { (yyval.etree) = exp_nameop (ORIGIN, (yyvsp[(3) - (4)].name)); }
    break;

  case 239:
#line 878 "ldgram.y"
    { (yyval.etree) = exp_nameop (LENGTH, (yyvsp[(3) - (4)].name)); }
    break;

  case 240:
#line 883 "ldgram.y"
    { (yyval.name) = (yyvsp[(3) - (3)].name); }
    break;

  case 241:
#line 884 "ldgram.y"
    { (yyval.name) = 0; }
    break;

  case 242:
#line 888 "ldgram.y"
    { (yyval.etree) = (yyvsp[(3) - (4)].etree); }
    break;

  case 243:
#line 889 "ldgram.y"
    { (yyval.etree) = 0; }
    break;

  case 244:
#line 893 "ldgram.y"
    { (yyval.etree) = (yyvsp[(3) - (4)].etree); }
    break;

  case 245:
#line 894 "ldgram.y"
    { (yyval.etree) = 0; }
    break;

  case 246:
#line 898 "ldgram.y"
    { (yyval.etree) = (yyvsp[(3) - (4)].etree); }
    break;

  case 247:
#line 899 "ldgram.y"
    { (yyval.etree) = 0; }
    break;

  case 248:
#line 903 "ldgram.y"
    { (yyval.token) = ONLY_IF_RO; }
    break;

  case 249:
#line 904 "ldgram.y"
    { (yyval.token) = ONLY_IF_RW; }
    break;

  case 250:
#line 905 "ldgram.y"
    { (yyval.token) = ONLY_IF_SPUGUID; }
    break;

  case 251:
#line 906 "ldgram.y"
    { (yyval.token) = SPECIAL; }
    break;

  case 252:
#line 907 "ldgram.y"
    { (yyval.token) = 0; }
    break;

  case 253:
#line 910 "ldgram.y"
    { ldlex_expression(); }
    break;

  case 254:
#line 914 "ldgram.y"
    { ldlex_popstate (); ldlex_script (); }
    break;

  case 255:
#line 917 "ldgram.y"
    {
			  lang_enter_output_section_statement((yyvsp[(1) - (9)].name), (yyvsp[(3) - (9)].etree),
							      sectype,
							      (yyvsp[(5) - (9)].etree), (yyvsp[(6) - (9)].etree), (yyvsp[(4) - (9)].etree), (yyvsp[(8) - (9)].token));
			}
    break;

  case 256:
#line 923 "ldgram.y"
    { ldlex_popstate (); ldlex_expression (); }
    break;

  case 257:
#line 925 "ldgram.y"
    {
		  ldlex_popstate ();
		  lang_leave_output_section_statement ((yyvsp[(17) - (17)].fill), (yyvsp[(14) - (17)].name), (yyvsp[(16) - (17)].section_phdr), (yyvsp[(15) - (17)].name));
		}
    break;

  case 258:
#line 930 "ldgram.y"
    {}
    break;

  case 259:
#line 932 "ldgram.y"
    { ldlex_expression (); }
    break;

  case 260:
#line 934 "ldgram.y"
    { ldlex_popstate (); ldlex_script (); }
    break;

  case 261:
#line 936 "ldgram.y"
    {
			  lang_enter_overlay ((yyvsp[(3) - (8)].etree), (yyvsp[(6) - (8)].etree));
			}
    break;

  case 262:
#line 941 "ldgram.y"
    { ldlex_popstate (); ldlex_expression (); }
    break;

  case 263:
#line 943 "ldgram.y"
    {
			  ldlex_popstate ();
			  lang_leave_overlay ((yyvsp[(5) - (16)].etree), (int) (yyvsp[(4) - (16)].integer),
					      (yyvsp[(16) - (16)].fill), (yyvsp[(13) - (16)].name), (yyvsp[(15) - (16)].section_phdr), (yyvsp[(14) - (16)].name));
			}
    break;

  case 265:
#line 953 "ldgram.y"
    { ldlex_expression (); }
    break;

  case 266:
#line 955 "ldgram.y"
    {
		  ldlex_popstate ();
		  lang_add_assignment (exp_assop ('=', ".", (yyvsp[(3) - (3)].etree)));
		}
    break;

  case 268:
#line 963 "ldgram.y"
    { sectype = noload_section; }
    break;

  case 269:
#line 964 "ldgram.y"
    { sectype = dsect_section; }
    break;

  case 270:
#line 965 "ldgram.y"
    { sectype = copy_section; }
    break;

  case 271:
#line 966 "ldgram.y"
    { sectype = info_section; }
    break;

  case 272:
#line 967 "ldgram.y"
    { sectype = overlay_section; }
    break;

  case 274:
#line 972 "ldgram.y"
    { sectype = normal_section; }
    break;

  case 275:
#line 973 "ldgram.y"
    { sectype = normal_section; }
    break;

  case 276:
#line 977 "ldgram.y"
    { (yyval.etree) = (yyvsp[(1) - (3)].etree); }
    break;

  case 277:
#line 978 "ldgram.y"
    { (yyval.etree) = (etree_type *)NULL;  }
    break;

  case 278:
#line 983 "ldgram.y"
    { (yyval.etree) = (yyvsp[(3) - (6)].etree); }
    break;

  case 279:
#line 985 "ldgram.y"
    { (yyval.etree) = (yyvsp[(3) - (10)].etree); }
    break;

  case 280:
#line 989 "ldgram.y"
    { (yyval.etree) = (yyvsp[(1) - (2)].etree); }
    break;

  case 281:
#line 990 "ldgram.y"
    { (yyval.etree) = (etree_type *) NULL;  }
    break;

  case 282:
#line 995 "ldgram.y"
    { (yyval.integer) = 0; }
    break;

  case 283:
#line 997 "ldgram.y"
    { (yyval.integer) = 1; }
    break;

  case 284:
#line 1002 "ldgram.y"
    { (yyval.name) = (yyvsp[(2) - (2)].name); }
    break;

  case 285:
#line 1003 "ldgram.y"
    { (yyval.name) = DEFAULT_MEMORY_REGION; }
    break;

  case 286:
#line 1008 "ldgram.y"
    {
		  (yyval.section_phdr) = NULL;
		}
    break;

  case 287:
#line 1012 "ldgram.y"
    {
		  struct lang_output_section_phdr_list *n;

		  n = ((struct lang_output_section_phdr_list *)
		       xmalloc (sizeof *n));
		  n->name = (yyvsp[(3) - (3)].name);
		  n->used = FALSE;
		  n->next = (yyvsp[(1) - (3)].section_phdr);
		  (yyval.section_phdr) = n;
		}
    break;

  case 289:
#line 1028 "ldgram.y"
    {
			  ldlex_script ();
			  lang_enter_overlay_section ((yyvsp[(2) - (2)].name));
			}
    break;

  case 290:
#line 1033 "ldgram.y"
    { ldlex_popstate (); ldlex_expression (); }
    break;

  case 291:
#line 1035 "ldgram.y"
    {
			  ldlex_popstate ();
			  lang_leave_overlay_section ((yyvsp[(9) - (9)].fill), (yyvsp[(8) - (9)].section_phdr));
			}
    break;

  case 296:
#line 1052 "ldgram.y"
    { ldlex_expression (); }
    break;

  case 297:
#line 1053 "ldgram.y"
    { ldlex_popstate (); }
    break;

  case 298:
#line 1055 "ldgram.y"
    {
		  lang_new_phdr ((yyvsp[(1) - (6)].name), (yyvsp[(3) - (6)].etree), (yyvsp[(4) - (6)].phdr).filehdr, (yyvsp[(4) - (6)].phdr).phdrs, (yyvsp[(4) - (6)].phdr).at,
				 (yyvsp[(4) - (6)].phdr).flags);
		}
    break;

  case 299:
#line 1063 "ldgram.y"
    {
		  (yyval.etree) = (yyvsp[(1) - (1)].etree);

		  if ((yyvsp[(1) - (1)].etree)->type.node_class == etree_name
		      && (yyvsp[(1) - (1)].etree)->type.node_code == NAME)
		    {
		      const char *s;
		      unsigned int i;
		      static const char * const phdr_types[] =
			{
			  "PT_NULL", "PT_LOAD", "PT_DYNAMIC",
			  "PT_INTERP", "PT_NOTE", "PT_SHLIB",
			  "PT_PHDR", "PT_TLS"
			};

		      s = (yyvsp[(1) - (1)].etree)->name.name;
		      for (i = 0;
			   i < sizeof phdr_types / sizeof phdr_types[0];
			   i++)
			if (strcmp (s, phdr_types[i]) == 0)
			  {
			    (yyval.etree) = exp_intop (i);
			    break;
			  }
#if (defined(BPA))
		      if (strcmp(s, "PT_SPU_INFO") == 0)
			(yyval.etree) = exp_intop(0x70000000);
#endif

		      if (i == sizeof phdr_types / sizeof phdr_types[0])
			{
			  if (strcmp (s, "PT_GNU_EH_FRAME") == 0)
			    (yyval.etree) = exp_intop (0x6474e550);
			  else if (strcmp (s, "PT_GNU_STACK") == 0)
			    (yyval.etree) = exp_intop (0x6474e551);
			  else
			    {
			      einfo (_("\
%X%P:%S: unknown phdr type `%s' (try integer literal)\n"),
				     s);
			      (yyval.etree) = exp_intop (0);
			    }
			}
		    }
		}
    break;

  case 300:
#line 1112 "ldgram.y"
    {
		  memset (&(yyval.phdr), 0, sizeof (struct phdr_info));
		}
    break;

  case 301:
#line 1116 "ldgram.y"
    {
		  (yyval.phdr) = (yyvsp[(3) - (3)].phdr);
		  if (strcmp ((yyvsp[(1) - (3)].name), "FILEHDR") == 0 && (yyvsp[(2) - (3)].etree) == NULL)
		    (yyval.phdr).filehdr = TRUE;
		  else if (strcmp ((yyvsp[(1) - (3)].name), "PHDRS") == 0 && (yyvsp[(2) - (3)].etree) == NULL)
		    (yyval.phdr).phdrs = TRUE;
		  else if (strcmp ((yyvsp[(1) - (3)].name), "FLAGS") == 0 && (yyvsp[(2) - (3)].etree) != NULL)
		    (yyval.phdr).flags = (yyvsp[(2) - (3)].etree);
		  else
		    einfo (_("%X%P:%S: PHDRS syntax error at `%s'\n"), (yyvsp[(1) - (3)].name));
		}
    break;

  case 302:
#line 1128 "ldgram.y"
    {
		  (yyval.phdr) = (yyvsp[(5) - (5)].phdr);
		  (yyval.phdr).at = (yyvsp[(3) - (5)].etree);
		}
    break;

  case 303:
#line 1136 "ldgram.y"
    {
		  (yyval.etree) = NULL;
		}
    break;

  case 304:
#line 1140 "ldgram.y"
    {
		  (yyval.etree) = (yyvsp[(2) - (3)].etree);
		}
    break;

  case 305:
#line 1148 "ldgram.y"
    {
		  ldlex_version_file ();
		  PUSH_ERROR (_("VERSION script"));
		}
    break;

  case 306:
#line 1153 "ldgram.y"
    {
		  ldlex_popstate ();
		  POP_ERROR ();
		}
    break;

  case 307:
#line 1162 "ldgram.y"
    {
		  ldlex_version_script ();
		}
    break;

  case 308:
#line 1166 "ldgram.y"
    {
		  ldlex_popstate ();
		}
    break;

  case 311:
#line 1178 "ldgram.y"
    {
		  lang_register_vers_node (NULL, (yyvsp[(2) - (4)].versnode), NULL);
		}
    break;

  case 312:
#line 1182 "ldgram.y"
    {
		  lang_register_vers_node ((yyvsp[(1) - (5)].name), (yyvsp[(3) - (5)].versnode), NULL);
		}
    break;

  case 313:
#line 1186 "ldgram.y"
    {
		  lang_register_vers_node ((yyvsp[(1) - (6)].name), (yyvsp[(3) - (6)].versnode), (yyvsp[(5) - (6)].deflist));
		}
    break;

  case 314:
#line 1193 "ldgram.y"
    {
		  (yyval.deflist) = lang_add_vers_depend (NULL, (yyvsp[(1) - (1)].name));
		}
    break;

  case 315:
#line 1197 "ldgram.y"
    {
		  (yyval.deflist) = lang_add_vers_depend ((yyvsp[(1) - (2)].deflist), (yyvsp[(2) - (2)].name));
		}
    break;

  case 316:
#line 1204 "ldgram.y"
    {
		  (yyval.versnode) = lang_new_vers_node (NULL, NULL);
		}
    break;

  case 317:
#line 1208 "ldgram.y"
    {
		  (yyval.versnode) = lang_new_vers_node ((yyvsp[(1) - (2)].versyms), NULL);
		}
    break;

  case 318:
#line 1212 "ldgram.y"
    {
		  (yyval.versnode) = lang_new_vers_node ((yyvsp[(3) - (4)].versyms), NULL);
		}
    break;

  case 319:
#line 1216 "ldgram.y"
    {
		  (yyval.versnode) = lang_new_vers_node (NULL, (yyvsp[(3) - (4)].versyms));
		}
    break;

  case 320:
#line 1220 "ldgram.y"
    {
		  (yyval.versnode) = lang_new_vers_node ((yyvsp[(3) - (8)].versyms), (yyvsp[(7) - (8)].versyms));
		}
    break;

  case 321:
#line 1227 "ldgram.y"
    {
		  (yyval.versyms) = lang_new_vers_pattern (NULL, (yyvsp[(1) - (1)].name), ldgram_vers_current_lang, FALSE);
		}
    break;

  case 322:
#line 1231 "ldgram.y"
    {
		  (yyval.versyms) = lang_new_vers_pattern (NULL, (yyvsp[(1) - (1)].name), ldgram_vers_current_lang, TRUE);
		}
    break;

  case 323:
#line 1235 "ldgram.y"
    {
		  (yyval.versyms) = lang_new_vers_pattern ((yyvsp[(1) - (3)].versyms), (yyvsp[(3) - (3)].name), ldgram_vers_current_lang, FALSE);
		}
    break;

  case 324:
#line 1239 "ldgram.y"
    {
		  (yyval.versyms) = lang_new_vers_pattern ((yyvsp[(1) - (3)].versyms), (yyvsp[(3) - (3)].name), ldgram_vers_current_lang, TRUE);
		}
    break;

  case 325:
#line 1243 "ldgram.y"
    {
			  (yyval.name) = ldgram_vers_current_lang;
			  ldgram_vers_current_lang = (yyvsp[(4) - (5)].name);
			}
    break;

  case 326:
#line 1248 "ldgram.y"
    {
			  struct bfd_elf_version_expr *pat;
			  for (pat = (yyvsp[(7) - (9)].versyms); pat->next != NULL; pat = pat->next);
			  pat->next = (yyvsp[(1) - (9)].versyms);
			  (yyval.versyms) = (yyvsp[(7) - (9)].versyms);
			  ldgram_vers_current_lang = (yyvsp[(6) - (9)].name);
			}
    break;

  case 327:
#line 1256 "ldgram.y"
    {
			  (yyval.name) = ldgram_vers_current_lang;
			  ldgram_vers_current_lang = (yyvsp[(2) - (3)].name);
			}
    break;

  case 328:
#line 1261 "ldgram.y"
    {
			  (yyval.versyms) = (yyvsp[(5) - (7)].versyms);
			  ldgram_vers_current_lang = (yyvsp[(4) - (7)].name);
			}
    break;

  case 329:
#line 1266 "ldgram.y"
    {
		  (yyval.versyms) = lang_new_vers_pattern (NULL, "global", ldgram_vers_current_lang, FALSE);
		}
    break;

  case 330:
#line 1270 "ldgram.y"
    {
		  (yyval.versyms) = lang_new_vers_pattern ((yyvsp[(1) - (3)].versyms), "global", ldgram_vers_current_lang, FALSE);
		}
    break;

  case 331:
#line 1274 "ldgram.y"
    {
		  (yyval.versyms) = lang_new_vers_pattern (NULL, "local", ldgram_vers_current_lang, FALSE);
		}
    break;

  case 332:
#line 1278 "ldgram.y"
    {
		  (yyval.versyms) = lang_new_vers_pattern ((yyvsp[(1) - (3)].versyms), "local", ldgram_vers_current_lang, FALSE);
		}
    break;

  case 333:
#line 1282 "ldgram.y"
    {
		  (yyval.versyms) = lang_new_vers_pattern (NULL, "extern", ldgram_vers_current_lang, FALSE);
		}
    break;

  case 334:
#line 1286 "ldgram.y"
    {
		  (yyval.versyms) = lang_new_vers_pattern ((yyvsp[(1) - (3)].versyms), "extern", ldgram_vers_current_lang, FALSE);
		}
    break;


/* Line 1267 of yacc.c.  */
#line 4132 "ldgram.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
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
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
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


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

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
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
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

