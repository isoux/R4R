/*
 * R4R License: MIT
 *
 * - Indentation: 4 spaces
 *
 * kernels/libs/libs_call_gates.c
 *
 * (C) Copyright 2025 Isa <isa@isoux.org>
 */

#include <typedef.h>
#include <gdt_sys.h>
#include <sys.h>
#include <gdt/gdt_types.h>
#include <sys/sys_gdt.h>
#include <gdt/gdt_build.h>

void libs_irq(void) {
    for (;;) {
        ;
    }
}

// Set call gate descriptor for devs
u64 set_libs_cg_desc(u8 dpl, void (*handler)(void), u8 count) {
    u8 type = SYS_CALL_GATE;
    u16 selector = LIBS_CODE;
    u32 offset = (u32) handler;
    u64 descriptor;
    descriptor = make_call_gate_descriptor(selector, offset, dpl, type, count);
    return descriptor;
}

void setup_libs_call_gates(void) {
    u64 desc;
    //CG_LIBS_TX_IRQ selector 0x110 decs. for RING 2 from RING 3
    desc = set_libs_cg_desc(DPL_RING_3, libs_irq, 0);
    syscall_gdt_desc_set(CG_LIBS_TX_IRQ, desc);

}
