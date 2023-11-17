#include smgdef
#include smg$routines
#include ints
#include "extools.h"


exscrlup ( row , col , height , width )
int row,col,height,width;
{
  int status;
/*
  union REGS wreg;
  struct text_info info;
  gettextinfo(&info);

  wreg.h.ah = 6;
  wreg.h.al = fnum;
  wreg.x.cx = fuleft;
  wreg.h.ch += crnt_window->rul;
  wreg.h.cl += crnt_window->cul;
  wreg.x.dx = flrite;
  wreg.h.dh += crnt_window->rul;
  wreg.h.dl += crnt_window->cul;
  wreg.h.bh = fattr;
  int86(0x10,&wreg,&wreg);
*/
  row++;
  col++;
  status = smg$scroll_display_area (&crnt_window->display_id/*Standard_Window*/,&row,&col,&height,&width,&SMG$M_UP,&1); 


}
