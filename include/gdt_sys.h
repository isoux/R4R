/*
 * R4R License: MIT
 *
 * - Indentation: 4 spaces
 *
 * include/gdt_sys.h
 *
 * Global Descriptor Table definitions.
 * All 8192 descriptors is used,
 * due to many future TASKS and CALL GATES.
 *
 * (C) Copyright 2021-2025 Isa <isa@isoux.org>
 */

#ifndef _GDT_SYS_H
#define _GDT_SYS_H

#define GDT_ENTRIES	(8192)	// number of entries (max is 8192)
#define ENTR_SIZE	(GDT_ENTRIES *8)
#define GDT_TABLE_SIZE    ENTR_SIZE

/// GDT Descriptors Selectors
#define DPL_RING_0	0
#define DPL_RING_1	1
#define DPL_RING_2	2
#define DPL_RING_3	3

#define CORE_BASE	0x00
#define CORE_CODE	0x08	// Ring 0 code desc
#define CORE_DATA	0x10	// Ring 0 data desc

#define DEVS_BASE   0x00
#define RING1_CODE	0x18	// Ring 1 code decs
#define RING1_DATA	0x20	// Ring 1 data decs

#define LIBS_BASE   0x00
#define RING2_CODE	0x28	// Ring 2 code decs
#define RING2_DATA	0x30	// Ring 2 data decs

#define USERS_BASE  0x00
#define RING3_CODE	0x38	// Ring 3 code decs
#define RING3_DATA	0x40	// Ring 3 data decs

/* RING 0 TASKS */
#define TSS_CORE	0x48
#define LDT_CORE	0x50
/* RING 1 TASKS */
#define TSS_DEVS_IRQ    0x58
#define LDT_DEVS        0x60
#define TSS_DEVS_SCHED  0x68
#define DEVS_ACCES_DATA 0x70
/* RING 2 TASKS */
#define TSS_LIBS_IRQ    0x78
#define LDT_LIBS        0x80
#define TSS_LIBS_SCHED  0x88
//#define USRS_ACCES_DATA 0x90
/* RING 3 TASKS */
#define TSS_MAIN_TASK   0x98
#define LDT_USERS       0xA0
#define TSS_USERS_TASK  0xA8
#define USRS_ACCES_DATA 0xB0

#define CG_IDT_SET      0xB8

/* Reserved
#define X 0xC0
#define X 0xC8
#define X 0xD0
#define X 0xD8
#define X 0xE0
#define X 0xE8
*/

/* Data access trough segment registers */
//  0xF0
//  0xF8

/* Call gates */
#define CG_CORE_TX_IRQ	0x100
#define CG_CORE_PRINTR	0x108
#define CG_LIBS_TX_IRQ	0x110
#define CG_DEVS_TTY_W   0x118
#define CG_GDT_SET      0x120
#define CG_CORE_RESUME  0x128

/// End GDT Descriptors Selectors

#define DEVS_CODE	RING1_CODE + DPL_RING_1 // DEVS sel. Ring1 code + 1 (dpl)
#define DEVS_DATA	RING1_DATA + DPL_RING_1 // DEVS sel. Ring1 data + 1 (dpl)
#define LIBS_CODE	RING2_CODE + DPL_RING_2 // LIBS sel. Ring2 code + 2 (dpl)
#define LIBS_DATA	RING2_DATA + DPL_RING_2 // LIBS sel. Ring2 data + 2 (dpl)
#define USERS_CODE	RING3_CODE + DPL_RING_3 // USERS sel. Ring3 code + 3 (dpl)
#define USERS_DATA	RING3_DATA + DPL_RING_3 // USERS sel. Ring3 data + 3 (dpl)

#define TSS_GDT(selector) ((tss_des_struct *) & gdt[selector >> 3])
#define TSS_BUSY(selector) ((TSS_GDT(selector)->type))

#endif /* _GDT_SYS_H */

