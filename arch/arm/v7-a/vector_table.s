/* 
 * arch/arm/v7-a/vector_table.s
 * Vector table handler in asm
 */

.section .vectors, "ax"
.globl vector_table

vector_table:
    ldr pc, _reset_addr
    ldr pc, _undef_addr     /* 0x04: Undefined Instruction */
    ldr pc, _svc_addr       /* 0x08: Software Interrupt (SVC) */
    ldr pc, _prefetch_addr  /* 0x0C: Prefetch Abort */
    ldr pc, _data_addr      /* 0x10: Data Abort */
    nop                     /* 0x14: Reserved */
    ldr pc, _irq_addr       /* 0x18: IRQ (Interrupt Request) */
    ldr pc, _fiq_addr       /* 0x1C: FIQ (Fast Interrupt Request) */
                

_reset_addr:    .word _Reset
_undef_addr:    .word undefined_handler
_svc_addr:      .word svc_handler
_prefetch_addr: .word prefetch_handler
_data_addr:     .word data_abort_handler
_irq_addr:      .word irq_wrapper
_fiq_addr:      .word fiq_handler

irq_wrapper:
    sub lr, lr, #4

    srsdb sp!, #19 // save PC & CPSR directly into the supervisor mode stack
    cps #19 // switch CPU to supervisor mode

    push {r0-r12, lr}

    // pass sp as the first arg to scheduler and call the function
    mov r0, sp
    bl scheduler

    mov sp, r0 // load the new sp after function
    pop {r0-r12, lr} // restore the new task's general purpose registers

    rfeia sp! // pops the PC and CPSR from the stack and jumps to the new task


svc_handler:      b .
prefetch_handler: b .
fiq_handler:      b .