#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

// Keyboard constants
#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_COMMAND_PORT 0x64

// Special keys
#define KEY_BACKSPACE 0x0E
#define KEY_ENTER 0x1C

// Keyboard functions
void init_keyboard();
void keyboard_callback(uint8_t scancode);

#endif