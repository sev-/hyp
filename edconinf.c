/*
 *  $Id: edconinf.c,v 1.2 1993/04/08 10:37:38 sev Exp $
 *
 * ---------------------------------------------------------- 
 *
 * $Log: edconinf.c,v $
 * Revision 1.2  1993/04/08 10:37:38  sev
 * edsetfta changed
 *
 * Revision 1.1  1993/04/06  14:14:07  sev
 * Initial revision
 *
 * Revision 1.3  1993/04/05  16:01:33  kas
 * *** empty log message ***
 *
 * Revision 1.2  1993/04/01  14:10:29  kas
 * *** empty log message ***
 *
 * Revision 1.1  1993/03/12  15:44:23  sev
 * Init
 *
 *
 */

static char rcsid[]="$Id: edconinf.c,v 1.2 1993/04/08 10:37:38 sev Exp $";


#include "vced.h"

/****************************************************************************/
/*                       Информации о ключевом понятии (поле)               */
/****************************************************************************/

COUNT edinf(vced)
VCED *vced;
{
      vced->edkey[0]=INF_CONC;
      return(0);
}

/* ------------------------------------------------------------------------ */

COUNT ed_con_inf(vced)
VCED *vced;
{
  VCEDLINE *cline;
  COUNT col_str;
  COUNT i=0;
  COUNT flag_is=0;
  TEXT	*start_conc;
  TEXT *ch;
  TEXT	*strstr(),*strchr(),*strnvh();
  TEXT buf[30];
  TEXT mesg_plase[45];
  TEXT mesg_seg[90];
  VOID edsetfta();
  SELSET *tmp,*sel,*getselitm();
  WPTR w;
  int paste=1;
  int more=1;

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
    strcpy(vcedfbuf,cline->ltext);
    if(from[i]<10 && to[i]>9 )
      sprintf(buf,"\033<0%1d-%2d",from[i],to[i]);
    if( from[i]<10 && to[i]<10 )
      sprintf(buf,"\033<0%1d-0%1d",from[i],to[i]);
    if(from[i]>9 && to[i]<10)
      sprintf(buf,"\033<%2d-0%1d",from[i],to[i]);
    if(from[i]>9 && to[i]>9)
      sprintf(buf,"\033<%2d-%2d",from[i],to[i]);

    tmp = dirr;

    start_conc=strstr(vcedfbuf,buf);
    ch=strchr(start_conc,'>');
    *ch='\0';

    strcpy(buf,start_conc+8);
    ch=strchr(buf,';');
    *ch='\0';

    sprintf(mesg_plase,"Поле расположено  с %d по %d колонку.",from[i],to[i]);

    if( (sel=getselitm(tmp,buf,buf))!=(SELSET *)0)
	sprintf(mesg_seg,"\"%s\"",sel->idisplay);
    else
    {
	strcpy(mesg_seg,"Поле не привязано.");
	paste=0;
    }
    w=wxxopen(8,3,16,70,NULLTEXT,ACTIVE+BORDER+BD2,0,0,0,32);

    atsay(1,1,mesg_plase);
    atsay(2,1,"Пoле привязанно к сегменту:");
    atsay(3,1,mesg_seg);
    if(stat_hyp==PRIV_SEGWDO)
    {
	 atsay(4,1,"<ВК> - привязать.");
	 atsay(5,1,"<АР2> - выход. <ПФ4> - отмена привязки.");
    }
    else
      atsay(5,1,"<АР2> - Выход.");
    while(more)
    {
	switch(getone())
	{
	 case RET:
	if(stat_hyp==PRIV_SEGWDO && paste==1)
	{
	       strcpy(privyz.mesg,sel->idisplay);
	       strcpy(privyz.name_seg,sel->ivalue);
	       strcpy(privyz.name_file,strchr(privyz.mesg,'(')+1);
	       *(strchr(privyz.name_file,')'))='\0';
	       strcpy(vceddfil,privyz.file_par);
	       stat_exec=PRIV_YES;
	       vced->edmail=VCEDNEWFIL;
	       more=0;
	}
	break;
	 case ESC:
	    more=0;
	    break;
	 case PF4:
	    if(stat_hyp==PRIV_SEGWDO)
	    {
	       stat_exec=PRIV_NO;
	       strcpy(vceddfil,privyz.file_par);
	       vced->edmail=VCEDNEWFIL;
	   }
	   more=0;
	   break;

	 default:
	     break;
	   }
    }
    wclose(w);

  }
  else
    ask_msg("Установите курсор на поле.",0);
}

/* ------------------------------------------------------------------------ */
