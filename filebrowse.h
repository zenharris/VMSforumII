
#define EDIT_MODE TRUE
#define REPLY_MODE FALSE

#define error_exit(format,filedef) error_ex(format,filedef,__FILE__,__LINE__)


struct rfabuff {
   uint16 b1,b2,b3;
/*   unsigned short b1,b2,b3;*/
};


#define LISTLINELNTH    128
struct List_Element {
   char Prompt[LISTLINELNTH];
   uint16 Index;
   struct rfabuff rfa;
   struct List_Element *Prev;
   struct List_Element *Next;
};


struct filestruct {
   char *filename;
   struct FAB fab;
   struct RAB rab;
   void (*initialize)();
   char *fileio;
   char *recbuff;
   uint16 recsize;
   uint16 fileiosz;
   void (*setprompt)();
   struct scr_params *entry_screen;
   uint16 entry_length;
   uint16 entry_comment;
   struct cmndline_params *read_cmndline;
   struct cmndline_params *list_cmndline;
   char *listkey;
   uint16 listkeysz;
   uint16 fwrdkeyid;
   uint16 revrkeyid;
   void (*setlistkey)(void);
   void (*padrecord)(void);
   void (*unpadrecord)(void);
   void (*loadvarbuff)();
   int  (*dumpvarbuff)();
   struct List_Element *FirstPtr;
   struct List_Element *LastPtr;
   struct List_Element *CurrentPtr;
   int   CurrentLine;
   int   ListCount;
   int ShowRead;
   void *startkey;
   uint16 startkeyln;
   int (*where)();
   void (*preedit)();
   int (*preinsert)();
   void (*postedit)();
   void (*postinsert)();
   unsigned long TopLine;
   unsigned long BottomLine;
   unsigned long RegionLength;
   void (*mainsrch)();
   int (*recalc)();
};

#ifdef FILEBROWSE_C_
struct filestruct filedef$init = {
   NULL,
   {0},
   {0},
   NULL,
   NULL,
   NULL,
   0,
   0,
   NULL,
   NULL,
   0,
   0,
   NULL,
   NULL,
   NULL,
   0,
   0,
   0,
   NULL,
   NULL,
   NULL,
   NULL,
   NULL,
   NULL,
   NULL,
   NULL,
   0,
   0,
   0,
   NULL,
   0,
   NULL,
   NULL,
   NULL,
   NULL,
   NULL,
   0,
   0,
   0,
   NULL,
   NULL
};
#else
extern struct filestruct filedef$init;
#endif


/* I think this is in extools.h
struct timebuff 
   {
      uint32 b1,b2;
   };
*/

enum filebrowse_mode {normal,select};

extern int ShowRead;
extern int rms_status;

void ReadMiddleOut(struct rfabuff *,struct filestruct *);
void Redraw_Screen(struct filestruct *);
void Seek_and_Edit(struct rfabuff *,int,struct filestruct *);
void RefreshList(struct filestruct *);
void Delete_Record(struct rfabuff *,struct filestruct *);
void DeleteList(struct filestruct *); 
void Read_Directory(int,struct filestruct *);

void rfa_copy(/* struct rfabuff *, struct rfabuff * */);
int rfa_iszero(struct rfabuff *);

int tbuff_iszero(struct timebuff *);
int tbuff_equal(struct timebuff *,struct timebuff *);
void tbuff_copy(struct timebuff *,struct timebuff *);
void get_time(struct timebuff *);
char *print_time(struct timebuff *);
char *print_date(struct timebuff *);

int ReadSearchTerm (char *);
void Insrt_Record();
void Edit_Record();
void Remove_Record();
void Search_Record();
void toggle_unread();
void reread_index();
/* void File_Browse(struct filestruct *,void *,void *); */
struct rfabuff *File_Browse();


void Status (const char *, ...);
void Help (const char *, ...);
void get_username(void);
void error_ex(const char *,struct filestruct *,char *,uint16);
void open_file(struct filestruct *);
char warn_user(const char *,...);
void mkuid(char *,struct timebuff);
char *filterfn(char *);
int rmstest(int,int,...);
void pad(char *,int);
void unpad(char *,int);
char *rmslookup(unsigned int);
void read_master_record();
void write_master_record();
void spawn_scroll_window(struct filestruct *,int,int);
struct timebuff *dttodo (struct timebuff *);

