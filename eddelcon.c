/*
 *  $Id: eddelcon.c,v 1.4 1993/04/22 13:23:26 sev Exp $
 *
 * ---------------------------------------------------------------------------
 *
 * $Log: eddelcon.c,v $
 * Revision 1.4  1993/04/22 13:23:26  sev
 * dir записывается один раз
 * \
 *
 * Revision 1.3  1993/04/20  16:04:12  sev
 * *** empty log message ***
 *
 * Revision 1.4  1993/04/15  15:05:51  kas
 * *** empty log message ***
 *
 * Revision 1.4  1993/04/15  15:05:51  kas
 * *** empty log message ***
 *
 * Revision 1.3  1993/04/15  07:21:41  kas
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
/*               Функция отмены ключевого понятия                           */
/****************************************************************************/

COUNT ed_del_conc(vced)
VCED *vced;
{
	DBDP line;
	VCEDLINE *cline;
	COUNT col_str;
	COUNT i=0;
	COUNT flag_is=0;                 /* флаг того,что поле существует   */
	TEXT	*ptr;
	TEXT *ch;
	TEXT    *start_conc,*end_conc;  /* начало и конец поля  в строке(*) */
	COUNT start_,end_;              /* начало и конец поля  в строке(i) */
	TEXT	*strstr(),*strrchr(),buf[10];

	if(vced->edcline == (DBDP)0)	      /* If not on real line	  */
		vcedmkreal(vced);             /* Make the current loc real*/
	line=(vced->edmail==VCEDNEWFIL)?privyz.line:vced->edcline;
	col_str=(vced->edmail==VCEDNEWFIL)?privyz.cchar:vced->edcchar;
	cline=vcedmline(vced->edbuffer,line);
	edsetfta(vced, line, cline,from,to,stat_);

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
		ptr=strchr(cline->ltext, vcedval.softret);
		
		trimstring(ptr);
		ptr = ptr + strlen(ptr);
		*ptr++ = vcedval.softret;
		*ptr = 0;
/*
    if(strstr(cline->ltext,"\033")==0)
    {
      while(ch=strrchr(ptr,' '))!=0)
      {
	if(*(ch+1)=='\0'|| *(ch+1)=='\n')
	  *ch='\0';
	else
	  break;
      }
    }
*/
		strcpy(buf,start_conc+8);
		ch=strchr(buf,';');
		*ch='\0';
		add_refer(buf, -1);

		cline->lused=strlen(cline->ltext);
		vcedrline(vced->edbuffer,cline,line);
		vced->edupval = VCEDUPDLINE;
		vced->edsline=1;
	}
	else
		ask_msg("Для удаления установите курсор на поле.",0);

}
