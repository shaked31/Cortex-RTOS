#include "../include/drivers/uart.h"

int main() {
    uart_init();
    uart_puts("hello world");
}