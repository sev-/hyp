/*
 *  $Id: edstruct.h,v 1.3 1993/04/20 16:04:12 sev Exp $
 *
 * ---------------------------------------------------------------------------
 *
 * $Log: edstruct.h,v $
 * Revision 1.3  1993/04/20 16:04:12  sev
 * a
 *
 * Revision 1.3  1993/04/15  15:05:51  kas
 * *** empty log message ***
 *
 * Revision 1.3  1993/04/15  15:05:51  kas
 * *** empty log message ***
 *
 * Revision 1.2  1993/04/13  09:21:13  kas
 * *** empty log message ***
 *
 * Revision 1.2  1993/04/13  09:21:13  kas
 * *** empty log message ***
 *
 * Revision 1.1  1993/04/12  15:13:06  kas
 * Initial revision
 *
 *
 */

typedef struct
    {                       /* ------------------------------------ */
    COUNT linlen;           /* Default line length                  */
    COUNT pages;            /* Number of pages to keep in memory    */
    COUNT slots;            /* Number of slots per page             */
    COUNT softret;          /* Soft return character                */
    COUNT hardret;          /* Hard return character                */
    COUNT justret;          /* Justified return (future use)        */
    COUNT realtabs;         /* Use real tabs                        */
    COUNT tabsize;          /* Tab size                             */
    COUNT tabchr;           /* Tab character                        */
    COUNT tabspace;         /* Tab space character                  */
    COUNT shiftleft;        /* Shift left and right value           */
    COUNT shiftup;          /* Shift up and down value              */
    COUNT dftrmar;          /* Default right margin                 */
    COUNT dftlmar;          /* Default left margin                  */
    COUNT dispctrl;         /* Display control characters           */
    COUNT bckupfil;         /* Make file backup                     */
    } VCEDVAL;              /* ------------------------------------ */

typedef struct
    {                       /* ------------------------------------ */
    DBDP lnext;             /* Next line                            */
    DBDP lprev;             /* Previous line                        */
    COUNT lstatus;          /* Line status byte                     */
    COUNT lused;            /* Number of characters used            */
    COUNT lsize;            /* Size of allocated line               */
    TEXT ltext[1];          /* Actual line of information           */
    } VCEDLINE;             /* ------------------------------------ */

typedef struct 
    {                       /* ------------------------------------ */
    TEXT *bfname;           /* File name for buffer                 */
    TEXT *bfbuffer;         /* Write back to buffer                 */
    COUNT bflinelen;        /* Line length of this file             */
    DBP bfdnum;             /* Database Number for buffer           */
    DBDP bffline;           /* First line of file                   */
    DBDP bfbline;           /* Bottom line of file                  */
    LONG bfnlines;          /* Number of lines                      */
    COUNT bfctrl;           /* Current control values               */
    COUNT bfchange;         /* Has buffer been changed              */
    COUNT bfperm;           /* File permission when opened          */
    } VCEDBUF;              /* ------------------------------------ */

typedef struct
    {                       /* ------------------------------------ */
    WPTR edwptr;            /* Window pointer if active             */
    WPTR edswptr;           /* Pointer to status window (if used)   */
    COUNT edkey[3];         /* Last three keys pressed              */
    COUNT edmode;           /* Editor mode                          */
    COUNT edmail;           /* Slot for inter program communication */
    COUNT edsline;          /* Status line clear flag               */
    VCEDBUF *edbuffer;      /* Pointer to buffer for file           */
    COUNT edrmar;           /* Right margin                         */
    COUNT edlmar;           /* Left margin                          */
    DBDP edtline;           /* Top line of buffer in window         */
    DBDP edcline;           /* Current line                         */
    COUNT edcchar;          /* Current character                    */
    COUNT edoffset;         /* Offset of file in window             */
    LONG edtrow;            /* Row in file of top line              */
    LONG edcrow;            /* Row location of current line         */
    DBDP edaline;           /* Anchor line                          */
    DBDP edarow;            /* Anchor row                           */
    COUNT edachar;          /* Anchor character                     */
    COUNT edrval;           /* Repeat value                         */
    COUNT edupval;          /* Update value for screen              */
    COUNT edurow;           /* Upper row for window                 */
    COUNT educol;           /* Upper column for window              */
    COUNT edlrow;           /* Lower row for window                 */
    COUNT edlcol;           /* Lower column for window              */
    COUNT edctrl;           /* Control value for window             */
    TEXT *edtitle;          /* Title for window (if needed)         */
    } VCED;                 /* ------------------------------------ */


typedef struct 
    {                       /* ------------------------------------ */
    TEXT *cmdname;          /* Textual name for command             */
    COUNT cmdkey;           /* Key pressed that executes command    */
    PFI cmdloc;             /* Function to execute                  */
    } VCEDCMD;              /* ------------------------------------ */


struct {
	    char *line[24];
	    int width;
	} mem_text;

struct  {
	      char file_par[16];
	      DBDP line;
	      COUNT cchar;
	      char mesg[MES_LEN];
	      char name_file[18];
	      char name_seg[NAME_LEN];
	}privyz;

SELSET *dirr;                               /* Справочник сегментов     */
SELSET *listwdo;                            /* Список файлов редактора  */

struct VCEDWDO
{                                           /* ------------------------ */
	struct VCEDWDO *vcedwnext;          /* Pointer to next file     */
	VCED *vcedwvced;                    /* Pointer to structure     */
	COUNT vcedwstat;                    /* Status of window         */
};                                          /* ------------------------ */

struct VCEDFILE
{                                           /* ------------------------ */
	struct VCEDFILE *vcedfnext;         /* Pointer to next file     */
	VCEDBUF *vcedfbuf;                  /* Pointer to structure     */
	TEXT *vcedfname;                    /* Name of file             */
	COUNT vcedfstat;                    /* Status of buffer         */
	LONG vcedftop;                      /* Last top of screen       */
	LONG vcedfcur;                      /* Last current line        */
	LONG vcedftlin;                     /* Last top of screen       */
	LONG vcedfclin;                     /* Last current line        */
	LONG vcedfarow;                     /* Anchor row               */
	DBDP vcedfalin;                     /* Anchor line              */
	COUNT vcedfchr;                     /* Current character        */
	COUNT vcedfachr;                    /* Anchor character         */
	COUNT vcedfoff;                     /* File offset              */
	COUNT vcedcolseg;                   /* Number segments          */
	TEXT   mesg[80];
};

char dir_file[80];     /* имя файла, содержащего справочник сегментов   */
