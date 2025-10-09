/*
 * R4R License: MIT
 *
 * - Indentation: 4 spaces
 *
 * init.c
 *
 * init() performs the full system bootstrap sequence.
 * After main() passes control to the kernel task,
 * returning here would indicate a critical error.
 * trap_halt() ensures the CPU halts in a safe loop.
 *
 * (C) Copyright 2025 Isa <isa@isoux.org>
 */

#include <init.h>
#include <gdt/gdt_build.h>

extern void gdt_fill_table(void);
extern void gdt_init(void);
extern void setup_idt(void);
extern void setup_paging(void);
extern void main(void);

// Stack for initialization
u32 init_stack [1024>>4] = {0};

static struct stack_descript  stack_start = { 
    & init_stack[1024>>4],
    CORE_DATA 
};

__naked_ void init(void) {
    setup_stack(stack_start);
    gdt_zero_fill(GDT_START);
    gdt_fill_table();
    gdt_init();
    setup_idt();
    setup_paging();
    disable_fpu();
    print_r4r();
    main();
}



