/*
 * R4R License: MIT
 *
 * - Indentation: 4 spaces
 *
 * kernels/users/users_init.c
 *
 * (C) Copyright 2025 Isa <isa@isoux.org>
 */

#include <typedef.h>
#include <gdt_sys.h>
#include <core/core_resume.h>
#include <sys/sys_gdt.h>
#include <sys/sys_printr.h>
#include <hw/vga_colors.h>

extern void setup_users_tasks(void);

#define USERS_COLOR  (FG_LBLUE | BG_BLACK)
#define LOGO_COLOR   (FG_WHITE | BG_BLACK)

void print_color(const char* msg, u8 color) {
    syscall_printr(msg, color);
}

void print_color_at(const char *msg, u8 color, u8 row, u8 col) {
    syscall_printr_at(msg, color, row, col);
}

void print_R3_msg (void) {

    print_color(
        "R3: Hello from Ring 3, USERS init is successfully done!\n",
        USERS_COLOR
    );

    print_color(
        "  _       _\n"
        " |_| |_| |_| -> 0.00 version!\n"
        " | \\   | | \\ <- 2025 year!  |\n\n",
        LOGO_COLOR
    );
}

__attribute__((section(".text.users_entry")))
void setup_users(void){

    u32 core_cont = set_resume();

    setup_users_tasks();
    // ...
    print_R3_msg();

    // Ring0: cg_core_resume_stub → jmp *EAX
    core_resume(core_cont);
    for(;;) { /* not reached */ }
}

