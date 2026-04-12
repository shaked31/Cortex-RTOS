#include "../include/lib/stdio.h"
#include "../include/drivers/uart.h"
#include "../include/drivers/gic.h"
#include "../include/drivers/timer.h"
#include "../arch/arm/v7-a/cpu.h"

int kernel_main() {
    uart_init();
    mini_printf("\n--- Cortex-RTOS Kernel Starting ---\n\n");

    gic_init();
    timer_init(1000000);
    gic_enable_interrupt(36);

    enable_interrupts();
    
    mini_printf("Interrupts are enabled, waiting for ticks from timer...\n");

    while (1) {
        // sp804_timer_t* timer = (sp804_timer_t*)TIMER0_BASE;
        // if (timer->value != last_val) {
        //     mini_printf("V: %x ", timer->value);
        //     last_val = timer->value;
        // }
    }
}