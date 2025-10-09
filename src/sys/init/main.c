/*
 * R4R License: MIT
 *
 * - Indentation: 4 spaces
 *
 * main.c
 *
 * Formatting note (this file):
 * - Indentation: 4 spaces (no tabs)
 *
 * Initialization philosophy:
 *     - Each ring (1–3) is entered via an IRET frame from Ring 0.
 *     - A continuation address (where Ring 0 should resume) is placed in EAX
 *       before IRET; general-purpose registers survive the privilege change.
 *     - After a ring finishes its own init, it returns control to Ring 0 via a
 *       Call Gate (CG_CORE_RESUME). The Ring 0 stub tail-jumps to EAX
 *       (continuation), preserving a clean control flow and privilege boundary.
 *
 * Each ring (1–3) is entered explicitly via an IRET frame
 * to ensure a safe and legitimate initialization path.
 *
 * The idea is that every mili-kernel (ring-specific domain)
 * performs its own initialization inside its privilege level,
 * instead of being bootstrapped entirely by ring 0.
 *
 * A "mili-kernel" is the minimal kernel entity for each ring,
 * providing its own scheduler, IRQ handling, and isolation
 * within its domain.
 *
 * From there, each ring can invoke ring 0 services through
 * call gates (syscalls) to request GDT updates, patch system
 * descriptors, or configure global parameters.
 *
 * This approach guarantees:
 *  - clear privilege separation from the start
 *  - legitimate ownership of initialization inside each ring
 *  - full traceability of how modules were initialized
 *  - protection against illegitimate or malicious patches
 *
 * Core (ring 0) only provides the entry points and call gate
 * services. Rings 1–3 take responsibility for their own setup,
 * ensuring a modular and auditable initialization sequence.
 *
 * (C) Copyright 2025 Isa <isa@isoux.org>
 */

#include <init.h>
#include <typedef.h>
#include <gdt_sys.h>
#include <sys.h>

#define EIFLAGS_R1 0
#define EIFLAGS_R2 0
#define EIFLAGS_R3 0

/* Helper: return address as u32 (readability) */
__attribute__((always_inline))
static inline u32 core_set_resume(void (*fn)(void)) {
    return (u32)(uintptr_t)fn;
}

static void after_devs(void);
static void after_libs(void);
static void after_users(void);

__attribute__((always_inline))
static inline void setup_core(void) {
    __asm__ __volatile__ (
        "lcall $" STR(CORE_CODE) ", $" STR(CORE_START)
        ::: "memory"
    );
}

/* General IRET with continuation in EAX */
__attribute__((always_inline))
static inline void iret_to_ringX_with_cont(u32 eip, u16 cs, u32 eflags,
    u32 esp, u16 ss, u32 cont)
{
    u32 cs32 = (u32)cs;
    u32 ss32 = (u32)ss;

    __asm__ __volatile__ (
        /* put continuation into EAX */
        /* variant 1 (shorter): use "a"(cont) constraint only */
        "pushl %4 \n\t"   /* SS'  */
        "pushl %3 \n\t"   /* ESP' */
        "pushl %2 \n\t"   /* EFLAGS */
        "pushl %1 \n\t"   /* CS'  */
        "pushl %0 \n\t"   /* EIP' */
        "iret     \n\t"
        :
        : "r"(eip), "r"(cs32), "r"(eflags), "r"(esp), "r"(ss32),
          "a"(cont)              /* EAX = cont */
        : "memory"
    );
}

void setup_devs(u32 cont) {
    iret_to_ringX_with_cont(DEVS_START, DEVS_CODE, EIFLAGS_R1,
        DEVS_STACK, DEVS_DATA, cont);
}

void setup_libs(u32 cont) {
    iret_to_ringX_with_cont(LIBS_START, LIBS_CODE, EIFLAGS_R2,
        LIBS_STACK, LIBS_DATA, cont);
}

void setup_users(u32 cont) {
    iret_to_ringX_with_cont(USERS_START, USERS_CODE, EIFLAGS_R3,
        USERS_STACK, USERS_DATA, cont);
}

void main(void){
    setup_core();

    u32 cont = core_set_resume(after_devs);
    setup_devs(cont);

    trap_halt();
}

/* Here you continue when Ring1 calls CG_CORE_RESUME */
static void after_devs(void) {
    u32 cont = core_set_resume(after_libs);
    setup_libs(cont);

    trap_halt();  // not reached
}

static void after_libs(void) {
    u32 cont = core_set_resume(after_users);
    setup_users(cont);

    trap_halt();  // not reached
}

static void after_users(void) {

    struct {
        u32 func_ptr;
        u16 selector;
    } __attribute__((packed)) far_jmp_args = {
        .func_ptr = 0,
        .selector = TSS_MAIN_TASK
    };

    // From this point onward, the context switch jumps permanently into the
    // user-space main task (Ring 3). All kernels and rings initialization phases
    // have been completed, and execution continues in the main user task.
    __asm__ volatile ("ljmp *%0" : : "m"(far_jmp_args));

    trap_halt();  // not reached
}
