#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <stdio.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 150
# endif


typedef struct s_info
{
	char	*tmp;
	char	*tmp2;
	char	readtmp[BUFFER_SIZE + 1];
	int		rv;
	size_t	cut;
}	t_info;

char    *get_next_line(int fd);
char    *ft_strjoin(char *s1, char *s2);
char   **ft_split(char const* s, char c);

#endif // GET_NEXT_LINE_H