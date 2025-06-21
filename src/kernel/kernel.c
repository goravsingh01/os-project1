#include "../include/screen.h"
#include "../include/keyboard.h"
#include "../include/shell.h"
#include "../include/filesystem.h"

void kernel_main() {
    // Initialize hardware
    init_screen();
    init_keyboard();
    
    // Initialize filesystem
    fs_init();
    
    // Clear screen and display welcome message
    clear_screen();
    print("--------       --------\n");
    print("|      |       |\n");
    print("|      |       |\n");
    print("|      |       |\n");
    print("|      |       --------\n");
    print("|      |               |\n");
    print("|      |               |\n");
    print("|      |               |\n");
    print("--------       ---------\n");


    print("Simple OS Booted Successfully!\n");
    
    // Start the shell
    shell_start();
    
    // Main loop - poll keyboard
    while(1) {
        keyboard_poll();
    }
}
