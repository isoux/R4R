/*
 * R4R License: MIT
 *
 * - Indentation: 4 spaces
 *
 * kernels/devs/devs_irq.c
 *
 * (C) Copyright 2025 Isa <isa@isoux.org>
 */

#include <typedef.h>

#include "devs_irq.h"

typedef void (*void_func)(void);
// Table of interrupt functions at devs ring 1.
// Here is place to put all main routines of all devices
// to handle their interrupts.
void_func devs_int_func_tbl[] = {
    (void_func)NULL,    // index 0
    get_keyboard_int    // index 1 (0x21 - 0x20)
};

__attribute__((naked))
void devs_irq_task(void) {
    for (;;) {
        // Call function from table index
        devs_int_func_tbl[tss_devs_irq.ebx]();
        // necessary for naked ISR
        __asm__ volatile ("iret");
    }
}
