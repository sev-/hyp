/*
 *  $Id: edkey.h,v 1.1 1993/04/06 14:14:07 sev Exp $
 *
 * ---------------------------------------------------------------------------
 *
 * $Log: edkey.h,v $
 * Revision 1.1  1993/04/06 14:14:07  sev
 * Initial revision
 *
 * Revision 3.2  1993/03/27  12:08:16  kas
 * *** empty log message ***
 *
 * Revision 3.1  1993/03/24  16:39:49  kas
 * ..
 *
 * Revision 1.2  1993/03/13  12:07:21  sev
 * Исправлены ошибки в delsels, убран ПФ4 и ПФ3
 *
 * Revision 1.1  1993/03/12  15:44:23  sev
 * Init
 *
 */

/* -------------------------- СМ7238 -------------------------------------- */

#define TAB          4004
#define BTAB         4005
#define ESC          4000
#define SPASE          32
#define RET          4002
#define BACKSP       4001
#define CUR_LEFT     4006
#define CUR_RIGHT    4007
#define CUR_UP       4008
#define CUR_DOWN     4009
#define HOME_KEY     4010
#define END_KEY      4011
#define PGUP         4012
#define PGDN         4013
#define INS_KEY      4014
#define DEL_KEY      4015
#define PF1          5000
#define PF2          5001
#define PF3          5002
#define PF4          5003

#define F6           5005
#define F7           5006
#define F8           5007
#define F9           5008
#define F10          5010
#define F11          5004
#define F12          5009
#define F13          5018
#define F14          5011
#define F15          5016
#define F16          5017
#define F17          5012
#define F18          5013
#define F19          5014
#define F20          5015

/* for hypertext */
#define HELP          PF1
#define POLE           F6
#define DEL_POLE       F7
#define PASTE          F8

#define DIR_SEG       F10
#define SEGM           F9

#define SYS_COM        11
#define INS_LINE       22

#define LISTWDO       F11
#define NEW_FILE      F12
#define FILES         F13
#define NEXT_FILE     F14
#define DEL_FILE      F15
#define SAVE_BUF      F16

#define CONF          F17
#define COMP          F18
#define VIEW          F19
#define SHELL         F20

#define EXIT          ESC

#define INF_CONC     4020
#define SCRN_SEG       23

#define DEL_LINE       12
#define CLEAR_ENDOFL    5
#define PG_MARK         3

#define REF             1

/* ------------------------------------------------------------------------ */
