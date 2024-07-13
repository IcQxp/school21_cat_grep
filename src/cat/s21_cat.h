
#ifndef SRC_CAT_S21_CAT_H_
#define SRC_CAT_S21_CAT_H_ 

#include <stdio.h>
#include <string.h>
#include <errno.h>

typedef struct {
	char b, e, n, s, t, v;
} Flags;

enum option_codes {
	CLEAR = 0,
	SET = 1,
	HELP = 2
};

enum e_stat {
	OK = 0,
	ERROR = 1,
};

int parser(int argc, char** argv, Flags* flag);
int file_handler(char* path, Flags* flag);
int file_exist(FILE* path);
void b_handler(FILE* path_file, const int* c, Flags* flag, unsigned* num_str, const char* flag_position);
void n_handler(FILE* path_file, const int* c, Flags* flag, unsigned* num_str);
void s_opt_handler(FILE* path_file, int* next_char, int* c, Flags* flag, unsigned* num_str, const char* flag_position);
void s_handler(FILE* file_ptr, int* c, Flags* flag, unsigned int* num_str, char* flag_position);
void v_handler(int* c, Flags* flag);
void t_handler(int* c, Flags* flag);
void e_handler(int* c, Flags* flag);



#endif // DEBUG
