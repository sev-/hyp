/*
 *  $Id: medtsub.c,v 1.1 1993/03/14 12:32:14 kas Exp $
 *
 *  --------------------------------------------------------------------------
 *
 *  $Log: medtsub.c,v $
 *  Revision 1.1  1993/03/14 12:32:14  kas
 *  Initial revision
 *
 *
 *
*/

#define VCGET_DEFS
#include <vcstdio.h>
#include <vcdbms.h>
#include <vced.h>

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

/* ---------------------------------------------------------------- */
/**
*
*   Name            vcedcmds.c - vced command structure
*                                   
*   Description     Tced command structure
*
**/

#define VCEDVARS

extern COUNT vcedmvleft();
extern COUNT vcedmvright();
extern COUNT vcedmvup();
extern COUNT vcedmvdown();
extern COUNT vcedexit();
extern COUNT vcedprtbuf();
extern COUNT vcedinstog();
extern COUNT vcedbksp();
extern COUNT vceddelete();
extern COUNT vcedtol();
extern COUNT vcedeol();
extern COUNT vcedtogstat();
extern COUNT vcedreturn();
extern COUNT vcedabort();
extern COUNT vcedhelp();
extern COUNT vcedpgup();
extern COUNT vcedpgdn();
extern COUNT vcedtab();
extern COUNT vcedbktab();
extern COUNT vcedsbuf();
extern COUNT vcedtof();
extern COUNT vcedeof();
extern COUNT vcedhome();
extern COUNT vcedend();
/* ------------------------------------------------------------------------ */

extern COUNT vceddelln();
extern COUNT vcedinsln();
extern COUNT vceddeol();
extern COUNT vcedmkpg();

/* ------------------------------------------------------------------------ */

VCEDCMD vcedcmds[] =
    {
    (TEXT *)"abort",        4000,vcedabort,     /* АР2    - Abort           */
    (TEXT *)"backspace",    4001,vcedbksp,      /* ВШ     - BackSpace       */
    (TEXT *)"backtab",      4005,vcedbktab,     /* ПС     - BackTab         */
    (TEXT *)"begin_of_line",5010,vcedtol,       /* Ф10    - Begin of line   */
    (TEXT *)"begin_of_file",4010,vcedtof,       /* ПТ     - Begin of file   */
    (TEXT *)"cursor_down",  4009,vcedmvdown,    /*        ^                 */
    (TEXT *)"cursor_left",  4006,vcedmvleft,    /*     <--|                 */
    (TEXT *)"cursor_right", 4007,vcedmvright,   /*        |-->              */
    (TEXT *)"cursor_up",    4008,vcedmvup,      /*        V                 */
    (TEXT *)"delete_char",  4015,vceddelete,    /* УТ     - Delete char     */
    (TEXT *)"delete_char",   127,vceddelete,    /*                          */
    (TEXT *)"end_of_line",  5011,vcedeol,       /* ПФ14   - End of line     */
    (TEXT *)"end_of_file",  4011,vcedeof,       /* ВНТ    - End of file     */
    (TEXT *)"exit",         5003,vcedexit,      /* PF4    - Exit            */
    (TEXT *)"menu",         5000,vcedhelp,      /* ПФ1    - Help            */
    (TEXT *)"page_down",    4013,vcedpgdn,      /* ВСС    - Page down       */
    (TEXT *)"page_up",      4012,vcedpgup,      /* ВПС    - Page up         */
    (TEXT *)"return",       4002,vcedreturn,    /* ВК     - Return          */
    (TEXT *)"save_buffer",  5002,vcedsbuf,      /* ПФ3    - Save file       */
    (TEXT *)"tab",          4004,vcedtab,       /* ГТ     - Tab             */
    (TEXT *)"toggle_insert",4014,vcedinstog,    /* РТ     - Insert/Over     */
    (TEXT *)"toggle_status",5013,vcedtogstat,   /* Ф18    - Off/On status   */
/* ------------------------------------------------------------------------ */
    (TEXT *)"delete line",    12,vceddelln,     /* УПР l  - Delete line     */
    (TEXT *)"insert line",    22,vcedinsln,     /* УПР v  - Insert line     */
    (TEXT *)"delete eol",      5,vceddeol,      /* УПР e  - Delete end line */
    (TEXT *)"mark page",       3,vcedmkpg,      /* УПР c  - Mark page       */
/* ------------------------------------------------------------------------ */
    NULLTEXT,                  0,(PFI)0,
    };

