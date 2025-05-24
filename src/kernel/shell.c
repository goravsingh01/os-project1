#include "../include/screen.h"
#include "../include/string.h"
#include "../include/shell.h"

void shell_start() {
    print("CLI OS Ready\n");
    print("> ");
}

void shell_handle_input(const char* input) {
    if (strcmp(input, "help") == 0) {
        print("Available commands:\n");
        print("help - Show this help\n");
        print("clear - Clear the screen\n");
        print("echo - Repeat what you type\n");
    } else if (strcmp(input, "clear") == 0) {
        clear_screen();
    } else if (strncmp(input, "echo ", 5) == 0) {
        print(input + 5);
        print("\n");
    } else {
        print("Unknown command: ");
        print(input);
        print("\n");
    }

    print("> ");
}
