/*
 *  $Id: eddelendl.c,v 1.1 1993/04/06 14:14:07 sev Exp $
 *
 * ---------------------------------------------------------- 
 *
 * $Log: eddelendl.c,v $
 * Revision 1.1  1993/04/06 14:14:07  sev
 * Initial revision
 *
 * Revision 1.1  1993/03/12  15:44:23  sev
 * Init
 *
 *
 */

static char rcsid[]="$Id: eddelendl.c,v 1.1 1993/04/06 14:14:07 sev Exp $";


#include "vced.h"

/*************************************************************************/
/*	      Функция удаления от курсора до конца строки		 */
/*************************************************************************/

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
