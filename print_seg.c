/*
 * $Id: print_seg.c,v 1.4 1995/12/25 11:46:14 sev Exp $
 * 
 * ----------------------------------------------------------
 * 
 * $Log: print_seg.c,v $
 * Revision 1.4  1995/12/25 11:46:14  sev
 * Added pri segment command
 *
 * Revision 1.3  1995/12/05  15:01:48  sev
 * I tryed to add "pri file" command, but failed
 *
 * Revision 1.2  1994/11/12  19:20:53  sev
 * Indented and added gzip
 * Revision 1.1  1994/03/05  21:56:56  sev Initial
 * revision
 * 
 * Revision 1.3  1993/05/22  12:03:07  sev Исправлена ошибка при записи
 * при отказе было перелистывание экрана)
 * 
 * Revision 1.2  1993/04/22  15:42:07  sev Delete some errors
 * 
 * Revision 1.1  1993/04/22  15:33:35  sev Initial revision
 * 
 * 
 */

#include "hyp.h"

void write_segment(infile)
char *infile;
{
  char name[80];
  int stop = 0;
  int i;
  long old_position;
  int more = 1;
  char *ch;
  FILE *outfile;
  WPTR w;

  old_position = end_super;
  end_super = atol(buf_pg[col_str] + 1);
  read_screen(infile, end_super);
  if (old_position == end_super)
    end_super = old_position = 0;
  else
    old_position = end_super;

  pg_open = pg_close = 0;

  name[0] = 0;

  if (!edit_string(10, 10, 70, 79, name, " Введите имя файла ", ATTR_B, ATTR_F))
    return;

  if ((outfile = fopen(name, "w")) == (FILE *) NULL)
  {
    ask_msg("Не могу открыть файл", 0);
    return;
  }

  w = message ("Записываю...");
  while (!stop && more)
  {
    if (!(more = read_screen(infile, end_super)))
      continue;

    for (i = 0; i < more; i++)
    {
      if ((ch = strchr(buf_pg[i], '\033')) != (char *) NULL)
      {
	*ch = 0;
	trim(buf_pg[i]);
	strcat(buf_pg[i], "\n");
      }

      fputs(buf_pg[i], outfile);
    }
    if (pg_open == pg_close)
      stop = 1;
  }

  end_super = old_position;
  fclose(outfile);

  wclose (w);
  ask_msg("Файл записан успешно", 0);
}

char template1[] = "/tmp/hyp_pXXXXXX";

void pri_segment(infile)
char *infile;
{
  char s[80];
  int stop = 0;
  int i;
  long old_position;
  int more = 1;
  char *ch;
  FILE *outfile;
  WPTR w;

  if(template1[12] == 'X')
    mktemp(&template1);

  old_position = end_super;
  end_super = atol(buf_pg[col_str] + 1);
  read_screen(infile, end_super);
  if (old_position == end_super)
    end_super = old_position = 0;
  else
    old_position = end_super;

  pg_open = pg_close = 0;

  if ((outfile = fopen(template1, "w")) == (FILE *) NULL)
  {
    ask_msg("Не могу открыть файл", 0);
    return;
  }

  w = message ("Записываю...");
  while (!stop && more)
  {
    if (!(more = read_screen(infile, end_super)))
      continue;

    for (i = 0; i < more; i++)
    {
      if ((ch = strchr(buf_pg[i], '\033')) != (char *) NULL)
      {
	*ch = 0;
	trim(buf_pg[i]);
	strcat(buf_pg[i], "\n");
      }

      fputs(buf_pg[i], outfile);
    }
    if (pg_open == pg_close)
      stop = 1;
  }

  end_super = old_position;
  fclose(outfile);

  wclose (w);

  sprintf (s, "pri %s;rm %s", template1, template1);

  system (s);

  refresh ();
}
