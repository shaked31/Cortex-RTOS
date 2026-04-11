ENTRY(_Reset) /* The entry point of the program will be in _Reset label in the startup.s file */

/* The starting point of memory in qemu for versatilepb is 0x10000
 * 128MB is the maximum amount of memory the linker can fill
 */
MEMORY
{
    RAM (rwx) : ORIGIN = 0x10000, LENGTH = 128M
}

SECTIONS
{
    /* Location counter - starts in the starting point of memory */
    . = 0x10000;

    /* Code section */
    .text :
    {
        /* MUST - vector table (of interrupts) must be the first in memory
         * Ensures it is not removed by the linker's garbage collector */
        KEEP(*(.vector_table))
        *(.text)
        *(.text.*)
    } > RAM

    /* Read-only data */
    .rodata :
    {
        *(.rodata)
        *(.rodata.*)
    } > RAM

    /* Initialized data */
    .data :
    {
        *(.data)
        *(.data.*)
    } > RAM

    /* Uninitialized data */
    .bss :
    {
        _bss_start = .;
        *(.bss)
        *(.bss.*)
        *(COMMON)
        _bss_end = .;
    } > RAM

    /* Stack defenition */
    . = ALIGN(8);     /* ARMv7-A architecture requires the stack to be aligned to 8 bytes */
    _stack_bottom = .;
    . += 0x4000; /* Allocation of 16KB for stack */
    _stack_top = .;

    _end = .;
}