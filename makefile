#
#  $Id: makefile,v 1.3 1993/04/20 16:04:12 sev Exp $
#
# $Log: makefile,v $
# Revision 1.3  1993/04/20 16:04:12  sev
# a
#
# Revision 1.2  1993/04/13  09:21:13  kas
# *** empty log message ***
#
# Revision 1.1  1993/04/12  15:13:06  kas
# Initial revision
#
#
#

CFLAGS = -Ot

CC = rcc

OBJS =  drivesel.o edaskdir.o edback.o edconf.o\
	edconinf.o eddelcon.o eddelendl.o eddellin.o eddir.o eddisp.o \
	edend.o edexec.o edsvf.o  edfnd.o edftostr.o\
	edhelp.o edhmark.o edhypend.o edinscon.o edinsline.o\
	edinsseg.o edit.o editor.o edmarkpg.o \
	edmesg.o edmove.o ednumseg.o edopen.o edpaste.o edprtb.o\
	edprtl.o edputch.o edsignal.o\
	edstat.o edsys.o edupda.o edvmark.o edwopen.o f_to_b.o loadfile.o\
	system.o wrkselset.o match.o crtlstfl.o addrefer.o

HFILES =	edextern.h	edextfun.h	edinitstr.h	edkey.h\
		edstruct.h	vced.h

edi:	$(OBJS)
	$(CC) -o edi $(CFLAGS) $(OBJS) -lvced -lvcdbms -lvc -lx

$(OBJS):	$(HFILES)

.c.o:
	     $(CC) -c $(CFLAGS) -I/usr/vcmu/include $<

