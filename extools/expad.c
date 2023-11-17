#include <string.h>

char *expad(flen,fchar)
int flen,fchar;
{
  static char wbuf[256];

  memset(wbuf,fchar,flen);

  wbuf[flen] = 0;		/* flag the end of string */

  return(wbuf);
}
