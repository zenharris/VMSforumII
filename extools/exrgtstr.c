#include ints
#include "extools.h"
#include string

char *exrgtstr(fstr,fnum)
char *fstr;
int fnum;
{
  unsigned int wlen;
  char *wptr,*w;

  w =  fstr;

  wlen = strlen(fstr);

  wptr =  w + wlen - fnum;

  return( ((wptr) >= (fstr)) ? wptr : fstr);

}
