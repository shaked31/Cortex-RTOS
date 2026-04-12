#include "../include/lib/stdio.h"
#include "../include/drivers/uart.h"

int kernel_main() {
    uart_init();

    mini_printf("\n--- Cortex-RTOS Kernel Starting ---\n");
    

    int test_num = 42;
    unsigned int test_hex = 0xDEADBEEF;

    mini_printf("[TEST] Printing integer: %d\n", test_num);
    mini_printf("[TEST] Printing hex address: %x\n", test_hex);
    mini_printf("[TEST] Printing char: %c and string: %s\n", 'A', "Success");
    
    mini_printf("Entering kernel idle loop...\n");

    while (1);
}