/*
 *  $Id: shell.c,v 1.2 1993/09/28 12:58:34 sev Exp $
 *
 * ---------------------------------------------------------- 
 *
 * $Log: shell.c,v $
 * Revision 1.2  1993/09/28 12:58:34  sev
 * Добавлена куча всего
 *
 * Revision 1.1  1993/09/14  16:45:50  sev
 * Initial revision
 *
 *
 */
 
#include "shell.h"

main()
{
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
  char name[80];
  char buf[120];
  char *wrote;

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
	  win2 = xdrivesel(clients, disp, val, " Список заказчиков ", &key,
								9, 12);
	  wclose(win2);
	  break;
	case 'n':
	  name[0] = 0;
	  if(edit_string(12, 10, 60, 65, name, " Введите имя зказчика ",
							ATR_B, ATR_F))
	  wrote = select_wrote();
	  addselitm(clients, name, wrote);
	  need_update = 1;
	  break;
	case 'k':
	  win2 = xdrivesel(clients, disp, val, " Список заказчиков ", &key,
								9, 12);
	  wclose(win2);
	  if(ask_msg("Вы действительно хотите удалить заказчика", disp, 1))
	  {
	    delselitm(clients, getselitm(clients, disp, val));
	    need_update = 1;
	  }
	  break;
	case 'q':
	  stop = 1;
	  break;
	case 's':	/* startpatch */
	case 'e':	/* endpatcch */
	  if(!passwd())
	    bell();
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

int passwd()
{
  WPTR w;
  static char passwd[] = "e_cpaepesd";
  int ok = 1;
  int i;
  

  w = wxopen(9, 28, 11, 41, " Password ", BORDER+BD2+CUSTOM+ACTIVE+CURSOR,
					0, 0);
  vcputc(' ', ATR_F);
  for(i = 9; ok && i != -2;)
  {
    if(getone() != passwd[i])
    {
      atsay(0, 0, "   ERROR  ");
      sleep(1);
      ok = 0;
    }
    if(ok)
      vcputc('*', ATR_F);

    if((i -= 2) == -1)
      i = 8;
  }

  if(ok)
  {
    atsay(0, 0, "    OK     ");
    sleep(1);
  }
  wclose(w);

  return ok;
}

