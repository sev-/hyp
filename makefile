#
#  $Id: makefile,v 1.2 1993/03/19 14:57:16 sev Exp $
#
#
#  $Log: makefile,v $
#  Revision 1.2  1993/03/19 14:57:16  sev
#  Я удалил vclib
#
# Revision 1.1  1993/03/04  10:43:52  sev
# Initial revision
#
#
CFLAGS		= -Ox

OFILES		= edit_str.o hyp.o lib_hyp.o print_pg.o search.o

hyp:		$(OFILES)
	 	rcc $(CFLAGS) -o hyp $(OFILES) -lvc
.c.o:
	 	rcc $(CFLAGS) -c -I /usr/vcmu/include $<

$(OFILES):	hyp.h
