#include <stdio.h>
#include <ints.h>
#include smgdef
#include smg$routines


#include "extools.h"
#include "screen.h"

int rms_status;

   Xinit_terminal ()
   {
       int status;
        status = smg$create_pasteboard (&PasteBoard);

        status = smg$create_virtual_display(& 50, &120,&Standard_Window);

        status = smg$paste_virtual_display (&Standard_Window,&PasteBoard,& 1,& 1);
  /*      Standard_Window := display_id;  */

        status = smg$create_virtual_keyboard(&KeyBoard);  /***, "SYS$INPUT:" ); */
  /*      KeyBoard := keyboard_id;        */

   }


   Xclose_terminal  ()
   {
      int status;
      status = smg$delete_virtual_display (Standard_Window);
      status = smg$delete_pasteboard (PasteBoard);
   }

   void XRefresh(win)
      unsigned int win;
   {
      int status;
      status = smg$end_display_update(&win);
      status = smg$begin_display_update(&win);

   }


main ()
{
   uint16 inchr,scan;
   int status;
   char *outstr = "This is a Test of the New Scrprn"; 
   struct line_rec *start_line = NULL;
   int retcode,edited;

   Xinit_terminal();
   init_windows();

   open_window(10,2,60,10,0,border);

 texaco (&start_line,0,9,&retcode,&edited,100,60,linedt,INSERT_MODE | WORD_WRAP,NULL);
/*
struct line_rec **crnt_list;
int t_line,b_line,*retcode,*edited,lines_limit,right_margin;
int (*line_ed)();
unsigned int cont;
struct cmndline_params cmndline[];
*/

   close_window();





   status = exscrprn(outstr,1,1,0);
   XRefresh(crnt_window->display_id);

   inchr = exinkey(&scan);


   printf("%i %i",inchr,scan);

   Xclose_terminal;

}

