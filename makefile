
#  $Id: makefile,v 1.3 1995/03/24 16:20:49 sev Exp $
#
# $Log: makefile,v $
# Revision 1.3  1995/03/24 16:20:49  sev
# added good feature with RCS
#
# Revision 1.2  1994/11/12  19:16:13  sev
# Added gzip
#
# Revision 1.1  1993/03/06  11:45:54  sev
# Initial revision
#
#


CFLAGS	= -O
CC	= gcc

CFILES = compiler.c lastcom.c gzip.c
HFILES = gzip.h

OBJS	= compiler.o gzip.o

compiler: $(OBJS)
	$(CC) $(CFLAGS) -o compiler $(OBJS)

lastcom: lastcom.c
	$(CC) $(CFLAGS) -o lastcom lastcom.c

clean:
	/bin/rm -f *.[ob~] core
	strip compiler
	ci *.[ch] makefile
	co -l makefile
	gzip -9 RCS/*

$(CFILES) $(HFILES): RCS/compiler.c,v
	co -l $(CFILES) $(HFILES)

RCS/compiler.c,v:
	gunzip RCS/*.gz

