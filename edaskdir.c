/*
 *  $Id: edaskdir.c,v 1.5 1993/07/10 08:28:31 sev Exp $
 *
 * ---------------------------------------------------------------------------
 *
 * $Log: edaskdir.c,v $
 * Revision 1.5  1993/07/10 08:28:31  sev
 * *** empty log message ***
 *
 * Revision 1.4  1993/04/22  13:23:26  sev
 * dir записывается один раз
 * \
 *
 * Revision 1.3  1993/04/20  16:04:12  sev
 * *** empty log message ***
 *
 * Revision 1.3  1993/04/15  15:05:51  kas
 * *** empty log message ***
 *
 * Revision 1.3  1993/04/15  15:05:51  kas
 * *** empty log message ***
 *
 * Revision 1.2  1993/04/13  13:50:41  kas
 * *** empty log message ***
 *
 * Revision 1.2  1993/04/13  13:50:41  kas
 * *** empty log message ***
 *
 * Revision 1.1  1993/04/12  15:13:06  kas
 * Initial revision
 *
 * Revision 1.1  1993/04/12  15:13:06  kas
 * Initial revision
 *
 *
 */

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

  strcpy(name,vced->edbuffer->bfname);
  if((ch=strchr(name,'.'))!=(char *)NULL)
    *ch='\0';
  sprintf(name_seg,"%s%d%%0",name,col_seg_of_file);
  /*                     ^^ - %кол-во ссылок на сегмент */

  strncat(name,
"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    NAME_LEN-strlen(name)-10);

  w_=wxxopen(8,9,14,71,NULL,ACTIVE+CURSOR+BORDER+BD2+SHADOW,0,0,0,32);

  empty(bb,40);
  atsay(1,2,"Введите комментарий к сегменту");
  atsay(3,1,"[");
  atsay(3,41,"]");

  xatget(3,2,bb,name,isblank,
      NULLTEXT,NULLTEXT,ATR_F,ATR_F);
  key = readgets();
  trim(bb);
  if(key==ESC)
  {
    erase();
    atsay(3,3,"Сегмент не отмечен. Нажмите любую клавишу...");
    getone();
    wclose(w_);
    return(0);
  }
  strcpy(mesg_of_seg,bb);
  col_seg_of_file++;			    /* количество сегментов в файле */
  sprintf(tmp1,"%s (%s) %%%d",mesg_of_seg,vced->edbuffer->bfname, len_seg);

  addselitm(dirr,tmp1,name_seg);

  wclose(w_);

  return(1);
}
