/*
 * R4R License: MIT
 *
 * - Indentation: 4 spaces
 *
 * core/core_print.h
 *
 * Interface for early core printing
 *
 * (C) Copyright 2025 Isa <isa@isoux.org>
 */

#ifndef CORE_PRINT_H
#define CORE_PRINT_H


#include <typedef.h>

// Fixed-color print (green on black, for early debug)
void core_print(const char *msg);

// Flexible print (you pass the color)
void core_print_color(const char *msg, u8 color);

// Flexible print (you pass the color and position)
void core_print_color_at(const char *msg, u8 color, u8 row, u8 col);


#endif // CORE_PRINT_H
