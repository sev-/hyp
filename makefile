#
#  $Id: makefile,v 1.1 1993/03/06 11:45:54 sev Exp $
#
# $Log: makefile,v $
# Revision 1.1  1993/03/06 11:45:54  sev
# Initial revision
#
#


CFLAGS	= -Ox
CC	= rcc

SOURCES	= compiler.c

compiler:
	$(CC) $(CFLAGS) -o compiler $(SOURCES)

compiler: $(SOURCES)
