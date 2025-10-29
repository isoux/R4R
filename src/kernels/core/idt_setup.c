/*
 * R4R License: MIT
 *
 * - Indentation: 4 spaces
 *
 * kernels/core/idt_setup.c
 *
 * Setup interrupt handlers at core (ring 0)
 *
 * (C) Copyright 2025 Isa <isa@isoux.org>
 */

#include <typedef.h>
#include <sys.h>
#include <gdt_sys.h>
#include <idt/idt_build.h>

#include "sys_exceptions.h"

// Dynamically set specific IDT entry
void idt_set_entry(u32 index, void (*handler)(void), u8 dpl) {

    u32 code_segment;
    switch (dpl) {
        case DPL_RING_0:
            code_segment = CORE_CODE;
            break;

        case DPL_RING_1:
            code_segment = DEVS_CODE;
            break;

        case DPL_RING_2:
            code_segment = LIBS_CODE;
            break;

        case DPL_RING_3:
            code_segment = USERS_CODE;
            break;

        default:
            code_segment = CORE_CODE;
            break;
    }

    u32 handler_addr = (u32)handler;
    u32 * idt_table = (u32 *) IDT_START;

    // Base type: 0x0E = 32-bit interrupt gate
    u8 type_attr = (0x0E & 0x1F) | ((dpl & 0x3) << 5) | 0x80;

    u32 descriptor_low = (handler_addr & 0xFFFF)
            | (code_segment << 16);
    u32 descriptor_high = (handler_addr & 0xFFFF0000)
            | ((u32)type_attr << 8);

    idt_table[index * 2] = descriptor_low;
    idt_table[index * 2 + 1] = descriptor_high;
}

// Setup default ignore handler from int 32 to int 256
void setup_sys_int_ignore(void) {
    u32 handler_addr = (u32)sys_int_ignore;
    u32 handler_low = (handler_addr & 0xFFFF) | (CORE_CODE << 16);
    u32 handler_high = (0x8E << 8) | (handler_addr & 0xFFFF0000);

    u32 *idt_ptr = (u32 *) IDT_START;

    // Fill rest (256-32) entries with the default ignore handler
    for (int i = 32; i < IDT_ENTRIES; i++) {
        idt_ptr[i * 2]     = handler_low;
        idt_ptr[i * 2 + 1] = handler_high;
    }
}

// -----------------------------------------------------------------------------
// Setup system exception handlers (Intel-defined 0x00–0x1F)
// -----------------------------------------------------------------------------
void setup_sys_exceptions(void) {

    // Core (Ring0) exceptions: 0–31
    idt_set_entry(0,  sys_int_0,  CORE_CODE);
    idt_set_entry(1,  sys_int_1,  CORE_CODE);
    idt_set_entry(2,  sys_int_2,  CORE_CODE);
    idt_set_entry(3,  sys_int_3,  CORE_CODE);
    idt_set_entry(4,  sys_int_4,  CORE_CODE);
    idt_set_entry(5,  sys_int_5,  CORE_CODE);
    idt_set_entry(6,  sys_int_6,  CORE_CODE);
    idt_set_entry(7,  sys_int_7,  CORE_CODE);
    idt_set_entry(8,  sys_int_8,  CORE_CODE);
    idt_set_entry(9,  sys_int_9,  CORE_CODE);
    idt_set_entry(10, sys_int_10, CORE_CODE);
    idt_set_entry(11, sys_int_11, CORE_CODE);
    idt_set_entry(12, sys_int_12, CORE_CODE);
    idt_set_entry(13, sys_int_13, CORE_CODE);
    idt_set_entry(14, sys_int_14, CORE_CODE);
    idt_set_entry(15, sys_int_15, CORE_CODE);
    idt_set_entry(16, sys_int_16, CORE_CODE);
    idt_set_entry(17, sys_int_17, CORE_CODE);
    idt_set_entry(18, sys_int_18, CORE_CODE);
    idt_set_entry(19, sys_int_19, CORE_CODE);
    idt_set_entry(20, sys_int_20, CORE_CODE);
    idt_set_entry(21, sys_int_21, CORE_CODE);
    idt_set_entry(22, sys_int_22, CORE_CODE);
    idt_set_entry(23, sys_int_23, CORE_CODE);
    idt_set_entry(24, sys_int_24, CORE_CODE);
    idt_set_entry(25, sys_int_25, CORE_CODE);
    idt_set_entry(26, sys_int_26, CORE_CODE);
    idt_set_entry(27, sys_int_27, CORE_CODE);
    idt_set_entry(28, sys_int_28, CORE_CODE);
    idt_set_entry(29, sys_int_29, CORE_CODE);
    idt_set_entry(30, sys_int_30, CORE_CODE);
    idt_set_entry(31, sys_int_31, CORE_CODE);
}

void setup_sys_interrupts(void) {
    setup_sys_int_ignore();
    setup_sys_exceptions();
}

