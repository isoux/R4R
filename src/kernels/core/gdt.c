/*
 * R4R License: MIT
 *
 * - Indentation: 4 spaces
 *
 * kernels/core/gdt.c
 *
 * (C) Copyright 2025 Isa <isa@isoux.org>
 */

#include <typedef.h>
#include <sys.h>
#include <task.h>

/*
 * Patch an existing call-gate placeholder in GDT at `selector`:
 * - Set handler offset (low/high)
 * - Set param_count (bits 32..36)
 */
void gdt_call_gate_set(u16 selector, void (*handler)(void), u8 param_count) {
    u64 *gdt_table = (u64 *)GDT_START;
    u32 offset = (u32)handler;
    u16 index = selector >> 3;

    u64 desc = gdt_table[index];

    // Clear offset_low (bits 0–15) and offset_high (48–63)
    desc &= ~0xFFFFULL;
    desc &= ~(0xFFFFULL << 48);

    // Set offset parts
    desc |= (offset & 0xFFFF);
    desc |= ((u64)(offset >> 16)) << 48;

    // Clear and set param count (bits 32–36)
    desc &= ~(0x1FULL << 32);
    desc |= ((u64)(param_count & 0b00011111)) << 32;

    gdt_table[index] = desc;
}

void gdt_ldt_set(u16 selector, u32 base, u32 limit) {
    u64 *gdt   = (u64 *)GDT_START;
    u16 index  = selector >> 3;
    u64 desc   = gdt[index];

    // Clear: limit[0..15], base[0..23], limit[16..19], base[24..31]
    desc &= ~0x000000000000FFFFULL;              //limit low
    desc &= ~0x000000FFFFFF0000ULL;              // base 0..23
    desc &= ~0x000F000000000000ULL;              // limit high nibble
    desc &= ~0xFF00000000000000ULL;              // base 24..31

    //Set new values
    desc |= (u64)(limit & 0xFFFF);               // limit low */
    desc |= ((u64)(base  & 0x00FFFFFF) << 16);   // base 0..23 */
    desc |= ((u64)((limit >> 16) & 0xF) << 48);  // limit high */
    desc |= ((u64)((base  >> 24) & 0xFF) << 56); // base 24..31 */

    gdt[index] = desc;
}

/* Patch existing TSS descriptor at `selector` with base=addr(tss)
 * and limit=sizeof(tss)-1. Keeps type (0x9), DPL, P and flags
 * exactly as they are in the placeholder.
 */
void gdt_tss_set(u16 selector, struct tss32 *tss) {
    u64 *gdt_table = (u64 *)GDT_START;
    u16 index = selector >> 3;

    u32 base  = (u32)(u32)tss;                 // TSS linear/phys base as used in your layout
    u32 limit = (u32)sizeof(struct tss32) - 1; // byte granularity for TSS */

    u64 desc = gdt_table[index];

    /* Clear fields we are going to replace:
       - limit[0..15]   (bits 0..15)
       - base[0..23]    (bits 16..39)
       - limit[16..19]  (bits 48..51)
       - base[24..31]   (bits 56..63)
       (Do NOT touch type/DPL/P/flags) */
    desc &= ~0x000000000000FFFFULL;
    desc &= ~0x000000FFFFFF0000ULL;
    desc &= ~0x000F000000000000ULL;
    desc &= ~0xFF00000000000000ULL;

    // Set new base/limit pieces
    desc |= (u64)(limit & 0xFFFF);                // limit low
    desc |= ((u64)(base  & 0x00FFFFFF) << 16);    // base 0..23
    desc |= ((u64)((limit >> 16) & 0xF) << 48);   // limit high
    desc |= ((u64)((base  >> 24) & 0xFF) << 56);  // base 24..31

    gdt_table[index] = desc;
}

void gdt_set_desc(u16 selector , u64 descriptor) {
    u64 *gdt_table = (u64 *)GDT_START;
    u16 index = selector >> 3;
    if (index < GDT_ENTRIES) {
        gdt_table[index] = descriptor;
    }
}
