/*
 *  $Id: edextern.h,v 1.2 1993/04/20 16:04:12 sev Exp $
 *
 * ---------------------------------------------------------------------------
 *
 * $Log: edextern.h,v $
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
