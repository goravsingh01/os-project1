#include "../include/screen.h"
#include "../include/idt.h"
#include "../include/shell.h"

void kernel_main() {
    clear_screen();
    print("Welcome to MyOS!\n");
    idt_install();
    shell_start();  // start CLI loop
}
