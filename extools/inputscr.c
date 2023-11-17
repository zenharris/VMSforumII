#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <float.h>
#include <math.h>
#include smgdef
#include smg$routines
#include ints
#include descrip

#include rms
#define INPUTSCR_C_
#include "extools.h"
#include "filebrowse.h"

extern struct filestruct *G_filedef;

extern struct line_rec *start_line;

#define RIGHT_MARGIN 78

init_screen(tbl,tbl_len)
struct scr_params tbl[];
int tbl_len;
{
    int iter,pcol;
    char println[81],*temp,fmtbuff[6];
    struct date_time dtbuff;
    struct date_only dbuff;

    for (iter = 0;iter < tbl_len;iter++) {
        if (tbl[iter].format & UNFMT_STRING) {
           exscrprn(tbl[iter].prompt,tbl[iter].row,tbl[iter].col,tbl[iter].prattr);
           exscrprn(tbl[iter].field,tbl[iter].row,tbl[iter].col + strlen(tbl[iter].prompt),tbl[iter].fattr);
        }else if (tbl[iter].format & DATE_ONLY || tbl[iter].format & BTRV_DATE) {
           exscrprn(tbl[iter].prompt,tbl[iter].row,tbl[iter].col,tbl[iter].prattr);
           /* if (!((tbl[iter].format & DATE_ONLY)?datenull(tbl[iter].field):btrv_datenull(tbl[iter].field))) {*/

            /*  if (tbl[iter].format & BTRV_DATE) bdtodo(&dbuff,tbl[iter].field);*/
            systodo (&dbuff,tbl[iter].field);
            /*  else memcpy(&dbuff,tbl[iter].field,sizeof(dbuff)); */
            /*  if ((getenv("KOMTDT") != NULL) && (strcmp(getenv("KOMTDT"),"USA") == 0))
                 sprintf (println,"%2.2d/%2.2d/%4.4d",dbuff.month,dbuff.day,dbuff.year);
              else */
            sprintf (println,"%2.2hu/%2.2hu/%4.4hu",dbuff.day,dbuff.month,dbuff.year);
            exscrprn(println,tbl[iter].row,tbl[iter].col + strlen(tbl[iter].prompt),tbl[iter].fattr);
           /* }  */
        }else if (tbl[iter].format & SINGLE_CHAR) {
           sprintf(println,tbl[iter].prompt/*,*(char *)tbl[iter].field*/);
           exscrprn(println,tbl[iter].row,tbl[iter].col,tbl[iter].prattr);
           sprintf(println,"%1c",*(char *)tbl[iter].field);
           exscrprn(println,tbl[iter].row,tbl[iter].col + strlen(tbl[iter].prompt) ,tbl[iter].fattr);
        }else if (tbl[iter].format & NUMERIC) {
           if ((temp = strchr(tbl[iter].prompt,'%')) != NULL){
              exgetpce(temp,fmtbuff,' ',1);
              pcol = tbl[iter].col + (index(tbl[iter].prompt,'%')-tbl[iter].prompt) ;
              switch (fmtbuff[strlen(fmtbuff) - 2]) {
                 case 'l':
                    switch (fmtbuff[strlen(fmtbuff)-1]) {
                       case 'd':
                       case 'i':
                       case 'u': /*     UNSIGNED LONG INTEGER   */
                          sprintf(println,tbl[iter].prompt,*(unsigned long *)tbl[iter].field);
                          exscrprn(println,tbl[iter].row,tbl[iter].col,tbl[iter].prattr);
                          sprintf(println,fmtbuff,*(unsigned long *)tbl[iter].field);
                          break;
                    }
                    break;
                 case 'h':
                    switch (fmtbuff[strlen(fmtbuff)-1]) {
                       case 'u':  /*   UNSIGNED BYTE    */
                          sprintf(println,tbl[iter].prompt,*(uint16 *)tbl[iter].field);
                          exscrprn(println,tbl[iter].row,tbl[iter].col,tbl[iter].prattr);
                          sprintf(println,fmtbuff,*(uint16 *)tbl[iter].field);
                          break;
                    }
                    break;
                 default :
                    switch (fmtbuff[strlen(fmtbuff)-1]) {
                       case 'd':
                       case 'i':
                       case 'u': /*        UNSIGNED PLAIN INTEGER    */
                          sprintf(println,tbl[iter].prompt,*(unsigned int *)tbl[iter].field);
                          exscrprn(println,tbl[iter].row,tbl[iter].col,tbl[iter].prattr);
                          sprintf(println,fmtbuff,*(unsigned int *)tbl[iter].field);
                          break;
                       case 'f': /*        FLOAT        */
                          sprintf(println,tbl[iter].prompt,*(float *)tbl[iter].field);
                          exscrprn(println,tbl[iter].row,tbl[iter].col,tbl[iter].prattr);
                          sprintf(println,fmtbuff,*(float *)tbl[iter].field);
                          break;
                    }
                    break;
              }
              exscrprn(println,tbl[iter].row,pcol,tbl[iter].fattr);
           }
        }else  if (tbl[iter].format & DATE_TIME || tbl[iter].format &  BTRV_DATE_TIME) {
           exscrprn(tbl[iter].prompt,tbl[iter].row,tbl[iter].col,tbl[iter].prattr);
           /* if (!((tbl[iter].format & DATE_TIME)?datetnull(tbl[iter].field):btrv_datetnull(tbl[iter].field))) {*/

/*              if (tbl[iter].format & BTRV_DATE_TIME) bdttodt(&dtbuff,tbl[iter].field);*/

              systodt (&dtbuff,tbl[iter].field);

/*              else memcpy(&dtbuff,tbl[iter].field,sizeof(struct date_time));
              if ((getenv("KOMTDT") != NULL) && (strcmp(getenv("KOMTDT"),"USA") == 0))
                 sprintf (println,"%2.2d/%2.2d/%2.2d %2.2d:%2.2d",
                                dtbuff.month,dtbuff.day,dtbuff.year,
                                dtbuff.hour,dtbuff.min);
              else
*/
                 sprintf (println,"%2.2hu/%2.2hu/%4.4hu %2.2hu:%2.2hu:%2.2hu.%2.2hu",
                                dtbuff.day,dtbuff.month,dtbuff.year,
                                dtbuff.hour,dtbuff.min,dtbuff.sec,dtbuff.hun);



              exscrprn(println,tbl[iter].row,tbl[iter].col + strlen(tbl[iter].prompt),tbl[iter].fattr);
/*           }   */
        }else  if (tbl[iter].format & FREE_TEXT) {
           exscrprn(excntstr(tbl[iter].prompt,78),tbl[iter].row - 1,0,tbl[iter].prattr);
           init_text(/*start_line*/tbl[iter].field,tbl[iter].row,tbl[iter].col,tbl[iter].validate);
        }else  if (tbl[iter].format == 0)
           exscrprn(tbl[iter].prompt,tbl[iter].row,tbl[iter].col,tbl[iter].prattr);
    }
}

/*
static int get_yn(void)
{
    int inchr;
    open_window (10,28,25,5,WHITE,border);
    gotoxy(1,1);
    cprintf ("Exit Enter/Edit Mode\n\r");
    cprintf ("Y/N  ? ");
    while(kbhit()) getch();
    do
       while (!isalpha(inchr = toupper(exinkey())));
    while (inchr != 'Y' && inchr != 'N');
    close_window();
    return (inchr == 'Y');
}
*/

void l_unpad(char *source,int lnth)
{
   int i;

   for (i = lnth-1;i>0 && source[i] == ' '; i--)
      source[i] = 0;
}

   
 

int enter_screen(tbl,tbl_len,edited,en_mode,filedef,startfield)
struct scr_params tbl[];
int tbl_len;
int *edited;
enum en_mode_type en_mode;
void *filedef;
int *startfield;
{
   unsigned int crc;
   int iter = *startfield;
   int wch = CTRL_X,pcol,c,crnt_pos = 0;
   unsigned char chr,att,chr2,att2;
   unsigned int edmode = TYPE_OVER;
   char compstr[81],tmp_fmt[10],blank[81],nulstr[]="";
   char buff[20],fmtbuff[6],*temp;

   int status;
   char line[100];
   $DESCRIPTOR(astring,line);
   
   struct date_only cvtdate;

   struct date_time dtbuff;
   struct date_only dbuff;

/*ZEN  */
   G_filedef = filedef;

   for EVER {
      *startfield = iter;  /* ZEN  New addition 25-AUG-2023 21:23:51 */
      if (tbl[iter].format  && !(tbl[iter].format & NON_EDIT)){
        if(!(tbl[iter].format & FREE_TEXT)){
          if ((temp = strchr(tbl[iter].prompt,'%')) != NULL)
              pcol = tbl[iter].col + (index(tbl[iter].prompt,'%')-tbl[iter].prompt);
          else
              pcol = tbl[iter].col + strlen(tbl[iter].prompt);
         
          /* exredchr(&chr,&att); */
      
          exloccur(tbl[iter].row,(pcol>0)?pcol-1:pcol);

          status = smg$read_from_display(&crnt_window->display_id,&astring);
          line[astring.dsc$w_length] = 0;           

          chr = line[0]; att = 0;
          chr2 = line[(pcol>0)?tbl[iter].lnth+1:tbl[iter].lnth ]; att2 = 0;

          if (pcol > 0)
             exwrtchr('[',WHITE/*tbl[iter].prattr*/);
        
 
          exloccur(tbl[iter].row,pcol +tbl[iter].lnth);
         /* exredchr(&chr2,&att2);  */
          exwrtchr(']',WHITE/*tbl[iter].prattr*/,1);
          Refresh(crnt_window->display_id/*Standard_Window*/);
        }
        edmode = (unsigned int)tbl[iter].format & (TYPE_OVER|INSERT_MODE|WORD_WRAP|SUPRESS_DELETE|CAPITALIZE);

        if (crnt_pos < 0) crnt_pos = 81;
        else crnt_pos = 0;

        if ((tbl[iter].format >> 16) != 0) crnt_pos = (int)(tbl[iter].format >> 16);

        if (tbl[iter].format & UNFMT_STRING) {  /* INPUT UNFORMATTED STRING  */
      /*     *compstr = 0;
           strncpy(compstr,tbl[iter].field,tbl[iter].lnth);  */

           strcpy(compstr,tbl[iter].field);

           /* l_unpad(compstr,tbl[iter].lnth); */
           do {
              wch = linedt(tbl[iter].field,tbl[iter].row,
                            tbl[iter].col + strlen(tbl[iter].prompt),tbl[iter].lnth,
                            tbl[iter].fattr,&crnt_pos,&edmode,tbl[iter].cmndline);
           /* ZENDIAG  warn_user("|%s| |%s|",compstr,tbl[iter].field); */
           /*   if (strncmp(compstr,tbl[iter].field,strlen(tbl[iter].field)) != 0) *edited = TRUE; */
              if (strcmp(compstr,tbl[iter].field) != 0) *edited = TRUE;
           }while( (strcmp(compstr,tbl[iter].field) != 0) && tbl[iter].validate != NULL && 
                                         (!(*tbl[iter].validate)(tbl[iter].field,&iter,&wch)));
           exscrprn(tbl[iter].field,tbl[iter].row,tbl[iter].col + strlen(tbl[iter].prompt),tbl[iter].fattr);
        } else if (tbl[iter].format & DATE_ONLY) {   /* INPUT DATE */
           crc = getcrc(tbl[iter].field,4);
           crnt_pos = 0;
           do {
              wch = datedt(tbl[iter].field,tbl[iter].row,
                           tbl[iter].col + strlen(tbl[iter].prompt),tbl[iter].fattr,&crnt_pos,&edmode,tbl[iter].cmndline);
           }while(tbl[iter].validate != NULL && !(*tbl[iter].validate)(tbl[iter].field));
    /*       if (datenull(tbl[iter].field)) {
              sprintf (blank,"%10s",nulstr);
              exscrprn(blank,tbl[iter].row,tbl[iter].col + strlen(tbl[iter].prompt),tbl[iter].fattr);
           }  */
           if (crc != getcrc(tbl[iter].field,4)) *edited = TRUE;
        } else  if (tbl[iter].format & BTRV_DATE) {   /* INPUT BTRIEVE DATE */
           crc = getcrc(tbl[iter].field,8);
           do {
              systodo(&dbuff,tbl[iter].field);
              do {
                 wch = datedt(&dbuff,tbl[iter].row,
                           tbl[iter].col + strlen(tbl[iter].prompt),tbl[iter].fattr,&crnt_pos,&edmode,tbl[iter].cmndline);
              } while (dotosys(tbl[iter].field,&dbuff)==NULL);
           }while(tbl[iter].validate != NULL && !(*tbl[iter].validate)(tbl[iter].field,&iter,&wch));
/*           if (btrv_datenull(tbl[iter].field)) {
              sprintf (blank,"%8s",nulstr);
              exscrprn(blank,tbl[iter].row,tbl[iter].col + strlen(tbl[iter].prompt),tbl[iter].fattr);
           }  
 */
           if (crc != getcrc(tbl[iter].field,8)) *edited = TRUE;

        } else if (tbl[iter].format & SINGLE_CHAR) {   /* INPUT CHARACTER */
           crc = *(char *)tbl[iter].field;
           do {
              do {
                 sprintf(buff,"%1c",*((char *)tbl[iter].field));
                 wch = linedt(buff,tbl[iter].row,tbl[iter].col + strlen(tbl[iter].prompt),1,
                              tbl[iter].fattr,&crnt_pos,&edmode,tbl[iter].cmndline);
              } while (sscanf(buff,"%1c",(char *)tbl[iter].field) == 0);
  	           if (crc != *(char*)tbl[iter].field) *edited = TRUE;
           }while(*edited && tbl[iter].validate != NULL && !(*tbl[iter].validate)(tbl[iter].field,&iter,&wch));
           sprintf(buff,"%1c",*((char *)tbl[iter].field));
           exscrprn(buff,tbl[iter].row,tbl[iter].col + strlen(tbl[iter].prompt),tbl[iter].fattr);
        } else if (tbl[iter].format & NUMERIC) {   /* INPUT NUMERIC */
           if ((temp = strchr(tbl[iter].prompt,'%')) != NULL){
              exgetpce(temp,fmtbuff,' ',1);
              pcol = tbl[iter].col + (index(tbl[iter].prompt,'%')-tbl[iter].prompt);
              switch (fmtbuff[strlen(fmtbuff) - 2]) {
                 case 'l':
                    switch (fmtbuff[strlen(fmtbuff)-1]) {
                       case 'd':
                       case 'i':
                          sprintf(compstr,fmtbuff,*(unsigned long *)tbl[iter].field);
                          do {
                             do {
                                sprintf(buff,fmtbuff,*(unsigned long *)tbl[iter].field);
                                wch = linedt(buff,tbl[iter].row,pcol,tbl[iter].lnth,tbl[iter].fattr,&crnt_pos,&edmode,tbl[iter].cmndline);
                             } while (sscanf(buff,"%11li",(unsigned long *)tbl[iter].field) == 0);
                             sprintf(buff,fmtbuff,*(long *)tbl[iter].field);
                          }while(strcmp(compstr,buff) 
								  			&& tbl[iter].validate != NULL 
											&& !(*tbl[iter].validate)(tbl[iter].field) 
											&& !(crnt_pos=0));
                          break;
                       case 'u': /*     UNSIGNED LONG INTEGER   */
                          sprintf(compstr,fmtbuff,*(unsigned long *)tbl[iter].field);
                          do {
                             do {
                                sprintf(buff,fmtbuff,*(unsigned long *)tbl[iter].field);
                                wch = linedt(buff,tbl[iter].row,pcol,tbl[iter].lnth,tbl[iter].fattr,&crnt_pos,&edmode,tbl[iter].cmndline);
                             } while (sscanf(buff,"%11lu",(unsigned long *)tbl[iter].field) == 0);
                             sprintf(buff,fmtbuff,*(unsigned long *)tbl[iter].field);
                          }while(strcmp(compstr,buff) 
								  			&& tbl[iter].validate != NULL 
											&& !(*tbl[iter].validate)(tbl[iter].field) 
											&& !(crnt_pos=0));
                          break;
                    }
                    break;
                 case 'h':
                    switch (fmtbuff[strlen(fmtbuff)-1]) {
                       case 'u':  /*   UNSIGNED BYTE        */
                          c = *(uint16 *)tbl[iter].field;
                          sprintf(compstr,fmtbuff,c);
                          do {
                             do {
                                sprintf(buff,fmtbuff,c);
                                wch = linedt(buff,tbl[iter].row,pcol,tbl[iter].lnth,tbl[iter].fattr,&crnt_pos,&edmode,tbl[iter].cmndline);
                             } while (sscanf(buff,"%hu",(uint16 *)&c) == 0);
                             sprintf(buff,fmtbuff,c);
                          }while(strcmp(compstr,buff) 
								  			&&tbl[iter].validate != NULL 
											&& !(*tbl[iter].validate)(tbl[iter].field) 
											&& !(crnt_pos=0));
                          *(uint16 *)tbl[iter].field = c;
                          break;
                    }
                    break;
                 default :
                    switch (fmtbuff[strlen(fmtbuff)-1]) {
                       case 'd':
                       case 'i':
                       case 'u': /*        UNSIGNED PLAIN INTEGER    */
                          sprintf(compstr,fmtbuff,*(unsigned int *)tbl[iter].field);
                          do {
                             do {
                                /* sprintf(tmp_fmt,"%%-%cu",tbl[iter].lnth + '0');*/
                                sprintf(buff,fmtbuff,*(unsigned int *)tbl[iter].field);
                                wch = linedt(buff,tbl[iter].row,pcol,tbl[iter].lnth,tbl[iter].fattr,&crnt_pos,&edmode,tbl[iter].cmndline);
                             } while (sscanf(buff,"%5u",(unsigned int *)tbl[iter].field) == 0);
                             sprintf(buff,fmtbuff,*(unsigned int *)tbl[iter].field);
                          }while(strcmp(compstr,buff) 
								  			&& tbl[iter].validate != NULL 
											&& !(*tbl[iter].validate)(tbl[iter].field) 
											&& !(crnt_pos=0));
                          break;
                       case 'f': /*        FLOAT        */
                          sprintf(compstr,fmtbuff,*(float *)tbl[iter].field);
                          do {
                             sprintf(buff,fmtbuff,*(float *)tbl[iter].field);
                             wch = linedt(buff,tbl[iter].row,pcol,tbl[iter].lnth,tbl[iter].fattr,&crnt_pos,&edmode,tbl[iter].cmndline);
                             *(float *)tbl[iter].field = (float)atof(buff);
                             sprintf(buff,fmtbuff,*(float *)tbl[iter].field);
                          }while(strcmp(compstr,buff) 
								  			&&tbl[iter].validate != NULL 
											&& !(*tbl[iter].validate)(tbl[iter].field) 
											&& !(crnt_pos=0));
                          break;
                    }
                    break;
              }
              if (strcmp(compstr,buff)) *edited = TRUE;
              exscrprn(buff,tbl[iter].row,pcol,tbl[iter].fattr);
           }
        } else /* if (tbl[iter].format & DATE_TIME) {   /* INPUT  DATE/TIME *//*
           crc = getcrc(tbl[iter].field,5);
           do {
              wch = datimedt(tbl[iter].field,tbl[iter].row,
              tbl[iter].col + strlen(tbl[iter].prompt),tbl[iter].fattr,&crnt_pos,&edmode,tbl[iter].cmndline);
           }while(tbl[iter].validate != NULL && !(*tbl[iter].validate)(tbl[iter].field));
           if (datetnull(tbl[iter].field)) {
              sprintf (blank,"%14s",nulstr);
              exscrprn(blank,tbl[iter].row,tbl[iter].col + strlen(tbl[iter].prompt),tbl[iter].fattr);
           }
           if (crc != getcrc(tbl[iter].field,5)) *edited = TRUE;
        } else */ if (tbl[iter].format & BTRV_DATE_TIME) {   /* INPUT Btrieve  DATE/TIME */
           crc = getcrc(tbl[iter].field,8);
           do {
              systodt(&dtbuff,tbl[iter].field);
              do {  
                 wch = datimedt(&dtbuff,tbl[iter].row,
                                tbl[iter].col + strlen(tbl[iter].prompt),
                                tbl[iter].fattr,&crnt_pos,&edmode,tbl[iter].cmndline);
              }while (dttosys(tbl[iter].field,&dtbuff) == NULL);
           }while(tbl[iter].validate != NULL && !(*tbl[iter].validate)(tbl[iter].field));
/*           if (btrv_datetnull(tbl[iter].field)) {
              sprintf (blank,"%14s",nulstr);
              exscrprn(blank,tbl[iter].row,tbl[iter].col + strlen(tbl[iter].prompt),tbl[iter].fattr);
           }
*/
           if (crc != getcrc(tbl[iter].field,8)) *edited = TRUE;
        } else if (tbl[iter].format & FREE_TEXT) { /* INPUT  VARIABLE LENGTH TEXT  */
           Help(TEXACOHLP);
           texaco(/*&start_line*/&tbl[iter].field,tbl[iter].row,tbl[iter].col,
                                                     &wch,edited,tbl[iter].lnth,RIGHT_MARGIN,
                                                     tbl[iter].validate,
                                                     edmode,tbl[iter].cmndline);
                                            /* ROW becomes T_LINE. COL , B_LINE */
           Help("");
        }

        if(!(tbl[iter].format & FREE_TEXT)){
            if ((temp = strchr(tbl[iter].prompt,'%')) != NULL)
                pcol = tbl[iter].col + (index(tbl[iter].prompt,'%')-tbl[iter].prompt);
            else
                pcol = tbl[iter].col + strlen(tbl[iter].prompt);

            if (pcol>0) { 
               exloccur(tbl[iter].row,pcol - 1);
               exwrtchr(chr,att,1);
            }
            exloccur(tbl[iter].row,pcol + tbl[iter].lnth);
            exwrtchr(chr2,att2,1);
            Refresh(crnt_window->display_id);
        }
      }else {
         if (iter == 0) wch = ESCEOL;
         if (iter == tbl_len-1) wch = (en_mode == breakout)?wch:ESCBOL;
      }

      if (en_mode == breakout)
         switch (wch){
            case ESCEOL : wch = SMG$K_TRM_RIGHT; /* ENTER;   */
            case SMG$K_TRM_HT    :
            case SMG$K_TRM_CR :
            case SMG$K_TRM_RIGHT:
            case ENTER  : if(iter < tbl_len-1) iter++;
                          else return (wch);
                          break;
            case ESCBOL : wch = SMG$K_TRM_UNKNOWN; /*BACKTAB; */
            case SMG$K_TRM_UNKNOWN: if (iter > 0) iter--; /* BACKTAB  */
                          else return (wch);
                          break;
/*            case UARR   :
            case SMG$K_TRM_DOWN   :
            case ALT_O  :
            case ALT_D  :
            case CTRL_PGUP:
            case CTRL_PGDN:
            case PGUP   :
            case PGDN   :
            case F10    :
            case F5     :
            case F9     :
            case F1     :
            case F2     : return(wch); */
            case ESC    :
            case SMG$K_TRM_CTRLZ: return(wch);
            default     : return(wch);
         }
      else
         switch (wch){
            case ESCEOL : wch = SMG$K_TRM_RIGHT;
            case SMG$K_TRM_HT    :
            case SMG$K_TRM_VT    :
            case SMG$K_TRM_DOWN   :
            case SMG$K_TRM_RIGHT:
            case SMG$K_TRM_CR  : if(iter < tbl_len-1) iter++;
                          break;
            case ESCBOL : wch = SMG$K_TRM_UNKNOWN;
            case SMG$K_TRM_UNKNOWN:   /* for BACKTAB */
            case SMG$K_TRM_UP: if (iter > 0) iter--;
                          break;
            case F10    :
            case F9     :
            case F1     :
            case F2     :	if (en_mode == special) return(wch);
									break;
            case ESC    :
            case SMG$K_TRM_CTRLZ:
            if(*edited) {
             /*
               Help("End Edit :  Y/N");
             */
               switch(warn_user("End Edit :  Y/N")/*Get_Keystroke()*/) {
               case 'y':
               case 'Y':
                          return(wch);
                        break;
               case 'n' :
               case 'N' :
                  /* return(ESC);   */
                   break;
               default : break;
               }
            } else return(wch);
          
                                 
      /*   }   */
      }
   }
}





