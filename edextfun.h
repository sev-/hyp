/*
 *  $Id: edextfun.h,v 1.1 1993/04/06 14:14:07 sev Exp $
 *
 * ---------------------------------------------------------- 
 *
 * $Log: edextfun.h,v $
 * Revision 1.1  1993/04/06 14:14:07  sev
 * Initial revision
 *
 * Revision 4.4  1993/04/01  14:10:29  kas
 * *** empty log message ***
 *
 * Revision 4.3  1993/03/29  11:28:20  kas
 * *** empty log message ***
 *
 * Revision 4.2  1993/03/27  12:08:16  kas
 * *** empty log message ***
 *
 * Revision 4.1  1993/03/25  11:01:23  kas
 * *** empty log message ***
 *
 * Revision 3.1  1993/03/24  16:39:49  kas
 * *** empty log message ***
 *
 * Revision 1.4  1993/03/23  13:10:22  kas
 * *** empty log message ***
 *
 * Revision 1.3  1993/03/17  15:53:58  sev
 * Добавил одну функцию, которую пропустил ранее
 *
 * Revision 1.2  1993/03/13  12:07:21  sev
 * Исправлены ошибки в delsels, убран ПФ4 и ПФ3
 *
 * Revision 1.1  1993/03/12  15:44:23  sev
 * Init
 *
 *
 */

COUNT     isblank();
char     *make_name();
VCEDLINE *vcedmline();
VCEDLINE *vcedgline();
VCEDLINE *vcednline();
PFI       edfndcmd();
TEXT     *strdob();
VCEDBUF  *loadfile();
VCED     *edopen();
SELSET   *getselitm();
COUNT     edir();
COUNT     edhelp();
COUNT     edlistwdo();
COUNT     edfiles();
COUNT     edcompil();
COUNT     edshow();
COUNT     edvmark();
COUNT     edhmark();
COUNT     ed_del_conc();
COUNT     edinf();
COUNT     edendmark();
COUNT     edunmark();
COUNT     vcedmvleft();
COUNT     vcedmvright();
COUNT     vcedmvup();
COUNT     vcedmvdown();
COUNT     vcedexit();
COUNT     vcedprtbuf();
COUNT     vcedinstog();
COUNT     vcedbksp();
COUNT     vcedtol();
COUNT     vcedeol();
COUNT     vcedreturn();
COUNT     vcedabort();
COUNT     vcedtab();
COUNT     vcedbktab();
COUNT     vcedtof();
COUNT     vcedeof();
COUNT     vcedhome();
COUNT     edend();
COUNT     ednewf();
COUNT     ednextf();
COUNT     edmark();
COUNT     edmarkpg();
COUNT     vcedpgdn();
COUNT     vcedpgup();
COUNT     edpaste();
COUNT     edconf();
COUNT     eddelf();
COUNT     eddelline();
COUNT     edsavef();
COUNT     eddelendl();
COUNT     vceddelete();
COUNT     edsys();
COUNT     edinsline();
COUNT     edrefresh();
WPTR      wopen();
WPTR      wxopen();
WPTR      wxxopen();
