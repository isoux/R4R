/*
 * R4R License: MIT
 *
 * - Indentation: 4 spaces
 *
 * include/gdt/gdt.h
 *
 * (C) Copyright 2025 Isa <isa@isoux.org>
 */

#ifndef GDT_H
#define GDT_H

#include <stdint.h>
#include <sys.h>

#define GDT_DWORD_COUNT (GDT_SIZE / 4) // number of 32-bit words

// Zero-initialize a GDT table (must be 65536 bytes, i.e., 16384 DWORDs)
__attribute__((always_inline))
static inline void gdt_zero_fill(u32 gdt_base_addr) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "cld\n\t"                   // clear direction flag (forward copy)
        "xor eax, eax\n\t"          // set EAX = 0 (source value is zero)
        "mov edi, %[dst]\n\t"       // destination address (GDT base)
        "mov ecx, %[count]\n\t"     // number of DWORDs to write
        "rep stosd\n\t"             // repeat storing EAX into [EDI], ECX times
        ".att_syntax prefix"
        :
        : [dst] "r"(gdt_base_addr), [count] "r"(GDT_DWORD_COUNT)
        : "eax", "edi", "ecx", "memory"
    );
}

#endif // GDT_H

