/**
*
*   Vitamin C Multi-User version 1.6
*   Copyright (c) 1985-1990
*   Creative Programming Consultants, Inc.
*   P.O. Box 112097
*   Carrollton, Texas  75006
*   (214) 416-6447
*   ALL RIGHTS RESERVED
*   Last Modification: .5 on 8/31/90 at 22:18:57
*
*   Name            Unix/Xenix Routines
*
**/

#define VCSTRIP

#define IOCTL

#include <vcstdio.h>
#include <vcstruct.h>
#include <sys/types.h>
#include <sys/select.h>
#include <time.h>

fd_set fdset;

#ifdef UNIX

#ifndef TRUE 
#define TRUE 1
#define FALSE 0
#endif

#include <fcntl.h>
#include <signal.h>
#include <setjmp.h>

#ifdef TTY
#include <sgtty.h>
static struct sgttyb tsavein;
static struct sgttyb tsaveout;
#endif

#ifdef IOCTL
#include <termio.h>
#include <sys/ioctl.h>
static struct termio tsavein;
static struct termio tsaveout;
#endif

COUNT vcbaud = 9600;
static COUNT vcrawset = FALSE;
extern struct termctrl vcterm;
int vcindev = 0;
static int vcoutdev = 1;
extern TEXT *vcinput;
extern TEXT *vcoutput;

VOID set_raw()
    {
    COUNT vcfdbaud();
    VOID vcdevsetup();
    VOID vcsetsig();
#ifdef IOCTL
    struct termio tbuf;
    vcdevsetup();
    if(ioctl(vcindev,TCGETA,&tsavein) == -1)
        return;
    if(ioctl(vcoutdev,TCGETA,&tsaveout) == -1)
        return;
    if(ioctl(vcindev,TCGETA,&tbuf) == -1)
        return;
    tbuf.c_iflag &= ~(INLCR | ICRNL | IUCLC);
    tbuf.c_lflag &= ~(ICANON | ECHO);
vcterm.zc = 1;
    tbuf.c_cc[4] = max(vcterm.zc,1);
#ifndef REPEAT_KEYRDY
    tbuf.c_cc[5] = max(vcterm.zt,1);
#else
    tbuf.c_cc[5] = 1;
#endif
    if(ioctl(vcindev,TCSETAF,&tbuf) == -1)
        return;
    if(ioctl(vcoutdev,TCGETA,&tbuf) == -1)
        return;
    tbuf.c_oflag &= ~OPOST;
    if(ioctl(vcoutdev,TCSETAF,&tbuf) == -1)
        return;
#endif
#ifdef TTY
    struct sgttyb tty;
    vcdevsetup();
    gtty(vcindev,&tsavein);
    gtty(vcoutdev,&tsaveout);
    gtty(vcindev,&tty);
    tty.sg_flags &= ~ECHO;
    tty.sg_flags |= CBREAK;
    stty(vcindev,&tty);
    gtty(vcoutdev,&tty);
    tty.sg_flags &= ~ECHO;
    tty.sg_flags |= CBREAK;
    stty(vcoutdev,&tty);
#endif
    vcrawset = TRUE;
    vcbaud = vcfdbaud();
    vcsetsig();
    }

VOID set_normal()
    {
    if(!vcrawset)
        return;
#ifdef IOCTL
    if(ioctl(vcoutdev,TCSETAF,&tsaveout) == -1)
        return;
    if(ioctl(vcindev,TCSETAF,&tsavein) == -1)
        return;
#endif
#ifdef TTY
    stty(vcoutdev,&tsaveout);
    stty(vcindev,&tsavein);
#endif
    if((vcinput != NULLTEXT) && (vcindev != 0))
        close(vcindev);
    if((vcoutput != NULLTEXT) && (vcoutdev != 1))
        close(vcoutdev);
    vcrawset = FALSE;
    }

VOID vcdevsetup()
    {
    TEXT buf[180];
    if(vcinput != NULLTEXT)
        if((vcindev = open(vcinput,O_RDONLY)) == -1)
            {
            strcpy(buf,"Error Opening Input: ");
            strcat(buf,vcinput);
            write(2,buf,strlen(buf));
            exit(1);
            }
    if(vcoutput != NULLTEXT)
        if((vcoutdev = open(vcoutput,O_WRONLY)) == -1)
            {
            strcpy(buf,"Error Opening Output: ");
            strcat(buf,vcoutput);
            write(2,buf,strlen(buf));
            exit(1);
            }
    }

VOID vcsetsig()
    {
    VOID vcsigint();
    VOID vcsigquit();
#ifdef SIGTSTP
    VOID vcsigtstp();
    VOID vcsigcont();
#endif
    signal(SIGINT,vcsigint);
    signal(SIGQUIT,vcsigquit);
    signal(SIGILL,SIG_IGN);
    signal(SIGFPE,SIG_IGN);
    signal(SIGBUS,SIG_IGN);
    signal(SIGSEGV,SIG_IGN);
#ifdef SIGTSTP
    signal(SIGTSTP,vcsigtstp);
    signal(SIGCONT,vcsigcont);
#endif
    }

VOID vcsigint()
    {
    signal(SIGINT,SIG_IGN);
    set_normal();
    vcout("\nInterrupted by user\n",21);
    exit(1);
    }

VOID vcsigquit()
    {
    signal(SIGQUIT,SIG_IGN);
    set_normal();
    vcout("\nInterrupted by user\n",21);
    exit(1);
    }

#ifdef SIGTSTP
VOID vcsigtstp()
    {
    set_normal();
    kill(getpid(),SIGSTOP);
    }

VOID vcsigcont()
    {
    set_raw();
    }
#endif
            

#ifdef NONBLOCK

static COUNT filestat;

VOID vcrdblk(flag)
COUNT flag;
    {
    COUNT noblock;
    if(flag)
        {
        filestat = fcntl(vcindev,F_GETFL,0);
        noblock = filestat | O_NDELAY;
        fcntl(vcindev,F_SETFL,noblock);
        }
    else
        fcntl(vcindev,F_SETFL,filestat);
    }

#endif

/**
*
*   Name            vcfdbaud.c  --  Find terminal baud rate
*
*   Description     This function returns the current baud rate
*
*   Return Value    VOID
*
**/

COUNT vcfdbaud()
    {
#ifdef VCDEBUG
    vcdebug = (TEXT *) "findbaud:";
    if(vcdbfunc != NULLFUNC)
        (*vcdbfunc)(CONTINUE);
#endif
#ifdef IOCTL
    struct termio tbuf;
    COUNT baud;
    static COUNT blist[16] = 
        { 0,50,75,110,130,150,200,300,600,1200,1800,2400,4800,9600,19200,0 };
    if(ioctl(vcoutdev,TCGETA,&tbuf) == -1)
        return(0);
    baud = tbuf.c_cflag & CBAUD;
    baud = blist[baud];
    return(baud);
#endif
#ifdef TTY
    return(9600);
#endif
    }

int flag_esc_sequence;
    
jmp_buf vcjmp_pt;
COUNT vcjump = FALSE;
extern PFI waitfunc;

COUNT vcin()
    {
    COUNT nread;
    static struct timeval tt; 
    COUNT cnt = 1;
    TEXT ch[100];
    PFI old;
    int otime = -1;
    VOID vcloop();
    
    
    /* "#$% */
    
    FD_SET(vcindev, &fdset);
    tt.tv_sec = 2l;
    tt.tv_usec = 0l;

    if(waitfunc)
        {
        old = (PFI)signal(SIGALRM,vcloop);
        otime = alarm(2);
        }
    setjmp(vcjmp_pt);
    while(TRUE)
        {
        if(flag_esc_sequence == 1)
        {
		select(32, &fdset, NULL, NULL, &tt);
		FD_SET(vcindev, &fdset);
	}

        nread = read(vcindev,&ch[0],vcterm.zc);
		if(ch[0] == 033)
		{
			select(32, &fdset, NULL, NULL, &tt);
			FD_SET(vcindev, &fdset);
			flag_esc_sequence = 1;
		}
        if(nread != BADREAD)
            break;
        }
    if(waitfunc)
        {
        alarm(0);
        signal(SIGALRM,SIG_IGN);
        }
    while(cnt < nread)
        ungetone((COUNT)ch[cnt++]);
    return((COUNT) ch[0]);
    }

VOID vcloop()
    {
    alarm(0);
    (*waitfunc)();
    signal(SIGALRM,vcloop);
    alarm(2);
    longjmp(vcjmp_pt,1);
    }

COUNT vcout(buf,nbytes)
TEXT *buf;
COUNT nbytes;
    {
    int write();
    return((COUNT)write(vcoutdev,buf,(int)nbytes));
    }

COUNT xkeyrdy()
    {
#ifdef NONBLOCK
    COUNT ch;
    TEXT ich;
#endif
#ifdef RDCHK
    if(rdchk(0) > 0)
        return((COUNT)1);
    else
        return((COUNT)0);
#endif
#ifdef NONBLOCK
    vcrdblk(TRUE);
    if(((ch = read(vcindev,&ich,1)) == BADREAD) ||
       (ch == 0))
        ch = 0;
    else
        {
        ch = (COUNT)ich;
        ungetone(ch);
        ch = 1;
        }
    vcrdblk(FALSE);
    return(ch);
#endif
    }

#endif


