/*
 *  $Id: edmove.c,v 1.2 1993/04/20 16:04:12 sev Exp $
 *
 * ---------------------------------------------------------------------------
 *
 * $Log: edmove.c,v $
 * Revision 1.2  1993/04/20 16:04:12  sev
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

#include "vced.h"

COUNT edmvleft(vced)
VCED *vced;                         /* Pointer to edit structure            */
{                                   /* ------------------------------------ */
  if((vced->edcrow == 0l) &&                    /* If current row equ zero  */
  (vced->edcchar == 0l) )                       /*  and on first char.      */
    return(0);                                  /* Return                   */
  if(vced->edcchar)                             /* If not first             */
  {                                             /*                          */
    vced->edcchar--;                            /* Dec. char.               */
    if(vced->edcchar < vced->edoffset)          /* Need to shift            */
    {                                           /*                          */
      vced->edupval |= VCEDUPDALL;              /* Update entire screen     */
      if(vced->edcchar < wcols(vced->edwptr))   /* < than screen            */
	vced->edoffset = 0;                     /* Peg to first half of scrn*/
      else                                      /*                          */
	vced->edoffset = max((vced->edoffset-vcedval.shiftleft),0);
    }                                           /*                          */
  }
  else
    return(0);
  return 1;  
}                                               /* ------------------------ */

/* ------------------------------------------------------------------------ */

COUNT edmvright(vced)
VCED *vced;                         /* Pointer to edit structure            */
{                                   /* ------------------------------------ */
  COUNT cols;                       /* Number of columns                    */

  if(vced->edcchar >= (vced->edbuffer->bflinelen-2))          /* If at eol  */
    return(0);                                  /*                          */
  else                                          /* Not at eol               */
  {                                             /*                          */
    vced->edcchar++;                            /* Inc. char.               */
    cols = wcols(vced->edwptr);                 /* Get number of columns    */
    if((vced->edcchar-vced->edoffset) >=        /* If next char off         */
    (cols+vced->edlmar))                        /* the screen               */
    {                                           /*                          */
      vced->edupval |= VCEDUPDALL;              /* Update entire screen     */
      vced->edoffset = min((vced->edoffset + vcedval.shiftleft)
	  ,(vced->edbuffer->bflinelen-1-cols));
    }                                           /* Adjust screen            */
  }
  return 1;
}
