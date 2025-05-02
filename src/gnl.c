#include "gnl.h"

void*  s_free(void* ptr) {
  if (ptr)
    free(ptr);
  return NULL;
}


void  s_bzero(void *p, size_t size) {
  if (!p)
    return ;
  while (size--)
    ((char *)p)[size] = 0;
}

static size_t  s_strlen(const char* s) {
  size_t  i = 0;

  while (s && s[i])
    i++;
  return i;
}

char* ft_strjoin(char *sfree, char *s2) {
  size_t  s1_i;
  size_t  s2_i;
  char  *new =  NULL;

  s1_i = s_strlen(sfree);
  s2_i = s_strlen(s2);
  new = calloc(s1_i + s2_i + 1, sizeof(char));
  if (!new) {
    sfree = s_free(sfree);
    return NULL;
  }
  while (s1_i + s2_i-- > s1_i)
    new[s1_i + s2_i] = s2[s2_i];
  while (s1_i--)
    new[s1_i] = sfree[s1_i];
  if (*new == 0)
    new = s_free(new);
  s_free(sfree);
  return new;
}

char*  ft_tiny_split(char *s, size_t *cut) {
  char*  new = NULL;
  size_t  i = 0;

    while (s[i])
    if (s[i++] == '\n')
      break ;
  new = calloc(i + 1, sizeof(char));
  if (!new) {
    return (NULL);
  }
  *cut = i;
  while (i--)
    new[i] = s[i];
  return new;
}

char  ft_find(const char* s) {
  size_t  i = 0;

  while (s && s[i]) {
    if (s[i] == '\n')
      return '\n';
    i++;
  }
  return '0';
}

char*  safe_return(char **book, t_info *t_val) {
  t_val->tmp = ft_tiny_split(*book, &t_val->cut);
  if (!t_val->tmp) {
    *book =  s_free(*book);
    return NULL;
  }
  t_val->tmp2 = *book;
  *book = ft_strjoin(NULL, *book + t_val->cut);
  t_val->tmp2 =  s_free(t_val->tmp2);
  return t_val->tmp;
}

char* get_next_line(int fd) {
  static char*  book = NULL;
  t_info        t_val;

  if (fd < 0 || BUFFER_SIZE <= 0)
    return (book =  s_free(book));
  if (!book)
    book = calloc(1, sizeof(char));
  if (!book)
    return NULL;
  t_val.rv = 0;
  while (ft_find(book) == '0') {
    s_bzero(t_val.readtmp, BUFFER_SIZE + 1);
    t_val.rv = read(fd, t_val.readtmp, BUFFER_SIZE);
    if (t_val.rv <= 0)
      break ;
    book = ft_strjoin(book, t_val.readtmp);
    if (!book)
      return NULL;
  }
  if (t_val.rv == -1 || (t_val.rv <= 0 && *book == 0)) {
    book =  s_free(book);
    return NULL;
  }
  return (safe_return(&book, &t_val));
}

static void  ft_allfree(char **p) {
  int  i = 0;

  while (p[i])
    free(p[i++]);
  free(p);
}

static int  nb_word(const char *str, char c) {
  int  i = 0;
  int  size = 0;

  while (str[i]) {
    while (str[i] && str[i] == c)
      i++;
    if (str[i])
      size++;
    while (str[i] && str[i] != c)
      i++;
  }
  return size;
}

char*  ft_strdup_c(const char *str, char c) {
  size_t  i = 0;
  char*   new = NULL;

  while (str[i] && str[i] != c)
    i++;
  new = calloc(i + 1, sizeof(char));
  if (!new)
    return NULL;
  while (i--)
    new[i] = str[i];
  return new;
}


char**  ft_split(char const* s, char c) {
  char** new = NULL;
  size_t  i = 0;
  size_t  index = 0;
  int nbWords = 0;

  if (!s)
    return (NULL);
  nbWords = nb_word(s, c);
  new = calloc(nbWords + 1, sizeof(char *));
  if (!new)
    return (NULL);
  while (nbWords--) {
    while (s[i] && s[i] == c)
      i++;
    new[index] = ft_strdup_c(s + i, c);
    if (!new[index++]) {
      ft_allfree(new);
      return NULL;
    }
    while (s[i] && s[i] != c)
      i++;
  }
  return new;
}