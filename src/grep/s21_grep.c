#include "s21_grep.h"

int main(int argc, char **argv) {
  int errcode = NO_MATCHES_FOUND;

  if (argc > 2) {
    Options Opt = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    char pattern[SIZE] = {0};
    int opt_symbol = 0;
    char *optstring = "violnce:f:sh?";

    while (-1 != (opt_symbol = getopt_long(argc, argv, optstring, 0, NULL))) {
      errcode = init_struct(&Opt, opt_symbol, pattern);
    }
    if ((Opt.e || Opt.f) & (argc < 4)) {
      errcode = ERROR;
    }
    if (ERROR != errcode) {
      executor((const char **)argv, pattern, &Opt);
    }
  }
  return errcode;
}

int executor(const char **argv, const char *pattern, Options const *Opt) {
  int errcode = NO_MATCHES_FOUND;
  int num_files = 0;
  int flag_no_pattern_opt = CLEAR;

  if (Opt->e || Opt->f) {
    num_files = file_counter(argv, flag_no_pattern_opt);
  } else {
    flag_no_pattern_opt = SET;
    pattern = argv[optind];
    num_files = file_counter(argv, flag_no_pattern_opt);
  }

  errcode = file_handler(argv, pattern, num_files, flag_no_pattern_opt, Opt);

  return errcode;
}

int file_counter(const char **argv, int flag_no_pattern_opt) {
  int num_files = 0;
  int ind = optind;

  if (flag_no_pattern_opt) {
    ind += 1;
  }
  for (int i = ind; NULL != argv[i]; ++i) {
    if (argv[i][0] != '-') {
      num_files += 1;
    }
  }
  return num_files;
}

int file_handler(const char **argv, const char *pattern, int num_files,
                 int flag_no_pattern_opt, Options const *Opt) {
  int errcode = NO_MATCHES_FOUND;

  for (int index_loop = 0; index_loop < num_files; ++index_loop) {
    FILE *file_ptr;
    int ind_file_arg = optind + index_loop + flag_no_pattern_opt;
    const char *file_name = argv[ind_file_arg];

    if (NULL == (file_ptr = fopen(file_name, "r"))) {
      if (!Opt->s) {
        fprintf(stderr, "s21_grep: %s: %s\n", file_name, strerror(2));
      }
      errcode = ERROR;
    } else {
      char opt_l_handling_is = CLEAR;
      regex_t preg;
      unsigned int num_matching_strings = 0;
      int regcode = Opt->i ? regcomp(&preg, pattern, REG_ICASE)
                           : regcomp(&preg, pattern, REG_EXTENDED);

      if (OK != regcode) {
        char reg_errbuf[LITTLE_SIZE] = {0};

        regerror(regcode, &preg, reg_errbuf, LITTLE_SIZE);
        fprintf(stderr, "Regexp compilation failed: '%s'\n", reg_errbuf);
        errcode = ERROR;
      }
      if (ERROR != errcode) {
        char buf_str[SIZE] = {0};
        for (int num_str = 1; NULL != fgets(buf_str, SIZE, file_ptr);
             ++num_str) {
          if ((!Opt->v && (regexec(&preg, buf_str, 0, NULL, 0) == OK)) ||
               (Opt->v && (regexec(&preg, buf_str, 0, NULL, 0) != OK))) {
            if (Opt->c) {
              Opt->l ? num_matching_strings = 1 : ++num_matching_strings;
            }
            if (Opt->l) {
              opt_l_handling_is = SET;
            } else {
              errcode = opt_handler(file_name, num_files, num_str, buf_str,
                                    pattern, Opt);
            }
          }
        }
      }

      if (Opt->c) {
        errcode = c_handler(Opt, num_files, file_name, num_matching_strings);
      }
      if (opt_l_handling_is == SET) {
        printf("%s\n", file_name);
        errcode = OK;
      }
      regfree(&preg);
      fclose(file_ptr);
    }
  }
  return errcode;
}

int opt_handler(const char *file_name, int num_files, int num_str,
                char *buf_str, const char *pattern, Options const *Opt) {
  int errcode = NO_MATCHES_FOUND;

  if (!Opt->c) {
    if (num_files > 1 && !Opt->h) {
      printf("%s:", file_name);
    }
    if (Opt->n) {
      printf("%d:", num_str);
    }

    if (Opt->o && !Opt->v) {
      errcode = o_handler(Opt, buf_str, pattern);
    } else {
      fputs(buf_str, stdout);
      errcode = OK;
    }

    if (!Opt->o) {
      int n = strlen(buf_str);
      if (buf_str[n] == '\0' && buf_str[n - 1] != '\n') {
        putchar('\n');
      }
    }
  }
  return errcode;
}

int c_handler(Options const *Opt, int num_files, const char *file_name,
              unsigned int num_matching_strings) {
  int errcode = NO_MATCHES_FOUND;

  if ((num_files > 1) && !Opt->h) {
    printf("%s:%u\n", file_name, num_matching_strings);
    errcode = OK;
  } else {
    printf("%u\n", num_matching_strings);
    errcode = OK;
  }

  return errcode;
}

int o_handler(Options const *Opt, char *buf_str, const char *pattern) {
  int errcode = NO_MATCHES_FOUND;
  regex_t preg;
  int regcode = Opt->i ? regcomp(&preg, pattern, REG_ICASE)
                       : regcomp(&preg, pattern, REG_EXTENDED);

  if (OK != regcode) {
    char reg_errbuf[LITTLE_SIZE] = {0};

    regerror(regcode, &preg, reg_errbuf, LITTLE_SIZE);
    fprintf(stderr, "Regexp compilation failed: '%s'\n", reg_errbuf);
    errcode = ERROR;
  }

  if (OK == regcode && !Opt->v) {
    regmatch_t pmatch[SIZE];
    char *s = buf_str;

    for (int i = 0; buf_str[i] != '\0'; ++i) {
      if (0 != regexec(&preg, s, 1, pmatch, 0)) {
        break;
      }
      printf("%.*s\n", (int)(pmatch->rm_eo - pmatch->rm_so),
                       s + pmatch->rm_so);
      s += pmatch->rm_eo;
      errcode = OK;
    }
  }
  regfree(&preg);
  return errcode;
}

int f_handler(char *pattern) {
  int errcode = NO_MATCHES_FOUND;

  FILE *file_pattern_pointer;
  const char *file_name_pattern = optarg;

  if (NULL == (file_pattern_pointer = fopen(file_name_pattern, "r"))) {
    fprintf(stderr, "s21_grep: %s: %s\n", file_name_pattern, strerror(2));
    errcode = ERROR;
  } else {
    char buf_str_pattern[SIZE] = {0};

    while (NULL != fgets(buf_str_pattern, SIZE, file_pattern_pointer)) {
      if ('\n' == *buf_str_pattern) {
        strcpy(pattern, ".*\0");  
      } else {
        buf_str_pattern[strlen(buf_str_pattern) - 1] =
            '\0';  
        init_pattern(pattern, buf_str_pattern);
      }
    }
    fclose(file_pattern_pointer);
  }
  return errcode;
}

void init_pattern(char *pattern, const char *string) {
  if (0 == *string) {
    strcpy(pattern, ".*\0");
  }

  if (0 == pattern[0]) {
    strcpy(pattern, string);

  } else if (strcmp(pattern, ".*") != 0) {
    strcat(pattern, "|");
    strcat(pattern, string);
  }
}

int init_struct(Options *Opt, int symbol, char *pattern) {
  int errcode = OK;

  switch (symbol) {
    case 'v':
      Opt->v = SET;
      break;
    case 'i':
      Opt->i = SET;
      break;
    case 'o':
      Opt->o = SET;
      break;
    case 'l':
      Opt->l = SET;
      break;
    case 'n':
      Opt->n = SET;
      break;
    case 'c':
      Opt->c = SET;
      break;
    case 'e':
      Opt->e = SET;
      init_pattern(pattern, optarg);
      break;
    case 'f':
      Opt->f = SET;
      errcode = f_handler(pattern);
      break;
    case 's':
      Opt->s = SET;
      break;
    case 'h':
      Opt->h = SET;
      break;
    case '?':
      errcode = ERROR;
  }
  return errcode;
}