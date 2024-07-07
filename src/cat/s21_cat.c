#include "s21_cat.h"

int main(int argc, char **argv)
{
  int er_status = OK;
  Flags Flag = {0, 0, 0, 0, 0, 0};
  er_status = parser(argc, argv, &Flag);
  for (int i = 1; (i < argc) && (er_status == OK); ++i)
  {
    if (argv[i][0] != '-')
    {
      er_status = file_handler(argv[i], &Flag);
    }
  }
  return (er_status);
}

int parser(int argc, char **argv, Flags *flag)
{

  int er_stat_parse = OK;
  int opt_long = CLEAR;

  for (int i = 1; i < argc; ++i)
  {
    if (argv[i][0] == '-')
    {
      size_t arg_len = strlen(argv[i]);
      char *opt_phrase = argv[i] + 2;
      for (size_t j = 1; (j < arg_len) && opt_long == CLEAR && er_stat_parse == OK;
           ++j)
      {
        switch (argv[i][j])
        {
        case '-':
          if (strcmp(opt_phrase, "help") == 0)
          {
            file_handler("help21.txt", flag);
            er_stat_parse = HELP;
          }
          else if (strcmp(opt_phrase, "number") == 0)
          {
            flag->n = opt_long = SET;
          }
          else if (strcmp(opt_phrase, "number-nonblank") == 0)
          {
            flag->b = opt_long = SET;
          }
          else if (strcmp(opt_phrase, "squeeze-blank") == 0)
          {
            flag->s = opt_long = SET;
          }
          else if (strcmp(opt_phrase, "show-nonprinting") == 0)
          {
            flag->v = opt_long = SET;
          }
          else if (strcmp(opt_phrase, "show-ends") == 0)
          {
            flag->e = opt_long = SET;
          }
          else if (strcmp(opt_phrase, "show-tabs") == 0)
          {
            flag->t = opt_long = SET;
          }
          else if (strcmp(opt_phrase, "show-all") == 0)
          {
            flag->v = flag->t = flag->e = opt_long = SET;
          }
          else
          {
            fprintf(stderr, "s21_cat: unrecognized option --%s\n",
                    opt_phrase);
            puts("Try './s21_cat --help' for more information.");
            er_stat_parse = ERROR;
          }
          break;
        case 'b':
          flag->b = SET;
          break;
        case 'n':
          flag->n = SET;
          break;
        case 's':
          flag->s = SET;
          break;
        case 'v':
          flag->v = SET;
          break;
        case 'T':
          flag->t = SET;
          break;
        case 'E':
          flag->e = SET;
          break;
        case 'e':
          flag->v = flag->e = SET;
          break;
        case 't':
          flag->v = flag->t = SET;
          break;
        case 'A':
          flag->v = flag->t = flag->e = SET;
          break;
        default:
          fprintf(stderr, "s21_cat: invalid option -- '%c'\n", argv[i][j]);
          puts("Try 's21_cat --help' for more information.");
          er_stat_parse = ERROR;
        }
      }
    }
  }
  return er_stat_parse;
}

int file_handler(char *path, Flags *flag)
{
  FILE *file_ptr;
  int errcode = OK;
  // if (NULL == (file_ptr = fopen(path, "r")))
  /*if (NULL == (file_ptr = fopen_s(&file_ptr, path, "r"))) {
    fprintf(stderr, "s21_cat: %s: %s\n", path, strerror(2));
    errcode = ERROR;
  }*/
  char buf[1024];
  if (0 != (errcode = fopen_s(&file_ptr, path, "r")))
  {
    // fprintf(stderr, "s21_cat: %s: %s\n", path, strerror(errcode));
    // fprintf(stderr, "s21_cat: %s: %s\n", path, strerror_s(buf, sizeof(buf), errcode));
    fprintf(stderr, "s21_cat: %s: %d\n", path, strerror_s(buf, sizeof(buf), errcode));
    errcode = ERROR;
  }
  else
  {
    int c = fgetc(file_ptr);

    if (file_exist(file_ptr))
    {
      unsigned num_str = 0;
      char flag_position = 0;
      b_handler(file_ptr, &c, flag, &num_str, &flag_position);
      n_handler(file_ptr, &c, flag, &num_str);
      s_handler(file_ptr, &c, flag, &num_str, &flag_position);
      flag_position = 1;
      do
      {
        v_handler(&c, flag);
        t_handler(&c, flag);
        s_handler(file_ptr, &c, flag, &num_str, &flag_position);
        e_handler(&c, flag);
        putchar(c);
        b_handler(file_ptr, &c, flag, &num_str, &flag_position);
        n_handler(file_ptr, &c, flag, &num_str);
        c = fgetc(file_ptr);
      } while (file_exist(file_ptr));
    }
    fclose(file_ptr);
  }
  return errcode;
}

int file_exist(FILE *path)
{
  int status = ERROR;

  if (0 != feof(path))
    status = OK;
  else if (0 != ferror(path))
  {
    status = OK;
  }
  return status;
}

void b_handler(FILE *path_file, const int *c, Flags *flag, unsigned *num_str, const char *flag_position)
{
  if (flag->b)
  {
    int next_char = fgetc(path_file);

    flag->n = CLEAR;
    if (file_exist(path_file))
    {
      if ((*c == '\n' && next_char != '\n' && *flag_position == 1) ||
          (*c != '\n' && *flag_position == 0))
      {
        printf("%6u\t", ++(*num_str));
      }
      fseek(path_file, -1, SEEK_CUR);
    }
  }
}

void n_handler(FILE *path_file, const int *c, Flags *flag, unsigned *num_str)
{
  if (flag->n)
  {
    fgetc(path_file);
    if (file_exist(path_file))
    {
      if (*c == '\n' || *num_str == 0)
      {
        printf("%6u\t", ++(*num_str));
      }
      fseek(path_file, -1, SEEK_CUR);
    }
  }
}

void s_opt_handler(FILE *path_file, int *next_char, int *c, Flags *flag, unsigned *num_str, const char *flag_position)
{
  if (*next_char == '\n')
  {
    if (flag->e)
    {
      printf("$");
    }
    while (*next_char == '\n')
    {
      *next_char = fgetc(path_file);
    }
    putchar('\n');
    if (*flag_position == 0)
    {
      *c = *next_char;
    }
    if (flag->n || (flag->b && *flag_position == 0))
    {
      printf("%6u\t", ++(*num_str));
    }
  }
}

void s_handler(FILE *file_ptr, int *c, Flags *flag, unsigned int *num_str,
               char *flag_position)
{
  if (flag->s)
  {
    int fut_c = fgetc(file_ptr);

    if (*flag_position == 0)
    {
      if (*c != '\n')
      {
        fseek(file_ptr, -1, SEEK_CUR);
      }
      s_opt_handler(file_ptr, &fut_c, c, flag, num_str, flag_position);
    }
    else if (*flag_position == 1)
    {
      if (*c == '\n')
      {
        s_opt_handler(file_ptr, &fut_c, c, flag, num_str, flag_position);
      }
      if (file_exist(file_ptr))
      {
        fseek(file_ptr, -1, SEEK_CUR);
      }
    }
  }
}

void v_handler(int *c, Flags *flag)
{
  if (flag->v)
  {
    if ((0 <= *c && *c <= 8) || (11 <= *c && *c <= 31))
    {
      putchar('^');
      *c += 64;
    }
    if (*c == 127)
    {
      putchar('^');
      *c -= 64;
    }
  }
}

void t_handler(int *c, Flags *flag)
{
  if (flag->t && *c == '\t')
  {
    putchar('^');
    *c += 64;
  }
  v_handler(c, flag);
}

void e_handler(int *c, Flags *flag)
{
  if (flag->e && (*c == '\n'))
  {
    printf("$");
  }
  v_handler(c, flag);
}