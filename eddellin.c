/*
 *  $Id: eddellin.c,v 1.1 1993/04/06 14:14:07 sev Exp $
 *
 * ---------------------------------------------------------- 
 *
 * $Log: eddellin.c,v $
 * Revision 1.1  1993/04/06 14:14:07  sev
 * Initial revision
 *
 * Revision 1.1  1993/03/12  15:44:23  sev
 * Init
 *
 *
 */

static char rcsid[]="$Id: eddellin.c,v 1.1 1993/04/06 14:14:07 sev Exp $";


#include "vced.h"

/*************************************************************************/
/*	      Функция удаления строки					*/
/*************************************************************************/

COUNT eddelline(vced)
VCED *vced;
{
  VCEDLINE *cline;

  if(vced->edcline == (DBDP)0)	      /* If not on real line	  */
  {
    bell();
    return(0);
  }
  cline=vcedgline(vced->edbuffer,vced->edcline);
  vceddline(vced->edbuffer,cline->lprev,vced->edcline,cline->lnext);
  if(vced->edcline==vced->edtline)
    vced->edtline=cline->lnext;
  vced->edcline=cline->lnext;
  vced->edupval = VCEDUPDALL;
  return 0;
}
