#include smgdef
#include smg$routines
#include descrip
#include string
#include ints
#include "extools.h"

int exscrprn(line,row,col,attr)
char *line;
int row,col,attr;
{
   int flags = 0; 
   $DESCRIPTOR(aline,line);

   aline.dsc$w_length = strlen(line);
   row++;
   col++;
   return (smg$put_chars (&crnt_window->display_id , &aline ,&row,&col,&flags,&attr));

}

