/*
 *  $Id: vced.h,v 1.1 1993/04/06 14:14:07 sev Exp $
 *
 * ---------------------------------------------------------- 
 *
 * $Log: vced.h,v $
 * Revision 1.1  1993/04/06 14:14:07  sev
 * Initial revision
 *
 * Revision 3.3  1993/04/01  14:10:29  kas
 * *** empty log message ***
 *
 * Revision 3.2  1993/03/27  12:08:16  kas
 * *** empty log message ***
 *
 * Revision 3.1  1993/03/24  16:39:49  kas
 * *** empty log message ***
 *
 * Revision 1.3  1993/03/17  15:53:58  sev
 * MAXLENSTR - размер строки в редакторе.
 *
 * Revision 1.2  1993/03/13  12:07:21  sev
 * Исправлены ошибки в delsels, убран ПФ4 и ПФ3
 *
 * Revision 1.1  1993/03/12  15:44:23  sev
 * Init
 *
 */

#define VCEDMAXREC       250

#define MAXLIN          1024        /* Maximum line to evaluate             */
#define CUTBUFS            3        /* Maximum number to cut buffers        */
#define BKSPNODEL                   /* Backspace does not delete in OverWrit*/

#define MAXLINESIZE      513

/* Buffer control values */
#define VCEDFBUF        BIT1
#define VCED2BUF        BIT2
#define VCEDFFIL        BIT3
#define VCED2FIL        BIT4
#define VCEDREQ         BIT5

/* Editor control values */
#define VCEDNOSHOW      BIT0
#define VCEDOVER        BIT1
#define VCEDSTATUS      BIT2

/* Editor update values */
#define VCEDUPDLINE     BIT0
#define VCEDUPDCHAR     BIT1
#define VCEDUPDEOL      BIT2
#define VCEDUPDPLINE    BIT3
#define VCEDUPDPCHAR    BIT4
#define VCEDUPDPEOL     BIT5
#define VCEDUPDALL      BIT6

                            /* ------- Editor Mail Values --------- */   
                            /* These cause vcedit to exit           */
#define VCEDSAVE        9999
#define VCEDABORT       9998
#define VCEDDELFILE     9997
#define VCEDFINDFILE    9996
#define VCEDNEWFIL      9994
#define VCEDNEXTFIL     9993

                            /* These are used by undo / redo        */
#define VCEDREDO        4444
#define VCEDUNDO        3333

#define ENDMARK         1234
#define UNMARK          4321

#define NAME_LEN          20
#define MES_LEN           60
#define ALL_LEN sizeof(struct dir)

#define SEGOFLINE         70

#define DIR_FILE           3
#define ED                 0

#define ON_                1
#define OFF_               0

#define PRIV_YES           1
#define PRIV_NO            0

#define PRIV_SEGWDO        4

#define SEL_DEFS

#include <vcstdio.h>
#include <vcdbms.h>
#include "edstruct.h"
#include "edkey.h"
#include "edextfun.h"
#include "edextern.h"


#define ATR_NO   vc.black+vc.bg*vc.white
#define ATR_YES  vc.blue+vc.bg*vc.black
#define ATR_B    vc.black+vc.bg*vc.white
#define ATR_F    vc.white+vc.bg*vc.black
#define ATR_FON  vc.black+vc.bg*vc.white
