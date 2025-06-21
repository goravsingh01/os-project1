#include "../include/screen.h"
#include "../include/io.h"

// Screen state
static uint16_t* video_memory = (uint16_t*)VIDEO_MEMORY;
static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;

// Update hardware cursor
static void update_cursor() {
    uint16_t pos = cursor_y * VGA_WIDTH + cursor_x;
    
    outb(0x3D4, 14);                  // Tell VGA we're setting high cursor byte
    outb(0x3D5, (pos >> 8) & 0xFF);   // Send high byte
    outb(0x3D4, 15);                  // Tell VGA we're setting low cursor byte
    outb(0x3D5, pos & 0xFF);          // Send low byte
}

// Scroll the screen if needed
static void scroll() {
    if (cursor_y >= VGA_HEIGHT) {
        // Move rows up
        for (int y = 0; y < VGA_HEIGHT - 1; y++) {
            for (int x = 0; x < VGA_WIDTH; x++) {
                video_memory[y * VGA_WIDTH + x] = video_memory[(y + 1) * VGA_WIDTH + x];
            }
        }
        
        // Clear the last row
        for (int x = 0; x < VGA_WIDTH; x++) {
            video_memory[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = (VGA_COLOR_WHITE_ON_BLACK << 8) | ' ';
        }
        
        cursor_y = VGA_HEIGHT - 1;
    }
}

void init_screen() {
    cursor_x = 0;
    cursor_y = 0;
    clear_screen();
}

void clear_screen() {
    for (int y = 0; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            video_memory[y * VGA_WIDTH + x] = (VGA_COLOR_WHITE_ON_BLACK << 8) | ' ';
        }
    }
    
    cursor_x = 0;
    cursor_y = 0;
    update_cursor();
}

void print_char(char c) {
    if (c == '\n') {
        // New line
        cursor_x = 0;
        cursor_y++;
    } else if (c == '\r') {
        // Carriage return
        cursor_x = 0;
    } else if (c == '\b') {
        // Backspace
        if (cursor_x > 0) {
            cursor_x--;
            video_memory[cursor_y * VGA_WIDTH + cursor_x] = (VGA_COLOR_WHITE_ON_BLACK << 8) | ' ';
        }
    } else {
        // Regular character
        video_memory[cursor_y * VGA_WIDTH + cursor_x] = (VGA_COLOR_WHITE_ON_BLACK << 8) | c;
        cursor_x++;
        
        // Wrap to next line if needed
        if (cursor_x >= VGA_WIDTH) {
            cursor_x = 0;
            cursor_y++;
        }
    }
    
    scroll();
    update_cursor();
}

void print(const char* message) {
    for (int i = 0; message[i] != 0; i++) {
        print_char(message[i]);
    }
}

void backspace() {
    if (cursor_x > 0) {
        cursor_x--;
        video_memory[cursor_y * VGA_WIDTH + cursor_x] = (VGA_COLOR_WHITE_ON_BLACK << 8) | ' ';
        update_cursor();
    }
}