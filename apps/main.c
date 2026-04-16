#include "../include/lib/stdio.h"
#include "../include/drivers/uart.h"
#include "../include/drivers/gic.h"
#include "../include/drivers/timer.h"
#include "../include/kernel/task.h"
#include "../include/kernel/sync.h"
#include "../arch/arm/v7-a/cpu.h"

mutex_t uart_mutex;

void delay() {
    for (volatile int i = 0 ; i < 100000000 ; i++) {}
}

void task1() {
    while (1) {
        mutex_lock(&uart_mutex);
        mini_printf("Task A is being exectuted right now!\n");
        mutex_unlock(&uart_mutex);
        delay();
    }
}

void task2() {
    while (1) {
        mutex_lock(&uart_mutex);
        mini_printf("Task B is being exectuted right now!\n");
        mutex_unlock(&uart_mutex);
        delay();
    }
}

int kernel_main() {
    uart_init();
    mini_printf("\n--- Cortex-RTOS Kernel Starting ---\n\n");

    gic_init();
    task_init();

    mutex_init(&uart_mutex);

    task_create(task1);
    task_create(task2);

    timer_init(1000000);
    gic_enable_interrupt(36);

    enable_interrupts();
    
    mini_printf("Interrupts are enabled, waiting for ticks from timer...\n");

    while (1);
    
}