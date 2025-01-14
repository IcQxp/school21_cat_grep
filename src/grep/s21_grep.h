
#ifndef SRC_GREP_S21_GREP_H_
#define SRC_GREP_S21_GREP_H_

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 1024
#define LITTLE_SIZE 128

typedef struct {
  char v, i, o, l, n, c, e, f, s, h;
} Options;

enum error_codes {
  OK = 0,
  NO_MATCHES_FOUND = 1,
  ERROR = 2,
};

enum option_codes {
  CLEAR = 0,
  SET = 1,
  HELP = 2,
};

void init_pattern(char *pattern, const char *src);
int init_struct(Options *Opt, int symbol, char *pattern);
int executor(const char **argv, const char *pattern, Options const *Opt);
int file_handler(const char **argv, const char *pattern, int num_files,
                 int flag_no_pattern_opt, Options const *Opt);
int file_counter(const char **argv, int flag_no_pattern_opt);
int opt_handler(const char *file_name, int num_files, int num_str,
                char *buf_str, const char *pattern, Options const *Opt);
int f_handler(char *pattern);
int o_handler(Options const *Opt, char *buffer, const char *pattern);
int c_handler(Options const *Opt, int num_files, const char *file_name,
              unsigned int num_matching_strings);

#endif  // SRC_GREP_S21_GREP_H_