/*
 * $Id: crtlstfl.c,v 1.2 1993/04/20 16:04:12 sev Exp $
 *
 * ---------------------------------------------------------------------------
 *
 * $Log: crtlstfl.c,v $
 * Revision 1.2  1993/04/20 16:04:12  sev
 * *** empty log message ***
 *
 * Revision 1.3  1993/04/15  15:05:51  kas
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

#define SEL_DEFS
#include <vcstdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

/* --------------------- Create list of files ----------------------------- */

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
