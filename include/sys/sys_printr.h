/*
 * R4R License: MIT
 *
 * - Indentation: 4 spaces
 *
 * include/sys/sys_printr.h
 *
 * (C) Copyright 2025 Isa <isa@isoux.org>
 */

#include <typedef.h>

__attribute__((always_inline))
static inline void syscall_printr(const char *msg, u8 color)
{
    __asm__ __volatile__ (
        "xor %%ecx, %%ecx\n\t"                   // ECX = 0 → fills both CH and CL with 0
        "lcall $" STR(CG_CORE_PRINTR) ", $0\n\t" // far call via call gate selector
        :
        : "a"((u32)color),   // eax
          "b"((u32)msg)      // ebx
        : "ecx", "memory"
    );
}

__attribute__((always_inline))
static inline void syscall_printr_at(const char *msg, u8 color, u8 row, u8 col)
{
    u16 cx_val = ((u16)col << 8) | row;  // CH=col, CL=row

    __asm__ __volatile__ (
        "lcall $" STR(CG_CORE_PRINTR) ", $0\n\t"   // far call via call gate selector
        :
        : "a"((u32)color),  // eax
          "b"((u32)msg),    // ebx
          "c"(cx_val)       // cx = (CH:CL) = (col:row)
        : "memory"
    );
}
