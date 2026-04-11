#include "../include/drivers/uart.h"

void uart_init() {
    *(volatile uint32_t *)UARTCR = 0x00; // Disable UART before config

    /* Set baud rate for 115200 at 24MHz clock
     * Calc: 24,000,000 / (16 * 115200) = 13.020833
     * Fractional part = 0.020833 * 64 + 0.5 ≈ 1
    */
    *(volatile uint32_t *)UARTIBRD = 13;
    *(volatile uint32_t *)UARTFBRD = 1;

    /* Config line control
     * word length - 8 bits
     * no parity (error checking mechanism)
     * 1 stop bit (after every letter (8 bits), the receiver - terminal will preaper to the next letter)
     * FIFOs enabled
    */
    *(volatile uint32_t *)UARTLCR_H = UART_LCR_H_FEN | UART_LCR_H_WLEN_8;

    // Enable UART, transmit and receive
    *(volatile uint32_t *)UARTCR = UART_CR_ENABLE | UART_CR_RXE | UART_CR_TXE;
}

void uart_putc(char c) {
    while (*(volatile uint32_t *)UARTFR & UART_FR_TXFF);

    *(volatile uint32_t *)UARTDR = c;
}

void uart_puts(const char *s) {
    while (*s) {
        uart_putc(*s++);
    }
}