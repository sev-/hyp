/*
 *  $Id: search.c,v 1.2 1993/03/05 17:04:23 sev Exp $
 *
 * ---------------------------------------------------------- 
 *
 * $Log: search.c,v $
 * Revision 1.2  1993/03/05 17:04:23  sev
 * теперь перелистывание страниц по сегментам
 *
 * Revision 1.1  1993/03/04  09:51:10  sev
 * Initial revision
 *
 *
*/

static char rcsid[]="$Id: search.c,v 1.2 1993/03/05 17:04:23 sev Exp $";

#include "hyp.h"

void forward_search(file)
FILE *file;
{
  int stop = 0;
  int i;
  long position;
  long old_position;
  int more = 1;
  int errorwind;
  char end[80];

  sprintf(end, "\033)%s", current_seg);
  old_position = end_super;
  end_super = atol(buf_pg[col_str]+1);
  read_screen(file, end_super);
  if(old_position == end_super)
    end_super = old_position = 0;
  else
    old_position = end_super;

  if(!edit_string(10, 10, 70, 256, old_pattern, " Поиск вперед ", ATTR_B, ATTR_F))
  	return;

  position = end_super;
  while(!stop&&more)
  {
    if(!(more = read_screen(file, end_super)))
      continue;
    for(i = 0; i < more; i++)
    {
      if(strstr(buf_pg[i], end) != (char *)NULL)
      {
        more = 0;
        stop = 1;
        break;
      }
      if(find(buf_pg[i], old_pattern))
      {
        stop = 1;
        break;
      }
    }
    if(!stop)
      position = end_super;
  }
  if(!more)
  {
    end_super = old_position;
    errorwind = wxopen(10, 30, 12, 50, (char *)NULL,ACTIVE+NOADJ+CURTAIN+BORDER+BD2, 0, 0);
    atsay(0,5,"Не найден");
    getone();
    wclose(errorwind);
  }
  else
  {
    show_finded(i, more);
    end_super = position;
  }
}

void backward_search(file)
FILE *file;
{
  int stop = 0;
  int i;
  int flag=0;
  long position;
  long old_position;
  int more = 1;
  int errorwind;
  char beg[80];

  sprintf(beg, "\033(%s", current_seg);
  old_position = end_super;
  end_super = atol(buf_pg[col_str]+1);
  read_screen(file, end_super);
  if(old_position == end_super)
  {
    end_super = old_position = 0;
    flag = 1;
  }
  else
    old_position = end_super;

  if(!edit_string(10, 10, 70, 256, old_pattern, " Поиск назад ", ATTR_B, ATTR_F))
  	return;

  position = end_super;
  while(!stop&&more)
  {
    more = read_screen(file, end_super);
    for(i = 0; i < more; i++)
    {
      if(strstr(buf_pg[i], beg) != (char *)NULL)
      {
        more = 0;
        stop = 1;
        break;
      }
      if(find(buf_pg[i], old_pattern))
      {
        stop = 1;
        break;
      }
    }
    if(!stop)
    {
      position = end_super = atol(buf_pg[more]+1);
      if(position == 0)
        if(flag)
          more = 0;
        else
          flag = 1;
    }
  }
  if(!more)
  {
    end_super = old_position;
    errorwind = wxopen(10, 30, 12, 50, (char *)NULL,ACTIVE+NOADJ+CURTAIN+BORDER+BD2, 0, 0);
    atsay(0,5,"Не найден");
    getone();
    wclose(errorwind);
  }
  else
  {
    show_finded(i, more);
    end_super = position;
  }
}

char *find(str, pattern)
char *str;
char *pattern;
{
  char *p;

  p = pattern;

  while(*p && *str && *str != '\033')
  {
    if(*str++ != *p++)
      p = pattern;
  }
  if(!*p)
    return str;
  else
    return 0;
}

void show_finded(line, more)
int line;
int more;
{
  int j;
  char *finded;
    
  woff();
  erase();
  print_pg(more, 0, 0, wdo1);
  xatsay(22, 40 - strlen(help_string)/2, help_string, ATTR_B);
  for(j = line; j < more; j++)
  {
    finded = buf_pg[j];
    while((finded = find(finded, old_pattern))!=(char *)NULL)
      xatsay(j, (int)(finded-buf_pg[j])-strlen(old_pattern), old_pattern, ATTR_F+vc.blink);
  }
  at(23,0);
  won();
  getone();
}
