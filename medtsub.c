/*
 *  $Id: medtsub.c,v 1.2 1993/03/14 17:23:04 sev Exp $
 *
 *  --------------------------------------------------------------------------
 *
 *  $Log: medtsub.c,v $
 *  Revision 1.2  1993/03/14 17:23:04  sev
 *  Добавлено меню (не нагружено)
 *
 * Revision 1.1  1993/03/14  12:32:14  kas
 * Initial revision
 *
 *
 *
*/

#define VCGET_DEFS
#include <vcstdio.h>
#include <vcdbms.h>
#include <vced.h>
#include "mtdefs.h"

/**
*
*   Name            vcedit.c - edit
*                                   
*   Description     Head of the party.  This is the command center
*                   for the editor
*
*   Return Value    Returns O.K.
*                                   
**/

COUNT vcedit(vced)
VCED *vced;                 /* Pointer to edit structure            */
    {                       /* ------------------------------------ */
    VOID vcedwopen();       /* Declare functions used               */
    COUNT vcedptrbuf();     /*    "        "      "                 */
    COUNT vcedexec();       /*    "        "      "                 */
    if(vced->edwptr == (WPTR)0)         /* If window not open       */
        {                               /*                          */
        vcedwopen(vced);                /* Open window              */ 
        vcedprtbuf(vced);               /* Display buffer           */
        }                               /*                          */
    else                                /* Window open so select    */
        wselect(vced->edwptr);          /* to be sure on top        */
    while(vced->edmail < 9990)          /* While not ready to exit  */
        {                               /*                          */
        vcedupdate(vced);               /* Do any updates           */
        vced->edkey[2] = vced->edkey[1];/* Move key down one        */
        vced->edkey[1] = vced->edkey[0];/* Move key down one        */
        vced->edkey[0] = getone();      /* Get key                  */
        vcedexec(vced);                 /* Execute key pressed      */
        }                               /*                          */
    }                                   /* ------------------------ */
/* ---------------------------------------------------------------- */
/**
*
*   Name            vcedexec.c - execute command
*                                   
*   Description     Searches command list to see if character
*                   is command character if is executes command
*                   or inserts char 
*
*   Return Value    Returns O.K.
*                                   
**/

COUNT vcedexec(vced)
VCED *vced;                 /* Pointer to edit structure            */
    {                       /* ------------------------------------ */
    PFI command = NULLFUNC; /* Command to execute                   */
    PFI vcedfndcmd();       /* Find command in command structure    */
    if(vcedisch(vced->edkey[0]))        /* If valid printable char  */
        {                               /*                          */
        if(vced->edmode & VCEDOVER)     /* Over write mode ?        */
            vcedover(vced);             /* Over write the character */
        else                            /* Else                     */
            vcedinsert(vced);           /* Insert the character     */
        }                               /*                          */
    else if((command = vcedfndcmd(vced)) != NULLFUNC) /* Find cmd   */
        (*command)(vced);               /* Execute command          */
    else                                /*                          */
        bell();                         /* Inform user              */
    return(0);                          /* Return O.K.              */
    }                                   /* ------------------------ */

COUNT vcedisch(key)
COUNT key;
    {
    COUNT retval = TRUE;
    if((key < ' ') || (key > (unsigned char)'\377'))
        retval = FALSE;
    return(retval);
    }

/* ---------------------------------------------------------------- */
/**
*
*   Name            vcedfnd.c - find
*                                   
*   Description     Finds a key in the command structure.  Called
*                   by vcedexec to locate command to exec.
*
*   Return Value    Returns pointer to command for key if available
*                                   
**/

PFI vcedfndcmd(vced)
VCED *vced;                 /* Pointer to edit structure            */
    {                       /* ------------------------------------ */
    VCEDCMD *curcmd;        /* Current command structure            */
    PFI retval = NULLFUNC;  /* Return value                         */
    curcmd = &vcedcmds[0];              /* Start at beginning       */
    while(curcmd->cmdname != NULLTEXT)  /* While more commands      */
        {                               /*                          */
        if(vced->edkey[0] == curcmd->cmdkey) /* Is this the key?    */
            {                           /*                          */
            retval = curcmd->cmdloc;    /* Assign function address  */
            break;                      /* and return               */
            }                           /*                          */
        curcmd++;                       /* Point to next member     */
        }                               /*                          */
    return(retval);                     /* Return function address  */
    }                                   /* ------------------------ */
