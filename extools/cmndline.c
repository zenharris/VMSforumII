
#include <stdio.h>
#include <stdlib.h>
/* #include <sets.h>     */
#include <string.h>


#include smgdef
#include smg$routines
#include ints
#include rms

#include "extools.h"
#include "filebrowse.h"
#include "warns"


#define TOGGLE (1<<15)

int cmndline_colour =  0;  /*WHITE;  /*(BROWN << 4) | BLACK;*/
int cmndline_inverse = SMG$M_UNDERLINE;      /*RED;   */

int text_hilite = SMG$M_BOLD;
int text_colour = 0;


extern int StatusLine;
extern int helpLine;
extern int TermLength;

void Rfresh(win)
      unsigned int win;
   {
      int status;
      status = smg$end_display_update(&win);
      status = smg$begin_display_update(&win);

   }
/*
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
*/

int cmd_menu(tbl,tbl_lnth,rul,cul,rlr,clr,norm,hilite)
struct menu_params tbl[];
int tbl_lnth;
int rul,cul,rlr,clr;
int norm,hilite;
{
    uint16 scan,c;
    int crnt_line = rul,iter;
    for (iter = 0;iter < tbl_lnth && (iter + crnt_line) <= rlr;iter++){
        exscrprn(tbl[iter].prompt,iter + crnt_line,cul,norm);
    }
    iter = 0;
    exscrprn(tbl[iter].prompt,crnt_line,cul,hilite);
    for (;;) {
        Refresh(crnt_window->display_id);
        c = exinkey(&scan);
        switch(c) {
            case 00 : switch(scan) {
                case SMG$K_TRM_DOWN : if (iter < tbl_lnth - 1) {
                               exscrprn(tbl[iter++].prompt,crnt_line++,cul,norm);
                               if (crnt_line > rlr) {
                                /*  exscrlup((rul << 8) + cul,(rlr << 8) + clr,1,norm); */
                                  crnt_line = rlr;
                               }
                               exscrprn(tbl[iter].prompt,crnt_line,cul,hilite);
                            }
                            break;
                case SMG$K_TRM_UP : if (iter > 0) {
                               exscrprn(tbl[iter--].prompt,crnt_line--,cul,norm);
                               if (crnt_line < rul) {
                                 /* exscrldn((rul << 8) + cul,(rlr << 8) + clr,1,norm); */
                                  crnt_line = rul;
                               }
                               exscrprn(tbl[iter].prompt,crnt_line,cul,hilite);
                            }
                            break;
                default  : return(scan);
            }
            break;
            case CR : return(iter/*|TOGGLE*/);
            case SMG$K_TRM_CTRLZ : return (ESC);
        }
    }
}

#define exscrprn(str,row,col,attr) exscrprn(str,row,col,attr)

void title_line(char *title)
{
	char blank[81];
	memset(blank,' ',80);
	blank[79] = 0;
	exscrprn(blank,2,0,WHITE);
 exscrprn(excntstr(title,80),2,0,WHITE);
}

/* defset(funct_keys,CHARACTER,256,0);  */


int command_line (cmd_pack,scan,message,filedef)
struct cmndline_params cmd_pack[];
uint16 *scan;
void *message;
struct filestruct *filedef;
{
   char cmn_line1[85],cmn_line2[85],blank[15] = "          ",out_buff[20];
   int fkey_index[10],x,y,menu_width,box_pos,ret,/*ret2,*/ret3,ret4,buff_row,buff_col;
   uint16 ret2;
   int funct = FALSE,local_keystroke = TRUE;
   int cmd_lnth = (cmd_pack != NULL) ? cmd_pack[0].fkey : 0;
   struct cmndline_params **cmd_buff;
   static int once = FALSE;
   char outstr[128];
   unsigned long row,col;
   static void *save_ptr = NULL;



   if (/*cmd_pack == NULL ||*/ cmd_lnth == 0xff) {
      save_ptr = NULL;
      return (exinkey(scan));
   }

	cmd_pack++;
   if (!once)
      {
     /* if (set_of(&funct_keys,F1 __ F10,_E)==FAILURE)
         {
         gen_error("Failure to populate Set");
         exit(99);
         }
      */
      once = TRUE;
      }
   memset(fkey_index,0,sizeof(fkey_index));
   for (x = 0;x < cmd_lnth;x++) fkey_index[cmd_pack[x].fkey - F1] = x+1;
   sprintf(cmn_line1,"[F1]%-11s [F2]%-11s [F3]%-11s [F4]%-11s [F5]%-12s\0",
                  (!cmd_lnth || fkey_index[0] == 0) ? blank : cmd_pack[fkey_index[0]-1].prompt,
                  (!cmd_lnth || fkey_index[1] == 0) ? blank : cmd_pack[fkey_index[1]-1].prompt,
                  (!cmd_lnth || fkey_index[2] == 0) ? blank : cmd_pack[fkey_index[2]-1].prompt,
                  (!cmd_lnth || fkey_index[3] == 0) ? blank : cmd_pack[fkey_index[3]-1].prompt,
                  (!cmd_lnth || fkey_index[4] == 0) ? blank : cmd_pack[fkey_index[4]-1].prompt);
   sprintf(cmn_line2,"[F6]%-11s [F7]%-11s [F8]%-11s [F9]%-11s [F10]%-11s\0",
                  (!cmd_lnth || fkey_index[5] == 0) ? blank : cmd_pack[fkey_index[5]-1].prompt,
                  (!cmd_lnth || fkey_index[6] == 0) ? blank : cmd_pack[fkey_index[6]-1].prompt,
                  (!cmd_lnth || fkey_index[7] == 0) ? blank : cmd_pack[fkey_index[7]-1].prompt,
                  (!cmd_lnth || fkey_index[8] == 0) ? blank : cmd_pack[fkey_index[8]-1].prompt,
                  (!cmd_lnth || fkey_index[9] == 0) ? blank : cmd_pack[fkey_index[9]-1].prompt);

	if (cmd_lnth == 0) {
           if (save_ptr != (void *)cmd_pack) {
                /*ZEN exscrprn(cmn_line1,TermLength-2,0,cmndline_inverse); */
                Status(cmn_line1);
  	        /* exscrprn(cmn_line2,TermLength-1,0,cmndline_inverse); */
		save_ptr = (void *)cmd_pack;
           }
           return (exinkey(scan));
        }
	if (save_ptr != (void *)cmd_pack) {
           /*ZEN exscrprn(cmn_line1,TermLength-2,0,cmndline_colour);*/
           Status(cmn_line1);
           /* exscrprn(cmn_line2,TermLength-1,0,cmndline_colour); */
	   save_ptr = (void *)cmd_pack;
	}


   do {
/*      CLEARKBD*/

/* ??????     Rfresh(Standard_Window);   */

      if (!funct) ret = exinkey(&ret2);
      funct = FALSE;
      if (ret == 0) {
         if (ret2 >=SMG$K_TRM_PF1 && ret2 <= SMG$K_TRM_PF4 /*in_set(&funct_keys,ret2)*/) {
            sprintf (out_buff,"[F%d]%-11s\0",(ret2 -SMG$K_TRM_PF1/* F1*/)+1,
                     (fkey_index[ret2-SMG$K_TRM_PF1/*F1*/]==0)? blank :
                                               cmd_pack[fkey_index[ret2 -SMG$K_TRM_PF1/* F1*/]-1].prompt);
            buff_row = (ret2 > SMG$K_TRM_PF4 /*F5*/) ? 24 : TermLength-2 /*23*/;
            buff_col = ((ret2 > SMG$K_TRM_PF4/* F5*/) ? ret2-F6 : ret2-SMG$K_TRM_PF1/*F1*/) * 16;
/* ZEN EDIT            exscrprn(out_buff,buff_row,buff_col,SMG$M_UNDERLINE); */

                       /* Execute Keystroke Routine */
            if (!((x = fkey_index[ret2 - SMG$K_TRM_PF1/*F1*/] - 1) < 0)) {
               if (cmd_pack[x].fn_ptr != NULL){
						  cmd_buff = message;
						  (*cmd_buff) = &cmd_pack[x];
/* ZEN change  */				  (*cmd_pack[x].fn_ptr)(message,filedef);  /* message); */
                  local_keystroke = FALSE;
               }
               else
               if (cmd_pack[x].menu != NULL) {



                  for (y=0,menu_width=0;y<cmd_pack[x].menu_lnth;y++)
                     if (strlen(cmd_pack[x].menu[y].prompt) > menu_width)
                        menu_width = strlen(cmd_pack[x].menu[y].prompt);
                  box_pos = max(0,(((((ret2 >SMG$K_TRM_PF4/* F5*/)?ret2-F6:ret2-SMG$K_TRM_PF1/*F1*/)*16)+8)-(menu_width/2)));

                  row = smg$cursor_row(&crnt_window->display_id);
                  col = smg$cursor_column(&crnt_window->display_id);


                  open_window(max(2,/*23*/TermLength-2 - (cmd_pack[x].menu_lnth+2)),
                              max(2,box_pos-max(0,(box_pos+menu_width+2)-80)),
                              min(80,menu_width/*+2*/),
                              min(23,cmd_pack[x].menu_lnth+2),text_colour,border);
                  exscrprn(cmd_pack[x].prompt,crnt_window->rul-1,crnt_window->cul,cmndline_inverse);
         Rfresh(crnt_window->display_id/*Standard_Window*/);
                  ret3 = cmd_menu(cmd_pack[x].menu,cmd_pack[x].menu_lnth,
                                  0,0,min(20,cmd_pack[x].menu_lnth-1),min(77,menu_width-1),
                                  text_colour,text_hilite);
                  close_window();

   smg$set_cursor_abs(&crnt_window->display_id,&row,&col);
   Refresh(crnt_window->display_id);


                  if (ret3 >= 0 && (ret3/*&(~TOGGLE)*/) < cmd_pack[x].menu_lnth){
                     if (cmd_pack[x].menu[ret3/*&(~TOGGLE)*/].fn_ptr != NULL)

/* Execute Menu Opt*/(*cmd_pack[x].menu[ret3/*&(~TOGGLE)*/].fn_ptr)(message,filedef);
                     
                     if (((struct scrllst_msg_rec *)message)->breaker) local_keystroke = FALSE;
                     
                     ret2 = 0;
                     local_keystroke = FALSE;

                  }/* else if (ret3 == ESC){
                     counter = F1 - 1;
                  }*/ else if (ret3 >= SMG$K_TRM_PF1 && ret3 <= SMG$K_TRM_PF4 /*in_set(&funct_keys,ret3)*/) {
                     funct = TRUE;
                     ret2 = ret3;
                  }

               } else local_keystroke = FALSE;
            }
/*ZEN edit            exscrprn(out_buff,buff_row,buff_col,cmndline_colour);  */
         } else local_keystroke = FALSE;
      } else local_keystroke = FALSE;
   } while (local_keystroke);
   *scan = ret2;
   return(ret);
}



