/*
 *  $Id: edftostr.c,v 1.2 1993/04/20 16:04:12 sev Exp $
 *
 * ---------------------------------------------------------------------------
 *
 * $Log: edftostr.c,v $
 * Revision 1.2  1993/04/20 16:04:12  sev
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
/*     Функция читает из файла <file>.dir , формирует структуру справочника */
/****************************************************************************/

int edftostr()
{
  char tmp2[80];

  if(getselset(dir_file, "r", dirr))
  {
    sprintf(tmp2, "Справочник (%s) пуст", dir_file);
    ask_msg(tmp2,0);
    return(0);
  }

  return GOOD;
}
