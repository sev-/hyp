/*
 *  $Id: edinsline.c,v 1.1 1993/04/06 14:14:07 sev Exp $
 *
 * ---------------------------------------------------------- 
 *
 * $Log: edinsline.c,v $
 * Revision 1.1  1993/04/06 14:14:07  sev
 * Initial revision
 *
 * Revision 1.1  1993/03/12  15:44:23  sev
 * Init
 *
 *
 */

static char rcsid[]="$Id: edinsline.c,v 1.1 1993/04/06 14:14:07 sev Exp $";


#include "vced.h"

COUNT edinsline(vced)
VCED *vced;
{
	int cchar,offset;
        
  cchar=vced->edcchar;
  offset=vced->edoffset;

	vcedtol(vced);
	vcedxreturn(vced);
	vced->edcchar=cchar;
  vced->edoffset=offset;
}
