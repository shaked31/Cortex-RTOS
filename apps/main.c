#include "../include/lib/stdio.h"
#include "../include/drivers/uart.h"
#include "../include/drivers/gic.h"
#include "../include/drivers/timer.h"
#include "../include/kernel/task.h"
#include "../arch/arm/v7-a/cpu.h"

void delay() {
    for (volatile int i = 0 ; i < 100000000 ; i++) {}
}

void task1() {
    while (1) {
        mini_printf("A");
        delay();
    }
}

void task2() {
    while (1) {
        mini_printf("B");
        delay();
    }
}

int kernel_main() {
    uart_init();
    mini_printf("\n--- Cortex-RTOS Kernel Starting ---\n\n");

    gic_init();
    task_init();

    task_create(task1);
    task_create(task2);

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