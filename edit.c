/*
 *  $Id: edit.c,v 1.1 1993/04/06 14:14:07 sev Exp $
 *
 * ---------------------------------------------------------------------------
 *
 * $Log: edit.c,v $
 * Revision 1.1  1993/04/06 14:14:07  sev
 * Initial revision
 *
 * Revision 4.5  1993/04/06  13:40:54  kas
 * *** empty log message ***
 *
 * Revision 4.4  1993/04/05  16:01:33  kas
 * *** empty log message ***
 *
 * Revision 4.3  1993/04/01  14:10:29  kas
 * *** empty log message ***
 *
 * Revision 4.2  1993/03/27  12:08:16  kas
 * *** empty log message ***
 *
 * Revision 4.1  1993/03/25  11:01:23  kas
 * *** empty log message ***
 *
 * Revision 1.3  1993/03/23  13:10:22  kas
 * *** empty log message ***
 *
 * Revision 1.2  1993/03/13  12:07:21  sev
 * Исправлены ошибки в delsels, убран ПФ4 и ПФ3
 *
 * Revision 1.1  1993/03/12  15:44:23  sev
 * Init
 *
 *
 */

static char rcsid[]="$Id: edit.c,v 1.1 1993/04/06 14:14:07 sev Exp $";

#include "vced.h"
/*********************************************************************/
/*   Основная функция редактора 				     */
/*********************************************************************/

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
