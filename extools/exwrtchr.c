#include smgdef
#include smg$routines
#include descrip
#include string
#include ints
#include "extools.h"

int exwrtchr(outc,attr,outnum)
char outc;
int attr;
int outnum;
{
   int flags = 0;
   int row = 0,col = 0;
  char line[2] = {outc,0}; 
   $DESCRIPTOR(aline,line);

   aline.dsc$w_length = strlen(line);
   row++;
   col++;
   return (smg$put_chars (&crnt_window->display_id , &aline ,NULL,NULL,&flags,&attr));

}

