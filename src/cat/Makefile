NAME = s21_cat
CC = gcc
CFLAGS = -std=c11 -Wall -Werror -Wextra -finput-charset=UTF-8
SRC = $(NAME).c
HEADER = $(NAME).h

all: s21_cat

s21_cat: $(SRC) $(HEADER)
		$(CC) $(CFLAGS) $(SRC) -o $(NAME)
clean:
		rm -rf $(NAME) *.log sample.txt logs/
re: clean all