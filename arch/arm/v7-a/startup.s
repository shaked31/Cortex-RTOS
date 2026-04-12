/* 
 * arch/arm/v7-a/startup.s
 * Entry point for Cortex-RTOS
 */

.section .text
.globl _Reset

_Reset:
    b reset_handler


reset_handler:
    /* In ARMv7-A, for each mode, the cpu stores private value of registers */
    cps #27 // change cpu mode to undefined instruction
    ldr sp, =_stack_top // init the "undefined" private sp register

    cps #19 // change cpu mode to supervisor
    ldr sp, =_stack_top // init the "supervisor" private sp register

    /* Set the VBAR (Vector Base Address Register) 
     * The VBAR holds the base address of the exception vector table
     * This allows to place the table at 0x10000 instead of 0x00 (qemu default)
     */
    ldr r0, =vector_table
    mcr p15, 0, r0, c12, c0, 0
    
    ldr sp, =_stack_top
    
    ldr r0, =_bss_start
    ldr r1, =_bss_end
    mov r2, #0
bss_clear_loop:
    cmp r0, r1
    bge bss_clear_done  // branch if r0 >= r1 (finished clearing the bss)
    str r2, [r0], #4    // store zero and post-increment r0 by 4 bytes
    blt bss_clear_loop  // jumps back to start of loop

bss_clear_done:
    bl kernel_main

halt:
    b halt
