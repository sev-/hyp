/*
 *  $Id: proto.h,v 1.3 1994/07/11 12:29:35 sev Exp $
 *
 * ---------------------------------------------------------- 
 *
 * $Log: proto.h,v $
 * Revision 1.3  1994/07/11 12:29:35  sev
 * *** empty log message ***
 *
 * Revision 1.2  1993/09/28  12:58:34  sev
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
void write_files();
void new_client();
void kill_client();
void statist_client();
void change_client();

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
int passwd();

/* copy.c */
void filecopy();

