/*
 *  $Id: edval.h,v 1.1 1993/03/19 17:48:53 sev Exp $
 *
 * ---------------------------------------------------------- 
 *
 * $Log: edval.h,v $
 * Revision 1.1  1993/03/19 17:48:53  sev
 * Initial revision
 *
 *
 */


VCEDVAL vcedval = 
    {                       /* ------------------------------------ */
    (COUNT) 256,            /* Maximum line length                  */
    (COUNT) 10,             /* Number of Pages to buffer            */
    (COUNT) 100,            /* Number of slots per page             */
    (COUNT)   0,            /* Soft Return                          */
    (COUNT)  13,            /* Hard Return                          */
    (COUNT)  14,            /* Justified Return                     */
    (COUNT)   0,            /* Use real tabs                        */
    (COUNT)   4,            /* Tab size                             */
    (COUNT)'\t',            /* Tab character                        */
    (COUNT)   0,            /* Tab space character                  */
    (COUNT)  10,            /* Number to move when shifting left    */
    (COUNT)   5,            /* Number to move when shifting up/down */
    (COUNT)   0,            /* Default Right Margin                 */
    (COUNT)   0,            /* Default Left Margin                  */
    (COUNT)   0,            /* Display control characters           */
    (COUNT)   1             /* Backup the file when saving          */
    };                      /* ------------------------------------ */

TEXT vcedfbuf[MAXLIN];      /* File buffer work area                */
TEXT vcedibuf[MAXLIN];      /* Internal buffer work area            */

#ifdef VCEDFULL

VCEDUNSTR *vcedustr = (VCEDUNSTR *)0;   /* Undo structure           */
VCEDCUTSTR *vcedscrap = (VCEDCUTSTR *)0;/* Cut buffers location     */
COUNT vcedrkeys[VCEDMAXREC];            /* Storage for keys         */
COUNT vcedcrkey = 0;                    /* Current position         */
COUNT vcedrecstat = 0;                  /* Record on or off         */

#endif
