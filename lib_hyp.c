/*
 * $Id: lib_hyp.c,v 1.5 1995/01/12 12:24:01 sev Exp $
 * 
 * ----------------------------------------------------------
 * 
 * $Log: lib_hyp.c,v $
 * Revision 1.5  1995/01/12 12:24:01  sev
 * changed help
 *
 * Revision 1.4  1994/12/16  11:01:07  sev
 * changes for network
 *
 * Revision 1.3  1994/11/12  19:20:53  sev
 * Indented and added gzip
 * Revision 1.2  1993/04/22  15:33:35  sev Добавлена еще одна запись сегмента в файл
 * 
 * 
 * Revision 1.1  1993/03/04  09:50:57  sev Initial revision
 * 
 * 
 */

static char rcsid[] = "$Id: lib_hyp.c,v 1.5 1995/01/12 12:24:01 sev Exp $";

/*
 * Файл lib_hyp.c Запорожье 1992.
 */

#include "hyp.h"
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

/*----------------------------------------------------------------------*/
/* функция возвращает offset в файле начала сегмента                    */
/*----------------------------------------------------------------------*/

long fpos(file, mark, set)
FILE *file;
long set;
char *mark;			  /* адрес сегмента      */
{
  long ftell();
  char *strstr();
  char buf[BUF], dd[BUF];

  strcpy(dd, mark);
  fseek(file, set, SEEK_SET);

  do
    if (fgets(buf, BUF, file) == (char *) NULL)
      return (-1);
  while (strstr(buf, mark) == (char *) NULL);

  return (ftell(file) - strlen(buf));
}

/*----------------------------------------------------------------------*/
/* функция вызывается по ESC                                            */
/*----------------------------------------------------------------------*/

int ask_msg(msg, mode)
TEXT *msg;
int mode;
{
  int w;
  int len_msg, width, row_u, key, more = 1, y_n = 0;

  len_msg = strlen(msg);
  if (len_msg < 14)
    width = 12 + 5;
  else if (len_msg > 79)
    return (-1);
  else
    width = len_msg + 5;

  row_u = (80 - width) / 2;
  w = wxopen(10, row_u, 15, row_u + width, NULL, ACTIVE + BORDER + BD2 + SHADOW + NOADJ, 0, 0);

  xatsay(1, 2, msg, ATTR_F);
  switch (mode)
  {
    case 0:			  /* любая клавиша   */
      xatsay(2, (width - 23) / 2, " Нажмите любую клавишу ", ATTR_F);
      getone();
      y_n = 1;
      break;
    case 1:			  /* ДА или НЕТ   */
      xatsay(2, (width - 14) / 2, "  НЕТ    ДA  ", ATTR_F);
      while (more)
      {
	selbar(w, 2, (width - 14) / 2 + 7 * y_n, ATTR_B, 6);
	key = getone();
	switch (key)
	{
	  case CUR_RIGHT:
	    if (y_n == 0)
	    {
	      selbar(w, 2, (width - 14) / 2, ATTR_F, 6);
	      y_n = 1;
	    }
	    break;
	  case CUR_LEFT:
	    if (y_n == 1)
	    {
	      selbar(w, 2, (width - 14) / 2 + 7, ATTR_F, 6);
	      y_n = 0;
	    }
	    break;
	  case RET:
	    more = 0;
	    break;
	  case 'q':
	    more = 0;
	    y_n = 0;
	    break;
	}
      }
  }
  wclose(w);
  return (y_n);
}
char help_sm[] = " ПФ1 - помощь ";
char *help_sm_wind[] =
{
  "ПТ  - вверх по дереву.         ",
  "ВПС - предыдущая страница.     ",
  "ВСС - следующая страница.      ",
  "PT  - в начало.                ",
  "ВНТ - в конец.                 ",
  "/   - поиск вперед.            ",
  "?   - поиск назад.             ",
  "ПФ4 - запись текущего сегмента.",
  "q   - выход.                   "
};

char help_ws[] = " PF1 - помощь ";
char *help_ws_wind[] =
{
  "Find       - вверх по дереву.         ",
  "PrevScreen - предыдущая страница.     ",
  "NextScreen - следующая страница.      ",
  "InsertHere - в начало.                ",
  "Select     - в конец.                 ",
  "/          - поиск вперед.            ",
  "?          - поиск назад.             ",
  "PF4        - запись текущего сегмента.",
  "q          - выход.                   "
};

char help_ansi[] = " F1  - помощь ";
char *help_ansi_wind[] =
{
  "Home   - вверх по дереву.         ",
  "PgUp   - предыдущая страница.     ",
  "PgDn   - следующая страница.      ",
  "Insert - в начало.                ",
  "End    - в конец.                 ",
  "/      - поиск вперед.            ",
  "?      - поиск назад.             ",
  "F4     - запись текущего сегмента.",
  "q      - выход.                   "
};

char *help_string;		  /* строка в нижней части экрана */
char *help_window[25];		  /* строки в окне		 */

void init_help()		  /* установка помощи для каждого терминала */
{
  char *terminal;		  /* тип терминала		 */
  int i;

  terminal = getenv("TERM");

  if (strcmp(terminal, "sm7238") == 0)
  {
    help_string = help_sm;
    for (i = 0; i < sizeof(help_sm_wind) / sizeof(char *); i++)
      help_window[i] = help_sm_wind[i];
  }
  else if (strcmp(terminal, "ws685") == 0)
  {
    help_string = help_ws;
    for (i = 0; i < sizeof(help_sm_wind) / sizeof(char *); i++)
      help_window[i] = help_ws_wind[i];
  }
  else
  {
    help_string = help_ansi;
    for (i = 0; i < sizeof(help_sm_wind) / sizeof(char *); i++)
      help_window[i] = help_ansi_wind[i];
  }
  old_pattern[0] = '\0';
}

int help_msg()			  /* функция вызывается по F1 */
{
  int help_w;			  /* окно помощи			 */
  int i;

  help_w = wxopen(10 - sizeof(help_sm_wind) / sizeof(char *) / 2,
		  35 - strlen(help_window[0]) /2,
		  15 + sizeof(help_sm_wind) / sizeof(char *) / 2,
		  45 + strlen(help_window[0]) /2,
		  " Помощь ", ACTIVE + BORDER + BD1 + NOADJ, 0, 0);
  for (i = 0; i < sizeof(help_sm_wind) / sizeof(char *); i++)
    atsay(i + 1, 5, help_window[i]);
  i += 2;
  atsay(i, 9, "Нажмите любую клавишу");
  getone();
  wclose(help_w);
}

char template[] = "/tmp/hypXXXXXX";
int temporaryfile;

FILE *datfopen(fname, mode)
char *fname, *mode;
{
  FILE *in;

  if(template[10] == 'X')
    mktemp(&template);

  if((in = fopen(fname, "r")) == (FILE *)NULL)
    return (FILE *)NULL;
  if(fgetc(in) == 0x1f && fgetc(in) == 0x8b)	/* if gzipped */
  {
    fclose(in);
    gunzip(fname, template);
    temporaryfile = 1;
    return(fopen(template, mode));
  }
  temporaryfile = 0;
  rewind(in);

  return in;
}

void datfclose(file)
FILE *file;
{
  fclose(file);

  if(temporaryfile)
    remove(template);
}
