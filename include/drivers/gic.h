#ifndef GIC_H
#define GIC_H

#include <stdint.h>

/* GIC base addresses for RealView PB A8
 * The distributer decides which interrupts are active and to which cpu they are sent
 * The CPU interface in the GIC is the device that is close to each core and responsible for the communication with the cpsr
*/
#define GIC_DIST_BASE   0X1E001000
#define GIC_CPU_BASE    0x1E000000

#define GICD_CTLR              (*(volatile uint32_t *)(GIC_DIST_BASE + 0x000)) // Distributer Control Register: the main switch of the distributer
#define GICD_INT_SET_ENR       (GIC_DIST_BASE + 0x100) // Interrupt Set-Enable Registers

#define GICC_CTLR       (*(volatile uint32_t *)(GIC_CPU_BASE + 0x000)) // CPU Interface Control Register: the main switch of the core
#define GICC_PMR        (*(volatile uint32_t *)(GIC_CPU_BASE + 0x004)) // Priority Mask Register: filtering priorities
#define GICC_IAR        (*(volatile uint32_t *)(GIC_CPU_BASE + 0x00c)) // Interrupt Acknowledge Register: contains value of interrupt
#define GICC_EOIR       (*(volatile uint32_t *)(GIC_CPU_BASE + 0x010)) // End Of Interrupt Register: when finishing handling the interrupt, must write the register the value of interrupt

void gic_init();
void gic_enable_interrupt(uint32_t irq_num);
#endif