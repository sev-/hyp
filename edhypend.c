/*
 *  $Id: edhypend.c,v 1.1 1993/04/06 14:14:07 sev Exp $
 *
 * ---------------------------------------------------------- 
 *
 * $Log: edhypend.c,v $
 * Revision 1.1  1993/04/06 14:14:07  sev
 * Initial revision
 *
 * Revision 3.3  1993/03/31  07:26:09  kas
 * compile call from vcsystem
 *
 * Revision 3.2  1993/03/27  12:08:16  kas
 * *** empty log message ***
 *
 * Revision 3.1  1993/03/24  16:39:49  kas
 * ..
 *
 * Revision 1.3  1993/03/23  13:10:22  kas
 * *** empty log message ***
 *
 * Revision 1.2  1993/03/14  11:39:19  sev
 * Исправлена ошибка в отметке поля. Полностью работающая версия
 * В файле vcопределение DEFSEG - добавление окна сегментов
 *
 * Revision 1.1  1993/03/12  15:44:23  sev
 * Init
 *
 *
 */

static char rcsid[]="$Id: edhypend.c,v 1.1 1993/04/06 14:14:07 sev Exp $";

#include "vced.h"

/* ------------------------------------------------------------------------ */

COUNT edcompil(vced)
VCED *vced;
{
  char *tmp1;
  char tmp2[80];

  tmp1 = make_name(dir_file, ".cnf");

       if(access(tmp1 ,0)!=0)
       {
	 sprintf(tmp2,"Выберите начальный сегмент.Нет фала \"%s\".",tmp1);
	 ask_msg(tmp2, 0);
	 return(0);
       }

  vcedsbuf(vced);
  sprintf(tmp2, "compiler %s\n", tmp1);
  vcsystem(tmp2,1," Press any key ");
  return GOOD;
}

/* ------------------------------------------------------------------------ */

COUNT edshow()
{
  char *tmp1;
  char tmp2[80];

  tmp1 = make_name(dir_file, ".cnf");

       if(access(tmp1,0)!=0)
       {
	 sprintf(tmp2, "Произведите компиляцию.Нет фала \"%s\".", tmp1);
	 ask_msg(tmp2, 0);
	 return(0);
       }
  sprintf(tmp2, "hyp %s\n", tmp1);
  vcsystem(tmp2,1," Press any key ");

  return GOOD;
}
/* ------------------------------------------------------------------------ */
