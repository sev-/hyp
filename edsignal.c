/*
 *  $Id: edsignal.c,v 1.1 1993/04/06 14:14:07 sev Exp $
 *
 * ---------------------------------------------------------- 
 *
 * $Log: edsignal.c,v $
 * Revision 1.1  1993/04/06 14:14:07  sev
 * Initial revision
 *
 * Revision 1.1  1993/03/12  15:44:23  sev
 * Init
 *
 *
 */

static char rcsid[]="$Id: edsignal.c,v 1.1 1993/04/06 14:14:07 sev Exp $";


#include <vcstdio.h>


#ifdef UNIX
#include <signal.h>

VOID edsignal()
{
  VOID vcintsig();
  VOID vcquitsig();
  VOID vcillsig();
  VOID vcfpesig();
  VOID vcbussig();
  VOID vcsegvsig();
#ifdef SIGTSTP
  VOID vcstopsig();
  VOID vccontsig();
#endif
  signal(SIGINT,vcintsig);
  signal(SIGQUIT,vcquitsig);
  signal(SIGILL,vcillsig);
  signal(SIGFPE,vcfpesig);
  signal(SIGBUS,vcbussig);
  signal(SIGSEGV,vcsegvsig);
#ifdef SIGTSTP
  signal(SIGTSTP,vcstopsig);
  signal(SIGCONT,vccontsig);
#endif
}

VOID vcintsig()
{
  signal(SIGINT,SIG_IGN);
  signal(SIGINT,vcintsig);
}

VOID vcquitsig()
{
  signal(SIGQUIT,SIG_IGN);
  signal(SIGQUIT,vcquitsig);
}

VOID vcillsig()
{
  vcend(0);
  terror("illegal instruction");
}

VOID vcfpesig()
{
  vcend(0);
  terror("floating point exception");
}

VOID vcbussig()
{
  vcend(0);
  terror("bus error");
}

VOID vcsegvsig()
{
  vcend(0);
  terror("segmentation violation");
}

#ifdef SIGTSTP
VOID vcstopsig()
{
  set_normal();
  flipattr(normattr);
  if((vcterm.ge != NULLTEXT) && graphattr)
    dist_str(vcterm.ge,0);
  if(vcterm.es != NULLTEXT)
    sendpstr(vcterm.es);
  kill(getpid(),SIGSTOP);
}

VOID vccontsig()
{
  if(vcterm.is != NULLTEXT)
    dist_str(vcterm.is,0);
  set_raw();
  wrefresh(TRUE);
}
#endif

#endif
