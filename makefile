#
#  $Id: makefile,v 1.1 1993/03/06 11:50:31 sev Exp $
#
# $Log: makefile,v $
# Revision 1.1  1993/03/06 11:50:31  sev
# Initial revision
#
#

CFLAGS	= -Ox
CC	= rcc

SOURCES	= report.c

report:
	$(CC) $(CFLAGS) -o report $(SOURCES)

report: $(SOURCES)
