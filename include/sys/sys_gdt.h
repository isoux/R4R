/*
 * R4R License: MIT
 *
 * - Indentation: 4 spaces
 *
 * include/sys/sys_gdt.h
 *
 * (C) Copyright 2025 Isa <isa@isoux.org>
 *
 * API Notes:
 * ==========
 * This header provides system call wrappers for lower-privilege rings
 * (Ring1–Ring3) to request Ring0 services via the call gate `CG_GDT_SET`.
 *
 * All functions here are `inline` assembly stubs that load arguments
 * into registers (EAX, EBX, ECX, EDX) and then issue an `lcall` through
 * the call gate. The Ring0 handler (`cg_entry_gdt_set`) will branch to
 * the correct service routine depending on the descriptor type in EDX.
 *
 * Available syscalls:
 *
 *   1) syscall_call_gate_set(selector, handler, param)
 *      - EAX = param_count
 *      - EBX = handler_addr
 *      - ECX = selector
 *      - EDX = CG_DESC
 *      → Installs a Call Gate descriptor
 *
 *   2) syscall_tss_desc_set(selector, tss_ptr)
 *      - EAX = struct tss32*
 *      - EBX = selector
 *      - EDX = TSS_DESC
 *      → Installs a TSS descriptor
 *
 *   3) syscall_ldt_desc_set(selector, base, limit)
 *      - EAX = limit
 *      - EBX = base
 *      - ECX = selector
 *      - EDX = LDT_DESC
 *      → Installs an LDT descriptor
 *
 * Design note:
 * ------------
 * This approach demonstrates how a **single call gate** can serve as a
 * multiplexer for multiple system services. By encoding the operation
 * type in EDX, different privileged actions can be dispatched from one
 * entry point. This follows Intel’s guidance that call gates are more
 * flexible and efficient than trap-based syscall mechanisms.
 */

#include <task.h>
#include <gdt/gdt_types.h>

/* Set placeholder for indexed CALL GATE descriptor at GDT
 * from lower privileged rings.
 * This function calls call_gate.
 */
__attribute__((always_inline))
static inline void syscall_call_gate_set(u16 selector, void (*handler)(void), u8 param)
{
    __asm__ __volatile__ (
        "lcall $" STR(CG_GDT_SET) ", $0\n\t"   // far call via call gate selector
        :
        : "a"((u32)param),     // eax
          "b"(handler),        // ebx
          "c"((u32)selector),  // ecx
          "d"((u32)CG_DESC)    // edx <- descriptor type
        : "memory"
    );
}

/* Set placeholder for indexed call TSS descriptor at GDT
 * from lower privileged rings.
 * This function calls call_gate.
 */
__attribute__((always_inline))
static inline void syscall_tss_desc_set(u16 selector, struct tss32 *tss)
{
    __asm__ __volatile__ (
        "lcall $" STR(CG_GDT_SET) ", $0\n\t"   // far call via call gate selector
        :
        : "a"((u32)tss),       // eax
          "b"((u32)selector),  // ebx
          "d"((u32)TSS_DESC)   // edx <- descriptor type
        : "memory"
    );
}

/* Set placeholder for indexed call LDT descriptor at GDT
 * from lower privileged rings.
 * This function calls call_gate.
 */
__attribute__((always_inline))
static inline void syscall_ldt_desc_set(u16 selector, u32 base, u32 limit)
{
    __asm__ __volatile__ (
        "lcall $" STR(CG_GDT_SET) ", $0\n\t"   // far call via call gate selector
        :
        : "a"((u32)limit),     // eax
          "b"((u32)base),      // ebx
          "c"((u32)selector),  // ecx
          "d"((u32)LDT_DESC)   // edx <- descriptor type
        : "memory"
    );
}

/* Set descriptor at GDT. If index is 0
 * core automatically finds free index at GDT.
 * This function calls call_gate from lower privileged rings.
 */
__attribute__((always_inline))
static inline void syscall_gdt_desc_set(u16 selector, u64 descriptor) {
    // Split 64-bit descriptor into two 32-bit parts
    u32 low  = (u32)(descriptor & 0xFFFFFFFFULL);      // lower 32 bits
    u32 high = (u32)((descriptor >> 32) & 0xFFFFFFFF); // upper 32 bits

    // Force the constant DESC64 into EDX by binding a C variable
    // directly to the "edx" register. This guarantees GCC will emit
    // a `mov $imm, %edx` instead of routing through another register.
    register u32 edx_val __asm__("edx") = (u32)DESC64;

    __asm__ __volatile__ (
        "lcall $" STR(CG_GDT_SET) ", $0\n\t"   // far call via call gate
        :
        : "a"(low),           // EAX = lower 32 bits of descriptor
          "b"(high),          // EBX = upper 32 bits of descriptor
          "c"((u32)selector), // ECX = selector
          "r"(edx_val)        // EDX = descriptor type
          : "memory"
    );
}
