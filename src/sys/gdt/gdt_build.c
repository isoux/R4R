/*
 * R4R License: MIT
 *
 * - Indentation: 4 spaces
 *
 * gdt_build.c
 *
 * This file provides the implementation of the Global Descriptor Table (GDT) setup
 * for the R4R kernel(s) project. It defines system-wide GDT entries for code and data
 * segments across all four privilege rings (Ring 0 to Ring 3), including special
 * system segments such as TSS and LDT descriptors.
 *
 * Additionally, it initializes call gate descriptors to enable controlled privilege
 * transitions (e.g., Ring 3 to Ring 0/1/2), with placeholders for dynamic offset
 * patching at runtime. This static setup ensures a clean base for task management
 * and system call mechanisms, while allowing future runtime expansion for task-specific
 * entries.
 *
 * The GDT table is allocated with the maximum possible size of 8192 descriptors,
 * to support a large number of kernel and user-level tasks, as well as system gates
 * across all privilege levels without the need for resizing.
 *
 * (C) Copyright 2025 Isa <isa@isoux.org>
 */

#include <gdt/gdt_build.h>

/// Set descriptor into GDT table at specified index
static inline void gdt_set_descriptor(u32 index, u64 descriptor) {
    u64 *gdt_table = (u64*) GDT_START;
    if (index < GDT_ENTRIES) {
        gdt_table[index] = descriptor;
    }
}

void gdt_fill_table(void) {

    u32 base = CORE_BASE;

    /// ----- CODE & DATA descriptors for general 4 RINGS -----

    /// ***** Ring 0 Code and Data *****
    u32 limit = MEM_LIMIT;
    // Access byte: Code Segment, Ring 0, Selector 0x08, Present
    u8 access = ACCESS_BYTE(CODE_EXECUTE_READ, RING_0, PRESENT);
    u8 flags = FLAG_32_BIT | FLAG_GRAN_4K;
    u64 descriptor = make_gdt_descriptor(base, limit, access, flags);
    gdt_set_descriptor(1, descriptor);
    // Access byte: Data Segment, Ring 0, Selector 0x10, Present
    access = ACCESS_BYTE(DATA_READ_WRITE, RING_0, PRESENT);
    flags = FLAG_32_BIT | FLAG_GRAN_4K;
    descriptor = make_gdt_descriptor(base, limit, access, flags);
    gdt_set_descriptor(2, descriptor);

    // ***** Ring 1 Code and Data *****
    limit = DEVS_LIMIT;
    // Access byte: Code Segment, Ring 1, Selector 0x18, Present
    access = ACCESS_BYTE(CODE_EXECUTE_READ, RING_1, PRESENT);
    flags = FLAG_32_BIT | FLAG_GRAN_4K;
    descriptor = make_gdt_descriptor(base, limit, access, flags);
    gdt_set_descriptor(3, descriptor);
    // Access byte: Data Segment, Ring 1, Selector 0x20, Present
    access = ACCESS_BYTE(DATA_READ_WRITE, RING_1, PRESENT);
    flags = FLAG_32_BIT | FLAG_GRAN_4K;
    descriptor = make_gdt_descriptor(base, limit, access, flags);
    gdt_set_descriptor(4, descriptor);

    // ***** Ring 2 Code and Data *****
    limit = LIBS_LIMIT;
    // Access byte: Code Segment, Ring 2, Selector 0x28, Present
    access = ACCESS_BYTE(CODE_EXECUTE_READ, RING_2, PRESENT);
    flags = FLAG_32_BIT | FLAG_GRAN_4K;
    descriptor = make_gdt_descriptor(base, limit, access, flags);
    gdt_set_descriptor(5, descriptor);
    // Access byte: Data Segment, Ring 2, Selector 0x30, Present
    access = ACCESS_BYTE(DATA_READ_WRITE, RING_2, PRESENT);
    flags = FLAG_32_BIT | FLAG_GRAN_4K;
    descriptor = make_gdt_descriptor(base, limit, access, flags);
    gdt_set_descriptor(6, descriptor);

    // ***** Ring 3 Code and Data. *****
    // Only for the main task in the user space!
    limit = USERS_SYS_LIMIT;
    // Access byte: Code Segment, Ring 3, Selector 0x38, Present
    access = ACCESS_BYTE(CODE_EXECUTE_READ, RING_3, PRESENT);
    flags = FLAG_32_BIT | FLAG_GRAN_4K;
    descriptor = make_gdt_descriptor(base, limit, access, flags);
    gdt_set_descriptor(7, descriptor);
    // Access byte: Data Segment, Ring 3, Selector 0x40, Present
    access = ACCESS_BYTE(DATA_READ_WRITE, RING_3, PRESENT);
    flags = FLAG_32_BIT | FLAG_GRAN_4K;
    descriptor = make_gdt_descriptor(base, limit, access, flags);
    gdt_set_descriptor(8, descriptor);

    /// ----- System segment descriptors -----

    // ***** RING 0 TASK descriptors *****
    limit = 1024;
    // TSS_CORE
    // Access byte: TSS Segment, Ring 0, Selector 0x48, Present
    access = ACCESS_BYTE(SYS_TSS_AVAILABLE, RING_0, PRESENT);
    flags = FLAG_GRAN_BYTE;
    descriptor = make_gdt_descriptor(base, limit, access, flags);
    gdt_set_descriptor(9, descriptor);
    // LDT_CORE
    // Access byte: LDT Segment, Ring 0, Selector 0x50, Present
    access = ACCESS_BYTE(SYS_LDT, RING_0, PRESENT);
    flags = FLAG_GRAN_BYTE;
    descriptor = make_gdt_descriptor(base, limit, access, flags);
    gdt_set_descriptor(10, descriptor);

    // ***** RING 1 TASKS and LDT descriptors *****
    //
    // The remaining 3 descriptors for primary tasks and LDT tables
    // are populated from Ring 1 using syscalls through call gates
    // (to core = ring 0) during the early system init phase.

    // DEVS_ACCES_DATA used for GS register
    // To access interrupts from devs  to core.
    // Access byte: Data Segment, Ring 1, Selector 0xF8, Present
    // Limit must be core limit = memory limit
    limit = MEM_LIMIT;
    access = ACCESS_BYTE(DATA_READ_WRITE, RING_1, PRESENT);
    flags = FLAG_32_BIT | FLAG_GRAN_4K;
    descriptor = make_gdt_descriptor(base, limit, access, flags);
    gdt_set_descriptor(14, descriptor);

    // ***** RING 2 TASKS descriptors *****
    // The remaining 3 descriptors for primary tasks and LDT tables
    // are populated from Ring 2 using syscalls through call gates
    // (to core = ring 0) during the early system init phase.

    // ***** RING 3 TASKS descriptors *****
    limit = 0;
    // TSS_MAIN_TASK
    // Access byte: TSS Segment, Ring 3, Selector 0x98, Present
    access = ACCESS_BYTE(SYS_TSS_AVAILABLE, RING_3, PRESENT);
    flags = FLAG_GRAN_BYTE;
    descriptor = make_gdt_descriptor(base, limit, access, flags);
    gdt_set_descriptor(19, descriptor);
    // LDT_USERS
    // Access byte: LDT Segment, Ring 3, Selector 0xA0, Present
    access = ACCESS_BYTE(SYS_LDT, RING_3, PRESENT);
    flags = FLAG_GRAN_BYTE;
    descriptor = make_gdt_descriptor(base, limit, access, flags);
    gdt_set_descriptor(20, descriptor);
    // TSS_USERS_TASK
    // Access byte: TSS Segment, Ring 3, Selector 0xA8, Present
    access = ACCESS_BYTE(SYS_TSS_AVAILABLE, RING_3, PRESENT);
    flags = FLAG_GRAN_BYTE;
    descriptor = make_gdt_descriptor(base, limit, access, flags);
    gdt_set_descriptor(21, descriptor);

    // USRS_ACCES_DATA
    // To access interrupts from users to devs.
    // Access byte: Data Segment, Ring 3, Selector 0xF0, Present
    // Limit must be devs limit
    limit = DEVS_LIMIT;
    access = ACCESS_BYTE(DATA_READ_WRITE, RING_3, PRESENT);
    flags = FLAG_32_BIT | FLAG_GRAN_4K;
    descriptor = make_gdt_descriptor(base, limit, access, flags);
    gdt_set_descriptor(22, descriptor);

    // RESERVED
    /*
     gdt_set_descriptor(23, 0); Selector 0xB8
     gdt_set_descriptor(24, 0); Selector 0xC0
     gdt_set_descriptor(25, 0); Selector 0cC8
     gdt_set_descriptor(26, 0); Selector 0xD0
     gdt_set_descriptor(27, 0); Selector 0xD8
     gdt_set_descriptor(28, 0); Selector 0xE0
     gdt_set_descriptor(29, 0); Selector 0xE8
     gdt_set_descriptor(30, 0); Selector 0xF0
     gdt_set_descriptor(31, 0); Selector 0xF8
     */

    /// CALL GATES
    // CG_CORE_TX_IRQ selector 0x100 decs. for RING 0 from RING 3
    u8 type = SYS_CALL_GATE;
    u16 selector = CORE_CODE;
    u8 dpl = DPL_RING_3;
    u8 count = 1;
    u32 offset = 0; // Placeholder for now, function will be filled in later
    descriptor = make_call_gate_descriptor(selector, offset, dpl, type, count);
    gdt_set_descriptor(32, descriptor);
    // CG_CORE_PRINTR selector 0x108 decs. for RING 0 from RING 3
    // Descriptor stays the same, only the function pointer changes
    gdt_set_descriptor(33, descriptor);
    // CG_LIBS_TX_IRQ selector 0x110 decs. for RING 2 from RING 3
    selector = LIBS_CODE;
    dpl = DPL_RING_3;
    descriptor = make_call_gate_descriptor(selector, offset, dpl, type, count);
    gdt_set_descriptor(34, descriptor);
    // CG_DEVS_TTY_W  selector 0x118 decs. for RING 1 from RING 2
    selector = DEVS_CODE;
    dpl = DPL_RING_2;
    count = 3;
    descriptor = make_call_gate_descriptor(selector, offset, dpl, type, count);
    gdt_set_descriptor(35, descriptor);
    // CG_GDT_SET selector 0x120 desc. for RING 0 from RING 3
    selector = CORE_CODE;
    dpl = DPL_RING_3;
    count = 0;
    descriptor = make_call_gate_descriptor(selector, offset, dpl, type, count);
    gdt_set_descriptor(36, descriptor);
    // CG_CORE_RESUME  selector 0x128 desc. for RING 0 from RING 3
    selector = CORE_CODE;
    dpl = DPL_RING_3;
    count = 0;
    descriptor = make_call_gate_descriptor(selector, offset, dpl, type, count);
    gdt_set_descriptor(37, descriptor);
}
