/*
 *  $Id: edextern.h,v 1.1 1993/04/06 14:14:07 sev Exp $
 *
 * ---------------------------------------------------------- 
 *
 * $Log: edextern.h,v $
 * Revision 1.1  1993/04/06 14:14:07  sev
 * Initial revision
 *
 * Revision 3.5  1993/04/05  16:01:33  kas
 * *** empty log message ***
 *
 * Revision 3.4  1993/04/01  14:10:29  kas
 * *** empty log message ***
 *
 * Revision 3.3  1993/03/29  11:28:20  kas
 * *** empty log message ***
 *
 * Revision 3.2  1993/03/27  12:08:16  kas
 * *** empty log message ***
 *
 * Revision 3.1  1993/03/24  16:39:49  kas
 * *** empty log message ***
 *
 * Revision 1.2  1993/03/13  12:07:21  sev
 * Исправлены ошибки в delsels, убран ПФ4 и ПФ3
 *
 * Revision 1.1  1993/03/12  15:44:23  sev
 * Init
 *
 */

extern VCEDVAL vcedval;
extern VCEDCMD vcedcmds[];
extern struct VCEDFILE *cfile;
int    col_seg_of_file;
int    from[50],to[50],stat_[50];
int    stat_hyp;
int    stat_exec;
int    show;
TEXT   vcedfbuf[MAXLIN];      /* File buffer work area                */
TEXT   vcedibuf[MAXLIN];      /* File buffer work area                */
TEXT   vceddfil[80];          /* Default file location                */
