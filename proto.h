/*
 *  $Id: proto.h,v 1.2 1993/09/28 12:58:34 sev Exp $
 *
 * ---------------------------------------------------------- 
 *
 * $Log: proto.h,v $
 * Revision 1.2  1993/09/28 12:58:34  sev
 * Добавлена куча всего
 *
 * Revision 1.1  1993/09/14  16:45:50  sev
 * Initial revision
 *
 *
 */

WPTR wxopen();
WPTR wxxopen();
 
/* drivesel.c */
int drivesel();
WPTR xdrivesel();

/* shell.c */
void view();
void read_db();
void write_db();
char *select_wrote();
int passwd();

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

/* menus.c */
void initmenus();

/* lib.c */
int ask_msg();

/* copy.c */
void filecopy();

