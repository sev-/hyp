/*
 *  $Id: refer.c,v 1.1 1993/03/31 16:38:21 sev Exp $
 *
 * ---------------------------------------------------------- 
 *
 * $Log: refer.c,v $
 * Revision 1.1  1993/03/31 16:38:21  sev
 * Initial revision
 *
 *
 */

#include <stdio.h>

#define PATHLEN 256
#define DEBUG

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

typedef struct SEGMENTS		/* список сегментов */
{
  char			 name[256];
  int			 num;
  struct SEGMENTS	*next;
} SEGMENTS;

COMPILED  *head_comp;
COMPILED  *curr_comp;
FILESTACK *current;
SEGMENTS  *head_seg;

void	 go_conf();
void	 refer();
void	 free_stack();
char	*find();
char	*make_name();
void	 newfile();
int	 oldfile();
char	*parser();
void	 add_seg();


main(argc, argv)
int	argc;
char  **argv;
{
  int carg;

  if(argc < 2)
  {
    return 0;
  }

  carg = 1;
  while(carg < argc)
  {
    go_conf(argv[carg]);
    ++carg;
  }

  return 0;
}

void go_conf(filename)	/* первая строка в файле - имя для обработки */
char *filename;
{
  FILE *conf_file;
  FILE *refer_file;
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

  if((refer_file = fopen(name, "w")) == (FILE *)NULL)
  {
    printf("Не могу открыть файл %s для записи в него отчета.\n", name);
    fclose(curr_file);
    return;
  }

  fclose(curr_file);
  fclose(refer_file);

  begin_file = (char *)strrchr(path, '/');	/* выделние пути и имени */
  *(begin_file ++) = '\0';

  refer(begin_file, name, path);

  free_stack();
}

void refer(filein, fileout, path)	/* функция обработки файла filein */
char *filein;				/* имя входного файла _БЕЗ ПУТИ_  */
char *fileout;				/* имя выходного файла		  */
char *path;				/* путь к входному файлу	  */
{
  FILE *in;
  FILE *out;
  char	buf[1024];
  long	position;
  char *where;
  char *p;
  char	curr_file[256];	/* Имя текущего файла				*/
  char	curr_seg[256];	/* Имя текущего сегмента			*/
  int	x;
  char	seg[256];	/* Имя сегмента, на который ссылается поле	*/
  SEGMENTS *tmps;
  int   was = 0;	/* флаг первого сегмента */

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
    sprintf(curr_file, "%s/%s", path, current->path);
  else
    strcpy(curr_file, current->path);

#ifdef DEBUG
  fputs(curr_file, stdout);
  fputc('\n', stdout);
#endif

  in = fopen(curr_file, "r");

  while(fgets(buf, 1024, in))	/* вывод в файл имен всех сегментов и полей */
  {
    position = ftell(in);

    /* нет ли в этой строке начала сегмента ? */
    if(where = find(buf, "\033("))	/* есть */
    {
      strcpy(curr_seg, where);
      p = (char *)strchr(curr_seg, '\n');
      if(p)
	*p = '\0';
      p = (char *)strchr(curr_seg, '\r');
      if(p)
	*p = '\0';

      if(!was)
      {
	head_seg = (SEGMENTS *)malloc(sizeof(SEGMENTS));
	head_seg->next = (SEGMENTS *)NULL;
	head_seg->num = 0;
	sprintf(head_seg->name, curr_seg);
	was = 1;
      }
      else
	add_seg(curr_seg, 0);	/* Добавляем сегмент в список */
    }

    if(where = find(buf, "\033<"))	/* нашли начало поля */
    {
      /* структура поля : \033<dd;dd;segment;file>
 		,где segment - имя сегмента,
		 file - имя файла, содержащего этот сегмент */

       sscanf(where, "%d-%d;%s", &x, &x, seg);
       p = (char *)strchr(seg, ';');
       *p = '\0';
       add_seg(seg, 1);		/* Добавляем сегмент */
    }
  }

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
      sprintf(curr_file, "%s/%s", path, current->path);
    else
      strcpy(curr_file, current->path);

    in = fopen(curr_file, "r");
    fseek(in, current->position, SEEK_SET);
    goto loop1;		/* уже не будем смотреть его поля и сегменты */
  }

  fclose(in);
  out = fopen(fileout, "w");

  tmps = head_seg;
  while(tmps != (SEGMENTS *)NULL)
  {
    sprintf(buf, "%s#%d\n", tmps->name, tmps->num);
    fputs(buf, out);
    tmps = tmps->next;
  }
  fclose(out);	/* конец refer() */
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
  SEGMENTS  *tmps;
  int numfiles = 0;
  int numseg = 0;

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

  while(head_seg->next != (SEGMENTS *)NULL)
  {
    tmps = head_seg->next;
    free(head_seg);
    head_seg = tmps;
    numseg ++;
  }
  numseg ++;
  free(head_seg);
#ifdef DEBUG
  printf("Обработано файлов: %d.\n", numfiles);
  printf("Встречено сегментов: %d.\n", numseg);
#endif
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

char *make_name(name)	/* добавляет к name .ref с контролем за длиной */
char *name;
{
  char buf[PATHLEN];

  if(strlen(&name[strrchr(name, '/')]) > 9)
  {
    printf("Предупреждение: имя %s слишком длинное\n", name);
    sprintf(buf, "%s", name);
    strcpy(&buf[strlen(buf)-4], ".ref");
  }
  else
    sprintf(buf, "%s.ref", name);

  return buf;
}

void add_seg(name, delta)	/* выделяет память для структуры и связывает */
char *name;			/* ее со списком */
int   delta;
{
  SEGMENTS *tmps, *tmps1;

  tmps = head_seg;
  while(strcmp(tmps->name, name) && tmps->next != (SEGMENTS *)NULL)
    tmps = tmps->next;

  if(!strcmp(tmps->name, name))	/* этот сегмент уже встречали */
    tmps->num += delta;
  else
  {
    tmps = (SEGMENTS *)malloc(sizeof(SEGMENTS));	/* теперь память */

    tmps1 = head_seg;				/* найдем конец списка */
    while(tmps1->next != (SEGMENTS *)NULL)
      tmps1 = tmps1->next;

    tmps1->next = tmps;			/* свяжем его */

    strcpy(tmps->name, name);

    tmps->num = 0;
    tmps->num += delta;
    tmps->next = (SEGMENTS *)NULL;
  }
}
