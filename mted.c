/*
 *  $Id: mted.c,v 1.4 1993/03/17 13:01:34 sev Exp $
 *
 * ---------------------------------------------------------------------------
 *
 *  $Log: mted.c,v $
 *  Revision 1.4  1993/03/17 13:01:34  sev
 *  Добавлен mted.h
 *
 * Revision 1.3  1993/03/15  14:58:47  sev
 * Опущено окно на одну строку
 *
 * Revision 1.2  1993/03/15  14:47:57  sev
 * Изменил структуру меню. По прежнему просто декорация.
 *
 * Revision 1.1  1993/03/14  12:32:14  kas
 * Initial revision
 *
 * Revision 1.1  1993/03/13  16:49:17  kas
 * Initial revision
 *
 *
*/

#define VCGET_DEFS
#include <vcstdio.h>
#include <vcdbms.h>
#include <vced.h>

#define DEFILE (TEXT *)"noname.hyp"

TEXT vceddfil[40];                      /* Default file location    */
    
/* ---------------------------------------------------------------- */

main(argc,argv)
int argc;                               /* Number of arg.           */
TEXT **argv;                            /* Command line arg.        */
    {                                   /* ------------------------ */
    COUNT key;                          /* Key returned             */
    COUNT changed;                      /* If files changed         */
    WPTR wptr;                          /* Status window            */
    VCEDBUF *vcedfbuf;                  /* Buffer for edit          */
    VCEDBUF *vcedload();                /* Declare functions        */
    VCED *vcedopen();                   /*    "        "            */
    VCED *vcedw;                        /*    "        "            */
    WPTR wxopen();                      /*    "        "            */
    TEXT *vcalloc();                    /*    "        "            */
    COUNT i;

/* ---------------------------------------------------------------- */

    vcstart(CLRSCRN);                   /* Start Vitamin C          */
    setkeytrap(NULLFUNC);               /* Stop keytrap function    */
    vcsignal();                         /* Start signal processing  */

    setaskmsg(22,0,22,40,-1,0,NULLTEXT);/* Setup askmsg location    */
    wptr = wxopen(22,0,22,40,NULLTEXT,ACTIVE,0,0);      /* Open stat*/
							/* Window   */
    if(argc < 2)                        /* If no file on line       */
        {                               /*                          */
        if(vcedaskfil() == spkey.abort) /* If escape entered        */
            {                           /*                          */
            vcend(CLOSE);               /* Close windows            */
            exit(0);                    /* Leave editor             */
            }                           /*                          */
        if(isblank(vceddfil))           /* If entered blank         */
            strcpy(vceddfil,DEFILE);    /* Copy default file name   */
        }                               /*                          */
    else                                /*                          */
	 strcpy(vceddfil,argv[1]);

	i = strlen(vceddfil);
	if(strcmp(vceddfil+ (i<5?0:i-4), ".hyp"))
	{
	   printf("\rПредупреждение: Файл %s не имеет расширения .hyp\r",
					vceddfil);
	   fflush(stdout);
	}

/* ---------------------------------------------------------------- */

    vcedfbuf = vcedload(vceddfil,NULLTEXT,VCEDFFIL);
    vcedw    = vcedopen(1,0,20,78,vceddfil,-1,VCEDSTATUS,vcedfbuf,-1,-1,wptr);
    vcedit(vcedw);                      /*                          */

    vcend(CLOSE);                       /* Quit Vitamin C           */
    execlp("clear","clear",(char *)NULL);
    }                                   /* ------------------------ */

/* ---------------------------------------------------------------- */

COUNT vcedaskfil()
    {                       /* ------------------------------------ */
    WPTR fwptr;             /* Current window                       */
    COUNT key;              /* Key returned from readgets           */
    WPTR wxopen();          /* Declare functions used               */
    TEXT *vcalloc();        /*    "        "      "                 */

    empty(vceddfil,30);                 /* Empty file name          */
    fwptr = wxopen(20,0,20,40,                      /* Open window  */
		    NULL,ACTIVE+SCROLL+CURSOR,0,0);
    atsay(0,1,"File to Edit:");         /* Prompt                   */
    atgetc(0,15,vceddfil,NULLTEXT,12,FLDBLANK+FLDSTRING,NULLTEXT);
    key = readgets();                   /* Get file name            */
    trim(vceddfil);                     /* Trim file name           */
    wclose(fwptr);                      /* Close window             */
    return(key);                        /* Return O.K.              */
    }                                   /* ------------------------ */

/* ---------------------------------------------------------------- */
