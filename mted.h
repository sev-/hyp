/*
 *  $Id: mted.h,v 1.1 1993/03/17 13:01:34 sev Exp $
 *
 * ---------------------------------------------------------------------------
 *
 * $Log: mted.h,v $
 * Revision 1.1  1993/03/17 13:01:34  sev
 * Initial revision
 *
 * Revision 1.2  1993/03/15  15:16:32  kas
 * mted 2
 * menu
 *
 * Revision 1.1  1993/03/15  06:57:59  kas
 * Initial revision
 *
 *
*/

#define ATR_NO   vc.black+vc.bg*vc.white
#define ATR_YES  vc.blue+vc.bg*vc.black
#define ATR_B    vc.black+vc.bg*vc.white
#define ATR_F    vc.white+vc.bg*vc.black
#define ATR_FON  vc.black+vc.bg*vc.white

#define SEL_DEFS
#define VCGET_DEFS
#define VCM_DEFS
#define VCERROR_DEFS
/*
#define VCEDVARS
*/
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <vcstdio.h>
#include <vcdbms.h>
#include <vced.h>

#include "edkey.h"
