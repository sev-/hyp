/*
 *  $Id: edprtl.c,v 1.2 1993/04/20 16:04:12 sev Exp $
 *
 * ---------------------------------------------------------------------------
 *
 * $Log: edprtl.c,v $
 * Revision 1.2  1993/04/20 16:04:12  sev
 * a
 *
 * Revision 1.4  1993/04/19  16:36:31  kas
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

COUNT edprtline(vced,dbdp,from,to,stat_)
VCED *vced;                         /* Pointer to edit structure            */
DBDP dbdp;                          /* Pointer to line to refresh           */
COUNT from[],to[],stat_[];          /* Начало и конец поля                  */
{                                   /* ------------------------------------ */
	VCEDLINE *lptr;                   /* Pointer to current line              */
	COUNT attr = -1;                  /* Attribute to use for erase           */
	COUNT row,col,bd;
	COUNT twidth;
	COUNT start=0;
	FAST TEXT *sptr;
#ifdef TERMINAL
	COUNT flow = FALSE;               /* Did we stop screen output            */
#endif
	WPTR wptr;                        /* Current window                       */

	wptr = vced->edwptr;              /* Window to use                        */
	lptr = vcedgline(vced->edbuffer,vced->edcline);/* Get cur line            */
	start = max(start,vced->edoffset);/* Update max                           */
#ifdef TERMINAL
	flow = vcpee();                   /* Turn off updates                     */
#endif
	cur_switch = FALSE;               /* Turn off cursor updates              */
	row = (COUNT)(vced->edcrow-vced->edtrow);
	woff();                           /* Turn window off                      */
	wat(wptr,row,vced->edlmar+start-vced->edoffset);/* Address cur.           */
	weraeol(wptr,attr);                 /* Erase to end of line               */
	eddisp(vced,wptr,lptr,start,from,to,stat_); /* Display line               */
	start -= vced->edoffset;
	bd=(wptr->wctrl & BORDER ? 1: 0);
	twidth = vcterm.cols << 1;
	row += wptr->upper_r+bd-wptr->cur_r;
	col = wptr->upper_c+bd-wptr->cur_c;
	sptr = wscreen+(row*twidth)+((col+start) << 1);
	wdoctrl->wfreeze = FALSE;
	wstack(wptr);
	vca_put(row,col+start,sptr,wcols(wptr)-start);
	cur_switch = TRUE;                  /* Turn cursor updates on             */
#ifdef TERMINAL
	if(flow)                            /* If we stopped scr output           */
		vcflush();                        /* Flush output                       */
#endif
}                                     /* ---------------------------------- */
