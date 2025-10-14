/*
 * R4R License: MIT
 *
 * - Indentation: 4 spaces
 *
 * kernels/core/core_init.c
 *
 * (C) Copyright 2025 Isa <isa@isoux.org>
 */
#include <sys.h>
#include <core/core_print.h>
#include <core/core_textio.h>

extern void setup_sys_interrupts(void);
extern void setup_core_call_gates(void);
extern void setup_core_main_task(void);

__attribute__((section(".text.core_entry")))
void setup_core(void){
    setup_sys_interrupts();
    setup_core_call_gates();
    setup_core_main_task();
    // ...
    textio_init();
    core_print(
        "Hello from Ring 0, CORE init is successfully done!\n"
    );
}

