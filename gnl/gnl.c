#include <stdlib.h>
#include <unistd.h>

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1024
#endif

char *ft_strdup(const char *s);
char *ft_strchr(const char *s, int c);
char *ft_strjoin_free(char *s1, const char *s2);
char *ft_substr(const char *s, unsigned int start, size_t len);
char *ft_strcpy(char *dest, const char *src);

char *get_next_line(int fd)
{
    static char buf[BUFFER_SIZE + 1];  // Static buffer retains leftovers between calls
    char *line;
    char *newline;
    ssize_t r;

    // Step 1: Start line with leftovers from static buffer
    line = ft_strdup(buf);
    if (!line)
        return NULL;

    // Step 2: Read until we find a newline or hit EOF
    r = read(fd, buf, BUFFER_SIZE);
    while (!(newline = ft_strchr(line, '\n')) && r > 0) {
        buf[r] = '\0';  // Null-terminate the buffer
        line = ft_strjoin_free(line, buf);  // Append buffer to line
        if (!line)
            return NULL;
        r = read(fd, buf, BUFFER_SIZE);
    }

    // Step 3: Handle read errors
    if (r < 0) {
        free(line);
        return NULL;
    }

    // Step 4: Handle EOF with no content
    if (r == 0 && *line == '\0') {
        free(line);
        return NULL;
    }

    // Step 5: Extract up to newline if found
    if (newline) {
        size_t len = newline - line + 1;             // Length of line including '\n'
        char *result = ft_substr(line, 0, len);       // Extract the valid line
        ft_strcpy(buf, newline + 1);                  // Save the remaining part into static buffer
        free(line);
        return result;
    }

    // Step 6: No newline, but something was read → return it
    buf[0] = '\0';   // No leftovers
    return line;
}
