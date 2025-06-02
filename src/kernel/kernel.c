#include "../include/screen.h"
#include "../include/keyboard.h"
#include "../include/shell.h"

void kernel_main() {
    // Initialize hardware
    init_screen();
    init_keyboard();
    
    // Clear screen and display welcome message
    clear_screen();
    print("Simple OS Booted Successfully!\n");
    
    // Start the shell
    shell_start();
    
    // Main loop - poll keyboard
    while(1) {
        keyboard_poll();
    }
}