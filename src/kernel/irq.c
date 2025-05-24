#include "../include/irq.h"
#include "../include/idt.h"
#include "../include/io.h"

#define IRQ_BASE 32

static void (*irq_routines[16])(registers_t r);

void irq_install_handler(int irq, void (*handler)(registers_t r)) {
    irq_routines[irq] = handler;
}

void irq_handler(registers_t r) {
    if (irq_routines[r.int_no - IRQ_BASE]) {
        irq_routines[r.int_no - IRQ_BASE](r);
    }

    if (r.int_no >= 40)
        outb(0xA0, 0x20); // slave
    outb(0x20, 0x20);     // master
}

void irq_install() {
    // Remap PIC
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    // Set IDT gates for IRQs
    for (int i = 0; i < 16; i++) {
        extern void irq0();
        idt_set_gate(IRQ_BASE + i, (uint32_t)((void*)irq0 + i * 8), 0x08, 0x8E);
    }
}
