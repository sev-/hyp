/*
 *  $Id: ednumseg.c,v 1.2 1993/04/08 10:37:38 sev Exp $
 *
 * ---------------------------------------------------------- 
 *
 * $Log: ednumseg.c,v $
 * Revision 1.2  1993/04/08 10:37:38  sev
 * delete one bug with num_seg_of_file
 *
 * Revision 1.1  1993/04/06  14:14:07  sev
 * Initial revision
 *
 * Revision 1.5  1993/04/05  16:01:33  kas
 * *** empty log message ***
 *
 * Revision 1.4  1993/04/01  14:10:29  kas
 * *** empty log message ***
 *
 * Revision 1.3  1993/03/15  13:47:31  sev
 * Полностью заменены функции работы со справочником. Теперь он текстовый
 *
 * Revision 1.2  1993/03/15  10:55:59  sev
 * Весьма глупая ошибка в подсчете сегментов файла. Искали черт знает что
 *
 * Revision 1.1  1993/03/12  15:44:23  sev
 * Init
 *
 *
 */

static char rcsid[]="$Id: ednumseg.c,v 1.2 1993/04/08 10:37:38 sev Exp $";

#include "vced.h"

/*************************************************************************/
/*	      Функция определяет количество сегментов в файле		 */
/*************************************************************************/
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
