/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoray <kkoray@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 11:48:00 by kkoray            #+#    #+#             */
/*   Updated: 2024/11/03 13:42:47 by kkoray           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"
#include <unistd.h>
#include <stdlib.h>

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

char	*get_next_line(int fd)
{
	static char	*buffer[MAX_FD];
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
	{
		free(buffer[fd]);
		buffer[fd] = NULL;
		return (NULL);
	}
	if (!buffer[fd])
		buffer[fd] = ft_calloc(1, 1);
	if (!buffer[fd])
		return (NULL);
	buffer[fd] = read_file(fd, buffer[fd]);
	if (!buffer[fd])
		return (NULL);
	line = get_line(buffer[fd]);
	if (line == NULL)
	{
		free(buffer[fd]);
		buffer[fd] = NULL;
	}
	return (line);
}

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#define TEST_FD_COUNT 1050  // Test için açılacak dosya sayısını MAX_FD'nin üzerine çıkaralım

int main()
{
    int fds[TEST_FD_COUNT];
    char *line;
    int i;

    // Dosyaları aç veya oluştur ve dosya sınırını aşmaya çalış
    for (i = 0; i < TEST_FD_COUNT; i++)
    {
        char filename[20];
        sprintf(filename, "test_file_%d.txt", i);

        // Dosya yoksa oluştur, varsa sadece okuma modunda aç
        fds[i] = open(filename, O_RDWR | O_CREAT, 0644);
		write(fds[i], "Hello, World!\n", 15);
        if (fds[i] == -1)
        {
            perror("Dosya açma hatası: FD sınırına ulaşıldı mı?");
            break;
        }
    }

    // Açılabilen dosyalar için get_next_line çağır
    for (int j = 0; j < i; j++)
    {
        printf("Dosya %d'den satır okuma:\n", j);
        line = get_next_line(fds[j]);
        while (line)
        {
            printf("%s", line);
            free(line);
            line = get_next_line(fds[j]);
        }
        printf("\n");
    }

    // Açık dosyaları kapat
    for (int j = 0; j < i; j++)
        close(fds[j]);

    return 0;
}

