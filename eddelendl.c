/*
 *  $Id: eddelendl.c,v 1.2 1993/04/20 16:04:12 sev Exp $
 *
 * ---------------------------------------------------------------------------
 *
 * $Log: eddelendl.c,v $
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
/*               Функция удаления от курсора до конца строки                */
/****************************************************************************/

COUNT eddelendl(vced)
VCED *vced;
{
  VCEDLINE *cline;
  TEXT *strstr();

  if(vced->edcline == (DBDP)0)	      /* If not on real line	  */
  {
    bell();
    return(0);
  }
  cline=vcedmline(vced->edbuffer,vced->edcline);
 
  vcedempty(cline->ltext+vced->edcchar,cline->lused-vced->edcchar+1);
  *(&cline->ltext[0]+vced->edbuffer->bflinelen-1)=vcedval.hardret;
  cline->lused=strlen(cline->ltext);
  vcedrline(vced->edbuffer,cline,vced->edcline);
  vced->edupval = VCEDUPDLINE;
  return 0;
}
