#include "../include/lib/stdio.h"
#include "../include/drivers/timer.h"
#include "../include/drivers/gic.h"

void undefined_handler() {
    mini_printf("[Exception] Undefined instruction! Halting...\n");
    while (1);
}

void data_abort_handler() {
    mini_printf("[Exception] Data abort (memory error)! Halting...\n");
    while (1);
}

void irq_handler() {
    uint32_t irq_id = GICC_IAR;
    mini_printf("[IRQ] ID: %d was triggered\n", irq_id);

    if (irq_id == TIMER0_INT_VALUE) {
        mini_printf("Tick from timer!\n");

        // Clear the timer interrupt
        sp804_timer_t* timer = (sp804_timer_t*)TIMER0_BASE;
        timer->int_clr = 1;
    }

    // Signal for end of interrupt
    GICC_EOIR = irq_id;
}
