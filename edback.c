/**
*
*   Vitamin C Multi-User version 1.1
*   Copyright (c) 1985-1990
*   Creative Programming Consultants, Inc.
*   P.O. Box 112097
*   Carrollton, Texas  75006
*   (214) 416-6447
*   ALL RIGHTS RESERVED
*   Last Modification: .4 on 8/31/90 at 21:50:15
*
*   Name            vcedback.c - to file
*                                   
*   Description     Make backup of original file
*
*   Return Value    Returns O.K.
*                                   
**/

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

