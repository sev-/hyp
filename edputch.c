/*
 *  $Id: edputch.c,v 1.2 1993/04/20 16:04:12 sev Exp $
 *
 * ---------------------------------------------------------------------------
 *
 * $Log: edputch.c,v $
 * Revision 1.2  1993/04/20 16:04:12  sev
 * a
 *
 * Revision 1.3  1993/04/15  15:05:51  kas
 * *** empty log message ***
 *
 * Revision 1.3  1993/04/15  15:05:51  kas
 * *** empty log message ***
 *
 * Revision 1.2  1993/04/13  13:50:41  kas
 * *** empty log message ***
 *
 * Revision 1.2  1993/04/13  13:50:41  kas
 * *** empty log message ***
 *
 * Revision 1.1  1993/04/12  15:13:06  kas
 * Initial revision
 *
 * Revision 1.1  1993/04/12  15:13:06  kas
 * Initial revision
 *
 *
 */

#include "vced.h"

COUNT edputch(vced,key)
VCED *vced;		    /* Pointer to edit structure	    */
COUNT key;		    /* Key to insert			    */
    {			    /* ------------------------------------ */
    VCEDLINE *cline;	    /* Pointer to temporary line	    */
    TEXT *ptr1,*ptr2,*ptr3; /* Temporary pointers		    */

    if(vced->edcline == (DBDP)0)	/* If not on real line	    */
	vcedmkreal(vced);		/* Make the current loc real*/
    cline = vcedmline(vced->edbuffer,vced->edcline);/* Get cur. line*/
    ptr1 = ptr2 = &cline->ltext[0];	/* Set to beginning of line */
    ptr1 += vced->edcchar;		/* Increment to current char*/
    *ptr1 = key;			/* Insert new character     */
    if(vced->edcchar >= cline->lused)	/* If passed end of line    */
	{				/*			    */
	ptr2 += cline->lused;		/* Point to end of line     */
	while(ptr2 != ptr1)		/* While more spaces to ins */
	    {				/*			    */
	    *(ptr2++) = ' ';		/* Insert spaces	    */
	    cline->lused++;		/* Increment chars used     */
	    }				/*			    */
	cline->lused++; 		/* Increment for new char.  */
	}				/*			    */
    vcedrline(vced->edbuffer,cline,vced->edcline);/* Update line    */
    vcedprtchar(vced,vced->edcline,max(0,vced->edcchar));
    }					/* ------------------------ */
