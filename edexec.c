/*
 *  $Id: edexec.c,v 1.1 1993/04/06 14:14:07 sev Exp $
 *
 * ---------------------------------------------------------- 
 *
 * $Log: edexec.c,v $
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

static char rcsid[]="$Id: edexec.c,v 1.1 1993/04/06 14:14:07 sev Exp $";


#include "vced.h"

/**********************************************************************/
/*   Функция выполняет комманду  в зависимости от нажатой клавиши     */
/**********************************************************************/

COUNT edexec(vced)
VCED *vced;		    /* Pointer to edit structure	    */
{                           /* ------------------------------------ */
  PFI command = NULLFUNC;   /* Command to execute                   */

  if(vcedisch(vced->edkey[0]))          /* If valid printable char  */
  {                                     /*                          */
    if(vced->edmode & VCEDOVER)         /* Over write mode ?        */
      vcedover(vced);                   /* Over write the character */
    else                                /* Else                     */
      vcedinsert(vced);                 /* Insert the character     */
  }                                     /*                          */
  else if((command = edfndcmd(vced)) != NULLFUNC)     /* Find cmd   */
    (*command)(vced);                   /* Execute command          */
  else                                  /*                          */
    bell();                             /* Inform user              */
  return(0);                            /* Return O.K.              */
}                                       /* ------------------------ */

COUNT vcedisch(key)
COUNT key;
{
  COUNT retval = TRUE;
  if((key < 32) || (key > 255 ))
    retval = FALSE;
  return(retval);
}
