/*
 *  $Id: edpaste.c,v 1.4 1993/04/20 16:04:12 sev Exp $
 *
 * ---------------------------------------------------------------------------
 *
 * $Log: edpaste.c,v $
 * Revision 1.4  1993/04/20 16:04:12  sev
 * a
 *
 * Revision 1.5  1993/04/19  16:36:31  kas
 * *** empty log message ***
 *
 * Revision 1.4  1993/04/15  07:21:41  kas
 * *** empty log message ***
 *
 * Revision 1.4  1993/04/15  07:21:41  kas
 * *** empty log message ***
 *
 * Revision 1.3  1993/04/13  13:50:41  kas
 * *** empty log message ***
 *
 * Revision 1.2  1993/04/13  09:21:13  kas
 * Удален вызов privmsg
 *
 * Revision 1.1  1993/04/12  15:13:06  kas
 * Initial revision
 *
 *
 */

#include "vced.h"

/* ------------------------------------------------------------------------ */

COUNT edpaste(vced)
VCED *vced;
{

	VCEDLINE *cline;
	int col_str;
	int i=0;
	int flag_is=0;
	char *strchr();

	if(stat_hyp==PRIV_SEGWDO)
		return(0);
	if(vced->edcline == (DBDP)0)            /* If not on real line      */
		vcedmkreal(vced);               /* Make the current loc real*/
	cline=vcedmline(vced->edbuffer,vced->edcline);
	edsetfta(vced, vced->edcline, cline,from,to,stat_);
	col_str=vced->edcchar;

	while( from[i]!=-1)
	{
		if(col_str >= from[i] && col_str <= to[i] )
		{
			flag_is=1;
			break;
		}
		i++;
	}
	if(flag_is==1)
	{
		if(eddir(vced)==0)
			ask_msg("Привязка не произведена.",0);
		else
		{
			ed_del_conc(vced);
			ed_ins_conc(vced,vced->edcline,from[i],to[i],privyz.name_seg,privyz.name_file,1);
			add_refer(privyz.name_seg, 1);
			if(putselset(dir_file, "w", dirr))
			{
				vcend(CLOSE);
				execlp("clear","clear",(char *)NULL);
				printf("Не могу записать в файл %s\n", dir_file);
				exit(1);
			}
			ask_msg("Привязка произведена успешно.",0);
		}
	}
	else
		ask_msg("Установите курсор на поле.",0);
	return GOOD;
}

/* ------------------------------------------------------------------------ */
