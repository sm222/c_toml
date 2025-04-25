NAME		=	c_toml.a

DEBUG_NAME	=	test.out

CC			=	cc
CFLAG 		=	-Wall -Werror -Wextra -g

MAKEFLAGS	+=	--no-print-directory

DEBUG		=	-g


SRCS	=	gnl.c \
			toml_main.c
##
OBJS	=	$(SRCS:.c=.o)

##

all: $(NAME)
	@echo $(NAME) done

$(NAME): $(OBJS)
	ar -rcs $(NAME) $(OBJS)

$(OBJS): $(SRCS)
	$(CC) $(DEBUG) $(CFLAGS) -c $(SRCS)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME) $(DEBUG_NAME)

re: fclean all