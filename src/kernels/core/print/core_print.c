/*
 * R4R License: MIT
 *
 * - Indentation: 4 spaces
 *
 * kernels/core/print/core_print.c
 *
 * Early debug output for R4R core (ring 0)
 *
 * (C) Copyright 2025 Isa <isa@isoux.org>
 */

#include <core/core_textio.h>
#include <hw/vga_colors.h>

void core_print(const char *msg) {
    textio_puts("R0: ", FG_RED | BG_BLACK);
    textio_puts(msg, FG_RED | BG_BLACK);
}

void core_print_color(const char *msg, u8 color) {
    textio_puts(msg, color);
}

void core_print_color_at(const char *msg, u8 color, u8 row, u8 col) {
    textio_puts_at(msg, color, row, col);
}
