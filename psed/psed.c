#include <stdio.h>

main (argc, argv)
int argc;
char *argv[];
{
  FILE *diffs, *in, *out;
  unsigned char tmp[1024];
  int from, to;			  /* значения полей в команде ed */
  char operation;		  /* операция из команды ed */
  int curline = 0;		  /* номер текущей строки во входном файле */
  long tmpseek;			  /* позиция начала пред. строки для возврата */
  char filein[80];
  char *homezakon;
  char tmp1[80];


  if ((diffs = fopen (argv[1], "rb")) == (FILE *) NULL)
  {
    printf ("Не могу открыть файл %s\n", argv[1]);
    exit (1);
  }

  if ((homezakon = (char *) getenv ("HOMEZAKON")) == (char *) NULL)
  {
    printf ("Определите переменную HOMEZAKON\n");
    exit (1);
  }

  out = fopen ("tmpd", "wb");

  while (!feof (diffs))
  {
    fgets (tmp, 1024, diffs);	  /* взяли команду */
    if (feof (diffs))
      continue;
    to = 0;
    if (sscanf (tmp, "%d,%d%c", &from, &to, &operation) != 3 &&
	sscanf (tmp, "%d%c", &from, &operation) != 2)
    {
      if (!strncmp (tmp, "diff +recursive", 15))
      {
	sscanf (tmp, "diff +recursive %*s %s", tmp1);
	sprintf (filein, "%s/%s", homezakon, tmp1);
	fprintf (stderr, "%s\n", filein);
	fclose (in);
	if ((in = fopen (filein, "rb")) == (FILE *) NULL)
	{
	  while (1)
	  {
	    tmpseek = ftell (diffs);
	    fgets (tmp, 1024, diffs);
	    if (feof (diffs) || !strncmp (tmp, "diff +recursive", 15))
	      break;
	  }
	  if (!feof (diffs))
	    fseek (diffs, tmpseek, SEEK_SET);
	}
	curline = 0;
	continue;
      }
      else
      {
	printf ("error command: \"%s\" = %d\n", tmp,
		sscanf (tmp, "%d%c", &from, &operation));
	exit (1);
      }
    }
    printf ("%s", tmp);

    while (curline < from - 1)
    {
      fgets (tmp, 1024, in);
      fputs (tmp, out);
      curline++;
    }

    if (to == 0)
      to = from;

    switch (operation)
    {
      case 'd':
	do
	{
	  fgets (tmp, 1024, in);
	  curline++;
	} while (curline < to);
	break;
      case 'a':
	fgets (tmp, 1024, in);
	fputs (tmp, out);
	while (!feof (diffs))
	{
	  tmpseek = ftell (diffs);
	  fgets (tmp, 1024, diffs);
	  if (*tmp == '>')
	    fputs (tmp + 2, out);
	  else
	    break;
	}
	if (!feof (diffs))
	  fseek (diffs, tmpseek, SEEK_SET);
	break;
      case 'c':
	for (; !feof (diffs) && curline < to; curline++)
	{
	  tmpseek = ftell (diffs);
	  fgets (tmp, 1024, diffs);
	  if (*tmp != '>')
	  {
	    fseek (diffs, tmpseek, SEEK_SET);
	    for (; !feof (diffs) && curline < to; curline++)
	      fgets (tmp, 1024, in);
	    break;
	  }
	  fputs (tmp + 2, out);
	  fgets (tmp, 1024, in);
	}
	while (!feof (diffs))
	{
	  tmpseek = ftell (diffs);
	  fgets (tmp, 1024, diffs);
	  if (*tmp == '>' && !feof (diffs))
	    fputs (tmp + 2, out);
	  else
	    break;
	}
	if (!feof (diffs))
	  fseek (diffs, tmpseek, SEEK_SET);
	break;
    }
    curline = to;
  }
  while (fgets (tmp, 1024, in) &&!feof (in))
    fputs (tmp, out);

  fclose (in);
  fclose (out);
  fclose (diffs);
}
