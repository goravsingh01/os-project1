[bits 32]
global isr0, isr1
global irq0, irq1
global idt_load

extern isr_handler
extern irq_handler

; ---- IDT Load ----
idt_load:
    mov eax, [esp+4]
    lidt [eax]
    ret

; ---- ISR stubs (exceptions) ----
isr0:
    cli
    push 0
    push 0
    jmp isr_common_stub
isr1:
    cli
    push 0
    push 1
    jmp isr_common_stub

; ---- IRQ stubs (hardware interrupts) ----
irq0:
    cli
    push 0
    push 32
    jmp isr_common_stub
irq1:
    cli
    push 0
    push 33
    jmp isr_common_stub

; ---- Common stub to call C handler ----
isr_common_stub:
    pusha
    mov ax, ds
    push eax
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    call isr_dispatch
    pop eax
    popa
    add esp, 8
    sti
    iret

isr_dispatch:
    cmp dword [esp + 32], 32
    jl .handle_isr
    jmp irq_handler
.handle_isr:
    jmp isr_handler
