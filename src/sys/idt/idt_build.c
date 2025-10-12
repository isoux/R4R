/*
 * 4RING_OS License: MIT
 *
 * - Indentation: 4 spaces
 *
 * sys/idt/idt.c
 *
 * Setup interrupt descriptor table at core (ring 0)
 *
 * (C) Copyright 2021 Isa <isa@isoux.org>
 */

#include <typedef.h>
#include <sys.h>
#include <gdt_sys.h>
#include <idt/idt_build.h>

#include "sys_exceptions.h"

void comm_ignore_int(void);
void setup_sys_exceptions(void);

u32 * idt_table = (u32 *) IDT_START;

struct __packed_ idt_opcode_p {
    u16 size;
    u32 *idt;
};

static struct idt_opcode_p idt_opcode;

void idt_prepare(void) {
    idt_opcode.size = IDT_ENTRIES - 1;
    idt_opcode.idt = idt_table;
};

void idt_init(void) {
    idt_prepare();
    __asm__ volatile (
        "lidt %0\n\t"
        : : "m"(idt_opcode) : "memory"
    );
}

// -----------------------------------------------------------------------------
// Setup entire IDT and install system exception handlers (0–31)
// -----------------------------------------------------------------------------
void setup_idt(void) {
    u32 handler_addr = (u32)comm_ignore_int;
    u32 handler_low = (handler_addr & 0xFFFF) | (CORE_CODE << 16);
    u32 handler_high = (0x8E << 8) | (handler_addr & 0xFFFF0000);

    u32 *idt_ptr = (u32 *) IDT_START;

    // Fill all 256 entries with the default ignore handler
    for (int i = 0; i < 256; i++) {
        idt_ptr[i * 2]     = handler_low;
        idt_ptr[i * 2 + 1] = handler_high;
    }

    // Install Intel-defined system exceptions 0–31
    setup_sys_exceptions();

    // Load IDT
    idt_init();
}


// Dynamically set specific IDT entry
void idt_set_entry(int index, void (*handler)(void), u8 dpl) {
    u32 handler_addr = (u32)handler;

    u32 descriptor_low = (handler_addr & 0xFFFF) | (CORE_CODE << 16);
    u32 descriptor_high = ((0x8E | (dpl << 5)) << 8) | (handler_addr & 0xFFFF0000);

    idt_table[index * 2] = descriptor_low;
    idt_table[index * 2 + 1] = descriptor_high;
}

// Default interrupt handler
__attribute__((naked)) void comm_ignore_int(void) {
    __asm__ volatile (
        ".intel_syntax noprefix\n\t"
        "push ds\n\t"
        "push eax\n\t"
        "pop eax\n\t"
        "pop ds\n\t"
        "iretd\n\t"
        ".att_syntax"
    );
}

// -----------------------------------------------------------------------------
// Setup system exception handlers (Intel-defined 0x00–0x1F)
// -----------------------------------------------------------------------------
void setup_sys_exceptions(void) {

    // Core (Ring0) exceptions: 0–31
    idt_set_entry(0,  sys_int_0,  0);
    idt_set_entry(1,  sys_int_1,  0);
    idt_set_entry(2,  sys_int_2,  0);
    idt_set_entry(3,  sys_int_3,  0);
    idt_set_entry(4,  sys_int_4,  0);
    idt_set_entry(5,  sys_int_5,  0);
    idt_set_entry(6,  sys_int_6,  0);
    idt_set_entry(7,  sys_int_7,  0);
    idt_set_entry(8,  sys_int_8,  0);
    idt_set_entry(9,  sys_int_9,  0);
    idt_set_entry(10, sys_int_10, 0);
    idt_set_entry(11, sys_int_11, 0);
    idt_set_entry(12, sys_int_12, 0);
    idt_set_entry(13, sys_int_13, 0);
    idt_set_entry(14, sys_int_14, 0);
    idt_set_entry(15, sys_int_15, 0);
    idt_set_entry(16, sys_int_16, 0);
    idt_set_entry(17, sys_int_17, 0);
    idt_set_entry(18, sys_int_18, 0);
    idt_set_entry(19, sys_int_19, 0);
    idt_set_entry(20, sys_int_20, 0);
    idt_set_entry(21, sys_int_21, 0);
    idt_set_entry(22, sys_int_22, 0);
    idt_set_entry(23, sys_int_23, 0);
    idt_set_entry(24, sys_int_24, 0);
    idt_set_entry(25, sys_int_25, 0);
    idt_set_entry(26, sys_int_26, 0);
    idt_set_entry(27, sys_int_27, 0);
    idt_set_entry(28, sys_int_28, 0);
    idt_set_entry(29, sys_int_29, 0);
    idt_set_entry(30, sys_int_30, 0);
    idt_set_entry(31, sys_int_31, 0);
}

