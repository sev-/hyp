/*
 *  $Id: lib.c,v 1.1 1993/09/28 12:58:34 sev Exp $
 *
 * ---------------------------------------------------------- 
 *
 * $Log: lib.c,v $
 * Revision 1.1  1993/09/28 12:58:34  sev
 * Initial revision
 *
 *
 *
*/

#include "shell.h"

int ask_msg(msg1, msg2, mode)
TEXT *msg1, *msg2;
int mode;
{
  int w;
  int len_msg, width, row_u, key, more = 1, y_n = 0;

  len_msg = strlen(msg1);
  if(strlen(msg2) > len_msg)
    len_msg = strlen(msg2);
  if(len_msg<14)
    width = 12+5;
  else
    if(len_msg>79)
      return(-1);
    else
      width = len_msg+5;

  row_u = (80-width)/2;
  w = wxopen(10, row_u, 16, row_u+width, NULL, ACTIVE+BORDER+BD2+SHADOW+NOADJ, 0, 0);

  xatsay(1, 2, msg1, ATR_F);
  xatsay(2, 2, msg2, ATR_F);
  switch(mode)
  {
    case 0:                  /*       любая клавиша   */
      xatsay(3, (width-23)/2, " Нажмите любую клавишу ", ATR_F);
      getone();
      y_n = 1;
      break;
    case 1:                  /*       ДА или НЕТ   */
      xatsay(3, (width-14)/2, "  НЕТ    ДА  ", ATR_F);
      while(more)
      {
        selbar(w, 3, (width-14)/2+7*y_n, ATR_B, 6);
        key = getone();
        switch(key)
        {
          case CUR_RIGHT:
            if(y_n == 0)
            {
              selbar(w, 3, (width-14)/2, ATR_F, 6);
              y_n = 1;
            }
            break;
          case CUR_LEFT:
            if(y_n == 1)
            {
              selbar(w, 3, (width-14)/2+7, ATR_F, 6);
              y_n = 0;
            }
            break;
          case RET:
            more = 0;
            break;
          case ESC:
            more = 0;
            y_n = 0;
            break;
      }
    }
  }
  wclose(w);
  return(y_n);
}
