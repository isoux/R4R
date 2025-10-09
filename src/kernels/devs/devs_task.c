/*
 * R4R License: MIT
 *
 * - Indentation: 4 spaces
 *
 * kernels/devs/devs_task.c
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

extern void devs_irq_task(void);
extern void devs_sched_task(void);

__attribute__((used, aligned(16)))
u64 ldt_devs[LDT_ENTRIES] = {0};

__attribute__((used, aligned(16)))
struct tss32 tss_devs_irq = {0};

__attribute__((used, aligned(16)))
struct tss32 tss_devs_sched = {0};

u32 ring0_irq_stack[STACK_SIZE];
struct stack_ptr r0_irq_stack = { & ring0_irq_stack[STACK_SIZE]};

u32 task_irq_stack[STACK_SIZE];
struct stack_ptr stack_irq = { & task_irq_stack[STACK_SIZE]};

u32 ring0_sched_stack[STACK_SIZE];
struct stack_ptr r0_sched_stack = { & ring0_sched_stack[STACK_SIZE]};

u32 task_sched_stack[STACK_SIZE];
struct stack_ptr stack_sched = { & task_sched_stack[STACK_SIZE]};

static inline void ldt_set_desc(u32 index, u64 descriptor) {
    u64 *ldt_table = ldt_devs;
    if (index < LDT_ENTRIES) {
        ldt_table[index] = descriptor;
    }
}

void ldt_fill_table(void) {

    u32 base = DEVS_BASE;

    /// ***** Ring 1 Code and Data for LDT *****
    u32 limit = DEVS_LIMIT;;
    // Access byte: Code Segment, Ring 1, Selector 0x05, Present
    u8 access = ACCESS_BYTE(CODE_EXECUTE_READ, RING_1, PRESENT);
    u8 flags = FLAG_32_BIT | FLAG_GRAN_4K;
    u64 descriptor = make_gdt_descriptor(base, limit, access, flags);
    ldt_set_desc(0, descriptor);
    // Access byte: Data Segment, Ring 1, Selector 0x0D, Present
    access = ACCESS_BYTE(DATA_READ_WRITE, RING_1, PRESENT);
    flags = FLAG_32_BIT | FLAG_GRAN_4K;
    descriptor = make_gdt_descriptor(base, limit, access, flags);
    ldt_set_desc(1, descriptor);
}

u64 set_devs_tss_ldt_desc(u8 sys_seg_type, u32 base, u32 limit){

    u8 access = ACCESS_BYTE(sys_seg_type, RING_1, PRESENT);
    u8 flags = FLAG_GRAN_BYTE;
    u64 descriptor = make_gdt_descriptor(base, limit, access, flags);
    return descriptor;
}

void setup_tss_devs_irq_struct(void){
    tss_devs_irq.ring0_st_seg = CORE_DATA;
    tss_devs_irq.ring0_stack = (u32)r0_irq_stack.end_stack; // ptr → u32 (ESP0)
    tss_devs_irq.task_stack = (u32)stack_irq.end_stack;     // ESP
    tss_devs_irq.debug_trap = 0;
    tss_devs_irq.io_map_base  = 0xFFFF;                     // core: disable IOPB
    tss_devs_irq.task = (u32)&devs_irq_task;                // EIP
    tss_devs_irq.cs = DEVS_LDT_CODE; tss_devs_irq._res_cs = 0;
    tss_devs_irq.ss = DEVS_LDT_DATA; tss_devs_irq._res_ss = 0;
    tss_devs_irq.ds = DEVS_LDT_DATA; tss_devs_irq._res_ds = 0;
    tss_devs_irq.es = DEVS_LDT_DATA; tss_devs_irq._res_es = 0;
    tss_devs_irq.fs = DEVS_LDT_DATA; tss_devs_irq._res_fs = 0;
    tss_devs_irq.gs = DEVS_ACCES_DATA; tss_devs_irq._res_gs = 0;
    tss_devs_irq.eflags = 0x00001000; // IF=0 IOPL=1
    tss_devs_irq.ldt = LDT_DEVS; tss_devs_irq._res_ldt = 0;
}

void setup_tss_devs_sched_struct(void){
    tss_devs_sched.ring0_st_seg = CORE_DATA;
    tss_devs_sched.ring0_stack = (u32)r0_sched_stack.end_stack; // ptr → u32 (ESP0)
    tss_devs_sched.task_stack = (u32)stack_sched.end_stack;     // ESP
    tss_devs_sched.debug_trap = 0;
    tss_devs_sched.io_map_base  = 0xFFFF;                       // core: disable IOPB
    tss_devs_sched.task = (u32)&devs_sched_task;                // EIP
    tss_devs_sched.cs = DEVS_LDT_CODE; tss_devs_sched._res_cs = 0;
    tss_devs_sched.ss = DEVS_LDT_DATA; tss_devs_sched._res_ss = 0;
    tss_devs_sched.ds = DEVS_LDT_DATA; tss_devs_sched._res_ds = 0;
    tss_devs_sched.es = DEVS_LDT_DATA; tss_devs_sched._res_es = 0;
    tss_devs_sched.fs = DEVS_LDT_DATA; tss_devs_sched._res_fs = 0;
    tss_devs_sched.gs = DEVS_ACCES_DATA; tss_devs_sched._res_gs = 0;
    tss_devs_sched.eflags = 0x00001200; // IF=1 IOPL=1
    tss_devs_sched.ldt = LDT_DEVS; tss_devs_sched._res_ldt = 0;
}

void setup_devs_tasks(void) {
    u64 desc;
    u32 base;
    u32 limit = (u32)sizeof(struct tss32) - 1; // byte granularity for TSS

    // TSS_DEVS_IRQ; Access byte: TSS Segment, Ring 1, Selector 0x58
    base  = (u32)&tss_devs_irq;
    desc = set_devs_tss_ldt_desc(SYS_TSS_AVAILABLE, base, limit);
    syscall_gdt_desc_set(TSS_DEVS_IRQ, desc);

    // TSS_DEVS_SCHED; Access byte: TSS Segment, Ring 1, Selector 0x68
    base  = (u32)&tss_devs_sched;
    desc = set_devs_tss_ldt_desc(SYS_TSS_AVAILABLE, base, limit);
    syscall_gdt_desc_set(TSS_DEVS_SCHED, desc);

    ldt_fill_table();

    // LDT_DEVS
    // Access byte: LDT Segment, Ring 1, Selector 0x60
    base  = (u32)(u32)ldt_devs;
    limit = (u32)(LDT_ENTRIES * 8) - 1;
    desc = set_devs_tss_ldt_desc(SYS_LDT, base, limit);
    syscall_gdt_desc_set(LDT_DEVS, desc);

    setup_tss_devs_irq_struct();
    setup_tss_devs_sched_struct();
}
