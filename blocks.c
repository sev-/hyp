/*
 *  $Id: blocks.c,v 1.4 1993/04/02 16:17:11 sev Exp $
 *
 * ---------------------------------------------------------- 
 *
 * $Log: blocks.c,v $
 * Revision 1.4  1993/04/02 16:17:11  sev
 * Теперь ввод-вывод блоков осуществляется через временный файл. Еще
 * есть баги
 *
 * Revision 1.3  1993/03/29  12:06:38  sev
 * Эта глупая штука не хочет работать из-за двух закоментаренных
 * строк ф функции инициализации
 *
 * Revision 1.2  1993/03/28  17:56:17  sev
 * *** empty log message ***
 *
 * Revision 1.1  1993/03/20  14:27:27  sev
 * Initial revision
 *
 * Revision 1.1  1993/03/19  17:48:53  sev
 * Initial revision
 *
 *
 */

#include "mted.h"

char cutbuf[80];

COUNT edstartcut()
{
  pid_t mted_pid;
  
  mted_pid = getpid();

  sprintf(cutbuf, "/tmp/mted%d", mted_pid);
}

COUNT edendcut()
{
  remove(cutbuf);
}

COUNT begin_mark(vced)
VCED *vced;
{
  vced->edaline = vced->edcline;
  vced->edachar = vced->edcchar;
  vced->edarow = vced->edcrow;
}

COUNT copy_block(vced)
VCED *vced;
{
  DBDP i, lptr;
  VCEDLINE *line;
  FILE *cut;

  cut = fopen(cutbuf, "w");
  line = vcedgline(vced->edbuffer, vced->edaline);

  for(i = vced->edaline; i <= vced->edcline; i++)
  {
    fputs(line->ltext, cut);
    fputc('\n', cut);
    line = vcednline(vced->edbuffer, line);
  }
  fclose(cut);

  vced->edaline = (DBDP)0;
  vced->edupval = VCEDUPDALL;
}

COUNT cut_block(vced)
VCED *vced;
{
  DBDP i, lptr, lptr1;
  VCEDLINE *line, *linetokill;
  FILE *cut;

  cut = fopen(cutbuf, "w");
  line = vcedgline(vced->edbuffer, vced->edaline);
  lptr = vced->edaline;

  for(i = vced->edaline; i <= vced->edcline; i++)
  {
    fputs(line->ltext, cut);
    fputc('\n', cut);
    linetokill = line;
    lptr1 = line->lnext;
    line = vcednline(vced->edbuffer, line);
    vceddline(vced->edbuffer, linetokill->lprev, lptr, linetokill->lnext);
    lptr = lptr1;
  }
  fclose(cut);

  vced->edcline = lptr;

  vced->edaline = (DBDP)0;
  vced->edupval = VCEDUPDALL;
}

COUNT paste_block(vced)
VCED *vced;
{
  VCEDLINE *line;
  DBDP prev, next; 
  FILE *cut;
  char buf[1024];
  DBDP old;

  cut = fopen(cutbuf, "r");
  line = vcedgline(vced->edbuffer, vced->edcline);
  prev = old = line->lprev;
  next = vced->edcline;

  while(fgets(buf, 1024, cut))
  {
    vcediline(vced->edbuffer, prev, next, buf, strlen(buf));
    line = vcedgline(vced->edbuffer, vced->edcline);
    prev = line->lprev;
  }

  fclose(cut);

  line = vcedgline(vced->edbuffer, old);
  vced->edcline = line->lnext;

  vced->edupval = VCEDUPDALL;
}
