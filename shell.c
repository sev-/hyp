/*
 *  $Id: shell.c,v 1.1 1993/09/14 16:45:50 sev Exp $
 *
 * ---------------------------------------------------------- 
 *
 * $Log: shell.c,v $
 * Revision 1.1  1993/09/14 16:45:50  sev
 * Initial revision
 *
 *
 */
 
#include "shell.h"

main()
{
  vcstart(0);

  view();

  vcend(1);
}

void view()
{
  char disp[80], val[80];

  read_db();

  drivesel(clients, disp, val, " Window ");

  atsay(10, 0, disp);
  atsay(11, 0, val);
}

void read_db()
{
  clients = aloselitm();

  getselset(".clients", "r", clients);
}
