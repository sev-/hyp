/*
 *  $Id: edstat.c,v 1.2 1993/04/20 16:04:12 sev Exp $
 *
 * ---------------------------------------------------------------------------
 *
 * $Log: edstat.c,v $
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

static TEXT *vcedmodes[] =
{
  (TEXT *)"Вставка",
  (TEXT *)"Замена "
};

COUNT vcedstatus(vced)
VCED *vced;                         /* Pointer to edit structure            */
{                                   /* ------------------------------------ */
  COUNT mode;                       /* Value of insert or overwrite         */
  TEXT modified[2];                 /* Is buffer Modified                   */
  TEXT status[80];                  /* Status message                       */

  if(vced->edswptr == (WPTR)0)      /* If window not open                   */
    return(0);                      /* Return                               */

  if(vced->edsline==2)
     return(0);
  if(vced->edsline ==1)             /* If status line need clr              */
  {                                 /*                                      */
    werase(vced->edswptr,ATR_F);
    vced->edsline = 0;              /* Clear flag                           */
  }                                 /*                                      */
  mode = (vced->edmode & VCEDOVER ? 1 : 0); /* Determine ins mode           */

  if(vced->edbuffer->bfchange)      /* If buffer changed                    */
     strcpy(modified,"*");          /* Display changed                      */
  else
     strcpy(modified," ");          /* Not changed                          */

  sprintf(status," F1 - Help         ======> %2d           Строка:%4d  Колонка:%3d     %s %s ",
		  numlinpage(vced),(COUNT)vced->edcrow+1,
		 (COUNT)vced->edcchar+1,vcedmodes[mode],modified);

  watsay(vced->edswptr,0, 0,status);  /* Display on status line             */
}                                   /* ------------------------------------ */
