/*
 *  $Id: addrefer.c,v 1.2 1993/04/20 16:04:12 sev Exp $
 *
 * ---------------------------------------------------------------------------
 *
 * $Log: addrefer.c,v $
 * Revision 1.2  1993/04/20 16:04:12  sev
 * *** empty log message ***
 *
 * Revision 1.4  1993/04/19  16:36:31  kas
 * *** empty log message ***
 *
 * Revision 1.3  1993/04/15  07:21:41  kas
 * *** empty log message ***
 *
 * Revision 1.3  1993/04/15  07:21:41  kas
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

/* -------------------- Добавление ссылки к сегменту ---------------------- */

int add_refer(name, delta)
char *name;
int delta;
{
	SELSET *tmp;
	TEXT value[128];
	TEXT display[256];
	int num;

	tmp = dirr;

	while(tmp && strncmp(tmp->ivalue, name, strlen(name)))
		tmp = tmp->inext;

	if(tmp)	/* Нашли сегмент */
	{
		num = atoi(strrchr(tmp->ivalue, '%')+1);
		if(delta)
		  num += delta;
		else
		  num = 0;
		    
		sprintf(value, "%s%%%d", name, num);
		strcpy(display, tmp->idisplay);

		repselset(dirr, tmp, display, value);
	}
}

