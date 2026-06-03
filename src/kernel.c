#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

//VGA text mode constants
#define VGA_WIDTH   80
#define VGA_HEIGHT  25
#define VGA_MEMORY  0xB8000 

//Defining some basic colours
enum vga_colour {
    VGA_COLOUR_BLACK = 0,
    VGA_COLOUR_BLUE = 1,
    VGA_COLOUR_GREEN = 2,
    VGA_COLOUR_CYAN = 3,
    VGA_COLOUR_RED = 4,
    VGA_COLOUR_MAGENTA = 5,
    VGA_COLOUR_BROWN = 6,
    VGA_COLOUR_LIGHT_GREY = 7,
    VGA_COLOUR_DARK_GREY = 8,
    VGA_COLOUR_LIGHT_BLUE = 9,
    VGA_COLOUR_LIGHT_GREEN = 10,
    VGA_COLOUR_LIGHT_CYAN = 11,
    VGA_COLOUR_LIGHT_RED = 12,
    VGA_COLOUR_LIGHT_MAGENTA = 13,
    VGA_COLOUR_LIGHT_BROWN = 14,
    VGA_COLOUR_WHITE = 15,
};

//Combining the foreground and background colours into a single byte
static inline uint8_t vga_entry_colour(enum vga_colour fg, enum vga_colour bg) {
    return fg | bg << 4;
}

//Combining the ASCII character and colour byte into a single 16-bit value
static inline uint16_t vga_entry(unsigned char uc, uint8_t colour) {
    return (uint16_t) uc | (uint16_t) colour << 8;
}

//Finds the length of a string. Needed to write to display
size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len]) {
        len++;
    }
    return len;
}

//Global variables to keep track of the terminal state
size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer = (uint16_t*)VGA_MEMORY;

//Initialises the terminal
void terminal_init() {
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_colour(VGA_COLOUR_LIGHT_GREY, VGA_COLOUR_BLACK);
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
}

void terminal_setcolor(uint8_t color) {
    terminal_color = color;
}

//Puts a single character at cursor position
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}

//Calls terminal_putentryat and advances the cursor position
void terminal_putchar(char c) {
    if (c == '\n') {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT) {
            terminal_row = 0;
        }
        return;
    }

    terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
    if (++terminal_column == VGA_WIDTH) {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT) {
            terminal_row = 0;
        }
    }
}

void terminal_write(const char* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        terminal_putchar(data[i]);
    }
}

void terminal_writestring(const char* data) {
    terminal_write(data, strlen(data));
}

void kernel_main() {
    terminal_init();

    terminal_writestring("Woah\nthis is a kernel!\n");
}