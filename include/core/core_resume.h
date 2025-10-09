/*
 * R4R License: MIT
 *
 * - Indentation: 4 spaces
 *
 * include/core_resume.h
 *
 * (C) Copyright 2025 Isa <isa@isoux.org>
 */

#pragma once
#include <typedef.h>


__attribute__((always_inline))
static inline void core_resume(u32 cont) {
    __asm__ __volatile__ (
        "lcall $" STR(CG_CORE_RESUME) ", $0"
        :
        : "a"(cont)   // cont → EAX
        : "memory"
    );
}

__attribute__((always_inline))
static inline u32 set_resume(void) {
    u32 out;
    __asm__ __volatile__ (
        // SS already holds the known segment for ringX
        // These two instructions set the data segment
        // before the function setup_devs_call_gates
        "movw  %%ss, %%bx     \n\t"
        "movw  %%bx, %%ds     \n\t"
        "movw  %%bx, %%es     \n\t"
        // EAX already contains the return function address
        : "=a"(out)               // output: out = EAX
        :                         // no inputs
        : "ebx", "cc", "memory"   // clobbers
    );
    return out;
}
