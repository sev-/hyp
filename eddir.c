/*
 *  $Id: eddir.c,v 1.5 1993/05/04 11:17:14 sev Exp $
 *
 * ---------------------------------------------------------------------------
 *
 * $Log: eddir.c,v $
 * Revision 1.5  1993/05/04 11:17:14  sev
 * *** empty log message ***
 *
 * Revision 1.4  1993/04/29  12:38:47  sev
 * Работает удаление сегмента
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

      tmp = dirr;
      strcpy(header," Cправочник ");
      key=drivesel(tmp,privyz.mesg,privyz.name_seg,header);
      if(key == DEL_KEY)
        switch(eddelseg(vced, privyz.name_seg))
        {
          case -1:	/* есть ссылки */
	    retval=0;
	    break;
	  case -2:	/* не текущий файл */
	    key = RET;
	    break;
	  default:	/* Все в порядке */
	    retval = 0;
	}
        if((ch=(char *)strrchr(privyz.name_seg,'%')) != (char *)NULL)
          *ch='\0';	/* Отсекаем кол-во ссылок */
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
