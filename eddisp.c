/*
 *  $Id: eddisp.c,v 1.1 1993/04/06 14:14:07 sev Exp $
 *
 * ---------------------------------------------------------------------------
 *
 * $Log: eddisp.c,v $
 * Revision 1.1  1993/04/06 14:14:07  sev
 * Initial revision
 *
 * Revision 1.3  1993/04/05  16:01:33  kas
 * *** empty log message ***
 *
 * Revision 1.2  1993/03/27  12:08:16  kas
 * *** empty log message ***
 *
 * Revision 1.1  1993/03/12  15:44:23  sev
 * Init
 *
 *
 */

static char rcsid[]="$Id: eddisp.c,v 1.1 1993/04/06 14:14:07 sev Exp $";


#include "vced.h"

/****************************************************************************/
/*          Функция выводит строку в окно с отмеченными полями              */
/****************************************************************************/

COUNT eddisp(vced,wptr,lptr,start,from1,to1,stat1_)
VCED *vced;                 /* Pointer to1 edit structure                   */
WPTR wptr;                  /* Pointer to1 window for display               */
VCEDLINE *lptr;             /* Pointer to1 line to1 display                 */
COUNT start;                /* Starting location                            */
COUNT from1[];              /* Start lighting infomation                    */
COUNT to1[];                /* End lighting information                     */
COUNT stat1_[];
{                           /* -------------------------------------------- */
  FAST TEXT *sptr;          /* Pointer to1 window buffer                    */
  COUNT offset;             /* Offset to1 start at in window buffer         */
  TEXT outchr;              /* Character to1 output                         */
  COUNT length = 0;         /* Length of current line                       */
  COUNT output = 0;         /* Temporary variable for output count          */
  COUNT width;              /* Width of window                              */
  COUNT attr;               /* Attribute to1 print with                     */
  COUNT tmp;                /* Temporary variable                           */
  TEXT *ptr;                /* Actual text of current line                  */
  COUNT i;

  width = wcols(wptr);                /* Get width                          */
  attr = wptr->say_at;                /* Set default attribute              */
  length = lptr->lused;               /* Find length of line                */

  if(from1[0] >= 0)                   /* If this row lighted                */
    for(i=0;i<=49;i++)
      length = max(length,to1[i]+1);  /* Set to1 display to1 cursor         */
  ptr = lptr->ltext;                  /* Point to1 first char               */
  ptr += start;                       /* Point to1 start char               */

  length -= start;                    /* Subtract for start                 */
  width -= start - vced->edoffset + vced->edlmar; /* Adjust width           */
  width = width-vced->edlmar;         /* Len must be < width                */
  output = 0;                         /* Initialize count variable          */
  offset = ((wptr->r_pos*wptr->width+wptr->c_pos) << 1);
  sptr = wptr->location + offset;     /* Find offset into1 window           */
  i=0;
  while((output < length) &&          /* While more characters              */
  (output < width) )                  /* And more room                      */
  {                                   /*                                    */
    if(from1[i] >= 0)                 /* If this row lighted                */
    {                                 /*                                    */
      tmp = output+start;             /* Temporary calculation              */
      if(tmp == from1[i+1] && from1[i+1] != -1)
	i++;
      if((tmp >= from1[i]) && (tmp <= to1[i]))
      {
	if(stat1_[i]==0)
	  attr = ATR_NO;
	else
	  attr = ATR_YES;
      }
      else                            /*                                    */
	attr = wptr->say_at;          /* Set normal attribute               */
    }
    if(*ptr == vcedval.tabchr)        /* If a tab character                 */
    {                                 /*                                    */
      if(vcedval.dispctrl)            /* If display controls                */
	outchr = '+';                 /* Display tab                        */
      else                            /*                                    */
	outchr = ' ';                 /* Display space                      */
    }                                 /*                                    */
    else if(*ptr == vcedval.tabspace) /* If a tab space                     */
    {                                 /*                                    */
      if(vcedval.dispctrl)            /* If display controls                */
	outchr = '.';                 /* Display tabsp                      */
      else                            /*                                    */
	outchr = ' ';                 /* Display space                      */
    }                                 /*                                    */
    else                              /*                                    */
      outchr = *ptr;                  /* Display character                  */
    *(sptr++) = outchr;               /* Move character                     */
    *(sptr++) = attr;                 /* Move attribute                     */
    output++;                         /* Next space                         */
    ptr++;
  }                                   /* ---------------------------------- */
}
