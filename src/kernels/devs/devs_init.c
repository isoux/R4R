/*
 * R4R License: MIT
 *
 * - Indentation: 4 spaces
 *
 * kernels/devs/devs_init.c
 *
 * (C) Copyright 2025 Isa <isa@isoux.org>
 */

#include <typedef.h>
#include <gdt_sys.h>
#include <core/core_resume.h>
#include <sys/sys_gdt.h>
#include <sys/sys_printr.h>
#include <hw/vga_colors.h>

#define DEVS_COLOR  (FG_YELLOW | BG_BLACK)

extern void setup_devs_call_gates(void);
extern void setup_devs_tasks(void);
extern void setup_devs_idt(void);


void print_R1_msg(void) {
    syscall_printr(
        "R1: Hello from Ring 1, DEVS init is successfully done!\n",
        DEVS_COLOR
    );
}

__attribute__((section(".text.devs_entry")))
void setup_devs(void) {
    u32 core_cont;

    core_cont = set_resume();

    setup_devs_call_gates();
    setup_devs_tasks();
    setup_devs_idt();
    // ...

    print_R1_msg();

    // Ring0: cg_core_resume_stub → jmp *EAX
    core_resume(core_cont);
    for (;;) { /* not reached */
    }
}
