/*
 *  $Id: edinitstr.h,v 1.4 1993/07/07 12:12:47 sev Exp $
 *
 * --------------------------------------------------------------------------
 *
 * $Log: edinitstr.h,v $
 * Revision 1.4  1993/07/07 12:12:47  sev
 * теперь перелистывает через пять строк
 *
 * Revision 1.3  1993/04/22  13:23:26  sev
 * dir записывается один раз
 * \
 *
 * Revision 1.2  1993/04/20  16:04:12  sev
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

VCEDCMD vcedcmds[] =
{
   (TEXT *)"cursor_down                         ",CUR_DOWN,vcedmvdown,
   (TEXT *)"cursor_left                         ",CUR_LEFT,vcedmvleft,
   (TEXT *)"cursor_right                        ",CUR_RIGHT,vcedmvright,
   (TEXT *)"cursor_up                           ",CUR_UP,vcedmvup,
   (TEXT *)"return                              ",RET,vcedreturn,
   (TEXT *)"return                              ",REF,edrefresh,
   (TEXT *)"команда UNIX                        ",SYS_COM,edsys,
   (TEXT *)"Вставить строку                     ",INS_LINE,edinsline,
   (TEXT *)"Выход.Отмена режимов.               ",EXIT,vcedabort,
   (TEXT *)"Помощь                              ",HELP,edhelp,
   (TEXT *)"Обратная табуляция                  ",BTAB,vcedbktab,
   (TEXT *)"Табуляция                           ",TAB,vcedtab,
   (TEXT *)"Удаление символа слева от курсора   ",BACKSP,vcedbksp,
   (TEXT *)"Удаление  cимвола под курсором      ",DEL_KEY,vceddelete,
   (TEXT *)"Удалить строку.                     ",DEL_LINE,eddelline,
   (TEXT *)"Очистить строку от курсора до конца ",CLEAR_ENDOFL,eddelendl,
   (TEXT *)"Отметить страницу.                  ",PG_MARK,edmarkpg,
   (TEXT *)"Вставка / замена.                   ",INS_KEY,vcedinstog,
   (TEXT *)"Конец строки/экрана/файла.          ",END_KEY,edend,
   (TEXT *)"Начало строки /экрана/файла.        ",HOME_KEY,vcedhome,
   (TEXT *)"Страница вниз.                      ",PGDN,vcedpgdn,
   (TEXT *)"Страница вверх.                     ",PGUP,vcedpgup,

   (TEXT *)"Новый файл.                     F12 ",NEW_FILE,ednewf,
   (TEXT *)"Следующий файл.                 F14 ",NEXT_FILE,ednextf,
/* (TEXT *)"Запись файла.                   F16 ",SAVE_BUF,edsavef, */
   (TEXT *)"Удалить текущий файл из окна.   F15 ",DEL_FILE,eddelf,
   (TEXT *)"Cписок файлов  в редакторе      F11 ",LISTWDO,edlistwdo,
   (TEXT *)"Cписок файлов  в каталоге       F13 ",FILES,edfiles,

   (TEXT *)"Отметить поле.                  F6  ",POLE,edhmark,
   (TEXT *)"Привязать поле.                 F8  ",PASTE,edpaste,
   (TEXT *)"Удалить поле.                   F7  ",DEL_POLE,ed_del_conc,
   (TEXT *)"Информация о поле.              ^-O ",INF_CONC,edinf,
   (TEXT *)"Справочник.                     F10 ",DIR_SEG,edir,

   (TEXT *)"Отметить сегмент.               F9  ",SEGM,edvmark,
   (TEXT *)"Создание файла config           F17 ",CONF,edconf,
   (TEXT *)"Компиляция                      F18 ",COMP,edcompil,
   (TEXT *)"Просмотр (hyp)                  F19 ",VIEW,edshow,
   (TEXT *)"Команда UNIX                    F20 ",SHELL,edsys,

	    NULLTEXT,0,(PFI)0,
};

VCEDVAL vcedval =
{                               /* ------------------------------------ */
	(COUNT) MAXLINESIZE-1,  /* Maximum line length                  */
	(COUNT) 10,             /* Number of Pages to buffer            */
	(COUNT) 100,            /* Number of slots per page             */
	(COUNT)   0,            /* Soft Return                          */
	(COUNT)  13,            /* Hard Return                          */
	(COUNT)  14,            /* Justified Return                     */
	(COUNT)   0,            /* Use real tabs                        */
	(COUNT)   8,            /* Tab size                             */
	(COUNT)'\t',            /* Tab character                        */
	(COUNT)   176,          /* Tab space character                  */
	(COUNT)   35,           /* Number to move when shifting left    */
	(COUNT)   5,            /* Number to move when shifting up/down */
	(COUNT)   0,            /* Default Right Margin                 */
	(COUNT)   0,            /* Default Left Margin                  */
	(COUNT)   0,            /* Display control characters           */
	(COUNT)   1             /* Backup the file when saving          */
};                              /* ------------------------------------ */

