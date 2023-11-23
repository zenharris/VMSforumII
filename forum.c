                                            
/*
 * Title:       forum.c
 * Author:      Michael Brown <vmslib@b5.net>
 * Created:     03 Jun 2023
 * Last update:
 */


#include rms
#include stdio
#include ssdef
#include string
#include stdlib
#include starlet
#include lib$routines
#include descrip
#include ints
#include jpidef
#include stdarg

#include smgdef
#include smg$routines

#include "extools.h"
#define SCREEN_C
#include "screen.h"
#include "warns.h"
#include "menu.h"

#include "locality.h"

#include "forfile.h"

#define VERSION "2.25"  


/*  Moved to forfile.h
#define SIZE_UNAME	12
#define SIZE_SUBJECT	160
#define SIZE_UID        16
#define SIZE_DATIME 8
#define SIZE_TIMESTR 23

#define LINE_LENGTH 100
#define TOTAL_LINES 200
*/

#define RIGHT_MARGIN 75

#define EDIT_MODE TRUE
#define REPLY_MODE FALSE

#define LISTKEY 1
#define REVERSEKEY 2
#define INCLUDEFIRST 1


int   TopLine;
int   BottomLine;
int   StatusLine;
int   RegionLength;
int   helpLine;
int   CurrentLine = 0;
int   ListCount = 0;


int ShowRead = TRUE; /*FALSE;*/

struct line_rec *start_line = NULL;


char user_name[SIZE_UNAME+1] = "";
char PreviousReplyTop[SIZE_UID+1] = "";
char PreviousReplyBot[SIZE_UID+1] = "";
char TempReplyTo[SIZE_UID+1] = "";


int AllReadBot= FALSE, AllReadTop = FALSE;


/*
struct timebuff
   {
      uint32 b1,b2;
   };
*/


struct rfabuff {
   uint16 b1,b2,b3;
};

struct rfabuff FindRec;


struct List_Element {
   char Prompt[SIZE_SUBJECT+64];
   struct rfabuff rfa;
   struct List_Element *Prev;
   struct List_Element *Next;
};


struct List_Element *FirstPtr = NULL;
struct List_Element *LastPtr = NULL;
struct List_Element *CurrentPtr = NULL;


char response[BUFSIZ];

void get_findrec(struct List_Element **,struct rfabuff *);
/* void Status (char *); */
void Redraw_Screen(void);
void Read_and_Reply(struct rfabuff *,int);
int rmsget(struct rfabuff *);
void align_list(struct List_Element **,struct rfabuff *);
void active_align_list(struct List_Element **,struct rfabuff *);
int Read_Directory(int);

typedef struct { uint32 hi, lo; } u64;

/* Return X < Y.  */
int u64lt (x, y)
u64 x,y;
{
  return x.hi < y.hi || (x.hi == y.hi && x.lo < y.lo);
}


   int tbuff_equal(b1,b2)
   struct timebuff b1,b2;
   {

     return ((b1.b1 == b2.b1) && (b1.b2 == b2.b2));

   }

   void tbuff_copy(b1,b2)
   struct timebuff *b1,*b2;
   {

      b1->b1 = b2->b1;
      b1->b2 = b2->b2;

   }

   void rfa_copy(b1,b2)
   struct rfabuff *b1,*b2;
   {
      b1->b1 = b2->b1;
      b1->b2 = b2->b2;
      b1->b3 = b2->b3;
   }

   int rfa_equal(b1,b2)
   struct rfabuff b1,b2;
   {

     return ((b1.b1 == b2.b1) &&
             (b1.b2 == b2.b2) &&
             (b1.b3 == b2.b3));

   }


int rmstest(int rmscode,int num,...) {

   va_list valist;
   int i,found = FALSE;

   /* initialize valist for num number of arguments */
   va_start(valist, num);

   /* access all the arguments assigned to valist */
   for (i = 0; i < num; i++) {
      if(rmscode == va_arg(valist, int)) {
         found = TRUE;
         break;
      }
   }
        
   /* clean memory reserved for valist */
   va_end(valist);

   return found;
}




   void Increment (IncLine)
   unsigned int *IncLine;
   {
      if (TopLine + (*IncLine) < BottomLine) {
         (*IncLine)++;
      } else {
         Scroll_Up(TopLine,BottomLine);
      }
   }

   void Decrement (DecLine)
   unsigned int *DecLine;
   {
      if ((*DecLine) > 0) {
         (*DecLine)--; 
      } else {
         Scroll_Down(TopLine,BottomLine);
      }
   }

   void HiLite (Win, Prompt, Line_Num)
   unsigned int Win;
   char *Prompt;
   unsigned int Line_Num;
   {
      Add_Attr (Win,
           Line_Num,
           2,
           Prompt,
           SMG$M_BOLD);
      Refresh(Win);
   }

   void LoLite (Win, Prompt, Line_Num)
   unsigned int Win;
   char *Prompt;
   unsigned int Line_Num;
   {
      Add (Win,
           Line_Num,
           2,
           Prompt);
      Refresh(Win);
   }




   void InsertNewLine(Ptr)
      struct List_Element **Ptr;
   {

      struct List_Element *WorkLine = NULL;

      WorkLine = malloc(sizeof(struct List_Element));

      if (FirstPtr == NULL) {
      
         FirstPtr = WorkLine;
         WorkLine->Next = NULL;
         WorkLine->Prev = NULL;
         LastPtr = WorkLine;
           
      } else {  
         WorkLine->Prev = (*Ptr);
         WorkLine->Next = (*Ptr)->Next;

         if ((*Ptr)->Next != NULL)
             (*Ptr)->Next->Prev = WorkLine;
   
         (*Ptr)->Next = WorkLine;

         if (WorkLine->Next == NULL) 
             LastPtr = WorkLine;
      }
      (*Ptr) = WorkLine;
      ListCount++;
   }

void PutOnTop(Ptr)
   struct List_Element **Ptr;
{

     struct List_Element *WorkLine = NULL;

     WorkLine = malloc(sizeof(struct List_Element));
     if (WorkLine == NULL) {
        error_exit("MALLOC");
     }
 
     if (FirstPtr == NULL) {      
         FirstPtr = WorkLine;
         WorkLine->Next = NULL;
         WorkLine->Prev = NULL;
         LastPtr = WorkLine;           
      } else { 
         WorkLine->Next = FirstPtr;
         FirstPtr->Prev = WorkLine;
         WorkLine->Prev = NULL;
         FirstPtr = WorkLine;
      }
      (*Ptr) = WorkLine;
      ListCount++;
}

void PutOnBottom(Ptr)
   struct List_Element **Ptr;
{

     struct List_Element *WorkLine = NULL;

     WorkLine = malloc(sizeof(struct List_Element));
     if (WorkLine == NULL) {
        error_exit("MALLOC");
     }
 
     if (FirstPtr == NULL) {      
         FirstPtr = WorkLine;
         WorkLine->Next = NULL;
         WorkLine->Prev = NULL;
         LastPtr = WorkLine;           
      } else { 
         WorkLine->Prev = LastPtr;
         LastPtr->Next = WorkLine;
         WorkLine->Next = NULL;
         LastPtr = WorkLine;
      }
      (*Ptr) = WorkLine;
      ListCount++;
}


   void DeleteLine(Ptr)
      struct List_Element **Ptr;
   {
      struct List_Element *DelPtr;
 
     if ((*Ptr)->Next != NULL) {
           (*Ptr)->Next->Prev = (*Ptr)->Prev;
           if ((*Ptr)->Prev != NULL) 
               (*Ptr)->Prev->Next = (*Ptr)->Next;
           DelPtr = (*Ptr);
      } else {
         if((*Ptr)->Prev != NULL) {
            (*Ptr)->Prev->Next = NULL;
            DelPtr = (*Ptr);
            (*Ptr) = LastPtr = (*Ptr)->Prev;
         } else {
            DelPtr = (*Ptr);
            (*Ptr) =CurrentPtr = FirstPtr = LastPtr = NULL;
         }

      }           
      free(DelPtr);
      ListCount--;     
   }


   void  DeleteList(void)
   {
      struct List_Element *DelPtr;

      while (FirstPtr != NULL) {
         DelPtr = FirstPtr;
         FirstPtr = FirstPtr->Next;
         free(DelPtr);
      }  
      LastPtr = NULL;
      CurrentPtr = NULL;
      ListCount = 0;
   }


   void save_record_read(Rec)
   struct timebuff *Rec;
   {

         tbuff_copy(&readbuff.posted,Rec);

         readrab.rab$b_rac = RAB$C_KEY;
         readrab.rab$l_rbf = (char *)&readbuff;
         readrab.rab$w_rsz = sizeof readbuff;


         rms_status = sys$put(&readrab);

         if (!rmstest(rms_status,3,RMS$_NORMAL,RMS$_DUP,RMS$_OK_DUP))
            error_exit("$PUT");
/*         else
            if (rmstest(rms_status,2,RMS$_NORMAL,RMS$_OK_DUP))
                Status ("[Article Marked Read.]"); 
            else
               Status ("[Article already Marked as Read.]");
*/                  
   }

int if_record_read(Rec)
struct timebuff *Rec;
{

   readrab.rab$b_krf = 0;
   readrab.rab$l_kbf = (char *)Rec;
   readrab.rab$b_ksz = sizeof readbuff;
   readrab.rab$b_rac = RAB$C_KEY;
   readrab.rab$l_rop = RAB$M_RRL|RAB$V_CDK;

   rms_status = sys$find(&readrab);

   if (!rmstest(rms_status,3,RMS$_NORMAL,RMS$_RNF,RMS$_OK_RRL))
      error_exit("$FIND"); 
   else
      if (rms_status == RMS$_RNF) 
         return (FALSE) ; 
      else return(TRUE);
}  



int ListRead (ListLength,includefirst,reverse)
int ListLength,includefirst, reverse;
{
   int number_elements,first = TRUE;      
   char timestr[SIZE_TIMESTR],indent[5];
   $DESCRIPTOR(atime,timestr);
   struct List_Element *tempptr = NULL;
   char Headr[128] = "",Tailr[161]="";
   struct rfabuff rfar;
   int rflg;


    AllReadTop = AllReadBot = FALSE;

    if(!includefirst) {
       rab.rab$b_rac = RAB$C_SEQ;

    }

   for(number_elements = 0;number_elements < ListLength ;/* number_elements++*/) {
      rms_status = sys$get(&rab);

      if(includefirst && first) {
         rab.rab$b_rac = RAB$C_SEQ;
         first = FALSE;
      }

      if (!rmstest(rms_status,3,RMS$_NORMAL,RMS$_EOF,RMS$_OK_RRL)) 
         error_exit("$GET"); 
      else
         if (rms_status == RMS$_EOF) break;

      
  /*    if (ShowRead || (!if_record_read(&fileio.record.posted))){ */
      rflg = if_record_read(&fileio.record.posted);
      if (ShowRead || !rflg) {
    
         number_elements++;


         rms_status = sys$asctim(0,
                                 &atime,
                                 &fileio.record.posted,
                                 0);
         if ((rms_status & 1) != 1) lib$signal(rms_status);
         timestr[atime.dsc$w_length] = 0;


if (reverse) {
  
         if ( strncmp(PreviousReplyTop,fileio.record.replyto,SIZE_UID) != 0) 
            strcpy(indent,"");
         else strcpy(indent,"   ");

         if(Headr[0] != 0)  {
            PutOnTop(&tempptr);
            sprintf(tempptr->Prompt,"%s%s%s",
                    Headr,
                    indent,
                    Tailr);

             rfa_copy(&tempptr->rfa,&rfar);

         } else strncpy(TempReplyTo,fileio.record.replyto,SIZE_UID);


         sprintf(Headr,"%c%.*s %.*s",
                rflg ? ' ':'>',
                SIZE_TIMESTR, timestr,
                SIZE_UNAME, fileio.record.user_name);

         sprintf(Tailr,"%.*s",SIZE_SUBJECT,fileio.record.subject);

         rfa_copy(&rfar,&rab.rab$w_rfa);

         strncpy(PreviousReplyTop,fileio.record.replyto,SIZE_UID);
 
} else {
         PutOnBottom(&tempptr);

         rfa_copy(&tempptr->rfa,&rab.rab$w_rfa);

         if ( strncmp(PreviousReplyBot,fileio.record.replyto,SIZE_UID) == 0)
            strcpy(indent,"   ");
         else strcpy(indent,"");

         sprintf(tempptr->Prompt,"%c%.*s %.*s%s%.*s",
                rflg ? ' ':'>',
                SIZE_TIMESTR, timestr,
                SIZE_UNAME, fileio.record.user_name,
                indent,
                SIZE_SUBJECT, fileio.record.subject);         
         strncpy(PreviousReplyBot,fileio.record.replyto,SIZE_UID);

}
         


      }
  
   }

   if (number_elements>0 && reverse) {

      rms_status = sys$get(&rab);
      if (!rmstest(rms_status,3,RMS$_NORMAL,RMS$_EOF,RMS$_OK_RRL)) 
         error_exit("$GET"); 
      else
         if (rms_status != RMS$_EOF){
            if ( strncmp(PreviousReplyTop,fileio.record.replyto,SIZE_UID) != 0) 
               strcpy(indent,"");
            else strcpy(indent,"   ");
         } else  strcpy(indent,"");
  
      PutOnTop(&tempptr);
      sprintf(tempptr->Prompt,"%s%s%s",
                       Headr,
                       indent,
                       Tailr);
      rfa_copy(&tempptr->rfa,&rfar);
   }


   return number_elements;
}


void ReadMiddleOut(Rec)
struct rfabuff *Rec;
{
   struct listkey key;
   int number_elements,first = TRUE,testline;      
   char timestr[SIZE_TIMESTR],indent[5];
   $DESCRIPTOR(atime,timestr);
   struct List_Element *tempptr = NULL;
   struct listkey savkey;

   if(Rec == NULL) {
      Read_Directory(LISTKEY);
      return;
   }

   rab.rab$b_krf = 0;
   
   rab.rab$b_rac = RAB$C_RFA;
   rfa_copy(&rab.rab$w_rfa,Rec);
  
   rab.rab$l_ubf = (char *)&fileio; /*record; */
   rab.rab$w_usz = sizeof fileio;
   rab.rab$l_rop = RAB$M_RRL;

   rms_status = sys$get(&rab);

   if (!rmstest(rms_status,4,RMS$_NORMAL,RMS$_RNF,RMS$_RLK,RMS$_OK_RRL)) 
      error_exit("$GET"); 
   else
      if (rmstest(rms_status,2,RMS$_RNF,RMS$_RLK))
         if (rms_status == RMS$_RLK) Status ("Record Locked");
         else Status(">> RMSEXP - specified record does not exist."); 
      else {

 
         memcpy(key.msgid,fileio.record.msgid,SIZE_UID);
         memcpy(key.replyto,fileio.record.replyto,SIZE_UID);
         savkey = key;

         rab.rab$b_krf = REVERSEKEY;
         rab.rab$l_kbf = (char *)&key;
         rab.rab$b_ksz = sizeof key;
         rab.rab$b_rac = RAB$C_KEY;
         rab.rab$l_ubf = (char *)&fileio; /*record; */
         rab.rab$w_usz = sizeof fileio;
         rab.rab$l_rop = RAB$M_RRL;
      
         DeleteList();
                     
         CurrentLine = ListRead(CurrentLine+1,INCLUDEFIRST,TRUE);
         if (CurrentLine > 0) CurrentLine--;
/*Mid Pointer*/ CurrentPtr = LastPtr;

         key = savkey;

         rab.rab$b_krf = LISTKEY;
         rab.rab$l_kbf = (char *)&key;
         rab.rab$b_ksz = sizeof key;
         rab.rab$b_rac = RAB$C_KEY;
     /*
         rab.rab$b_rac = RAB$C_RFA;
         rfa_copy(&rab.rab$w_rfa,Rec);
     */
         rab.rab$l_ubf = (char *)&fileio; /*record; */
         rab.rab$w_usz = sizeof fileio;
         rab.rab$l_rop = RAB$M_RRL;

         rms_status = sys$get(&rab);



         if (!rmstest(rms_status,4,RMS$_NORMAL,RMS$_RNF,RMS$_RLK,RMS$_OK_RRL)) 
            error_exit("$GET"); 
         else
            if (rmstest(rms_status,2,RMS$_RNF,RMS$_RLK))
               if (rms_status == RMS$_RLK) Status ("Record Locked");
               else Status("Test Point RMSEXP - specified record does not exist."); 
            else {

                strcpy(PreviousReplyBot,TempReplyTo); 

                ListRead(BottomLine-(CurrentLine+TopLine) /* RegionLength/2*/,FALSE,FALSE);

                if(CurrentPtr == NULL) CurrentPtr = FirstPtr;

              /*  rmsget(Rec); /*Set Current Record to original value */
            }

 
      }


}

int ReadNextBlock (Count,Rec,reverse)
int Count;
struct rfabuff *Rec;
int reverse;
{
int number_elements, i;
      char outbuffer[255];
      int subjlen;
   char timestr[SIZE_TIMESTR], indent[10];
   $DESCRIPTOR(atime,timestr);
   struct List_Element *tempptr = CurrentPtr,*delptr;
   int rflg;
   struct listkey key;

   char Headr[1024] = "",Tailr[1024]="";
   struct rfabuff rfar;
   outbuffer[0]=0;

if(reverse && AllReadBot) AllReadBot = FALSE;
if((!reverse) && AllReadTop) AllReadTop = FALSE;

/* ZEN 23/10/2023  removing for test
if (AllReadBot || AllReadTop) return(FALSE);  
*/

   rab.rab$b_krf = 0;

   rab.rab$b_rac = RAB$C_RFA;
   rfa_copy(&rab.rab$w_rfa,Rec);

   rab.rab$l_ubf = (char *)&fileio; /*record; */
   rab.rab$w_usz = sizeof fileio;
   rab.rab$l_rop = RAB$M_RRL;

   rms_status = sys$get(&rab);

   if (!rmstest(rms_status,4,RMS$_NORMAL,RMS$_RNF,RMS$_RLK,RMS$_OK_RRL)) 
      error_exit("$GET"); 
   else
      if (rmstest(rms_status,2,RMS$_RNF,RMS$_RLK))
         if (rms_status == RMS$_RLK) Status ("Record Locked");
         else Status("Test Point 1 RMSEXP - specified record does not exist."); 
      else {
 
         memcpy(key.msgid,fileio.record.msgid,SIZE_UID);
         memcpy(key.replyto,fileio.record.replyto,SIZE_UID);

if(reverse)
         rab.rab$b_krf = REVERSEKEY;
else          
         rab.rab$b_krf = LISTKEY;

         rab.rab$l_kbf = (char *)&key;
         rab.rab$b_ksz = sizeof key;

         rab.rab$b_rac = RAB$C_KEY;
         rab.rab$l_ubf = (char *)&fileio; /*record; */
         rab.rab$w_usz = sizeof fileio;
         rab.rab$l_rop = RAB$M_RRL;
      
         rms_status = sys$get(&rab);

         if (!rmstest(rms_status,4,RMS$_NORMAL,RMS$_RNF,RMS$_RLK,RMS$_OK_RRL)) 
            error_exit("$GET"); 
         else
            if (rmstest(rms_status,2,RMS$_RNF,RMS$_RLK))
               if (rms_status == RMS$_RLK) Status ("Record Locked");
               else Status("Test Point 2 RMSEXP - specified record does not exist."); 
            else {
 
 
               rab.rab$b_rac = RAB$C_SEQ;

               for(number_elements = 0;number_elements < Count ;/* number_elements++ */) {

                  rms_status = sys$get(&rab);

                  if (!rmstest(rms_status,3,RMS$_NORMAL,RMS$_EOF,RMS$_OK_RRL)) 
                     error_exit("$GET"); 
                  else
                     if (rms_status == RMS$_EOF){
if(reverse)             AllReadTop = TRUE;
else                    AllReadBot = TRUE;
                        break;
                     }
         /*         if (ShowRead || (!if_record_read(&fileio.record.posted))){ */
                  rflg = if_record_read(&fileio.record.posted);
                  if (ShowRead || !rflg) {
                     number_elements++;
if (reverse) {
                     if(ListCount > RegionLength*2)
                        if(LastPtr != NULL && LastPtr->Prev != NULL) {
                           delptr = LastPtr;
                           LastPtr = LastPtr->Prev;
                           LastPtr->Next = NULL;
                           free(delptr);
                           ListCount--;
                        }
}else {
                     if(ListCount > RegionLength*2)
                        if(FirstPtr != NULL && FirstPtr->Next != NULL) {
                           delptr = FirstPtr;
                           FirstPtr = FirstPtr->Next;
                           FirstPtr->Prev = NULL;
                           free(delptr);
                           ListCount--;
                        }
}


 
                     rms_status = sys$asctim(0,
                              &atime,
                              &fileio.record.posted,
                              0);
                     if ((rms_status & 1) != 1) lib$signal(rms_status);
                     timestr[atime.dsc$w_length] = 0;

if (reverse) {

                     if ( strncmp(PreviousReplyTop,fileio.record.replyto,SIZE_UID) != 0) 
                        strcpy(indent,"");
                     else strcpy(indent,"   ");
                     strncpy(PreviousReplyTop,fileio.record.replyto,SIZE_UID);

                     if(Headr[0] != 0)  {
                        PutOnTop(&tempptr);
                        sprintf(tempptr->Prompt,"%s%s%s",
                                Headr,
                                indent,
                                Tailr);
                        rfa_copy(&tempptr->rfa,&rfar);
                     }


                     sprintf(Headr,"%c%.*s %.*s",
                            rflg ? ' ':'>',
                            SIZE_TIMESTR, timestr,
                            SIZE_UNAME, fileio.record.user_name);
              
                     sprintf(Tailr,"%.*s",SIZE_SUBJECT,fileio.record.subject);
                     rfa_copy(&rfar,&rab.rab$w_rfa);

} else {

                     PutOnBottom(&tempptr);

                     rfa_copy(&tempptr->rfa,&rab.rab$w_rfa);

                     if ( strncmp(PreviousReplyBot,fileio.record.replyto,SIZE_UID) == 0) 
                        strcpy(indent,"   ");
                     else strcpy(indent,"");
                     strncpy(PreviousReplyBot,fileio.record.replyto,SIZE_UID);

                     sprintf(tempptr->Prompt,"%c%.*s %.*s%s%.*s",
                            rflg ? ' ':'>',
                            SIZE_TIMESTR, timestr,
                            SIZE_UNAME, fileio.record.user_name,
                            indent,
                            SIZE_SUBJECT,fileio.record.subject);

}
   
                     if (number_elements == 0) 
                        Status (" [End of Records.]") ;  
                  }
               }

               if (number_elements>0 && reverse) {

                  rms_status = sys$get(&rab);
                  if (!rmstest(rms_status,3,RMS$_NORMAL,RMS$_EOF,RMS$_OK_RRL)) 
                     error_exit("$GET"); 
                  else
                     if (rms_status != RMS$_EOF){
                        if ( strncmp(PreviousReplyTop,fileio.record.replyto,SIZE_UID) != 0) 
                           strcpy(indent,"");
                        else strcpy(indent,"   ");
                     } else strcpy(indent,"");

                  PutOnTop(&tempptr);
                  sprintf(tempptr->Prompt,"%s%s%s",
                          Headr,
                          indent,
                          Tailr);
                  rfa_copy(&tempptr->rfa,&rfar);
                    
               }
            } /* end of if     */
        }   
   if(number_elements > 0) 
      return TRUE;
   else
      return FALSE;
}


int Read_Directory(index_key)
int index_key;
{
   int number_elements;
   
   rab.rab$b_krf = index_key;

   rms_status = sys$rewind(&rab);
   if (rms_status != RMS$_NORMAL) 
      error_exit("$REWIND"); 


   rab.rab$b_rac = RAB$C_SEQ;
   rab.rab$l_ubf = (char *)&fileio;   /*  record;  */
   rab.rab$w_usz = sizeof fileio;  /*RECORD_SIZE;  */
   rab.rab$l_rop = RAB$M_RRL;

   DeleteList();
   number_elements = ListRead(RegionLength,FALSE,
                              REVERSEKEY == index_key ? TRUE : FALSE); 

   if (number_elements == 0) 
      Status (" [There are no unread messages.]") ;  
   return(number_elements);
}



   void Redraw_Screen(void)
   {
      struct List_Element *curs2;
      int LineNum = 0, i,trip = FALSE;
   
      Clear_Region(TopLine,BottomLine);
      if (FirstPtr != NULL) {
         curs2 = CurrentPtr;
         for (i= 1; i <= CurrentLine; i++ ) {
            if (curs2 != FirstPtr)
               curs2 = curs2->Prev;
           
         }

         while (curs2 != NULL) { 
            Add(Standard_Window,TopLine + LineNum,2,curs2->Prompt);
            clreol(Standard_Window);

            LineNum++;
            if (LineNum+ TopLine > BottomLine) break;

            if (!trip && curs2->Next == NULL) {
               trip = TRUE;
               ReadNextBlock (RegionLength,&curs2->rfa,FALSE);
            }

            curs2 = curs2->Next;
         }
         Refresh(Standard_Window);
      }
   }


   void ReRead_Directory(void)
   {
      Read_Directory(LISTKEY);
      Redraw_Screen();
   }



pad_record()
{
   int i;

   for (i = strlen(fileio.record.subject); i< SIZE_SUBJECT; i++)
      fileio.record.subject[i] = 0;
/*   for (i = strlen(fileio.record.last_name); i< SIZE_LNAME; i++)
      fileio.record.last_name[i] = ' ';
   for (i = strlen(fileio.record.first_name); i < SIZE_FNAME; i++)
      fileio.record.first_name[i] = ' ';
   for (i = strlen(fileio.record.comments); i < SIZE_COMMENTS; i++)
      fileio.record.comments[i] = ' ';   */
}

void mkuid(dest,time)
char *dest;
struct timebuff time;
{
   static unsigned short int numvec[7];
   int status;

   status = sys$numtim (numvec,&time);

   (void)sprintf (dest,"%04hu%02hu%02hu%02hu%02hu%02hu%02hu",
                  numvec[0],
                  numvec[1],
                  numvec[2],
                  numvec[3],
                  numvec[4],
                  numvec[5],
                  numvec[6]);
}



   int ReadSubject (initial_value)
      char *initial_value;
   {
     unsigned int status,i;
     uint16 lnth, terminator_code;
     $DESCRIPTOR(subject_d,fileio.record.subject);
     $DESCRIPTOR(prompt_d,"Subject : ");
     $DESCRIPTOR(initstr_d,initial_value);

     prompt_d.dsc$w_length = 10;
     initstr_d.dsc$w_length = strlen(initial_value);

     status = smg$end_display_update(&Standard_Window);
     for(;;) {
        set_cursor(Standard_Window,StatusLine,1);
        clreol(Standard_Window);
        status =smg$read_string(&KeyBoard,
                                &subject_d,
                                &prompt_d,
                                0,
                                0,
                                0,
                                0,
                                &lnth,
                                &terminator_code,
                                &Standard_Window,
                                &initstr_d); 
        if (terminator_code == SMG$K_TRM_CTRLZ) return(FALSE);
        if (lnth != 0) break;
     }
 
     Status("");
 
     fileio.record.subject[lnth] = 0;

     for (i = strlen(fileio.record.subject); i< SIZE_SUBJECT; i++)
         fileio.record.subject[i] = 0;

     return(TRUE);

   }



   void delete_editor_buffer(listptr)
   struct line_rec **listptr;
   {
      struct line_rec *delptr;

      while ((*listptr) != NULL && (*listptr)->last != NULL) (*listptr) = (*listptr)->last;
                
      while ((*listptr) != NULL) {
         delptr = (*listptr);
         (*listptr) = (*listptr)->next;
         free(delptr);
      }

   }



   void save_file(update)
   int update;
   {
     struct line_rec *curs;
     int ind = 0,j;
    
      if (start_line != NULL) {
          
          
          curs = start_line;
          
          while(curs != NULL && curs->last != NULL) curs = curs->last;

          while (curs != NULL && ind < TOTAL_LINES) {
       
             strcpy(fileio.lines[ind],curs->storage);
             for (j = strlen(fileio.lines[ind]); j < LINE_LENGTH; j++)
                 fileio.lines[ind][j] = 0;
        
             ind++;

             curs = curs->next;
          }

          delete_editor_buffer(&start_line);


         rab.rab$b_rac = RAB$C_KEY;
         rab.rab$l_rbf = (char *)&fileio;
         fileio.record.reclen = RECORD_SIZE + (ind * LINE_LENGTH);
         rab.rab$w_rsz = fileio.record.reclen;


if(update) {

         rms_status = sys$update(&rab);
         if (rms_status != RMS$_NORMAL) 
            error_exit("$UPDATE");

 
} else {

         rms_status = sys$put(&rab);
}


         if (!rmstest(rms_status,3,RMS$_NORMAL,RMS$_DUP,RMS$_OK_DUP))
            error_exit("$PUT");
         else
            if (rmstest(rms_status,2,RMS$_NORMAL,RMS$_OK_DUP))
               Status ("[Record Written.]");
            else
               Status ("RMSEXP - Existing record with same key. not written.");
                   
       }
   }

   void get_username(void)
   {
      static int i, item_code = JPI$_USERNAME; /*JPI$_PRCNAM;*/
      $DESCRIPTOR(user_name_d,user_name);
      uint16 lnth;

      rms_status = lib$getjpi (&item_code,
                               0,
                               0,
                               0,
                               &user_name_d,
                               &lnth);  /*user_name_d.dsc$w_length); */

/*      user_name[user_name_d.dsc$w_length] = 0; */

      for (i=lnth-1;user_name[i]==' ';i--) user_name[i] = 0;

   }


int allocate_line (ptr,first_line)
struct line_rec **ptr,**first_line;
{
   struct line_rec *memptr /*,*first_line*/;
     if ((memptr =(struct line_rec *) malloc(sizeof(struct line_rec))) == NULL){
         Status("Memory Error");
         error_exit("Memory Allocation");
      } else {
          memptr->storage[0] = 0;
          memptr->next = NULL;
          memptr->last = NULL;
            
          if ((*ptr) == NULL) {
             (*first_line) = (*ptr) = memptr;                  
          } else {
             (*ptr)->next = memptr;
             memptr->last = (*ptr);
             (*ptr) = memptr;
          }
          return TRUE;
      }
      return FALSE;
}

void load_editor_buffer (ptr,quote)
struct line_rec **ptr;
int quote;
{
   int NumOfLines,i,first = TRUE;
   struct line_rec *memptr,*first_line = NULL;

   NumOfLines = (fileio.record.reclen - RECORD_SIZE )/ LINE_LENGTH;


   for (i=0;i<NumOfLines; i++) {
         if (allocate_line(ptr,&first_line)) {
            if(quote) {
                if (first){ 
                   sprintf((*ptr)->storage,"> %.*s wrote..",SIZE_UNAME,fileio.record.user_name);
                   first = FALSE;
                   allocate_line(ptr,&first_line);
                }
                sprintf((*ptr)->storage,">%s",fileio.lines[i]);
             }else
                strcpy((*ptr)->storage,fileio.lines[i]);
         }
      
   }
   /* ZEN  */
   if(quote) {
      allocate_line(ptr,&first_line);
      sprintf((*ptr)->storage,"");
   }
   /* Mod Ends */
   (*ptr) = first_line;         
}


   void Post_Message (Rec,update)
   struct rfabuff *Rec;
   int update;
   {
      $DESCRIPTOR(user_name_d,fileio.record.user_name);
      static int item_code = JPI$_USERNAME; /* JPI$_PRCNAM;*/
      struct line_rec *delptr;
      int retcode,edited = FALSE, exit = FALSE;
      char outbuffer[300];
      int SnapTo;

      Clear_Region(TopLine,BottomLine);
 
      if(!ReadSubject("")) return;


      rms_status = lib$getjpi (&item_code,
                               0,
                               0,
                               0,
                               &user_name_d,
                               &user_name_d.dsc$w_length);



      set_cursor(Standard_Window,2,1);
      sprintf(outbuffer,"Sender  : %.*s",SIZE_UNAME,fileio.record.user_name); 
      put_line(Standard_Window,outbuffer); 

      sprintf(outbuffer,"Subject : %.*s\n\n",SIZE_SUBJECT,fileio.record.subject);
      put_line(Standard_Window,outbuffer); 
 
/*      sprintf(outbuffer,"Posted  : %.*s",SIZE_TIMESTR-1,timestr); 
      put_line(Standard_Window,outbuffer); 
*/
  
      put_line(Standard_Window,"Enter your text.  ^Z to exit");
      

      delete_editor_buffer(&start_line);

      while(!exit) {
         Help("Texaco v3.01    ^Z exit  ^L insert line  ^D delete line"); 

         texaco (&start_line,TopLine+4,BottomLine-1,&retcode,&edited,TOTAL_LINES,RIGHT_MARGIN,linedt,INSERT_MODE | WORD_WRAP,NULL);

         if (edited) {

            Help("Exit Editor :  Y/N");

            switch(Get_Keystroke()) {
            case 'y':
               Help("Save :  Y/N");
               switch(Get_Keystroke()) {
               case 'y':                                  
                  rms_status = sys$gettim(&fileio.record.posted);
                  if ((rms_status & 1) != 1) lib$signal( rms_status);

                  mkuid(fileio.record.msgid,fileio.record.posted);
                  mkuid(fileio.record.replyto,fileio.record.posted);

                  save_file(update);
                  exit = TRUE; 
 

                   if(FirstPtr == NULL) {
                      Read_Directory(LISTKEY);
                      CurrentPtr = FirstPtr;  
                   } else {                     
                      SnapTo = RegionLength/2;
                      if(CurrentLine < SnapTo) CurrentLine = (SnapTo<ListCount)?SnapTo:ListCount;
                      ReadMiddleOut(&rab.rab$w_rfa);
                   }
                   Help("");
               case 'n':
                   exit = TRUE;
                   Help("");
                   break;
               }
               

 
               break;
            case 'n' :
              /*  exit = TRUE;   */
               Help("");
               break;
            }
         } else exit = TRUE;
      }

      delete_editor_buffer(&start_line);
 

   }


   void Post_Reply (update)
   int update;
   {
      $DESCRIPTOR(user_name_d,fileio.record.user_name);
      static int item_code = JPI$_USERNAME; /* JPI$_PRCNAM;*/
      struct line_rec *delptr;
      int retcode,edited = FALSE, exit = FALSE;

      char timestr[SIZE_TIMESTR],outbuffer[300];
      $DESCRIPTOR(timestrdsc,timestr);
      int status;

      outbuffer[0] = 0;

if(update){
      if (strncmp(fileio.record.user_name,user_name,strlen(user_name)) != 0){
         Status("You are not authorised to write to this message");            
         return;         
      } else Status("Editing Message.");
}

      Clear_Region(TopLine,BottomLine);

      load_editor_buffer(&start_line,!update);

if (!update) {
      if (strncmp(fileio.record.subject,"Re.",3) != 0) {
         strcpy (outbuffer,"Re. ");
         strncat (outbuffer,fileio.record.subject,SIZE_SUBJECT);
      }else
         strncpy(outbuffer,fileio.record.subject,SIZE_SUBJECT);
      
      if(!ReadSubject(outbuffer)) {
         delete_editor_buffer(&start_line);
         return;
      }

      rms_status = lib$getjpi (&item_code,
                               0,
                               0,
                               0,
                               &user_name_d,
                               &user_name_d.dsc$w_length);

      Clear_Region(TopLine,BottomLine);
}
 
      set_cursor(Standard_Window,2,1);
      sprintf(outbuffer,"Sender  : %.*s",SIZE_UNAME,fileio.record.user_name); 
      put_line(Standard_Window,outbuffer); 

      sprintf(outbuffer,"Subject : %.*s",SIZE_SUBJECT,fileio.record.subject);
      put_line(Standard_Window,outbuffer); 

 

      rms_status = sys$asctim(0,
                              &timestrdsc,
                              &fileio.record.posted,
                              0);
      if ((rms_status & 1) != 1) lib$signal(rms_status);
 
      sprintf(outbuffer,"Posted  : %.*s\n",SIZE_TIMESTR-1,timestr); 
      put_line(Standard_Window,outbuffer); 

      put_line(Standard_Window,"Enter your text.  ^Z to exit");
   

      Refresh(Standard_Window);


      while(!exit) {
         Help("Texaco v3.01    ^Z exit  ^L insert line  ^D delete line");

         texaco (&start_line,TopLine+4,BottomLine-1,&retcode,&edited,TOTAL_LINES,RIGHT_MARGIN,linedt,INSERT_MODE | WORD_WRAP,NULL);

         if (edited) {
      
            Help("Exit Editor :  Y/N");

            switch(Get_Keystroke()) {
            case 'Y':
            case 'y':
               Help("Save :  Y/N");
               switch(Get_Keystroke()){
               case 'Y':
               case 'y':

if (!update) {
/* ZEN do not update replyto field
              if (strcmp(fileio.record.msgid,fileio.record.replyto) == 0)
                  mkuid(fileio.record.replyto,fileio.record.posted);
*/                                                 
               rms_status = sys$gettim(&fileio.record.posted);
               if ((rms_status & 1) != 1) lib$signal( rms_status);

               mkuid(fileio.record.msgid,fileio.record.posted);               
}
               save_file(update);
if(!update){
               
                if(FirstPtr == NULL) {
                   Read_Directory(LISTKEY);
                   CurrentPtr = FirstPtr;
                } else {
                   rmsget(&CurrentPtr->rfa); /*ZEN 30/10/2023  */
                   if(ShowRead) {
                      ReadMiddleOut(&CurrentPtr->rfa);
                      rmsget(&CurrentPtr->rfa);                      
                   }
                   
                }

}
               case 'N':
               case 'n':
                  break;
               }
               Help("");
               exit = TRUE;
               break;
            case 'N' :
            case 'n' :
              /* ZEN exit = TRUE; */
               Help("");
               break;
            }
         } else exit = TRUE;
      }

      delete_editor_buffer(&start_line);


   }


   int Count_Back(Csr)
   struct List_Element *Csr;
   {
      struct List_Element *CountCsr = Csr;
      int Counter = 1;
   
      for (;;) {
         if (CountCsr = FirstPtr) break;
         CountCsr = CountCsr->Prev;
         Counter++;
      }
      return Counter;
   }


int rmsget(Rec)
struct rfabuff *Rec;
{

   rab.rab$b_krf = 0;
   rab.rab$b_rac = RAB$C_RFA;
   rfa_copy(&rab.rab$w_rfa,Rec);

   rab.rab$l_ubf = (char *)&fileio; /*record; */
   rab.rab$w_usz = sizeof fileio;
   rab.rab$l_rop = RAB$M_RRL;

   rms_status = sys$get(&rab);

   if (!rmstest(rms_status,5,RMS$_NORMAL,RMS$_RNF,RMS$_RLK,RMS$_OK_RRL,RMS$_EOF)) 
      error_exit("$GET"); 
   else
      if (rmstest(rms_status,2,RMS$_RNF,RMS$_RLK))
         if (rms_status == RMS$_RLK) Status ("Record Locked");
         else Status("RMSEXP - specified record does not exist."); 
      else {
        
      
         return TRUE;
                     
      }
      return FALSE;

}


   void display_post(Rec)
   struct rfabuff *Rec;
   {
      char timestr[SIZE_TIMESTR],outbuffer[1024];
      $DESCRIPTOR(timestrdsc,timestr);
      int retcode,edited,Exit = FALSE,EndThread = FALSE;
      do { 
         save_record_read(&fileio.record.posted);
    
         Clear_Region(TopLine,BottomLine);         

         sprintf(outbuffer,"Sender  : %.*s",SIZE_UNAME,fileio.record.user_name); 
         put_line(Standard_Window,outbuffer); 


         sprintf(outbuffer,"Subject : %s",fileio.record.subject);
         put_line(Standard_Window,outbuffer); 

          rms_status = sys$asctim(0,
                                 &timestrdsc,
                                 &fileio.record.posted,
                                 0);
         if ((rms_status & 1) != 1) lib$signal(rms_status);
 
         sprintf(outbuffer,"Posted  : %.*s",SIZE_TIMESTR-1,timestr); 
         put_line(Standard_Window,outbuffer); 
         Refresh(Standard_Window);

         delete_editor_buffer(&start_line);

         load_editor_buffer(&start_line,FALSE);

         Help("|          |^R Reply  |^E Edit   |^N Next   |^P Prev   |^Z exit  PgUp/PgDn Page");


         read_only = TRUE;
         texaco (&start_line,TopLine+4,BottomLine-1,&retcode,&edited,TOTAL_LINES,RIGHT_MARGIN,linedt,INSERT_MODE | WORD_WRAP,NULL);
         read_only = FALSE;

         delete_editor_buffer(&start_line);
 
         
         switch (retcode) {
            case SMG$K_TRM_CTRLZ:
               Exit = TRUE;
               break;
            case SMG$K_TRM_CTRLR:
            case SMG$K_TRM_PF2:
               Post_Reply(REPLY_MODE);
              /* Exit = TRUE; */
               break;
            case SMG$K_TRM_CTRLE:
            case SMG$K_TRM_PF3:            
               Post_Reply(EDIT_MODE);
             /*  Exit = TRUE; */
               break;
            case SMG$K_TRM_CTRLN:
            case SMG$K_TRM_PF4:
               if (CurrentPtr->Next != NULL)
                  CurrentPtr = CurrentPtr->Next;
               else {
                  if (ReadNextBlock(5,&CurrentPtr->rfa,FALSE))
                     CurrentPtr = CurrentPtr->Next;
               } 

               strncpy(TempReplyTo,fileio.record.replyto,SIZE_UID);               

               rmsget(&CurrentPtr->rfa);

               if ( strncmp(TempReplyTo,fileio.record.replyto,SIZE_UID) != 0){
                 
                  if (CurrentPtr->Prev != NULL) {
                     CurrentPtr = CurrentPtr->Prev; /* TEST*/

                     rmsget(&CurrentPtr->rfa);
                  }
                  Status("[End of Thread]");
                  EndThread = TRUE;
               } else Status(""); 
               if(!EndThread) {
                  if (TopLine + CurrentLine < BottomLine &&
                      CurrentPtr->Next != NULL) CurrentLine++;
                  *CurrentPtr->Prompt = ' ';
               } else EndThread = FALSE;
               break;
            case SMG$K_TRM_CTRLP:
            case SMG$K_TRM_F5:

               if (CurrentPtr->Prev != NULL)
                  CurrentPtr = CurrentPtr->Prev;
               else {
                  if (ReadNextBlock(5,&CurrentPtr->rfa,TRUE))
                     CurrentPtr = CurrentPtr->Prev;
               }

               strncpy(TempReplyTo,fileio.record.replyto,SIZE_UID);               
         
               rmsget(&CurrentPtr->rfa);

               if ( strncmp(TempReplyTo,fileio.record.replyto,SIZE_UID) != 0){
                  if (CurrentPtr->Next != NULL){
                     CurrentPtr = CurrentPtr->Next; /* TEST */
                     rmsget(&CurrentPtr->rfa);
                  }
                  Status("[Beginning of Thread]");
                  EndThread = TRUE; 
               } else Status("");
               if(!EndThread) {
                  if(CurrentLine > 0) CurrentLine--;
                  *CurrentPtr->Prompt = ' ';
               } else EndThread = FALSE;
               break;
               
         }

      } while (!Exit);

   }



   void Read_Post(Rec)
   struct rfabuff *Rec;
   {

      rab.rab$b_krf = 0;

      rfa_copy(&rab.rab$w_rfa,Rec);
      rab.rab$b_rac = RAB$C_RFA;

      rab.rab$l_ubf = (char *)&fileio; /*record; */
      rab.rab$w_usz = sizeof fileio;

      rms_status = sys$get(&rab);

      if (!rmstest(rms_status,4,RMS$_NORMAL,RMS$_RNF,RMS$_RLK,RMS$_OK_RRL)) 
         error_exit("$GET"); 
      else
         if (rmstest(rms_status,2,RMS$_RNF,RMS$_RLK))
            if (rms_status == RMS$_RLK) Status ("Record Locked");
            else Status("RMSEXP - specified record does not exist."); 
         else {
            Status("");
            save_record_read(&fileio.record.posted);      
            *CurrentPtr->Prompt = ' ';
            display_post(Rec);
         }
 
   }

   void execute_post(update)
   int update;
   {
      if (update) {         
         Post_Reply(update);
      }else {
         Status("Reply To Message.");             
         Post_Reply(update);
      }    
   }

   void Read_and_Reply(Rec,update)
   struct rfabuff *Rec;
   int update;
   {
      char outbuffer[255];
      int subjlen;

      outbuffer[0]=0;


      rab.rab$b_krf = 0;

      rab.rab$b_rac = RAB$C_RFA;
      rfa_copy(&rab.rab$w_rfa,Rec);

      rab.rab$l_ubf = (char *)&fileio; /*record; */
      rab.rab$w_usz = sizeof fileio;
      rab.rab$l_rop = RAB$M_RRL;

      rms_status = sys$get(&rab);

      if (!rmstest(rms_status,4,RMS$_NORMAL,RMS$_RNF,RMS$_RLK,RMS$_OK_RRL)) 
         error_exit("$GET"); 
      else
         if (rmstest(rms_status,2,RMS$_RNF,RMS$_RLK))
            if (rms_status == RMS$_RLK) Status ("Record Locked");
            else Status("RMSEXP - specified record does not exist."); 
         else {
            execute_post(update);
         }
 
   /*   Status("Leaving Reader"); */

   }

   void execute_delete(void)
   {
         rms_status = sys$delete(&rab); 
         if (rms_status !=RMS$_NORMAL)
            error_exit("$DELETE");

          
   /*      if (strncmp(PreviousReplyTop,fileio.record.replyto,SIZE_UID) == 0)  
            DeleteLine(&CurrentPtr);
         else {    */                    
            DeleteLine(&CurrentPtr);
            if (CurrentPtr != NULL && CurrentPtr->Next == NULL) CurrentLine--;
   /*      }    */
   }

   void Delete_Post(Rec)
   struct rfabuff *Rec;
   {
   
      rab.rab$b_krf = 0;

      rab.rab$b_rac = RAB$C_RFA;
      rfa_copy(&rab.rab$w_rfa,Rec);

      rab.rab$l_ubf = (char *)&fileio; /*record; */
      rab.rab$w_usz = sizeof fileio;

      rab.rab$l_rop = RAB$M_RRL;

      rms_status = sys$get(&rab);

      if (!rmstest(rms_status,3,RMS$_NORMAL,RMS$_RNF,RMS$_OK_RRL))
         error_exit("$FIND"); 
      else
      if (rms_status == RMS$_RNF) 
         Status("RMSEXP - specified record does not exist."); 
      else if (rms_status == RMS$_OK_RRL) {
         Status("RMSEXP - specified record is locked.");
   /*      execute_delete(); */
      } else {
         execute_delete();
      } 
   }

   void align_list(CPtr,FindRec)
   struct List_Element **CPtr;
   struct rfabuff *FindRec;
   {
      int i;

      return;  /* Disable  */

      (*CPtr) = FirstPtr;
      while ((*CPtr) != NULL) {
         if (rfa_equal((*CPtr)->rfa,(*FindRec))) break;
            (*CPtr) = (*CPtr)->Next;
      }
      if((*CPtr) == NULL){
         (*CPtr) = FirstPtr;
         for(i=0;i< CurrentLine;i++)
            if ((*CPtr)->Next != NULL) (*CPtr) = (*CPtr)->Next;
 
      }
   }

   void active_align_list(CPtr,FindRec)
   struct List_Element **CPtr;
   struct rfabuff *FindRec;
   {
      int i;

      (*CPtr) = FirstPtr;
      while ((*CPtr) != NULL) {
         if (rfa_equal((*CPtr)->rfa,(*FindRec))) break;
            (*CPtr) = (*CPtr)->Next;
      }
      if((*CPtr) == NULL){
         (*CPtr) = FirstPtr;
         for(i=0;i< CurrentLine;i++)
            if ((*CPtr)->Next != NULL) (*CPtr) = (*CPtr)->Next;
 
      }
   }


   void get_findrec(CPtr,FindRec)
   struct List_Element **CPtr;
   struct rfabuff *FindRec;
   {
       if ((*CPtr) != NULL)
           rfa_copy(FindRec,&(*CPtr)->rfa);
   }   

   void RefreshList(void)
   {
      if(FirstPtr == NULL) {   
         Read_Directory(LISTKEY);
         CurrentPtr = FirstPtr;
      } else {
         ReadMiddleOut(&CurrentPtr->rfa);
      }
   }


int set_all_read(void)
{
   int number_messages, i;
   char outline[80];
   
   rab.rab$b_krf = 0;

   rms_status = sys$rewind(&rab);
   if (rms_status != RMS$_NORMAL) 
      error_exit("$REWIND"); 

   rab.rab$b_rac = RAB$C_SEQ;
   rab.rab$l_ubf = (char *)&fileio; /*record; */
  
   rab.rab$w_usz = sizeof fileio;  /*RECORD_SIZE;  */
   rab.rab$l_rop = RAB$M_RRL;


   for(number_messages = 0; ; number_messages++) {
      rms_status = sys$get(&rab);
      if (!rmstest(rms_status,3,RMS$_NORMAL,RMS$_EOF,RMS$_OK_RRL)) 
         error_exit("$GET"); 
      else
         if (rms_status == RMS$_EOF) break;

         save_record_read(&fileio.record.posted);     
         sprintf (outline," [Marked %6d messages Read]",number_messages) ;
         Status (outline);
   }
 

   sprintf (outline," [Marked %6d messages Read]",number_messages) ;
   Status (outline);

   RefreshList();
   align_list(&CurrentPtr,&FindRec);  
   Redraw_Screen();
  
   return(TRUE);
}



int PF1key (void)
{
   get_findrec(&CurrentPtr,&FindRec);
   Post_Message(&CurrentPtr->rfa,FALSE);
   align_list(&CurrentPtr,&FindRec);  
   Redraw_Screen();
   return(TRUE);
}

int PF2key (void)
{
   get_findrec(&CurrentPtr,&FindRec);
   Read_and_Reply(&CurrentPtr->rfa,REPLY_MODE);
   align_list(&CurrentPtr,&FindRec);  
   Redraw_Screen();
   return(TRUE);
}

int PF3key (void)
{
   get_findrec(&CurrentPtr,&FindRec);
   Read_and_Reply(&CurrentPtr->rfa,EDIT_MODE);
   align_list(&CurrentPtr,&FindRec);  
   Redraw_Screen();
   return(TRUE);
}

int PF4key (void)
{

   if(!ShowRead) return(FALSE); /* Do Not Allow delete in Unread mode */

   if (strcmp(user_name,PRIV_USER) != 0) {
      rab.rab$b_rac = RAB$C_RFA;
      rfa_copy(&rab.rab$w_rfa,&CurrentPtr->rfa);
      rab.rab$l_ubf = (char *)&fileio; /*record; */
      rab.rab$w_usz = sizeof fileio;
      rab.rab$l_rop = RAB$M_RRL;
      rms_status = sys$get(&rab);

      if (!rmstest(rms_status,2,RMS$_NORMAL,RMS$_OK_RRL)) {
         warn_user("RMS error %u  %s %u",rms_status,filterfn(__FILE__),__LINE__);
         return(FALSE);
      }


      if (strncmp(fileio.record.user_name,user_name,strlen(user_name)) != 0)   {
         warn_user("Not Authorised to Delete this Post %s %u",filterfn(__FILE__),__LINE__);
         return(FALSE);
      }
   }


   Help("Do you want to Delete this record :  Y/N");
   switch(Get_Keystroke()) {
      case 'Y':
      case 'y':
         if (CurrentPtr != NULL)
            if(CurrentPtr->Next != NULL)
               rfa_copy(&FindRec,&CurrentPtr->Next->rfa);
            else if(CurrentPtr->Prev!= NULL){ 
               rfa_copy(&FindRec,&CurrentPtr->Prev->rfa);
            }
            Delete_Post(&CurrentPtr->rfa);
              
/*          Read_Directory(LISTKEY);    */
/*          DeleteLine(&CurrentPtr);     */

            active_align_list(&CurrentPtr,&FindRec);
            break;
      case 'n':
      case 'N' :break;
      default : break;
   }
   Redraw_Screen();

   return(TRUE);
}

int toggle_unread(void)
{
   char outstr[20];
   static struct rfabuff StoreRFA;
   static int StoreLine;

   if (ShowRead) {
      ShowRead = FALSE;
      strcpy(outstr,"Unread  ");
      rfa_copy(&StoreRFA,&CurrentPtr->rfa);
      StoreLine = CurrentLine;
   } else{
      ShowRead = TRUE;
      strcpy(outstr,"All Read");
   }
   Add_Attr(Standard_Window,1,60,outstr,SMG$M_UNDERLINE);
   Refresh(Standard_Window);

   CurrentLine = StoreLine;
   ReadMiddleOut(&StoreRFA);

   Redraw_Screen();
   return(TRUE);
}




int reread_index (void)
{
   RefreshList();
   Redraw_Screen();
   return(TRUE);
}


struct Menu_Record MenuSpec[] = {
   {"Post Message",PF1key},
   {"Reply",PF2key},
   {"Edit Message",PF3key},
   {"Delete Message",PF4key},
   {"Mark All Read",set_all_read},
   {"Toggle Un/Read",toggle_unread},
   {"ReRead Index",reread_index},
   {"",NULL}   
};



   void Read_Messages(void)
   {
      uint16 c;
      int i, exit = FALSE, ende = TRUE;
      int swtch;

      Clear_Region(TopLine,BottomLine);
      CurrentLine = 0;
      Read_Directory(REVERSEKEY/*LISTKEY*/);
      CurrentPtr = FirstPtr;
      Redraw_Screen();

      while (!exit) {
         Help("|^P Post   |^R Reply  |^E Edit   |^D Delete |^W Reindex|^U Un/Read| PgUp/PgDn  ^Z to exit");

         if (FirstPtr != NULL) {
            HiLite(Standard_Window,CurrentPtr->Prompt,CurrentLine+TopLine);
            clreol(Standard_Window);

         }
         
         c = Get_Keystroke();

         if ((c >= 1 && c <=31) || (c >= 127 && c <= 328)) {
            switch (c) {
            case SMG$K_TRM_PF1:
            case SMG$K_TRM_CTRLP:
              /*  PF1key(); */
                Open_Menu(1,MenuSpec);
                break;
           /* case SMG$K_TRM_CTRLP: 
               PF1key(); 
               break; */
            case SMG$K_TRM_CTRLR: 
            case SMG$K_TRM_PF2:
               if(CurrentPtr == NULL) break;
               PF2key();
               break;
            case SMG$K_TRM_CTRLE: 
            case SMG$K_TRM_PF3:
               if(CurrentPtr == NULL) break;
               PF3key();
               break;
            case SMG$K_TRM_CTRLD: 
            case SMG$K_TRM_PF4:
               if(CurrentPtr == NULL) break;
               PF4key();
               break;
            case SMG$K_TRM_CTRLW: 
               reread_index();
               break;            
            case SMG$K_TRM_CTRLU:
               toggle_unread();
               break;

            case SMG$K_TRM_DOWN:
               if(CurrentPtr == NULL) break;
               if (CurrentPtr->Next == NULL)    /* ZEN 23/10/2023  */
                  if (!ReadNextBlock(5,&CurrentPtr->rfa,FALSE)) break;
               LoLite(Standard_Window,CurrentPtr->Prompt,CurrentLine+TopLine);
               Increment(&CurrentLine);
               CurrentPtr = CurrentPtr->Next;
               break;
            case SMG$K_TRM_UP:
               if(CurrentPtr == NULL) break;
               if (CurrentPtr->Prev == NULL)    /* ZEN 23/10/2023  */
                  if (!ReadNextBlock(5,&CurrentPtr->rfa,TRUE)) break;
               LoLite(Standard_Window,CurrentPtr->Prompt,CurrentLine+TopLine);
               Decrement(&CurrentLine);
               CurrentPtr = CurrentPtr->Prev;
               break;
            case SMG$K_TRM_NEXT_SCREEN :
               if(CurrentPtr == NULL) break;
               for (i=0; i <=  BottomLine-TopLine; i++) {
                  if (CurrentPtr->Next == NULL)
                     if(!ReadNextBlock(RegionLength,&LastPtr->rfa,FALSE)) break;
                  CurrentPtr = CurrentPtr->Next;
               }
               Redraw_Screen();
               break;
            case SMG$K_TRM_PREV_SCREEN:
               if(CurrentPtr == NULL) break;
               for (i= 0;i <= (BottomLine-TopLine)+CurrentLine; i++) {
                  if(CurrentPtr->Prev == NULL)
                     if(!ReadNextBlock(RegionLength,&FirstPtr->rfa,TRUE)) break;
                  CurrentPtr = CurrentPtr->Prev;
               }
             for (i=0;i< CurrentLine;i++) if(CurrentPtr->Next != NULL) CurrentPtr = CurrentPtr->Next;
             /*    
               if (Count_Back(CurrentPtr) < BottomLine-TopLine)
                  CurrentLine = 0;
             */
         
               
               Redraw_Screen();
               break;
/*
--               when Key_Home =>
--                  CurrentCurs := Directory_Buffer.First;
--                  CurrentLine := 0;
--                  Redraw_Screen;
--               when Key_End =>
---                  CurrentCurs := Directory_Buffer.Last;
--                  CurrentLine := BottomLine-TopLine;
--                  Redraw_Screen;
--            when Key_Resize =>
--               Get_Size(Standard_Window,Number_Of_Lines => TermLnth,Number_Of_Columns => TermWdth);
--               BottomLine := TermLnth - 4;
--               Clear();
--               Redraw_Screen();
*/
            case SMG$K_TRM_CTRLZ: exit = TRUE;
                 break;
            case SMG$K_TRM_LF:
            case SMG$K_TRM_CR:
               Status("");
               if (CurrentPtr != NULL) {
                  get_findrec(&CurrentPtr,&FindRec);
                  Read_Post(&CurrentPtr->rfa);

                  align_list(&CurrentPtr,&FindRec);
         	  Redraw_Screen();
               }
               break;

            default : break;
            }
         } else if (c >= 32 && c <= 126) {
           /* Process regular alpha keys here */
         }

      }


   }





main(argc,argv)
int argc;
char **argv;
{
   char ffn[128];


   if (argc < 1 || argc > 2)
      printf("RMSEXP - incorrect number of arguments");
   else {

#ifdef ENABLE_CREATE
      filename = (argc == 2 ? *++argv : FORUM_FILE_NAME); 
      sprintf(ffn,"%s%s",FORUM_FILE_PATH,filename);
#else
      sprintf(ffn,"%s%s",FORUM_FILE_PATH,FORUM_FILE_NAME);
#endif     

      forum_initialize(ffn);

      initread("sys$login:.forumrc");

      open_file() ;
      open_readfile();

      termsize(&TermWidth,&TermLength);

      TopLine = 2;
      RegionLength = TermLength-3;
      BottomLine  = TermLength-2;
      StatusLine = TermLength -1;
      helpLine = TermLength;


      init_terminal(TermWidth,TermLength);

      init_windows();

      get_username();
 
      HeadLine("%VMSforumII V%s - Posting as %s",left,VERSION,user_name);


      Read_Messages();

  
      close_terminal();


      rms_status = sys$close(&fab);

      if (rms_status != RMS$_NORMAL) 
         error_exit("$CLOSE"); 

      rms_status = sys$close(&readfab);

      if (rms_status != RMS$_NORMAL) 
         error_exit("$CLOSE"); 


   } 
}       



