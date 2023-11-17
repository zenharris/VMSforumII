
#ifdef SCREEN_C
int   TermLength;
int   TermWidth;
unsigned int Standard_Window;
unsigned int   PasteBoard;
unsigned int   KeyBoard;
#else
extern int   TermLength;
extern int   TermWidth;
extern unsigned int Standard_Window;
extern unsigned int PasteBoard;
extern unsigned int KeyBoard;
#endif




void init_terminal (int,int);

void close_terminal();

int Move_Cursor (unsigned int,unsigned int,unsigned int);

int set_cursor (unsigned int,unsigned int,unsigned int);

int Add (unsigned int,unsigned int,unsigned int,char *);

int Add_Attr (unsigned int,unsigned int,unsigned int,char *,unsigned int);

int put_chars (unsigned int,char *);
 
int put_line (unsigned int,char *);
 

void clreol(unsigned int);

void clreolpos(unsigned int,unsigned int,unsigned int);


void Refresh(unsigned int);

uint16 Get_Keystroke(void);

unsigned int open_win(unsigned int,unsigned int,unsigned int,
                   unsigned int,unsigned int);
void close_win (unsigned int);

void Scroll_Up(unsigned int,unsigned int);

void Scroll_Down(unsigned int,unsigned int);

int Clear_Region(unsigned int,unsigned int);



