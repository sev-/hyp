/*
 *  $Id: shell.c,v 1.4 1994/07/11 12:31:07 sev Exp $
 *
 * ---------------------------------------------------------- 
 *
 * $Log: shell.c,v $
 * Revision 1.4  1994/07/11 12:31:07  sev
 * *** empty log message ***
 *
 * Revision 1.3  1993/09/28  16:25:40  sev
 * *** empty log message ***
 *
 * Revision 1.2  1993/09/28  12:58:34  sev
 * Добавлена куча всего
 *
 * Revision 1.1  1993/09/14  16:45:50  sev
 * Initial revision
 *
 *
 */
 
#include "shell.h"
#include <sys/utsname.h>

main()
{
/*  struct utsname unam;

  uname(&unam);
  if(strcmp(unam.sysname, "givi"))
  {
    puts("Only for givi!");
    exit(1);
  }
*/
  vcstart(0);

  need_update = 0;
  initmenus();
  view();

  vcend(1);

  return(0);
}

void view()
{
  char disp[80], val[80];
  int key, stop = 0;
  WPTR win1, win2;
  char buf[120];

  read_db();

  while( !stop )
  {
    win1 = xdrivesel(mainmenu, disp, val, " Главное меню ", &key, 5, 5);


    if(key == ESC)
      stop = 2;
    else
      switch(val[0])
      {
	case 'w':
	  write_files();
	  break;
	case 'n':
	  new_client();
	  break;
	case 'k':
	  kill_client();
	  break;
	case 'q':
	  stop = 1;
	  break;
	case 's':	/* statistic */
	  statist_client();
	  break;
	case 'c':	/* characteristics*/
	  change_client();
	  break;
      }
    wclose(win1);
  }
  write_db();
}

void read_db()
{
  clients = aloselitm();

  getselset(".clients", "r", clients);
}

void write_db()
{
  if(need_update)
    putselset(".clients", "w", clients);
}

char *select_wrote()
{
  SELSET *list;
  WPTR win;
  char disp[80], val[80];
  int key;

  list = aloselitm();

  getselset(".files", "r", list);
  win = xdrivesel(list, disp, val, " Какой файл был записан последним ",
			&key, 9, 12);
  wclose(win);

  return val;
}

void write_files()
{
  WPTR win;
  char disp[80], val[80];
  int key;
  int mode, drive;
  char tmp[80];

  win = xdrivesel(clients, disp, val, " Список заказчиков ", &key, 9, 12);
  wclose(win);
  if(key != ESC)
  {
    mode = ask_msg("Выберите формат записи", "", 2);
    drive = ask_msg("Выберите устройство", "", 3);
  }
  getdate(tmp);
}


void new_client()
{
  char name[70];
  char tmp[80];

  name[0] = 0;
  if(edit_string(12, 10, 60, 65, name, " Введите имя зказчика ",
							ATR_B, ATR_F))
  {
    sprintf(tmp, "%s %s", select_wrote(), "Нет");
    addselitm(clients, name, tmp);
    need_update = 1;
  }
}

void kill_client()
{
  WPTR win;
  int key;
  char disp[80], val[80];
  
  win = xdrivesel(clients, disp, val, " Список заказчиков ", &key, 9, 12);
  wclose(win);
  if(key != ESC && ask_msg("Вы действительно хотите удалить заказчика",
								disp, 1))
  {
    delselitm(clients, getselitm(clients, disp, val));
    need_update = 1;
  }
}

void statist_client()
{
  WPTR w;
  int key;
  char disp[80], val[80];
  char tmp[40], tmpdate[40];
  
  w = xdrivesel(clients, disp, val, " Список заказчиков ", &key, 9, 12);
  wclose(w);

  w = wxopen(10, 10, 20, 70, " Статистика клиента ",
				BORDER+BD1+CUSTOM+ACTIVE+CURSOR, 0, 0);
  atsay(1, 1, "Клиент: ");
  say(disp);
  sscanf(val, "%s %s", tmp, tmpdate);
  atsay(2, 1, "Последняя дата обновления: ");
  say(tmpdate);
  atsay(3, 1, "Последний файл: ");
  say(tmp);

  atsay(15, 9, "Нажмите любую клавишу");
  getone();
  wclose(w);
}

void change_client()
{
  WPTR w;
  int key;
  char disp[80], val[80];

  if(!passwd())
    return;
  w = xdrivesel(clients, disp, val, " Список заказчиков ", &key, 9, 12);
  wclose(w);
}

