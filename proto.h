/*
 *  $Id: proto.h,v 1.1 1993/09/14 16:45:50 sev Exp $
 *
 * ---------------------------------------------------------- 
 *
 * $Log: proto.h,v $
 * Revision 1.1  1993/09/14 16:45:50  sev
 * Initial revision
 *
 *
 */

WPTR wxopen();
WPTR wxxopen();
 
/* drivesel.c */
int drivesel();

/* shell.c */
void view();
void read_db();

/* wrkselset.c */
SELSET *aloselitm();
SELSET *addselitm();
SELSET *getselitm();
SELSET *delselitm();
SELSET *delselset();
VOID lstsel();
int putselset();
int getselset();
SELSET *initselset();
SELSET *repselset();
