/*
 *  $Id: edsys.c,v 1.2 1993/04/20 16:04:12 sev Exp $
 *
 * ---------------------------------------------------------------------------
 *
 * $Log: edsys.c,v $
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

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "vced.h"

COUNT edsys(vced)
VCED *vced;  
{
  char buf[256];
  int key;
  WPTR w_;

  w_ = wxxopen(vcterm.rows-1,0,vcterm.rows-1,
	  77,NULLTEXT,ACTIVE+CURSOR,0,0,1,32);

  empty(buf,256);
  atsay(0,0," Команда :                                                   ");

      while(1)
      {
	xatgetc(0,11,buf,NULLTEXT,NULLFUNC,NULLTEXT,NULLTEXT,
			ATR_B,ATR_B,30,FLDSTRING,NULLTEXT);
	key = readgets();
	if(key==ESC)
	{
	    wclose(w_);
	    wselect(vced->edwptr);
	    break;
	}
	trim(buf);
	vcsystem(buf,1,NULL);
      } 
 }
