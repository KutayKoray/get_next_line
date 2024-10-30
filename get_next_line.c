/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoray <kkoray@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 17:48:23 by kkoray            #+#    #+#             */
/*   Updated: 2024/10/30 20:53:12 by kkoray           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static char	*read_file(int fd, char *buffer);
static char	*get_line(char *buffer);
static char	*update_buffer(char *buffer, int i);
void		*ft_memset(void *b, int c, size_t len);

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*ptr;

	ptr = (unsigned char *)b;
	while (len > 0)
	{
		*(ptr++) = (unsigned char)c;
		len--;
	}
	return (b);
}

char	*get_next_line(int fd)
{
	static char	*buffer;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
		return (NULL);
	buffer = read_file(fd, buffer);
	if (!buffer)
		return (NULL);
	line = get_line(buffer);
	if (line == NULL)
	{
		free(buffer);
		buffer = NULL;
	}
	return (line);
}

static char	*get_line(char *buffer)
{
	int		i;
	char	*line;

	i = 0;
	if (*buffer == 0)
		return (NULL);
	while (buffer[i] && buffer[i] != '\n')
		i++;
	line = ft_calloc(i + 2, sizeof(char));
	i = 0;
	while (buffer[i] && buffer[i] != '\n')
	{
		line[i] = buffer[i];
		i++;
	}
	buffer = update_buffer(buffer, i);
	if (check_null(line) == 1)
		line[i] = '\0';
	else if (check_null(line) == -1)
		return (NULL);
	else
		line[i] = '\n';
	return (line);
}

static char	*update_buffer(char *buffer, int i)
{
	int	j;

	i += 1;
	j = 0;
	if (!buffer)
		return (NULL);
	while (buffer[i] != '\0')
	{
		buffer[j] = buffer[i];
		i++;
		j++;
	}
	while (buffer[j])
	{
		buffer[j] = 0;
		j++;
	}
	return (buffer);
}

static char	*read_file(int fd, char *buffer)
{
	int		readed_bytes;
	char	*tmp_str;
	char	*tmp_buf;

	if (!buffer)
		buffer = ft_calloc(1, 1);
	tmp_str = ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	if (!tmp_str)
		return (free(tmp_str), free(buffer), NULL);
	readed_bytes = 1;
	while (readed_bytes > 0)
	{
		readed_bytes = read(fd, tmp_str, BUFFER_SIZE);
		if (readed_bytes == -1)
			return (free(tmp_str), NULL);
		if (readed_bytes == 0)
			return (free(tmp_str), buffer);
		tmp_buf = ft_strjoin(buffer, tmp_str);
		free(buffer);
		ft_memset(tmp_str, 0, BUFFER_SIZE + 1);
		buffer = tmp_buf;
		if (ft_strchr(buffer, '\n'))
			break ;
	}
	return (free(tmp_str), buffer);
}

int	main(void)
{
	int		fd;
	char	*str;
	int		i;

	fd = open("./texts/variable_nls.txt", O_RDWR | O_CREAT, 0777);
	i = 0;
	while (i < 13)
	{
		str = get_next_line(fd);
		printf("get_next_line():%s", str);
		i++;
		free(str);
	}
}
