/*
 * $Id: compiler.c,v 1.17 1995/06/30 12:12:27 sev Exp $
 * 
 * ----------------------------------------------------------
 * 
 * $Log: compiler.c,v $
 * Revision 1.17  1995/06/30 12:12:27  sev
 * Added -g key
 *
 * Revision 1.16  1995/03/30  13:29:58  sev
 * Added good feature in message (progress indicator)
 *
 * Revision 1.15  1995/03/24  16:08:03  sev
 * Added time checking (got from very old version)
 *
 * Revision 1.14  1995/03/02  14:16:45  sev
 * Added some changes to output message
 *
 * Revision 1.13  1995/02/06  14:14:27  sev
 * added verbose message
 *
 * Revision 1.12  1994/11/18  17:59:22  sev
 * Deleted NestLevel
 *
 * Revision 1.11  1994/11/15  14:40:06  sev
 * Some bugs hhave been deleted
 *
 * Revision 1.10  1994/11/12  19:16:13  sev
 * added gzip (may be some bugs)
 * Revision 1.9  1994/06/18  16:36:48  sev I was a stupid
 * idiot, when didn't delete a trace message
 * 
 * Revision 1.8  1994/06/18  15:16:15  sev Added date control (to avoid
 * unnecessary compilation)
 * 
 * Revision 1.7  1994/04/26  11:17:22  sev Another bug...
 * 
 * Revision 1.6  1994/04/26  11:12:08  sev Added -f flag
 * 
 * Revision 1.5  1993/06/08  12:09:42  sev Полностью измененная программа
 * 
 * 
 * 
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>

#define PATHLEN 256
#define BUF	1024

typedef struct FILESTACK	  /* стэк файлов */
{
  char path[64];
  long position;		  /* текущий offset в файле */
  int pos_in_string;		  /* последняя обработанная колонка */
  struct FILESTACK *previous;
} FILESTACK;

FILESTACK *current;
int fail = 0;
time_t last_modify;
char *compilefilename;
long compilefilesize;
int dont_gzip = 0;

void go_conf();
void refer();
void free_stack();
char *find();
int newfile();
int oldfile();
void parser();
void printprogress();
void nullfunc ();

main(argc, argv)
int argc;
char **argv;
{
  int carg;

  if (argc < 2)
  {
    puts("Компилятор гипертекста. Версия 2.5\n\n\
 Использование:\n\t\tcompiler [-g] [-f] file\n");
    return 1;
  }

  carg = 1;
  while (carg < argc)
  {
    if (!strcmp(argv[carg], "-f"))
    {
      while (++carg < argc)
	compile(argv[carg]);
      return 0;
    }

    if (!strcmp(argv[carg], "-g"))
    {
      dont_gzip = 1;
      carg++;
      continue;
    }

    go_conf(argv[carg]);
    ++carg;
    if (fail)
    {
      printf("Компиляция прошла не полностью!\n");
      fail = 0;
    }
    else
      printf("Компиляция прошла успешно!\n");
  }

  return 0;
}

void go_conf(filename)		  /* первая строка в файле - имя для обработки */
char *filename;
{
  FILE *conf_file;
  FILE *curr_file;
  char *begin_file;
  char path[PATHLEN];
  char str[2][80];

  if ((conf_file = fopen(filename, "r+")) == (FILE *) NULL)
  {
    printf("Не могу открыть файл %s для чтения/записи.\n", filename);
    fail = 1;
    return;
  }

  fgets(str[0], PATHLEN, conf_file);	/* берем имя файла */
  strcpy(path, str[0]);

  path[strlen(path) - 1] = '\0';  /* убираем \n */
  if ((curr_file = fopen(path, "r")) == (FILE *) NULL)
  {
    printf("Нет имени файла в %s\n", filename);
    fclose(conf_file);
    fail = 1;
    return;
  }

  fclose(curr_file);

  fgets(str[1], 80, conf_file);	  /* throw segment name */
  if (fscanf(conf_file, "compiled at: %ld", &last_modify) != 1)
  {
    printf("Нет отметки времени в %s\n", filename);
    fclose(conf_file);
    fail = 1;
    return;
  }
  fclose(conf_file);

  begin_file = (char *) strrchr(path, '/');	/* выделние пути и имени */
  *(begin_file++) = '\0';

  refer(begin_file, path);

  conf_file = fopen(filename, "w");
  fprintf(conf_file, "%s%scompiled at: %ld\n", str[0], str[1], time(NULL));
  fclose(conf_file);

  free_stack();
}

void refer(filein, path)	  /* функция обработки файла filein */
char *filein;			  /* имя входного файла _БЕЗ ПУТИ_  */
char *path;			  /* путь к входному файлу	  */
{
  FILE *in;
  char buf[1024];
  long position;
  char *where;
  char curr_file[256];		  /* Имя текущего файла	 */
  struct stat times[1];

  int gzipped;

  current = (FILESTACK *) malloc(sizeof(FILESTACK));
  current->previous = (FILESTACK *) NULL;

  strcpy(current->path, filein);
  current->position = 0;
  current->pos_in_string = 0;

loop:

  gzipped = 0;
  if (path != (char *) NULL)	  /* сделаем имя файла */
    sprintf(curr_file, "%s/%s", path, current->path);
  else
    strcpy(curr_file, current->path);

  in = fopen(curr_file, "r");
  if(fgetc(in) == 0x1f && fgetc(in) == 0x8b)	/* if gzipped */
    gzipped = 1;
  else
    rewind(in);

loop1:
  if(!gzipped)
  {
    fseek(in, current->position, SEEK_SET);
  /* Ищутся ссылки на файлы */
    fgets(buf, 1024, in);

    if ((where = find(buf + current->pos_in_string, "\033<")) && !oldfile(where))
    {
      current->pos_in_string = where - buf;	/* сначала в строке, на */
      if (newfile(current, where, path))	/* которой закончили */
      {
        fclose(in);
        goto loop;		  /* откроем новый файл */
      }
      goto loop1;
    }

    position = ftell(in);
  /* теперь в остатке файла */
    while (!(where = find(buf, "\033<")) && fgets(buf, 1024, in))
      position = ftell(in);

    if (where && !oldfile(where))	  /* если файл не встречался раньше */
    {
      current->position = position;
      current->pos_in_string = where - buf;
      if (newfile(current, where, path))	/* которой закончили */
      {
        fclose(in);
        goto loop;		  /* откроем новый файл */
      }
      goto loop1;
    }
  }	/* if(!gzipped) */

  if (gzipped || (!gzipped && current->previous != (FILESTACK *) NULL))
  {	/* перейдем к предыдущему, если есть 		   */
    FILESTACK *tmph;

    fclose(in);
    compile(curr_file);

    tmph = current;
    current = current->previous;
    free(tmph);

    if (path != (char *) NULL)
      sprintf(curr_file, "%s/%s", path, current->path);
    else
      strcpy(curr_file, current->path);

    if ((in = fopen(curr_file, "r")) !=(FILE *) NULL)
    {
      fseek(in, current->position, SEEK_SET);
      gzipped = 0; 
      goto loop1;
    }
  }

  fclose(in);
}

int oldfile(string)		  /* пробегает по списку файлов */
char *string;			  /* в поисках такого же имени  */
{
  FILESTACK *tmpf;
  char name[1024];

  parser(string, name);
  tmpf = current;
  while (strcmp(tmpf->path, name) && tmpf->previous != (FILESTACK *) NULL)
    tmpf = tmpf->previous;

  if (!strcmp(tmpf->path, name))  /* этот файл уже обрабатывали */
    return 1;

  return 0;
}

int newfile(old, string, path)	  /* выделяет память для структуры и связывает */
FILESTACK *old;			  /* ее со списком */
char *string, *path;
{
  FILESTACK *tmph;
  char name[1024];
  char curr_file[256];		  /* Имя текущего файла	 */
  FILE *tmpfil;

  parser(string, name);

  if (path != (char *) NULL)	  /* сделаем имя файла */
    sprintf(curr_file, "%s/%s", path, name);
  else
    strcpy(curr_file, name);

  if ((tmpfil = fopen(curr_file, "r+")) == (FILE *) NULL)
  {
    printf("Не могу открыть файл %s.\n", curr_file);
    fail = 1;
    return 0;
  }
  fclose(tmpfil);
  tmph = (FILESTACK *) malloc(sizeof(FILESTACK));	/* теперь память */

  tmph->previous = old;		  /* свяжем его */

  strcpy(tmph->path, name);

  tmph->position = 0;
  tmph->pos_in_string = 0;

  current = tmph;

  return 1;
}

void parser(string, name1)		  /* выделяет из строки string */
char *string;			  /* последнюю строку */
char *name1;
{
  int x;
  char b[PATHLEN];
  char *p;
  char name[PATHLEN];

  sscanf(string, "%d-%d;%s", &x, &x, b);
  sscanf((char *) strchr(b, ';'), ";%s", name);
  p = (char *) strchr(name, '>');
  *p = '\0';

  strcpy(name1, name);
}

void free_stack()		  /* освобождает память, выделенную под структуры */
{
  FILESTACK *tmpf;

  while (current->previous != (FILESTACK *) NULL)
  {
    tmpf = current->previous;
    free(current);
    current = tmpf;
  }
  free(current);
}

char *find(string, pattern)	  /* поиск подстроки. Возвращает указатель на */
char *string;			  /* конец найденной подстроки или 0 */
char *pattern;
{
  char *p;

  p = pattern;

  while (*p && *string)
  {
    if (*string++ != *p++)
      p = pattern;
  }
  if (!*p)
    return string;
  else
    return 0;
}

compile(name)
char *name;
{
  char string[BUF];		  /* Строка из файла            */

  long current_pg;		  /* Указатели на начальную строку     */
  long current_work;		  /* в текущей странице          */
  long previous_pg;		  /* Указатель на начальную строку    */
  long previous_work;		  /* в предыдущей странице        */

  FILE *in_file;

  char tmpname[1024];

  extern long bytes_in, bytes_out;

  struct stat Statbuf;
  
  current_pg = 0;
  previous_pg = 0;
  previous_work = 0;
  current_work = 0;

  stat (name, &Statbuf);
  if (Statbuf.st_mtime < last_modify)
    return 0;

  if ((in_file = fopen(name, "r+")) == (FILE *) NULL)
    return 0;

  if(fgetc(in_file) == 0x1f && fgetc(in_file) == 0x8b)	/* if gzipped */
  {
    fclose(in_file);
    return 0;
  }
  else
    rewind(in_file);

  printf("Compiling file %s...", name);
  fflush(stdout);
  while (fgets(string, BUF, in_file) != (char *) NULL)
  {
    if ((char *) strchr(string, '\014') == (char *) NULL)	/* Поиск символа    */
      continue;			  /* новой сраницы    */

    previous_work = current_pg;	  /* Запомнить      */
    current_pg = previous_pg;	  /* указатель      */
    previous_pg = ftell(in_file); /* на страницу     */

    current_work = ftell(in_file);
    fseek(in_file, current_work - strlen(string), SEEK_SET);
    fprintf(in_file, "\014%8ld\n", previous_work);	/* Запись  */
    /* в  файл */
    fseek(in_file, current_work, SEEK_SET);
  }
  fclose(in_file);

  if(strcmp(name, "glmenu.hyp") || !dont_gzip)	/* glmenu is never compiled */
  {
    sprintf(tmpname, "%s..", name);
    compilefilename = name;
    compilefilesize = Statbuf.st_size;
    bytes_out = 0;
    printprogress ();
    gzip(name, tmpname);
    signal(SIGALRM, *nullfunc);
    rename(tmpname, name);
  }
  printf("\rCompiling file %s...%dK %03.5g%% Done\n", name, (bytes_in+1023)/1024,
						100.0*bytes_out/bytes_in);
  return 1;
}

void printprogress()
{
  extern long bytes_in;
  static long old_bytes_in = 0;

  if (bytes_in != old_bytes_in)
  {
    printf("\rCompiling file %s...%.0f%% Done", compilefilename,
		99.0*bytes_in/compilefilesize);
    fflush (stdout);
    old_bytes_in = bytes_in;
  }
  alarm(1);
  signal(SIGALRM, *printprogress);
}

void nullfunc () {}
