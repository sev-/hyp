/*
 *  $Id: ednumseg.c,v 1.3 1993/04/20 16:04:12 sev Exp $
 *
 * ---------------------------------------------------------------------------
 *
 * $Log: ednumseg.c,v $
 * Revision 1.3  1993/04/20 16:04:12  sev
 * a
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

#include "vced.h"

/****************************************************************************/
/*               Функция определяет количество сегментов в файле            */
/****************************************************************************/
int  number_seg(name_file)
char *name_file;
{
  char name_seg[500];
  char *strchr(),*strstr(),*ch;
  int  i=0;
  SELSET *tmp;

  tmp = dirr;
  while(tmp)
  {
    ch=strchr(tmp->idisplay,'(');
    strcpy(name_seg,ch+1);
    if((ch = strchr(name_seg,')')) != (char *)NULL)
	*ch = '\0';
    if( strcmp(name_seg,name_file) == 0)
      i++;
    tmp = tmp->inext;
  }
  col_seg_of_file = i;
}
