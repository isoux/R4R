/*
 * R4R License: MIT
 *
 * - Indentation: 4 spaces
 *
 * sys/idt/idt.c
 *
 * Setup interrupt descriptor table at init phase (ring 0)
 *
 * (C) Copyright 2025 Isa <isa@isoux.org>
 */

#include <typedef.h>
#include <sys.h>
#include <gdt_sys.h>
#include <idt/idt_build.h>

void comm_ignore_int(void);

u32 * idt_table = (u32 *) IDT_START;

struct __packed_ idt_opcode_p {
    u16 size;
    u32 *idt;
};

static struct idt_opcode_p idt_opcode;

void idt_prepare(void) {
    idt_opcode.size = (IDT_ENTRIES * 8) - 1;
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
    for (int i = 0; i < IDT_ENTRIES; i++) {
        idt_ptr[i * 2]     = handler_low;
        idt_ptr[i * 2 + 1] = handler_high;
    }

    // Load IDT
    idt_init();
}


// Dynamically set specific IDT entry
void idt_set_entry(u32 index, void (*handler)(void), u8 dpl) {
    u32 handler_addr = (u32)handler;
    u32 * idt_table = (u32 *) IDT_START;

    u32 descriptor_low = (handler_addr & 0xFFFF) | (CORE_CODE << 16);
    u32 descriptor_high = ((0x8E | (dpl << 5)) << 8)
        | (handler_addr & 0xFFFF0000);

    idt_table[index * 2] = descriptor_low;
    idt_table[index * 2 + 1] = descriptor_high;
}

// Default interrupt handler
// “INT” appears in red when an unexpected termination occurs during early system
// initialization — serving as a low-level visual indicator of an abort state.
void comm_ignore_int(void) {

    __asm__ volatile (
        // Each VGA cell: [character byte][attribute byte]
        // Attribute 0x04 = FG_RED | BG_BLACK

        // 'I' (0x49)
        "movl $0xb8000, %eax\n\t"
        "movw $0x0449, (%eax)\n\t"        // 'I' red on black

        // 'N' (0x4E)
        "addl $2, %eax\n\t"
        "movw $0x044e, (%eax)\n\t"        // 'N' red on black

        // 'T' (0x54)
        "addl $2, %eax\n\t"
        "movw $0x0454, (%eax)\n\t"        // 'T' red on black
    );

    while(1);
}

