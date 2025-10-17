/*
 * R4R License: MIT
 *
 * - Indentation: 4 spaces
 *
 * This is where all core settings are initialized after the main function from
 * the init domain jumps for the first time into the core_main_task in the system.
 *
 * kernels/core/sys_init.c
 *
 * (C) Copyright 2025 Isa <isa@isoux.org>
 */
#include <sys.h>

#define SYS_CLEAN_BASE 0x100000
#define SYS_CLEAN_SIZE ((USERS_START - SYS_CLEAN_BASE) / 4)


// Clears memory from 1MB up to USERS_START using 4-byte writes.
// This region previously held GRUB and INIT modules
// which are no longer needed.
void clear_user_memory(void) {
    __asm__ __volatile__ (
        "cld\n\t"                           // Clear direction flag
        "xor %%eax, %%eax\n\t"              // Set EAX = 0
        "mov %[dst], %%edi\n\t"             // EDI = destination address
        "mov %[count], %%ecx\n\t"           // ECX = number of DWORDs
        "rep stosl\n\t"                     // Store EAX into [EDI], ECX times
        :
        : [dst] "r"(SYS_CLEAN_BASE), [count] "r"(SYS_CLEAN_SIZE)
        : "eax", "edi", "ecx", "memory"
    );
}

//__attribute__((always_inline))
//static inline
void enter_users_main_task(void) {

    struct {
        u32 func_ptr;
        u16 selector;
    } __attribute__((packed)) far_jmp_args = {
        .func_ptr = 0,
        .selector = TSS_MAIN_TASK
    };

    // From this point onward, the context switch jumps permanently into the
    // user-space main task (Ring 3).
    __asm__ volatile ("ljmp *%0" : : "m"(far_jmp_args));
}
