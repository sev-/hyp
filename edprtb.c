/*
 *  $Id: edprtb.c,v 1.4 1993/09/27 13:02:11 sev Exp $
 *
 * ---------------------------------------------------------------------------
 *
 * $Log: edprtb.c,v $
 * Revision 1.4  1993/09/27 13:02:11  sev
 * Исправлен способ выдачи конца страницы на экран
 *
 * Revision 1.3  1993/04/20  16:04:12  sev
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

#include <math.h>
#include <string.h>
#include "vced.h"

extern int flagsegm;

/* ------------------------------------------------------------------------ */

COUNT edprtbuf(vced)
VCED *vced;                         /* Pointer to edit structure            */
{                                   /* ------------------------------------ */
	VCEDLINE *lptr = (VCEDLINE *)0;   /* Pointer to current line              */
	VCEDBUF *edbuf;                   /* Edit buffer                          */
	DBDP cdbdp;                       /* Current data base data pointer       */
	LONG crow;                        /* Current row in file                  */
	COUNT row = 0;                    /* Current row                          */
	COUNT depth;                      /* Depth of current window              */
	WPTR wptr;                        /* Current window                       */
	COUNT ret;

	edbuf = vced->edbuffer;                       /* Get buffer pointer       */
	wptr = vced->edwptr;                          /* Window to use            */
	depth = wrows(wptr);                          /* Get depth                */

	woff();                                       /* Freeze output to screen  */
	werase(wptr,-1);                              /* Erase Window             */
	cdbdp = vced->edtline;                        /* Assign current dbase ptr */
	if(cdbdp != (DBDP)0);                         /* If there is a top line   */
	lptr = vcedgline(edbuf,cdbdp);                /* Get top of screen line   */
	crow = vced->edtrow;                          /* Start at top row         */
	while((lptr != (VCEDLINE *)0) &&              /* While more lines         */
	(row < depth) )                               /*  and not at bottom       */
	{                                             /*                          */

		edsetfta(vced, crow, lptr,from,to,stat_);   /* Find settings            */
		wat(wptr,row++,vced->edlmar);               /* Next row                 */
		if(strstr(lptr->ltext,"\014")!=0)
			watsay(wptr,row-1,0,"--<EOP>----------------------------------------------------------------------");
		else
			eddisp(vced,wptr,lptr,vced->edoffset,from,to,stat_);
		cdbdp = lptr->lnext;                        /* Assign current dbase ptr */
		lptr = vcednline(edbuf,lptr);               /* Get next line of file    */
		crow++;                                     /* Looking at next line     */
	}                                             /*                          */
#ifdef IBM
	wdoctrl->wfreeze = FALSE;                     /* Turn on updates          */
	wstack(wptr);                                 /* Redraw window            */
	vca_put(0,0,wscreen,vcterm.rows*vcterm.cols); /*   Put on screen          */
#else
	won();                                        /* Turn on window           */
#endif
}                                               /* ------------------------ */

/* ------------------------------------------------------------------------ */

VOID edsetfta(vced, crow, lptr, from, to, stat_)
VCED *vced;
LONG crow;                  /* Current row evaluating               */
VCEDLINE *lptr;
COUNT from[];                       /* Начало поля                          */
COUNT to[];                         /* Конец  поля                          */
COUNT stat_[];
{
	char buf[513];
	TEXT from_[3],to_[3];
	char *work,*tmp,*strstr(),*ch;
	int i1,i2,i=0;

	for(i=0;i<=49;i++)
	{
		to[i] = -1;
		from[i] = -1;                               /* Set from to -1           */
		stat_[i]=1;
	}
	vcedempty(buf,513);
	strncpy(buf,lptr->ltext,lptr->lused);
	tmp=buf;
	i=0;
	while( (work=strstr(tmp,"\033<")) != (char *)NULL )
	{
		if((ch=strstr(work,"#;#"))!=(char *)NULL  && (strlen(ch) > strlen(strstr(work,">"))) )
			stat_[i]=0;
		tmp=work+2;
		vcedempty(from_,3);
		vcedempty(to_,3);
		strncpy(from_,work+2,2);
		strncpy(to_,work+5,2);

		from[i]=atoi(from_);
		to[i]=atoi(to_);
		i++;
	}
	if(flagsegm)
	{
		if(crow == vced->edcrow)
		{
			if(vced->edarow < vced->edcrow)
			{
				from[i] = 0;
				to[i] = vcedval.linlen;
				stat_[i]=0;
			}
			else if(vced->edarow == vced->edcrow)
			{
				from[i] = 0;
				to[i] = vcedval.linlen;
				stat_[i]=0;
			}
			else 
			{
				from[i] = 0;
				to[i] = vcedval.linlen;
				stat_[i]=0;
			}
		}
		else if(crow == vced->edarow)
		{
			if(vced->edcrow < vced->edarow)
			{
				from[i] = 0;
				to[i] = vced->edachar;
				stat_[i]=0;
			}
			else 
			{
				from[i] = 0;
				to[i] = vcedval.linlen;
				stat_[i]=0;
			}
		}
		else if(((crow > vced->edarow) && (crow < vced->edcrow)) ||
		    ((crow < vced->edarow) && (crow > vced->edcrow)) )
		{
			from[i] = 0;
			to[i] = vcedval.linlen;
			stat_[i]=0;
		}
	}
}

/* ------------------------------------------------------------------------ */
