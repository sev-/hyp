/*
 *  $Id: edinsseg.c,v 1.1 1993/04/06 14:14:07 sev Exp $
 *
 * ---------------------------------------------------------- 
 *
 * $Log: edinsseg.c,v $
 * Revision 1.1  1993/04/06 14:14:07  sev
 * Initial revision
 *
 * Revision 1.6  1993/04/06  13:40:54  kas
 * *** empty log message ***
 *
 * Revision 1.5  1993/04/01  14:10:29  kas
 * *** empty log message ***
 *
 * Revision 1.4  1993/03/27  12:08:16  kas
 * *** empty log message ***
 *
 * Revision 1.3  1993/03/17  15:53:58  sev
 * Чего-то там менял, чего не помню
 *
 * Revision 1.2  1993/03/15  13:47:31  sev
 * Полностью заменены функции работы со справочником. Теперь он текстовый
 *
 * Revision 1.1  1993/03/12  15:44:23  sev
 * Init
 *
 *
 */

static char rcsid[]="$Id: edinsseg.c,v 1.1 1993/04/06 14:14:07 sev Exp $";

#define SP_BAR "                                                                                              "

#include <unistd.h>
#include "vced.h"

/**********************************************************************/
/*Функция отмечает в текущем файле начало  и конец           сегмента */
/**********************************************************************/

inc_seg_str(vced,start,end,len_seg,name_seg)
VCED *vced;
DBDP  start,end;
int   len_seg;
TEXT *name_seg;
{
  SELSET   *tmp;
  char      cur_name[80];
  char     *tmpch;
  char      buf[80];
  int       is=0,save=0;
  VCEDLINE *line;
  char      ins[120];
  char      ins_tmp[120];
  char     *strchr(),*strstr(),*strrchr();
  char     *ch,*prom,*cc;
  int       len=0;

  tmp = dirr;       /* Справочник сегментов */

  sprintf(ins,"\033(%s",name_seg);

  line = vcedmline(vced->edbuffer,start);
  prom=(char *)line->ltext;

  while((ch=strstr(prom,"\033("))!=(char *)NULL)
  {
    is=1;
    vcedempty(buf,80);
    strcpy(buf,ch+2);

    if((cc=strchr(buf,'\033'))!=(char *)NULL)
      *cc='\0';
    while(tmp)
    {
      tmpch = strchr(tmp->idisplay, '(');
      strcpy(cur_name, tmpch+1);
      if((tmpch = strchr(name_seg,')')) != (char *)NULL)
 	 *tmpch = '\0';

      if(strcmp(cur_name, buf) == 0)
      {
        tmpch = strchr(tmp->idisplay, '(');
        sscanf(tmpch, ") %%%d", &len);
	break;
      }
      tmp = tmp->inext;
    }

    if(len_seg >= len )
    {
      save=1;
      strins(ch,ins);
      break;
    }
    prom=ch+2;
  }

/* ------ */
  prom=(char *)line->ltext;
  if(is == 0 )
  {
    if((ch=strstr(prom,"\033<")) != (char *)NULL)
      strins(ch,ins);
    else
    {
      vcedempty(ins,30);
      sprintf(ins,"\033(%s",name_seg);
      vcedempty(ins_tmp,30);
      strncat(ins_tmp,SP_BAR,80-line->lused);
      strcat(ins_tmp,ins);
      strins(prom+line->lused,ins_tmp);
    }
  }

  if( is==1 && save==0 )
  {
    if((ch=strstr(prom,"\033<")) != (char *)NULL)
      strins(ch,ins);
    else
    {
      ch=prom+strlen(prom);
      strins(ch,ins);
    }
  }
  line->lused=strlen(line->ltext);
  vcedrline(vced->edbuffer,line,end);

  line = vcedmline(vced->edbuffer,end);
  prom=(char *)line->ltext;
  strcpy(ins,"\033)");

  if((ch=strstr(prom,"\033)")) != (char *)NULL )
    strins(ch,ins);
  else
  {
    if((ch=strrchr(prom,'>')) != (char *)NULL )
      strins(ch+1,ins);
    else
    {
      if((ch=strstr(prom,"\033(")) != (char *)NULL )
	strcat(prom,ins);
      else
      {
	vcedempty(ins_tmp,30);
	strncat(ins_tmp,SP_BAR,80-line->lused);
	strcat(ins_tmp,ins);
	strins(prom+line->lused,ins_tmp);
      }
    }
  }
  line->lused=strlen(line->ltext);
  vcedrline(vced->edbuffer,line,end);
}

/* ------------------------------------------------------------------------ */

int strins(where,what)
char *where,*what;
{
  char buf[512];

  strcpy(buf,where);
  strcpy(where,what);
  strcat(where,buf);
}

/* ------------------------------------------------------------------------ */
