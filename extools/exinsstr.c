#include ints
#include "extools.h"
#include <stdlib.h>
#include string

exinsstr(fdestin,fsource,fstart)
char *fdestin;
char *fsource;
int fstart;
{

  int wdlen,wslen;
  int w,wlen,wpad;

  wdlen = strlen(fdestin);	/* get destination string length */
  wslen = strlen(fsource);	/* get source string length */

  wlen = min(fstart,wdlen);	/* don't initially point past destin */

  wpad = fstart - wlen; 	/* get extra length to pad */

  for (w = wlen ; w < fstart ; w ++)	/* pad with blanks if neccessary */
    fdestin[w] = ' ';

  /* start at end of string and move characters to the right */
  /* draw it out if necessary; It's hard to follow if you don't */

  for (w = wdlen + wslen - 1 ; w >= fstart + wslen ; w --)
    fdestin[w] = fdestin[w - wslen];

  for (w = 0 ; w < wslen ; w ++)	/* now insert into the dest string */
    fdestin[w+fstart] = *fsource++;

  fdestin[wslen+wdlen+wpad] = 0;	/* string is bigger, needs NULL */
}

