/*
 *  $Id: compiler.c,v 1.3 1993/05/24 13:47:39 sev Exp $
 *
 * ---------------------------------------------------------- 
 *
 * $Log: compiler.c,v $
 * Revision 1.3  1993/05/24 13:47:39  sev
 * Убрал обидную ошибку в отчетк
 * 	ю
 *
 * Revision 1.2  1993/05/24  13:40:10  sev
 * Добавлен новый ключ -f - компиляция только одного файла
 *
 * Revision 1.1  1993/03/01  16:00:44  sev
 * Initial revision
 *
 *
 */

static char rcsid[]="$Id: compiler.c,v 1.3 1993/05/24 13:47:39 sev Exp $";

/*
	Файл compiler.c
	Запорожье 1993.
*/

#define BUF 1024
#define PATH 80


#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

/*============================================================*/
/*================ Начало главной программы ==================*/
/*============================================================*/
main(argc,argv)       /*         begin of main-programm       */
int  argc;
char **argv;
{

  struct stat STATBUF[1];

  FILE *fopen();
  FILE *in_file_config;
  FILE *in_file_table;
  FILE *current_file;

  char string[BUF];   /*           Строка из файла            */
  char path_file[PATH];
  char name_file[PATH];
  char work[PATH];
  char path[PATH];
  char *pointer;
  char *strstr();
  char *strchr();
  char *strcat();
  char *strncpy();
  char *strcpy();
  char *strrchr();

  time_t last_time_modify_config;
  time_t last_time_modify;

  long main_pointer_in_file;
  long current_pointer_in_file;
  int flag=0;
  int  counter;
  int  more;
  int  yes_no_compile;
  int  set;
  int  not_all_compile;

  void compile();


  if(argc<2)         /*      Если нет входных параметров     */
  {
    puts("Компилятор гипертекста. Версия 1.1\n\n\
 Использование:\n\t\tcompiler [-f] file\n");
    exit(0);          /*             Выход в DOS              */
  }

  if(!strcmp(argv[1], "-f"))
  {
    if( (current_file = fopen(argv[2],"r+")) == (FILE *)NULL )
    {
      printf(" Не могу открыть файл %s\n",argv[2]);
      exit(1);
    }

    printf("Компилируется файл : %s",argv[2]);
    fseek(current_file,0l,SEEK_SET);
    compile(&current_file);
    printf("\rОткомпилирован файл : %s\n",argv[2]);
    fclose(current_file);

    exit(0);
  }

  if((in_file_config=fopen(argv[1],"r+"))==(FILE *)NULL)
  {
    printf(" Ошибка :не могу открыть файл %s\n",argv[1]);
    exit(0);          /*             Выход в DOS              */
  }

  main_pointer_in_file    = 0l;/*                             */
  last_time_modify_config = 0; /*      Инициализация          */
  yes_no_compile          = 1; /*        переменных           */
  counter                 = 0; /*                             */

  while( fgets(string,BUF,in_file_config) != (char *)NULL )
  {
    if( counter == 0 )
    {
      strcpy(path_file,string);

      while( strchr(path_file,'\\') )
        *(strchr(path_file,'\\'))='/';

      memset(path,'\0',PATH);
      strncpy(path,path_file,
              strlen(path_file)-strlen(strrchr(path_file,'/'))+1);
      counter ++;
    }
    if( strstr(string,"for compiler : ")!=(char *)NULL)
    {
      sscanf(string,"for compiler : %ld",&last_time_modify_config);
      flag=1;
      break;
    }
    main_pointer_in_file = ftell(in_file_config);
  }
  flag=(flag==1)?0:1;
  if( last_time_modify_config == 0 )
  {
    stat(argv[1],STATBUF);
    last_time_modify_config = STATBUF->st_mtime;
    fseek(in_file_config,main_pointer_in_file,SEEK_SET);
    fprintf(in_file_config,"for compiler : %ld\n",last_time_modify_config);
    if( ( in_file_table=fopen("ft_hyp","a") ) == (FILE *)NULL )
    {
      printf(" Ошибка :нет места на диске\n");
      exit(0);        /*             Выход в DOS              */
    }

    fclose(in_file_table);
    in_file_table=fopen("ft_hyp","r+");
    fprintf(in_file_table,"%d%s",yes_no_compile,path_file);
  }
  else
  {
    if( ( in_file_table=fopen("ft_hyp","r+") ) == (FILE *)NULL )
    {
      if( ( in_file_table=fopen("ft_hyp","a+") ) == (FILE *)NULL )
      {
        printf(" Ошибка :нет места на диске\n");
        exit(0);        /*             Выход в DOS              */
      }
      else
      {
        fclose(in_file_table);
        in_file_table=fopen("ft_hyp","r+");
        fprintf(in_file_table,"%d%s",yes_no_compile,path_file);
      }
    }
  }

  main_pointer_in_file = 0l;
  counter               = 0;
  fseek(in_file_table,0l,SEEK_SET);

  while( fgets(string,BUF,in_file_table) != (char *)NULL )
  {
    if( string[0] == '1' )
    {
      counter ++;
      break;
    }
  }

  if( counter == 0 )
    more = 0;
  else
    more = 1;

  not_all_compile = 0;

  while( more )
  {
    fseek(in_file_table,main_pointer_in_file,SEEK_SET);

    if( fgets(string,BUF,in_file_table) == (char *)NULL )
    {
      more = 0;
      continue;
    }

    current_pointer_in_file = ftell(in_file_table);
    strcpy(path_file,&string[1]);
    *(strchr(path_file,'\n')) = '\0';

    if( (current_file = fopen(path_file,"r+")) == (FILE *)NULL )
    {
      printf(" Не могу открыть файл %s\n",path_file);
      main_pointer_in_file = current_pointer_in_file;
      not_all_compile = 1;
      remove("ft_hyp");
      continue;
    }

    while( ! feof( current_file ))
    {
      fgets(string,BUF,current_file);

      if( pointer = strstr(string,"\033<") )
      {
        pointer = strrchr(pointer,';');
        sscanf(pointer,";%s>",work);
        *(strchr(work,'>')) = '\0';
        strcpy(name_file,path);
        strcat(name_file,work);

        if( strchr(name_file,'*') )
        {
          counter = 1;
          set = SEEK_END;
        }
        else
        {
          set = SEEK_SET;
          counter = 0;
        }

        fseek(in_file_table,0l,set);

        while( fgets(string,BUF,in_file_table) != (char *)NULL )
          if( strstr(string,name_file) )
          {
            counter ++;
            break;
          }

        if( counter == 0 )
        {
          fseek(in_file_table,0l,SEEK_END);
          fprintf(in_file_table,"%d%s\n",yes_no_compile,name_file);
        }
      }
    }

    stat(path_file,STATBUF);
    last_time_modify = STATBUF->st_mtime;

    if(( (last_time_modify > last_time_modify_config)) ||( flag==1))
    {
      printf("Компилируется файл : %s",path_file);
      fseek(current_file,0l,SEEK_SET);
      compile(&current_file);
      printf("\rОткомпилирован файл : %s\n",path_file);
    }
    else
      printf("Файл %s был уже откомпилирован \n",path_file);

    fseek(in_file_table,main_pointer_in_file,SEEK_SET);
    yes_no_compile = 0;
    fprintf(in_file_table,"%d",yes_no_compile);
    yes_no_compile = 1;
    main_pointer_in_file = current_pointer_in_file;
    fclose(current_file);
  }

  last_time_modify_config =  0;
  main_pointer_in_file    = 0l;
  fseek( in_file_config,0l,SEEK_SET );

  if ( !not_all_compile )
    while( fgets(string,BUF,in_file_config) != (char *)NULL )
    {
      if( strstr(string,"for compiler : ") )
      {
        fseek( in_file_config,main_pointer_in_file,SEEK_SET );
        fprintf(in_file_config,"for compiler : %0.9ld",
                last_time_modify_config);
        printf(" Компиляция прошла успешно.\n ");
        fclose(in_file_table);
        fclose(in_file_config);
        remove("ft_hyp");
        exit(0);
      }
      main_pointer_in_file = ftell(in_file_config);
    }
  else
  {
    fclose(in_file_table);
    fclose(in_file_config);
    printf(" Компиляция произведена не полностью.\n ");
  }
  return 0;
}                     /*          end of main-programm        */
/*============================================================*/
/*================ Конец главной программы ===================*/
/*============================================================*/



/*------------------------------------------------------------*/
/*-------------- Начало подпрограммы компиляции --------------*/
/*------------------------------------------------------------*/
void compile(in_file) /*          begin of subroutine         */
FILE *(*in_file);        /*              Входной файл            */
{
  char string[BUF];   /*           Строка из файла            */

  long current_pg;    /*    Указатели на начальную строку     */
  long current_work;  /*          в текущей странице          */
  long previous_pg;   /*     Указатель на начальную строку    */
  long previous_work; /*         в предыдущей странице        */
  long ftell();

  current_pg = 0;     /*                                      */
  previous_pg = 0;    /*      Инициализация переменных        */
  previous_work = 0;  /*                                      */
  current_work = 0;   /*                                      */

  while( fgets(string,BUF,*in_file) != (char *)NULL )
	{

    if( strchr(string,'\014') == (char *)NULL ) /*   Поиск символа    */
      continue;                         	/*   новой сраницы    */

      previous_work=current_pg;         	/*     Запомнить      */
      current_pg=previous_pg;           	/*     указатель      */
      previous_pg=ftell(*in_file);      	/*    на страницу     */

    current_work=ftell(*in_file);
    fseek(*in_file,current_work-strlen(string),SEEK_SET);
    fprintf(*in_file,"\014%8ld\n",previous_work);/*  Запись  */
                                                 /*  в  файл */
    fseek(*in_file,current_work,SEEK_SET);

  }                   /*                while                 */
  return;
}                     /*            end of subroutine         */
/*------------------------------------------------------------*/
/*-------------- Конец подпрограммы компиляции ---------------*/
/*------------------------------------------------------------*/
