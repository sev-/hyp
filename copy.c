#include "vced.h"
#include <sys/types.h>
#include <sys/stat.h>

#define INDLEN 56
#define SIZEBUF 65536

filecopy(innam, outnam)
char *innam, *outnam;
{
  long filelen;
  struct stat tmpstat;
  FILE *in, *out;
  long bytes = 0;
  int stepbytes;
  char *inbuf, *outbuf;
  WPTR win;

  win = wxopen(5, 10, 11, 70, " Copy ", BORDER+ACTIVE+NOADJ, BD2, 0,0);

  atsay(1, 1, outnam);
  if((in = fopen(innam, "r")) == (FILE *)NULL)
    return;

  if((out = fopen(outnam, "w")) == (FILE *)NULL)
    return;
  
  inbuf = (char *)malloc(SIZEBUF);
  outbuf = (char *)malloc(SIZEBUF);

  setvbuf(in, inbuf, _IOFBF, SIZEBUF);
  setvbuf(out, outbuf, _IOFBF, SIZEBUF);

  stat(innam, &tmpstat);
  filelen = tmpstat.st_size;
  stepbytes = filelen/INDLEN;
  if(stepbytes < 5120)
    stepbytes = 5120;

  while(!feof(in))
  {
    fputc(fgetc(in), out);
    bytes ++;
    if(bytes % stepbytes == 0)
      Indicator(100*bytes/filelen);
  }
  Indicator(100);
  fclose(in);
  fclose(out);
  wclose(win);
}


Indicator(len)
int len;
{
  int i, j;
  char ind[INDLEN+5];

  j = len*INDLEN/100 + 1;
  for(i = 0; i < j; i ++)
    ind[i] = '█';
  for(; i < INDLEN; i ++)
    ind[i] =  '░';
  ind[i] = 0;
  atsay(3, 1, ind);
}

#ifdef TEST
main(argc, argv)
int argc;
char *argv[];
{
  filecopy(argv[1], argv[2]);
  fputc('\n', stdout);
}
#endif
