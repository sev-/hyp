/*
 * $Id: hyp.c,v 1.8 1995/03/30 13:52:39 sev Exp $
 * 
 * ----------------------------------------------------------
 * 
 * $Log: hyp.c,v $
 * Revision 1.8  1995/03/30 13:52:39  sev
 * Added progress indicator
 *
 * Revision 1.7  1995/01/12  12:24:01  sev
 * cd RCS
 * gzip -9 *&
 *
 * Revision 1.6  1994/12/16  11:01:07  sev
 * changes for network
 *
 * Revision 1.5  1994/11/12  19:20:53  sev
 * Indented and added gzip
 * Revision 1.4  1993/04/22  15:33:35  sev Добавлена еще одна
 *  запись сегмента в файл
 * 
 * Revision 1.3  1993/03/14  18:04:21  sev Замечена маленькая ошибка, но если бы эта
 * то диагностическое сообщение было бы неправильно
 * 
 * 
 * Revision 1.2  1993/03/05  17:04:23  sev теперь перелистывание страниц по сегментам
 * 
 * 
 * Revision 1.1  1993/03/04  09:50:50  sev Initial revision
 * 
 * 
 */

static char rcsid[] = "$Id: hyp.c,v 1.8 1995/03/30 13:52:39 sev Exp $";

/*
 * Файл hyp.c Запорожье 1993-1995.
 */

#include "hyp.h"
#include <string.h>


main(argc, argv)		  /* Передается имя файла, где содержатся	 */
int argc;			  /* имя начального файла и адрес сегмента	 */
char *argv[];
{
  FILE *infile;			  /* текущий файл				 */
  FILE *conffile;		  /* файл командной строки		 */
  FILE *tempfile;

  int key;
  int more = 1;			  /* more = 0 выводится новая страница	 */
  int current;
  int col_seg = 0;		  /* счетчик стека			 */
  int len, i;
  int home = 0;			  /* флаг нажатия HOME_KEY		 */
  int pg_up = 0;		  /* флаг нажатия PG_UP			 */
  int good = 1;			  /* good = 0 если в странице нет ни одного поля */

  char *work_chr;
  char *ch;
  char work_char[BUF];
  char work_ch[BUF];
  char *tmp;

  char dir[PATH];		  /* каталог файлов гипертекста	 */
  char name_all[PATH + 18];	  /* полное имя файла		 */
  char current_file[PATH];	  /* Текущий файл		 */
  char beg_adr[45];

  struct
  {				  /* Структура стека	 */
    long adr_pg[N_STACK];	  /* offset страничы в файле	 */
    char adr_seg[N_STACK][40];	  /* адрес сегмента		 */
    char file[N_STACK][PATH];	  /* имя файла			 */

    int row[N_STACK];		  /* строка ипозиция поля		 */
    int col_1[N_STACK];		  /* по которому вышли		 */

    int _open[N_STACK];		  /* количество открытых и	 */
    int _close[N_STACK];	  /* закр. скобок на момент	 */
    /* выхода			 */
  } stack;

  /*----------------------------------------------------------------------*/
  /* конец обьявления переменных			 */
  /*----------------------------------------------------------------------*/


  /*------------------------------------------------------------------------*/
  /* разбор ком. строки и считывание имени начального файла иадреса сегмента */
  /*------------------------------------------------------------------------*/

  if (argc < 2)
  {
    puts("Просмотрщик гипертекста. Версия 2.3\n\n\
 Использование:\n\t\thyp file\n");
    exit(1);
  }
  if ((conffile = fopen(argv[1], "r")) == (FILE *) NULL)
  {
    printf(" Ошибка :не могу открыть файл %s \n", argv[1]);
    exit(1);
  }
  fgets(path_file, PATH, conffile);
  fgets(adress, 40, conffile);
  fclose(conffile);

  tmp = strchr(path_file, '\n');
  if (tmp == (char *) NULL)
  {
    printf("ошибка в файле %s (путь к файлу).\n", argv[1]);
    return 1;
  }
  *tmp = '\0';
  tmp = strchr(adress, '\n');
  if (tmp == (char *) NULL)
  {
    printf("ошибка в файле %s (начальный сегмент).\n", argv[1]);
    return 1;
  }
  *tmp = '\0';

  /*-------------------------------------------------------------------------*/
  /* определяется имя каталога содержащий файлы гипертекста		   */
  /*-------------------------------------------------------------------------*/

  while ((ch = strchr(path_file, '\\')) != (char *) NULL)
    *ch = '/';
  empty(dir, PATH);
  if ((ch = strrchr(path_file, '/')) != 0)
  {
    strncpy(dir, path_file, strlen(path_file) - strlen(ch));
    strcpy(path_file, ch + 1);
  }
  else
    getcwd(dir, PATH);
  trim(dir);
  /*----------------------------------------------------------------------*/
  /* определеются первые текущие параметры				 */
  /*----------------------------------------------------------------------*/

  sprintf(name_all, "%s/%s", dir, path_file);
  if ((infile = datfopen(name_all, "r")) == (FILE *) NULL)
  {
    printf("Ошибка открытия файла %s\n", name_all);
    exit(1);
  }
  strcpy(current_file, path_file);
  strcpy(current_seg, adress);
  sprintf(beg_adr, "\033(%s\0", adress);
  end_super = fpos(infile, beg_adr, 0l);

  vcstart(CLRSCRN);
  vckeyrdy = 0;
  wdo1 = wxopen(0, 0, 23, 79, (char *) NULL, ACTIVE + CURSOR + COOKED + NOADJ, 0, 0);
  init_help();

  /*-------------------------------------------------------------------------*/
  /* на каждом шаге этого цикла происходит перерисовка окна(страницы, HOME) */
  /*-------------------------------------------------------------------------*/
  while (1)
  {
    /*-----------------Открытие файла по HOME---------------------------*/

    if (strcmp(current_file, path_file) != 0 && strchr(path_file, '*') == (char *) NULL)
    {
      datfclose(infile);
      sprintf(name_all, "%s/%s", dir, path_file);
      if ((infile = datfopen(name_all, "rt")) == (FILE *) NULL)
      {
	vcend(CLOSE);
	printf("ГИПЕРТЕКСТ: Ошибка открытия файла %s \n", name_all);
	exit(1);
      }
      strcpy(current_file, path_file);
    }
    /*---------------------------------------------------------------------*/
    fseek(infile, 0l, SEEK_SET);
    more = 1;
    stack.adr_pg[col_seg] = end_super;
    woff();
    erase();
    xatsay(22, 40 - strlen(help_string) / 2, help_string, ATTR_B);
    good = read_print_pg(infile, end_super, home, pg_up, wdo1);
    won();

    home = pg_up = 0;

    while (more)
    {
      strcpy(work_char, chr);
      if (good == 1)
	selbar(wdo1, row, col_1, ATTR_B, col_2 - col_1 + 1);
      key = getone();

      /*-- если нет полей в странице то блокируются клавиши -----------*/

      if (good == 0 && (key == GO_TREE || key == MOVE_UP_KEY ||
			key == MOVE_DOWN_KEY || key == MOVE_LEFT_KEY ||
			key == MOVE_RIGHT_KEY))
	continue;
      /*-----------------------------------------------------------------*/
      switch (key)
      {
	case HELP_KEY:
	  help_msg();
	  break;
	case MOVE_RIGHT_KEY:
	  if ((chr = strstr(chr, "<")) != (char *) NULL)
	  {
	    selbar(wdo1, row, col_1, ATTR_F, col_2 - col_1 + 1);
	    string(&col_1, &col_2, adress, path_file);
	  }
	  else
	    ungetone(MOVE_DOWN_KEY);
	  break;
	case MOVE_LEFT_KEY:
	  ch = chr;
	  empty(work_char, BUF);
	  len = strlen(buf_pg[row]) - strlen(chr);
	  strncpy(work_char, buf_pg[row], len);
	  trim(work_char);

	  chr = strrchr(work_char, '\033');
	  chr--;

	  if (*chr == '>')
	  {
	    len = strlen(work_char) - strlen(chr++);
	    empty(work_char, BUF);
	    strncpy(work_char, buf_pg[row], len + 1);
	    trim(work_char);
	    chr = strrchr(work_char, '<');
	    strcpy(work_ch, chr);

	    selbar(wdo1, row, col_1, ATTR_F, col_2 - col_1 + 1);
	    string(&col_1, &col_2, adress, path_file);
	    len = strlen(work_char) - strlen(chr);
	    strncpy(work_char, work_char, len);
	    chr = &buf_pg[row][strlen(work_char)];
	    strcpy(work_char, chr);
	  }
	  else
	  {
	    chr = ch;
	    ungetone(MOVE_UP_KEY);
	  }
	  break;
	case MOVE_DOWN_KEY:
	  current = row;
	  for (i = row + 1; i <= col_str; i++)
	    if ((work_chr = strstr(buf_pg[i], "\033<")) != (char *) NULL)
	    {
	      selbar(wdo1, current, col_1, ATTR_F, col_2 - col_1 + 1);
	      chr = work_chr + 1;
	      strcpy(work_char, chr);
	      string(&col_1, &col_2, adress, path_file);
	      current = row = i;
	      break;
	    }
	  break;
	case MOVE_UP_KEY:
	  current = row;
	  for (i = row - 1; i >= 0; i--)
	    if ((work_chr = strstr(buf_pg[i], "\033<")) != (char *) NULL)
	    {
	      selbar(wdo1, current, col_1, ATTR_F, col_2 - col_1 + 1);
	      chr = work_chr + 1;
	      strcpy(work_char, chr);
	      string(&col_1, &col_2, adress, path_file);
	      current = row = i;
	      break;
	    }
	  break;
	case PAGE_DOWN_KEY:
	  if ((level <= 0))
	    break;
	  for (i = 0; i <= col_str; i++)
	    strcpy(buf_pg[i], "\0");
	  more = 0;
	  strcpy(path_file, current_file);
	  break;
	case PAGE_UP_KEY:
	  if ((strstr(buf_pg[0], beg_adr)) != 0)
	  {
	    more = 1;
	    break;
	  }
	  end_super = atol(buf_pg[col_str] + 1);
	  for (i = 0; i <= col_str; i++)
	    strcpy(buf_pg[i], "\0");
	  strcpy(path_file, current_file);
	  pg_up = 1;
	  more = FALSE;
	  _open -= pg_open;
	  _close -= pg_close;
	  break;
	case HOME_FILE_KEY:
	  find_begin_seg(infile);
	  for (i = 0; i <= col_str; i++)
	    strcpy(buf_pg[i], "\0");
	  strcpy(path_file, current_file);
	  more = 0;
	  break;
	case END_FILE_KEY:
	  if (level <= 0)
	    break;
	  find_end_seg(infile);
	  for (i = 0; i <= col_str; i++)
	    strcpy(buf_pg[i], "\0");
	  strcpy(path_file, current_file);
	  more = 0;
	  pg_up = 1;
	  break;
	case FORWARD_SEARCH_KEY: /* search forward */
	  forward_search(infile);
	  strcpy(path_file, current_file);
	  more = 0;
	  break;
	case BACKWARD_SEARCH_KEY:/* backward search */
	  backward_search(infile);
	  strcpy(path_file, current_file);
	  more = 0;
	  break;
	case PRINT_SEGMENT:	  /* print current segment */
	  print_segment(infile);
	  strcpy(path_file, current_file);
	  more = 0;
	  break;
	case GO_TREE:
	  if (strchr(path_file, '*') != (char *) NULL)
	    strcpy(path_file, current_file);
	  else if (strcmp(current_file, path_file) != 0)
	  {
	    sprintf(name_all, "%s/%s", dir, path_file);
	    if ((tempfile = datfopen(name_all, "rt")) == (FILE *) NULL)
	    {
	      sprintf(work_char, "Информация временно отсуствует(%s)", path_file);
	      ask_msg(work_char, 0);
	      break;
	    }
	    else
	    {
	      datfclose(infile);
	      infile = tempfile;
	    }
	  }
	  sprintf(work_char, "\033(%s", adress);
	  if ((end_super = fpos(infile, work_char, 0l)) == -1l)
	  {
	    ask_msg("Информация временно отсуствует.", 0);
	    break;
	  }
	  stack._open[col_seg] = _open;
	  stack._close[col_seg] = _close;
	  stack.row[col_seg] = row;
	  stack.col_1[col_seg] = col_1;
	  strcpy(stack.adr_seg[col_seg], current_seg);
	  strcpy(stack.file[col_seg], current_file);

	  sprintf(beg_adr, "\033(%s", adress);
	  strcpy(current_file, path_file);
	  strcpy(current_seg, adress);
	  col_seg++;
	  _open = _close = level = 0;
	  more = 0;
	  break;
	case TREE_UP_KEY:
	  col_seg--;
	  if (col_seg == -1)
	  {
	    col_seg = 0;
	    break;
	  }
	  _open = stack._open[col_seg];
	  _close = stack._close[col_seg];
	  row = stack.row[col_seg];
	  col_1 = stack.col_1[col_seg];
	  end_super = stack.adr_pg[col_seg];
	  strcpy(current_seg, stack.adr_seg[col_seg]);
	  sprintf(beg_adr, "\033(%s", current_seg);
	  strcpy(path_file, stack.file[col_seg]);
	  home = 1;
	  more = 0;
	  break;
	case 'q':
	case 'Q':
	case 'я':
	case 'Я':
	  if (!ask_msg("Вы уверены что хотите выйти из ГИПЕРТЕКСТA ?.", 1))
	    break;
	  datfclose(infile);
	  wclose(wdo1);
	  vcend(NOCLOSE);
	  system("clear");
	  exit(0);
	  break;
	default:
	  bell();
	  break;
      }
    }
  }
}
