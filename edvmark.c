/*
 *  $Id: edvmark.c,v 1.2 1993/04/08 10:37:38 sev Exp $
 *
 * ---------------------------------------------------------- 
 *
 * $Log: edvmark.c,v $
 * Revision 1.2  1993/04/08 10:37:38  sev
 * edsetfta changed
 *
 * Revision 1.1  1993/04/06  14:14:07  sev
 * Initial revision
 *
 * Revision 1.4  1993/04/06  13:40:54  kas
 * *** empty log message ***
 *
 * Revision 1.3  1993/04/05  16:01:33  kas
 * *** empty log message ***
 *
 * Revision 1.2  1993/03/27  12:08:16  kas
 * *** empty log message ***
 *
 * Revision 1.1  1993/03/12  15:44:23  sev
 * Init
 *
 *
 */

static char rcsid[]="$Id: edvmark.c,v 1.2 1993/04/08 10:37:38 sev Exp $";


#include <string.h>
#include <memory.h>
#include "vced.h"

int flagsegm = 0;

#define UP_TO_DN 1
#define DN_TO_UP 0

/****************************************************************************/
/*                      Функция  отметки  сегмента                          */
/****************************************************************************/

COUNT edvmark(vced)
VCED *vced;
{
  vced->edkey[0]=SEGM;
  return(0);
}

/* ------------------------------------------------------------------------ */

COUNT ed_vert_mark(vced)
VCED *vced;
{
  WPTR wdo1,w;
  TEXT buf[132];
  int len=1,key;
  int flag_seg=-1;	    /* флаг отметки сегмента	*/
  int row=0,col=0;	    /* текущие стр. и позиция	*/
  int col_mark=0;	    /* ширина отмеченного блока */
  int up_crow;		    /* начало отмеченного блока */
  int dn_crow;		    /* конец отмеченного блока */
  int prom_crow;
  int prom_trow;
  int prom;
  int i=0,more=1;
  int start,end;	    /* начало и конец сегмента		       */
  int retval=1; 	    /* ret 1 если отмечена ,0 если нет и -1 ESC*/
  VCEDLINE *cline,*prev,*next;
  DBDP up_line,dn_line,mem_edtline;
  DBDP mem_line;
  LONG mem_edtrow;
  int len_seg;
  int up_dn;
  int block=0;
  char *strchr();
  int ret;
  char *ch,name_seg[NAME_LEN];

  wdo1=vced->edwptr;
  w=vced->edswptr;
  more=1;

flagsegm = 1;

vced->edaline = vced->edcline;
vced->edachar = vced->edcchar;
vced->edarow = vced->edcrow;

  ungetone(SEGM);
  while(more)
  {
    if(vced->edupval & VCEDUPDALL )
    {
      edprtbuf(vced);

      if( key ==CUR_UP && col_mark < 1 && end<vced->edlrow-vced->edurow-2)
	end++;
      if( key==CUR_DOWN && col_mark > 1 && start>0 )
	start--;
      ed_vert_atr(wdo1,start,end,ATR_B);
    }
    vcedstatus(vced);
    vcedaddcur(vced);
    vced->edupval =0;
    key=getone();
vced->edupval |=VCEDUPDALL;
    switch(key)
    {
    case CUR_DOWN:
      cline=vcedgline(vced->edbuffer,vced->edcline);
      if((col_mark==1 && up_dn==DN_TO_UP) || (cline->lnext == (DBDP )0) )
	    {
	       bell();
	       break;
	    }
      vcedmvdown(vced);
      row=vced->edcrow-vced->edtrow;
      if(col_mark>=1)
      {
	end=row;
/*	selbar(wdo1,row,0,ATR_B,wcols(wdo1));
*/	col_mark++;
      }
      else
      {
	start=row;
/*	selbar(wdo1,row-1,0,ATR_F,wcols(wdo1));
*/	col_mark++;
      }
      break;

    case CUR_UP:
      cline=vcedgline(vced->edbuffer,vced->edcline);
      if((col_mark==1 && up_dn==UP_TO_DN) || (cline->lprev == (DBDP )0) )
	    {
		bell();
		break;
	    }
      vcedmvup(vced);
      row=vced->edcrow-vced->edtrow;
      if(col_mark<=1)
      {
	start=row;
/*	selbar(wdo1,row,0,ATR_B,wcols(wdo1));
*/	col_mark--;
      }
      else
      {
	end=row;
/*	selbar(wdo1,row+1,0,ATR_F,wcols(wdo1));
*/	col_mark--;
      }
      break;

    case PGDN:
      if(up_dn==DN_TO_UP)
      {
	if(col_mark==1)
	       {
		   bell();
		   break;
	       }
	if(vced->edcrow+wrows(wdo1) >= up_crow)
	{
	  vced->edtline=mem_edtline;
	  vced->edcline=up_line;
	  vced->edtrow=mem_edtrow;
	  vced->edcrow=up_crow;
	  end=up_crow-vced->edtrow;
	}
	else
	{
	  vcedpgdn(vced);
	  if(up_crow-vced->edtrow < wrows(wdo1) && up_crow-vced->edtrow > 0)
	    end=up_crow-vced->edtrow;
	  else
	    end=wrows(wdo1)-1;
	}
	start=vced->edcrow-vced->edtrow;
	col_mark=vced->edcrow-up_crow+1;
	vced->edupval |=VCEDUPDALL;
	break;
      }
      vcedpgdn(vced);
      start=0;
      end=vced->edcrow-vced->edtrow;
      col_mark=vced->edcrow-up_crow+1;
      vced->edupval |=VCEDUPDALL;
      break;

    case PGUP:
      if(up_dn==UP_TO_DN)
      {
	if(col_mark==1)
	   {
	       bell();
	       break;
	   }
	if(vced->edcrow-wrows(wdo1) <= up_crow)
	{
	  vced->edtline=mem_edtline;
	  vced->edcline=up_line;
	  vced->edtrow=mem_edtrow;
	  vced->edcrow=up_crow;
	  start=up_crow-vced->edtrow;
	}
	else
	{
	  vcedpgup(vced);
	  if(up_crow-vced->edtrow < wrows(wdo1) && up_crow-vced->edtrow > 0)
	    start=up_crow-vced->edtrow;
	  else
	    start=0;
	}
	end=vced->edcrow-vced->edtrow;
	col_mark=vced->edcrow-up_crow+1;
	vced->edupval |=VCEDUPDALL;
	break;
      }
      vcedpgup(vced);

      if(up_crow-vced->edtrow <= wrows(wdo1))	 
	    end=up_crow-vced->edtrow;
      else
	    end=wrows(wdo1)-1;
      start=vced->edcrow-vced->edtrow;
      col_mark=vced->edcrow-up_crow+1;
      vced->edupval |=VCEDUPDALL;
      break;

    case HOME_KEY:

      if(up_dn==UP_TO_DN)
      {
	if(col_mark==1)
	   {
	      bell();
	      break;
	   }
	vced->edtline=mem_edtline;
	vced->edcline=up_line;
	vced->edtrow=mem_edtrow;
	vced->edcrow=up_crow;
      }
      else
	vcedtof(vced);
      row=vced->edcrow-vced->edtrow;
      if(up_crow <= wrows(wdo1))
      {
	start=row;
	end=up_crow-vced->edtrow;
      }
      else
      {
	start=row;
	end=wrows(wdo1)-1;
      }
      col_mark=vced->edcrow-up_crow+1;
      vced->edupval |=VCEDUPDALL;
      break;

    case END_KEY:

      if(up_dn==DN_TO_UP)
      {
	if(col_mark==1)
	     {
		bell();
		break;
	     }
	vced->edtline=mem_edtline;
	vced->edcline=up_line;
	vced->edtrow=mem_edtrow;
	vced->edcrow=up_crow;
      }
      else
	vcedeof(vced);
      row=vced->edcrow-vced->edtrow;
      if(up_crow >= vced->edcrow-wrows(wdo1))
      {
	end=row;
	start=up_crow-vced->edtrow;
      }
      else
      {
	start=0;
	end=row;
      }
      col_mark=vced->edcrow-up_crow+1;
      vced->edupval |=VCEDUPDALL;
      break;

    case SEGM:

      flag_seg=(flag_seg==0 || flag_seg==-1)?1:0;
      if(flag_seg)
      {
	if(vced->edcline == (DBDP)0)	    /* If not on real line	*/
	    vcedmkreal(vced);		     /* Make the current loc real*/
	cline=vcedgline(vced->edbuffer,vced->edcline);
	prev=vcedgline(vced->edbuffer,cline->lprev);
	next=vcedgline(vced->edbuffer,cline->lnext);
	if(strchr(prev->ltext,'\014')!=NULL || vced->edcrow==0 )
	  up_dn=UP_TO_DN;
	else
	  if(strchr(next->ltext,'\014')!=NULL || strchr(prev->ltext,'\014')!=NULL)
	    up_dn=DN_TO_UP;
	  else
	  {
	    ask_msg("Начало отметки сегмента - до или после строки ----------... ",0);
	    vced->edupval = 0;
	    more=0;
	    break;
	  }
	row=vced->edcrow-vced->edtrow;
/*	selbar(wdo1,row,0,ATR_B,wcols(wdo1));
*/	start=end=row;
	up_crow=vced->edcrow;
	up_line=vced->edcline;
	mem_edtrow=vced->edtrow;
	mem_edtline=vced->edtline;
	watsay(w,0,3,"Oтметка сегмента		  ");
	col_mark=1;
	break;
      }
      if(vced->edcline == (DBDP)0)	  /* If not on real line      */
	 vcedmkreal(vced);		  /* Make the current loc real*/
      cline=vcedgline(vced->edbuffer,vced->edcline);
      prev=vcedgline(vced->edbuffer,cline->lprev);
      next=vcedgline(vced->edbuffer,cline->lnext);
      if(up_dn==UP_TO_DN && strchr(next->ltext,'\014')==NULL && start!=end)
      {
	ask_msg("Конец отметки сегмента - до строки ----------... ",0);
	flag_seg=1;
	break;
      }
      if(up_dn==DN_TO_UP && strchr(prev->ltext,'\014')==NULL && start!=end && vced->edcrow!=0)
      {
	ask_msg("Конец отметки сегмента - после строки ----------... ",0);
	flag_seg=1;
	break;
      }
      watsay(w,0,3,"Сегмент отмечен	       ");
      unblkkey(END_KEY);
      unblkkey(CUR_DOWN);
      unblkkey(PGDN);
      unblkkey(HOME_KEY);
      unblkkey(CUR_UP);
      unblkkey(PGUP);
      dn_crow=vced->edcrow;
      dn_line=vced->edcline;
      if(up_crow > dn_crow)
      {
	mem_line=up_line;
	up_line=dn_line;
	dn_line=mem_line;

	prom=up_crow;
	up_crow=dn_crow;
	dn_crow=prom;
      }
      len_seg=dn_crow-up_crow;
      edprtbuf(vced);
/*      i=0;
      while(cfile->drawseg.segs[i]!=-1)
      {
	   i++;
	   if(i==SEGOFLINE)
	break;
      }

       if(i==SEGOFLINE)
       {
	ask_msg("Сегмент не отмечен.Количество сегментов больше допустимого.",0);
	ungetone(ESC);
	break;
       }
       cfile->drawseg.segs[i]=1;
       cfile->drawseg.beg_seg[i]=up_line;
       cfile->drawseg.crow_beg[i]=up_crow;
*/       vcedempty(name_seg,NAME_LEN);
       strcpy(name_seg,vced->edbuffer->bfname);
       if((ch=strchr(name_seg,'.'))!=NULL)
	   *ch='\0';
       sprintf(buf,"%s%d",name_seg,col_seg_of_file+1);
      if(ask_dir(vced,len_seg)==0)
      {
	ungetone(ESC);
	break;
      }
      inc_seg_str(vced,up_line,dn_line,len_seg,buf);
      vced->edupval = VCEDUPDALL;
      vced->edsline = 1;
      more=0;
      break;
    case ESC:
      unblkkey(END_KEY);
      unblkkey(CUR_DOWN);
      unblkkey(PGDN);
      unblkkey(HOME_KEY);
      unblkkey(CUR_UP);
      unblkkey(PGUP);
      vced->edupval |= VCEDUPDALL;
      vced->edsline = 1;
      more=0;
      break;
    }
  }
vced->edaline = (DBDP)0;
vced->edachar = -1;
vced->edarow = -1;
vced->edupval |= VCEDUPDALL;
flagsegm = 0;
}

/****************************************************************************/
/*                         Функция маркирует область окна                   */
/****************************************************************************/

ed_vert_atr(w,start,end,atr)
WPTR w;
int start,end,atr;
{
  woff();
  while(start <= end)
  {
/*    selbar(w,start,0,atr,wcols(w));
*/    start++;
  }
  won();
}

/* ------------------------------------------------------------------------ */
