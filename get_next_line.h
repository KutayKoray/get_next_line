#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 10
#endif

#include <stddef.h>

size_t			ft_strlen(char *str);
char			*ft_strchr(char *s, int c);
char			*ft_strjoin(char *s1, char *s2);
char			*get_next_line(int fd);
void    *ft_calloc(size_t count, size_t size);

#endif

