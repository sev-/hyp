/*
 * $Id: edit_str.c,v 1.2 1994/11/12 19:20:53 sev Exp $
 * 
 * ----------------------------------------------------------
 * 
 * $Log: edit_str.c,v $
 * Revision 1.2  1994/11/12 19:20:53  sev
 * Indented and added gzip
 * Revision 1.1  1994/03/05  21:56:56  sev Initial revision
 * 
 * Revision 1.1  1993/03/04  09:50:43  kas Initial revision
 * 
 * 
 */

static char rcsid[] = "$Id: edit_str.c,v 1.2 1994/11/12 19:20:53 sev Exp $";

#define VCKEY_DEFS
#include<string.h>
#include<stdlib.h>
#include<vcstdio.h>

#define ENTER		RET	  /* ВК */
#define HOMEKEY		HOME_KEY  /* РТ */
#define PGUPKEY		PGUP	  /* УТ */
#define LEFTKEY		CUR_LEFT
#define ENDKEY		END_KEY	  /* ВПС */
#define RIGHTKEY	CUR_RIGHT
#define DELKEY		DEL_KEY	  /* ВНТ */
#define BACKSPACE	BACKSP	  /* ВШ */
#define OVER		INS_KEY	  /* ПТ */
#define HELP		F1


/**** Функция редактирования строки в окне ****/

/*
 * Функция позволяет редактировать в окне заданного размера строку
 *  сдвигая ее влево иои вправо и помечая выход строки за рамки окна. Функция возвращает отредактированную строку в том
 *  в котором была передана. Вход: row             - номер строки окна
 * start_col       - левая координата окна на экране; end_col         - правая координата окна
 * buffersize      - размер бувера строки, байт; originalstring  - указатель на
 * title           - указатель на заголовок окна; sourceattr      - атрибуты для
 * destattr        - атрибуты редактированной строки; Выход: Функция
 * , если строка не редактировалась ( была нажата  клавиша ESCAPE ), либо 1, если строка была отредактирована
 */

/* функции - прототипы */
int edit_string();
void update();
void put_status();
void delete_char();
void insert_char();
void chmod();
void init_edit_string_help();
void edit_string_help_msg();

int edit_string(row, start_col, end_col, buffersize, originalstring,
		 title, sourceattr, destattr)
int row;
int start_col;
int end_col;
int buffersize;
char *originalstring;
char *title;
char sourceattr;
char destattr;
{
  int ch;			  /* код символа */
  int endcolumn, width;

  /* начальная и конечная координаты строки на экране, ширина окна */
  int beg_status, end_status;

  /* флаги выхода строки за рамки окна */
  int length, rest;

  /* текущая длина строки, длина остатка строки справа от курсора */
  char *buffer;			  /* буфер для редактирования */
  int entry;			  /* номер вхождения в цикл */
  int cursorpos_w;		  /* позиция курсора в окне */
  char *pos_beg;		  /* начало видимой части текста */
  char *cursorpos_s;		  /* указатель на позицию курсора в строке */
  int ret_status;		  /* возвращаемое значение */
  int wdo;			  /* указатель на окно */
  int mode;			  /* текущий режим ( вставка, замена ) переключение ПТ */

  /* зарезервировать буфер для редактирования строки размера buffersize */
  if (!(buffer = (char *) malloc(buffersize + 5)))
    return 0;

  /* открыть окно */
  if ((wdo = wxopen(row - 1, start_col - 1, row + 1, end_col + 1, title,
	     BORDER + BD1 + ACTIVE + CURSOR + NOADJ + CURTAIN, 0, 0)) == -1)
    return 0;

  /* вычислить промежуточные значения */
  width = endcolumn = end_col - start_col;

  /* максимальная длина строки */
  buffersize--;

  /*
   * скопировать строку в буфер и получить ее длину, задать начальное значение остатка
   */
  memset(buffer, 0, buffersize + 5);
  length = rest = strlen(originalstring);
  strcpy(buffer, originalstring);

  /* начальное положение курсоров и нач. знач. всех индикаторов */
  cursorpos_s = pos_beg = buffer;
  cursorpos_w = 0;
  beg_status = 0;
  end_status = (rest <= width) ? 0 : 1;
  mode = 0;			  /* вставка */
  init_edit_string_help();

  /* вывести строку и текущий режим на экран */
  update(row, start_col, end_col, beg_status, end_status, pos_beg,
	 sourceattr);
  chmod(row, end_col, mode);

  /* установить курсор на нужную позицию */
  at(0, cursorpos_w);

  entry = 0;			  /* нулевое вхождение в цикл */

key_loop:
  /* прочитать код с клавиатуры */
  switch (ch = getone())
  {
    case ESC:			  /* нажата клавиша АР2 */
      ret_status = 0;
      goto quit;		  /* перейтина возврат */
    case ENTER:		  /* нажата клавиша ВК */
      ret_status = 1;
      /* скопировать отредактированную строку в первоначальный буфер */
      strcpy(originalstring, buffer);
      goto quit;		  /* перейтина возврат */
    case HOMEKEY:		  /* нажата клавиша РТ */
      /* если нулевое вхождение, то установить атрибуты редактированной стрроки */
      if (!entry)
	entry = 1;
      else
      {
	if (rest != length)
	{
	  cursorpos_w = 0;
	  cursorpos_s = pos_beg = buffer;
	  rest = length;
	  beg_status = 0;
	  end_status = (rest <= width) ? 0 : 1;
	}
	else
	  goto key_loop;
      }
      update(row, start_col, end_col, beg_status,
	     end_status, pos_beg, destattr);
      break;
    case PGUPKEY:		  /* нажата клавиша УТ */
      if (!entry)
	entry = 1;
      length = rest = strlen(originalstring);
      memset(buffer, 0, buffersize + 5);
      strcpy(buffer, originalstring);
      cursorpos_s = pos_beg = buffer;
      cursorpos_w = 0;
      beg_status = 0;
      end_status = (rest <= width) ? 0 : 1;
      update(row, start_col, end_col, beg_status,
	     end_status, pos_beg, destattr);
      break;
    case LEFTKEY:
      if (!entry)
	entry = 1;
      if (cursorpos_w > 0)
      {
	cursorpos_w--;
	cursorpos_s--;
	rest++;
	break;
      }
      else
      {
	if (rest < length)
	{
	  cursorpos_s--;
	  rest++;
	  pos_beg--;
	  beg_status = (rest == length) ? 0 : 1;
	  end_status = (rest <= width) ? 0 : 1;
	  update(row, start_col, end_col, beg_status,
		 end_status, pos_beg, destattr);
	}
	break;
      }
    case ENDKEY:		  /* нажата клавиша ВПС */
      if (!entry)
	entry = 1;
      if (rest)
      {
	cursorpos_s = buffer + length;
	end_status = 0;
	rest = 0;
	if (length < width)
	{
	  cursorpos_w = length;
	  pos_beg = buffer;
	  beg_status = 0;
	}
	else
	{
	  cursorpos_w = endcolumn;
	  pos_beg = buffer - width + length;
	  beg_status = 1;
	}
	update(row, start_col, end_col, beg_status,
	       end_status, pos_beg, destattr);
	break;
      }
      goto key_loop;
    case RIGHTKEY:
      if (!entry)
      {
	update(row, start_col, end_col, beg_status,
	       end_status, pos_beg, destattr);
	entry = 1;
      }
      if (cursorpos_w < endcolumn)
      {
	if (rest)
	{
	  cursorpos_w++;
	  cursorpos_s++;
	  rest--;
	  break;
	}
	goto key_loop;
      }
      else
      {
	if (rest)
	{
	  cursorpos_s++;
	  rest--;
	  pos_beg++;
	  beg_status = ((length - rest) < width) ? 0 : 1;
	  end_status = (rest <= 1) ? 0 : 1;
	  update(row, start_col, end_col, beg_status,
		 end_status, pos_beg, destattr);
	}
	break;
      }
    case DELKEY:		  /* нажата клавиша ВНТ */
      if (!entry)
	entry = 1;
      if (rest)
      {
	delete_char(cursorpos_s, rest);
	rest--;
	length--;
	if (rest < endcolumn - cursorpos_w)
	{
	  end_status = 0;
	  if (beg_status)
	  {
	    cursorpos_w++;
	    pos_beg--;
	    beg_status = (length - rest < cursorpos_w) ? 0 : 1;
	    update(row, start_col, end_col, beg_status,
		   end_status, pos_beg, destattr);
	    break;
	  }
	  update(row, start_col, end_col, beg_status,
		 end_status, pos_beg, destattr);
	}
	else
	{
	  end_status = (rest <= width - cursorpos_w) ? 0 : 1;
	  update(row, start_col, end_col, beg_status,
		 end_status, pos_beg, destattr);
	}
      }
      break;
    case BACKSPACE:		  /* нажата клавиша ВШ */
      if (!entry)
	entry = 1;
      if (rest < length)
      {
	cursorpos_s--;
	delete_char(cursorpos_s, rest + 1);
	length--;
	if (rest <= width - cursorpos_w)
	{
	  end_status = 0;
	  if (beg_status)
	  {
	    beg_status = (length - rest <= cursorpos_w) ? 0 : 1;
	    pos_beg--;
	    update(row, start_col, end_col, beg_status,
		   end_status, pos_beg, destattr);
	  }
	  else
	  {
	    cursorpos_w--;
	    update(row, start_col, end_col, beg_status,
		   end_status, pos_beg, destattr);
	    break;
	  }
	}
	else
	{
	  if (cursorpos_w > 0)
	  {
	    cursorpos_w--;
	    end_status = (rest <= width - cursorpos_w) ? 0 : 1;
	    update(row, start_col, end_col, beg_status,
		   end_status, pos_beg, destattr);
	    break;
	  }
	  else
	  {
	    if (rest == length)
	    {
	      beg_status = 0;
	      update(row, start_col, end_col, beg_status,
		     end_status, pos_beg, destattr);
	    }
	  }
	}
      }
      break;
    case OVER:			  /* нажата клавиша ПТ */
      if (mode)
	mode = 0;		  /* вставка */
      else
	mode = 1;		  /* замена */
      chmod(row, end_col, mode);
      break;
    case HELP:
      edit_string_help_msg();
      break;
    default:
      if (ch < 32 || ch > 255)
	goto key_loop;
      if (!entry)
      {
	memset(buffer, 0, buffersize + 5);
	length = rest = beg_status = end_status = cursorpos_w = 0;
	cursorpos_s = pos_beg = buffer;
	entry = 1;
	update(row, start_col, end_col, beg_status,
	       end_status, pos_beg, destattr);
	at(0, 0);
      }
      if ((!mode) || (!rest))
      {
	/* если буфер полон */
	if (length >= buffersize)
	  goto key_loop;
	insert_char(cursorpos_s, rest, ch);
	length++;
	if (cursorpos_w < endcolumn)
	{
	  end_status = (rest < width - cursorpos_w) ? 0 : 1;
	  update(row, start_col, end_col, beg_status,
		 end_status, pos_beg, destattr);
	  cursorpos_s++;
	  cursorpos_w++;
	  break;
	}
	pos_beg++;
	cursorpos_s++;
	beg_status = (length - rest < cursorpos_w - width) ? 0 : 1;
	update(row, start_col, end_col, beg_status,
	       end_status, pos_beg, destattr);
      }
      else
	/* режим замены */
      {
	cursorpos_s[0] = ch;
	if (cursorpos_w == width)
	  pos_beg++;
	else
	  cursorpos_w++;

	rest--;
	cursorpos_s++;
	update(row, start_col, end_col, beg_status,
	       end_status, pos_beg, destattr);
      }
  }
  at(0, cursorpos_w);
  goto key_loop;

quit:
  free(buffer);
  wclose(wdo);
  if (!*originalstring)
    ret_status = 0;
  return ret_status;
}


void update(row, startcolumn, endcolumn, beg_status, end_status
	    ,pos_beg, attr)
int row;
int startcolumn;
int endcolumn;
int beg_status;
int end_status;
char *pos_beg;
char attr;
{
  register int counter;

  at(0, 0);
  for (counter = 0; counter <= (endcolumn - startcolumn); counter++)
    vcputc(pos_beg[counter], attr);

  put_status(row, startcolumn, endcolumn, beg_status, end_status);
}

void put_status(row, startcolumn, endcolumn, beg_status, end_status)
int row;
int startcolumn;
int endcolumn;
int beg_status;
int end_status;
{
  /* отключение оконной логики */
  override = 1;

  at(row, startcolumn - 1);

  if (beg_status)
    vcputc('<', vc.white + vc.bg * vc.black);
  else
    vcputc('\272', vc.white + vc.bg * vc.black);

  at(row, endcolumn + 1);

  if (end_status)
    vcputc('>', vc.white + vc.bg * vc.black);
  else
    vcputc('\272', vc.white + vc.bg * vc.black);

  /* включение оконной логики */
  override = 0;
}

void delete_char(sourcestring, stringlength)
char *sourcestring;
int stringlength;
{
  register int counter;

  for (counter = 1; counter <= stringlength; counter++)
    sourcestring[counter - 1] = sourcestring[counter];
}

void insert_char(sourcestring, stringlength, character)
char *sourcestring;
int stringlength;
int character;
{
  register int counter;

  for (counter = stringlength + 1; counter >= 0; counter--)
    sourcestring[counter] = sourcestring[counter - 1];
  sourcestring[0] = character;
}

void chmod(row, end_col, mode)
int row;
int end_col;
int mode;
{
  /* отключение оконной логики */
  override = 1;

  at(row - 1, end_col - 7);

  if (mode)
    vcputs(" Ovr ", vc.white + vc.bg * vc.black);
  else
    vcputs(" Ins ", vc.white + vc.bg * vc.black);

  /* включение оконной логики */
  override = 0;
}

char edit_string_help_sm[] = " ПФ1 - помощь ";
char *edit_string_help_sm_wind[] =
{
  "21Р2    - Выход без поиска.               ",
  "ВПС    - Вывод исходной строки.          ",
  "ПТ     - В начало строки.                ",
  "ВНТ    - В конец.                        ",
  "РТ     - Переключение режима Ins/Ovr     ",
  "ЗБ     - Удаление символа перед курсором.",
  "УТ     - Удаление символа над курсором.  ",
};

char edit_string_help_ws[] = " PF1 - помощь ";
char *edit_string_help_ws_wind[] =
{
  "ESC    - Выход без поиска.               ",
  "PgUp   - Вывод исходной строки.          ",
  "Home   - В начало строки.                ",
  "End    - В конец.                        ",
  "Insert - Переключение режима Ins/Ovr     ",
  "BkSp   - Удаление символа перед курсором.",
  "Del    - Удаление символа над курсором.  ",
};

char edit_string_help_ansi[] = " F1  - помощь ";
char *edit_string_help_ansi_wind[] =
{
  "ESC    - Выход без поиска.               ",
  "PgUp   - Вывод исходной строки.          ",
  "Home   - В начало строки.                ",
  "End    - В конец.                        ",
  "Insert - Переключение режима Ins/Ovr     ",
  "BkSp   - Удаление символа перед курсором.",
  "Del    - Удаление символа над курсором.  ",
};

char *edit_string_help_string;	  /* строка в нижней части экрана */
char *edit_string_help_window[25];/* строки в окне		 */

void init_edit_string_help()	  /* установка помощи для каждого терминала */
{
  char *terminal;		  /* тип терминала		 */
  int i;

  terminal = getenv("TERM");

  if (strcmp(terminal, "sm7238") == 0)
  {
    edit_string_help_string = edit_string_help_sm;
    for (i = 0; i < sizeof(edit_string_help_sm_wind) / sizeof(char *); i++)
      edit_string_help_window[i] = edit_string_help_sm_wind[i];
  }
  else if (strcmp(terminal, "ws685") == 0)
  {
    edit_string_help_string = edit_string_help_ws;
    for (i = 0; i < sizeof(edit_string_help_sm_wind) / sizeof(char *); i++)
      edit_string_help_window[i] = edit_string_help_ws_wind[i];
  }
  else
  {
    edit_string_help_string = edit_string_help_ansi;
    for (i = 0; i < sizeof(edit_string_help_sm_wind) / sizeof(char *); i++)
      edit_string_help_window[i] = edit_string_help_ansi_wind[i];
  }
}

void edit_string_help_msg()	  /* функция вызывается по F1 */
{
  int edit_string_help_w;	  /* окно помощи			 */
  int i;

  edit_string_help_w = wxopen(10 - sizeof(edit_string_help_sm_wind) / sizeof(char *) / 2,
			      35 - strlen(edit_string_help_window[0]) /2,
		 15 + sizeof(edit_string_help_sm_wind) / sizeof(char *) / 2,
			      45 + strlen(edit_string_help_window[0]) /2,
			   " Помощь ", ACTIVE + BORDER + BD1 + NOADJ, 0, 0);
  for (i = 0; i < sizeof(edit_string_help_sm_wind) / sizeof(char *); i++)
    atsay(i + 1, 5, edit_string_help_window[i]);
  i += 2;
  atsay(i, 9, "Нажмите любую клавишу");
  getone();
  wclose(edit_string_help_w);
}
