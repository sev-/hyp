/*
 * $Id$
 * 
 * ----------------------------------------------------------
 * 
 * $Log$
 * 
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>

#define PATHLEN 256
#define BUF	1024

char template[] = "/tmp/optimXXXXXX";
char template1[] = "/tmp/optXXXXXX";

int fail = 0;
time_t last_modify;
char *compilefilename;
long compilefilesize;

void go_conf();
void refer();
void free_stack();
char *find();
int newfile();
int oldfile();
void parser();
void nullfunc () {}

main(argc, argv)
int argc;
char **argv;
{
  int carg;

  if (argc < 2)
  {
    puts("Оптимизфтор гипертекста. Версия 1.0\n\n\
 Использование:\n\t\toptim file\n");
    return 1;
  }

  carg = 0;
  while (++carg < argc)
    optim(argv[carg]);

  return 0;
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

  current_pg = 0;
  previous_pg = 0;
  previous_work = 0;
  current_work = 0;

  in_file = fopen (name, "r+");
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

  if(strcmp(name, "glmenu.hyp"))	/* glmenu is never compiled */
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


long loadfilesize;
void printprogress1 ();

void unpackfile (char *name)
{
  FILE *in;
  long fsize[4];
  extern long bytes_out;

  in = fopen(name, "r");
  fseek (in, -4, SEEK_END);
  fsize[0] = getc (in);
  fsize[1] = getc (in);
  fsize[2] = getc (in);
  fsize[3] = getc (in);
  loadfilesize = fsize[0] | (fsize[1] << 8) | (fsize[2] << 16) |
			(fsize[3] << 24);
  fclose(in);

  printprogress1 ();
  gunzip (name, template);
  signal(SIGALRM, *nullfunc);
}

void printprogress1()
{
  extern long bytes_out;
  static long old_bytes_out = 0;

  if (bytes_out != old_bytes_out)
  {
    printf("\rOptimizing file %s...Unpacking %.0f%% Done", compilefilename,
		99.0*bytes_out/loadfilesize);
    fflush (stdout);
    old_bytes_out = bytes_out;
  }
  alarm(1);
  signal(SIGALRM, *printprogress1);
}

void optim (char *name)
{
  struct stat Statbuf;
  FILE *in_file;
  
  stat (name, &Statbuf);
  if (Statbuf.st_mtime < last_modify)
    return 0;

  if ((in_file = fopen(name, "r+")) == (FILE *) NULL)
    return 0;

  if(fgetc(in_file) != 0x1f || fgetc(in_file) != 0x8b)	/* if not gzipped */
  {
    fclose(in_file);
    return 0;
  }
  fclose (in_file);

  if(template[12] == 'X')
    mktemp(&template);
  if(template1[10] == 'X')
    mktemp(&template1);

  unpackfile (name);

  optimize (template);

  compile (template);
  printf("\rOptimizing file %s...Done                          \n");
}

void optim(char *name)
{
  FILE *in, *out, *ftmp;
  char *where;
  char buf[1024];
  int pos_in_string;

  in = fopen(name, "r");
  out = fopen (template1, "w");

  while (fgets(buf, 1024, in) != NULL)
  {
    pos_in_string = 0;
    while (where = find(buf + pos_in_string, "\033<"))
    {
      pos_in_string = where - buf;
      if ((ftmp = fopen (where, "r")) == (FILE *)NULL)
      {
        while(1)	/* deleting */
        {
          s = getline(listfile)+"\n";
          if(s[0] == "-")
            break;
          if(toint(s[0]) == 0x0c)
            continue;
          if(!(s[0] == s[1] && s[1] == s[2] && s[2] == s[3] && s[3] == s[4]))
            break;	/* not continue string */
          tmpstr += s;
          lines_in_text++;
        }

  fclose(in);
}
