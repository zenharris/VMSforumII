.ifdef __AXP__
ARCH    = AXP
.else
ARCH    = VAX
.endif

.ifdef GNUC
CC      = GCC
CFLAGS	=
POST    = _GNU
OPT	= GNUC_$(ARCH).OPT
LINKFLAGS= ,$(OPT)/OPT
.else
CC      = CC
CFLAGS	= /INCLUDE_DIRECTORY=[.extools]
POST    =
OPT	=
LINKFLAGS= ,[.extools]extools/lib
.endif


EXE     = EXE$(POST)
OBJ	= OBJ$(POST)


OBJECTS = FORUM.$(OBJ)

FORUM.$(EXE) : $(OBJECTS) $(OPT)
        LINK/NOTRACE/EXEC=$(MMS$TARGET) $(OBJECTS) $(LINKFLAGS)
        @ purge/nolog $(OBJECTS)


FORUM.$(OBJ) : FORUM.C FORFILE.H LOCALITY.H
        $(CC) $(CFLAGS) /OBJECT=$(MMS$TARGET)  FORUM.C


CLEAN :
        @ if f$search("*.$(OBJ)").nes."" then delete/nolog/noconfirm *.$(OBJ);*

