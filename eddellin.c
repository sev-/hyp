/*
 *  $Id: eddellin.c,v 1.2 1993/04/20 16:04:12 sev Exp $
 *
 * ---------------------------------------------------------------------------
 *
 * $Log: eddellin.c,v $
 * Revision 1.2  1993/04/20 16:04:12  sev
 * *** empty log message ***
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
/*                Функция удаления строки                                   */
/****************************************************************************/

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
