/*
 * R4R License: MIT
 *
 * - Indentation: 4 spaces
 *
 * include/sys/sys_idt.h
 *
 * (C) Copyright 2025 Isa <isa@isoux.org>
 */

#include <typedef.h>
#include <gdt_sys.h>

// Set descriptor at IDT.
// This function calls call_gate from lower privileged rings.
__attribute__((always_inline))
static inline void
syscall_idt_desc_set(u32 index, void (*handler)(void), u8 dpl) {

    __asm__ __volatile__ (
        "lcall $"STR(CG_IDT_SET)", $0\n\t" // far call via call gate
        :
        : "a"((u32)dpl),    // EAX = dpl
          "b"(handler),     // EBX = handler
          "c"(index)        // ECX = index
          : "memory"
    );
}
