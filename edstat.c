/*
 *  $Id: edstat.c,v 1.1 1993/04/06 14:14:07 sev Exp $
 *
 * ---------------------------------------------------------------------------
 *
 * $Log: edstat.c,v $
 * Revision 1.1  1993/04/06 14:14:07  sev
 * Initial revision
 *
 * Revision 1.3  1993/04/05  16:06:22  kas
 * Добавлен номер строки текущей страницы
 *
 * Revision 1.1  1993/04/03  14:31:33  sev
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

static char rcsid[]="$Id: edstat.c,v 1.1 1993/04/06 14:14:07 sev Exp $";

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
    werase(vced->edswptr,ATR_B);
    vced->edsline = 0;              /* Clear flag                           */
  }                                 /*                                      */
  mode = (vced->edmode & VCEDOVER ? 1 : 0); /* Determine ins mode           */

  if(vced->edbuffer->bfchange)      /* If buffer changed                    */
     strcpy(modified,"*");          /* Display changed                      */
  else
     strcpy(modified," ");          /* Not changed                          */

  sprintf(status,"F1 - Help       ======> %2d           Строка:%4d  Колонка:%3d     %s %s",
		  numlinpage(vced),(COUNT)vced->edcrow+1,
		 (COUNT)vced->edcchar+1,vcedmodes[mode],modified);

  watsay(vced->edswptr,0, 0,status);  /* Display on status line             */
}                                   /* ------------------------------------ */
