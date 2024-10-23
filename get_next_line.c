#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include "get_next_line.h"

static char *read_file(int fd, char *buffer);
static char *get_line(char *buffer);
static char *update_buffer(char *buffer, int i);

char    *get_next_line(int fd)
{
    static char *buffer;
    char *line;

    if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
        return (NULL);
    buffer = read_file(fd, buffer);
    if(!buffer)
        return (NULL);
    line = get_line(buffer);
    return (line);
}

static char *get_line(char *buffer)
{
    int i;
    char *line;

    if (!buffer)
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
    printf("update sonrası buffer:%s\n", buffer);
    line[i] = '\n';
    return (line);
}

static char *update_buffer(char *buffer, int i)
{
    int j;
    i += 1;
    j = 0;
    if(!buffer)
        return (NULL);
    printf("buffer ilk indeksi:%c\nbuffer son indeksi:%c\n", buffer[0], buffer[i]);
    printf("update girince buffer:%s\ni'nin degeri:%d\n", buffer, i);
    while(buffer[i] != '\0')
    {
        buffer[j] = buffer[i];
        i++;
        j++;
    }
    while(buffer[j])
    {
        buffer[j] = 0;
        j++;
    }
    printf("return öncesi buffer:%s\n", buffer);
    return (buffer);
}

static char *read_file(int fd, char *buffer)
{
    int readed_bytes;
    char *tmp_str;

    if(!buffer)
            buffer = ft_calloc(1, 1);
    tmp_str = ft_calloc(BUFFER_SIZE + 1, sizeof(char));
    if(!tmp_str)
        return (NULL);
    readed_bytes = 1;
    while (readed_bytes > 0)
    {
        readed_bytes = read(fd, tmp_str, BUFFER_SIZE);
        printf("readed_bytes:%d\n", readed_bytes);
        if (readed_bytes == -1)
            free(tmp_str);
        if (readed_bytes == -1)
            return (NULL);
        tmp_str[readed_bytes] = 0;
        printf("tmp_str:%s\n", tmp_str);
        printf("tmp_str son indeks:%s\n", tmp_str[BUFFER_SIZE]);
        printf("buffer:%s\n", buffer);
        buffer = ft_strjoin(buffer, tmp_str);
        printf("buffer str_join sonrası:%s\n", buffer);
        if (ft_strchr(buffer, '\n'))
            break;
    }
    free(tmp_str);
    return (buffer);
}



int main ()
{
    int fd = open("test.txt", O_CREAT | O_RDWR, 0777);
    char *str;

    write(fd, "Merhaba ben\nKutay\nKoray", 24);

    close(fd);
    open("test.txt", O_RDWR | O_CREAT, 0777);

    int i = 0;
    while(i < 3)
    {    
        printf("get_next_line():%s\n", get_next_line(fd));
        i++;
    }
}
