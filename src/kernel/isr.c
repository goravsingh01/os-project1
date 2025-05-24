#include "../include/isr.h"
#include "../include/idt.h"
#include "../include/io.h"
#include "../include/screen.h"

extern void isr0();
extern void isr1();
// up to isr31...

void isr_install() {
    idt_set_gate(0, (uint32_t)isr0, 0x08, 0x8E);
    idt_set_gate(1, (uint32_t)isr1, 0x08, 0x8E);
    // ... more ISRs
}

void isr_handler(registers_t r) {
    print("Received interrupt: ");
    print_num(r.int_no);
    print("\n");
}
