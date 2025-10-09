/*
 * R4R License: MIT
 *
 * - Indentation: 4 spaces
 *
 * gdt_init.c
 *
 * (C) Copyright 2025 Isa <isa@isoux.org>
 */

#include <typedef.h>
#include <gdt_sys.h>
#include <sys.h>

u32 *gdt_table = (u32*) GDT_START;

struct __packed_ gdt_opcode_p {
    u16 size;
    u32 *gdt;
};

static struct gdt_opcode_p gdt_opcode;

void gdt_prepare (void) {
    gdt_opcode.size = GDT_ENTRIES - 1;
    gdt_opcode.gdt = gdt_table;
}

void gdt_init (void) {
    gdt_prepare ();
    __asm__ __volatile__ (
        "lgdt %0\n\t"
        : : "m"(gdt_opcode) : "memory"
    );
}
