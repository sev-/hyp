/*
 *  $Id: edend.c,v 1.2 1993/04/20 16:04:12 sev Exp $
 *
 * ---------------------------------------------------------------------------
 *
 * $Log: edend.c,v $
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
