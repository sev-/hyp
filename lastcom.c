/*
 * $Id: lastcom.c,v 1.2 1994/11/12 19:16:13 sev Exp $
 * 
 * ----------------------------------------------------------
 * 
 * $Log: lastcom.c,v $
 * Revision 1.2  1994/11/12 19:16:13  sev
 * Indented
 * Revision 1.1  1994/07/27  13:51:30  sev Initial revision
 * 
 * 
 */

#include <stdio.h>
#include <sys/types.h>
#include <time.h>

char usage[] = "Выдает время последней компиляции\n\n\
Использование:\n\t\tlastcom [file]\n";

void go();

main(argc, argv)
int argc;
char **argv;
{
  int carg;

  carg = 1;
  while (carg < argc)
  {
    if (!strcmp(argv[carg], "-h") || !strcmp(argv[carg], "-?"))
    {
      write(2, usage, sizeof(usage));
      return 0;
    }
    go(argv[carg]);
    ++carg;
  }

  if (argc == 1)
    go("glmenu.cnf");
  return 0;
}

void go(filename)
char *filename;
{
  int conf_file;
  char str[8192];
  long last_modify;
  char *ptr;

  if ((conf_file = open(filename, 0)) == -1)
  {
    write(2, "Нет отметки времени в ", 22);
    write(2, filename, strlen(filename));
    write(2, "\n", 1);
    close(conf_file);
    return;
  }
  read(conf_file, str, 8192);
  close(conf_file);

  for (ptr = str; *ptr && *ptr != ':'; ptr++);
  ptr += 2;

  last_modify = atol(ptr);
  ptr = ctime(&last_modify);
  write(1, ptr, strlen(ptr));

  return;
}
