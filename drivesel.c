/*
 *  $Id: drivesel.c,v 1.1 1993/09/14 16:45:50 sev Exp $
 *
 * ---------------------------------------------------------- 
 *
 * $Log: drivesel.c,v $
 * Revision 1.1  1993/09/14 16:45:50  sev
 * Initial revision
 *
 *
 */

#include "shell.h"
 
#define PGSIZE	15

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
    return(ESC);

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

      if( cnt > 1 )
      {
	selbar(w1,cnt-1,0,lo,width);
	cnt--;
	picked = picked->iprev;
      }
      break;

    case CUR_DOWN:

      if( cnt < nbr )
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

      if( cnt != nbr )
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

      if( (cnt != 1) && (cnt != 0) )
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

      if( cnt!=1 )
      {
	selbar(w1,cnt-1,0,lo,width);
	picked = head__;
	cnt=1;
      }
      break;

    case END_KEY:

      if( cnt!=nbr )
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
