/*
 *  $Id: eddir.c,v 1.1 1993/04/06 14:14:07 sev Exp $
 *
 * ---------------------------------------------------------- 
 *
 * $Log: eddir.c,v $
 * Revision 1.1  1993/04/06 14:14:07  sev
 * Initial revision
 *
 * Revision 1.6  1993/04/01  14:10:29  kas
 * *** empty log message ***
 *
 * Revision 1.5  1993/03/31  07:26:09  kas
 * test of crtlstfl
 *
 * Revision 1.4  1993/03/29  13:48:35  kas
 * *** empty log message ***
 *
 * Revision 1.3  1993/03/29  11:28:20  kas
 * *** empty log message ***
 *
 * Revision 1.2  1993/03/13  12:07:21  sev
 * Исправлены ошибки в delsels, убран ПФ4 и ПФ3
 *
 * Revision 1.1  1993/03/12  15:44:23  sev
 * Init
 *
 *
 */

static char rcsid[]="$Id: eddir.c,v 1.1 1993/04/06 14:14:07 sev Exp $";

#include "vced.h"

char reg[80] = "*.hyp";

/* ------------------------------------------------------------------------ */

COUNT eddir(vced)
VCED *vced;
{
  int key;
  int retval=1;
  char *strchr(),*ch;
  TEXT header[80];
  TEXT path[80];
  SELSET *tmp;
  WPTR w_;
  SELSET  *crt_lst_file();
  char  *cwd, *getcwd();

  if(vced->edkey[0]==LISTWDO)
  {
      tmp= listwdo;
      strcpy(header," Cписок файлов ");
      key=drivesel(tmp,privyz.name_file,privyz.name_seg,header);
  }
  else

  if(vced->edkey[0]==FILES)
  {
     cwd = getcwd((char *)NULL,64);
     strcpy(path,cwd);
     strcpy(header,path);

     w_=wxxopen(0,2,3,36,NULL,ACTIVE+CURSOR+BORDER+BD2,0,0,0,32);

     atsay(0,0,"Файлы:                  ");
     while(1)
       {
	xatgetc(0,7,reg,NULLTEXT,NULLFUNC,NULLTEXT,NULLTEXT,
		ATR_F,ATR_F,13,FLDBLANK+FLDSTRING,NULLTEXT);
	key = readgets();
	if(key==ESC)
	    {
	       wclose(w_);
	       return(0);
	    }
	else
       {
/*     atsay(1,0," Работаю.       ");   */
       trim(reg);

       tmp = crt_lst_file(path,reg);

/*     atsay(1,0,"               ");    */

       key=drivesel(tmp,privyz.name_file,privyz.name_file,header);
       if(key==RET)
	 {
	   wclose(w_);
	   break;
	 }

       }
   }   /* end of while                  */
  }    /* end if FILES                  */
  else
  {

      tmp= dirr;
      strcpy(header," Cправочник ");
      key=drivesel(tmp,privyz.mesg,privyz.name_seg,header);
      strcpy(privyz.name_file,strchr(privyz.mesg,'(')+1);
      if((ch=strchr(privyz.name_file,')'))!=(char *)NULL)
	*ch='\0';

  }

  switch(key)
  {
  case ESC:
    retval=0;
    break;
  case RET:
    if((vced->edkey[0]==DIR_SEG) ||(vced->edkey[0]==LISTWDO) ||
       (vced->edkey[0]==FILES))
    {
      vced->edmail=VCEDNEWFIL;
      strcpy(vceddfil,privyz.name_file);
    }
    break;
  }
  return(retval);
}

/* ------------------------------------------------------------------------ */
