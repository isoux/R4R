/*
 * R4R License: MIT
 *
 * - Indentation: 4 spaces
 *
 * kernels/core/core_init.c
 *
 * (C) Copyright 2025 Isa <isa@isoux.org>
 */

#include <typedef.h>
#include <sys.h>
#include <core/core_resume.h>
#include <core/core_print.h>
#include <core/core_textio.h>

extern void setup_sys_interrupts(void);
extern void setup_core_call_gates(void);
extern void setup_core_main_task(void);
extern void clear_user_memory(void);
extern void enter_users_main_task(void);

void resume_sys_setup(void);

// Get value from eax register
__attribute__((always_inline))
static inline u32 is_sys_init(void) {
    u32 value;
    __asm__ volatile (
        "movl %%eax, %0"
        : "=r"(value)
    );
    return value;
}

// This function is called twice during the initialization phase:
// first from the init module, and second after all privilege rings
// have been initialized.
// The second call occurs via an IRET frame from sys_setup(), where
// the SYS_INIT value is passed through the EAX register.
// This indicates that further system initialization should proceed
// in the core (kernel) domain — including memory cleanup,
// configuration, interrupt setup, and finally transitioning into
// the user-space main task.
__attribute__((section(".text.core_entry")))
void setup_core(void){

    u32 sys_init;
    sys_init = is_sys_init();
    if (sys_init != SYS_INIT) {
        setup_sys_interrupts();
        setup_core_call_gates();
        setup_core_main_task();
        // ...
        textio_init();
        core_print(
            "Hello from Ring 0, CORE init is successfully done!\n"
        );
    } else {
        resume_sys_setup();
    }
}

void resume_sys_setup(void) {

    clear_user_memory();

    // From this point onward, the context switch jumps permanently into the
    // user-space main task (Ring 3).
    enter_users_main_task();

    while(1);
}

