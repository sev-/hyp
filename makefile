#
#  $Id: makefile,v 1.1 1993/09/14 16:45:50 sev Exp $
#
# $Log: makefile,v $
# Revision 1.1  1993/09/14 16:45:50  sev
# Initial revision
#
#

CFLAGS =

CC = rcc

OBJS =  drivesel.o shell.o wrkselset.o
HFILES =  shell.h global.h proto.h

shell:	$(OBJS)
	$(CC) -o shell $(CFLAGS) $(OBJS) -lvced -lvcdbms -lvc -lx

$(OBJS):	$(HFILES)

.c.o:
	     $(CC) -c $(CFLAGS) -I/usr/vcmu/include $<


clean:
	rm -f *.[ob] shell core
