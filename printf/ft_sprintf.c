#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
void ft_putchar(char *str, int *len)
{
    if (!str) str = "(null)";
    while (*str)
        *len += write(1, str++, 1);
}
void ft_putnum(long long num, int base, int *len)
{
    char *hex = "0123456789abcdef";
    if (num < 0) {*len += write(1, "-", 1); num = -num;}
    if (num >= base) ft_putnum(num / base, base, len);
    *len += write(1, &hex[num % base], 1);
}
void print_loop(const char *f,  va_list ap, int *len)
{
    while (*f)
    {
        if (*f == '%' && (*(f + 1) == 's' || *(f + 1) == 'd' ||  *(f + 1) == 'x'))
        {
            f++;
            if (*f == 's') ft_putchar(va_arg(ap, char *), len);
            else if (*f == 'd') ft_putnum(va_arg(ap, int), 10, len);
            else if (*f == 'x') ft_putnum(va_arg(ap, unsigned int), 16, len);
        }
        else
            *len += write(1, f, 1);
        f++;
    }
}
int ft_printf(const char *f, ... )
{
    va_list  ap;
    int len = 0;
    va_start(ap, f);
    print_loop(f, ap, &len);
    va_end(ap);
    return (len);
}
int main(void)
{
    int l = ft_printf("Hey.\n");
    ft_printf("Magic %s is %d\n", "number", l);
    l = ft_printf("Hexadecimal for %d is %x\n", 42, 42);
    ft_printf("%d\n", l);
    l = printf("Hey.\n");
    printf("Magic %s is %d\n", "number", l);
    l = printf("Hexadecimal for %d is %x\n", 42, 42);
    printf("%d\n", l);
    return 0;
}