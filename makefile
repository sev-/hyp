#
#  $Id: makefile,v 1.4 1993/08/25 12:40:46 sev Exp $
#
#
#  $Log: makefile,v $
#  Revision 1.4  1993/08/25 12:40:46  sev
#  проба
#
# Revision 1.3  1993/04/22  15:33:35  sev
# Добавлена еще одна функция - запись сегмента в файл
#
# Revision 1.2  1993/03/19  14:57:16  sev
# Я удалил vclib
#
# Revision 1.1  1993/03/04  10:43:52  sev
# Initial revision
#
#
CFLAGS		= -Ox

OFILES		= edit_str.o hyp.o lib_hyp.o print_seg.o print_pg.o search.o

hyp:		$(OFILES)
	 	rcc $(CFLAGS) -o hyp $(OFILES) ../getone.o ../unix.o -lvc
.c.o:
	 	rcc $(CFLAGS) -c -I /usr/vcmu/include $<

$(OFILES):	hyp.h
