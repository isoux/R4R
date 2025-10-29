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

#include "users_task.h"

#define PROMPT_COLOR (FG_GREEN | BG_BLACK)
#define SYS_COLOR    (FG_BLACK | BG_GREEN)

void print_main_task_msg(void) {
    syscall_printr_at(
        "SYS is ready and waiting from USERS main TASK!!!",
        SYS_COLOR, 22, 31
    );
}

// We don't have syscall_putc yet,
// so for now each character is printed as a null-terminated string.
void print_char(char c) {
    char msg[2];
    msg[0] = c;
    msg[1] = 0;
    syscall_printr(msg, PROMPT_COLOR);
}

void print_prompt(void) {
    syscall_printr(
        "R4R<:>", PROMPT_COLOR
    );
}


void users_main_task(void) {
    u32 *ptr_r1_stack;

    print_main_task_msg();
    print_prompt();

    // Basic event loop (or event/message queue) mechanism.
    // For now, it only monitors keyboard input and simply prints
    // the received character to the primary console
    // through a system call executed in ring 0.
    while (1) {
        // EAX contains a pointer to the stack location
        // where the ASCII key value from the interrupt is stored.
        __asm__ volatile ("movl %%eax, %0" : "=r"(ptr_r1_stack));

        if (ptr_r1_stack && *ptr_r1_stack) {
            print_char(*(char *)ptr_r1_stack);
            *ptr_r1_stack = 0;
        }

        __asm__ volatile("pause");
    }
}

