#include ints
#include smgdef
#include smg$routines

#include "extools.h"

exloccur ( row , col)
int row,col;
{
  int status;
  row++;
  col++;
  status = smg$set_cursor_abs (&crnt_window->display_id/*&Standard_Window*/,&row,&col); 


}
