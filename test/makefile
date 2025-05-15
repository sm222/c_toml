NAME		=		a.out

CC		=			cc
LIB			=		../src/*.c

CFLAGS		=		-g -Wall -Werror -Wextra
MAKEFLAGS	+=	--no-print-directory

SRC				=		src/main.c


all: build test

build:
	@make -C ../src

test:
	$(CC) $(CFLAGS) $(LIB) $(SRC)


clean:

fclean: clean
	@rm -fv $(NAME)