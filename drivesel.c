/*
 *  $Id: drivesel.c,v 1.3 1993/04/21 14:54:27 sev Exp $
 *
 * ---------------------------------------------------------------------------
 *
 * $Log: drivesel.c,v $
 * Revision 1.3  1993/04/21 14:54:27  sev
 * Есть клавиша УТ
 *
 * Revision 1.2  1993/04/20  16:04:12  sev
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

#define PGSIZE	15

#include "vced.h"

/* ------------------ Display and selection from SELSET ------------------- */

int   drivesel(lst,disp,val,header)
SELSET	 *lst;
TEXT *disp;
TEXT *val;
TEXT *header;
{
  SELSET *itmptr,*head__,*tail;
  SELSET *picked;
  COUNT hi,lo,pg;
  COUNT more=1;
  COUNT n,i=0;
  COUNT width=0;
  COUNT nbr=0;
  COUNT vert=0;
  COUNT bottom;
  COUNT keypress_;
  COUNT cnt=1;
  WPTR w1;

  itmptr=lst->inext;

  if( itmptr == (SELSET *)0 )
  {
    ask_msg("Справочник пуст.",0);
    return(ESC);
  }
  width=strlen(header)+2;
  while(itmptr)
  {
    width=(strlen(itmptr->idisplay)>width)?strlen(itmptr->idisplay):width;
    itmptr=itmptr->inext;
    nbr++;
  }
  bottom=min(nbr,PGSIZE);
  vert=(nbr>PGSIZE)?nbr:0;
  width+=5;
  w1=wxxopen(2,2,3+bottom,width+3,header,
	     BORDER+BD2+SCROLL+SHADOW,vert,0,0,32);

  tail=head__=itmptr=lst->inext;
  whide(w1);
  while( itmptr )
  {
    tail=itmptr;
    atsay(i,0,itmptr->idisplay);
    i++;
    itmptr=itmptr->inext;
  }
  wat(w1,0,0);
  wshow(w1);
  hi=ATR_B;
  lo=ATR_F;
  pg=wrows(w1);
  i=cnt;
  picked=head__;

  while( more )
  {
    if(cnt)
      selbar(w1,cnt-1,0,hi,width);
    keypress_=getone();

    switch(keypress_)
    {
    case CUR_UP:

      if( cnt-1 < 1 )
	bell();
      else
      {
	selbar(w1,cnt-1,0,lo,width);
	cnt--;
	picked = picked->iprev;
      }
      break;

    case CUR_DOWN:

      if( cnt+1 > nbr )
	bell();
      else
      {
	selbar(w1,cnt-1,0,lo,width);
	cnt++;

	if(cnt == 1)
	  picked = head__;
	else
	  picked = picked->inext;

      }
      break;

    case PGDN:

      if( cnt == nbr )
	bell();
      else
      {
	selbar(w1,cnt-1,0,lo,width);
	n=pg;
	while( n && cnt < nbr )
	{
	  n--;
	  cnt++;
	  if(cnt == 1)
	    picked = head__;
	  else
	    picked = picked->inext;
	}
      }
      break;

    case PGUP:

      if( (cnt == 1) || (cnt == 0) )
	bell();
      else
      {
	selbar(w1,cnt-1,0,lo,width);
	n=pg;
	while( n && cnt > 1 )
	{
	  n--;
	  picked = picked->iprev;
	  cnt--;
	}
      }
      break;

    case HOME_KEY:

      if( cnt==1 )
	bell();
      else
      {
	selbar(w1,cnt-1,0,lo,width);
	picked = head__;
	cnt=1;
      }
      break;

    case END_KEY:

      if( cnt==nbr )
	bell();
      else
      {
	selbar(w1,cnt-1,0,lo,width);
	picked = tail;
	cnt=nbr;
      }
      break;

    case ESC:
      more = 0;
      break;

    case RET:

      more = 0;
      break;

    case DEL_KEY:

      more = 0;
      break;

    default:
      break;

    }
  }
  strcpy(disp,picked->idisplay);
  strcpy(val,picked->ivalue);
  wclose(w1);
  return(keypress_);
}
