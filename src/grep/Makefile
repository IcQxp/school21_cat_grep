NAME		= s21_grep
CC			= gcc
CFLAGS		= -std=c11 -pedantic -Wall -Werror -Wextra
SRC			= $(NAME).c
HEADER		= $(NAME).h
TEST_FILE	= functional_test_s21_grep.sh

all: s21_grep

s21_grep: $(SRC) $(HEADER)
		$(CC) $(CFLAGS) $(SRC) -o $(NAME)
test: $(NAME)
		bash $(TEST_FILE)
clean:
		rm -rf $(NAME) *.log pattern.file empty.file lyrics.txt lyrics2.txt logs/
re:
		$(MAKE) clean
		$(MAKE) all