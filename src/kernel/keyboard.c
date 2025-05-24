#include "../include/keyboard.h"
#include "../include/screen.h"
#include "../include/io.h"

#define BUFFER_SIZE 256

char input_buffer[BUFFER_SIZE];
int buffer_index = 0;

extern void shell_handle_input(const char* input);

const char scancode_table[] = {
    0, 27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b', // Backspace
    '\t',                                                        // Tab
    'q','w','e','r','t','y','u','i','o','p','[',']','\n',        // Enter key
    0,                                                           // Ctrl
    'a','s','d','f','g','h','j','k','l',';','\'','`',
    0,                              // Left shift
    '\\','z','x','c','v','b','n','m',',','.','/',
    0,                              // Right shift
    '*',
    0,                              // Alt
    ' ',                            // Spacebar
    0,                              // Caps lock
    // ... more as needed
};

void keyboard_handler(registers_t r) {
    uint8_t scancode = inb(0x60);

    if (scancode > sizeof(scancode_table)) return;

    char key = scancode_table[scancode];
    if (!key) return;

    if (key == '\n') {
        input_buffer[buffer_index] = '\0';  // Null-terminate string
        print("\n");
        shell_handle_input(input_buffer);   // Send to shell
        buffer_index = 0;                   // Reset for next command
    } else {
        if (buffer_index < BUFFER_SIZE - 1) {
            input_buffer[buffer_index++] = key;
            char str[2] = {key, '\0'};
            print(str);
        }
    }
}
