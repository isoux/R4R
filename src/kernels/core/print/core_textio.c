/*
 * R4R License: MIT
 *
 * - Indentation: 4 spaces
 *
 * kernels/core/print/core_textio.c
 *
 * Basic VGA text output for core R4R kernel
 *
 * (C) Copyright 2025 Isa <isa@isoux.org>
 */

#include <core/core_textio.h>
#include <hw/vga_colors.h>
#include <hw/io.h>

#define VGA_MEM        ((volatile u16*)0xB8000)
#define VGA_COLS       80
#define VGA_ROWS       25
#define DEFAULT_COLOR  (FG_GREEN | BG_BLACK)

static u8 cursor_row = 0;
static u8 cursor_col = 0;

static void update_cursor() {
    u16 pos = cursor_row * VGA_COLS + cursor_col;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (u8) (pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (u8) ((pos >> 8) & 0xFF));
}

void textio_clear(void) {
    for (u16 i = 0; i < VGA_COLS * VGA_ROWS; i++) {
        VGA_MEM[i] = (u16) ' ' | (DEFAULT_COLOR << 8);
    }
    cursor_row = 0;
    cursor_col = 0;
    update_cursor();
}

void textio_putc(char c, u8 color) {
    if (c == '\n') {
        cursor_row++;
        cursor_col = 0;
    } else if (c == '\r') {
        cursor_col = 0;
    } else {
        VGA_MEM[cursor_row * VGA_COLS + cursor_col] = (u16) c | (color << 8);
        cursor_col++;
        if (cursor_col >= VGA_COLS) {
            cursor_col = 0;
            cursor_row++;
        }
    }
    if (cursor_row >= VGA_ROWS) {
        textio_scroll();
    }
    update_cursor();
}

void textio_puts(const char *s, u8 color) {
    while (*s) {
        textio_putc(*s++, color);
    }
}

void textio_scroll(void) {
    for (u16 row = 1; row < VGA_ROWS; row++) {
        for (u16 col = 0; col < VGA_COLS; col++) {
            VGA_MEM[(row - 1) * VGA_COLS + col] = VGA_MEM[row * VGA_COLS + col];
        }
    }
    for (u16 col = 0; col < VGA_COLS; col++) {
        VGA_MEM[(VGA_ROWS - 1) * VGA_COLS + col] = ' ' | (DEFAULT_COLOR << 8);
    }
    if (cursor_row > 0)
        cursor_row--;
}

void textio_set_cursor(u8 row, u8 col) {
    if (row >= VGA_ROWS)
        row = VGA_ROWS - 1;
    if (col >= VGA_COLS)
        col = VGA_COLS - 1;

    cursor_row = row;
    cursor_col = col;
    update_cursor();
}

void textio_get_cursor(u16 *row, u16 *col) {
    u16 pos = 0;
    outb(0x3D4, 0x0F);
    pos |= inb(0x3D5);
    outb(0x3D4, 0x0E);
    pos |= ((u16) inb(0x3D5)) << 8;

    *row = pos / VGA_COLS;
    *col = pos % VGA_COLS;
}

/*
 * Write a string at a given screen position.
 * This version does NOT change the global cursor position.
 */
void textio_puts_at(const char *msg, u8 color, u8 row, u8 col) {
    if (row >= VGA_ROWS || col >= VGA_COLS)
        return;

    u8 old_row = cursor_row;
    u8 old_col = cursor_col;

    cursor_row = row;
    cursor_col = col;
    textio_puts(msg, color);

    // Restore previous cursor position
    cursor_row = old_row;
    cursor_col = old_col;
    update_cursor();
}

void textio_init(void) {
    u16 row, col;
    textio_get_cursor(&row, &col);

    cursor_row = row;
    cursor_col = col;

    update_cursor();
}
