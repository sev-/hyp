/*
 *  $Id: edmenu.c,v 1.1 1993/03/14 12:32:14 kas Exp $
 *
 * ---------------------------------------------------------- 
 *
 * $Log: edmenu.c,v $
 * Revision 1.1  1993/03/14 12:32:14  kas
 * Initial revision
 *
 * Revision 1.1  1993/03/04  07:41:26  kas
 * Initial revision
 *
 *
*/

static char rcsid[]="$Id: edmenu.c,v 1.1 1993/03/14 12:32:14 kas Exp $";

#define VCM_DEFS                /* Include Menu Defines and structures      */

#include <vcstdio.h>

/* ------------------------------------------------------------------------ */

COUNT menu(vced,mode)
VCED *vced;
COUNT mode;
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
	COUNT ednewf();
	COUNT ednextf();
	COUNT eddelf();
	COUNT edsavef();
	COUNT edir();
	COUNT vcedabort();
	COUNT edconf();
	COUNT edhmark();
	COUNT edvmark();
	COUNT ed_del_conc();
	COUNT edpaste();
	COUNT edendmark();
	COUNT edunmark();
	COUNT edhelp();
	COUNT edinf();
        COUNT edseginf();
	COUNT edfindu();
       	COUNT edfindch();
	COUNT edsys();
        COUNT edgotom();
        COUNT edcompil();
        COUNT edshow();
	COUNT eddrawch();
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
	filesmenu = menunew(2,0,NULL,"NO HELP","STDVERT");
	textmenu  = menunew(2,10,NULL,"NO HELP","STDVERT");
	segmenu   = menunew(2,19,NULL,"NO HELP","STDVERT");
	polemenu  = menunew(2,32,NULL,"NO HELP","STDVERT");

	cntrl_parm = STRPARM+SENDTXT+HIDE;
	cntrl_escape = STRPARM+ESCAPE+SENDTXT+HIDE;

	if(mode==POLE || mode==SEGM)
	{
	  ungetone(RET);
	  cntrl_dir=cntrl_menu=UNAVAILABLE;
	}
	else
	{                /*
	    if(stat_hyp==PRIV_SEGWDO)
	    {
		ungetone(RET);
		cntrl_pole=UNAVAILABLE;
		cntrl_menu=UNAVAILABLE;
		cntrl_dir=UNAVAILABLE;
	     }

	     else
	     {
			  */
		 cntrl_pole=MENU;
		 cntrl_dir=STRPARM+ESCAPE+SENDTXT+HIDE;
		 cntrl_menu=MENU;

		 cntrl_seg=MENU;
	}
	if(mode==POLE)
	{
	     cntrl_pole=MENU;
	     cntrl_escape=UNAVAILABLE;
	     cntrl_seg=UNAVAILABLE;
	}
	if(mode==SEGM)
	{
	     cntrl_seg=MENU;
	     cntrl_escape=UNAVAILABLE;
	     cntrl_pole=UNAVAILABLE;
	}

	/********************************************************************/
	/**************************  Add Menu Items *************************/
	/********************************************************************/

	menuxitem(mainmenu,"  Файл  ",NULL,0,NULLFUNC,(TEXT *)filesmenu,
	    NULLTEXT,"NO HELP",cntrl_menu);
	menuxitem(mainmenu," Текст ",NULL,0,NULLFUNC,(TEXT *)textmenu,
	    NULLTEXT,"NO HELP",cntrl_menu);

	menuxitem(mainmenu,"  Сегмент  ",NULL,0,NULLFUNC,(TEXT *)segmenu,
	    NULLTEXT,"NO HELP",cntrl_seg);
	menuxitem(mainmenu,"  Поле ",NULL,0,NULLFUNC,(TEXT *)polemenu,
	    NULLTEXT,"NO HELP",cntrl_pole);
	menuxitem(mainmenu," Cправочник ",NULL,0,edir,(TEXT *)vced,
	    NULLTEXT,"NO HELP",cntrl_dir);
	menuxitem(mainmenu,"  Помощь  ",NULL,0,edhelp,(TEXT *)vced,
	    NULLTEXT,"NO HELP",cntrl_escape);
	menuxitem(mainmenu,"  Выход  ",NULL,0,vcedabort,(TEXT *)vced,
	    NULLTEXT,"NO HELP",cntrl_escape);

	/************************** Files Menu Items ************************/

	menuxitem(filesmenu,"Загрузить новый файл       F12 ",NULL,0,ednewf,(TEXT *)vced,NULL,
		  "NO HELP",cntrl_escape);
	menuxitem(filesmenu,"Список файлов              F11 ",NULL,0,edlistwdo,(TEXT *)vced,NULL,
		  "NO HELP",cntrl_escape);
	menuxitem(filesmenu,"Список файлов в каталоге   F13 ",NULL,0,edfiles,(TEXT *)vced,NULL,
		  "NO HELP",cntrl_escape);
	menuxitem(filesmenu,"Сохранить текущий файл     F16 ",NULL,0,edsavef,(TEXT *)vced,NULL,
		 "NO HELP",cntrl_escape);
	menuxitem(filesmenu,"Удалить файл из окна       F15 ",NULL,0,eddelf,(TEXT *)vced,NULL,
		 "NO HELP",cntrl_escape);
	menuxitem(filesmenu,"Переход к следующему файлу F14 ",NULL,0,ednextf,(TEXT *)vced,NULL,
		 "NO HELP",cntrl_escape);
	menuxitem(filesmenu,NULLTEXT,NULL,0,NULLFUNC,NULL,NULL,
		 "NO HELP",SEPARATOR);
       	menuxitem(filesmenu,"Начальный  сегмент            ",NULL,0,edconf,(TEXT *)vced,NULL,
		 "NO HELP",cntrl_parm);
	menuxitem(filesmenu,"Компиляция                    ",NULL,0,edcompil,(TEXT *)vced,NULL,
		 "NO HELP",cntrl_parm);
	menuxitem(filesmenu,"Просмотр                      ",NULL,0,edshow,(TEXT *)vced,NULL,
		 "NO HELP",cntrl_parm);

	/*************************** SEG Menu Items ************************/

	if(mode==SEGM)
	{
	    menuxitem(segmenu,"Oтметить сегмент ",NULL,0,edvmark,(TEXT *)vced,NULL,
		  "NO HELP",UNAVAILABLE);
	    menuxitem(segmenu,NULLTEXT,NULL,0,NULLFUNC,NULL,NULL,
		 "NO HELP",SEPARATOR);
	    menuxitem(segmenu,"Конец отметки сегмента ",NULL,0,edendmark,(TEXT *)vced,
		      NULL,"NO HELP",STRPARM+SENDTXT+ESCAPE);
	    menuxitem(segmenu,"Oтмена сегмента",NULL,0,edunmark,(TEXT *)vced,
		      NULL,"NO HELP",STRPARM+SENDTXT+ESCAPE);
	 }
	else
	    menuxitem(segmenu,"Oтметить сегмент       F9 ",NULL,0,edvmark,(TEXT *)vced,NULL,
		  "NO HELP",cntrl_escape);
	
	    menuxitem(segmenu,"Информация о сегменте  УПР-w ",NULL,0,edseginf,(TEXT *)vced,NULL,
		     "NO HELP",cntrl_escape);
	/*************************** POLE Menu Items **********************/

	menuxitem(polemenu,"Oтметить поле     F6 ",NULL,0,edhmark,(TEXT *)vced,NULLTEXT,
	    "NO HELP",cntrl_escape);
	menuxitem(polemenu,"Удалить поле      F7 ",NULL,0,ed_del_conc,(TEXT *)vced,
	    NULLTEXT,"NO HELP",cntrl_escape);
	menuxitem(polemenu,"Привязать поле    F8 ",NULL,0,edpaste,(TEXT *)vced,
	    NULLTEXT,"NO HELP",cntrl_escape);
	menuxitem(polemenu,"Информация о поле УПР-o ",NULL,0,edinf,(TEXT *)vced,
	    NULLTEXT,"NO HELP",cntrl_escape);     	if(mode==POLE)
	{
	    menuxitem(polemenu,NULLTEXT,NULL,0,NULLFUNC,NULL,NULL,
		 "NO HELP",SEPARATOR);
	    menuxitem(polemenu,"Конец отметки поля",NULL,0,edendmark,(TEXT *)vced,
		      NULLTEXT,"NO HELP",STRPARM+SENDTXT+ESCAPE);
	    menuxitem(polemenu,"Oтмена поля",NULL,0,edunmark,(TEXT *)vced,
		      NULLTEXT,"NO HELP",STRPARM+SENDTXT+ESCAPE);
	 }
	/*************************** TEXT Menu Items **********************/

	menuxitem(textmenu,"Поиск строки      УПР-f ",NULL,0,edfindu,(TEXT *)vced,NULLTEXT,
	    "NO HELP",cntrl_escape);
	menuxitem(textmenu,"Поиск с заменой   УПР-u ",NULL,0,edfindch,(TEXT *)vced,
	    NULLTEXT,"NO HELP",cntrl_escape);
	menuxitem(textmenu,"Перейти на строку УПР-g ",NULL,0,edgotom,(TEXT *)vced,
	    NULLTEXT,"NO HELP",cntrl_escape);
	menuxitem(textmenu,NULLTEXT,NULL,0,NULLFUNC,NULL,NULL,
		 "NO HELP",SEPARATOR);
	menuxitem(textmenu,"Команда системе   УПР-k ",NULL,0,edsys,(TEXT *)vced,
	    NULLTEXT,"NO HELP",cntrl_escape);
	menuxitem(textmenu,NULLTEXT,NULL,0,NULLFUNC,NULL,NULL,
		 "NO HELP",SEPARATOR);
	menuxitem(textmenu,"Рисование символов  F20 ",NULL,0,eddrawch,(TEXT *)vced,
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
