/*
 *  $Id: edpaste.c,v 1.2 1993/04/08 10:37:38 sev Exp $
 *
 * ---------------------------------------------------------- 
 *
 * $Log: edpaste.c,v $
 * Revision 1.2  1993/04/08 10:37:38  sev
 * *** empty log message ***
 *
 * Revision 1.1  1993/04/06  14:14:07  sev
 * Initial revision
 *
 * Revision 1.3  1993/03/25  12:22:15  kas
 * *** empty log message ***
 *
 * Revision 1.2  1993/03/14  11:39:19  sev
 * Исправлена ошибка в отметке поля. Полностью работающая версия
 * В файле vcопределение DEFSEG - добавление окна сегментов
 *
 * Revision 1.1  1993/03/12  15:44:23  sev
 * Init
 *
 *
 */

static char rcsid[]="$Id: edpaste.c,v 1.2 1993/04/08 10:37:38 sev Exp $";


#define WIND 1
#define DIR_  0

#include "vced.h"

/* int flag_helpline;           */

COUNT edpaste(vced)
VCED *vced;
{
  int ret;
  VCEDLINE *cline;
  int col_str;
  int i=0;
  int flag_is=0;
  char *strchr();

  if(stat_hyp==PRIV_SEGWDO)
    return(0);
  if(vced->edcline == (DBDP)0)	      /* If not on real line	  */
     vcedmkreal(vced);		      /* Make the current loc real*/
  cline=vcedmline(vced->edbuffer,vced->edcline);
  edsetfta(vced, vced->edcline, cline,from,to,stat_);
  col_str=vced->edcchar;

  while( from[i]!=-1)
  {
    if(col_str >= from[i] && col_str <= to[i] )
    {
      flag_is=1;
      break;
    }
    i++;
  }
  if(flag_is==1)
  {
    ret=ask_msg("Место привязки",2);
    switch(ret)
    {
    case   WIND:

      stat_hyp=PRIV_SEGWDO;
      strcpy(privyz.file_par,vced->edbuffer->bfname);
      privyz.line=vced->edcline;
      privyz.cchar=vced->edcchar;
      watsay(vced->edswptr,0,1,"    <УПР-w> - привязка из окна сегментов.  <УПР-o> - привязка от поля.		  ");
      vced->edsline=2;
      break;
    case   DIR_:

      if(eddir(vced)==0)
	ask_msg("Привязка не произведена.",0);
      else
      {
	ed_del_conc(vced);
	ed_ins_conc(vced,vced->edcline,from[i],to[i],privyz.name_seg,privyz.name_file,1);
	ask_msg("Привязка произведена успешно.",0);
      }
      break;
    default:
      stat_hyp=ED;
      break;

    }
  }
  else
    ask_msg("Установите курсор на поле.",0);
  return GOOD;
}

COUNT privmsg(vced)
VCED *vced;
{
  VCEDLINE *cline;
  int col_str;
  int i=0;
  char *strchr();

  if(stat_exec==PRIV_NO)
  {
    ask_msg("Привязка не произведена.",0);
    stat_hyp=ED;
    vced->edsline=1;
    return(0);
  }

  cline=vcedmline(vced->edbuffer,privyz.line);
  edsetfta(vced, privyz.line, cline,from,to,stat_);
  col_str=privyz.cchar;

  while( from[i]!=-1)
  {
    if(col_str >= from[i] && col_str <= to[i] )
      break;
    i++;
  }

  if(stat_exec==PRIV_YES)
  {
    ed_del_conc(vced);
    ed_ins_conc(vced,privyz.line,from[i],to[i],privyz.name_seg,privyz.name_file,1);
    ask_msg("Привязка произведена успешно.",0);
  }
  vced->edsline=1;
  stat_hyp=ED;
  return GOOD;
}
