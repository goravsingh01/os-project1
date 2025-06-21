#ifndef SCREEN_H
#define SCREEN_H

#include <stdint.h>

#define VIDEO_MEMORY 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_COLOR_WHITE_ON_BLACK 0x0F

void init_screen();
void clear_screen();
void print(const char* message);
void print_char(char c);
void backspace();

#endif