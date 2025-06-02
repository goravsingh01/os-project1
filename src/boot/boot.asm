; Multiboot header for GRUB
section .multiboot
    align 4
    dd 0x1BADB002          ; magic number
    dd 0x00                ; flags
    dd -(0x1BADB002 + 0x00); checksum

section .bss
    align 16
    stack_bottom:
    resb 16384             ; 16 KB stack
    stack_top:

section .text
    global _start

_start:
    ; Setup stack
    mov esp, stack_top

    ; Call the kernel main function
    extern kernel_main
    call kernel_main

    ; Halt if kernel returns
    cli
.hang:
    hlt
    jmp .hang