/*
 *  $Id: edmenu.c,v 1.3 1993/03/15 14:47:57 sev Exp $
 *
 * ---------------------------------------------------------- 
 *
 * $Log: edmenu.c,v $
 * Revision 1.3  1993/03/15 14:47:57  sev
 * Изменил структуру меню. По прежнему просто декорация.
 *
 * Revision 1.2  1993/03/14  17:23:04  sev
 * Добавлено меню (не нагружено)
 *
 * Revision 1.1  1993/03/14  12:32:14  kas
 * Initial revision
 *
 * Revision 1.1  1993/03/04  07:41:26  kas
 * Initial revision
 *
 *
*/

static char rcsid[]="$Id: edmenu.c,v 1.3 1993/03/15 14:47:57 sev Exp $";

#define VCM_DEFS            /* Include Menu Defines and structures      */
#define VCGET_DEFS
#include <vcstdio.h>
#include <vcdbms.h>
#include <vced.h>

/* ------------------------------------------------------------------------ */

COUNT edmenu(vced)
VCED *vced;
{

	COUNT vcmhf3();
	COUNT vcmhf4();
	WPTR wxopen(),wopen();
	VCMENU *menuxnew(),*menunew();          /* Function declaration     */
	VCMENU *mainmenu;                       /* Declare menu pointer     */
	VCMENU *filesmenu;                      /*   "      "      "        */
	VCMENU *segmenu;                        /*   "      "      "        */
	VCMENU *polemenu;                       /*   "      "      "        */
	VCMENU *textmenu;                       /*   "      "      "        */

	MENUITEM *menuxitem();                  /* Function declaration     */
	/***************/
	COUNT cntrl_menu;
	COUNT cntrl_parm;
	COUNT cntrl_escape;
	COUNT cntrl_seg;
	COUNT cntrl_pole;
	COUNT cntrl_dir;
	/********************************************************************/
	/***************************  Menu Styles ***************************/
	/********************************************************************/

	addmstyle("STDHORIZ",HORIZONTAL|TITLECENTER|AUTOMENU|BORDERLESS,79,
	    vc.black+vc.bg*vc.white,                /* Normal      */
	vc.white+vc.bg*vc.black,                    /* Bar         */
	vc.black+vc.bg*vc.white,                    /* Unavailable */
	vc.white+vc.blue*vc.bg,                     /* Border      */
	0);                                         /* Permlvl     */
	addmstyle("STDVERT",VERTICAL|SHADOW,AUTO,
	    vc.black+vc.bg*vc.white,                /* Normal      */
	vc.white+vc.bg*vc.black,                    /* Bar         */
	vc.black+vc.bg*vc.white,                    /* Unavailable */
	vc.black+vc.bg*vc.white,                    /* Border      */
	0);                                         /* Permlvl     */

	/********************************************************************/
	/***************************  Open Menus ****************************/
	/********************************************************************/

	mainmenu  = menunew(0,0,NULLTEXT,"NO HELP","STDHORIZ");
	filesmenu = menunew(1,0,NULL,"NO HELP","STDVERT");
	textmenu  = menunew(1,8,NULL,"NO HELP","STDVERT");
	segmenu   = menunew(1,17,NULL,"NO HELP","STDVERT");
	polemenu  = menunew(1,27,NULL,"NO HELP","STDVERT");

	cntrl_parm = STRPARM+SENDTXT+HIDE;
	cntrl_escape = STRPARM+ESCAPE+SENDTXT+HIDE;

	cntrl_pole=MENU;
	cntrl_dir=STRPARM+ESCAPE+SENDTXT+HIDE;
	cntrl_menu=MENU;
 	cntrl_seg= MENU;

	/********************************************************************/
	/**************************  Add Menu Items *************************/
	/********************************************************************/

	menuxitem(mainmenu," Файл ",NULL,0,NULLFUNC,(TEXT *)filesmenu,
	    NULLTEXT,"NO HELP",cntrl_menu);
	menuxitem(mainmenu," Текст ",NULL,0,NULLFUNC,(TEXT *)textmenu,
	    NULLTEXT,"NO HELP",cntrl_menu);

	menuxitem(mainmenu," Сегмент ",NULL,0,NULLFUNC,(TEXT *)segmenu,
	    NULLTEXT,"NO HELP",cntrl_seg);
	menuxitem(mainmenu," Поле ",NULL,0,NULLFUNC,(TEXT *)polemenu,
	    NULLTEXT,"NO HELP",cntrl_pole);
	menuxitem(mainmenu," Cправочник ",NULL,0,NULLFUNC,(TEXT *)vced,
	    NULLTEXT,"NO HELP",cntrl_dir);
	menuxitem(mainmenu,"                  ",NULL,0,NULLFUNC,(TEXT *)vced,
	    NULLTEXT,"NO HELP",UNAVAILABLE);
	menuxitem(mainmenu," Помощь ",NULL,0,NULLFUNC,(TEXT *)vced,
	    NULLTEXT,"NO HELP",cntrl_escape);

	/************************** Files Menu Items ************************/

	menuxitem(filesmenu,"Открыть           ",NULL,0,NULLFUNC,(TEXT *)vced,NULL,
		  "NO HELP",cntrl_escape);
	menuxitem(filesmenu,"Закрыть           ",NULL,0,NULLFUNC,(TEXT *)vced,NULL,
		  "NO HELP",cntrl_escape);
	menuxitem(filesmenu,"Сохранить         ",NULL,0,NULLFUNC,(TEXT *)vced,NULL,
		  "NO HELP",cntrl_escape);
	menuxitem(filesmenu,"Сохранить...      ",NULL,0,NULLFUNC,(TEXT *)vced,NULL,
		 "NO HELP",cntrl_escape);
	menuxitem(filesmenu,NULLTEXT,NULL,0,NULLFUNC,NULL,NULL,
		 "NO HELP",SEPARATOR);
       	menuxitem(filesmenu,"Начальный сегмент ",NULL,0,NULLFUNC,(TEXT *)vced,NULL,
		 "NO HELP",cntrl_parm);
	menuxitem(filesmenu,"Компиляция        ",NULL,0,NULLFUNC,(TEXT *)vced,NULL,
		 "NO HELP",cntrl_parm);
	menuxitem(filesmenu,"Просмотр          ",NULL,0,NULLFUNC,(TEXT *)vced,NULL,
		 "NO HELP",cntrl_parm);
	menuxitem(filesmenu,NULLTEXT,NULL,0,NULLFUNC,NULL,NULL,
		 "NO HELP",SEPARATOR);
       	menuxitem(filesmenu,"Выход             ",NULL,0,NULLFUNC,(TEXT *)vced,NULL,
		 "NO HELP",cntrl_parm);
       	menuxitem(filesmenu,"Временный выход   ",NULL,0,NULLFUNC,(TEXT *)vced,NULL,
		 "NO HELP",cntrl_parm);

	/*************************** SEG Menu Items ************************/

	    menuxitem(segmenu,"Oтметить сегмент ",NULL,0,NULLFUNC,(TEXT *)vced,NULL,
		  "NO HELP",STRPARM+SENDTXT+ESCAPE);
	    menuxitem(segmenu,NULLTEXT,NULL,0,NULLFUNC,NULL,NULL,
		 "NO HELP",SEPARATOR);
	    menuxitem(segmenu,"Конец отметки сегмента ",NULL,0,NULLFUNC,(TEXT *)vced,
		      NULL,"NO HELP",STRPARM+SENDTXT+ESCAPE);
	    menuxitem(segmenu,"Oтмена сегмента",NULL,0,NULLFUNC,(TEXT *)vced,
		      NULL,"NO HELP",STRPARM+SENDTXT+ESCAPE);

	/*************************** POLE Menu Items **********************/

	menuxitem(polemenu,"Oтметить     ",NULL,0,NULLFUNC,(TEXT *)vced,NULLTEXT,
	    "NO HELP",cntrl_escape);
	menuxitem(polemenu,"Удалить      ",NULL,0,NULLFUNC,(TEXT *)vced,
	    NULLTEXT,"NO HELP",cntrl_escape);
	menuxitem(polemenu,"Привязать    ",NULL,0,NULLFUNC,(TEXT *)vced,
	    NULLTEXT,"NO HELP",cntrl_escape);
	menuxitem(polemenu,"О поле       ",NULL,0,NULLFUNC,(TEXT *)vced,
	    NULLTEXT,"NO HELP",cntrl_escape);
	menuxitem(polemenu,NULLTEXT,NULL,0,NULLFUNC,NULL,NULL,
		 "NO HELP",SEPARATOR);
	menuxitem(polemenu,"Конец отметки ",NULL,0,NULLFUNC,(TEXT *)vced,
		      NULLTEXT,"NO HELP",STRPARM+SENDTXT+ESCAPE);
	menuxitem(polemenu,"Oтменить      ",NULL,0,NULLFUNC,(TEXT *)vced,
		      NULLTEXT,"NO HELP",STRPARM+SENDTXT+ESCAPE);

	/*************************** TEXT Menu Items **********************/

	menuxitem(textmenu,"Поиск строки    ",NULL,0,NULLFUNC,(TEXT *)vced,NULLTEXT,
	    "NO HELP",cntrl_escape);
	menuxitem(textmenu,"Поиск с заменой ",NULL,0,NULLFUNC,(TEXT *)vced,
	    NULLTEXT,"NO HELP",cntrl_escape);
	menuxitem(textmenu,"На строку       ",NULL,0,NULLFUNC,(TEXT *)vced,
	    NULLTEXT,"NO HELP",cntrl_escape);
	menuxitem(textmenu,NULLTEXT,NULL,0,NULLFUNC,NULL,NULL,
		 "NO HELP",SEPARATOR);
	menuxitem(textmenu,"Команда системе ",NULL,0,NULLFUNC,(TEXT *)vced,
	    NULLTEXT,"NO HELP",cntrl_escape);

	vcmhk4=vcmhf4;                  /* Setup hook function              */
	vcmhk3=vcmhf3;                  /* Setup hook function              */
	return(vcm(mainmenu,vced));     /* Call menus using first menu      */
}


/************************************************************************/
/**************************  Hook Function ******************************/
/************************************************************************/
/*                                                                      */
/* This is executed after a key is press inside the menu system. One of */
/* the methods for customizing menus.                                   */
/*                                                                      */
/************************************************************************/

COUNT vcmhf3(m)
VCMENU *m;                                      /* Pointer to current menu  */
{
	if( (m->mstyle & VERTICAL) &&               /* If the current menu  */
	(m->mcalledby->mstyle & HORIZONTAL) )       /* was called by a hort.*/
	{                                           /* menu                 */
		if( m->mkeyhit == vckey.cur_right )  /* If the user hit cursor   */
		{                               /*  right                   */

			m->mresult=ESCAPE;              /* Set menu to exit         */
			ungetone( vckey.cur_right );    /* Put cur_right on key buf.*/
			if( m->mcalledby->mcuritmptr->enext->ectrl & MENU )
				ungetone( vckey.ret );  /* Put return on key buffer */
			return(TRUE);                   /* Tell menu to goto top of */
		}                               /* loop and the m->result   */ 
		    else                        /* will cause exit          */
		{                               /*                          */
			if(m->mkeyhit == vckey.cur_left)  /* If the user hit cursor */
			{                             /*  left                  */
				m->mresult=ESCAPE;          /* Set menu to exit         */
				ungetone( vckey.cur_left ); /* Put cur_left on key buf. */
				if(( m->mcalledby->mcuritmptr->eprev->ectrl & MENU ) &&
				    !(m->mcalledby->mcuritmptr==m->mcalledby->mitemhead) )
					ungetone( vckey.ret );      /* Put return on key buffer */
				return(TRUE);               /* Tell menu to goto top of */
			}                           /* loop and forgets the key */
		}                               /* entered and exits        */
	}                                       /*                          */
	return(0);                              /* return with no changes   */
}

/* ------------------------------------------------------------------------ */

COUNT vcmhf4(m)
VCMENU *m;                                      /* Pointer to current menu  */
{
	return(0);
}

/* ------------------------------------------------------------------------ */
