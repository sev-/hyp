/*
 *  $Id: edupda.c,v 1.3 1993/04/20 16:04:12 sev Exp $
 *
 * ---------------------------------------------------------------------------
 *
 * $Log: edupda.c,v $
 * Revision 1.3  1993/04/20 16:04:12  sev
 * a
 *
 * Revision 1.3  1993/04/15  15:05:51  kas
 * *** empty log message ***
 *
 * Revision 1.3  1993/04/15  15:05:51  kas
 * *** empty log message ***
 *
 * Revision 1.2  1993/04/13  13:50:41  kas
 * *** empty log message ***
 *
 * Revision 1.2  1993/04/13  13:50:41  kas
 * *** empty log message ***
 *
 * Revision 1.1  1993/04/12  15:13:06  kas
 * Initial revision
 *
 * Revision 1.1  1993/04/12  15:13:06  kas
 * Initial revision
 *
 *
 */

#include <string.h>
#include "vced.h"

COUNT edupdate(vced)
VCED *vced;                         /* Pointer to edit structure            */
{                                   /* ------------------------------------ */
  COUNT col;
  static COUNT fast = 0;            /* Restore complete window (type ahead) */
  static DBDP row = (DBDP)-1;       /* Row on when fast started             */
  static LONG line = (DBDP)-1;      /* Line on when fast started            */
  VCEDLINE *lptr;

  if((vced->edaline != (DBDP)0) && !vced->edupval)
    vced->edupval = VCEDUPDALL;     /* Update screen                        */
  if(vckeyrdy && keyrdy() &&        /* If another key waiting               */
  (vced->edupval & VCEDUPDALL) )    /* And update entire screen             */
  {                                 /*                                      */
    fast = TRUE;                    /* Flag type ahead                      */
    if(vced->edmode & VCEDSTATUS)   /* If display status line               */
      vcedstatus(vced);             /* Display status line                  */
    vcedaddcur(vced);               /* Address cursor                       */
    return(0);                      /* Return and process                   */
  }                                 /*                                      */
  if(fast || (vced->edupval & VCEDUPDALL))/* Update entire screen           */
  {                                 /*                                      */
    fast = FALSE;                   /* Reset fast variable                  */
    edprtbuf(vced);                 /* Restore entire screen                */
    vced->edupval = 0;              /* Clear update flag                    */
  }                                 /*                                      */
  if((vced->edupval & VCEDUPDLINE) ||  /* Update line ?                     */
  (vced->edupval & VCEDUPDPEOL)||      /* Update prev. char to eol          */
  (vced->edupval & VCEDUPDEOL) )       /* Update character to eol           */
  {
    lptr = vcedgline(vced->edbuffer,vced->edcline);
    edsetfta(vced, vced->edcline, lptr,from,to,stat_);
    edprtline(vced,vced->edcline,from,to,stat_);
  }                                 /*                                      */
  if(vced->edupval & VCEDUPDPCHAR)  /* Update previous char                 */
  {                                 /*                                      */
    col = max(0,vced->edcchar-1);   /* Max current char or 0                */
    vcedprtchar(vced,vced->edcline,col);/* Update line                      */
  }                                 /*                                      */
  if(vced->edupval  & VCEDUPDCHAR)  /* Update character                     */
  {
    col = max(0,vced->edcchar);     /* Max current char or 0                */
    vcedprtchar(vced,vced->edcline,col);/* Update line                      */
  }                                 /*                                      */
  vced->edupval = 0;                /* Clear update flag                    */
  if(vced->edmode & VCEDSTATUS)     /* If display status line               */
    vcedstatus(vced);               /* Display status line                  */
  vcedaddcur(vced);                 /* Address cursor                       */
}                                   /* ------------------------------------ */
