/*
 * R4R License: MIT
 *
 * - Indentation: 4 spaces
 *
 * include/init.h
 *
 * (C) Copyright 2025 Isa <isa@isoux.org>
 */

#include "typedef.h"

/// Structure layout must match Intel LSS operand (6 bytes)
struct __packed_ stack_descript {
    u32* ptr_end_stack;  // 4 bytes → will be moved into ESP
    u16  base;           // 2 bytes → will be moved into SS
};

__attribute__((always_inline))
static inline void setup_stack(struct stack_descript stack_desc) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "lss esp, [%0]\n\t"
        ".att_syntax prefix"
        :
        : "r" (&stack_desc)
          : "memory"
    );
}

__attribute__((always_inline))
static inline void disable_fpu(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "mov eax, cr0\n\t"              // Load the value of control register CR0 into EAX
        "and eax, 0x80000011\n\t"       // Mask to preserve PG, ET, and PE
        "test eax, 0x10\n\t"            // Check if ET is set (indicates FPU available)
        "jnz 1f\n\t"                    // If ET is set, skip setting EM
        "or eax, 4\n\t"                 // Otherwise, set EM to enable FPU emulation
        "1:\n\t"
        "mov cr0, eax\n\t"              // Write the modified value back to CR0
        ".att_syntax prefix\n\t"
    );
}

__attribute__((always_inline))
static inline void print_r4r(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"

        // Write 'R' (0x52) with green-on-black (0x02)
        "mov eax, 0x0252\n\t"
        "mov [0xb8016], ax\n\t"

        // Write '4' (0x34) with green-on-black (0x03)
        "mov eax, 0x0234\n\t"
        "mov [0xb8018], ax\n\t"

        // Write 'R' (0x52) again
        "mov eax, 0x0252\n\t"
        "mov [0xb801a], ax\n\t"

        ".att_syntax prefix"
    );
}


/*
 * This trap function should never be reached under normal execution.
 * The main() function, after completing initialization, is expected
 * to transfer control to the core kernel and never return here.
 */
__attribute__((always_inline))
static inline void trap_halt(void) {
    __asm__("1: jmp 1b");
}

