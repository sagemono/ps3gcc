#include "version.h"

/* This is the string reported as the version number by all components
   of the compiler.  If you distribute a modified version of GCC,
   please modify this string to indicate that, e.g. by putting your
   organization's name in parentheses at the end of the string.  */

 /* CELL FIXME: configure does not like dots in anything other than
    the gcc version below. */
const char version_string[] = "4.0.2 (CELL 4.1.28, $Rev: 1849 $)";

/* This is the location of the online document giving instructions for
   reporting bugs.  If you distribute a modified version of GCC,
   please change this to refer to a document giving instructions for
   reporting bugs to you, not us.  (You are of course welcome to
   forward us bugs reported to you, if you determine that they are
   not bugs in your modifications.)  */

#if 0
const char bug_report_url[] = "<URL:https://rd.scea.com/bug/spu-gcc/>";
#else
const char bug_report_url[] = "<URL:https://ps3.scedev.net/>";
#endif
