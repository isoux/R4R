/*
 * R4R License: MIT
 *
 *  - Indentation: 4 spaces
 *
 * include/task.h
 *
 * (C) Copyright 2025 Isa <isa@isoux.org>
 */

#ifndef _TASK_H
#define _TASK_H

#include <typedef.h>

// Structure for setting stack pointers into tss structure
struct stack_ptr {
    u32 * end_stack;
};

struct tss32 {
    u16 back_link; u16 _res;
    u32 ring0_stack; u16 ring0_st_seg; u16 _res0;
    u32 ring1_stack; u16 ring1_st_seg; u16 _res1;
    u32 ring2_stack; u16 ring2_st_seg; u16 _res2;
    u32 cr3;
    u32 task;               // eip
    u32 eflags;
    u32 eax, ecx, edx, ebx;
    u32 task_stack;         // esp
    u32 ebp, esi, edi;
    u16 es, _res_es, cs, _res_cs;
    u16 ss, _res_ss, ds, _res_ds;
    u16 fs, _res_fs, gs, _res_gs;
    u16 ldt, _res_ldt;
    u16 debug_trap;
    u16 io_map_base;
} __attribute__((packed, aligned(16)));

#endif /* _TASK_H */
