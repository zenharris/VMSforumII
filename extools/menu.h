
#define PLENGTH 80
#define MLENGTH 80

struct Menu_Record {
     char Prompt[PLENGTH];
     int (*Func)();
};


void Open_Menu (unsigned int,struct Menu_Record[]);

