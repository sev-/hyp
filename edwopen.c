/*
 *  $Id: edwopen.c,v 1.1 1993/04/06 14:14:07 sev Exp $
 *
 * ---------------------------------------------------------------------------
 *
 * $Log: edwopen.c,v $
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

static char rcsid[]="$Id: edwopen.c,v 1.1 1993/04/06 14:14:07 sev Exp $";


#include "vced.h"

VOID edwopen(vced)
VCED *vced;                         /* Pointer to edit structure            */
{                                   /* ------------------------------------ */
  if((vced->edwptr = wxxopen(vced->edurow,vced->educol,/* Open wdo          */
  vced->edlrow,vced->edlcol,vced->edtitle,
      vced->edctrl,0,0,0,32)) == (WPTR)0)
    askmsg("Editor Message","Error Opening Window",0);/* Inform             */
}                                   /* ------------------------------------ */
