#include <stdlib.h>

#include <stdio.h>
/* #include <conio.h>   */

#include smgdef
#include smg$routines
#include ints

#include "extools.h"
#include "screen.h"

struct crnt_wind_rec *crnt_window = NULL;
struct crnt_wind_rec *root_window = NULL;

void destroy_windows(void)
{
   register void *tempw;
   while (crnt_window != NULL) {
      tempw = crnt_window;
      crnt_window = crnt_window->last;
      free(tempw);
   }
}

void init_windows(void)
{

   if (crnt_window == NULL){
      if ((crnt_window = malloc(sizeof(struct crnt_wind_rec))) != NULL){
/*         set_window (NULL,0,0,79,24,BLACK,80,25);*/
         crnt_window->display_id = Standard_Window;
         crnt_window->svscr = NULL;
         crnt_window->cul   = 0;
         crnt_window->rul   = 0;
         crnt_window->clr   = TermWidth-1; /* 79; */
         crnt_window->rlr   = TermLength-1; /* 24; */
         crnt_window->attr  = WHITE;
         crnt_window->width = TermWidth; /*80; */
         crnt_window->lnth  = TermLength; /* 25; */

         crnt_window->last = NULL;
/*         atexit (destroy_windows);*/
      }; /******** else gen_error("Not enough memory for windows"); *****/
   }
   if (root_window == NULL) root_window = crnt_window;
}

void *open_window(row,col,width,lnth,attr,type)
int row,col,width,lnth,attr;
enum win_type type;
{
   int status;
   void *svscr;
   struct crnt_wind_rec *tempw;
   if (crnt_window == NULL) init_windows();
/****   if ((svscr = malloc((width*lnth)*2)) == NULL ) return(NULL); */
   if ((tempw = malloc(sizeof(struct crnt_wind_rec))) == NULL){
/****      free(svscr);  ****/
      return(NULL);
   }
/****   savscrn(svscr,col,row,col+width-1,row+lnth-1);    ********/
   tempw->last = crnt_window;
   crnt_window = tempw;
   if (type == border) {
/*****      window(col+2,row+2,col+width-1,row+lnth-1);  ****************/

      status = smg$create_virtual_display(&lnth,&width,&crnt_window->display_id,&SMG$M_BORDER);
      status = smg$paste_virtual_display(&crnt_window->display_id,&PasteBoard,&row,&col);

/*   set_window(svscr,col+1,row+1,col+width-2,row+lnth-2,attr,width-2,lnth-2);*/
      {
          crnt_window->svscr = NULL; /*  svscr; */
          crnt_window->cul   = col; /*+1; */
          crnt_window->rul   = row; /*+1; */
          crnt_window->clr   = col+width-1; /* 2; */
          crnt_window->rlr   = row+lnth-1; /* 2; */
          crnt_window->attr  = attr;
          crnt_window->width = width; /*  - 2; */
          crnt_window->lnth  = lnth;  /*  - 2; */
          crnt_window->type  = type;
       }
/******       exdrwbox(row,col,width,lnth,border_colour); ******/
   } else if (type == borderless){
/*******      window(col+1,row+1,col+width,row+lnth);   ********/

      status = smg$create_virtual_display(&lnth,&width,&crnt_window->display_id,&SMG$M_BORDER);
      status = smg$paste_virtual_display(&crnt_window->display_id,&PasteBoard,&row,&col);



/*   set_window(svscr,col+1,row+1,col+width-2,row+lnth-2,attr,width-2,lnth-2);*/
      {
          crnt_window->svscr = NULL;  /****  svscr;    ***/
          crnt_window->cul   = col;
          crnt_window->rul   = row;
          crnt_window->clr   = col+width-1;
          crnt_window->rlr   = row+lnth-1;
          crnt_window->attr  = attr;
          crnt_window->width = width;
          crnt_window->lnth  = lnth;
          crnt_window->type  = type;
       }
   }
/*************   textattr(attr);
   cls(attr); **********/
   return(crnt_window);
}

void close_window(void)
{
   int status;
   struct crnt_wind_rec *tempw;
   if(crnt_window->last != NULL){
/*****      if (crnt_window->type == border)
         rstscrn(crnt_window->svscr,crnt_window->cul-1,crnt_window->rul-1,
                                 crnt_window->clr+1,crnt_window->rlr+1);
      else if (crnt_window->type == borderless)
         rstscrn(crnt_window->svscr,crnt_window->cul,crnt_window->rul,
                                 crnt_window->clr,crnt_window->rlr);


      free(crnt_window->svscr);
********/

      status = smg$delete_virtual_display(&crnt_window->display_id);
      Refresh(crnt_window->display_id);

      tempw = crnt_window;
      crnt_window = crnt_window->last;
      free(tempw);
/***
      window(crnt_window->cul+1,crnt_window->rul+1,
             crnt_window->clr+1,crnt_window->rlr+1);
      textattr(crnt_window->attr);
*****/
   }
}
