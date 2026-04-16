#include "../include/lib/stdio.h"
#include "../include/drivers/uart.h"

#define MAX_INT_BIT_SIZE 32
#define MAX_HEX_BIT_SIZE 32

static void itoa(unsigned int value, char *str, int base) {
    char *ptr = str, *low;

    if (value == 0) {
        *ptr++ = '0';
        *ptr = '\0';
        return;
    }
    
    low = ptr;

    while (value > 0) {
        int res = value % base;
        if (res < 10)
            *ptr++ = res + '0';
        else
            *ptr++ = res - 10 + 'a';
        value /= base;
    }

    *ptr = '\0';
    ptr--;
    // Reverse the string
    while (low < ptr) {
        char temp = *low;
        *low++ = *ptr;
        *ptr-- = temp;
    }
}

void mini_printf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    for (const char* p = fmt ; *p != '\0' ; p++) {
        if (*p != '%') {
            uart_putc(*p);
            continue;
        }
        p++; // skip '%'

        switch (*p)
        {
            case 'c': {
                uart_putc((char)va_arg(args, int));
                break;
            }
            case 's': {
                uart_puts(va_arg(args, char*));
                break;
            }
            case 'd': {
                char buf[MAX_INT_BIT_SIZE];
                itoa((unsigned int)va_arg(args, int), buf, 10);
                uart_puts(buf);
                break;
            }
            case 'x': {
                char buf[MAX_HEX_BIT_SIZE];
                itoa((unsigned int)va_arg(args, unsigned int), buf, 16);
                uart_puts("0x");
                uart_puts(buf);
                break;
            }
            default: {
                uart_putc('%');
                uart_putc(*p);
                break;
            }
        }
    }
    va_end(args);
}