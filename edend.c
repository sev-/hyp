/*
 *  $Id: edend.c,v 1.1 1993/04/06 14:14:07 sev Exp $
 *
 * ---------------------------------------------------------- 
 *
 * $Log: edend.c,v $
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

static char rcsid[]="$Id: edend.c,v 1.1 1993/04/06 14:14:07 sev Exp $";


#include "vced.h"

COUNT edend(vced)
VCED *vced;                         /* Pointer to edit structure            */
    {                               /* ------------------------------------ */
    COUNT retval;                   /* Return value                         */
    if(vced->edkey[0] != vced->edkey[1])        /* Last key not same        */
	retval = vcedeol(vced);                 /* Call end line            */
    else if(vced->edkey[0] == vced->edkey[2])   /* Last two key ==          */
	 {
       vcedtol(vced);
	     retval = vcedeof(vced);            /* Goto end of file         */
	 }
    else  
	{

      vcedtol(vced);
	    retval = vcedeos(vced);             /* Goto bottom of screen    */
	}
    return(retval);                             /* Return status            */
    }                                           /* ------------------------ */
