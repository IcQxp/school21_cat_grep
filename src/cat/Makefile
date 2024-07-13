NAME = s21_cat
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -pedantic 
SRC = $(NAME).c
HEADER = $(NAME).h

all: s21_cat

s21_cat: $(SRC) $(HEADER)
		$(CC) $(CFLAGS) $(SRC) -o $(NAME)
clean:
		rm -rf $(NAME) *.log  logs/
re: clean all