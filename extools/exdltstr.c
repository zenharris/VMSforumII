#include ints
#include "extools.h"
#include <stdlib.h>
#include string

exdltstr(fdestin,fstart,fnum)
char *fdestin;
int fstart;
int fnum;
{

  int wdlen;                    /* hold length of destination string */
  char *p;                      /* temporary pointer to fstart+fnum  */
  int w;                        /* string counter */

  /* fstart is greater than string length */
  if (((wdlen = strlen(fdestin)) < fstart))
    return(0);

  fnum = min(fnum,wdlen-fstart);        /* don't delete past end of string */
  p = fdestin + fstart + fnum;          /* point to part of string to move */
  fdestin += fstart;                    /* point to part of string to delete*/

  for (w = 0 ; *fdestin++ = *p++ ; w ++)
     ;

  return(w);
}

