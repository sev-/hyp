/*
 *  $Id: edopen.c,v 1.2 1993/04/20 16:04:12 sev Exp $
 *
 * ---------------------------------------------------------------------------
 *
 * $Log: edopen.c,v $
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

VCED *edopen(urow,ucol,lrow,lcol,title,wctrl,ectrl,edbuf,rmar,lmar,wp)
COUNT urow,ucol;	    /* Upper row and column		    */
COUNT lrow,lcol;	    /* Lower row and column		    */
TEXT *title;		    /* Pointer to title for window	    */
COUNT wctrl;		    /* Control value for window 	    */
COUNT ectrl;		    /* Control value for editor 	    */
VCEDBUF *edbuf; 	    /* Edit buffer			    */
COUNT rmar,lmar;	    /* Right / Left margin		    */
WPTR wp;		    /* Optional pointer to status window    */
{                           /* ------------------------------------ */
  VCED *newed;              /* Pointer to new buffer                */
  TEXT *vcalloc();          /* Declare functions used               */

  newed = (VCED *)vcalloc(1, sizeof(*newed), vcdebug); /* Get struct*/
  if(rmar == -1)                        /* If right margin -1       */
    newed->edrmar = vcedval.dftrmar;    /*   use default            */
  else                                  /* else                     */
    newed->edrmar = rmar;               /*   use value passed       */
  if(lmar == -1)                        /* If left margin -1        */
    newed->edlmar = vcedval.dftlmar;    /*   use default            */
  else                                  /* else                     */
    newed->edlmar = lmar;               /*   use value passed       */
  newed->edurow = urow;                 /* Assign upper row         */
  newed->educol = ucol;                 /* Assign upper column      */
  newed->edlrow = lrow;                 /* Assign lower row         */
  newed->edlcol = lcol;                 /* Assign lower column      */
  if(wctrl == -1)                       /* If window control == -1  */
    wctrl = wdoctrl->wstdctrl;          /*   use default value      */
  wctrl |= NOAUTO;                      /* Do not auto move         */
  newed->edctrl = wctrl;                /* Assign window control    */
  newed->edtitle = title;               /* Assign title             */
  newed->edtline = edbuf->bffline;      /* Start with first line    */
  newed->edcline = edbuf->bffline;      /* Make first line current  */
  newed->edcchar = 0;                   /* Make first character cur.*/
  newed->edtrow = 0l;                   /* Make top row first line  */
  newed->edcrow = 0l;                   /* Current row first line   */
  newed->edmode = ectrl;                /* Assign editor control    */
  newed->edbuffer = edbuf;              /* Assign buffer to edit    */
  newed->edswptr = wp;                  /* Assign status window     */
  return(newed);                        /* Return new structure     */
}                                       /* ------------------------ */
