/*
 *  $Id: edaskdir.c,v 1.2 1993/04/10 13:52:22 sev Exp $
 *
 * ---------------------------------------------------------- 
 *
 * $Log: edaskdir.c,v $
 * Revision 1.2  1993/04/10 13:52:22  sev
 * Изменена структура справочника
 *
 * Revision 1.1  1993/04/06  14:14:07  sev
 * Initial revision
 *
 * Revision 1.4  1993/04/01  14:10:29  kas
 * *** empty log message ***
 *
 * Revision 1.3  1993/03/27  12:08:16  kas
 * *** empty log message ***
 *
 * Revision 1.2  1993/03/15  13:47:31  sev
 * Полностью заменены функции работы со справочником. Теперь он текстовый
 *
 * Revision 1.1  1993/03/12  15:44:23  sev
 * Init
 *
 *
 */

static char rcsid[]="$Id: edaskdir.c,v 1.2 1993/04/10 13:52:22 sev Exp $";

#define VCGET_DEFS

#include "vced.h"
#include <string.h>

/***************************************************************************/
/*  Функция записывает в файл  *.dir  строку вида:		           */
/*  "коментарий (имя сегмента) %длина сегмента|имя сегмента%кол-во ссылок" */
/***************************************************************************/

int ask_dir(vced,len_seg)
VCED *vced;
int len_seg;
{
  WPTR w_;
  char bb[40];
  char mesg_of_seg[MES_LEN];
  char name[NAME_LEN];
  char name_seg[NAME_LEN];
  char *strstr(),*strchr(),*ch;
  int  key;
  char tmp1[256];

  w_=wxxopen(8,9,14,71,NULL,ACTIVE+CURSOR+BORDER+BD2+SHADOW,0,0,0,32);

  empty(bb,40);
  atsay(1,2,"Введите комментарий к сегменту");
  atsay(3,1,"[");
  atsay(3,41,"]");

  xatget(3,2,bb,"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",isblank,
      NULLTEXT,NULLTEXT,ATR_F,ATR_F);
  key = readgets();
  trim(bb);
  if(key==ESC)
  {
    erase();
    atsay(3,3,"Блок не отмечен. Нажмите любую клавишу...");
    getone();
    wclose(w_);
    return(0);
  }
  strcpy(mesg_of_seg,bb);
  col_seg_of_file++;			    /* количество сегментов в файле */
  strcpy(name,vced->edbuffer->bfname);
  if((ch=strchr(name,'.'))!=(char *)NULL)
    *ch='\0';
  sprintf(name_seg,"%s%d%%0",name,col_seg_of_file);
  /*                     ^^ - %кол-во ссылок на сегмент */

  sprintf(tmp1,"%s (%s) %%%d",mesg_of_seg,vced->edbuffer->bfname, len_seg);

  addselitm(dirr,tmp1,name_seg);

  if(putselset(dir_file, "w", dirr))
  {
    vcend(CLOSE);
    execlp("clear","clear",(char *)NULL);
    printf("Не могу записать в файл %s\n", dir_file);
    exit(1);
  }

  wclose(w_);

  return(1);
}
