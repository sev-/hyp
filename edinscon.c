/*
 *  $Id: edinscon.c,v 1.2 1993/04/08 10:37:38 sev Exp $
 *
 * ---------------------------------------------------------- 
 *
 * $Log: edinscon.c,v $
 * Revision 1.2  1993/04/08 10:37:38  sev
 * edsetfta changed
 *
 * Revision 1.1  1993/04/06  14:14:07  sev
 * Initial revision
 *
 * Revision 1.2  1993/03/15  13:47:31  sev
 * Полностью заменены функции работы со справочником. Теперь он текстовый
 *
 * Revision 1.1  1993/03/12  15:44:23  sev
 * Init
 *
 *
 */

static char rcsid[]="$Id: edinscon.c,v 1.2 1993/04/08 10:37:38 sev Exp $";


#define SP_BAR "											"


#include "vced.h"

/**************************************************************************/
/*	 Функция  вставки строки ^[<ss-ss;#;#>				  */
/**************************************************************************/

COUNT ed_ins_conc(vced,line,start,end,seg,file,mode)
VCED *vced;
DBDP line;
COUNT start,end;	  /* координаты ключевого слова.	 */
TEXT *seg;
TEXT *file;
COUNT mode;
{
  VCEDLINE *cline;
  char part[500];
  char ins[100];
  char *ptr,*ch;
  char *strstr(),*strrchr();
  char *strnvh();
  COUNT i=0;
  char seg_name[NAME_LEN];
  char file_name[25];

  if(mode==0)
  {
    strcpy(seg_name,"#");
    strcpy(file_name,"#");
  }
  else
  {
    strcpy(seg_name,seg);
    strcpy(file_name,file);
  }

  if(start<10 && end>9)
    sprintf(ins,"\033<0%1d-%2d;%s;%s>",start,end,seg_name,file_name);
  if( start<10 && end<10 )
    sprintf(ins,"\033<0%1d-0%1d;%s;%s>",start,end,seg_name,file_name);

  if(start>9 && end<10)
    sprintf(ins,"\033<%2d-0%1d;%s;%s>",start,end,seg_name,file_name);
  if(start>9 && end>9)
    sprintf(ins,"\033<%2d-%2d;%s;%s>",start,end,seg_name,file_name);

  cline = vcedmline(vced->edbuffer,line);
  edsetfta(vced, line, cline,from,to,stat_);
  ptr=(char *)cline->ltext;

  if( from[0] == -1 )
  {
    if((ch=strstr(ptr,"\033)"))!=(char *)NULL)
      strins(ch,ins);
    else
    {
      if((ch=strstr(ptr,"\033("))!=(char *)NULL)
	strcat(ptr,ins);
      else
      {
	strncat(ptr,SP_BAR,80-cline->lused);
	strcat(ptr,ins);
      }
    }
    cline->lused=strlen(ptr);
    vcedrline(vced->edbuffer,cline,line);
    vced->edupval |= VCEDUPDLINE;
    return(1);
  }
  if( end < from[0])
  {
    ch=strstr(ptr,"\033<");
    strcpy(part,ch);
    sprintf(ch,"%s%s",ins,part);
    cline->lused=strlen(ptr);
    vcedrline(vced->edbuffer,cline,line);
    vced->edupval |= VCEDUPDLINE;
    return(1);
  }
  i=0;
  while( from[i] != -1 )
  {
    if(start > to[i] && ( end < from[i+1] || from[i+1]==-1) )
      break;
    i++;
  }
  if( from[i+1] == -1 )
    if((ch=strstr(ptr,"\033)"))!=(char *)NULL)
      strins(ch,ins);
    else
      strcat(ptr,ins);
  else
  {
    ch=strnvh(ptr,">\033",i+1);
    strcpy(part,ch+1);
    sprintf(ch+1,"%s%s",ins,part);
  }
  cline->lused=strlen(ptr);
  vcedrline(vced->edbuffer,cline,line);
  vced->edupval |= VCEDUPDLINE;
  return(1);
}

/**************************************************************************/
/*	 Функция  нахождения в строке N-го вхождения			  */
/**************************************************************************/

char *strnvh(where,what,n)
char *where,*what;
int n;
{
  int i;
  char *ch;
  char *tmp,*strstr();
  if(n<0)
    return(0);
  tmp=where;
  for(i=1;i<=n;i++)
  {
    ch=strstr(tmp,what);
    tmp=ch+2;
  }
  return(ch);
}
