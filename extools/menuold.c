#include <stdio.h>
#include <dos.h>

#include <extools.h>


static int get_key (scan)
int *scan;
{
   union REGS wreg;
   wreg.h.ah = 00;
   int86(0x16,&wreg,&wreg);
   *scan = (int)wreg.h.ah;
   return((int)wreg.h.al);
}


int menu(tbl,tbl_lnth,rul,cul,rlr,clr,norm,hilite,no_mac)
char *tbl[];
int tbl_lnth;
int rul,cul,rlr,clr;
int norm,hilite,no_mac;
{
    int scan,c;
    int crnt_line = rul,iter;
/*    exscrldn((rul << 8) + cul,(rlr << 8) + clr,0,norm);*/
    for (iter = 0;iter < tbl_lnth && (iter + crnt_line) <= rlr;iter++){
        exscrprn(tbl[iter],iter + crnt_line,cul,norm);
    }
    iter = 0;
    exscrprn(tbl[iter],crnt_line,cul,hilite);
    for (;;) {
        if (no_mac) c = get_key(&scan);
        else c = exinkey(&scan);
        switch(c) {
            case 00 : switch(scan) {
                case DARR : if (iter < tbl_lnth - 1) {
                               exscrprn(tbl[iter++],crnt_line++,cul,norm);
                               if (crnt_line > rlr) {
                                  exscrlup((rul << 8) + cul,(rlr << 8) + clr,1,norm);
                                  crnt_line = rlr;
                               }
                               exscrprn(tbl[iter],crnt_line,cul,hilite);
                            }
                            break;
                case UARR : if (iter > 0) {
                               exscrprn(tbl[iter--],crnt_line--,cul,norm);
                               if (crnt_line < rul) {
                                  exscrldn((rul << 8) + cul,(rlr << 8) + clr,1,norm);
                                  crnt_line = rul;
                               }
                               exscrprn(tbl[iter],crnt_line,cul,hilite);
                            }
                            break;
            }
            break;
            case CR : return(iter + 1);
            case ESC : return (ESC);
        }
    }
}

