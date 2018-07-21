#include <cstdarg>
#include <iostream>

void simple_printf(const char*fmt, ...);

int main()
{
    simple_printf("dcff", 3, 'a', 1.99, 42.5);
    return 0;
}

void simple_printf(const char*fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    while (*fmt != '\0') {
        if (*fmt == 'd') {
            int i = va_arg(args, int);
            std::cout << i << std::endl;
        } else if (*fmt == 'c') {
            int c = va_arg(args, int);
            std::cout << static_cast<char>(c) << std::endl;
        } else if (*fmt == 'f') {
            double d = va_arg(args, double);
            std::cout << d << std::endl;
        }
        ++fmt;
    }
    va_end(args);
}
