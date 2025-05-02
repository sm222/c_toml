TEST		=	test.out

CC			=	cc
CFLAG 		=	-Wall -Werror -Wextra
LIB			=	src/c_toml.a

DEBUG		=	-g

MAKEFLAGS	+=	--no-print-directory

SRCS		=	main.c
##
OBJS		=	$(SRCS:.c=.o)

##

all: build

build:
	@make -C src

run: all $(TEST)
	./$(TEST) test/test.toml

$(TEST): $(OBJS)
	$(CC) $(CFLAG) $(DEBUG) $(OBJS) $(LIB) -o $(TEST)

$(OBJS): $(SRCS)
	$(CC) $(CFLAG) $(DEBUG) -c $(SRCS)

clean:
	$(RM) $(OBJS)
	@make -C src clean

fclean: clean
	$(RM) $(NAME) $(TEST)
	@make -C src fclean

re: fclean all

rerun: re run
