/*
 *  $Id: eddelseg.c,v 1.3 1993/05/04 11:17:14 sev Exp $
 *
 * ---------------------------------------------------------------------------
 *
 * $Log: eddelseg.c,v $
 * Revision 1.3  1993/05/04 11:17:14  sev
 * *** empty log message ***
 *
 * Revision 1.2  1993/04/29  12:38:47  sev
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
  DBDP dline, edfind();
  VCEDLINE *line;
  char buf[80], tmp1[1024];
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
      line = vcedgline(vced->edbuffer, dline);

      strcpy(tmp1, value);	/* Выделим имя сегмента */
      if(ch = strrchr(tmp1, '%'))
        *ch = 0;
      sprintf(buf, "\033(%s", tmp1);

      ch = strstr(line->ltext, buf);	/* Удалим скобку */
      strcpy(tmp1, ch);
      *ch = 0;
      strcat(line->ltext, tmp1+strlen(buf));
      trimstring(line->ltext);
      vcedrline(vced->edbuffer,line,dline);
      num = atoi(strrchr(tmp->idisplay, '%')+1)+1; /* Строк в сегменте */
      while(num--)
      {
	line = vcedgline(vced->edbuffer, dline);
	dline = line->lnext;
      }

      ch = strstr(line->ltext, "\033)");	/* Удалим вторую скобку */
      strcpy(tmp1, ch);
      *ch = 0;
      strcat(line->ltext, tmp1+2);
      trimstring(line->ltext);
      vcedrline(vced->edbuffer,line,dline);
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
