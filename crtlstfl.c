/*
 * $Id: crtlstfl.c,v 1.1 1993/04/06 14:14:07 sev Exp $
 *
 * ---------------------------------------------------------------------------
 *
 * $Log: crtlstfl.c,v $
 * Revision 1.1  1993/04/06 14:14:07  sev
 * Initial revision
 *
 * Revision 1.4  1993/04/01  14:10:29  kas
 * *** empty log message ***
 *
 * Revision 1.3  1993/03/31  07:26:09  kas
 * not worked
 *
 * Revision 1.2  1993/03/25  16:14:11  kas
 * *** empty log message ***
 *
 * Revision 1.1  1993/03/14  12:32:14  kas
 * Initial revision
 *
*/

#define SEL_DEFS
#include <vcstdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

SELSET *crt_lst_file(path,pattern)
char   *path;                           /* name of directory                */
char   *pattern;                        /* pattern of selection             */
{
  SELSET *list;
  DIR    *dird;
  struct  dirent *dirp;
  char    tbuf[80];
  struct  stat STATBUF[1];

  list = aloselitm();

  dird = opendir(path);
  while ((dirp=readdir(dird)) != NULL)
	{
		strcpy(tbuf,(dirp->d_name)-2);
		stat(tbuf,STATBUF);
		if((STATBUF->st_mode & S_IFMT) == S_IFREG)
			if (wildmat(tbuf,pattern))
				{
				addselitm(list,tbuf,tbuf);
				}
	}
  return(list);
}
