/*
 *  $Id: edsvf.c,v 1.2 1993/04/10 13:52:22 sev Exp $
 *
 * ---------------------------------------------------------------------------
 *
 * $Log: edsvf.c,v $
 * Revision 1.2  1993/04/10 13:52:22  sev
 * Изменена структура справочника
 *
 * Revision 1.1  1993/04/06  14:14:07  sev
 * Initial revision
 *
 * Revision 1.2  1993/04/05  16:08:05  kas
 * *** empty log message ***
 *
 * Revision 1.1  1993/04/03  14:31:33  sev
 * Initial revision
 *
 * Revision 1.1  1993/04/01  14:10:29  kas
 * Initial revision
 *
 *
 */

#define FILE_ 1
#define DIR_  0

#include "vced.h"

/* ------------------------------------------------------------------------ */

COUNT ednewf(vced)
VCED *vced;
{
  vced->edmail=VCEDNEWFIL;
     return(0);
}

/* ------------------------------------------------------------------------ */

COUNT ednextf(vced)
VCED *vced;
{
  vced->edmail=VCEDNEXTFIL;
     return(0);
}

/* ------------------------------------------------------------------------ */

COUNT eddelf(vced)
VCED *vced;
{
  vced->edmail=VCEDDELFILE;
     return(0);
}

/* ------------------------------------------------------------------------ */

COUNT edrefresh()
{
  wrefresh(1);
  return(0);
}

/* ------------------------------------------------------------------------ */

COUNT edsavef(vced)
VCED *vced;
{
  TEXT name[80];

  vcedsbuf(vced);
  vced->edbuffer->bfchange=1;
  sprintf(name,"Файл \"%s\" записан на диск",vced->edbuffer->bfname);
  ask_msg(name,0);
     return(0);
}

/* ------------------------------------------------------------------------ */

COUNT edunmark(vced)
VCED *vced;
{
     vced->edmail=UNMARK;
     return(0);
}

/* ------------------------------------------------------------------------ */

COUNT edendmark(vced)
VCED *vced;
{
     vced->edmail=ENDMARK;
     return(0);
}

/* ------------------------------------------------------------------------ */

char *make_name(file, type)
char *file;
char *type;
{
  char buf[256];
  char *b;

  strcpy(buf, file);
  b = (char *)strrchr(buf, '.');
  if(b != (char *)NULL)
    *b = '\0';
  strcat(buf, type);

  return buf;
}

/* ------------------------------------------------------------------------ */

void askdirfile()
{
  WPTR window;
  char nm[80];
  char *p;

  window = wxopen(10,5,15,75,"",BORDER+ACTIVE+CURSOR+NOADJ+CURTAIN,BD1,0,0);

  strcpy(nm, make_name(cfile->vcedfname, ".dir"));

  sprintf(nm, "%-69s", nm);
  atsay(1,1, "Введите имя файла справочника: ");
  atget(2,0, nm, "");
  readgets();

  if((p = (char *)strchr(nm, ' ')) != (char *)NULL)
    *p = '\0';

  if(!strstr(nm, ".dir"))
    strcat(nm, ".");
  strcpy(dir_file, make_name(cfile->vcedfname, ".dir"));

  wclose(window);
}

/* ------------------------------------------------------------------------ */

COUNT edir(vced)
VCED *vced;
{
   dirr = delselset(dirr);
   edftostr();

   vced->edkey[0]=DIR_SEG;
   return(0);
}

/* ------------------------------------------------------------------------ */

COUNT edlistwdo(vced)
VCED *vced;
{
   vced->edkey[0]=LISTWDO;
   return(0);
}

/* ------------------------------------------------------------------------ */

COUNT edfiles(vced)
VCED *vced;
{
   vced->edkey[0]=FILES;
   return(0);
}

/* ------------------------------------------------------------------------ */
/*                                                                          */
/*    Текущий номер строки на странице. Страница начинается с \014          */
/*                                                                          */
/* ------------------------------------------------------------------------ */

COUNT numlinpage(vced)
VCED *vced;
{
  COUNT rows=0;
  LONG rest = vced->edcrow;
  VCEDLINE *line;

  if(vced->edcrow >= vced->edbuffer->bfnlines)
     return 0;

  line = vcedgline(vced->edbuffer, vced->edcline);

  while(line->ltext[0] != (TEXT)0x0c && rest)
  {
    line = vcedgline(vced->edbuffer, line->lprev);
    rows ++;
    rest --;
  }

  return rows;
}
  
/* ------------------------------------------------------------------------ */
