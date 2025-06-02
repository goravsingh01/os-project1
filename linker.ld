ENTRY(_start)
SECTIONS
{
    . = 0x100000;          /* Load at 1MB */

    .text : {
        *(.multiboot)      /* Multiboot header first */
        *(.text)
    }

    .rodata : {
        *(.rodata)
    }

    .data : {
        *(.data)
    }

    .bss : {
        *(.bss)
    }
}