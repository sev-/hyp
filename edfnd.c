/*
 *  $Id: edfnd.c,v 1.2 1993/04/20 16:04:12 sev Exp $
 *
 * ---------------------------------------------------------------------------
 *
 * $Log: edfnd.c,v $
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

/****************************************************************************/
/*          Функция находит команду  в структуре по клавише                 */
/****************************************************************************/

PFI edfndcmd(vced)
VCED *vced;                         /* Pointer to edit structure            */
{                                   /* ------------------------------------ */
      VCEDCMD *curcmd;              /* Current command structure            */
      PFI retval = NULLFUNC;        /* Return value                         */
  
      curcmd = &vcedcmds[0];                    /* Start at beginning       */
      while(curcmd->cmdname != NULLTEXT)        /* While more commands      */
    {                                           /*                          */
	if(vced->edkey[0] == curcmd->cmdkey)    /* Is this the key?         */
	 {                                      /*                          */
	  retval = curcmd->cmdloc;              /* Assign function address  */
	  break;                                /* and return               */
	  }                                     /*                          */
      curcmd++;                                 /* Point to next member     */
      }                                         /*                          */
	return(retval);                         /* Return function address  */
}                                               /* ------------------------ */
