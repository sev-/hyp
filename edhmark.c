/*
 *  $Id: edhmark.c,v 1.1 1993/04/06 14:14:07 sev Exp $
 *
 * ---------------------------------------------------------- 
 *
 * $Log: edhmark.c,v $
 * Revision 1.1  1993/04/06 14:14:07  sev
 * Initial revision
 *
 * Revision 1.4  1993/04/06  13:40:54  kas
 * *** empty log message ***
 *
 * Revision 1.3  1993/03/27  12:08:16  kas
 * *** empty log message ***
 *
 * Revision 1.2  1993/03/14  11:04:07  sev
 * Исправлена ошибка в отметке поля
 *
 * Revision 1.1  1993/03/12  15:44:23  sev
 * Init
 *
 *
 */

static char rcsid[]="$Id: edhmark.c,v 1.1 1993/04/06 14:14:07 sev Exp $";

#include "vced.h"

/****************************************************************************/
/*                      Функция  отметки  ключевого понятия                 */
/****************************************************************************/

COUNT edhmark(vced)
VCED *vced;
{
  vced->edkey[0]=POLE;
  return(0);
}

/* ------------------------------------------------------------------------ */

COUNT ed_horiz_mark(vced)
VCED *vced;
{
  WPTR wdo1,w;
  int key;
  int from_ = -1,to_ = -1;
  int from1[50],to1[50];
  int col=0;	    /* текущие стр. и позиция	*/
  int more=1;
  int flag_str = -1;	     /* флаг отметки ключевого слова	   */
  int c_start;		    /* координаты ключевого слова.	   */
  int col_str=1;	      /* длина клачевого слова		     */
  int retval=1; 	    /* ret 1 если отмечена ,0 если нет и -1 ESC*/
  VCEDLINE *cline;
  int block=0;
  int in_corr;

  wdo1=vced->edwptr;
  w=vced->edswptr;
  if(vced->edcline == (DBDP)0)	      /* If not on real line	  */
     vcedmkreal(vced);		      /* Make the current loc real*/
  cline=vcedgline(vced->edbuffer,vced->edcline);
  more=1;
  ungetone(POLE);
  while(more)
  {
    edsetfta(cline,from1,to1,stat_);
    in_corr=(col_str >= 1)?to_+1:from_-1;

    if(correct(in_corr,from1,to1)==0 && block !=1 )
    {
      block=1;
      if(col_str>1)
	blockkey(CUR_RIGHT);
      else
	blockkey(CUR_LEFT);
    }
    else if(block==1)
    {
      unblkkey(CUR_RIGHT);
      unblkkey(CUR_LEFT);
      block=0;
    }
    if(from_ != -1)
      inc_from_to(from1,to1,stat_,from_,to_);
    if(vced->edupval & VCEDUPDALL   )
      edprtbuf(vced);
    edprtline(vced,vced->edcline,from1,to1,stat_);
    vced->edupval=0;
    vcedstatus(vced);
    vcedaddcur(vced);
    key=getone();
    switch(key)
    {
    case CUR_RIGHT:
      if(!edmvright(vced))
	break;
      col=vced->edlmar+vced->edcchar;
      if(flag_str==1)
      {
	if(col_str>=1)
	  to_=col;
	else
	  from_=col;
	col_str++;
      }
      break;

    case CUR_LEFT:
      if(!edmvleft(vced))
	break;
      col=vced->edlmar+vced->edcchar;
      if(flag_str==1)
      {
	if(col_str<=1)
	  from_=col;
	else
	  to_=col;
	col_str--;
      }
      break;
    case POLE:
      edsetfta(cline,from1,to1,stat_);
      flag_str=(flag_str==0 || flag_str== -1)?1:0;
      if(flag_str)
      {
	c_start=vced->edlmar+vced->edcchar;
	from_=to_=c_start;
	if(correct(c_start,from1,to1)==0)
	{
	  bell();
	  unblkkey(CUR_LEFT);
	  ask_msg("Пересекающиеся поля !!",0);
	  vced->edupval |= VCEDUPDLINE;
	  more=0;
	  break;
	}
	watsay(w,0,3,"Oтметка поля	 ");
	edprtline(vced,vced->edcline,from1,to1,stat_);
	col_str=1;
	break;
      }
      if(block==1)
      {
	unblkkey(CUR_RIGHT);
	unblkkey(CUR_LEFT);
      }
      watsay(w,0,3,"Поле отмечено	 ");
      ed_ins_conc(vced,vced->edcline,from_,to_,NULLTEXT,NULLTEXT,0);
      more=0;
      vced->edupval = VCEDUPDLINE;
      vced->edsline = 1;
      break;

    case DEL_POLE:

      if(flag_str== -1)
	break;
      flag_str = -1;
      from_ = -1;
      to_=0;
      edprtline(vced,vced->edcline,from1,to1,stat_);
      watsay(w,0,3,"Отмена поля      ");
      vced->edupval |= VCEDUPDLINE;
      vced->edsline = 1;
      more=0;
      retval=0;
      break;

    case ESC:
      if(block==1)
      {
	unblkkey(CUR_RIGHT);
	unblkkey(CUR_LEFT);
      }
      more=0;
      vced->edupval = VCEDUPDLINE;
      vced->edsline = 1;
      retval = -1;
      break;
    }
  }
  wselect(wdo1);
  won();
  return(retval);
}

/* ------------------------------------------------------------------------ */

int inc_from_to(from1,to1,stat1_,from_,to_)
int stat1_[];
int from1[];
int to1[];
int from_,to_;
{
  int l=0,i=0;
  int mem_from,mem_to,mem_stat;
  int wr_from,wr_to,wr_stat;
  int find = -1;
  int len;

  if(from1[0] == -1)
  {
    from1[0]=from_;
    to1[0]=to_;
    stat1_[0]=0;
    return(1);
  }
  while(from1[l] != -1)
  {
    if( from_ > to1[l] )
      if( to_ ||  from1[l+1] )
	find=l;
    l++;
  }
  len=l+1;
  for(i=find+1;i<=len+1;i++)
  {
    mem_from=from1[i];
    mem_to=to1[i];
    mem_stat=stat1_[i];
    if( i==find+1)
    {
      from1[i]=from_;
      to1[i]=to_;
      stat1_[i]=0;
    }
    else
    {
      from1[i]=wr_from;
      to1[i]=wr_to;
      stat1_[i]=wr_stat;
    }
    wr_from=mem_from;
    wr_to=mem_to;
    wr_stat=mem_stat;
  }
  return(1);
}

/* ------------------------------------------------------------------------ */

int correct(start,from1,to1)
int start;
int from1[];
int to1[];
{
  int i=0;

  if(start== -1)
    return(-1);

  if(from1[0]== -1)
    return(1);

  if(start < from1[0])
    return(1);
  while( from1[i]  != -1 )
  {
    if((start > to1[i]) && ( (start < from1[i+1] )||(from1[i+1] == -1)) )
      return(1);
    i++;
  }
  return(0);
}

/* ------------------------------------------------------------------------ */
