/*
 *  $Id: system.c,v 1.2 1993/04/20 16:04:12 sev Exp $
 *
 * ---------------------------------------------------------------------------
 *
 * $Log: system.c,v $
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

/**
*
*   Name            vcsystem - make system call
*                                   
*   Description     This function sets up Vitamin C
*                   to make a system call.
*
*   Return Value    Returns what system returns
*                                   
**/

#include <vcstdio.h>

COUNT vcsystem(cmd,flag,msg)
TEXT *cmd;                  /* Command to execute                   */
COUNT flag;                 /* True to clear screen before call     */
TEXT *msg;                  /* Pause and print this message (askmsg)*/
    {                       /* ------------------------------------ */
    COUNT curstat;          /* Save the cursor status               */
    COUNT retval;           /* Value returned by the system call    */    
    VOID erase();           /* Declare functions used               */
    VOID vccurs();          /*    "        "      "                 */
    VOID wrefresh();        /*    "        "      "                 */
#ifdef TERMONLY
    VOID set_normal();      /*    "        "      "                 */
    VOID set_raw();         /*    "        "      "                 */
#endif
    if(flag)                            /* If we are to clear screen*/
        {                               /*                          */
        override = TRUE;                /* Go direct to screen      */
        erase();                        /* Erase the screen         */
        override = FALSE;               /* Restore override value   */
        }                               /*                          */
    curstat = vccursor;                 /* Save the cursor status   */
    if((vcterm.ge != NULLTEXT) && graphattr)    /* If graphics on   */
        {                               /*                          */
        dist_str(vcterm.ge,0);          /* Turn graphics off        */
        graphattr = FALSE;              /* Set graphics flag        */
        }                               /*                          */
    if(curstat > 2)                     /* Is the cursor off        */
        vccurs(ON);                     /* Turn the cursor on       */
#ifdef TERMONLY
    flipattr(normattr);                 /* Make attribute normal    */
    if(vcterm.es != NULLTEXT)           /* If there is exit string  */
        sendpstr(vcterm.es);            /* Send exit string         */
    set_normal();                       /* Set terminal normal      */
#endif
    retval = system(cmd);               /* Call system command      */
#ifdef UNIX
    set_raw();                          /* Set terminal in raw mode */
    if(vcterm.is != NULLTEXT)           /* Is there a setup string  */
        dist_str(vcterm.is,0);          /* Send setup string        */
#endif
    if(msg != NULLTEXT)                 /* If they want to pause    */
        askmsg(NULLTEXT,msg,0);         /* Display message          */
    wrefresh(flag);                     /* Redraw the screen        */
    if(curstat > 2)                     /* If cursor was off        */
        vccurs(OFF);                    /* Turn cursor off          */
    return(retval);                     /* Return the value         */
    }                                   /* ------------------------ */

/* ------------------------------------------------------------------------ */
/*                                                                          */
/*       The UNIX Programming Environment  B. Kernigan & Rob Pike           */
/*                                                                          */
/* ------------------------------------------------------------------------ */
#include <signal.h>
#include <stdio.h>

system(s)
char *s;
{
  int status, pid, w, tty1;
  int (*istat)(), (*qstat)();
  extern char *programme;

  fflush(stdout);
  tty1 = open("/dev/tty", 2);
  if(tty1 == -1)
    return -1;

  if((pid = fork()) == 0)
  {
    close(0);
    dup(tty1);
    close(1);
    dup(tty1);
    close(2);
    dup(tty1);
    close(tty1);
    execlp("ksh", "ksh", "-c", s, (char *)0);
    exit(127);
  }
  close(tty1);
  istat = signal(SIGINT, SIG_IGN);
  qstat = signal(SIGQUIT, SIG_IGN);
  while((w = wait(&status)) != pid && w != -1);

  if(w == -1)
    status = -1;
  signal(SIGINT, istat);
  signal(SIGQUIT, qstat);

  return status;
}
/* ------------------------------------------------------------------------ */
