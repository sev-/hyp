/**
*
*   Vitamin C Multi-User version 1.6
*   Copyright (c) 1985-1990
*   Creative Programming Consultants, Inc.
*   P.O. Box 112097
*   Carrollton, Texas  75006
*   (214) 416-6447
*   ALL RIGHTS RESERVED
*   Last Modification: .4 on 8/31/90 at 22:05:19
*
*   Name            getone.c  --  Get a single character
*
*   Description     This is the main input routine. This function
*                   waits until a key is pressed. While waiting
*                   the function can process a loop function.
*
*   Return Value    Returns the key pressed
*
**/

#include <vcstdio.h>
#include <sys/types.h>
#include <sys/select.h>
#include <time.h>

extern int vcindev;
extern int flag_esc_sequence;
fd_set fdset;

COUNT getone()
{
  FAST COUNT ich = 9999;
  FAST COUNT cnt = 0;
  COUNT in[20];
  COUNT out[20];
  COUNT nbr;
  COUNT keyrdy();
  COUNT xgetone();
  COUNT vcpushkey();
  static struct timeval tt;
  struct KEYTBLNODE *nptr, *intable();

#ifdef VCDEBUG
  vcdebug = (TEXT *) "getone:";
  if (vcdbfunc != NULLFUNC)
    (*vcdbfunc) (CONTINUE);
#endif

  FD_SET(vcindev, &fdset);
  tt.tv_sec = 2l;
  tt.tv_usec = 0l;

  while (ich == 9999)
  {
    cnt = 0;
    out[cnt] = 0;

    /*
     * if((in[cnt] = xgetone()) == 27) select(32, &fdset, NULL, NULL, &tt);
     * FD_SET(vcindev, &fdset);
     */
    in[cnt] = xgetone();

    nptr = vcterm.table;
    while (((nptr = intable(nptr, in[cnt], &out[cnt])) !=
	    (struct KEYTBLNODE *) 0) &&
	   (vckeyrdy ? keyrdy() : TRUE) && (cnt < 19))
    {
      cnt++;

      /*
       * select(32, &fdset, NULL, NULL, &tt); FD_SET(vcindev, &fdset);
       */ in[cnt] = xgetone();

      out[cnt] = 0;
    }
    flag_esc_sequence = 0;
    nbr = cnt;
    while (cnt > 0)
    {
      if (out[cnt])
      {
	ich = out[cnt];
	break;
      }
      cnt--;
    }
    if (cnt == 0)
    {
      if (out[cnt] == 0)
	ich = in[cnt];

      else
	ich = out[cnt];
    }
    while (cnt < nbr)
      vcpushkey(in[nbr--]);
    if (syskeytrap)
      ich = (*syskeytrap) (ich);
    if (keytrap)
      ich = (*keytrap) (ich);
  }
  return (ich);
}

COUNT vcpushkey(ans)
PFAST COUNT ans;		  /* Character to put into front of buffer        */
{
#ifdef VCDEBUG
  vcdebug = (TEXT *) "vcpushkey:";
  if (vcdbfunc != NULLFUNC)
    (*vcdbfunc) (CONTINUE);
#endif
  *kbbufhd = ans;
  if (kbbufhd == &kbbuffer[0])
    kbbufhd = kbbufed;
  else
    kbbufhd--;
  if (kbbuftl == kbbufhd)
  {
    if (kbbuftl == &kbbuffer[0])
      kbbuftl = kbbufed;
    else
      kbbuftl--;
  }
  return (ans);
}
