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
    push {r0-r3, r12, lr}
    bl irq_handler
    pop {r0-r3, r12, lr}
    movs pc, lr

svc_handler:      b .
prefetch_handler: b .
fiq_handler:      b .