/*
 *  $Id: edhypend.c,v 1.2 1993/04/20 16:04:12 sev Exp $
 *
 * ---------------------------------------------------------------------------
 *
 * $Log: edhypend.c,v $
 * Revision 1.2  1993/04/20 16:04:12  sev
 * a
 *
 * Revision 1.4  1993/04/19  16:36:31  kas
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
  sprintf(tmp2, "view %s\n", tmp1);
  vcsystem(tmp2,1,NULL);

  return GOOD;
}
/* ------------------------------------------------------------------------ */
