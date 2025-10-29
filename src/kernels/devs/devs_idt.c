/*
 * R4R License: MIT
 *
 * - Indentation: 4 spaces
 *
 * kernels/devs/devs_idt.c
 *
 * (C) Copyright 2025 Isa <isa@isoux.org>
 */

#include <typedef.h>
#include <gdt_sys.h>
#include <sys.h>
#include <sys/sys_idt.h>
#include <devs/interrupt.h>

extern void keyboard_interrupt(void);

// Set call gate descriptor for devs
void setup_keyboard_int(void (*handler)(void)) {
    syscall_idt_desc_set(KEY_INT, handler, DPL_RING_1);
}

void setup_devs_idt(void) {

    setup_keyboard_int(keyboard_interrupt);
    //...
}
