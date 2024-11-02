/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoray <kkoray@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 17:48:23 by kkoray            #+#    #+#             */
/*   Updated: 2024/11/02 16:27:04 by kkoray           ###   ########.fr       */
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

char	*get_next_line(int fd)
{
	static char	*buffer;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
	{
		free(buffer);
		buffer = NULL;
		return (NULL);
	}
	if (!buffer)
		buffer = ft_calloc(1, 1);
	if (!buffer)
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
	if (buffer[i] == '\n')
		i++;
	line = ft_calloc(i + 1, sizeof(char));
	if (!line)
		return (NULL);
	i = 0;
	while (buffer[i] && buffer[i] != '\n')
	{
		line[i] = buffer[i];
		i++;
	}
	if (buffer[i] == '\0')
		line[i] = '\0';
	else if (buffer[i] == '\n')
		line[i] = '\n';
	buffer = update_buffer(buffer, i);
	return (line);
}

static char	*update_buffer(char *buffer, int i)
{
	int	j;

	j = 0;
	if (!buffer)
		return (NULL);
	while (buffer[i] && buffer[i + 1] != '\0')
	{
		buffer[j] = buffer[i + 1];
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
		if (!tmp_buf)
			return (free(tmp_str), free(buffer), NULL);
		free(buffer);
		ft_memset(tmp_str, 0, BUFFER_SIZE + 1);
		buffer = tmp_buf;
		if (ft_strchr(buffer, '\n'))
			break ;
	}
	return (free(tmp_str), buffer);
}

// int	main(void)
// {
// 	int		fd;
// 	char	*str;
// 	int		i;

// 	fd = open("./texts/giant_line_nl.txt", O_RDWR | O_CREAT, 0777);
// 	i = 0;
// 	while (i < 1)
// 	{
// 		str = get_next_line(fd);
// 		printf("get_next_line():%s", str);
// 		i++;
// 		free(str);
// 	}
// }
