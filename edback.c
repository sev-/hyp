/*
 *  $Id: edback.c,v 1.2 1993/04/20 16:04:12 sev Exp $
 *
 * ---------------------------------------------------------------------------
 *
 *  $Log: edback.c,v $
 *  Revision 1.2  1993/04/20 16:04:12  sev
 *  *** empty log message ***
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
 * ---------------------------------------------------------------------------
 *
 *   Name            vcedback.c - to file
 *
 *   Description     Make backup of original file
 *
 *   Return Value    Returns O.K.
 *
*/

#include <vcstdio.h>
#include <vcdbms.h>
#include <vced.h>

COUNT vcedback(edbuf)
VCEDBUF *edbuf;             /* Edit buffer                          */
    {                       /* ------------------------------------ */
    TEXT backname[60];      /* File backup name                     */
    TEXT *eptr;             /* End of line pointer for return char  */
    TEXT *vcgetenv();                   /* Get environment          */
    if(vcfaccess(edbuf->bfname,0))      /* If file does not exist   */
        return(0);                      /* Exit                     */
    if((eptr = vcgetenv("VCBACKUP")) != NULLTEXT)   /* If backup dir*/
        {                               /*                          */
        strcpy(backname,eptr);          /* Move to backup name      */
        strcat(backname,edbuf->bfname); /* Add file to name         */
        }                               /*                          */
    else                                /*                          */
        strcpy(backname,edbuf->bfname); /* Copy file name           */
    trim(backname);                     /* Trim name                */

    if(strlen(&backname[strrchr(backname, '/')]) > 10)
      backname[strlen(backname)-2] = '\0'; /* Обрезаем два последних символа */
    strcat(backname, ".b");

    if(!vcfaccess(backname,0))          /* If backup file exists    */
        vcferase(backname);             /* Erase backup file        */
    rename(edbuf->bfname,backname);     /* Make backup of file      */
    return(0);                          /* Return no errors         */
    }                                   /* ------------------------ */

