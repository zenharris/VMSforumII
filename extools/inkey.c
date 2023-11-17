/* #include <dos.h>   */
#include <stdlib.h>
#include <stdio.h>

#include <ints.h>
#include smgdef
#include smg$routines
#include "extools.h"

/*  KEYBOARD  MACRO  RECORDING   ROUTINES  */

#define exscrprn(str,row,col,attr) exscrprn(str,row,col,attr)

extern void	exrtcinit_at(void);
extern void	exrtcinit_xt(void);
extern void	exrtcterm(void);


struct keyb_comm {
   int inchr,scan;
};

struct keyb_cell {
   struct keyb_comm comm;
   struct keyb_cell *next;
};

struct keyb_queue {
   struct keyb_cell *front,*rear;
};


static struct keyb_queue *crnt_macro = NULL;
int exec_macro = FALSE;
int recd_macro = FALSE;
int macro_changed_flag = FALSE;
static int once = FALSE;
static int macro_key;
/* void interrupt(*saveclk)() = NULL;  */
static struct keyb_cell *curs_macro = NULL;
static struct keyb_queue FX_macro[10] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
                                         NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
                                         NULL,NULL,NULL,NULL};

static mac_makenull(keyb_macro)
struct keyb_queue *keyb_macro;
{
   if((keyb_macro->front = (struct keyb_cell *) malloc(sizeof(struct keyb_cell))) != NULL) {
      keyb_macro->front->next = NULL;
      keyb_macro->rear = keyb_macro->front;
   }
}

static int mac_empty(keyb_macro)
struct keyb_queue *keyb_macro;
{
   if (keyb_macro->front == keyb_macro->rear)
      return (TRUE);
   else
      return (FALSE);
}

static mac_enqueue(inchr,scan,keyb_macro)
int inchr,scan;
struct keyb_queue *keyb_macro;
{
   if((keyb_macro->rear->next = (struct keyb_cell *) malloc(sizeof(struct keyb_cell))) != NULL) {
      keyb_macro->rear = keyb_macro->rear->next;
      keyb_macro->rear->comm.inchr = inchr;
      keyb_macro->rear->comm.scan = scan;
      keyb_macro->rear->next = NULL;
   }
}

static mac_dequeue (keyb_macro)
struct keyb_queue *keyb_macro;
{
   char *del_ptr = (char *)keyb_macro->front;
   if (!mac_empty(keyb_macro)) {
      keyb_macro->front = keyb_macro->front->next;
      free(del_ptr);
   }
}

static mac_read(inchr,scan,keyb_macro)
int *inchr,*scan;
struct keyb_queue *keyb_macro;
{
   if (!mac_empty(keyb_macro)){
      if (curs_macro == NULL) curs_macro = keyb_macro->front->next;
      *inchr = curs_macro->comm.inchr;
      *scan = curs_macro->comm.scan;
      curs_macro = curs_macro->next;
      if (curs_macro == NULL) exec_macro = FALSE;
   }
}


mac_save(filename)
char *filename;
{
   FILE *mac_file;
   struct keyb_cell *curs;
   int c,count,header[10];
   if ((mac_file = fopen(filename,"wb")) != NULL){
      fseek(mac_file,20L,SEEK_SET);
      for (c=0;c < 10;c++){
         if (FX_macro[c].front != NULL && !mac_empty(&FX_macro[c])){
            for (curs = FX_macro[c].front->next,count = 0;curs != NULL;curs = curs->next,count++)
               fwrite (&curs->comm,sizeof(struct keyb_comm),1,mac_file);
            header[c] = count;
         } else header[c] = 0;
      }
      fseek(mac_file,0L,SEEK_SET);
      fwrite (header,20,1,mac_file);
      fclose (mac_file);
   }
}

static void destroy_macro_storage()
{
   int c;
   for (c=0;c<10;c++){
     if (FX_macro[c].front != NULL){
        while (!mac_empty(&FX_macro[c])) mac_dequeue(&FX_macro[c]);
        free(FX_macro[c].front);
     }
   }
}
/*
void interrupt mac_warn2()
{
   if (recd_macro) exscrprn("Macro On",0,0,BLINK|LIGHTGRAY);
   else exscrprn("        ",0,0,LIGHTGRAY);
   if (insert_mode) exscrprn("Insrt",0,9,LIGHTGRAY);
   else exscrprn("TypOv",0,9,LIGHTGRAY);

}

void interrupt mac_warn()
{
   static char buffer[82];
   static struct x_date_time cdt;
   static int c = 0;
   if (recd_macro) exscrprn("Macro On",0,0,BLINK|LIGHTGRAY);
   else exscrprn("        ",0,0,LIGHTGRAY);
   if (insert_mode) exscrprn("Insrt",0,9,LIGHTGRAY);
   else exscrprn("TypOv",0,9,LIGHTGRAY);
   if (c++ == 18 ) {
      exscrprn(asc_x_date_time(buffer,sys_date_time(&cdt)),0,53,LIGHTGRAY);
      c = 0;
   }
}
*/

void warn_off(void)
{
/********   exrtcterm(); *********/
}

void warn_on()
{				 
/*      if (saveclk == NULL) atexit(warn_off);*/
/**********************		exrtcinit_xt(); ***************/
}

void clock_on()
{
/**********   exrtcinit_at(); *********/
/*	if (saveclk == NULL) atexit(warn_off);*/
}						 

mac_load(filename)
char *filename;
{
   FILE *mac_file;
   struct keyb_comm in_comm;
   int c,count,header[10];
   if ((mac_file = fopen(filename,"rb")) != NULL){
      fseek(mac_file,0L,SEEK_SET);
      fread (header,20,1,mac_file);
      for (c=0;c < 10;c++){
         if (header[c] != 0){
            if (!once) {
               atexit (destroy_macro_storage);
               once = TRUE;
            }
            if (FX_macro[c].front == NULL) mac_makenull(&FX_macro[c]);
            else while (!mac_empty(&FX_macro[c])) mac_dequeue(&FX_macro[c]);
            for (count = 0;count < header[c];count++){
               fread (&in_comm,sizeof(struct keyb_comm),1,mac_file);
               mac_enqueue(in_comm.inchr,in_comm.scan,&FX_macro[c]);
            }
         }
      }
      fclose (mac_file);
   }
}


static toggle_record()
{
   static char *trash_macro[] = {
      "Re-Record Macro",
      " Cancel Record "
   };
   if (recd_macro) {
      recd_macro = FALSE;
      macro_key = TRUE;
   } else {
      if (crnt_macro->front == NULL) mac_makenull(crnt_macro);
      if (!mac_empty(crnt_macro) && !verifyx(trash_macro,TRUE)) return;
      while (!mac_empty(crnt_macro)) mac_dequeue(crnt_macro);
      macro_key = recd_macro = TRUE;
      if (!once) {
         atexit (destroy_macro_storage);
         once = TRUE;
      }
      macro_changed_flag = TRUE;
   }
}

static stop_record()
{
    recd_macro = FALSE;
    macro_key = TRUE;
}

static macro_play(inchr,scan)
int *inchr,*scan;
{
   if (!mac_empty(crnt_macro)){
      exec_macro = TRUE;
      recd_macro = FALSE;
      mac_read(inchr,scan,crnt_macro);
   }
}

unsigned __int16 exinkey (intptr)
/*       int *intptr; */
unsigned __int16 *intptr;
{
  uint16 inchr,scan;
/*  union REGS wreg;  */
  uint16  word_terminator_code;
  int status;
  if(exec_macro)
      mac_read(&inchr,&scan,crnt_macro);
  else {
     do{

        macro_key = FALSE;
/*
        wreg.h.ah = 00;
        int86(0x16,&wreg,&wreg);
        scan = (int)wreg.h.ah;
        inchr = (int)wreg.h.al;
*/
        status = smg$read_keystroke (&KeyBoard,&inchr);
        scan = inchr; 
        if ((inchr > 127)) inchr = 00;

/*
        if (inchr == 00){
           switch (scan) {
              case CTRL_F1 :crnt_macro = &FX_macro[0];
                            toggle_record();
                            break;
              case CTRL_F2 :crnt_macro = &FX_macro[1];
                            toggle_record();
                            break;
              case CTRL_F3 :crnt_macro = &FX_macro[2];
                            toggle_record();
                            break;
              case CTRL_F4 :crnt_macro = &FX_macro[3];
                            toggle_record();
                            break;
              case CTRL_F5 :crnt_macro = &FX_macro[4];
                            toggle_record();
                            break;
              case CTRL_F6 :crnt_macro = &FX_macro[5];
                            toggle_record();
                            break;
              case CTRL_F7 :crnt_macro = &FX_macro[6];
                            toggle_record();
                            break;
              case CTRL_F8 :crnt_macro = &FX_macro[7];
                            toggle_record();
                            break;
              case CTRL_F9 :crnt_macro = &FX_macro[8];
                            toggle_record();
                            break;
              case CTRL_F10 :crnt_macro = &FX_macro[9];
                             toggle_record();
                             break;
              case ALT_F1   :
              case ALT_F2   :
              case ALT_F3   :
              case ALT_F4   :
              case ALT_F5   :
              case ALT_F6   :
              case ALT_F7   :
              case ALT_F8   :
              case ALT_F9   :
              case ALT_F10  : stop_record();
                             break;
              case SHFT_F1 : crnt_macro = &FX_macro[0];
                              macro_play(&inchr,&scan);
                             break;
              case SHFT_F2: crnt_macro = &FX_macro[1];
                             macro_play(&inchr,&scan);
                             break;
              case SHFT_F3 : crnt_macro = &FX_macro[2];
                             macro_play(&inchr,&scan);
                             break;
              case SHFT_F4 : crnt_macro = &FX_macro[3];
                             macro_play(&inchr,&scan);
                              break;
              case SHFT_F5 : crnt_macro = &FX_macro[4];
                             macro_play(&inchr,&scan);
                              break;
              case SHFT_F6 : crnt_macro = &FX_macro[5];
                             macro_play(&inchr,&scan);
                              break;
              case SHFT_F7 : crnt_macro = &FX_macro[6];
                             macro_play(&inchr,&scan);
                             break;
              case SHFT_F8 : crnt_macro = &FX_macro[7];
                             macro_play(&inchr,&scan);
                              break;
              case SHFT_F9 : crnt_macro = &FX_macro[8];
                             macro_play(&inchr,&scan);
                              break;
              case SHFT_F10: crnt_macro = &FX_macro[9];
                             macro_play(&inchr,&scan);
                             break;
           }
        }
*/
     }while (macro_key);
     if(recd_macro)
        mac_enqueue(inchr,scan,crnt_macro);
  }

  *intptr = scan;
  return(inchr);
}
