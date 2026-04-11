#ifndef UART_H
#define UART_H

#define UART_BASE 0X10009000

#define UARTDR (UART_BASE + 0X00) // Data register
#define UARTFR (UART_BASE + 0X18) // Flag register

#define UARTIBRD (UART_BASE + 0X24) // Baud rate divisor
#define UARTFBRD (UART_BASE + 0X28) // Baud rate divisor
#define UARTLCR_H (UART_BASE + 0X2C) // Line control
#define UARTCR (UART_BASE + 0X30) // Control register

#define UART_CR_ENABLE (1 << 0) // UART enable
#define UART_CR_TXE (1 << 8) // Transmit enable
#define UART_CR_RXE (1 << 9) // Receive enable


#define UART_LCR_H_WLEN_8 (3 << 5) // 8 bit word length
#define UART_LCR_H_FEN (1 << 4) // Enable FIFOs


#define UART_FR_TXFF (1 << 5) // bit 5 is marked when the transmit FIFO is full

#include <stdint.h>

void uart_init();
void uart_putc(char c);
void uart_puts(const char *s);

#endif