/*
 * R4R License: MIT
 *
 * - Indentation: 4 spaces
 *
 * kernels/devs/libs_init.c
 *
 * (C) Copyright 2025 Isa <isa@isoux.org>
 */

#include <typedef.h>
#include <gdt_sys.h>
#include <core/core_resume.h>
#include <sys/sys_gdt.h>
#include <sys/sys_printr.h>
#include <hw/vga_colors.h>

#define LIBS_COLOR  (FG_LCYAN| BG_BLACK)

extern void setup_libs_call_gates(void);
extern void setup_libs_tasks(void);

void print_R2_msg(void) {
    syscall_printr(
        "R2: Hello from Ring 2, LIBS init is successfully done!\n",
        LIBS_COLOR
    );
}

__attribute__((section(".text.libs_entry")))
void setup_libs(void) {
    u32 core_cont;

    core_cont = set_resume();

    setup_libs_call_gates();
    setup_libs_tasks();
    // ...

    print_R2_msg();

    // Ring0: cg_core_resume_stub → jmp *EAX
    core_resume(core_cont);
    for (;;) { /* not reached */
    }
}
