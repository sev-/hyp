/*
 *  $Id: editor.c,v 1.2 1993/04/08 10:37:38 sev Exp $
 *
 * ---------------------------------------------------------------------------
 *
 * $Log: editor.c,v $
 * Revision 1.2  1993/04/08 10:37:38  sev
 * drawseg removed
 *
 * Revision 1.1  1993/04/06  14:14:07  sev
 * Initial revision
 *
 * Revision 1.10  1993/04/05  16:01:33  kas
 * *** empty log message ***
 *
 * Revision 1.9  1993/04/02  16:46:36  kas
 * *** empty log message ***
 *
 * Revision 1.8  1993/04/01  14:10:29  kas
 * *** empty log message ***
 *
 * Revision 1.7  1993/03/29  13:48:35  kas
 * *** empty log message ***
 *
 * Revision 1.6  1993/03/27  12:08:16  kas
 * *** empty log message ***
 *
 * Revision 1.5  1993/03/25  12:22:15  kas
 * *** empty log message ***
 *
 * Revision 1.4  1993/03/23  13:10:22  kas
 * *** empty log message ***
 *
 * Revision 1.3  1993/03/15  10:55:59  sev
 * Все - таки он не совсем корректно считывал справочник
 *
 * Revision 1.2  1993/03/14  18:36:54  sev
 * Была одна маленькая деталь: если не вызывать функцию "СПРАВОЧНИК" из
 * меню, а сразу привязывать поля, то СПРАВОЧНИК ПУСТ ??? Исправлено
 *
 * Revision 1.1  1993/03/12  15:44:23  sev
 * Init
 *
 *
 */

static char rcsid[]="$Id: editor.c,v 1.2 1993/04/08 10:37:38 sev Exp $";

#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "vced.h"
#include "edinitstr.h"

#define DEFILE (TEXT *)"noname.hyp"

struct VCEDFILE *vcedfiles = (struct VCEDFILE *)0; /* List of files         */
struct VCEDWDO *vcedwdos = (struct VCEDWDO *)0;    /* List of window        */
struct VCEDFILE *cfile;                            /* Current buffer        */

/* ======================== M A I N ======================================= */

main(argc,argv)
int argc;                                       /* Number of arg.           */
TEXT **argv;                                    /* Command line arg.        */
{                                               /* ------------------------ */
  struct VCEDFILE *curfile;                     /* Current buffer           */
  struct VCEDFILE *tmpfile1;                    /* Temporary buffer         */
  struct VCEDWDO *curwdo;                       /* Current window           */
  struct VCEDWDO *tmpwdo;                       /* Temporary window         */
  COUNT key;                                    /* Key returned             */
  COUNT key_dir;                                /* Key returned             */
  COUNT more;
  COUNT changed;                                /* If files changed         */
  WPTR wptr;                                    /* Status window            */
  WPTR wmesg;
  int i,j;
  SELSET *tmp;

  vcstart(CLRSCRN);                             /* Start Vitamin C          */

  setkeytrap(NULLFUNC);                         /* Stop keytrap function    */
  edsignal();                                   /* Start signal processing  */

  dirr    = aloselitm();   /* init list of files from dir of segments       */
  listwdo = aloselitm();   /* init list of files for editor                 */

  show=OFF_;
  if(argc < 2)                                  /* If no file on line       */
  {                                             /*                          */
    if(vcedaskfil() == spkey.abort)             /* If escape entered        */
    {                                           /*                          */
      vcend(CLOSE);                             /* Close windows            */
      exit(0);                                  /* Leave                    */
    }                                           /*                          */
    if(isblank(vceddfil))                       /* If entered blank         */
      strcpy(vceddfil,DEFILE);                  /* Copy default file name   */
    curfile = (struct VCEDFILE *)vcalloc(1,sizeof(*curfile),vcdebug);
    curfile->vcedfname=vceddfil;                /* Assign file name         */
    curfile->vcedfstat = 0;                     /* Not opened yet           */
    curfile->vcedfnext = vcedfiles;             /* Assign next buffer       */
    vcedfiles = curfile;                        /* Make head of list        */
  }                                             /*                          */
  else                                          /*                          */
  {                                             /*                          */
    argc--;                                     /* Subtract for prog. name  */
    argv++;                                     /* Point to first name      */
    while(argc)                                 /* While more files on line */
    {
      i = strlen(*argv);
/*
      if(strcmp(*argv + (i<5?0:i-4), ".hyp"))
    printf("Предупреждение: Файл %s \
не имеет расширения .hyp\n\r", *argv);
*/
      curfile = (struct VCEDFILE *)vcalloc(1,sizeof(*curfile),vcdebug);
      curfile->vcedfname  = *argv;              /* Assign file name         */
      curfile->vcedfstat = 0;                   /* Not opened yet           */
      curfile->vcedftop = 0;                    /* Start on first line      */
      curfile->vcedfcur = 0;                    /* Start on first line      */
      curfile->vcedfnext = vcedfiles;           /* Assign next buffer       */
      vcedfiles = curfile;                      /* Make head of list        */
      argc--;                                   /* Subtract arg.            */
      argv++;                                   /* Next arg                 */
    }                                           /*                          */
  }                                             /*                          */

  cfile = curfile;

  askdirfile();

  wptr = wxxopen(vcterm.rows-1,0,vcterm.rows-1, /* Open stat                */
  vcterm.cols-2,NULLTEXT,ACTIVE,0,0,1,32);      /* Window                   */

  curfile = vcedfiles;                          /* Start at last file       */
  while(curfile->vcedfnext != (struct VCEDFILE *)0)  /* Goto top            */
    curfile = curfile->vcedfnext;               /* Look at next             */
  vcedwdos = (struct VCEDWDO *)vcalloc(1,sizeof(*vcedwdos),vcdebug);
  vcedwdos->vcedwnext = (struct VCEDWDO *)0;    /* No next window           */
  curwdo = vcedwdos;                            /* Start with first window  */
  curfile->vcedfbuf = loadfile(curfile->vcedfname,NULLTEXT,VCEDFFIL);
  curfile->vcedfstat=TRUE;
  edftostr();
  number_seg(curfile->vcedfname);

  curfile->vcedcolseg=col_seg_of_file;
    addselitm(listwdo,curfile->vcedfname,curfile->vcedfname);

  curwdo->vcedwvced = edopen(1,0,vcterm.rows-3,vcterm.cols-2,
      curfile->vcedfname,ACTIVE+BORDER+BD2+CURSOR+COOKED,VCEDSTATUS,
      curfile->vcedfbuf,-1,-1,wptr);            /* Open window              */
  edwopen(curwdo->vcedwvced);

  curwdo->vcedwstat = TRUE;

  wmesg=wxxopen(0,0,0,vcterm.cols-4,NULLTEXT,ACTIVE,0,SEGOFLINE,0,32);

  edprtbuf(curwdo->vcedwvced);
  stat_hyp=ED;

  while(TRUE)                                   /* While still editing      */
  {
    werase(wmesg,ATR_F);
    medit(curwdo->vcedwvced);                   /* Edit current window      */
    if(curwdo->vcedwvced->edmail == VCEDNEWFIL) /* Wants new fil            */
    {
      key_dir=curwdo->vcedwvced->edkey[0];
      if((curwdo->vcedwvced->edkey[0]!=DIR_SEG) &&
   (curwdo->vcedwvced->edkey[0]!=LISTWDO) &&
   (curwdo->vcedwvced->edkey[0]!=FILES)  &&
   (curwdo->vcedwvced->edkey[0]!=SCRN_SEG)&&
   (curwdo->vcedwvced->edkey[0]!=INF_CONC))
  key = vcedaskfil();                           /* Get file name            */
      else
  key=RET;

      if((key != spkey.abort) &&                /* If abort not entered     */
      !isblank(vceddfil)  )                     /* And is not empty         */
      {                                         /*                          */
  curfile = vcedfiles;
  while(curfile != (struct VCEDFILE *)0)
  {
    if(strcmp(curfile->vcedfbuf->bfname,vceddfil)==0)
      break;
    curfile = curfile->vcedfnext;
  }
  if(curfile != (struct VCEDFILE *)0)
  {
    if(strcmp(curwdo->vcedwvced->edbuffer->bfname,curfile->vcedfname)==0)
      tmpfile1=curfile;
    else
    {
      tmpfile1=curfile;
      curfile=vcedfiles;
      while(curfile != (struct VCEDFILE *)0)
      {
	if(curfile->vcedfstat)
    if(curfile->vcedfbuf == curwdo->vcedwvced->edbuffer)
       break;
	curfile = curfile->vcedfnext;
      }
      memcurfile(curfile,curwdo);
      memcurwdo(curwdo,tmpfile1);
    }                                           /*                          */
  }
  else
  {
    curfile=vcedfiles;
    while(curfile != (struct VCEDFILE *)0)
    {
      if(curfile->vcedfstat)
	if(curfile->vcedfbuf == curwdo->vcedwvced->edbuffer)
    break;
      curfile = curfile->vcedfnext;
    }
    tmpfile1 = (struct VCEDFILE *)vcalloc(1,sizeof(struct VCEDFILE),
	vcdebug);
    tmpfile1->vcedfname = vceddfil;             /* Assign file name         */
    tmpfile1->vcedfstat = 0;                    /* Not opened yet           */
    tmpfile1->vcedftop = 0;                     /* Start on first line      */
    tmpfile1->vcedfcur = 0;                     /* Start on first line      */
    tmpfile1->vcedfnext = vcedfiles;            /* Assign nxt buf           */
    vcedfiles = tmpfile1;                       /* Make head of list        */

    tmpfile1->vcedfbuf =loadfile(tmpfile1->vcedfname,NULLTEXT,VCEDFFIL);
    tmpfile1->vcedfstat = TRUE;                 /* File opened              */
    tmpfile1->vcedftop = 0;                     /* Starting values          */
    tmpfile1->vcedfcur = 0;                     /*    "      "              */
    tmpfile1->vcedftlin = tmpfile1->vcedfbuf->bffline;
    tmpfile1->vcedfclin = tmpfile1->vcedfbuf->bffline;
    tmpfile1->vcedfarow = 0l;                   /* Starting values          */
    tmpfile1->vcedfalin = (DBDP)0;              /*   "      "               */
    tmpfile1->vcedfchr = 0;                     /*    "      "              */
    tmpfile1->vcedfachr = 0;                    /*    "      "              */
    tmpfile1->vcedfoff = 0;                     /*    "      "              */
    memcurfile(curfile,curwdo);
    number_seg(tmpfile1->vcedfname);
    tmpfile1->vcedcolseg=col_seg_of_file;
    memcurwdo(curwdo,tmpfile1);
    addselitm(listwdo,vceddfil,vceddfil);
    cfile=tmpfile1;
  }

      cfile=tmpfile1;
      if(key_dir==DIR_SEG)
      {
    strcpy(tmpfile1->mesg,privyz.mesg);
    edprtbuf(curwdo->vcedwvced);
/*	  strcpy(cfile->drawseg.draw_name,privyz.name_seg);
*/     }

      if(stat_hyp==PRIV_SEGWDO && (key_dir==SCRN_SEG || key_dir==INF_CONC))
	     privmsg(curwdo->vcedwvced);
      stat_hyp=(stat_hyp==PRIV_SEGWDO)?stat_hyp:ED;
      curwdo->vcedwvced->edmail = 0;
      curwdo->vcedwvced->edupval = VCEDUPDALL;

      }
      curwdo->vcedwvced->edmail = 0;            /* Clear mail               */
    }                                           /*                          */
    if(curwdo->vcedwvced->edmail == VCEDNEXTFIL)/* Next buf                 */
    {                                           /*                          */
      curfile = vcedfiles;                      /* Start at end of files    */
      while(curfile != (struct VCEDFILE *)0)    /* While more               */
      {                                         /*                          */
  if(curfile->vcedfstat)                        /* If file opened           */
    if(curfile->vcedfbuf == curwdo->vcedwvced->edbuffer)
      break;                                    /* If this is it break      */
  curfile = curfile->vcedfnext;                 /* Next file                */
      }                                         /*                          */
      if(curfile != (struct VCEDFILE *)0)       /* If found                 */
      {                                         /*                          */
  if(curfile->vcedfnext != (struct VCEDFILE *)0)
    tmpfile1 = curfile->vcedfnext;              /* Assign next              */
  else                                          /*                          */
    tmpfile1 = vcedfiles;                       /* Must loop to top         */
      }                                         /*                          */
      if(tmpfile1->vcedfbuf != curwdo->vcedwvced->edbuffer)
      {                                         /* If buffer should change  */
  memcurfile(curfile,curwdo);
  if(!tmpfile1->vcedfstat)                      /* If not open              */
  {                                             /*                          */
    tmpfile1->vcedfbuf = loadfile(tmpfile1->vcedfname,NULLTEXT,VCEDFFIL);
    number_seg(tmpfile1->vcedfname);
    tmpfile1->vcedcolseg=col_seg_of_file;
    tmpfile1->vcedfstat = TRUE;                 /* File opened              */
    tmpfile1->vcedftop = 0;                     /* Starting values          */
    tmpfile1->vcedfcur = 0;                     /*    "      "              */
    tmpfile1->vcedftlin = tmpfile1->vcedfbuf->bffline;
    tmpfile1->vcedfclin = tmpfile1->vcedfbuf->bffline;
    tmpfile1->vcedfarow = 0l;                   /* Starting values          */
    tmpfile1->vcedfalin = (DBDP)0;              /*   "      "               */
    tmpfile1->vcedfchr = 0;                     /*    "      "              */
    tmpfile1->vcedfachr = 0;                    /*    "      "              */
    tmpfile1->vcedfoff = 0;                     /*    "      "              */
    cfile=tmpfile1;
  }                                             /*                          */
  memcurwdo(curwdo,tmpfile1);
  cfile=tmpfile1;
  curwdo->vcedwvced->edupval = VCEDUPDALL;      /* Upate                    */
      }                                         /*                          */
      else                                      /*                          */
      {                                         /*                          */
  if(curwdo->vcedwvced->edswptr != (WPTR)0)
    watsay(curwdo->vcedwvced->edswptr,0,1,
	"Нет следующего файла      ");
	 curwdo->vcedwvced->edsline = 1;        /* Clear status line        */
      }                                         /*                          */
      curwdo->vcedwvced->edmail = 0;            /* Clear mail               */
    }
    if(curwdo->vcedwvced->edmail == VCEDDELFILE)
    {
      if((stat_hyp==PRIV_SEGWDO) && (strcmp(curfile->vcedfname,privyz.file_par)==0))
      {
   ask_msg("В этом файле вы собирались привязать поле.",0);
   curwdo->vcedwvced->edmail=0;
   continue;
      }
      curfile = vcedfiles;
      i=0;
      while(curfile != (struct VCEDFILE *)0)
      {
  i++;
  if(curfile->vcedfbuf == curwdo->vcedwvced->edbuffer)
    break;
  curfile = curfile->vcedfnext;
      }
      tmpfile1=curfile;
       key=ask_msg("Удалять из окна:",3);
       if(key==-1)
       {
     curwdo->vcedwvced->edmail = 0;
     continue;
       }
       if(key==0 && (tmpfile1->vcedfbuf->bfchange) && (tmpfile1->vcedfstat))
    vcedsave(curfile->vcedfbuf);

      curfile = vcedfiles;
      for(j=1;j< i-1;j++)
  curfile = curfile->vcedfnext;
      if(i==1)
      {
  if(curfile->vcedfnext==(struct VCEDFILE *)0)
  {
    ask_msg("Последний файл не удаляется.",0);
    curwdo->vcedwvced->edmail = 0;
    continue;
  }
  vcedfiles=curfile->vcedfnext;
  curfile=vcedfiles;
      }
      else
  if(tmpfile1->vcedfnext==(struct VCEDFILE*)0)
    curfile->vcedfnext=(struct VCEDFILE*)0;
  else
    curfile->vcedfnext=tmpfile1->vcedfnext;
      tmp=getselitm(listwdo,cfile->vcedfbuf->bfname,cfile->vcedfbuf->bfname);
      if(tmp==(SELSET *)0)
	   ask_msg(cfile->vcedfbuf->bfname,0);
      delselitm(tmp,tmp);
      if(tmpfile1->vcedfstat)
	    vcedkill(tmpfile1->vcedfbuf);       /* Close buffer             */
      vcfree(tmpfile1,vcdebug);                 /* Free link                */

      memcurwdo(curwdo,curfile);
      cfile=curfile;
      curwdo->vcedwvced->edupval = VCEDUPDALL;  /* Upate                    */
      curwdo->vcedwvced->edmail = 0;

    }

    if(curwdo->vcedwvced->edmail == VCEDABORT)  /* Abort editor             */
    {
      changed = FALSE;
      curfile = vcedfiles;
      while(curfile != (struct VCEDFILE *)0)    /* While more               */
      {
  if(curfile->vcedfstat)
    if(curfile->vcedfbuf->bfchange)             /* If file chg.             */
      changed = TRUE;
  curfile = curfile->vcedfnext;                 /* Next file                */
      }
      if(changed)
      {
  more=0;
  key=ask_msg("Выход :",3);
  switch(key)
  {
  case 0:
    curfile = vcedfiles;
    while(curfile != (struct VCEDFILE *)0)
    {
      if(curfile->vcedfstat)
	vcedsave(curfile->vcedfbuf);
      curfile = curfile->vcedfnext;
    }
    more=1;
    break;

  case 1:
    more=1;
    break;
  case -1:
    curwdo->vcedwvced->edmail = 0;
    break;

  }
  if(more)
    break;
      }                                         /*                          */
      else                                      /*                          */
  break;                                        /* Exit editor              */
    }
  }                                             /*                          */
  curfile = vcedfiles;                          /* Start at end of files    */
  while(curfile != (struct VCEDFILE *)0)        /* While more               */
  {                                             /*                          */
    tmpfile1 = curfile->vcedfnext;              /* Next file                */
    if(curfile->vcedfstat)                      /* If file opened           */
      vcedkill(curfile->vcedfbuf);              /* Close buffer             */
    vcfree(curfile,vcdebug);                    /* Free link                */
    curfile = tmpfile1;                         /* Next file                */
  }                                             /*                          */
  curwdo = vcedwdos;                            /* Start at top             */
  while(curwdo != (struct VCEDWDO *)0)          /* While more windows       */
  {                                             /*                          */
    tmpwdo = curwdo->vcedwnext;                 /* Next window              */
    if(curwdo->vcedwstat)                       /* If window opened         */
      vcedclose(curwdo->vcedwvced);             /* Close window             */
    vcfree(curwdo,vcdebug);                     /* Free link                */
    curwdo = tmpwdo;                            /* Next window              */
  }                                             /*                          */
  vcend(CLOSE);                                 /* Quit Vitamin C           */
  execlp("clear","clear",(char *)NULL);

  return 0;
}

/* ------------------------------------------------------------------------ */

COUNT vcedaskfil()
{                                   /* ------------------------------------ */
  WPTR fwptr;                       /* Current window                       */
  COUNT key;                        /* Key returned from readgets           */
				    /* ------------------------------------ */
  empty(vceddfil,75);               /* Empty file name                      */
  fwptr = wxxopen(vcterm.rows-1,0,vcterm.rows-1,   /* Open window           */
  27,NULL,ACTIVE|SCROLL|CURSOR,0,0,1,32);
  atsay(0,1,"Имя файла [       ]");
  atgetc(0,12,vceddfil,NULLTEXT,13,FLDBLANK+FLDSTRING,NULLTEXT);
  key = readgets();                 /* Get file name                        */
  trim(vceddfil);                   /* Trim file name                       */
  wclose(fwptr);                    /* Close window                         */
  return(key);                      /* Return O.K.                          */
}                                   /* ------------------------------------ */

/* ------------------------------------------------------------------------ */

COUNT memcurfile(curfil,currwstr)
struct VCEDFILE   *curfil;
struct VCEDWDO	  *currwstr;
{
  curfil->vcedftop = currwstr->vcedwvced->edtrow;
  curfil->vcedfcur = currwstr->vcedwvced->edcrow;
  curfil->vcedftlin = currwstr->vcedwvced->edtline;
  curfil->vcedfclin = currwstr->vcedwvced->edcline;
  curfil->vcedfarow = currwstr->vcedwvced->edarow;
  curfil->vcedfalin = currwstr->vcedwvced->edaline;
  curfil->vcedfchr = currwstr->vcedwvced->edcchar;
  curfil->vcedfachr = currwstr->vcedwvced->edachar;
  curfil->vcedfoff = currwstr->vcedwvced->edoffset;
  curfil->vcedcolseg=col_seg_of_file;
}

/* ------------------------------------------------------------------------ */

COUNT memcurwdo(curwdo,tmpfile1)
struct VCEDWDO	  *curwdo;
struct VCEDFILE   *tmpfile1;
{

  curwdo->vcedwvced->edbuffer = tmpfile1->vcedfbuf;
  curwdo->vcedwvced->edcrow = tmpfile1->vcedfcur;
  curwdo->vcedwvced->edtrow = tmpfile1->vcedftop;
  curwdo->vcedwvced->edtline = tmpfile1->vcedftlin;
  curwdo->vcedwvced->edcline = tmpfile1->vcedfclin;
  curwdo->vcedwvced->edaline = tmpfile1->vcedfalin;
  curwdo->vcedwvced->edarow = tmpfile1->vcedfarow;
  curwdo->vcedwvced->edcchar = tmpfile1->vcedfchr;
  curwdo->vcedwvced->edachar = tmpfile1->vcedfachr;
  curwdo->vcedwvced->edoffset = tmpfile1->vcedfoff;
  vcfree(curwdo->vcedwvced->edwptr->title,vcdebug);
  curwdo->vcedwvced->edwptr->title = (TEXT *)vcalloc(1,
      strlen(curwdo->vcedwvced->edbuffer->bfname)+1,vcdebug);
  strcpy(curwdo->vcedwvced->edwptr->title,
      curwdo->vcedwvced->edbuffer->bfname);
  curwdo->vcedwvced->edupval = VCEDUPDALL;       /* Upate                   */
  col_seg_of_file=tmpfile1->vcedcolseg;
}

/* ------------------------------------------------------------------------ */
