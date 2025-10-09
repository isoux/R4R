/*
 * R4R License: MIT
 *
 * - Indentation: 4 spaces
 *
 * include/core/core_textio.h
 *
 * Header for basic VGA text output
 *
 * (C) Copyright 2025 Isa <isa@isoux.org>
 */

#ifndef CORE_TEXTIO_H
#define CORE_TEXTIO_H

#include <typedef.h>

void textio_init(void);
void textio_clear(void);
void textio_putc(char c, u8 color);
void textio_puts(const char *s, u8 color);
void textio_scroll(void);
void textio_get_cursor(u16 *row, u16 *col);
void textio_set_cursor(u8 row, u8 col);
void textio_puts_at(const char *msg, u8 color, u8 row, u8 col);

#endif // CORE_TEXTIO_H
