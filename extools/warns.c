#include stdio
#include stdlib
#include stdarg
#include smgdef
#include smg$routines
#include ints
#include string
#include rms

#include "screen.h"
#include "extools.h"
#include "filebrowse.h"
#include "warns.h"

#define extscrprn(buff,row,col,attr) exscrprn(buff,row-1,col-1,attr)

int   TopLine;
int   BottomLine;
int   StatusLine;
int   RegionLength;
int   helpLine;


void Error(const char* format, ...)
{
    char buffer[1024];
    va_list argptr;
    va_start(argptr, format);
    vsprintf(buffer, format, argptr);
    va_end(argptr);
}

char *filterfn(char *fullfn)
{
   char *fn;
   char delim[] = "];";
   char filebuff[256];

   strcpy (filebuff,fullfn);
   fn = strtok(filebuff,delim);
   if (fn != NULL)
      fn = strtok(NULL,delim);
   if(fn == NULL)return(fullfn);
   else return(fn);

}

void error_ex(const char *format,struct filestruct *filedef,char *file,uint16 line)
{


   printf("\nFile operation failed (%s) rmsret: %u \n",
             filedef->filename,rms_status);
   printf("%s\n",format);
   printf("file %s line %u\n",file,line);

   exit(rms_status);
}

char warn_user(const char *format, ...)
{
   unsigned int warnwin;
   unsigned long row,col;
   char buffer[1024],retch;
   uint16 colpos,cols;
   va_list argptr;

   va_start(argptr, format);
   vsprintf(buffer,format,argptr);
   va_end(argptr);

   row = smg$cursor_row(&crnt_window->display_id);
   col = smg$cursor_column(&crnt_window->display_id);


   colpos = (strlen(buffer)+5)/2;
   if (colpos > (TermWidth/2)-3) colpos = (TermWidth/2)-3;
   cols = strlen(buffer)+5;
   if (cols > TermWidth-3) cols = TermWidth-3;

   warnwin = open_win(PasteBoard,             /*Paste Board ID */
                             3,               /* rows  */
                             cols,            /* cols  */
                             (TermLength/2),  /* top left row */
                             (TermWidth/2) - colpos); /* top left col*/



   Add (warnwin,2,3,buffer);

   retch = Get_Keystroke();

   close_win(warnwin);

   smg$set_cursor_abs(&crnt_window->display_id,&row,&col);
   Refresh(crnt_window->display_id);
   return (retch);
}



void HeadLine (const char *format,enum headline_mode mode,...)
{
   char buffer[1024];
   long Width,Length;
   int col;
   va_list argptr;

   va_start(argptr, mode);
   vsprintf(buffer,format,argptr);
   va_end(argptr);

   smg$get_display_attr(&crnt_window->display_id,&Length,&Width);
   if (mode == centre)
      col = (Width/2) - (strlen(buffer)/2);
   else
      col = 2;
   clreolpos(crnt_window->display_id,1,1);
   /*Add_Attr(Standard_Window,1,1, buffer,SMG$M_UNDERLINE );*/
   extscrprn(buffer,1,col,SMG$M_UNDERLINE);
   Refresh(crnt_window->display_id);
}


void Status (const char *format, ...)
{
   unsigned long row,col;
   char buffer[1024];
   va_list argptr;

   va_start(argptr, format);
   vsprintf(buffer,format,argptr);
   va_end(argptr);

   row = smg$cursor_row(&crnt_window->display_id);
   col = smg$cursor_column(&crnt_window->display_id);

   clreolpos(Standard_Window,StatusLine,1);
   Add(Standard_Window,StatusLine,1, buffer );
   Refresh(Standard_Window);

   smg$set_cursor_abs(&crnt_window->display_id,&row,&col);
   Refresh(crnt_window->display_id);

   
}

void Help (const char *format, ...)
{
   unsigned long row,col;
   char buffer[1024];
   va_list argptr;

   va_start(argptr, format);
   vsprintf(buffer,format,argptr);
   va_end(argptr);

   row = smg$cursor_row(&crnt_window->display_id);
   col = smg$cursor_column(&crnt_window->display_id);

   clreolpos(Standard_Window,helpLine,1);
   Add(Standard_Window,helpLine,1, buffer);
   Refresh(Standard_Window);

   smg$set_cursor_abs(&crnt_window->display_id,&row,&col);
   Refresh(crnt_window->display_id);


}


