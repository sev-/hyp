/*
 *  $Id: edit.c,v 1.2 1993/04/20 16:04:12 sev Exp $
 *
 * ---------------------------------------------------------------------------
 *
 * $Log: edit.c,v $
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

/****************************************************************************/
/*          Основная функция редактора                                      */
/****************************************************************************/

COUNT medit(vced)
VCED *vced;                  /* Pointer to edit structure            */
{                            /* ------------------------------------ */
  wselect(vced->edwptr);                 /* to be sure on top        */
  while(vced->edmail < 9990)             /* While not ready to exit  */
  {                                      /*                          */
    edupdate(vced);                      /* Do any updates           */
    vced->edkey[2] = vced->edkey[1];     /* Move key down one        */
    vced->edkey[1] = vced->edkey[0];     /* Move key down one        */
    vced->edkey[0] = getone();           /* Get key                  */
    if(stat_hyp==PRIV_SEGWDO)
    {
	   switch(vced->edkey[0])
	   {
		case  DEL_FILE:
		case  DEL_POLE:
		case  CONF:
		case  SEGM:
		case  POLE:
		case  PASTE:
		      continue;
		default:
		       break;
	   }
    }
    edexec(vced);                        /* Execute key pressed      */
    edexeckey(vced);                     /*                          */
  }                                      /* End of while             */
}                                        /* ------------------------ */

/* ------------------------------------------------------------------------ */

int edexeckey(vced)
VCED *vced;
{
     switch(vced->edkey[0])
     {
	case POLE:
	     if(stat_hyp==PRIV_SEGWDO)
		  break;
	     ed_horiz_mark(vced);
	     break;
	case SEGM:
	     if(stat_hyp==PRIV_SEGWDO)
		  break;
	     ed_vert_mark(vced);
	     break;
	case DIR_SEG:
	case LISTWDO:
	case FILES:
	     eddir(vced);
	     break;
	case INF_CONC:
	     ed_con_inf(vced);
	     break;
	case SCRN_SEG:
	     break;
      }
}

/* ------------------------------------------------------------------------ */
