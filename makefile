#
#  $Id: makefile,v 1.1 1993/04/06 14:14:07 sev Exp $
#
# $Log: makefile,v $
# Revision 1.1  1993/04/06 14:14:07  sev
# Initial revision
#
# Revision 4.3  1993/03/27  12:12:40  kas
# Not use MENU
#
# Revision 4.2  1993/03/25  12:20:46  kas
# *** empty log message ***
#
# Revision 4.1  1993/03/25  11:06:21  kas
# *** empty log message ***
#
# Revision 3.1  1993/03/24  16:42:05  kas
# *** empty log message ***
#
# Revision 1.4  1993/03/23  08:09:37  kas
# Исправлена ошибка в перечне OBJ-файлов
#
# Revision 1.3  1993/03/19  15:05:55  sev
# Убрана vclib и добавлены два новых фалйа, которыее были в vclib
#
# Revision 1.2  1993/03/15  13:47:31  sev
# Добавлен новый файл selsetio.c
#
# Revision 1.1  1993/03/12  15:44:23  sev
# Init
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
	edprtl.o          edputch.o edsignal.o\
	edstat.o edsys.o edupda.o edvmark.o edwopen.o f_to_b.o loadfile.o\
	system.o wrkselset.o match.o crtlstfl.o

HFILES =	edextern.h	edextfun.h	edinitstr.h	edkey.h\
		edstruct.h	vced.h

edi:	$(OBJS)
	$(CC) -o edi $(CFLAGS) $(OBJS) -lvced -lvcdbms -lvc -lx

$(OBJS):	$(HFILES)

.c.o:
	     $(CC) -c $(CFLAGS) -I/usr/vcmu/include $<

