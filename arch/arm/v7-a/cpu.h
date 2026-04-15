#ifndef CPU_H
#define CPU_H

static inline void enable_interrupts() {
    __asm__ volatile("cpsie i" : : : "memory");
}

static inline void disable_interrupts() {
    __asm__ volatile("cpsid i" : : : "memory");
}

#endif