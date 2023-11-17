#include <stdio.h>
#include smgdef
#include smg$routines
#include ints
#include string

#include "menu.h"
#include "screen.h"

/*  Moved to menu.h
#define MLENGTH 80
#define PLENGTH 80


typedef struct MenuRec
{
     char Prompt[PLENGTH];
     int (*Func)();
} MenuRec;


struct Menu_Record {
     char Prompt[PLENGTH];
     int (*Func)();
};

void Open_Menu (unsigned int,struct Menu_Record[],unsigned int);
*/


   menu$HiLite (Win,Menu_Array,Item_Num)
   int Win;
   struct Menu_Record Menu_Array[];
   int Item_Num;
   {
      Add_Attr (Win,
           Item_Num,
           1,
           Menu_Array[Item_Num-1].Prompt,
           SMG$M_BOLD);
      Refresh(Win);
      return(TRUE);
   }

   void menu$LoLite (Win, Menu_Array, Item_Num)
   int Win;
   struct Menu_Record Menu_Array[];
   int Item_Num;

   {
      Add (Win,
           Item_Num,
           1,
           Menu_Array[Item_Num-1].Prompt);
      Refresh(Win);
   }


   void Open_Menu (Function_Number,Menu_Array)
   unsigned int Function_Number;
   struct Menu_Record Menu_Array[];
   
   {
                       
      unsigned int menu_win;
      char Ch;
      int Menu_Length;
      int menu$Current_Line = 1,i;
      int exit = FALSE,width=0,plen;
      uint16 c;
      unsigned int Lines=TermLength,Columns=TermWidth;
      int window_closed = FALSE;

      for (Menu_Length=0;Menu_Array[Menu_Length].Func != NULL; Menu_Length++) {
         plen = strlen(Menu_Array[Menu_Length].Prompt);
         if (plen > width) width = plen;
      }

    
      menu_win = open_win(PasteBoard,
                             Menu_Length,
                             width,
                             Lines - Menu_Length,
                             ((Function_Number-1)*10)+2);
   

      for (i=0;i < Menu_Length; i++) {
         Add (menu_win,
              i+1,
              1,
              Menu_Array[i].Prompt);
      }
      Refresh(menu_win);
 

      while (!exit) {
         menu$HiLite(menu_win,Menu_Array,menu$Current_Line);
         c = Get_Keystroke();
         if ((c >= 1 && c <= 31) || (c >= 127 && c <= 328)) {
            switch( c ) {
            case SMG$K_TRM_DOWN:
               if (menu$Current_Line < Menu_Length) {
                  menu$LoLite(menu_win,Menu_Array,menu$Current_Line);
                  menu$Current_Line++;
               }
               break;
            case SMG$K_TRM_UP:
               if (menu$Current_Line > 1) {    /*  Menu_Array'First) then  */
                  menu$LoLite(menu_win,Menu_Array,menu$Current_Line);
                  menu$Current_Line--;
               }
               break;
            case SMG$K_TRM_CR:
            
                  close_win (menu_win);
                  window_closed = TRUE;

                  (*Menu_Array[menu$Current_Line-1].Func)();

                  exit = TRUE;;
               break;
            case SMG$K_TRM_CTRLZ:
                  menu$Current_Line = Menu_Length + 1;
                  exit = TRUE;
               break;

            default: exit = TRUE;
               break;
            } /*end switch */
         } else if (c>= 32 && c <= 126) {
/*
            Ch := Character'Val (c);

            case Ch is
            when others => null;
            end case;
*/
         }

      }
      if(!window_closed) close_win (menu_win);
    /*  return (Current_Line);  */
   }
   
