/*
 *  $Id: edsave.c,v 1.3 1993/08/18 11:44:08 sev Exp $
 *
 * ---------------------------------------------------------------------------
 *
 * $Log: edsave.c,v $
 * Revision 1.3  1993/08/18 11:44:08  sev
 * Теперь на экране отображается копирование файлов
 *
 * Revision 1.2  1993/06/08  12:10:31  sev
 * Вот теперь этот глюк действительно убран
 *
 * Revision 1.1  1993/05/24  15:14:24  sev
 * Initial revision
 *
 *
 */
/**
*
*   Vitamin C Multi-User version 1.1
*   Copyright (c) 1985-1990
*   Creative Programming Consultants, Inc.
*   P.O. Box 112097
*   Carrollton, Texas  75006
*   (214) 416-6447
*   ALL RIGHTS RESERVED
*   Last Modification: .4 on 8/31/90 at 21:51:40
*
*   Name            vcedsave.c - save
*                                   
*   Description     Save buffer to original location
*
*   Return Value    Returns O.K.
*                                   
**/

#include <vcstdio.h>
#include <vcdbms.h>
#include <vced.h>

TEXT *genback();

VOID vcedsave(edbuf)
VCEDBUF *edbuf;             /* Edit buffer                          */
    {
      char directory[256];
      char *tmp1;
      FILE *save;
      char buf[256];

                      /* ------------------------------------ */
    if(!edbuf->bfchange)                /* If buffer not changed    */
        return;                         /* Return nothing to save   */
    if(edbuf->bfctrl & VCED2BUF)        /* If write to buffer       */
        edbuf->bfchange = vcedtobuf(edbuf); /* Write to buffer      */
    else                                /* else                     */
        {                               /*                          */
        if(vcedval.bckupfil)            /* If backup file wanted    */
            vcedback(edbuf);            /* Make backup of file      */
        edbuf->bfchange = vcedtofil(edbuf); /* Write to file        */
	if((tmp1 = (char *)getenv("HOMEZAKON")) != (char *)NULL)
	{
	  sprintf(directory, "%s/diffs/zakon/%s", tmp1, edbuf->bfname);
	  if((save = fopen(directory, "r")) == (FILE *)NULL)
	  {
	    fclose(save);
	    strcpy(buf, genback(edbuf));
	    filecopy(buf, directory);
	  }
	}                               /*                          */
	}        
    }                                   /* ------------------------ */

