/*
 *  $Id: eddelcon.c,v 1.1 1993/04/06 14:14:07 sev Exp $
 *
 * ---------------------------------------------------------- 
 *
 * $Log: eddelcon.c,v $
 * Revision 1.1  1993/04/06 14:14:07  sev
 * Initial revision
 *
 * Revision 1.2  1993/03/25  12:22:15  kas
 * *** empty log message ***
 *
 * Revision 1.1  1993/03/12  15:44:23  sev
 * Init
 *
 *
 */

static char rcsid[]="$Id: eddelcon.c,v 1.1 1993/04/06 14:14:07 sev Exp $";

#include "vced.h"

/*************************************************************************/
/*	      Функция отмены ключевого понятия				 */
/*************************************************************************/
/*  int flag_helpline;          */

COUNT ed_del_conc(vced)
VCED *vced;
{
  DBDP line;
  VCEDLINE *cline;
  COUNT col_str;
  COUNT i=0;
  COUNT flag_is=0;		     /* флаг того,что поле существует	*/
  TEXT	*ptr;
  TEXT *ch;
  TEXT	*start_conc,*end_conc;	     /* начало и конец поля  в строке(*)    */
  COUNT start_,end_;		     /* начало и конец поля  в строке(i)    */
  TEXT	*strstr(),*strrchr(),buf[10];

  if(vced->edcline == (DBDP)0)	      /* If not on real line	  */
     vcedmkreal(vced);		      /* Make the current loc real*/
  line=(vced->edmail==VCEDNEWFIL)?privyz.line:vced->edcline;
  col_str=(vced->edmail==VCEDNEWFIL)?privyz.cchar:vced->edcchar;
  cline=vcedmline(vced->edbuffer,line);
  edsetfta(cline,from,to,stat_);

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
    vcedempty(vcedfbuf,MAXLIN);
    strncpy(vcedfbuf,cline->ltext,cline->lused);
    vcedempty(cline->ltext,cline->lused);
    if(from[i]<10 && to[i]>9 )
      sprintf(buf,"\033<0%1d-%2d",from[i],to[i]);
    if( from[i]<10 && to[i]<10 )
      sprintf(buf,"\033<0%1d-0%1d",from[i],to[i]);
    if(from[i]>9 && to[i]<10)
      sprintf(buf,"\033<%2d-0%1d",from[i],to[i]);
    if(from[i]>9 && to[i]>9)
      sprintf(buf,"\033<%2d-%2d",from[i],to[i]);

    start_conc=strstr(vcedfbuf,buf);
    end_conc=strstr(start_conc+1,">");

    start_=strlen(vcedfbuf)-strlen(start_conc);
    end_=strlen(vcedfbuf)-strlen(end_conc);
    ptr=cline->ltext;
    for(i=0;i <= cline->lused;i++)
    {
      if(i < start_ || i > end_)
      {
	*ptr=vcedfbuf[i];
	ptr++;
      }
    }
    ptr=cline->ltext;
    trim(ptr);
    if(strstr(cline->ltext,"\033")==0)
    {
      while((ch=strrchr(ptr,' '))!=0)
      {
	if(*(ch+1)=='\0'|| *(ch+1)=='\n')
	  *ch='\0';
	else
	  break;
      }
    }
    cline->lused=strlen(cline->ltext);
    vcedrline(vced->edbuffer,cline,line);
    vced->edupval = VCEDUPDLINE;
    if(vced->edkey[0]==DEL_POLE)
      watsay(vced->edswptr,0,3," Отмена отметки");
    vced->edsline=1;
  }
  else
    ask_msg("Для удаления установите курсор на поле.",0);

}
