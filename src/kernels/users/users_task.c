/*
 * R4R License: MIT
 *
 * - Indentation: 4 spaces
 *
 * kernels/users/users_task.c
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

extern void users_main_task(void);
extern void users_nested_task(void);

__attribute__((used, aligned(16)))
u64 ldt_users[LDT_ENTRIES] = {0};

__attribute__((used, aligned(16)))
struct tss32 tss_users_main = {0};

__attribute__((used, aligned(16)))
struct tss32 tss_users_nested = {0};

u32 ring0_main_stack[STACK_SIZE];
struct stack_ptr r0_main_stack = { & ring0_main_stack[STACK_SIZE]};

u32 ring1_main_stack[STACK_SIZE];
struct stack_ptr r1_main_stack = { & ring1_main_stack[STACK_SIZE]};

u32 ring2_main_stack[STACK_SIZE];
struct stack_ptr r2_main_stack = { & ring2_main_stack[STACK_SIZE]};

u32 task_main_stack[STACK_SIZE];
struct stack_ptr stack_main = { & task_main_stack[STACK_SIZE]};

u32 ring0_nested_stack[STACK_SIZE];
struct stack_ptr r0_nested_stack = { & ring0_nested_stack[STACK_SIZE]};

u32 ring1_nested_stack[STACK_SIZE];
struct stack_ptr r1_nested_stack = { & ring1_nested_stack[STACK_SIZE]};

u32 ring2_nested_stack[STACK_SIZE];
struct stack_ptr r2_nested_stack = { & ring2_nested_stack[STACK_SIZE]};

u32 task_nested_stack[STACK_SIZE];
struct stack_ptr stack_nested = { & task_nested_stack[STACK_SIZE]};

static inline void ldt_set_desc(u32 index, u64 descriptor) {
    u64 *ldt_table = ldt_users;
    if (index < LDT_ENTRIES) {
        ldt_table[index] = descriptor;
    }
}

void ldt_fill_table(void) {

    u32 base = USERS_BASE;

    /// ***** Ring 3 Code and Data for LDT *****
    u32 limit = USERS_SYS_LIMIT;
    // Access byte: Code Segment, Ring 3, Selector 0x07, Present
    u8 access = ACCESS_BYTE(CODE_EXECUTE_READ, RING_3, PRESENT);
    u8 flags = FLAG_32_BIT | FLAG_GRAN_4K;
    u64 descriptor = make_gdt_descriptor(base, limit, access, flags);
    ldt_set_desc(0, descriptor);
    // Access byte: Data Segment, Ring 3, Selector 0x0F, Present
    access = ACCESS_BYTE(DATA_READ_WRITE, RING_3, PRESENT);
    flags = FLAG_32_BIT | FLAG_GRAN_4K;
    descriptor = make_gdt_descriptor(base, limit, access, flags);
    ldt_set_desc(1, descriptor);
}

u64 set_users_tss_ldt_desc(u8 sys_seg_type, u32 base, u32 limit){
    u8 access = ACCESS_BYTE(sys_seg_type, RING_3, PRESENT);
    u8 flags = FLAG_GRAN_BYTE;
    u64 descriptor = make_gdt_descriptor(base, limit, access, flags);
    return descriptor;
}

void setup_tss_users_main_struct(void){
    tss_users_main.ring0_st_seg = CORE_DATA;
    tss_users_main.ring0_stack = (u32)r0_main_stack.end_stack; // ptr → u32 (ESP0)
    tss_users_main.ring1_st_seg = DEVS_DATA;
    tss_users_main.ring1_stack = (u32)r1_main_stack.end_stack; // ptr → u32 (ESP1)
    tss_users_main.ring2_st_seg = LIBS_DATA;
    tss_users_main.ring2_stack = (u32)r2_main_stack.end_stack; // ptr → u32 (ESP3)
    tss_users_main.task_stack = (u32)stack_main.end_stack;     // ESP
    tss_users_main.debug_trap = 0;
    tss_users_main.io_map_base  = 0xFFFF;                      // core: disable IOPB
    tss_users_main.task = (u32)&users_main_task;               // EIP
    tss_users_main.cs = USERS_LDT_CODE; tss_users_main._res_cs = 0;
    tss_users_main.ss = USERS_LDT_DATA; tss_users_main._res_ss = 0;
    tss_users_main.ds = USERS_LDT_DATA; tss_users_main._res_ds = 0;
    tss_users_main.es = USERS_LDT_DATA; tss_users_main._res_es = 0;
    tss_users_main.fs = USERS_LDT_DATA; tss_users_main._res_fs = 0;
    tss_users_main.gs = USERS_LDT_DATA; tss_users_main._res_gs = 0;
    tss_users_main.eflags = 0x00001000; // IF=0 IOPL=1
    tss_users_main.ldt = LDT_USERS; tss_users_main._res_ldt = 0;
}

void setup_tss_users_nested_struct(void){
    tss_users_nested.ring0_st_seg = CORE_DATA;
    tss_users_nested.ring0_stack = (u32)r0_nested_stack.end_stack; // ptr → u32 (ESP0)
    tss_users_nested.ring1_st_seg = DEVS_DATA;
    tss_users_nested.ring1_stack = (u32)r1_nested_stack.end_stack; // ptr → u32 (ESP1)
    tss_users_nested.ring2_st_seg = LIBS_DATA;
    tss_users_nested.ring2_stack = (u32)r2_nested_stack.end_stack; // ptr → u32 (ESP2)
    tss_users_nested.task_stack = (u32)stack_nested.end_stack;     // ESP
    tss_users_nested.debug_trap = 0;
    tss_users_nested.io_map_base  = 0xFFFF;                        // core: disable IOPB
    tss_users_nested.task = (u32)&users_nested_task;               // EIP
    tss_users_nested.cs = USERS_LDT_CODE; tss_users_nested._res_cs = 0;
    tss_users_nested.ss = USERS_LDT_DATA; tss_users_nested._res_ss = 0;
    tss_users_nested.ds = USERS_LDT_DATA; tss_users_nested._res_ds = 0;
    tss_users_nested.es = USERS_LDT_DATA; tss_users_nested._res_es = 0;
    tss_users_nested.fs = USERS_LDT_DATA; tss_users_nested._res_fs = 0;
    tss_users_nested.gs = USERS_LDT_DATA; tss_users_nested._res_gs = 0;
    tss_users_nested.eflags = 0x00001200; // IF=1 IOPL=1
    tss_users_nested.ldt = LDT_USERS; tss_users_nested._res_ldt = 0;
}

void setup_users_tasks(void) {
    u64 desc;
    u32 base;
    u32 limit = (u32)sizeof(struct tss32) - 1; // byte granularity for TSS

    // TSS_MAIN_TASK; Access byte: TSS Segment, Ring 3, Selector 0x78
    base  = (u32)&tss_users_main;
    desc = set_users_tss_ldt_desc(SYS_TSS_AVAILABLE, base, limit);
    syscall_gdt_desc_set(TSS_MAIN_TASK, desc);

    // TSS_USERS_TASK; Access byte: TSS Segment, Ring 3, Selector 0x88
    base  = (u32)&tss_users_nested;
    desc = set_users_tss_ldt_desc(SYS_TSS_AVAILABLE, base, limit);
    syscall_gdt_desc_set(TSS_USERS_TASK, desc);

    ldt_fill_table();

    // LDT_USERS
    // Access byte: LDT Segment, Ring 3, Selector 0x80
    base  = (u32)(u32)ldt_users;
    limit = (u32)(LDT_ENTRIES * 8) - 1;
    desc = set_users_tss_ldt_desc(SYS_LDT, base, limit);
    syscall_gdt_desc_set(LDT_USERS, desc);

    setup_tss_users_main_struct();
    setup_tss_users_nested_struct();
}
