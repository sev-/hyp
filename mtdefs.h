/*
 *  $Id: mtdefs.h,v 1.3 1993/03/19 17:48:53 sev Exp $
 *
 * ---------------------------------------------------------- 
 *
 * $Log: mtdefs.h,v $
 * Revision 1.3  1993/03/19 17:48:53  sev
 * Добавлена функция начала отметки блока. Отлично работает
 *
 * Revision 1.2  1993/03/19  16:18:38  sev
 * Delete some bugs
 *
 *
 */

#define VCEDVARS

COUNT vcedmvleft();
COUNT vcedmvright();
COUNT vcedmvup();
COUNT vcedmvdown();
COUNT vcedexit();
COUNT vcedprtbuf();
COUNT vcedinstog();
COUNT vcedbksp();
COUNT vceddelete();
COUNT vcedtol();
COUNT vcedeol();
COUNT vcedtogstat();
COUNT vcedreturn();
COUNT vcedabort();
COUNT vcedhelp();
COUNT vcedpgup();
COUNT vcedpgdn();
COUNT vcedtab();
COUNT vcedbktab();
COUNT vcedsbuf();
COUNT vcedtof();
COUNT vcedeof();
COUNT vcedhome();
COUNT vcedend();
COUNT edmenu();

/* ------------------------------------------------------------------------ */

COUNT begin_mark();

/* ------------------------------------------------------------------------ */

COUNT vceddelln();
COUNT vcedinsln();
COUNT vceddeol();
COUNT vcedmkpg();

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
    (TEXT *)"help",         5000,vcedhelp,      /* ПФ1    - Help            */
    (TEXT *)"page_down",    4013,vcedpgdn,      /* ВСС    - Page down       */
    (TEXT *)"page_up",      4012,vcedpgup,      /* ВПС    - Page up         */
    (TEXT *)"return",       4002,vcedreturn,    /* ВК     - Return          */
    (TEXT *)"save_buffer",  5002,vcedsbuf,      /* ПФ3    - Save file       */
    (TEXT *)"tab",          4004,vcedtab,       /* ГТ     - Tab             */
    (TEXT *)"toggle_insert",4014,vcedinstog,    /* РТ     - Insert/Over     */
    (TEXT *)"toggle_status",5013,vcedtogstat,   /* Ф18    - Off/On status   */
    (TEXT *)"menu",         5001,edmenu,        /* ПФ1    - Menu            */
/* ------------------------------------------------------------------------ */
    (TEXT *)"delete line",    12,vceddelln,     /* УПР l  - Delete line     */
    (TEXT *)"insert line",    22,vcedinsln,     /* УПР v  - Insert line     */
    (TEXT *)"delete eol",      5,vceddeol,      /* УПР e  - Delete end line */
    (TEXT *)"mark page",       3,vcedmkpg,      /* УПР c  - Mark page       */
/* ------------------------------------------------------------------------ */
    (TEXT *)"begin mark",   5004,begin_mark,	/* F6     - Begin mark reg  */ 
/* ------------------------------------------------------------------------ */
    NULLTEXT,                  0,(PFI)0,
    };

