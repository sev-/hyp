/*
 *  $Id: edmarkpg.c,v 1.1 1993/04/06 14:14:07 sev Exp $
 *
 * ---------------------------------------------------------- 
 *
 * $Log: edmarkpg.c,v $
 * Revision 1.1  1993/04/06 14:14:07  sev
 * Initial revision
 *
 * Revision 1.2  1993/03/17  15:53:58  sev
 * Проблемы с отметкой страницы. Виноват размер буфера. Ох и достал
 * он меня. Пока нашел, перерыл все исходники.
 *
 * Revision 1.1  1993/03/12  15:44:23  sev
 * Init
 *
 *
 */

static char rcsid[]="$Id: edmarkpg.c,v 1.1 1993/04/06 14:14:07 sev Exp $";


#include "vced.h"

/*************************************************************************/
/*	      Функция отметки конца страницы				*/
/*************************************************************************/

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
