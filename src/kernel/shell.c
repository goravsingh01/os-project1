#include "../include/shell.h"
#include "../include/screen.h"
#include "../include/string.h"

void shell_start() {
    print("Simple OS Shell v0.1\n");
    print("Type 'help' for available commands\n");
    print("> ");
}

void shell_process_command(const char* command) {
    if (strcmp(command, "help") == 0) {
        print("Available commands:\n");
        print("  help    - Display this help message\n");
        print("  clear   - Clear the screen\n");
        print("  echo    - Echo the arguments\n");
        print("  version - Display OS version\n");
    }
    else if (strcmp(command, "clear") == 0) {
        clear_screen();
    }
    else if (strncmp(command, "echo ", 5) == 0) {
        print(command + 5);
        print("\n");
    }
    else if (strcmp(command, "version") == 0) {
        print("Simple OS v0.1\n");
    }
    else if (command[0] != '\0') {
        print("Unknown command: ");
        print(command);
        print("\n");
    }
    
    // Show prompt
    print("> ");
}