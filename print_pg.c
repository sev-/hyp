/*
 * $Id: print_pg.c,v 1.2 1994/11/12 19:20:53 sev Exp $
 * 
 * ----------------------------------------------------------
 * 
 * $Log: print_pg.c,v $
 * Revision 1.2  1994/11/12 19:20:53  sev
 * Indented and added gzip
 * Revision 1.1  1994/03/05  21:56:56  sev Initial revision
 * 
 * Revision 1.2  1993/03/05  17:04:23  sev теперь перелистывание страниц по сегментам
 * 
 * 
 * Revision 1.1  1993/03/04  09:51:07  sev Initial revision
 * 
 * 
 */

static char rcsid[] = "$Id: print_pg.c,v 1.2 1994/11/12 19:20:53 sev Exp $";

/*
 * Файл print_pg.c Запорожье 1992.
 */

#include "hyp.h"
#include <string.h>

/*------------------------------------------------------------------------*/
/* функция выводит страницу в окно , отмечает поля , возвращает 0 если нет */
/* ни одного поля в странице                                              */
/*------------------------------------------------------------------------*/

int read_print_pg(file, start, home, pg_up, w)
FILE *file;
long start;			  /* offset в файле начала  страницы */
int home;			  /* флаг нажатия HOME_KEY           */
int pg_up;			  /* флаг нажатия PG_UP              */
int w;
{
  int more;

  pg_open = pg_close = 0;
  more = read_screen(file, start);
  return print_pg(more, home, pg_up, w);
}

int print_pg(more, home, pg_up, w)
int more;
int home;			  /* флаг нажатия HOME_KEY           */
int pg_up;			  /* флаг нажатия PG_UP              */
int w;
{
  int i;
  int first;
  int good = 0;
  int c1, c2;
  int len;
  int row_up;
  int one = 2;
  char *p_buf;
  char *cc;
  char path[PATH];
  char adr[40];
  char buf[BUF], chr_hide[BUF], beg[45];

  sprintf(beg, "\033(%s", current_seg);
  first = home + 1;

  for (i = 0; i < more; i++)
  {
    empty(buf, 80);
    if ((cc = strchr(buf_pg[i], '\033')) != (char *) NULL)
    {
      len = strlen(buf_pg[i]) - strlen(cc);
      strncpy(buf, buf_pg[i], len);
      trim(buf);
      if (strlen(buf) >= 79)
      {
	empty(buf, 80);
	strncpy(buf, buf_pg[i], 79);
	trim(buf);
      }
    }
    else
    {
      if (strlen(buf_pg[i]) >= 79)
	strncpy(buf, buf_pg[i], 79);
      else
      {
	strcpy(buf, buf_pg[i]);
	*(strchr(buf, '\n')) = '\0';
      }
    }
    trim(buf);
    /*----------------------------------------------------------------------*/
    /* вывод строки с отмеченными полямии                */
    /*----------------------------------------------------------------------*/
    atsay(i, 0, buf);
    p_buf = &buf_pg[i][0];
    while ((chr = strstr(p_buf, "\033<")) != (char *) NULL)
    {
      good = 1;
      chr++;
      string(&c1, &c2, adr, path);
      selbar(w, i, c1, ATTR_F, c2 - c1 + 1);

      if (home == 1 && col_1 == c1 && row == i)	/*******************/
      {				  /* HOME aктивным де- */
	col_1 = c1;		  /* лается поле по  */
	col_2 = c2;		  /* которому раньше */
	strcpy(adress, adr);	  /* вышли         */
	row = i;		  /* */
	strcpy(path_file, path);  /* */
	strcpy(chr_hide, chr);	  /* */
      }				  /*******************/
      if (first == 1)
      {				  /*******************/
	col_1 = c1;		  /* aктивным делает- */
	col_2 = c2;		  /* ся первое поле  */
	strcpy(adress, adr);	  /* */
	row = i;		  /* */
	first++;		  /* */
	strcpy(path_file, path);  /* */
	strcpy(chr_hide, chr);	  /* */
      }				  /*******************/
      one = 1;
      p_buf = chr;
    }

    if (one == 1)
      row_up = i;
    one = 2;
  }
  if (pg_up == 1)
  {
    col_1 = c1;
    col_2 = c2;
    strcpy(adress, adr);
    row = row_up;
    if (good)
    {
      strcpy(path_file, path);
      strcpy(chr_hide, chr);
    }
  }
  if (home != 1 && pg_up != 1)
  {
    _open += pg_open;
    _close += pg_close;
  }
  chr = chr_hide;
  col_str = i;
  level = _open - _close;
  return (good);
}

/*------------------------------------------------------------------------*/
/* функция  разборки служебной информации для выделения полей        */
/*------------------------------------------------------------------------*/

int string(c_1, c_2, adr, path_f)
int *c_1, *c_2;
char *adr, *path_f;
{
  int i1, i2;
  char *ch, bb[70];

  ch = chr;
  sscanf(ch, "<%d-%d;%s>", &i1, &i2, bb);
  *(ch = strchr(bb, ';')) = '\0';
  strcpy(adr, bb);
  strcpy(path_f, ch + 1);
  *(strchr(path_f, '>')) = '\0';
  chr += (8 + strlen(adr) + strlen(path_f));
  *c_1 = i1;
  *c_2 = i2;
}

/*------------------------------------------------------------------------*/
/* функция  подсчета скобок                         */
/*------------------------------------------------------------------------*/

int end_or_beg(buf)
char *buf;
{
  char *ch;

  ch = buf;
  while ((ch = strstr(ch, "\033(")) != (char *) NULL)
  {
    pg_open++;
    ch++;
  }
  ch = buf;
  while ((ch = strstr(ch, "\033)")) != (char *) NULL)
  {
    pg_close++;
    ch++;
  }

}

int read_screen(file, start)
FILE *file;
long start;
{
  int i = 0, more = 1;
  char *end_f;

  fseek(file, start, SEEK_SET);

  while (more)
  {
    end_f = fgets(buf_pg[i], BUF, file);

    end_or_beg(buf_pg[i]);

    /*----------------------------------------------------------------------*/
    /* определение длины строки для вывода , отсечение служебной информации */
    /*----------------------------------------------------------------------*/
    if (strstr(buf_pg[i], "\014") != (char *) NULL || i >= 22
	|| end_f == (char *) NULL)
    {
      end_super = ftell(file);
      more = 0;
      continue;
    }
    i++;
  }
  return i;
}

void find_begin_seg(file)
FILE *file;
{
  char beg[80];
  int more, i;
  int stop = 0;
  long off;

  end_super = off = 0;

  sprintf(beg, "\033(%s", current_seg);

  while (!stop && (more = read_screen(file, end_super)))
  {
    for (i = 0; i < more; i++)
      if (strstr(buf_pg[i], beg) != (char *) NULL)
      {
	stop = 1;
	break;
      }
    if (!stop)
      off = end_super;
  }
  _open = _close = 0;
  end_super = off;
}

void find_end_seg(file)
FILE *file;
{
  long off;

  find_begin_seg(file);

  off = end_super;
  pg_open = pg_close = 0;

  while (read_screen(file, end_super) && (pg_open != pg_close))
    off = end_super;

  _open = _close = pg_open = pg_close = 0;

  end_super = off;
}
