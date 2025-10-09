/*
 * 4RING_OS License: MIT
 *
 *  - Indentation: 4 spaces
 *
 * include/idt/idt_build.h
 *
 * IDT table definitions and initialization functions for core (ring 0)
 *
 * (C) Copyright 2025 Isa <isa@isoux.org>
 */

#ifndef IDT_BUILD_H
#define IDT_BUILD_H

#include <typedef.h>

// Total IDT entries
#define IDT_ENTRIES 256

// Exposed IDT table
//extern l_long idt[IDT_ENTRIES];

// Initialize the whole IDT with the default handler
void setup_idt(void);

// Dynamically set a specific IDT entry
void idt_set_entry(int index, void (*handler)(void), u8 dpl);

#endif /* IDT_BUILD_H */

