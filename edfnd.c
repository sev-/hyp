/*
 *  $Id: edfnd.c,v 1.1 1993/04/06 14:14:07 sev Exp $
 *
 * ---------------------------------------------------------- 
 *
 * $Log: edfnd.c,v $
 * Revision 1.1  1993/04/06 14:14:07  sev
 * Initial revision
 *
 * Revision 1.2  1993/04/01  14:10:29  kas
 * *** empty log message ***
 *
 * Revision 1.1  1993/03/12  15:44:23  sev
 * Init
 *
 *
 */

static char rcsid[]="$Id: edfnd.c,v 1.1 1993/04/06 14:14:07 sev Exp $";


#include "vced.h"

/*********************************************************************/
/*   Функция находит команду  в структуре по клавише		     */
/*********************************************************************/


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
