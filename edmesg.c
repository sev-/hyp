/*
 *  $Id: edmesg.c,v 1.2 1993/04/20 16:04:12 sev Exp $
 *
 * ---------------------------------------------------------------------------
 *
 * $Log: edmesg.c,v $
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

int ask_msg(msg,mode)
TEXT *msg;
int mode;
{
  WPTR w;
  int len_msg,width,row_u,key,more=1,y_n=0;

  len_msg=strlen(msg);
  if(len_msg<26)
    width=26+5;
  else if(len_msg>79)
    return(-1);
  else
    width=len_msg+5;

  row_u=(80-width)/2;
  w=wxxopen(10,row_u,15,row_u+width,NULL,ACTIVE+BORDER+BD1+SHADOW,
      0,0,0,32);
  xatsay(1,(width-len_msg)/2,msg,ATR_F);
  switch(mode)
  {
  case 0:		   /*	    любая клавиша   */
    xatsay(2,(width-23)/2," Нажмите любую клавишу.. ",ATR_F);
    getone();
    y_n=1;
    break;
  case 3:
  case 2:
  case 1:
    if(mode==1)
    {
      len_msg=14;
      xatsay(2,(width-len_msg)/2,"  НЕТ    ДА  ",ATR_F);
    }
    if(mode==2)
    {
      len_msg=26;
      xatsay(2,(width-len_msg)/2," Справочник   Окно/Поле  ",ATR_F);
    }
    if(mode==3)
    {
      len_msg=26;
      xatsay(2,(width-len_msg)/2," С записью    Без записи ",ATR_F);
      mode=2;
    }
    while(more)
    {
      selbar(w,2,(width-len_msg)/2+7*y_n*mode,ATR_B,6*mode);
      key=getone();
      switch(key)
      {
      case CUR_RIGHT:
	if(y_n==0)
	{
	  selbar(w,2,(width-len_msg)/2,ATR_F,6*mode);
	  y_n=1;
	}
	break;
      case CUR_LEFT:
	if(y_n==1)
	{
	  selbar(w,2,(width-len_msg)/2+7*mode,ATR_F,6*mode);
	  y_n=0;
	}
	break;

      case RET :
	more=0;
	break;
      case ESC :
	more=0;
	y_n = -1;
	break;

      default:
	break;
      }
    }
  }
  wclose(w);
  return(y_n);
}
