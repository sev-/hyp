/*
 *  $Id: edinitstr.h,v 1.1 1993/04/06 14:14:07 sev Exp $
 *
 * ---------------------------------------------------------- 
 *
 * $Log: edinitstr.h,v $
 * Revision 1.1  1993/04/06 14:14:07  sev
 * Initial revision
 *
 * Revision 4.2  1993/03/27  12:08:16  kas
 * *** empty log message ***
 *
 * Revision 4.1  1993/03/25  11:01:23  kas
 * *** empty log message ***
 *
 * Revision 3.1  1993/03/24  16:39:49  kas
 * ..
 *
 * Revision 1.5  1993/03/17  16:34:20  sev
 * Заменил константу, теперь перелистывание при стрелочках на пять строк
 * , а не на одну, что удобно на терминалах.
 *
 * Revision 1.4  1993/03/17  15:53:58  sev
 * Была большая беда с отметкой страницы. Виновата измененная строка
 * количества символов в строке (там был буфер меньшей длины и вылетало
 * segmentation violation или bus error. Теперь если проблемы этого
 * плана, то в файле vced.h есть MAXLENSTR. Вот ее и надо ставитьь.
 *
 * Revision 1.3  1993/03/15  10:55:59  sev
 * Увеличен размер строки до 1024 байт. Переключатель насчет
 * special characters (он последний) лучше не трогать (печальная картина)
 *
 * Revision 1.2  1993/03/13  12:07:21  sev
 * Исправлены ошибки в delsels, убран ПФ4 и ПФ3
 *
 * Revision 1.1  1993/03/12  15:44:23  sev
 * Init
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

   (TEXT *)"Новый файл.                         ",NEW_FILE,ednewf,
   (TEXT *)"Следующий файл.                     ",NEXT_FILE,ednextf,
   (TEXT *)"Запись файла.                       ",SAVE_BUF,edsavef,
   (TEXT *)"Удалить текущий файл из окна.       ",DEL_FILE,eddelf,
   (TEXT *)"Cписок файлов  в редакторе          ",LISTWDO,edlistwdo,
   (TEXT *)"Cписок файлов  в каталоге           ",FILES,edfiles,

   (TEXT *)"Отметить поле.                  F6  ",POLE,edhmark,
   (TEXT *)"Привязать поле.                 F8  ",PASTE,edpaste,
   (TEXT *)"Удалить поле.                   F7  ",DEL_POLE,ed_del_conc,
   (TEXT *)"Информация о поле.                  ",INF_CONC,edinf,
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
	(COUNT)   1,            /* Number to move when shifting up/down */
	(COUNT)   0,            /* Default Right Margin                 */
	(COUNT)   0,            /* Default Left Margin                  */
	(COUNT)   0,            /* Display control characters           */
	(COUNT)   1             /* Backup the file when saving          */
};                              /* ------------------------------------ */

