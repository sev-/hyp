/*
 *  $Id: addrefer.c,v 1.1 1993/04/10 13:52:22 sev Exp $
 *
 * ---------------------------------------------------------- 
 *
 * $Log: addrefer.c,v $
 * Revision 1.1  1993/04/10 13:52:22  sev
 * Initial revision
 *
 * 
 */

#include "vced.h"

/* Добавление ссылки к сегменту  */

int add_refer(name)
char *name;
{
  SELSET *tmp;
  TEXT value[128];
  TEXT display[256];
  int num;

  tmp = dirr;

  while(tmp && strncmp(tmp->ivalue, name, strlen(name)))
    tmp = tmp->inext;

  if(tmp)	/* Нашли сегмент */
  {
    num = atoi(strrchr(tmp->ivalue, '%')+1);
    sprintf(value, "%s%%%d", name, num+1);
    strcpy(display, tmp->idisplay);
    repselset(dirr, tmp, display, value);
  }
}

