#
#  $Id: makefile,v 1.1 1993/03/31 16:38:21 sev Exp $
#
# $Log: makefile,v $
# Revision 1.1  1993/03/31 16:38:21  sev
# Initial revision
#
#

CFLAGS	= -Ox
CC	= rcc

SOURCES	= refer.c

refer:
	$(CC) $(CFLAGS) -o refer $(SOURCES)

refer: $(SOURCES)
