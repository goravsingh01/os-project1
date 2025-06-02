#include "../include/keyboard.h"
#include "../include/io.h"
#include "../include/screen.h"
#include "../include/shell.h"

// Keyboard buffer
#define BUFFER_SIZE 256
static char input_buffer[BUFFER_SIZE];
static uint32_t buffer_pos = 0;

// US keyboard layout scancode to ASCII
static const char scancode_to_ascii[] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' '
};

void init_keyboard() {
    // Simple polling-based keyboard for now
    buffer_pos = 0;
}

void keyboard_callback(uint8_t scancode) {
    // Handle key release (bit 7 set)
    if (scancode & 0x80) {
        return;
    }
    
    // Handle special keys
    if (scancode == KEY_ENTER) {
        // Null-terminate the buffer
        input_buffer[buffer_pos] = '\0';
        print("\n");
        
        // Process the command
        shell_process_command(input_buffer);
        
        // Reset buffer
        buffer_pos = 0;
    } 
    else if (scancode == KEY_BACKSPACE && buffer_pos > 0) {
        buffer_pos--;
        backspace();
    }
    // Handle regular keys
    else if (scancode < sizeof(scancode_to_ascii) && scancode_to_ascii[scancode]) {
        char c = scancode_to_ascii[scancode];
        if (buffer_pos < BUFFER_SIZE - 1) {
            input_buffer[buffer_pos++] = c;
            print_char(c);
        }
    }
}

// Poll keyboard for input (simple approach without interrupts)
void keyboard_poll() {
    uint8_t status = inb(KEYBOARD_COMMAND_PORT);
    
    // Check if output buffer is full (bit 0 of status)
    if (status & 1) {
        uint8_t scancode = inb(KEYBOARD_DATA_PORT);
        keyboard_callback(scancode);
    }
}