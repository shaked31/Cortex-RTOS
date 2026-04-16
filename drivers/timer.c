#include "../include/drivers/timer.h"
#include "../include/lib/stdio.h"

void timer_init(uint32_t interval) {
    sp804_timer_t* timer = (sp804_timer_t*)TIMER0_BASE;

    timer->control = 0; // first, turn of the timer in order to configure it safely

    timer->load = interval; // load the countdown value, the timer will count from this value to 0.

    timer->control = TIMER_CTRL_EN | TIMER_CTRL_PREIODIC | TIMER_CTRL_INT_EN | TIMER_CTRL_32BIT;

    mini_printf("[Timer] SP804 init with interval %d\n", interval);
}