#include <string.h>

char *excntstr(fstr,fwidth)
char *fstr;
int fwidth;
{
  static char wstr[256];	/* static work buffer */
  char *expad();
  strcpy(wstr,expad((fwidth-strlen(fstr)) >> 1,' '));

  strcat(wstr,fstr);

  return(wstr);
}


