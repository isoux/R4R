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

void comm_ignore_int(void);

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
// Setup entire IDT with default handler
void setup_idt(void) {
    u32 handler_addr = (u32)comm_ignore_int;
    u32 handler_low = (handler_addr & 0xFFFF) | (CORE_CODE << 16);
    u32 handler_high = (0x8E << 8) | (handler_addr & 0xFFFF0000);

    u32 *idt_ptr = (u32 *) IDT_START;
    for (int i = 0; i < 256; i++) {
        idt_ptr[i * 2] = handler_low;
        idt_ptr[i * 2 + 1] = handler_high;
    }
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
        "nop \n\t"
        "pop eax\n\t"
        "pop ds\n\t"
        "iretd\n\t"
        ".att_syntax"
    );
}

