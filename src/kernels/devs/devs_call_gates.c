/*
 * R4R License: MIT
 *
 * - Indentation: 4 spaces
 *
 * kernels/devs/devs_call_gates.c
 *
 * (C) Copyright 2025 Isa <isa@isoux.org>
 */

#include <typedef.h>
#include <gdt_sys.h>
#include <sys.h>
#include <gdt/gdt_types.h>
#include <sys/sys_gdt.h>
#include <gdt/gdt_build.h>

void devs_tty_write(void) {
    for (;;) {
        ;
    }
}

// Set call gate descriptor for devs
u64 set_devs_cg_desc(u8 dpl, void (*handler)(void), u8 count) {
    u8 type = SYS_CALL_GATE;
    u16 selector = DEVS_CODE;
    u32 offset = (u32) handler;
    u64 descriptor;
    descriptor = make_call_gate_descriptor(selector, offset, dpl, type, count);
    return descriptor;
}

void setup_devs_call_gates(void) {
    u64 desc;
    // CG_DEVS_TTY_W  selector 0x118 decs. for RING 1 from RING 2
    // desc = set_devs_cg_desc(DPL_RING_2, devs_tty_write, 3);
    desc = set_devs_cg_desc(DPL_RING_3, devs_tty_write, 0);
    syscall_gdt_desc_set(CG_DEVS_TTY_W, desc);

}

