/*
 *  $Id: edconf.c,v 1.1 1993/04/06 14:14:07 sev Exp $
 *
 * ---------------------------------------------------------- 
 *
 * $Log: edconf.c,v $
 * Revision 1.1  1993/04/06 14:14:07  sev
 * Initial revision
 *
 * Revision 1.3  1993/04/01  14:10:29  kas
 * *** empty log message ***
 *
 * Revision 1.2  1993/03/27  12:08:16  kas
 * *** empty log message ***
 *
 * Revision 1.1  1993/03/12  15:44:23  sev
 * Init
 *
 *
 */

static char rcsid[]="$Id: edconf.c,v 1.1 1993/04/06 14:14:07 sev Exp $";

#include "vced.h"

/*************************************************************************/
/*	      Функция coздает файл conf.hyp				 */
/*************************************************************************/

COUNT edconf(vced)
VCED *vced;
{
  FILE *name;
  TEXT conf_str[200];
  TEXT path[80];
  WPTR w;
  char *tmp1;
  char tmp2[80];

  w=wxxopen(2,54,8,77,NULL,ACTIVE+BORDER+BD2+SHADOW,0,0,0,32);
  xatsay(1,1,"Выберите из спра-",ATR_F);
  xatsay(2,1,"вочника начальный",ATR_F);
  xatsay(3,1,"сегмент.",ATR_F);
  if(eddir(vced)==0)
  {
    xatsay(1,1,"  Файл не создан. ",ATR_F);
    xatsay(2,1,"		  ",ATR_F);
    xatsay(3,1,"  Любая клавиша...",ATR_F);
    getone();
    wclose(w);
    return(0);
  }
  tmp1 = make_name(dir_file, ".cnf");
  if((name=fopen(tmp1,"w+"))==(FILE *)NULL)
  {
    sprintf(tmp2, "	 Не могу создать файл %s.", tmp1);
    ask_msg(tmp2, 0);
    return(0);
  }
  getcwd(path,80);
  sprintf(conf_str,"%s/%s\n%s\n",path,privyz.name_file,privyz.name_seg);
  fputs(conf_str,name);
  fclose(name);
  sprintf(tmp2," Файл  \"%s\" ", tmp1);
  xatsay(1,1,tmp2,ATR_F);
  xatsay(2,1," создан успешно.	  ",ATR_F);
  xatsay(3,1,"	Любая клавиша...  ",ATR_F);
  getone();
  wclose(w);
  return 0;
}
