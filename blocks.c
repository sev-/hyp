/*
 *  $Id: blocks.c,v 1.3 1993/03/29 12:06:38 sev Exp $
 *
 * ---------------------------------------------------------- 
 *
 * $Log: blocks.c,v $
 * Revision 1.3  1993/03/29 12:06:38  sev
 * Эта глупая штука не хочет работать из-за двух закоментаренных
 * строк ф функции инициализации
 *
 * Revision 1.2  1993/03/28  17:56:17  sev
 * *** empty log message ***
 *
 * Revision 1.1  1993/03/20  14:27:27  sev
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
  static TEXT cutname[] = "cutbuf";
  
  cutbuf = (VCEDBUF *)vcalloc(1, sizeof(VCEDBUF *), vcdebug);

  if((cutbuf->bfdnum = dbmsxopn(-1,sizeof(VCEDLINE) + cutbuf->bflinelen,
	0,1,0,vcedval.pages,vcedval.slots)) == (DBP)0)
  {
     askmsg("Editor Error","Can't alloc cut buffer",0);
     return 0;
  }
  cutbuf->bfname = cutname;
/*  cutbuf->bfbuffer = NULLTEXT;
  cutbuf->bfchange = 0;
*/  cutbuf->bfperm = 0;
  cutbuf->bflinelen = vcedval.linlen;
  cutbuf->bffline = (DBDP)0;
  cutbuf->bfbline = (DBDP)0;
  cutbuf->bfnlines = 0l;
  cutbuf->bfctrl = VCEDFFIL;
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
  DBDP i, lptr;
  VCEDLINE *line;

/*  for(i = vced->edaline; i <= vced->edcline; i++)
  {
    line = vcedgline(vced->edbuffer, vced->edaline);
    vcedaline(cutbuf, line->ltext, line->lused);
*/
  vcedaline(cutbuf, "string", 7);

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
  VCEDLINE *line;
  DBDP prev, next;
  int i;

/*
  line = vcedgline(vced->edbuffer, vced->edcline);
  prev = line->lprev;
  next = line->lnext;
*/
  line = vcedgline(cutbuf, 1);

  for(i = 0; i < cutbuf->bfnlines; i++, prev ++, next ++)
  {
    vcedaline(vced->edbuffer, line->ltext, line->lused);
    line = vcednline(cutbuf, line);
  }    
  vced->edupval = VCEDUPDALL;
}
