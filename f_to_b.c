/*
 *  $Id: f_to_b.c,v 1.4 1993/06/08 12:10:31 sev Exp $
 *
 * ---------------------------------------------------------------------------
 *
 * $Log: f_to_b.c,v $
 * Revision 1.4  1993/06/08 12:10:31  sev
 * Вот теперь этот глюк действительно убран
 *
 * Revision 1.3  1993/04/20  16:04:12  sev
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
#include <sys/types.h>
#include <sys/stat.h>


COUNT file_to_buf(edbuf)
VCEDBUF *edbuf;                     /* Edit buffer                          */
{                                   /* ------------------------------------ */
  TEXT *mptr;                       /* Temporary memory pointer             */
  COUNT len;                        /* Length of the line read              */
  COUNT retchr;                     /* Return character                     */
  struct stat info;                 /* File information structure           */
  FILE *tfptr;                      /* Temporary file pointer - for reading */

  if(stat(edbuf->bfname,&info))     /* Get permission                       */
    return(1);                      /* Error getting status                 */
  if((tfptr = vcfopen(edbuf->bfname,VCF_R)) == (FILE *)NULL)
    return(1);                      /* Error opening file                   */
  edbuf->bfperm = info.st_mode;     /* Save status of file                  */
  vcedempty(vcedfbuf,edbuf->bflinelen);/* Empty work buffer                 */
  while(fgets(vcedfbuf,edbuf->bflinelen,tfptr) != (char *)0)
  {
    trimstring1(vcedfbuf);
    len = strlen(vcedfbuf);         /* Get length of line                   */
    mptr = vcedfbuf + len - 1;      /* Point to end of line                 */
    if(*mptr == '\n')               /* If new line character                */
    {                               /*                                      */
      len--;                        /* Subtract for new line                */
      *mptr = '\0';                 /* Remove new line character            */
      retchr = vcedval.hardret;     /* Has a hard return                    */
    }                               /*                                      */
    else                            /*   or                                 */
      retchr = vcedval.softret;     /* Has a soft return                    */
    vcedexptab(edbuf,vcedfbuf,len,retchr);/* Expand & save                  */
    vcedempty(vcedfbuf,edbuf->bflinelen);/* Empty work buffer               */
  }                                 /*                                      */
  vcfclose(tfptr);                  /* Close file                           */
  return(0);                        /* Return no errors                     */
}
