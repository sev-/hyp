/*
 *  $Id: edextfun.h,v 1.3 1993/04/20 16:04:12 sev Exp $
 *
 * ---------------------------------------------------------------------------
 *
 * $Log: edextfun.h,v $
 * Revision 1.3  1993/04/20 16:04:12  sev
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
SELSET   *aloselitm();
SELSET   *addselitm();
SELSET   *getselitm();
SELSET   *delselitm();
SELSET   *delselset();
VOID 	  lstsel();
int 	  putselset();
int 	  getselset();
SELSET   *initselset();
int	  add_refer();
SELSET 	 *repselset();
