#include "../include/drivers/gic.h"

void gic_init() {
    // Enable the CPU interface to receive interrupts
    GICC_CTLR = 1;

    // Set PMR to allow all interrupts (lower value means only higher priority - 0xff allows all)
    GICC_PMR = 0xff;

    // Enable the distributer to forward interrupts to the CPU interfaces
    GICD_CTLR = 1;
}

void gic_enable_interrupt(uint32_t irq_num) {
    volatile uint8_t* target_reg = (volatile uint8_t*)(GIC_DIST_BASE + 0x800 + irq_num);
    *target_reg = 0x01;

    volatile uint8_t* priority_reg = (volatile uint8_t*)(GIC_DIST_BASE + 0x400 + irq_num);
    *priority_reg = 0xA0;

    /* Each bit in the GICD_INT_SET_ENR register enables one IRQ
     * Register 0 handles IRQs 0-31, Register 1 handles 32-63, etc...
    */
    uint32_t reg_offset = (irq_num / 32) * 4;
    uint32_t bit_mask = 1 << (irq_num % 32);

    volatile uint32_t* enable_reg = (volatile uint32_t*)(GICD_INT_SET_ENR + reg_offset);
    *enable_reg = bit_mask;
}