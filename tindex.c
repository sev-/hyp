/*
 *
 * $Id: tindex.c,v 1.1 1993/03/10 08:05:42 kas Exp $
 *
 * ---------------------------------------------------------------------------
 *
 * $Log: tindex.c,v $
 * Revision 1.1  1993/03/10 08:05:42  kas
 * Initial revision
 *
 *
*/

static char rcsid[]="$Id: tindex.c,v 1.1 1993/03/10 08:05:42 kas Exp $";

tindex(s,t)          /* return index of t in s, -1 if none                  */

	char s[];
	char t[];
{
	int i, j, k;

	for (i=0; s[i] != '\0'; i++)
		{
		for (j=i,k=0; t[k] != '\0' && s[j] == t[k]; j++,k++);
		if ( t[k] == '\0') return (i);
		}
	return(-1);
}
