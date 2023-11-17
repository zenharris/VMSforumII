#include stdio
#include string



exchrstr (deststr,addchar)
char *deststr;
char addchar;
{
   char addstr[2];
   addstr[0] = addchar;
   addstr[1] = 0;
   strcat(deststr,addstr);

}
