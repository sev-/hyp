/*
 *  $Id: blocks.c,v 1.1 1993/03/20 14:27:27 sev Exp $
 *
 * ---------------------------------------------------------- 
 *
 * $Log: blocks.c,v $
 * Revision 1.1  1993/03/20 14:27:27  sev
 * Initial revision
 *
 * Revision 1.1  1993/03/19  17:48:53  sev
 * Initial revision
 *
 *
 */

#include "mted.h"

COUNT edstartcut()
{
  cutbuf = (VCEDBUF *)vcalloc(1, sizeof(VCEDBUF *), vcdebug);
  cutbuf->bflinelen = vcedval.linlen;
  cutbuf->bffline = (DBDP)0;
  cutbuf->bfbline = (DBDP)0;
  cutbuf->bfnlines = 0l;
  cutbuf->bfctrl = VCEDFFIL;

  if((cutbuf->bfdnum = dbmsxopn(-1,sizeof(VCEDLINE) + cutbuf->bflinelen,
	0,1,0,vcedval.pages,vcedval.slots)) == (DBP)0)
  {
     askmsg("Editor Error","Can't alloc cut buffer",0);
     return 0;
  }
}

COUNT begin_mark(vced)
VCED *vced;
{
  vced->edaline = vced->edcline;
  vced->edachar = vced->edcchar;
  vced->edarow = vced->edcrow;
}

COUNT copy_block(vced)
VCED *vced;
{
  vced->edaline = (DBDP)0;
  vced->edupval = VCEDUPDALL;
}

COUNT cut_block(vced)
VCED *vced;
{
}

COUNT paste_block(vced)
VCED *vced;
{
}

