/*
 *  $Id: menus.c,v 1.1 1993/09/28 12:58:34 sev Exp $
 *
 * ---------------------------------------------------------- 
 *
 * $Log: menus.c,v $
 * Revision 1.1  1993/09/28 12:58:34  sev
 * Initial revision
 *
 *
 */
 
#include "shell.h"

#define MAINMENUSIZE (sizeof(mainmenustr)/sizeof(char *))

char *mainmenustr[] =
{
  "Записать законы заказчику", "w",
  "Новый заказчик", "n",
  "Удалить заказчика", "k",
  "Startpatch", "s",
  "Endpatch", "e",
  "Выйти из программы", "q"
};

void initmenus()
{
  int i;

  mainmenu = aloselitm();
  
  for(i = 0; i < MAINMENUSIZE; i += 2)
    addselitm(mainmenu, mainmenustr[i], mainmenustr[i+1]);
}
