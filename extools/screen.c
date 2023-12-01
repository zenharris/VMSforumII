
#include smgdef
#include smg$routines
#include descrip
#include string
#include ints

#include ssdef

#define SCREEN_C
#include "screen.h"

#include "extools.h" 

void ServiceBroadcast()
{
   char message[128];
   int status;
   uint16 msgtype,lnth;
   $DESCRIPTOR(amessage,message);

   status = smg$get_broadcast_message(&PasteBoard,
                                      &amessage,
                                      &lnth, /*amessage.dsc$w_length,*/
                                      &msgtype);
   message[lnth] = 0;
   Status(message);
}


   void init_terminal (TermWidth,TermLength)
   int TermWidth,TermLength;
   {
       int status;
        status = smg$create_pasteboard (&PasteBoard);
        TermWidth++;
        TermLength++;
        status = smg$create_virtual_display(&TermLength, &TermWidth,&Standard_Window);

        status = smg$paste_virtual_display (&Standard_Window,&PasteBoard,& 1,& 1);
 
        status = smg$create_virtual_keyboard(&KeyBoard);

        status = smg$set_broadcast_trapping(&PasteBoard,ServiceBroadcast,0);
        if (!(status == SS$_NORMAL)) {
           Status("Set Broadcast Trap Failed");
        }


   }


   void close_terminal  ()
   {
      int status;
      status = smg$delete_virtual_display (&Standard_Window);

      status = smg$disable_broadcast_trapping(&PasteBoard);

      status = smg$delete_pasteboard (&PasteBoard);
   }

   int Move_Cursor (Win,Line,Column)
      unsigned int Win;
      unsigned int Line;
      unsigned int Column;
   {
        return smg$set_cursor_abs(&Win,&Line,&Column);
   }

   int set_cursor (Win,Line,Column)
      unsigned int Win;
      unsigned int Line;
      unsigned int Column;
   {
        return smg$set_cursor_abs(&Win,&Line,&Column);
   }

   int Add (win,Line, Column,str)
      unsigned int win;
      unsigned int Line;
      unsigned int Column;
      char *str;
   { 
      $DESCRIPTOR(aline,str);

      aline.dsc$w_length = strlen(str);
      return (smg$put_chars (&win , &aline , &Line,&Column,0,0,0,0));
   }

   int Add_Attr (win,Line, Column,str,Attr)
      unsigned int win;
      unsigned int Line;
      unsigned int Column;
      char *str;
      unsigned int Attr;
   { 
      $DESCRIPTOR(aline,str);

      aline.dsc$w_length = strlen(str);
      return (smg$put_chars (&win , &aline , &Line,&Column,0,&Attr,0,0));
   }

   int put_chars (win,str)
      unsigned int win;
      char *str;
   { 
      $DESCRIPTOR(aline,str);

      aline.dsc$w_length = strlen(str);
      return (smg$put_chars (&win , &aline,0,0,0,0,0,0));
   }
 
   int put_line (win,str)
      unsigned int win;
      char *str;
   { 
      $DESCRIPTOR(aline,str);

      aline.dsc$w_length = strlen(str);
      return (smg$put_line (&win , &aline,0,0,0,0,0,0));
   }


   unsigned int open_win(pasteboard_id,rows,cols,top_left_row,
                                         top_left_col)
       unsigned int pasteboard_id;
       unsigned int rows;
       unsigned int cols;
       unsigned int top_left_row;
       unsigned int top_left_col;
   {
       int status;
       unsigned int display_id;

       status = smg$create_virtual_display (&rows, &cols, &display_id,&SMG$M_BORDER);

       status = smg$paste_virtual_display (&display_id,&pasteboard_id, &top_left_row, &top_left_col);

       return (display_id);
   }

   void close_win (display_id)
      unsigned int display_id;
   {
      int status;
      
      status = smg$delete_virtual_display (&display_id);
   }
 

   void clreol(win)
      unsigned int win;
   {
      int status;
      status = smg$erase_line(&win,0,0);
   }

   void clreolpos(win,row,col)
      unsigned int win,row,col;
   {
      int status;
      status = smg$erase_line(&win,&row,&col);
   }


   void Refresh(win)
      unsigned int win;
   {
      int status;
      status = smg$end_display_update(&win);
      status = smg$begin_display_update(&win);

   }

   uint16 Get_Keystroke(void)
   {
      uint16 inchr;
      int status;
      status = smg$read_keystroke (&KeyBoard,&inchr);
      return inchr;
   }

   void Scroll_Up(unsigned int TopLine,unsigned int BottomLine)
   {
      int status, height = (BottomLine - TopLine)+1;
      status = smg$scroll_display_area (&crnt_window->display_id,
                                        &TopLine,&1,&height,&crnt_window->width/*TermWidth*/,&SMG$M_UP,&1);
      Refresh(crnt_window->display_id);
   }

   void Scroll_Down(unsigned int TopLine,unsigned int BottomLine)
   {
      int status, height = (BottomLine - TopLine)+1;
      status = smg$scroll_display_area (&crnt_window->display_id,
                                        &TopLine,&1,&height,&crnt_window->width/*TermWidth*/,&SMG$M_DOWN,&1);
      Refresh(crnt_window->display_id);
   }


   int Clear_Region(unsigned int TopLine,unsigned int BottomLine)
   {

      return smg$erase_display(&crnt_window->display_id,&TopLine,&1,&BottomLine,&crnt_window->width);
/*
      return smg$erase_display(&crnt_window->display_id,&TopLine,&1);
*/
/*      Refresh(crnt_window->display_id); */
   }




