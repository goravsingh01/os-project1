#include "../include/screen.h"
#include "../include/io.h"
#include <stdint.h>

uint16_t* video_memory = (uint16_t*) VIDEO_ADDRESS;
uint8_t cursor_row = 0;
uint8_t cursor_col = 0;

static void move_cursor() {
    uint16_t pos = cursor_row * MAX_COLS + cursor_col;
    // Cursor ports
    outb(0x3D4, 14);                  // High byte of cursor pos
    outb(0x3D5, (pos >> 8) & 0xFF);
    outb(0x3D4, 15);                  // Low byte of cursor pos
    outb(0x3D5, pos & 0xFF);
}

static void scroll() {
    if (cursor_row >= MAX_ROWS) {
        // Shift all rows up by one
        for (int row = 1; row < MAX_ROWS; row++) {
            for (int col = 0; col < MAX_COLS; col++) {
                video_memory[(row - 1) * MAX_COLS + col] =
                    video_memory[row * MAX_COLS + col];
            }
        }

        // Clear last line
        for (int col = 0; col < MAX_COLS; col++) {
            video_memory[(MAX_ROWS - 1) * MAX_COLS + col] = ((uint16_t)WHITE_ON_BLACK << 8) | ' ';
        }

        cursor_row = MAX_ROWS - 1;
    }
}

void clear_screen() {
    for (int i = 0; i < MAX_ROWS * MAX_COLS; i++) {
        video_memory[i] = ((uint16_t)WHITE_ON_BLACK << 8) | ' ';
    }
    cursor_row = 0;
    cursor_col = 0;
    move_cursor();
}

void print_char(char c) {
    if (c == '\n') {
        cursor_col = 0;
        cursor_row++;
    } else if (c == '\r') {
        cursor_col = 0;
    } else {
        video_memory[cursor_row * MAX_COLS + cursor_col] = ((uint16_t)WHITE_ON_BLACK << 8) | c;
        cursor_col++;
        if (cursor_col >= MAX_COLS) {
            cursor_col = 0;
            cursor_row++;
        }
    }
    scroll();
    move_cursor();
}

void print(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        print_char(str[i]);
    }
}

void print_num(int num) {
    if (num == 0) {
        print_char('0');
        return;
    }

    char buf[12];  // Enough for 32-bit int
    int i = 0;
    int is_negative = 0;

    if (num < 0) {
        is_negative = 1;
        num = -num;
    }

    while (num > 0) {
        buf[i++] = (num % 10) + '0';
        num /= 10;
    }

    if (is_negative) {
        buf[i++] = '-';
    }

    for (int j = i - 1; j >= 0; j--) {
        print_char(buf[j]);
    }
}
