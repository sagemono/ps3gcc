/*

@deftypefn Replacement {char*} ldirseparator (const char *@var{name})

@end deftypefn

*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "ansidecl.h"
#include "libiberty.h"
#include "safe-ctype.h"
#include "filenames.h"

char *
ldirseparator (const char *name)
{
  char *lsep = NULL;

  do {
    if (IS_DIR_SEPARATOR (*name))
      lsep = (char *)name;
  } while (*name++);

  return lsep;
}
