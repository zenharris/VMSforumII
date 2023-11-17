void Error(const char *, ...);

char *filterfn(char *);
/*
void error_ex(const char *,struct filestruct *,char *,uint16);
*/

enum headline_mode {centre,left,right};
char warn_user(const char *, ...);
void HeadLine (const char *,enum headline_mode, ...);
void Status (const char *, ...);
void Help (const char *, ...);

