/*
 * R4R License: MIT
 *
 * - Indentation: 4 spaces
 *
 * kernels/core/core_task.c
 *
 * (C) Copyright 2025 Isa <isa@isoux.org>
 */

#include <typedef.h>
#include <gdt_sys.h>
#include <gdt/gdt_build.h>
#include <sys.h>
#include <task.h>
#include <ldt.h>

#define LDT_ENTRIES 2

extern void gdt_ldt_set(u16 selector, u32 base, u32 limit);
extern void gdt_tss_set(u16 selector, struct tss32 *tss);

__attribute__((used, aligned(16)))
 struct tss32 tss_core = { 0 };

// The reason for hardcoding the stack pointer here is that it’s placed at the end
// of the core milli-kernel. After the iret frames from the init phase, there is no
// valid stack for the core. By setting it here within the task, it becomes the active
// ring 0 stack without needing manual switching it from LPR (lower priv. ring) returns.
// It also serves as the default kernel stack for code outside of tasks.
u32 r0_stack = CORE_STACK;

u32 task_stack[256];
struct stack_ptr stack =
    { &task_stack[256] };

__attribute__((used, aligned(16)))
 u64 ldt_core[LDT_ENTRIES] =
    { 0 };

// Set descriptor into LDT table at specified index
static inline void ldt_set_descriptor(u32 index, u64 descriptor) {
    u64 *ldt_table = ldt_core;
    if (index < LDT_ENTRIES) {
        ldt_table[index] = descriptor;
    }
}

void ldt_fill_table(void) {

    u32 base = CORE_BASE;

    /// ***** Ring 0 Code and Data for LDT *****
    u32 limit = MEM_LIMIT;
    // Access byte: Code Segment, Ring 0, Selector 0x08, Present
    u8 access = ACCESS_BYTE(CODE_EXECUTE_READ, RING_0, PRESENT);
    u8 flags = FLAG_32_BIT | FLAG_GRAN_4K;
    u64 descriptor = make_gdt_descriptor(base, limit, access, flags);
    ldt_set_descriptor(0, descriptor);
    // Access byte: Data Segment, Ring 0, Selector 0x10, Present
    access = ACCESS_BYTE(DATA_READ_WRITE, RING_0, PRESENT);
    flags = FLAG_32_BIT | FLAG_GRAN_4K;
    descriptor = make_gdt_descriptor(base, limit, access, flags);
    ldt_set_descriptor(1, descriptor);
}

static inline void lldt_set(u16 sel) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "lldt %0\n\t"
        ".att_syntax prefix\n"
        :
        : "r"(sel)
          : "memory"
    );
}

static inline u16 read_ldtr_sel(void) {
    u16 sel;
    __asm__ __volatile__(
        ".intel_syntax noprefix\n\t"
        "sldt %0\n\t"
        ".att_syntax prefix\n"
        : "=r"(sel)
          :
          : "memory"
    );
    return sel;
}

static inline void ltr_load(u16 sel) {
    __asm__ __volatile__("ltr %0" : : "r"(sel) : "memory");
}

void core_ldt_activate(void) {
    u32 base = (u32) (u32) ldt_core;
    u32 limit = (u32) (LDT_ENTRIES * 8) - 1;

    // patch  LDT system descriptor at GDT-у
    gdt_ldt_set(LDT_CORE, base, limit);

    // load LDTR
    lldt_set(LDT_CORE);
}

void core_main_task(void) {

    for (;;) {
        ;
    }
}

void setup_tss_core_struct(void) {
    tss_core.ring0_st_seg = CORE_DATA;
    tss_core.ring0_stack = r0_stack;                 // ptr → u32 (ESP0)
    tss_core.task_stack = (u32) stack.end_stack;     // ESP
    tss_core.debug_trap = 0;
    tss_core.io_map_base = 0xFFFF;                   // core: disable IOPB
    tss_core.task = (u32) &core_main_task;           // EIP
    tss_core.cs = CORE_LDT_CODE;
    tss_core._res_cs = 0;
    tss_core.ss = CORE_LDT_DATA;
    tss_core._res_ss = 0;
    tss_core.ds = CORE_LDT_DATA;
    tss_core._res_ds = 0;
    tss_core.es = CORE_LDT_DATA;
    tss_core._res_es = 0;
    tss_core.fs = CORE_LDT_DATA;
    tss_core._res_fs = 0;
    tss_core.gs = CORE_LDT_DATA;
    tss_core._res_gs = 0;
    tss_core.eflags = 0x00000200;                    // IF=1 IOPL=0
    tss_core.ldt = LDT_CORE;
    tss_core._res_ldt = 0;
}

__attribute__((always_inline))
static inline void enter_core_task(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "ljmp " STR(TSS_CORE) ":0 \n\t"
        ".att_syntax prefix"
    );
}

// use at core_init.c
void setup_core_main_task(void) {
    setup_tss_core_struct();
    ldt_fill_table();
    core_ldt_activate();
    gdt_tss_set(TSS_CORE, &tss_core);
    ltr_load(TSS_CORE);
    //enter_core_task();
}

