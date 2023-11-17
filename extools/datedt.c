
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include smgdef
#include smg$routines
#include ints
#include descrip
#include starlet
#include stsdef

#include "extools.h"


#define CR    13
#define DATIMELNTH 22
#define DATIMEFRMT "%2.2hu/%2.2hu/%4.4hu %2.2hu:%2.2hu:%2.2hu.%2.2hu"
#define DATIMESCNFRMT "%2hu/%2hu/%4hu %2hu:%2hu:%2hu.%2hu"
#define DATELNTH 10
#define DATEFRMT "%2.2hu/%2.2hu/%4.4hu"
#define DATESCNFRMT "%2hu/%2hu/%4hu"

void Refresh();



struct date_only *systodo (struct date_only *out_date,struct timebuff *in_date)
{
   int status;
   unsigned short int numvec[7];

    status = sys$numtim (numvec,
                            in_date); /* &time_now);  */


   out_date->day = numvec[2];  /*in_date->day; */
   out_date->month = numvec[1]; /* in_date->month; */
   out_date->year = numvec[0];  /*in_date->year%100; */
   return(out_date);
}

struct date_time *systodt (struct date_time *out_date,struct timebuff *in_date)
{
   int status;
   unsigned short int numvec[7];

    status = sys$numtim (numvec, in_date);

   out_date->day = numvec[2]; 
   out_date->month = numvec[1];
   out_date->year = numvec[0]; 
   out_date->hour = numvec[3];
   out_date->min = numvec[4];
   out_date->sec = numvec[5];
   out_date->hun = numvec[6];

   return(out_date);
}



static char *months[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun",
                          "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

struct timebuff *dotosys (struct timebuff *out_date,struct date_only *in_date)
{

   int status;
   char outstr[25] = "";

   $DESCRIPTOR (output_d, outstr);   

   if (in_date->month < 1 || in_date->month > 12) {
      Status("Invalid Month");
      return(NULL);
   }

   sprintf(outstr,"%2.2hu-%3s-%4.4hu 00:00:00.00",in_date->day,
                                                months[in_date->month-1],
                                                in_date->year);
   
   output_d.dsc$w_length = strlen(outstr);

   status = sys$bintim (&output_d,out_date);

   if (!$VMS_STATUS_SUCCESS (status)) {
       
       Status("Invalid Date ");
       return(NULL);
    /*   (void)lib$signal (status);*/
   }


   return(out_date);
}

struct timebuff *dttosys (struct timebuff *out_date,struct date_time *in_date)
{

   int status;
   char outstr[25] = "";

   $DESCRIPTOR (output_d, outstr);   

   if (in_date->month < 1 || in_date->month > 12) {
      Status("Invalid Month");
      return(NULL);
   }

   sprintf(outstr,"%2.2hu-%3s-%4.4hu %2.2hu:%2.2hu:%2.2hu.%2.2hu",in_date->day,
                                                months[in_date->month-1],
                                                in_date->year,
                                                in_date->hour,in_date->min,
                                                in_date->sec,in_date->hun);
  

   output_d.dsc$w_length = strlen(outstr);

   status = sys$bintim (&output_d,out_date);

   if (!$VMS_STATUS_SUCCESS (status)) {
       
       Status("Invalid Date Time ");
       return(NULL);
    /*   (void)lib$signal (status);*/
   }


   return(out_date);
}





int datedt(dat,row,col,attr,edtpos,control,cmndline)
struct date_only *dat;
int col,row,attr,*edtpos;
unsigned int *control;
struct cmndline_params cmndline[];
{
   unsigned short int day,month,year;
    int c,exit;
   unsigned int inchr,yank_date = FALSE; /* (getenv("KOMTDT") != NULL) && (strcmp(getenv("KOMTDT"),"USA") == 0); */
   uint16 scan;
   char edtstr[15],outstr[16];
   struct date_only test_date;
	struct linedt_msg_rec msg;

	msg.edtstr = edtstr;
	msg.col = col;
	msg.row = row;
	msg.lnth = DATELNTH; /*8; */
	msg.attr = attr;
	msg.pos = edtpos;
	msg.control = control;
	msg.cmndline = cmndline;


	if (yank_date) {
      day = dat->month;
      month = dat->day;
   } else {
      day = dat->day;
      month = dat->month;
   }
   year = dat->year;
   sprintf(edtstr,DATEFRMT,day,month,year);
   if (*edtpos >= DATELNTH) *edtpos = DATELNTH - 1;
   else if (*edtpos < 0) *edtpos = 0;
/*   exloccur(row,*edtpos + col);*/
   exscrprn(edtstr,row,col,attr);
/*   exsetcur(6,8); */
   exloccur(row,*edtpos + col);

   exit = TRUE;
   while (exit) {
/*      exscrprn(edtstr,row,col,attr); */
      exloccur(row,*edtpos + col);
      Refresh(crnt_window->display_id/*Standard_Window*/);
      inchr = command_line(cmndline,&scan,&msg);

      switch (inchr) {

         case 0 :
                 switch (scan) {
  
                     case SMG$K_TRM_RIGHT  :
                                 if (*edtpos < strlen(edtstr)) (*edtpos)++;
                                 else *edtpos = DATELNTH + 1;
                                   switch (*edtpos)  {
                                      case 2 :
                                      case 5 : (*edtpos)++;
                                               break;
                                    }
                                   break;
                      case SMG$K_TRM_LEFT  : (*edtpos)--;
                                   switch (*edtpos) {
                                      case 5 :
                                      case 2 : (*edtpos)--;
                                               break;
                                   }
                                   break;
                      case SMG$K_TRM_DOWN :
                      case SMG$K_TRM_UP :
                                   exit = FALSE;
                                   break;
                      case SMG$K_TRM_UNKNOWN /*HOME*/  : if (!read_only) { 
                                     exdltstr(edtstr,0,strlen(edtstr));
      	                             *edtpos = 0;
   	                                exloccur(row,*edtpos + col);
	                                   sprintf(edtstr,DATEFRMT,day,month,year);
											  }
                                   break;


                      default    :exit = FALSE;
                                  break;

                  } 
                  break;

         case SMG$K_TRM_DELETE :  /*BACKSPACE : */
                    if (*edtpos > 0) {
                       (*edtpos)--;
                       switch (*edtpos) {
                          case 5 :
                          case 2 : (*edtpos)--;
                                   break;
                       }
                       edtstr[*edtpos] = ' ';
                       exscrprn(edtstr,row,col,attr);
                       exloccur(row,*edtpos + col);
                    }
                    break;

         case  SMG$K_TRM_CTRLZ:  exit = FALSE;
                     break; 

         case SMG$K_TRM_HT : scan = TAB;
         case SMG$K_TRM_CR :exit = FALSE;
                   break;
         default : if (!read_only && isdigit(inchr)) {
                         edtstr[*edtpos] = inchr;
                         exscrprn(edtstr,row,col,attr);
                         (*edtpos)++;
                         switch (*edtpos) {
                           case 2 :
                           case 5 : (*edtpos)++;
                                    break;
                        }
                   }
                   break;
     }
     if (*edtpos >= DATELNTH) {
        exit = FALSE;
        scan = ESCEOL ; /* DARR; */
     }else if (*edtpos < 0) {
        exit = FALSE;
        scan = ESCBOL ; /* UARR; */
     }


     if (!exit) {
        if (sscanf(edtstr,DATESCNFRMT,&day,&month,&year) == 3){
        /*   test_date.day = (unsigned short int) day;
           test_date.month = (unsigned short int) month;
           test_date.year = (unsigned short int) year;
           if (!check_date(&test_date)) {
              exscrprn("Invalid ",row,col,RED);
              fprintf (stderr,"%c",7);
              exit = TRUE;
           } 
        */
        } else exit = TRUE;

        if (exit)
           if (*edtpos >= DATELNTH) *edtpos = DATELNTH - 1;
           else if (*edtpos < 0) *edtpos = 0;
     }
   }
   sprintf(edtstr,DATEFRMT,day,month,year);
   exscrprn(edtstr,row,col,attr);
   if (yank_date) {
      dat->day =  month;
      dat->month =  day;
   } else {
      dat->day = day;
      dat->month = month;
   }
   dat->year =  year;
/*   if (*control & INSERT_MODE) exsetcur(3,8); */

   return(scan);
}



int datimedt(dt,row,col,attr,edtpos,control,cmndline)
struct date_time *dt;
int col,row,attr,*edtpos;
unsigned int *control;
struct cmndline_params cmndline[];
{
   int c,exit;
   unsigned int inchr,yank_date = FALSE; /* (getenv("KOMTDT") != NULL) && (strcmp(getenv("KOMTDT"),"USA") == 0);*/
   uint16 scan;
   char edtstr[25];
   unsigned short int day,month,year,hour,min,sec,hun;
   struct date_only test_date;
	struct linedt_msg_rec msg;

	msg.edtstr = edtstr;
	msg.col = col;
	msg.row = row;
	msg.lnth = 14;
	msg.attr = attr;
	msg.pos = edtpos;
	msg.control = control;
	msg.cmndline = cmndline;


    if (yank_date) {
      day = dt->month;
      month = dt->day;
   } else {
      day = dt->day;
      month = dt->month;
   }
   year = dt->year;
   hour = dt->hour;
   min = dt->min;
   sec = dt->sec;
   hun = dt->hun;

   sprintf(edtstr,DATIMEFRMT,day,month,year,hour,min,sec,hun);

   if (*edtpos >= DATIMELNTH) *edtpos = DATIMELNTH - 1;
   else if (*edtpos < 0) *edtpos = 0;

   exscrprn(edtstr,row,col,attr);
   exloccur(row,*edtpos + col);

/*   exsetcur(6,8);  */
   exit = TRUE;
   do {
/*      exscrprn(edtstr,row,col,attr);*/
      exloccur(row,*edtpos + col);

      Refresh(crnt_window->display_id/*Standard_Window*/);
      inchr = command_line(cmndline,&scan,&msg);

      switch (inchr) {
         case 0 : 
                   switch (scan) {
                      case SMG$K_TRM_RIGHT  : 
                                   if (*edtpos < strlen(edtstr)) (*edtpos)++;
                                   else *edtpos = DATIMELNTH + 1;
                                   switch (*edtpos) {
                                      case 2 :
                                      case 5 :
                                      case 10 :
                                      case 13 :
                                      case 16 :
                                      case 19: (*edtpos)++;
                                               break;
                                    }
                                   break;
                      case SMG$K_TRM_LEFT  : (*edtpos)--;
                                   switch (*edtpos) {
                                      case 2 :
                                      case 5 :
                                      case 10 :
                                      case 13 :
                                      case 16 :
                                      case 19: (*edtpos)--;
                                               break;
                                   }
                                   break;
                      case SMG$K_TRM_UP :
                      case SMG$K_TRM_DOWN :
                                   exit = FALSE;
                                   break;
 
                      case SMG$K_TRM_UNKNOWN /*HOME */ : if (!read_only) {
		 	           exdltstr(edtstr,0,strlen(edtstr));
      	                           *edtpos = 0;
   	                           exloccur(row,*edtpos + col);
	                           sprintf(edtstr,DATIMEFRMT,day,month,
                                                    year,hour,min,sec,hun);
											  }
                                   break;
                      default    :exit = FALSE;
                                  break;
          
                   }
                   break;
         case SMG$K_TRM_HT : scan = TAB;
         case SMG$K_TRM_CR : exit = FALSE;
                   break;
         case SMG$K_TRM_CTRLZ:  exit = FALSE;
                     break;
         case SMG$K_TRM_DELETE :
                    if (*edtpos > 0 ) {
                       (*edtpos)--;
                       switch (*edtpos) {
                           case 2 :
                           case 5 :
                           case 10 :
                           case 13 :
                           case 16 :
                           case 19: (*edtpos)--;
                                    break;
                       }
/*                       edtstr[*edtpos] = ' ';
                       exscrprn(edtstr,row,col,attr);*/
                       exloccur(row,*edtpos + col);
                    }
                    break;
         default : if (!read_only && isdigit(inchr)) {
                         edtstr[*edtpos] = inchr;
                         exscrprn(edtstr,row,col,attr);
                         (*edtpos)++;
                         switch (*edtpos) {
                           case 2 :
                           case 5 :
                           case 10 :
                           case 13 :
                           case 16 :                           
                           case 19: (*edtpos)++;
                                    break;
                        }
                   }
                   break;
     }
     if (*edtpos >= DATIMELNTH) {
        exit = FALSE;
        scan = ESCEOL ; /* DARR;*/
     }else if (*edtpos < 0) {
        exit = FALSE;
        scan = ESCBOL ; /* UARR;*/
     }

     if (!exit) {
        if (sscanf(edtstr,DATIMESCNFRMT,&day,&month,&year,&hour,&min,&sec,&hun) == 7){
/*           test_date.day = (unsigned char) day;
           test_date.month = (unsigned char) month;
           test_date.year = (unsigned char) year;
           if (!check_date(&test_date)) {
              exscrprn("    Invalid   ",row,col,RED);
              fprintf (stderr,"%c",7);
              exit = TRUE;
           }
*/
        } else exit = TRUE;

        if (exit)
           if (*edtpos >= DATIMELNTH) *edtpos = DATIMELNTH - 1;
           else if (*edtpos < 0) *edtpos = 0;
     }
   }while (exit);
   sprintf(edtstr,DATIMEFRMT,day,month,year,hour,min,sec,hun);
   exscrprn(edtstr,row,col,attr);
   if (yank_date) {
      dt->day =  month;
      dt->month =  day;
   } else {
      dt->day = day;
      dt->month = month;
   }
   dt->year = year;
   dt->hour = hour;
   dt->min = min;
   dt->sec = sec;
   dt->hun = hun;
/*   if (*control & INSERT_MODE) exsetcur(3,8); */
   return(scan);
}





