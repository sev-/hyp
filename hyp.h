/*
 *  $Id: hyp.h,v 1.4 1993/04/22 15:33:35 sev Exp $
 *
 * ---------------------------------------------------------- 
 *
 * $Log: hyp.h,v $
 * Revision 1.4  1993/04/22 15:33:35  sev
 * Добавлена еще одна функция - запись сегмента в файл
 *
 * Revision 1.3  1993/03/05  17:05:33  sev
 * теперь перелистывание страниц по сегментам
 *
 * Revision 1.2  1993/03/04  10:52:15  sev
 * *** empty log message ***
 *
 * Revision 1.1  1993/03/04  09:50:53  sev
 * Initial revision
 *
 *
*/

#define VCKEY_DEFS
#define N_STACK		50	/* глубина стека                */
#define N		23	/* Размер       окна            */
#define BUF		1024
#define PATH		80

#define ATTR_F vc.blue+vc.bg*vc.black	/* атрибуты  неактивного поля  */
#define ATTR_B vc.black+vc.bg*vc.white	/* атрибуты    активного поля  */

#include <vcstdio.h>

#define	TREE_UP_KEY		HOME_KEY
#define	HOME_FILE_KEY		INS_KEY
#define	END_FILE_KEY		END_KEY
#define	PAGE_UP_KEY		PGUP
#define	PAGE_DOWN_KEY		PGDN
#define	MOVE_UP_KEY		CUR_UP
#define	MOVE_DOWN_KEY		CUR_DOWN
#define	MOVE_RIGHT_KEY		CUR_RIGHT
#define	MOVE_LEFT_KEY		CUR_LEFT
#define	GO_TREE			RET
#define	HELP_KEY		F1
#define	FORWARD_SEARCH_KEY	'/'
#define	BACKWARD_SEARCH_KEY	'?'
#define PRINT_SEGMENT		F4

char adress[40];	/*  адрес сегмента в активном поле */
char path_file[PATH];	/*     имя  файла  в активном поле */
char current_seg[40];	/*  адрес текущего сегмента        */
char buf_pg[N][BUF];	/*  буфер для одной страницы       */
char *chr;
char *help_string; 	/* строка в нижней части экрана */
char old_pattern[260];	/* старый шаблон поиска		*/

long end_super;		/* offset в файле конца	 текущей страницы */
int  pg_open, pg_close;	/* количество { и } в странице            */
int  _open, _close;	/* количество { и } в сегменте            */
int  col_1, col_2, row;	/* координаты   активного поля            */
int  col_str;		/* количество строк в странице            */

int  level;		/* Oпределяет вложенность скобок, при     */
			/* level = 0 конец сегмента.                */
			/* (level = pg_open-pg_close)               */
int wdo1; 		/* основное окно			*/


int get_string();
int read_screen();
int help_msg();	/* функция вызывается по F1 */
void init_help(); /* установка помощи для каждого терминала */
int  print_pg();
int  read_print_pg();
char *strchr();
char *strstr();
char *getenv();
int  string();
void forward_search();
int edit_string();
char *find();
void show_finded();
void backward_search();
void find_begin_seg();
void find_end_seg();
void print_segment();
