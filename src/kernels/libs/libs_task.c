/*
 * R4R License: MIT
 *
 * - Indentation: 4 spaces
 *
 * kernels/libs/libs_task.c
 *
 * (C) Copyright 2025 Isa <isa@isoux.org>
 */

#include <typedef.h>
#include <gdt_sys.h>
#include <sys.h>
#include <task.h>
#include <ldt.h>
#include <gdt/gdt_types.h>
#include <gdt/gdt_build.h>
#include <sys/sys_gdt.h>

#define LDT_ENTRIES 2
#define STACK_SIZE 0x100

extern void libs_irq_task(void);
extern void libs_sched_task(void);

__attribute__((used, aligned(16)))
u64 ldt_libs[LDT_ENTRIES] = {0};

__attribute__((used, aligned(16)))
struct tss32 tss_libs_irq = {0};

__attribute__((used, aligned(16)))
struct tss32 tss_libs_sched = {0};

u32 ring0_irq_stack[STACK_SIZE];
struct stack_ptr r0_irq_stack = { & ring0_irq_stack[STACK_SIZE]};

u32 ring1_irq_stack[STACK_SIZE];
struct stack_ptr r1_irq_stack = { & ring1_irq_stack[STACK_SIZE]};

u32 task_irq_stack[STACK_SIZE];
struct stack_ptr stack_irq = { & task_irq_stack[STACK_SIZE]};

u32 ring0_sched_stack[STACK_SIZE];
struct stack_ptr r0_sched_stack = { & ring0_sched_stack[STACK_SIZE]};

u32 ring1_sched_stack[STACK_SIZE];
struct stack_ptr r1_sched_stack = { & ring1_sched_stack[STACK_SIZE]};

u32 task_sched_stack[STACK_SIZE];
struct stack_ptr stack_sched = { & task_sched_stack[STACK_SIZE]};

static inline void ldt_set_desc(u32 index, u64 descriptor) {
    u64 *ldt_table = ldt_libs;
    if (index < LDT_ENTRIES) {
        ldt_table[index] = descriptor;
    }
}

void ldt_fill_table(void) {

    u32 base = LIBS_BASE;

    /// ***** Ring 2 Code and Data for LDT *****
    u32 limit = LIBS_LIMIT;;
    // Access byte: Code Segment, Ring 1, Selector 0x06, Present
    u8 access = ACCESS_BYTE(CODE_EXECUTE_READ, RING_2, PRESENT);
    u8 flags = FLAG_32_BIT | FLAG_GRAN_4K;
    u64 descriptor = make_gdt_descriptor(base, limit, access, flags);
    ldt_set_desc(0, descriptor);
    // Access byte: Data Segment, Ring 1, Selector 0x0E, Present
    access = ACCESS_BYTE(DATA_READ_WRITE, RING_2, PRESENT);
    flags = FLAG_32_BIT | FLAG_GRAN_4K;
    descriptor = make_gdt_descriptor(base, limit, access, flags);
    ldt_set_desc(1, descriptor);
}

u64 set_libs_tss_ldt_desc(u8 sys_seg_type, u32 base, u32 limit){

    u8 access = ACCESS_BYTE(sys_seg_type, RING_2, PRESENT);
    u8 flags = FLAG_GRAN_BYTE;
    u64 descriptor = make_gdt_descriptor(base, limit, access, flags);
    return descriptor;
}

void setup_tss_libs_irq_struct(void){
    tss_libs_irq.ring0_st_seg = CORE_DATA;
    tss_libs_irq.ring0_stack = (u32)r0_irq_stack.end_stack; // ptr → u32 (ESP0)
    tss_libs_irq.ring1_st_seg = DEVS_DATA;
    tss_libs_irq.ring1_stack = (u32)r1_irq_stack.end_stack; // ptr → u32 (ESP1)
    tss_libs_irq.task_stack = (u32)stack_irq.end_stack;     // ESP
    tss_libs_irq.debug_trap = 0;
    tss_libs_irq.io_map_base  = 0xFFFF;                     // core: disable IOPB
    tss_libs_irq.task = (u32)&libs_irq_task;                // EIP
    tss_libs_irq.cs = LIBS_LDT_CODE; tss_libs_irq._res_cs = 0;
    tss_libs_irq.ss = LIBS_LDT_DATA; tss_libs_irq._res_ss = 0;
    tss_libs_irq.ds = LIBS_LDT_DATA; tss_libs_irq._res_ds = 0;
    tss_libs_irq.es = LIBS_LDT_DATA; tss_libs_irq._res_es = 0;
    tss_libs_irq.fs = LIBS_LDT_DATA; tss_libs_irq._res_fs = 0;
    tss_libs_irq.gs = LIBS_LDT_DATA; tss_libs_irq._res_gs = 0;
    tss_libs_irq.eflags = 0x00001000; // IF=0 IOPL=1
    tss_libs_irq.ldt = LDT_LIBS; tss_libs_irq._res_ldt = 0;
}

void setup_tss_libs_sched_struct(void){
    tss_libs_sched.ring0_st_seg = CORE_DATA;
    tss_libs_sched.ring0_stack = (u32)r0_sched_stack.end_stack; // ptr → u32 (ESP0)
    tss_libs_sched.ring1_st_seg = DEVS_DATA;
    tss_libs_sched.ring1_stack = (u32)r1_sched_stack.end_stack; // ptr → u32 (ESP1)
    tss_libs_sched.task_stack = (u32)stack_sched.end_stack;     // ESP
    tss_libs_sched.debug_trap = 0;
    tss_libs_sched.io_map_base  = 0xFFFF;                       // core: disable IOPB
    tss_libs_sched.task = (u32)&libs_sched_task;                // EIP
    tss_libs_sched.cs = LIBS_LDT_CODE; tss_libs_sched._res_cs = 0;
    tss_libs_sched.ss = LIBS_LDT_DATA; tss_libs_sched._res_ss = 0;
    tss_libs_sched.ds = LIBS_LDT_DATA; tss_libs_sched._res_ds = 0;
    tss_libs_sched.es = LIBS_LDT_DATA; tss_libs_sched._res_es = 0;
    tss_libs_sched.fs = LIBS_LDT_DATA; tss_libs_sched._res_fs = 0;
    tss_libs_sched.gs = LIBS_LDT_DATA; tss_libs_sched._res_gs = 0;
    tss_libs_sched.eflags = 0x00001200; // IF=1 IOPL=1
    tss_libs_sched.ldt = LDT_LIBS; tss_libs_sched._res_ldt = 0;
}

void setup_libs_tasks(void) {
    u64 desc;
    u32 base;
    u32 limit = (u32)sizeof(struct tss32) - 1; // byte granularity for TSS

    // TSS_LIBS_IRQ; Access byte: TSS Segment, Ring 2, Selector 0x78
    base  = (u32)&tss_libs_irq;
    desc = set_libs_tss_ldt_desc(SYS_TSS_AVAILABLE, base, limit);
    syscall_gdt_desc_set(TSS_LIBS_IRQ, desc);

    // TSS_LIBS_SCHED; Access byte: TSS Segment, Ring 2, Selector 0x88
    base  = (u32)&tss_libs_sched;
    desc = set_libs_tss_ldt_desc(SYS_TSS_AVAILABLE, base, limit);
    syscall_gdt_desc_set(TSS_LIBS_SCHED, desc);

    ldt_fill_table();

    // LDT_LIBS
    // Access byte: LDT Segment, Ring 2, Selector 0x80
    base  = (u32)(u32)ldt_libs;
    limit = (u32)(LDT_ENTRIES * 8) - 1;
    desc = set_libs_tss_ldt_desc(SYS_LDT, base, limit);
    syscall_gdt_desc_set(LDT_LIBS, desc);

    setup_tss_libs_irq_struct();
    setup_tss_libs_sched_struct();
}
