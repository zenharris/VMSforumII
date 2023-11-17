
#define SIZE_UNAME	12
#define SIZE_SUBJECT	160
#define SIZE_UID        16
#define SIZE_DATIME 8
#define SIZE_TIMESTR 23

#define LINE_LENGTH 100
#define TOTAL_LINES 200



#define DEFAULT_FILE_NAME ".dat"

#define RECORD_SIZE (sizeof fileio.record)



struct FAB fab,readfab;
struct RAB rab,readrab;
struct XABPRO xabpro;
struct XABKEY read_key,primary_key,alternate_key,altreverse_key,posted_key,username_key;


struct recbuff
   {
      struct timebuff  posted;
      char  msgid[SIZE_UID+1],replyto[SIZE_UID+1];
      char	user_name[SIZE_UNAME];
      char      subject[SIZE_SUBJECT];
      int16 reclen;
   };

struct
   {
       struct recbuff record;
       char lines[TOTAL_LINES][LINE_LENGTH];
   } fileio;

struct listkey
   {
      char replyto[SIZE_UID];
      char msgid[SIZE_UID];
   };

#define READRECORD_SIZE (sizeof readbuff)

struct readrec
   {
      struct timebuff posted;
   } readbuff;



char *filename;
int rms_status;

error_exit(operation)
char *operation;
{
   printf("RMSEXP - file %s failed (%s)\n",operation,filename);
   exit(rms_status);
}


void forum_initialize(fn) char *fn;
{
   fab = cc$rms_fab;

   fab.fab$b_bks = 63; /*  4;        */
   fab.fab$l_dna = DEFAULT_FILE_NAME;
   fab.fab$b_dns = sizeof DEFAULT_FILE_NAME -1;
   fab.fab$b_fac = FAB$M_DEL | FAB$M_GET |  FAB$M_PUT | FAB$M_UPD;
   fab.fab$l_fna = fn;
   fab.fab$b_fns = strlen(fn);
   fab.fab$l_fop = FAB$M_CIF;
   fab.fab$w_mrs = 0;  /*RECORD_SIZE; */
   fab.fab$b_org = FAB$C_IDX;
   fab.fab$b_rat = FAB$M_CR;
   fab.fab$b_rfm = FAB$C_VAR;  /*FAB$C_FIX; */
   fab.fab$b_shr = FAB$V_SHRDEL | FAB$V_SHRGET | FAB$V_SHRPUT | FAB$V_SHRUPD;  /*FAB$M_NIL;  */
   fab.fab$l_xab = &primary_key;  /* &xabpro;    */

   rab = cc$rms_rab;
   rab.rab$l_fab = &fab;

   xabpro = cc$rms_xabpro;
   xabpro.xab$w_pro = 0xFC00; /* 64512; /*"s:rwed,o:rwed,g:rw,w"; */
   xabpro.xab$l_nxt = &primary_key; 


   primary_key = cc$rms_xabkey;
   
   primary_key.xab$b_dtp = XAB$C_BN8;
   primary_key.xab$b_flg = 0;
   primary_key.xab$w_pos0 = ((char *)&fileio.record.posted - (char *)&fileio.record);
   primary_key.xab$b_siz0 = SIZE_DATIME;
   primary_key.xab$b_ref = 0;
   primary_key.xab$l_nxt = &alternate_key;


   alternate_key = cc$rms_xabkey;

   alternate_key.xab$b_dtp = XAB$C_STG;
   alternate_key.xab$b_flg = 0;
   alternate_key.xab$w_pos0 = ((char *)&fileio.record.replyto - (char *)&fileio.record);
   alternate_key.xab$b_siz0 = SIZE_UID;
   alternate_key.xab$w_pos1 = ((char *)&fileio.record.msgid - (char *)&fileio.record);
   alternate_key.xab$b_siz1 = SIZE_UID;
   alternate_key.xab$b_ref = 1;
   alternate_key.xab$l_nxt = &altreverse_key;


   altreverse_key = cc$rms_xabkey;

   altreverse_key.xab$b_dtp = XAB$C_DSTG;
   altreverse_key.xab$b_flg = 0;
   altreverse_key.xab$w_pos0 = ((char *)&fileio.record.replyto - (char *)&fileio.record);
   altreverse_key.xab$b_siz0 = SIZE_UID;
   altreverse_key.xab$w_pos1 = ((char *)&fileio.record.msgid - (char *)&fileio.record);
   altreverse_key.xab$b_siz1 = SIZE_UID;
   altreverse_key.xab$b_ref = 2;
   altreverse_key.xab$l_nxt = &username_key;



   username_key = cc$rms_xabkey;

   username_key.xab$b_dtp = XAB$C_STG;
   username_key.xab$b_flg = XAB$M_DUP;
   username_key.xab$w_pos0 = ((char *) &fileio.record.user_name - (char *) &fileio.record) ; 
   username_key.xab$b_siz0 = SIZE_UNAME;
   username_key.xab$b_ref = 3;


}

open_file()
{
   rms_status = sys$create(&fab);
   if (rms_status != RMS$_NORMAL &&
       rms_status != RMS$_CREATED)
      error_exit("$OPEN");

   if (rms_status == RMS$_CREATED)
      printf("[Created New Data file.]\n");

   rms_status = sys$connect(&rab);
   if (rms_status != RMS$_NORMAL)
      error_exit("$CONNECT");

}


void initread(fn)
char *fn;
{

   readfab = cc$rms_fab;

   readfab.fab$b_bks = 4;
   readfab.fab$l_dna = DEFAULT_FILE_NAME;
   readfab.fab$b_dns = sizeof DEFAULT_FILE_NAME -1;
   readfab.fab$b_fac = FAB$M_DEL | FAB$M_GET |  FAB$M_PUT | FAB$M_UPD;
   readfab.fab$l_fna = fn;
   readfab.fab$b_fns = strlen(fn);
   readfab.fab$l_fop = FAB$M_CIF;
   readfab.fab$w_mrs = READRECORD_SIZE;
   readfab.fab$b_org = FAB$C_IDX;
   readfab.fab$b_rat = FAB$M_CR;
   readfab.fab$b_rfm = FAB$C_FIX;
   readfab.fab$b_shr = FAB$V_SHRDEL | FAB$V_SHRGET | FAB$V_SHRPUT | FAB$V_SHRUPD;
   readfab.fab$l_xab = &read_key;


   readrab = cc$rms_rab;
   readrab.rab$l_fab = &readfab;
    

   read_key = cc$rms_xabkey;
   
   read_key.xab$b_dtp = XAB$C_BN8;
   read_key.xab$b_flg = 0;
   read_key.xab$w_pos0 = ((char *)&readbuff.posted - (char *)&readbuff);
   read_key.xab$b_siz0 = SIZE_DATIME;
   read_key.xab$b_ref = 0;

}

open_readfile()
{
   rms_status = sys$create(&readfab);
   if (rms_status != RMS$_NORMAL &&
       rms_status != RMS$_CREATED)
      error_exit("$OPEN");

   if (rms_status == RMS$_CREATED)
      printf("[Created New Read Index file.]\n");

   rms_status = sys$connect(&readrab);
   if (rms_status != RMS$_NORMAL)
      error_exit("$CONNECT");

}



