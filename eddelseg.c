/*
 *  $Id: eddelseg.c,v 1.1 1993/04/20 16:04:12 sev Exp $
 *
 * ---------------------------------------------------------------------------
 *
 * $Log: eddelseg.c,v $
 * Revision 1.1  1993/04/20 16:04:12  sev
 * Initial revision
 *
 *
 */

eddelcon(value)
char *value;
{
  char *ch;
  int num;

  num = atoi(strrchr(value, '%')+1);
  if(!num)
  {
    ask_msg("Есть ссылки на  этот сегмент !!!",0);
    return(-1);
  }
  

}
