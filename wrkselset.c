/*
 *
 * $Id: wrkselset.c,v 1.1 1993/09/14 16:45:50 sev Exp $
 *
 * ---------------------------------------------------------------------------
 *
 * $Log: wrkselset.c,v $
 * Revision 1.1  1993/09/14 16:45:50  sev
 * Initial revision
 *
 *
 *
*/

/*------------------------------------------------------------------*/
/*---------------------Selection Set Structures---------------------*/
/*------------------------------------------------------------------*/
/*
 *
 * struct SEL_ITEM
 *    {
 *      COUNT imagic;
 *      struct SEL_ITEM *inext;
 *      struct SEL_ITEM *iprev;
 *      TEXT *idisplay;
 *      TEXT *ivalue;
 *    };
 *
 * typedef struct SEL_ITEM SELSET;
*/

#define SEL_DEFS
#include <vcstdio.h>

/**
*
*   Name            aloselitm.c --  Allocate select item
*
*   Description     Allocates space for a select item
*                   
*   Return Value    Return NULL if error else returns pointer to
*                   allocated space for select item
*
**/

SELSET *aloselitm()
   {
   SELSET *ptr;
   TEXT *vcalloc();

   if( (ptr=(SELSET *)vcalloc(1,sizeof(SELSET),vcdebug)) == (SELSET *) 0)
      vcerror='?';
   else
      ptr->imagic=SELITEMMAGIC;
   return(ptr);
   }

/**
*
*   Name            addselitm.c -- Add or Create a Selection set
*
*   Description     Allows initialization and addition to a selection
*                   set
*                   
*   Return Value    Returns NULL if error
*
**/

SELSET *addselitm(list,display,value)
SELSET *list;       /* Pointer to selection list                    */
TEXT   *display;    /* Pointer to display string                    */
TEXT   *value;      /* Pointer to data entry string                 */
    {
    SELSET *newitem;
    SELSET *aloselitm();
    TEXT *vcalloc();

    if( (newitem = aloselitm()) == (SELSET *) 0 )
        return((SELSET *)0);
    if(display != NULLTEXT)
        {
        newitem->idisplay = vcalloc(1,strlen(display)+1,vcdebug);
        strcpy(newitem->idisplay,display);
        }
    if(value != NULLTEXT)
        {
        newitem->ivalue = vcalloc(1,strlen(value)+1,vcdebug);
        strcpy(newitem->ivalue,value);
        }
    if(list)
        {
        while(list->inext)
            list = list->inext;
        list->inext = newitem;
        newitem->iprev = list;
        }
    return(newitem);
    }

/**
*
*   Name            getselitm.c -- Get selection item
*
*   Description     Gets the pointer to an item in a selection
*                   set.
*                   
*   Return Value    Returns pointer to select item or NULL if
*                   error.
*
**/

SELSET *getselitm(list,display,value)
SELSET *list;         /* Pointer to selection list                    */
TEXT   *display;      /* Pointer to display string                    */
TEXT   *value;        /* Pointer to data entry string                 */
    {
    COUNT dtst=0,vtst=0;
    COUNT strcmp();

    if (list == (SELSET *) 0 || list->imagic != SELITEMMAGIC )
	{
	vcerror='?';
	return((SELSET *)0);
	}
    list = list->inext;
    while( list )
        {
        if((display != NULLTEXT) && (list->idisplay != NULLTEXT))
            dtst=strcmp(display,list->idisplay);
        if((value != NULLTEXT) && (list->ivalue != NULLTEXT))
            vtst=strcmp(value,list->ivalue);
        if((dtst == 0) && (vtst == 0))
            return(list);
        list=list->inext;
        }
    vcerror='?';
    return((SELSET *)0);
    }

/**
*
*   Name            delselitm.c -- Delete select item
*
*   Description     Deletes the select item from the selection
*                   set.
*                   
*   Return Value    Returns a NULL if error
*
**/

SELSET *delselitm(list,selitem)
SELSET *list;       /* Pointer to selection set                     */
SELSET *selitem;    /* Pointer to item to be removed                */
    {
    SELSET *listhead;

    if((list->imagic !=SELITEMMAGIC) ||
       (selitem->imagic != SELITEMMAGIC))
	{
	vcerror='?';
	return((SELSET *) 0);
	}
    listhead=list;
    if(selitem->iprev)
        selitem->iprev->inext = selitem->inext;
    else
        listhead=selitem->inext;
    if(selitem->inext)
        selitem->inext->iprev = selitem->iprev;
    if(selitem->idisplay != NULLTEXT)
        vcfree(selitem->idisplay,vcdebug);
    if(selitem->ivalue != NULLTEXT)
        vcfree(selitem->ivalue,vcdebug);
    vcfree(selitem,vcdebug);
    if(listhead == (SELSET *)0)
        vcerror='?';       /* not really an error.  list is now empty */
    return(listhead);
    }

/**
*
*   Name            delselset.c -- Delete selection set
*
*   Description     Deletes the selection set
*                   
*   Return Value    Returns a empy selection set
*
**/

SELSET *delselset(list)
SELSET *list;               /* Pointer to selection set to delete   */
    {
    SELSET *head;
    SELSET *sel;

    head = list->inext;
    while (head)
        {
	sel  = getselitm(list,head->idisplay,head->ivalue);
	list = delselitm(list,sel);
	head = head->inext;
        }
    return(list);
    }

/**
*
*   Name            lstsel -- List selection set
*
*   Description     List of the selection set
*                   
*   Return Value    VOID
*
**/

VOID lstsel(list)
SELSET *list;
{
  SELSET  *head;
  WPTR wwd;
  WPTR wwv;
  WPTR wopen();

  wwd = wopen(2,2,5,50," display ");
  wwv = wopen(6,2,9,50," value ");

  head = list->inext;
  while (head)
	{
	watsay(wwd,0,0,head->idisplay);
	watsay(wwv,0,0,head->ivalue);
	getone();
	werase(wwd);
	werase(wwv);
	head = head->inext;
	}
  wclose(wwd);
  wclose(wwv);
}

#define MAXLINE 1024

/* ------------------------------------------------------------------------ */
/*                                                                          */
/* Вывод списковой структуры SELSET в текстовый файл, с именем name_file.   */
/* Режим файла указывается в переменной mode_file, и может быть "w" или "a".*/
/* Список подлежащий выводу указывается в переменной list.                  */
/* Разделителем между переменными display и value, входящие в элемент спис- */
/* ка, является вертикальная черта (|). Конец строки - символ решетки (#).  */
/*                                                                          */
/* ------------------------------------------------------------------------ */

int putselset(name_file,mode_file,list)
char  *name_file;
char  *mode_file;
SELSET *list;
{
   SELSET *head;
   FILE *fp, *fopen();

   if ((fp = fopen(name_file,mode_file)) == (FILE *)NULL)
		{
		 return(1);
		}
   head = list->inext;
   while (head)
      {
	fprintf(fp,"%s|",head->idisplay);
	fprintf(fp,"%s#\n",head->ivalue);
	head = head->inext;
      }
   fclose(fp);
   return(0);
}

/* ------------------------------------------------------------------------ */
/*                                                                          */
/* Ввод списковой структуры SELSET из текстового файла, с именем name_file. */
/* Режим файла указывается в переменной mode_file, и должен быть "r".       */
/* Результат - указатель на список в переменной list.                       */
/* Разделителем между переменными display и value, входящие в элемент спис- */
/* ка, является вертикальная черта (|). Конец строки - символ решетки (#).  */
/*                                                                          */
/* ------------------------------------------------------------------------ */

int getselset(name_file,mode_file,list)
char  *name_file;
char  *mode_file;
SELSET *list;
{

    FILE  *fp,*fopen();
    int   in;
    char  bufin[MAXLINE];
    char  bufd[MAXLINE];
    char  bufv[MAXLINE];
    char  cin;

    if ((fp = fopen(name_file,mode_file)) == (FILE *)NULL)
      {
       return(1);
      }

    in = -1;
    while ((cin=getc(fp)) != EOF)
      {
	if (cin == '\n') goto cont;
	if (cin == '\r') goto cont;
	in++;
	bufin[in] = cin;
	if (cin == '|')
	   {
	    bufin[in] = '\0';
	    strcpy(bufd,bufin);
	    in = -1;
	   }
	if (cin == '#')
	   {
	    bufin[in] = '\0';
	    strcpy(bufv,bufin);
	    in = -1;
	    addselitm(list,bufd,bufv);
	   }
cont:
	continue;
      }            /* end of while */

      fclose(fp);
      return(0);
}

/* ------------------------------------------------------------------------ */
/**
*
*   Name            initselset.c -- Create a Selection set
*
*   Description     Allows initialization a selection set
*                   
*   Return Value    Returns NULL if error
*
**/

SELSET *initselset()
	{
		SELSET *list;       /* Pointer to selection list            */

		list = aloselitm();
		return(list);
	}

/* ------------------------------------------------------------------------ */
/**
*
*   Name            repselitm.c -- Replace a selection item
*
*   Description     Replacing selitm from selection set
*                   
*   Return Value    Returns NULL if error
*
**/

SELSET *repselset(list, old, display, value)
SELSET *list;
SELSET *old;
TEXT *display;
TEXT *value;
{
  SELSET *prev, *next, *new;

  prev = old->iprev;
  next = old->inext;

  if(delselitm(list, old) == (SELSET *)NULL)
    return (SELSET *)NULL;

  new = addselitm(list, display, value);
  new->iprev->inext = (SELSET *)NULL;	/* А мы не добавляли */
  new->iprev = prev;
  new->inext = next;
  if(new->iprev)
    new->iprev->inext = new;
  if(new->inext)
    new->inext->iprev = new;

  return new;
}
