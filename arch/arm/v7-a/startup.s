/* 
 * arch/arm/v7-a/startup.s
 * Entry point for Cortex-RTOS
 */

.section .vector_table, "a"
.globl _Reset

_Reset:
    b reset_handler
    b .                  /* 0x04: Undefined Instruction */
    b .                  /* 0x08: Software Interrupt (SVC) */
    b .                  /* 0x0C: Prefetch Abort */
    b .                  /* 0x10: Data Abort */
    b .                  /* 0x14: Reserved */
    b .                  /* 0x18: IRQ (Interrupt Request) */
    b .                  /* 0x1C: FIQ (Fast Interrupt Request) */


.section .text
reset_handler:
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
    bl main

halt:
    b halt
