/*
 *  $Id: edhelp.c,v 1.1 1993/04/06 14:14:07 sev Exp $
 *
 * ---------------------------------------------------------- 
 *
 * $Log: edhelp.c,v $
 * Revision 1.1  1993/04/06 14:14:07  sev
 * Initial revision
 *
 * Revision 1.4  1993/03/23  13:10:22  kas
 * *** empty log message ***
 *
 * Revision 1.3  1993/03/15  10:55:59  sev
 * Была опечатка в справке
 *
 * Revision 1.2  1993/03/14  18:36:54  sev
 * За ней были замечены странные вещи. Исправлено
 *
 * Revision 1.1  1993/03/12  15:44:23  sev
 * Init
 *
 *
 */

static char rcsid[]="$Id: edhelp.c,v 1.1 1993/04/06 14:14:07 sev Exp $";

#include "vced.h"

COUNT edhelp(vced)
VCED *vced;
{
  WPTR whlp, wopen();

  whlp = wopen(10,10,16,60," H E L P ");

  getone();
  wclose(whlp);
  return(0);
}
