/*
 *  $Id: edmarkpg.c,v 1.2 1993/04/20 16:04:12 sev Exp $
 *
 * ---------------------------------------------------------------------------
 *
 * $Log: edmarkpg.c,v $
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

/****************************************************************************/
/*                Функция отметки конца страницы                            */
/****************************************************************************/

COUNT edmarkpg(vced)
VCED *vced;
{
  VCEDLINE *cline;
  TEXT pg_str[MAXLINESIZE];

  vcedmvup(vced);
  if(vced->edcline == (DBDP)0)	      /* If not on real line	  */
    vcedmkreal(vced);		     /* Make the current loc real*/
  cline=vcedmline(vced->edbuffer,vced->edcline);
  vcedempty(pg_str,vcedval.linlen);
  strcpy(pg_str,"\014________");
  *(&pg_str[0]+vced->edbuffer->bflinelen-1) = vcedval.hardret;
  vcediline(vced->edbuffer,vced->edcline,cline->lnext,pg_str,strlen(pg_str));
  vcedmvdown(vced);
  vced->edupval = VCEDUPDALL;
}
