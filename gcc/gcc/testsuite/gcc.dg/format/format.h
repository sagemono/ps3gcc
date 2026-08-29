/* Format checking tests: common header.  */
/* Origin: Joseph Myers <jsm28@cam.ac.uk> */

#include <stdarg.h>
#include <stddef.h>

#ifndef _WINT_T
#ifndef __WINT_TYPE__
#define __WINT_TYPE__ unsigned int
#endif
typedef __WINT_TYPE__ wint_t;
#endif

/* Kludges to get types corresponding to size_t and ptrdiff_t.  */
#define unsigned signed
typedef __SIZE_TYPE__ signed_size_t;
/* We also use this type to approximate ssize_t.  */
typedef __SIZE_TYPE__ ssize_t;
#undef unsigned
#define signed /* Type might or might not have explicit 'signed'.  */
typedef unsigned __PTRDIFF_TYPE__ unsigned_ptrdiff_t;
#undef signed

__extension__ typedef long long int llong;
__extension__ typedef unsigned long long int ullong;

/* %q formats want a "quad"; GCC considers this to be a long long.  */
typedef llong quad_t;
typedef ullong u_quad_t;

__extension__ typedef __INTMAX_TYPE__ intmax_t;
__extension__ typedef __UINTMAX_TYPE__ uintmax_t;

#if __STDC_VERSION__ < 199901L && !defined(restrict)
#define restrict /* "restrict" not in old C standard.  */
#endif

/* This may not be correct in the particular case, but allows the
   prototypes to be declared, and we don't try to link.
*/
typedef struct _FILE FILE;
extern FILE *stdin;
extern FILE *stdout;

extern int fprintf (FILE *restrict, const char *restrict, ...);
extern int printf (const char *restrict, ...);
extern int fprintf_unlocked (FILE *restrict, const char *restrict, ...);
extern int printf_unlocked (const char *restrict, ...);
extern int sprintf (char *restrict, const char *restrict, ...);
extern int vfprintf (FILE *restrict, const char *restrict, va_list);
extern int vprintf (const char *restrict, va_list);
extern int vsprintf (char *restrict, const char *restrict, va_list);
extern int snprintf (char *restrict, size_t, const char *restrict, ...);
extern int vsnprintf (char *restrict, size_t, const char *restrict, va_list);

extern int fscanf (FILE *restrict, const char *restrict, ...);
extern int scanf (const char *restrict, ...);
extern int sscanf (const char *restrict, const char *restrict, ...);
extern int vfscanf (FILE *restrict, const char *restrict, va_list);
extern int vscanf (const char *restrict, va_list);
extern int vsscanf (const char *restrict, const char *restrict, va_list);

extern int fwprintf (FILE *restrict, const wchar_t *restrict, ...);
extern int wprintf (const wchar_t *restrict, ...);
extern int swprintf (wchar_t *restrict, size_t, const wchar_t *restrict, ...);
extern int vfwprintf (FILE *restrict, const wchar_t *restrict, va_list);
extern int vwprintf (const wchar_t *restrict, va_list);
extern int vswprintf (wchar_t *restrict, size_t, const wchar_t *restrict,
		      va_list);

extern int fwscanf (FILE *restrict, const wchar_t *restrict, ...);
extern int wscanf (const wchar_t *restrict, ...);
extern int swscanf (const wchar_t *restrict, const wchar_t *restrict, ...);
extern int vfwscanf (FILE *restrict, const wchar_t *restrict, va_list);
extern int vwscanf (const wchar_t *restrict, va_list);
extern int vswscanf (const wchar_t *restrict, const wchar_t *restrict,
		     va_list);

extern char *gettext (const char *);
extern char *dgettext (const char *, const char *);
extern char *dcgettext (const char *, const char *, int);

struct tm;

extern size_t strftime (char *restrict, size_t, const char *restrict,
			const struct tm *restrict);
extern size_t wcsftime (wchar_t *restrict, size_t, const wchar_t *restrict,
			const struct tm *restrict);
extern ssize_t strfmon (char *restrict, size_t, const char *restrict, ...);
