!++
!     Define symbols according to macro values
!==

.IFDEF DEBUG
DBG = /DEBUG
DBGOPT = /NOOPTIMIZE/DEBUG
.ELSE
DBG = /NODEBUG
DBGOPT = /OPTIMIZE/NODEBUG
.ENDIF

.IFDEF LIST
LST = /LIST
.ELSE
LST = /NOLIST
.ENDIF

.IFDEF DIAG
DIA = /DIAGNOSTICS
.ELSE
DIA = /NODIAGNOSTICS
.ENDIF

.IFDEF PCA
PCAOPT = /DEBUG
.ENDIF

!++
!      List of tools used and required symbols
!
!      !C used
!      !Executables used
!==


!++
!     Missing sources catch-all
!==

.DEFAULT
   ! No source found for $(MMS$TARGET)
   ! - Attempting to continue



!++
!   Complete application - default build item
!==

COMPLETE_APPLICATION depends_on -
     TEST.EXE
        CONTINUE

CFLAGS = /INCLUDE_DIRECTORY=[-]

!++
!   C
!==

EXTOOLS.OLB(CMNDLINE=CMNDLINE.OBJ) depends_on -
      CMNDLINE.C -
     ,EXTOOLS.H -
     ,[-]FILEBROWSE.H -
     !
       $(CC) $(CFLAGS)  $(LST) $(DBGOPT) $(DIA) /OBJ=CMNDLINE.OBJ CMNDLINE.C
       LIBRARY/REPLACE EXTOOLS.OLB CMNDLINE.OBJ
       DELETE CMNDLINE.OBJ;*

EXTOOLS.OLB(DATEDT=DATEDT.OBJ) depends_on -
      DATEDT.C -
     ,EXTOOLS.H -
     !
       $(CC) $(CFLAGS)  $(LST) $(DBGOPT) $(DIA) /OBJ=DATEDT.OBJ DATEDT.C
       LIBRARY/REPLACE EXTOOLS.OLB DATEDT.OBJ
       DELETE DATEDT.OBJ;*

EXTOOLS.OLB(EXCHRSTR=EXCHRSTR.OBJ) depends_on -
      EXCHRSTR.C -
     !
       $(CC) $(CFLAGS)  $(LST) $(DBGOPT) $(DIA) /OBJ=EXCHRSTR.OBJ EXCHRSTR.C
       LIBRARY/REPLACE EXTOOLS.OLB EXCHRSTR.OBJ
       DELETE EXCHRSTR.OBJ;*

EXTOOLS.OLB(EXCLREOL=EXCLREOL.OBJ) depends_on -
      EXCLREOL.C -
     ,EXTOOLS.H -
     !
       $(CC) $(CFLAGS)  $(LST) $(DBGOPT) $(DIA) /OBJ=EXCLREOL.OBJ EXCLREOL.C
       LIBRARY/REPLACE EXTOOLS.OLB EXCLREOL.OBJ
       DELETE EXCLREOL.OBJ;*

EXTOOLS.OLB(EXCNTSTR=EXCNTSTR.OBJ) depends_on -
      EXCNTSTR.C -
     !
       $(CC) $(CFLAGS)  $(LST) $(DBGOPT) $(DIA) /OBJ=EXCNTSTR.OBJ EXCNTSTR.C
       LIBRARY/REPLACE EXTOOLS.OLB EXCNTSTR.OBJ
       DELETE EXCNTSTR.OBJ;*

EXTOOLS.OLB(EXDLTSTR=EXDLTSTR.OBJ) depends_on -
      EXDLTSTR.C -
     ,EXTOOLS.H -
     !
       $(CC) $(CFLAGS)  $(LST) $(DBGOPT) $(DIA) /OBJ=EXDLTSTR.OBJ EXDLTSTR.C
       LIBRARY/REPLACE EXTOOLS.OLB EXDLTSTR.OBJ
       DELETE EXDLTSTR.OBJ;*

EXTOOLS.OLB(EXGETPCE=EXGETPCE.OBJ) depends_on -
      EXGETPCE.C -
     !
       $(CC) $(CFLAGS)  $(LST) $(DBGOPT) $(DIA) /OBJ=EXGETPCE.OBJ EXGETPCE.C
       LIBRARY/REPLACE EXTOOLS.OLB EXGETPCE.OBJ
       DELETE EXGETPCE.OBJ;*

EXTOOLS.OLB(EXINSSTR=EXINSSTR.OBJ) depends_on -
      EXINSSTR.C -
     ,EXTOOLS.H -
     !
       $(CC) $(CFLAGS)  $(LST) $(DBGOPT) $(DIA) /OBJ=EXINSSTR.OBJ EXINSSTR.C
       LIBRARY/REPLACE EXTOOLS.OLB EXINSSTR.OBJ
       DELETE EXINSSTR.OBJ;*

EXTOOLS.OLB(EXLFTSTR=EXLFTSTR.OBJ) depends_on -
      EXLFTSTR.C -
     !
       $(CC) $(CFLAGS)  $(LST) $(DBGOPT) $(DIA) /OBJ=EXLFTSTR.OBJ EXLFTSTR.C
       LIBRARY/REPLACE EXTOOLS.OLB EXLFTSTR.OBJ
       DELETE EXLFTSTR.OBJ;*

EXTOOLS.OLB(EXLOCCUR=EXLOCCUR.OBJ) depends_on -
      EXLOCCUR.C -
     ,EXTOOLS.H -
     !
       $(CC) $(CFLAGS)  $(LST) $(DBGOPT) $(DIA) /OBJ=EXLOCCUR.OBJ EXLOCCUR.C
       LIBRARY/REPLACE EXTOOLS.OLB EXLOCCUR.OBJ
       DELETE EXLOCCUR.OBJ;*

EXTOOLS.OLB(EXPAD=EXPAD.OBJ) depends_on -
      EXPAD.C -
     !
       $(CC) $(CFLAGS)  $(LST) $(DBGOPT) $(DIA) /OBJ=EXPAD.OBJ EXPAD.C
       LIBRARY/REPLACE EXTOOLS.OLB EXPAD.OBJ
       DELETE EXPAD.OBJ;*

EXTOOLS.OLB(EXRGTSTR=EXRGTSTR.OBJ) depends_on -
      EXRGTSTR.C -
     ,EXTOOLS.H -
     !
       $(CC) $(CFLAGS)  $(LST) $(DBGOPT) $(DIA) /OBJ=EXRGTSTR.OBJ EXRGTSTR.C
       LIBRARY/REPLACE EXTOOLS.OLB EXRGTSTR.OBJ
       DELETE EXRGTSTR.OBJ;*

EXTOOLS.OLB(EXSCRLDN=EXSCRLDN.OBJ) depends_on -
      EXSCRLDN.C -
     ,EXTOOLS.H -
     !
       $(CC) $(CFLAGS)  $(LST) $(DBGOPT) $(DIA) /OBJ=EXSCRLDN.OBJ EXSCRLDN.C
       LIBRARY/REPLACE EXTOOLS.OLB EXSCRLDN.OBJ
       DELETE EXSCRLDN.OBJ;*

EXTOOLS.OLB(EXSCRLUP=EXSCRLUP.OBJ) depends_on -
      EXSCRLUP.C -
     ,EXTOOLS.H -
     !
       $(CC) $(CFLAGS)  $(LST) $(DBGOPT) $(DIA) /OBJ=EXSCRLUP.OBJ EXSCRLUP.C
       LIBRARY/REPLACE EXTOOLS.OLB EXSCRLUP.OBJ
       DELETE EXSCRLUP.OBJ;*

EXTOOLS.OLB(EXSCRPRN=EXSCRPRN.OBJ) depends_on -
     ,EXSCRPRN.C -
     ,EXTOOLS.H -
     !
       $(CC) $(CFLAGS)  $(LST) $(DBGOPT) $(DIA) /OBJ=EXSCRPRN.OBJ EXSCRPRN.C
       LIBRARY/REPLACE EXTOOLS.OLB EXSCRPRN.OBJ
       DELETE EXSCRPRN.OBJ;*

EXTOOLS.OLB(EXSETCUR=EXSETCUR.OBJ) depends_on -
      EXSETCUR.C -
     !
       $(CC) $(CFLAGS)  $(LST) $(DBGOPT) $(DIA) /OBJ=EXSETCUR.OBJ EXSETCUR.C
       LIBRARY/REPLACE EXTOOLS.OLB EXSETCUR.OBJ
       DELETE EXSETCUR.OBJ;*

EXTOOLS.OLB(EXWRTCHR=EXWRTCHR.OBJ) depends_on -
      EXWRTCHR.C -
     ,EXTOOLS.H -
     !
       $(CC) $(CFLAGS)  $(LST) $(DBGOPT) $(DIA) /OBJ=EXWRTCHR.OBJ EXWRTCHR.C
       LIBRARY/REPLACE EXTOOLS.OLB EXWRTCHR.OBJ
       DELETE EXWRTCHR.OBJ;*

EXTOOLS.OLB(GETCRC=GETCRC.OBJ) depends_on -
      GETCRC.C -
     !
       $(CC) $(CFLAGS)  $(LST) $(DBGOPT) $(DIA) /OBJ=GETCRC.OBJ GETCRC.C
       LIBRARY/REPLACE EXTOOLS.OLB GETCRC.OBJ
       DELETE GETCRC.OBJ;*

EXTOOLS.OLB(INKEY=INKEY.OBJ) depends_on -
      INKEY.C -
     ,EXTOOLS.H -
     !
       $(CC) $(CFLAGS)  $(LST) $(DBGOPT) $(DIA) /OBJ=INKEY.OBJ INKEY.C
       LIBRARY/REPLACE EXTOOLS.OLB INKEY.OBJ
       DELETE INKEY.OBJ;*

EXTOOLS.OLB(INPUTSCR=INPUTSCR.OBJ) depends_on -
      INPUTSCR.C -
     ,EXTOOLS.H -
     ,[-]FILEBROWSE.H -
     !
       $(CC) $(CFLAGS)  $(LST) $(DBGOPT) $(DIA) /OBJ=INPUTSCR.OBJ INPUTSCR.C
       LIBRARY/REPLACE EXTOOLS.OLB INPUTSCR.OBJ
       DELETE INPUTSCR.OBJ;*

EXTOOLS.OLB(MENU=MENU.OBJ) depends_on -
      MENU.C -
     ,MENU.H -
     ,SCREEN.H -
     !
       $(CC) $(CFLAGS)  $(LST) $(DBGOPT) $(DIA) /OBJ=MENU.OBJ MENU.C
       LIBRARY/REPLACE EXTOOLS.OLB MENU.OBJ
       DELETE MENU.OBJ;*

EXTOOLS.OLB(SCREEN=SCREEN.OBJ) depends_on -
      SCREEN.C -
     ,SCREEN.H -
     ,EXTOOLS.H -
     !
       $(CC) $(CFLAGS)  $(LST) $(DBGOPT) $(DIA) /OBJ=SCREEN.OBJ SCREEN.C
       LIBRARY/REPLACE EXTOOLS.OLB SCREEN.OBJ
       DELETE SCREEN.OBJ;*

EXTOOLS.OLB(TERMSIZE=TERMSIZE.OBJ) depends_on -
      TERMSIZE.C -
     ,CRT.H -
     !
       $(CC) $(CFLAGS)  $(LST) $(DBGOPT) $(DIA) /OBJ=TERMSIZE.OBJ TERMSIZE.C
       LIBRARY/REPLACE EXTOOLS.OLB TERMSIZE.OBJ
       DELETE TERMSIZE.OBJ;*

TEST.OBJ depends_on -
      TEST.C -
     ,EXTOOLS.H -
     !
       $(CC) $(CFLAGS)  $(LST) $(DBGOPT) $(DIA) /OBJ=TEST.OBJ TEST.C
!       LIBRARY/REPLACE EXTOOLS.OLB TEST.OBJ
!       DELETE TEST.OBJ;*

EXTOOLS.OLB(TEXACO=TEXACO.OBJ) depends_on -
      TEXACO.C -
     ,EXTOOLS.H -
     ,[-]FILEBROWSE.H -
     !
       $(CC) $(CFLAGS)  $(LST) $(DBGOPT) $(DIA) /OBJ=TEXACO.OBJ TEXACO.C
       LIBRARY/REPLACE EXTOOLS.OLB TEXACO.OBJ
       DELETE TEXACO.OBJ;*

EXTOOLS.OLB(WARNS=WARNS.OBJ) depends_on -
      WARNS.C -
     ,EXTOOLS.H -
     ,[-]FILEBROWSE.H -
     !
       $(CC) $(CFLAGS)  $(LST) $(DBGOPT) $(DIA) /OBJ=WARNS.OBJ WARNS.C
       LIBRARY/REPLACE EXTOOLS.OLB WARNS.OBJ
       DELETE WARNS.OBJ;*

EXTOOLS.OLB(WINDOW=WINDOW.OBJ) depends_on -
      WINDOW.C -
     ,EXTOOLS.H -
     !
       $(CC) $(CFLAGS)  $(LST) $(DBGOPT) $(DIA) /OBJ=WINDOW.OBJ WINDOW.C
       LIBRARY/REPLACE EXTOOLS.OLB WINDOW.OBJ
       DELETE WINDOW.OBJ;*


!++
!   Links
!==

TEST.EXE depends_on -
      EXTOOLS.OLB(CMNDLINE=CMNDLINE.OBJ) -
     ,EXTOOLS.OLB(DATEDT=DATEDT.OBJ) -
     ,EXTOOLS.OLB(EXCHRSTR=EXCHRSTR.OBJ) -
     ,EXTOOLS.OLB(EXCLREOL=EXCLREOL.OBJ) -
     ,EXTOOLS.OLB(EXCNTSTR=EXCNTSTR.OBJ) -
     ,EXTOOLS.OLB(EXDLTSTR=EXDLTSTR.OBJ) -
     ,EXTOOLS.OLB(EXGETPCE=EXGETPCE.OBJ) -
     ,EXTOOLS.OLB(EXINSSTR=EXINSSTR.OBJ) -
     ,EXTOOLS.OLB(EXLFTSTR=EXLFTSTR.OBJ) -
     ,EXTOOLS.OLB(EXLOCCUR=EXLOCCUR.OBJ) -
     ,EXTOOLS.OLB(EXPAD=EXPAD.OBJ) -
     ,EXTOOLS.OLB(EXRGTSTR=EXRGTSTR.OBJ) -
     ,EXTOOLS.OLB(EXSCRLDN=EXSCRLDN.OBJ) -
     ,EXTOOLS.OLB(EXSCRLUP=EXSCRLUP.OBJ) -
     ,EXTOOLS.OLB(EXSCRPRN=EXSCRPRN.OBJ) -
     ,EXTOOLS.OLB(EXSETCUR=EXSETCUR.OBJ) -
     ,EXTOOLS.OLB(EXWRTCHR=EXWRTCHR.OBJ) -
     ,EXTOOLS.OLB(GETCRC=GETCRC.OBJ) -
     ,EXTOOLS.OLB(INKEY=INKEY.OBJ) -
     ,EXTOOLS.OLB(INPUTSCR=INPUTSCR.OBJ) -
     ,EXTOOLS.OLB(MENU=MENU.OBJ) -
     ,EXTOOLS.OLB(SCREEN=SCREEN.OBJ) -
     ,EXTOOLS.OLB(TERMSIZE=TERMSIZE.OBJ) -
     ,TEST.OBJ -
     ,EXTOOLS.OLB(TEXACO=TEXACO.OBJ) -
     ,EXTOOLS.OLB(WARNS=WARNS.OBJ) -
     ,EXTOOLS.OLB(WINDOW=WINDOW.OBJ) -
     !
       LINK $(DBG) $(PCAOPT) /EXE=TEST.EXE TEST.OBJ, EXTOOLS.OLB/LIBRARY -
          ! End of Link



!++
!  Create object library if it doesn't already exist
!==

.FIRST
        ! MMS $(mmsqualifiers) $(mmstargets)
       IF F$SEARCH( "EXTOOLS.OLB" ) .EQS. "" -
                THEN $(LIBR)/CREATE EXTOOLS.OLB

!++
!  End of build cleanup work
!==

!.LAST
!       CONTINUE

