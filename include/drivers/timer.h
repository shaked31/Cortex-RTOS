#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

#define TIMER0_BASE 0x10011000  // SP804 timer0 base address on RealView PB A8

typedef struct {
    volatile uint32_t load;     // 0x00: Load register
    volatile uint32_t value;    // 0x04: Current value register
    volatile uint32_t control;  // 0x08: Control register
    volatile uint32_t int_clr;  // 0x0C: Interrupt clear register
    volatile uint32_t raw_int;  // 0x10: Raw interrupt status
    volatile uint32_t mask_int; // 0x14: Masked interrupt status
} sp804_timer_t;

// Control register bits
#define TIMER_CTRL_EN        (1 << 7) // Start the timer
#define TIMER_CTRL_PREIODIC  (1 << 6) // auto reload after reaching 0
#define TIMER_CTRL_INT_EN    (1 << 5) // trigger irq when the counter reaches to 0
#define TIMER_CTRL_32BIT     (1 << 1) // use 32-bit counter

void timer_init(uint32_t interval);

#endif