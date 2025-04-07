#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1024
#endif

size_t gnl_strlen(char *s);
char *gnl_strdup(char *s);
char *gnl_strchr(char *s, int c);
char *gnl_strjoin_free(char *s1, char *s2);
char *gnl_substr(char *s, int start, int len);
void gnl_strcpy(char *dst, char *src);

size_t ft_strlen(const char *s) {
    size_t i = 0;
    while (s && s[i])
        i++;
    return i;
}

char *ft_strdup(const char *s) {
    size_t len = ft_strlen(s);
    char *dup = malloc(len + 1);
    if (!dup)
        return NULL;
    for (size_t i = 0; i < len; i++)
        dup[i] = s[i];
    dup[len] = '\0';
    return dup;
}

char *ft_strchr(const char *s, int c) {
    if (!s)
        return NULL;
    while (*s) {
        if (*s == (char)c)
            return (char *)s;
        s++;
    }
    return NULL;
}

char *ft_strjoin_free(char *s1, const char *s2) {
    size_t len1 = ft_strlen(s1);
    size_t len2 = ft_strlen(s2);
    char *joined = malloc(len1 + len2 + 1);
    if (!joined)
        return (free(s1), NULL);

    for (size_t i = 0; i < len1; i++)
        joined[i] = s1[i];
    for (size_t j = 0; j < len2; j++)
        joined[len1 + j] = s2[j];
    joined[len1 + len2] = '\0';
    free(s1);
    return joined;
}

char *ft_substr(const char *s, unsigned int start, size_t len) {
    if (!s || ft_strlen(s) < start)
        return ft_strdup("");
    size_t slen = ft_strlen(s + start);
    if (len > slen)
        len = slen;
    char *sub = malloc(len + 1);
    if (!sub)
        return NULL;
    for (size_t i = 0; i < len; i++)
        sub[i] = s[start + i];
    sub[len] = '\0';
    return sub;
}

char *ft_strcpy(char *dest, const char *src) {
    size_t i = 0;
    while (src[i]) {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
    return dest;
}

char *get_next_line(int fd) {
    static char buf[BUFFER_SIZE + 1];
    char *line;
    char *newline;
    ssize_t r;

    // Step 1: Start line with leftovers from previous call
    line = ft_strdup(buf);
    if (!line)
        return NULL;

    // Step 2: Keep reading until newline or EOF
    r = read(fd, buf, BUFFER_SIZE);
    while (!(newline = ft_strchr(line, '\n')) && r > 0) {
        buf[r] = '\0';
        line = ft_strjoin_free(line, buf);
        if (!line)
            return NULL;
        r = read(fd, buf, BUFFER_SIZE);
    }

    // Step 3: Handle read error
    if (r < 0) {
        free(line);
        return NULL;
    }

    // Step 4: EOF + nothing in line
    if (r == 0 && *line == '\0') {
        free(line);
        return NULL;
    }

    // Step 5: Extract up to newline
    if (newline) {
        size_t len = newline - line + 1;
        char *result = ft_substr(line, 0, len);
        ft_strcpy(buf, newline + 1); // Save leftovers
        free(line);
        return result;
    }

    // Step 6: No newline, but return what's there
    buf[0] = '\0';
    return line;
}

int main(int argc, char **argv) {
    if (argc != 2)
        return 1;

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0)
        return 1;

    char *line;
    while ((line = get_next_line(fd))) {
        printf("%s", line);
        free(line);
    }

    close(fd);
    return 0;
}