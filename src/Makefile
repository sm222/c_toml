NAME		=	c_toml.a

DEBUG_NAME	=	test.out

CC			=	cc
CFLAGS 		=	-Wall -Werror -Wextra -g

MAKEFLAGS	+=	--no-print-directory

DEBUG		=	-g


SRCS	=	gnl.c \
			toml_main.c \
			toml_utils.c
##
OBJS	=	$(SRCS:.c=.o)

##

all: $(NAME)
	@echo $(NAME) done

$(NAME): $(CC)
	ar -rcs $(NAME) $(OBJS)

$(CC): $(OBJS)
	$(CC) $(DEBUG) $(CFLAG) -c $(SRCS)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME) $(DEBUG_NAME)

re: fclean all