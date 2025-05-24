#include "../include/idt.h"
#include "../include/isr.h"

idt_entry_t idt[IDT_ENTRIES];
idt_ptr_t idtp;

extern void idt_load(); // implemented in ASM

void idt_set_gate(int num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_low = base & 0xFFFF;
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].selector = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}

void idt_install() {
    idtp.limit = (sizeof(idt_entry_t) * IDT_ENTRIES) - 1;
    idtp.base = (uint32_t)&idt;

    for (int i = 0; i < IDT_ENTRIES; i++) {
        idt_set_gate(i, 0, 0, 0);
    }

    isr_install();
    irq_install();

    asm volatile("lidtl (%0)" : : "r" (&idtp));
}
