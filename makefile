#
#  $Id: makefile,v 1.2 1994/11/12 19:16:13 sev Exp $
#
# $Log: makefile,v $
# Revision 1.2  1994/11/12 19:16:13  sev
# Added gzip
#
# Revision 1.1  1993/03/06  11:45:54  sev
# Initial revision
#
#


CFLAGS	= -O
CC	= gcc

OBJS	= compiler.o gzip.o

compiler: $(OBJS)
	$(CC) $(CFLAGS) -o compiler $(OBJS)

lastcom: lastcom.c
	$(CC) $(CFLAGS) -o lastcom lastcom.c

clean:
	/bin/rm -f *.[ob~] core
	strip compiler
	ci *.[ch] makefile
