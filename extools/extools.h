#include <setjmp.h>
/*#include <dos.h>*/

#define savscrn(a,b,c,d,e) gettext((b) + 1,(c) + 1,(d) + 1,(e) + 1,(a))
#define rstscrn(a,b,c,d,e) puttext((b) + 1,(c) + 1,(d) + 1,(e) + 1,(a))
/* #define exscrprn(str,row,col,attr) gotoxy(col+1,row+1);textattr(attr);cputs((row==24 && strlen(str)>79)?str[79]=0,str:str)*/

/*#define exclreol(row,col,attr) gotoxy(col+1,row+1);textattr(attr);clreol()*/
/*#define cls(attr) textattr(attr);clrscr()*/
/*#define exloccur(row,col) gotoxy(col+1,row+1)*/

struct line_rec {
   struct line_rec *next;
   struct line_rec *last;
   char storage[101]; /*81]; */
};

/*    ++++++++  struct line_rec *balloc();   +++++++*/
void texaco();

/*****************************************/
/*      Window  Definitions              */
/*****************************************/

enum win_type {borderless,border};
enum en_mode_type {breakout,nobreakout,special};

struct crnt_wind_rec {
    unsigned int display_id;
    int cul,rul,clr,rlr,attr;
    void *svscr;
    int width,lnth;
    struct crnt_wind_rec *last;
    enum win_type type;
};

extern struct crnt_wind_rec *crnt_window;
extern struct crnt_wind_rec *root_window;

#define set_window(svscr,cul,rul,clr,rlr,attr,width,lnth); \
                              {crnt_window->svscr = svscr;\
                               crnt_window->cul = cul;\
                               crnt_window->rul=rul;\
                               crnt_window->clr=clr;\
                               crnt_window->rlr=rlr;\
                               crnt_window->attr=attr;\
                               crnt_window->width=width;\
                               crnt_window->lnth=lnth;}

/*#define exscrprn(str,row,col,attr) exscrprn(str,(row)+crnt_window->rul,(col)+crnt_window->cul,attr)
#define exloccur(row,col) exloccur((row)+crnt_window->rul,(col)+crnt_window->cul)
*/
#define exdelln(line) exscrlup(((line) << 8) + 0,((crnt_window->lnth-1) << 8)+crnt_window->width-1,1,crnt_window->attr);
#define exscrlupw() exscrlup(0,((crnt_window->lnth-1) << 8) + crnt_window->width-1,1,crnt_window->attr);
#define exscrldnw() exscrldn(0,((crnt_window->lnth-1) << 8) + crnt_window->width-1,1,crnt_window->attr);

struct list_element {
   char disp_line[81];
   char tag;
   long rec_num;
   struct list_element *next;
   struct list_element *last;
};


struct indexr_params {
    struct list_element *frst_list;
    struct list_element *last_list;
    char *main_file;
    void *main_buff;
    int *main_lnth;
    void *key_buff;
    int path_no;
    int col,row,width,lnth;
    enum win_type type;
};


struct menu_params {
    char *prompt;
    void (*fn_ptr)();
};

struct cmndline_params {
   int fkey;
   char *prompt;
   struct menu_params *menu;
   int menu_lnth;
   void (*fn_ptr)();
};

struct scrllst_msg_rec {
	struct cmndline_params *cmndline;
	 struct list_element **crnt_list;
   int *pos_chngd;
   int *crnt_line;
   struct indexr_params *scrl_consts;
   int breaker;
};

struct linedt_msg_rec {
	struct cmndline_params *cmndline;
	char *edtstr;
	int col,row,lnth,attr,*pos;
	unsigned int *control;
};


struct scr_params {
    int row,col;
    char *prompt;
    char prattr;
    unsigned long format;
    void *field;
    int fattr;
    int lnth;
    int (*validate)();
    struct cmndline_params *cmndline;
};

struct x_date_time {
   unsigned char day,month,year,cent,hour,min,sec;
};


struct date_time {
    unsigned short int month,day,year,hour,min,sec,hun;
};

struct date_only {
    unsigned short int month,day,year;
};


struct timebuff
   {
      uint32 b1,b2;
   };



struct btr_datetime {
       unsigned char day;
       unsigned char month;
       unsigned int year;
       unsigned char hndrdths;
       unsigned char sec;
       unsigned char min;
       unsigned char hour;
};


struct btr_date {
       unsigned char day;
       unsigned char month;
       unsigned int year;
};


struct time_total {
    unsigned int hours;
    unsigned char mins;
};


/* misc directory structures */

struct tdirtime {
  unsigned secs      : 5; /* time in 2 sec. int (5 bits)*/
  unsigned mins      : 6; /* minutes (6 bits)		*/
  unsigned hours     : 5; /* hours (5 bits)		*/
};

struct tdirdate {
  unsigned day	     : 5; /* day of month (5 bits)	*/
  unsigned month     : 4; /* month (4 bits)		*/
  unsigned year      : 7; /* year since 1980 (7 bits)	*/
};

struct tdir
{
  char	   dirfiller[21];
  unsigned char attr;		       /* file attribute */
  struct tdirtime time; 	       /* creation time  */
  struct tdirdate date; 	       /* creation date  */
  long	   size;		       /* file size	 */
  char	   name[13];	       /* file name	 */
};
#define TDIR struct tdir


struct list_element *lalloc();
int delete_list();
int display_list();


/* given a segment and offset, return a pointer which will point to the */
/* seg:off pair 							*/

#define LPOINTER(type,seg,ofs) (type far *) ((long) seg << 16 | ofs)
#define CNULL 0
#define SEG(x) *((unsigned*)&(x)+1)
#define OFS(x) *((unsigned*)&(x))
#define msdos(_wreg) intr(0x21,_wreg)	/* define turbo like msdos call */
#define lobyte(a) *((unsigned char *)&(a))
#define hibyte(a) *((unsigned char *)&(a)+1)
#define loword(a) *((unsigned *)&(a))
#define hiword(a) *((unsigned *)&(a)+1)



#define TRUE 1
#define FALSE 0
#define EVER (;;)
#define ETERNITY (;;)


#define file_error(x) exfilerr(x,__FILE__,__LINE__)
#define gen_error(x)  exgenerr(__FILE__,__LINE__,"",x,NULL)
#define btrv_ok(status) (status == 22 || status == 0)
#define btrv_locked(status) (status == 84 || status == 85)


/* 컴컴컴컴컴컴컴컴 define various memory peek macros. 컴컴컴컴컴컴컴컴 */

/* this one returns a char */
#define MEMB(seg,ofs) (*LPOINTER(unsigned char,seg,ofs))

/* this one returns a word */
#define MEMW(seg,ofs) (*LPOINTER(unsigned int,seg,ofs))

#define MEML(seg,ofs) (*LPOINTER(long,seg,ofs))
/* this one returns a long */


/* misc function key and keypad scan definitions */

#define CLEARKBD while (exkeyprs()) { int w; exinkey(&w); }
#define EJECT   12
#define PGUP	73
#define PGDN	81
#define HOME	71
#define END	79
#define UARR	72
#define DARR	80
#define LARR	75
#define RARR	77
#define DELETE	83
#define INSERT	82
#define BACKSPACE 8
#define PLUS	78
#define F1	59
#define F2	60
#define F3	61
#define F4	62
#define F5	63
#define F6	64
#define F7	65
#define F8	66
#define F9	67
#define F10	68
#define CTRL_F1  94
#define CTRL_F2  95
#define CTRL_F3  96
#define CTRL_F4  97
#define CTRL_F5  98
#define CTRL_F6  99
#define CTRL_F7  100
#define CTRL_F8  101
#define CTRL_F9  102
#define CTRL_F10 103
#define ALT_F1  104
#define ALT_F2  105
#define ALT_F3  106
#define ALT_F4  107
#define ALT_F5  108
#define ALT_F6  109
#define ALT_F7  110
#define ALT_F8  111
#define ALT_F9  112
#define ALT_F10 113
#define SHFT_F1  84
#define SHFT_F2  85
#define SHFT_F3  86
#define SHFT_F4  87
#define SHFT_F5  88
#define SHFT_F6  89
#define SHFT_F7  90
#define SHFT_F8  91
#define SHFT_F9  92
#define SHFT_F10 93
#define ENTER   28
#define CTRL_A	    1
#define CTRL_F	    6
#define CTRL_E	    5
#define CTRL_X	    24
#define CTRL_S	    19
#define CTRL_D	    4
#define CTRL_G	    7
#define CTRL_V	    22
#define CTRL_C	    3
#define CTRL_R	    18
#define CTRL_M	    13
#define CTRL_PGUP   132
#define CTRL_PGDN   118
#define CTRL_HOME   119
#define CTRL_END    117
#define ALT_S	    31
#define ALT_V	    47
#define ALT_O       24
#define ALT_D       32
#define ALT_X       45
#define TAB     9
#define BACKTAB     15
#define ESC	    27
#define ESCSCAN 1
#define CR 13

#define ESCEOL 0xff
#define ESCBOL 0xfe

#define BLACK 0
#define BLUE  1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define BROWN 6
#define LIGHTGRAY 0 /* 7  */
#define DARKGRAY 8
#define LIGHTBLUE 9
#define LIGHTGREEN 0x0A
#define LIGHTCYAN 0x0B
#define LIGHTRED 0x0C
#define LIGHTMAGENTA 0x0D
#define YELLOW 0x0E
#define WHITE SMG$M_BOLD /* 0x0F */
#define BLINK 0x80

#define B_OPEN 0
#define B_CLOSE 1
#define B_DEL 4
#define B_INS 2
#define B_UPD 3
#define B_GETFIRST 12
#define B_GETEQ 5
#define B_GETNX 6
#define B_GETPR 7
#define B_GETGT 8
#define B_GETGE 9
#define B_GETLT 10
#define B_GETLE 11
#define B_GETLAST 13
#define B_CREATE 14
#define B_CD 17
#define B_BEGIN 19
#define B_END 20
#define B_ABORT 21
#define B_GETPOS 22
#define B_GETDRC 23
#define B_STOP 25
#define B_UNLOCK 27
#define B_SETOWN 29

#define DUP 1
#define MOD 2
#define BIN 4
#define HASNULL 8
#define SEGMNTD 16
#define DESC 64
#define EXT_TYPE 256
#define B_ZSTR_TYPE 11
#define B_INT_TYPE 1
#define B_DATE_TYPE 3
#define B_TIME_TYPE 4


#define SUPRESS_DELETE 1
#define INSERT_MODE 2
#define WORD_WRAP 4
#define NON_EDIT 8
#define CAPITALIZE 8192

#define UNFMT_STRING 16
#define DATE_ONLY 32
#define DATE_TIME 64
#define SINGLE_CHAR 128
#define NUMERIC 256
#define FREE_TEXT 512
#define TYPE_OVER 1024
#define BTRV_DATE_TIME 2048
#define BTRV_DATE 4096
#define INIT_ONLY 16384

#define ALLOCATE 4096
#define COMP_LOAD  2048
#define COMP_CHECK 64


#define X_DELETE 0xff
#define X_INSERT 0xfe

#define KEY_ONLY 0x01
#define READ_ONLY 0x02
#define NORMAL 0x00

extern int right_margin;
extern int read_only;
extern int insert_mode;
extern int recd_macro;
extern int exec_macro;
extern int macro_changed_flag;
extern jmp_buf crit_error_context;
extern int ctrlc_flag;
extern long last_insert_recno;
extern void (*after_insert)(void);
extern void (*after_init_text)(void);
extern int block_transaction;


extern int border_colour;
extern int text_colour;
extern int text_hilite;
extern int cmndline_colour;
extern int cmndline_inverse;
		 

void init_windows(void);
void close_window(void);

unsigned getcrc();
char *exlftstr();
char *exrgtstr();
char *exgetpce();
char *excntstr();
char *exgetvol();
char *exalloc();

char *exchrstr();
int exinsstr(char *,char *,int);
int exgenerr(char *,int,char *,... );
int exkeyprs();
unsigned __int16 exinkey(unsigned __int16 *);
int exscrprn (char *,int, int, int);
int exloccur(int,int);
int exsetcur(int,int);

int command_line (/*struct cmndline_params[],uint16 *,void **/);

int exscrlup(int,int,int,int);
int exscrldn(int,int,int,int);
int exclreol(int,int,int);
int exdltstr(char *,int,int);

/* type int id_type;  */
           
unsigned int Standard_Window;
unsigned int   PasteBoard;
unsigned int   KeyBoard;

#define max(a,b)        (((a) > (b)) ? (a) : (b))
#define min(a,b)        (((a) < (b)) ? (a) : (b))




struct line_rec *balloc();

int verify();
int verifyx();
long gtoj(struct date_only *);
long btrv_gtoj(struct btr_date *);
int jtog();
int btrv_jtog(long,struct btr_date *);

char *weekday();
char *mnthyear();
char *btrv_mnthyear();
char check_date(struct date_only *);
int  check_btrv_date(struct btr_date *);
struct date_only *todays_date(struct date_only *);
struct btr_date *todays_btrv_date(struct btr_date *);

long today_comp(struct date_only *);
long today_btrv_comp(struct btr_date *);
void exclock(void);
/* byte normyl(byte);*/
struct date_only *xdtodo(struct date_only *,struct x_date_time *);
struct x_date_time *sys_date_time(struct x_date_time *);
struct x_date_time *dos_date_time(struct x_date_time *);
char *asc_x_date_time(char *,struct x_date_time *);
char *asc_btr_date(char *,struct btr_date *);
char *asc_date(char *,struct date_only *);
char *asc_datetime(char *,struct date_time *);
char *asc_btr_datetime(char *,struct btr_datetime *);

struct date_time *bdttodt (struct date_time *,struct btr_datetime *);
struct btr_datetime *dttobdt (struct btr_datetime *,struct date_time *);
struct date_only *bdtodo (struct date_only *,struct btr_date *);
struct btr_date *dotobd (struct btr_date *,struct date_only *);

struct date_only *systodo (struct date_only *,struct timebuff *);
struct date_time *systodt (struct date_time *,struct timebuff *);
struct timebuff *dotosys (struct timebuff *,struct date_only *);
struct timebuff *dttosys (struct timebuff *,struct date_time *);




int critical_error_handler(int ,int ,int ,int );
void ctrlc_handler(int );

void *open_window(int,int,int,int,int,enum win_type);

int linedt(char *,int,int,int,int,int *,unsigned int *,struct cmndline_params *);
int enter_screen(struct scr_params[],int,int *,enum en_mode_type,void *,int *);
int init_screen();


int datedt(struct date_only *,int,int,int,int *,unsigned int *,struct cmndline_params[]);
int datimedt(struct date_time *,int,int,int,int *,unsigned int *,struct cmndline_params[]);

/*struct date_only *dat;
int col,row,*edtpos;
unsigned int *control;
struct cmndline_params cmndline[];
*/




unsigned int select_rec();

extern  int     termsize (int *width_, int *length_);
extern  int exwrtchr();

void Help(const char *, ...);
void Status (const char *, ...);
void Refresh();
uint16 Get_Keystroke(void);
int init_text (struct line_rec *,int,int,int (*)());

#ifdef INPUTSCR_C_
char *TEXACOHLP = "Texaco :    ^Z exit  ^L insert line  ^D delete line";
#else
extern char *TEXACOHLP; 
#endif

