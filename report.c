/*
 *  $Id: report.c,v 1.1 1993/03/06 11:50:31 sev Exp $
 *
 * ---------------------------------------------------------- 
 *
 * $Log: report.c,v $
 * Revision 1.1  1993/03/06 11:50:31  sev
 * Initial revision
 *
 *
 */

static char rcsid[]="$Id: report.c,v 1.1 1993/03/06 11:50:31 sev Exp $";

/* Last modification  27 Фев Сб  16:44:05 kiev 1993 */
/*
	Файл report.c
	Запорожье 1993.
*/

#include <stdio.h>

#define PATHLEN 256

typedef struct FILESTACK	/* стэк файлов */
{
  char			 path[32];
  long			 position;	/* текущий offset в файле */
  int			 pos_in_string;	/* последняя обработанная колонка */
  struct FILESTACK	*previous;
} FILESTACK;

typedef struct COMPILED		/* список обработанных файлов */
{
  char			 path[32];
  struct COMPILED	*next;
} COMPILED;

COMPILED  *head_comp;
COMPILED  *curr_comp;
FILESTACK *current;

void	 no_conf();
void	 go_conf();
void	 report();
void	 free_stack();
char	*find();
char	*make_name();
void	 newfile();
int	 oldfile();
char	*parser();


main(argc, argv)
int	argc;
char  **argv;
{
  int carg;

  if(argc < 2)
  {
    printf("Построитель отчетов для гипертекста. Версия 1.0\n\n\
 Использование:\n\
%s [-c] file ...\n\
\t, где\nfile - файл для компиляции.\n\
\t-c  - если этот ключ стоит перед именем файла, то из его первой строки\n\
\t      берется имя файла для обработки, при этом ко всем позже встретив-\n\
\t      шимся файлам добавляется путь к нему.\n\
%s генерирует файл с именем *.rep, где * - имя входного файла.\n\n"
, argv[0], argv[0]);
    return 0;
  }

  carg = 1;
  while(carg < argc)
  {
    if(!strcmp(argv[carg], "-c"))
      if(++carg < argc)
	go_conf(argv[carg]);
      else
	puts("Ошибочный ключ -c.");
    else
      no_conf(argv[carg]);
    ++carg;
  }

  return 0;
}

void no_conf(filename)	/* входной файл не имеет структуру conf.hyp */
char *filename;
{
  FILE *report_file;
  FILE *curr_file;
  char *name;

  if((curr_file = fopen(filename, "r")) == (FILE *)NULL)
  {
    printf("Не могу открыть файл %s для чтения.\n", filename);
    return;
  }

  name = make_name(filename);	/* добавление .rep к имени файла */

  if((report_file = fopen(name, "w")) == (FILE *)NULL)
  {
    printf("Не могу открыть файл %s для записи в него отчета.\n", name);
    fclose(curr_file);
    return;
  }

  fclose(curr_file);
  fclose(report_file);

  report(filename, name, (char *)NULL);		/* NULL - путь к файлу */

  free_stack();
}

void go_conf(filename)	/* первая строка в файле - имя для обработки */
char *filename;
{
  FILE *conf_file;
  FILE *report_file;
  FILE *curr_file;
  char *begin_file;
  char *name;
  char  path[PATHLEN];

  if((conf_file = fopen(filename, "r")) == (FILE *)NULL)
  {
    printf("Не могу открыть файл %s для чтения.\n", filename);
    return;
  }

  fgets(path, PATHLEN, conf_file);	/* берем имя файла */

  path[strlen(path)-1] = '\0';	/* убираем \n */
  if((curr_file = fopen(path, "r")) == (FILE *)NULL)
  {
    printf("Неправильный файл %s.\n", filename);
    fclose(conf_file);
    return;
  }

  fclose(conf_file);

  name = make_name(filename);

  if((report_file = fopen(name, "w")) == (FILE *)NULL)
  {
    printf("Не могу открыть файл %s для записи в него отчета.\n", name);
    fclose(curr_file);
    return;
  }

  fclose(curr_file);
  fclose(report_file);

  begin_file = (char *)strrchr(path, '/');	/* выделние пути и имени */
  *(begin_file ++) = '\0';

  report(begin_file, name, path);

  free_stack();
}

void report(filein, fileout, path)	/* функция обработки файла filein */
char *filein;
char *fileout;
char *path;
{
  FILE *in;
  FILE *out;
  char  buf[1024];
  char  tmp[PATHLEN];
  long  position;
  int   numstring;	/* номер строки */
  int   numstring1;
  char *where;
  char  name[256];
  int   x;
  char *p;
  char  tmp1[256];

  out = fopen(fileout, "w");

  current = (FILESTACK *)malloc(sizeof(FILESTACK));
  current->previous = (FILESTACK *)NULL;

  strcpy(current->path, filein);
  current->position = 0;
  current->pos_in_string = 0;

  curr_comp = (COMPILED *)malloc(sizeof(COMPILED));
  head_comp = curr_comp;
  curr_comp->next = (COMPILED *)NULL;

  strcpy(curr_comp->path, filein);

loop:

  if(path != (char *)NULL)	/* сделаем имя файла */
    sprintf(tmp1, "%s/%s", path, current->path);
  else
    strcpy(tmp1, current->path);

  fputs(tmp1, stdout);
  fputc('\n', stdout);

  in = fopen(tmp1, "r");

  fputs("Файл: ", out);
  fputs(tmp1, out);	/* в файл */
  fputc('\n', out);

  numstring = 0;

  while(fgets(buf, 1024, in))	/* вывод в файл имен всех сегментов и полей */
  {
    position = ftell(in);
    if(where = find(buf, "\033<"))	/* нашли начало поля */
    {
      /* структура поля : \033<dd;dd;segment;file>
 		,где segment - имя сегмента,
		 file - имя файла, содержащего этот сегмент */
 
       sscanf(where, "%d-%d;%s", &x, &x, tmp);	/* существенно только имя */
       sscanf((char *)strchr(tmp, ';'), ";%s", name);
       p = (char *)strchr(name, '>');
       *p = '\0';
       p = (char *)strchr(tmp, ';');
       *p = '\0';
       sprintf(tmp1, "Поле: строка %u файл %s сегмент %s\n", numstring, name, tmp);
       fputs(tmp1, out);	/* это в файл */
    }
    /* нет ли в этой строке начала сегмента ? */
    if(where = find(buf, "\033("))	/* есть */
    {
      numstring1 = numstring;

      strcpy(tmp, where);
					/* ищем его конец */
      while(!(where = find(buf, "\033)")) && fgets(buf, 1024, in))
	numstring1 ++;	/* считаем строки */

      sprintf(buf, "Сегмент: с %u по %u строку. Имя: %s", numstring, numstring1, tmp);
      fputs(buf, out);

      fseek(in, position, SEEK_SET);	/* станем на прежнее место */
    }
    numstring ++;
  }

  fputs("Конец файла\n\n", out);

loop1:

  fseek(in, current->position, SEEK_SET);
		/* второй проход. Теперь ищутся ссылки на файлы */
  fgets(buf, 1024, in);

  if((where = find(buf + current->pos_in_string, "\033<")) && !oldfile(where))
  {
    current->pos_in_string = where - buf;	/* сначала в строке, на */
    newfile(current, where);			/* которой закончили */
    fclose(in);
    goto loop;	/* откроем новый файл */
  }

  position = ftell(in);
				/* теперь в остатке файла */
  while(!(where = find(buf, "\033<")) && fgets(buf, 1024, in))
    position = ftell(in);

  if(where && !oldfile(where))	/* если файл не встречался раньше */
  {
    current->position = position;
    current->pos_in_string = where - buf;
    newfile(current, where);
    fclose(in);
    goto loop;
  }

  if(current->previous != (FILESTACK *)NULL)	/* перейдем к предыдущему, */
  {						/* если есть 		   */
    current = current->previous;
    fclose(in);

    if(path != (char *)NULL)
      sprintf(tmp1, "%s/%s", path, current->path);
    else
      strcpy(tmp1, current->path);

    in = fopen(tmp1, "r");
    fseek(in, current->position, SEEK_SET);
    goto loop1;		/* уже не будем смотреть его поля и сегменты */
  }

  fclose(in);
  fclose(out);	/* конец report() */
}

int oldfile(string)	/* пробегает по списку файлов */
char *string;		/* в поисках такого же имени  */
{
  COMPILED  *tmpc;
  char	    *name;

  name = parser(string);
  tmpc = head_comp;
  while(strcmp(tmpc, name) && tmpc->next != (COMPILED *)NULL)
    tmpc = tmpc->next;

  if(!strcmp(tmpc, name))	/* этот файл уже обрабатывали */
    return 1;

  return 0;
}  

void newfile(old, string)	/* выделяет память для структуры и связывает */
FILESTACK *old;			/* ее со списком */
char	  *string;
{
  FILESTACK *tmph;
  COMPILED  *tmpc;
  char	    *name;

  name = parser(string);	/* выделим имя файла */

  tmph = (FILESTACK *)malloc(sizeof(FILESTACK));	/* теперь память */

  tmph->previous = old;		/* свяжем его */

  strcpy(tmph->path, name);

  tmph->position = 0;
  tmph->pos_in_string = 0;

  current = tmph;

  tmpc = (COMPILED *)malloc(sizeof(COMPILED));	/* то же для списка обработанных */
  tmpc->next = (COMPILED *)NULL;

  strcpy(tmpc->path, name);
  curr_comp->next = tmpc;

  curr_comp = tmpc;
}

char *parser(string)	/* выделяет из строки вида dd;dd;string;string> */
char *string;		/* последнюю строку */
{
  int	     x;
  char	     b[PATHLEN];
  char	     name[PATHLEN];
  char	    *p;
  
  sscanf(string, "%d-%d;%s", &x, &x, b);
  sscanf((char *)strchr(b, ';'), ";%s", name);
  p = (char *)strchr(name, '>');
  *p = '\0';

  return name;
}

void free_stack()	/* освобождает память, выделенную под структуры */
{
  FILESTACK *tmpf;
  COMPILED  *tmpc;
  int numfiles = 0;

  while(current->previous != (FILESTACK *)NULL)
  {
    tmpf = current->previous;
    free(current);
    current = tmpf;
  }
  free(current);
  
  while(head_comp->next != (COMPILED *)NULL)
  {
    tmpc = head_comp->next;
    free(head_comp);
    head_comp = tmpc;
    numfiles ++;
  }
  numfiles ++;
  free(head_comp);

  printf("Обработано файлов: %d.\n", numfiles);
}

char *find(string, pattern)	/* поиск подстроки. Возвращает указатель на */
char *string;			/* конец найденной подстроки или 0 */
char *pattern;
{
  char *p;

  p = pattern;

  while(*p && *string)
  {
    if(*string++ != *p++)
      p = pattern;
  }
  if(!*p)
    return string;
  else
    return 0;
}

char *make_name(name)	/* добавляет к name .rep с контролем за длиной */
char *name;
{
  char buf[PATHLEN];

  if(strlen(&name[strrchr(name, '/')]) > 9)
  {
    printf("Предупреждение: имя %s слишком длинное\n", name);
    sprintf(buf, "%s", name);
    strcpy(&buf[strlen(buf)-4], ".rep");
  }
  else
    sprintf(buf, "%s.rep", name);

  return buf;
}
