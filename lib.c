/*
 *  $Id: lib.c,v 1.2 1994/07/11 12:31:07 sev Exp $
 *
 * ---------------------------------------------------------- 
 *
 * $Log: lib.c,v $
 * Revision 1.2  1994/07/11 12:31:07  sev
 * *** empty log message ***
 *
 * Revision 1.1  1993/09/28  12:58:34  sev
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
  WPTR w;
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
    case 0:	/*       любая клавиша   */
      xatsay(3, (width-23)/2, " Нажмите любую клавишу ", ATR_F);
      getone();
      y_n = 1;
      break;
    case 1:	/*       ДА или НЕТ   */
    case 2:	/* dos / unix */
    case 3:
      xatsay(3, (width-14)/2, (mode == 1) ? "  НЕТ    ДА  " : (mode == 2) ?
		"  dos   unix" : (mode == 3) ? "  a:     b:" : "",
					 ATR_F);
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

int passwd()
{
  WPTR w;
  static char passwd[] = "e_cpaepesd";
  int ok = 1;
  int i;
  

  w = wxopen(9, 28, 11, 41, " Password ", BORDER+BD2+CUSTOM+ACTIVE+CURSOR,
					0, 0);
  vcputc(' ', ATR_F);
  for(i = 9; ok && i != -2;)
  {
    if(getone() != passwd[i])
    {
      atsay(0, 0, "   ERROR  ");
      sleep(1);
      ok = 0;
    }
    if(ok)
      vcputc('*', ATR_F);

    if((i -= 2) == -1)
      i = 8;
  }

  if(ok)
  {
    atsay(0, 0, "    OK     ");
    sleep(1);
  }
  wclose(w);

  return ok;
}
