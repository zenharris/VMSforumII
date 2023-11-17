#include string

char *exlftstr(fstr,fnum)
char *fstr;
int fnum;
{
  static char wstr[256];

  strncpy(wstr,fstr,fnum);
  *(wstr+fnum) = 0;			/* put in the terminating byte */

  return(wstr);
}
