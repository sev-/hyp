#
#  $Id: makefile,v 1.7 1993/06/08 12:10:31 sev Exp $
#
# $Log: makefile,v $
# Revision 1.7  1993/06/08 12:10:31  sev
# Вот теперь этот глюк действительно убран
#
# Revision 1.6  1993/05/28  10:51:07  sev
# *** empty log message ***
#
# Revision 1.5  1993/05/24  15:14:24  sev
# Добавлен файл записи файлов в директорию сохранения
#
# Revision 1.4  1993/04/29  12:38:47  sev
# Работает удаление сегмента
#
# Revision 1.3  1993/04/20  16:04:12  sev
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

CFLAGS = -Ox

CC = rcc

OBJS =  drivesel.o edaskdir.o edback.o edconf.o\
	edconinf.o eddelcon.o eddelendl.o eddellin.o eddir.o eddisp.o\
	edend.o edexec.o edsvf.o edfnd.o edftostr.o\
	edhelp.o edhmark.o edhypend.o edinscon.o edinsline.o\
	edinsseg.o edit.o editor.o edmarkpg.o\
	edmesg.o edmove.o ednumseg.o edopen.o edpaste.o edprtb.o\
	edprtl.o edputch.o edsave.o edsignal.o\
	edstat.o edsys.o edupda.o edvmark.o edwopen.o f_to_b.o loadfile.o\
	system.o wrkselset.o match.o crtlstfl.o addrefer.o eddelseg.o

HFILES =	edextern.h	edextfun.h	edinitstr.h	edkey.h\
		edstruct.h	vced.h

edi:	$(OBJS)
	$(CC) -o edi $(CFLAGS) $(OBJS) -lvced -lvcdbms -lvc -lx

$(OBJS):	$(HFILES)

.c.o:
	     $(CC) -c $(CFLAGS) -I/usr/vcmu/include $<

