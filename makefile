#
#  $Id: makefile,v 1.3 1994/07/11 12:31:07 sev Exp $
#
# $Log: makefile,v $
# Revision 1.3  1994/07/11 12:31:07  sev
# *** empty log message ***
#
# Revision 1.2  1993/09/28  12:58:34  sev
# Добавлена куча всего
#
# Revision 1.1  1993/09/14  16:45:50  sev
# Initial revision
#
#

CFLAGS = -Ox

CC = rcc

OBJS =  drivesel.o edit_str.o lib.o menus.o shell.o wrkselset.o
CFILES =  drivesel.c edit_str.c lib.c menus.c shell.c wrkselset.c
HFILES =  shell.h global.h proto.h

shell:	$(OBJS)
	$(CC) -o shell $(CFLAGS) $(OBJS) -lvc

$(OBJS):	$(HFILES)

.c.o:
	     $(CC) -c $(CFLAGS) -I/usr/vcmu/include $<


clean:
	rm -f *.[ob] shell core
	ci $(HFILES) $(CFILES) makefile
