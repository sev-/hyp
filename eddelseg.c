/*
 *  $Id: eddelseg.c,v 1.2 1993/04/29 12:38:47 sev Exp $
 *
 * ---------------------------------------------------------------------------
 *
 * $Log: eddelseg.c,v $
 * Revision 1.2  1993/04/29 12:38:47  sev
 * Работает удаление сегмента
 *
 * Revision 1.1  1993/04/20  16:04:12  sev
 * Initial revision
 *
 *
 */

#include "vced.h"

eddelseg(vced, value)
char *value;
{
  char *ch;
  int num;
  SELSET *tmp;
  char filename[80];
  DBDP dline, dline1, edfind();
  VCEDLINE *line;
  char buf[80];
  char *strchr(), *strrchr();

  num = atoi(strrchr(value, '%')+1);
  if(num)
  {
    ask_msg("Есть ссылки на  этот сегмент !!!",0);
    return(-1);
  }
  
  tmp = dirr;	/*  Ищем сегмент */
  while(tmp && strncmp(tmp->ivalue, value, strlen(value)))
	tmp = tmp->inext;

  if(tmp)	/* Нашли сегмент */
  {
    if(!ask_msg("Вы действительно хотите удалить этот сегмент?", 1))
      return(-1);
    ch = strchr(tmp->idisplay, '(');
    strcpy(filename, ch+1);
    ch = strrchr(filename, ')');
    if(ch)
      *ch = 0;

    if(strcmp(filename, cfile->vcedfname))
    {
      ask_msg("Файл не текущий, перехожу",0);
      return(-2);
    }

    sprintf(buf, "\033(%s", value);
    ch = strrchr(buf, '%');
    if(ch)
      *ch = 0;
    if((dline = edfind(vced, buf)) != (DBDP)NULL)
    {
      num = atoi(strrchr(tmp->idisplay, '%')+1)+2; /* Строк в сегменте + ^L */
      while(num--)
      {
	line = vcedgline(vced->edbuffer, dline);
	dline1 = line->lnext;
	vceddline(vced->edbuffer, line->lprev, dline, line->lnext);
	dline = dline1;
      }
    }
  }
  return 0;
}

DBDP edfind(vced, find)
VCED *vced;
char *find;
{ 
	VCEDLINE *line;
	DBDP dline;
	char *strstr();

	vcedtof(vced);

	dline=vced->edcline;
        while(dline != (DBDP)0)
        {
	     line=vcedgline(vced->edbuffer,dline);
	     if(strstr(line->ltext,find)!=(char *)NULL)
		return dline;
	     dline = line->lnext;
	}
	return (DBDP)NULL;
}                
