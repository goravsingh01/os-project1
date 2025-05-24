section .multiboot
    align 4
    dd 0x1BADB002          ; magic number
    dd 0x00                ; flags
    dd -(0x1BADB002 + 0x00); checksum

section .text
    extern kernel_main     ; Declare external C function
    global start

start:
    cli                    ; Clear interrupts
    call kernel_main       ; Call the C function
    hlt                    ; Halt if it returns
