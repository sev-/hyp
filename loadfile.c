/*
 *  $Id: loadfile.c,v 1.1 1993/04/06 14:14:07 sev Exp $
 *
 * ---------------------------------------------------------------------------
 *
 * $Log: loadfile.c,v $
 * Revision 1.1  1993/04/06 14:14:07  sev
 * Initial revision
 *
 * Revision 1.2  1993/03/27  12:08:16  kas
 * *** empty log message ***
 *
 * Revision 1.1  1993/03/12  15:44:23  sev
 * Init
 *
 *
 */

static char rcsid[]="$Id: loadfile.c,v 1.1 1993/04/06 14:14:07 sev Exp $";

#include "vced.h"

VCEDBUF *loadfile(fname,bloc,flag)
TEXT *fname;                        /* Filename for file and buffer title   */
TEXT *bloc;                         /* Pointer to Buffer                    */
COUNT flag;                         /* Control Flags                        */
{                                   /* ------------------------------------ */
  VCEDBUF *newbuf;                  /* Pointer to new buffer                */
  COUNT size;                       /* Size of line with buffer and control */
  COUNT err = FALSE;                /* Error condition                      */

  newbuf = (VCEDBUF *)vcalloc(1,sizeof(*newbuf),vcdebug);/* Buffer*/
  newbuf->bflinelen = vcedval.linlen;           /* Assign line len          */
  newbuf->bfbuffer = bloc;                      /* Assign buffer            */
  newbuf->bfctrl = flag;                        /* Assign control           */
  newbuf->bfnlines = 0l;                        /* Reset line nums.         */
  newbuf->bffline = (DBDP)0;                    /* Make first line empty    */
  newbuf->bfbline = (DBDP)0;                    /* Set last line empty      */
  if(fname != NULLTEXT)                         /* If there is file name    */
  {                                             /*                          */
    newbuf->bfname = vcalloc(1,strlen(fname)+1,vcdebug);/* Alloc*/
    strcpy(newbuf->bfname,fname);               /* Assign File name         */
  }                                             /*                          */
  else                                          /*                          */
    newbuf->bfname = fname;                     /* Assign NULL value        */

  newbuf->bfperm = 0;                           /* Assign permission        */
  size = sizeof(VCEDLINE) + newbuf->bflinelen;  /* Calc line length         */
  if((newbuf->bfdnum = dbmsxopn(-1,size,0,1,0,  /* Open database            */
  vcedval.pages,vcedval.slots)) == (DBP)0)      /* for storage              */
  {                                             /* If error                 */
    askmsg("Editor Message","Error opening internal File",0);
    return((VCEDBUF *)0);                       /* Return NULL              */
  }                                             /*                          */
  if(newbuf->bfctrl & VCEDFBUF)                 /* Read from buffer?        */
    err = vcedfrmbuf(newbuf);                   /*   read buffer            */
  else                                          /*  else                    */
    err = file_to_buf(newbuf);                  /*   read file              */
  newbuf->bfchange = FALSE;                     /* File not changed         */
  return(newbuf);                               /*   return buffer          */
}                                               /* ------------------------ */
