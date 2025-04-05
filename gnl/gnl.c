#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1024
#endif

// Helper: String length
size_t str_len(char *s) {
    size_t len = 0;
    while (s && s[len])
        len++;
    return len;
}

// Helper: Copy string from src to dest
char *str_copy(char *dest, char *src) {
    char *start = dest;
    while (*src)
        *dest++ = *src++;
    *dest = '\0';
    return start;
}

// Helper: Find first occurrence of character c in string
char *str_chr(char *s, int c) {
    while (*s && *s != c)
        s++;
    return (*s == c) ? s : NULL;
}

// Helper: Duplicate a string
char *str_dup(char *s) {
    size_t len = str_len(s);
    char *dup = malloc(len + 1);
    return dup ? str_copy(dup, s) : NULL;
}

// Helper: Join two strings, freeing the first one
char *str_join_and_free(char *s1, char *s2) {
    size_t len1 = str_len(s1), len2 = str_len(s2);
    char *result = malloc(len1 + len2 + 1);
    if (!result) return NULL;
    str_copy(result, s1);
    str_copy(result + len1, s2);
    free(s1);
    return result;
}

// Main line-reading function
char *get_next_line(int fd) {
    static char buffer[BUFFER_SIZE + 1];
    char *line = str_dup(buffer);  // Start with what’s left in buffer
    if (!line) return NULL;

    ssize_t bytes_read;
    char *newline_pos;

    // Read until we find a newline or reach EOF
    while (!(newline_pos = str_chr(line, '\n')) &&
           (bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0) {
        buffer[bytes_read] = '\0';
        line = str_join_and_free(line, buffer);
    }

    // If read fails or no content, clean up
    if (bytes_read < 0 || (!*line && bytes_read == 0)) {
        free(line);
        return NULL;
    }

    // If we found a newline, cut it from buffer for next call
    if (newline_pos) {
        str_copy(buffer, newline_pos + 1);
        line[newline_pos - line + 1] = '\0'; // keep '\n'
    } else {
        buffer[0] = '\0'; // clear buffer if EOF
    }

    return line;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        return 1;
    }

    int file = open(argv[1], O_RDONLY);
    if (file < 0) {
        perror("open");
        return 1;
    }

    char *line;
    while ((line = get_next_line(file))) {
        printf("%s", line);  // line already contains newline if present
        free(line);
    }

    close(file);
    return 0;
}