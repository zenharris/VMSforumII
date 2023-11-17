#include ints

#include smgdef
#include smg$routines


#include "extools.h"

exclreol(frow,fcol,fattr)
int frow,fcol,fattr;
{
   int status;
/*  char fill[81];

  memset(fill,' ',crnt_window->width-fcol);

  fill[crnt_window->width-fcol] = 0;    /* mark end of string */
/*
  exscrprn(fill,frow,fcol,crnt_window->attr);
*/
   frow++;
   fcol++;

   status = smg$erase_line(&crnt_window->display_id /*&Standard_Window*/,&frow,&fcol);
}

