/*
 * R4R License: MIT
 *
 * - Indentation: 4 spaces
 *
 * kernels/users/main_task.c
 *
 * (C) Copyright 2025 Isa <isa@isoux.org>
 */

#include <gdt_sys.h>
#include <sys/sys_printr.h>
#include <hw/vga_colors.h>

#define PROMPT_COLOR (FG_GREEN | BG_BLACK)
#define SYS_COLOR    (FG_BLACK | BG_GREEN)

void print_main_task_msg(void) {
    syscall_printr_at(
        "SYS is ready and waiting from USERS main TASK!!!",
        SYS_COLOR, 22, 31
    );
}

void print_prompt(void) {
    syscall_printr(
        "R4R<:>", PROMPT_COLOR
    );
}

void users_main_task(void) {

    print_main_task_msg();
    print_prompt();

    for (;;) {
        ;;
    }
}
