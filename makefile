#
#  $Id: makefile,v 1.9 1995/03/24 16:16:10 sev Exp $
#
#
#  $Log: makefile,v $
#  Revision 1.9  1995/03/24 16:16:10  sev
#  added good RCS feature
#
# Revision 1.8  1994/11/12  19:20:53  sev
# Indented and added gzip
#
# Revision 1.7  1994/03/05  21:56:56  sev
# o
#
# Revision 1.6  1993/09/16  15:12:06  sev
# Добавлен \
#
# Revision 1.5  1993/09/13  11:37:04  sev
# Работаем в сети и без проблем (благодаря unix.c и getone.c)
#
# Revision 1.4  1993/08/25  12:40:46  sev
# проба
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
CC = gcc

CFLAGS = -O

CFILES = edit_str.c gunzip.c hyp.c lib_hyp.c print_seg.c print_pg.c search.c
HFILES = hyp.h

OFILES = edit_str.o gunzip.o hyp.o lib_hyp.o print_seg.o print_pg.o search.o

hyp:	   $(OFILES)
	   gcc $(CFLAGS) -o hyp $(OFILES) -lvc

$(OFILES): $(HFILES)

clean:
	/bin/rm *.[ob~]
	strip hyp
	ci $(CFILES) $(HFILES) makefile
	co -l makefile
	gzip -9 RCS/*

$(CFILES) $(HFILES): RCS/hyp.c,v
	co -l $(CFILES) $(HFILES)

RCS/hyp.c,v:
	gunzip RCS/*.gz

