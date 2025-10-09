/*
 * R4R License: MIT
 *
 * - Indentation: 4 spaces
 *
 * include/gdt/gdt_desc_type.h
 *
 * (C) Copyright 2025 Isa <isa@isoux.org>
 */

#ifndef GDT_DESC_TYPE_H
#define GDT_DESC_TYPE_H

#include <typedef.h>

// Segment descriptor types (lower 4 bits of type field in access byte)
typedef u8 gdt_seg_type_t;

enum {
    // Data Segment Types
    GDT_DATA_RO                = 0x00, // Read-Only
    GDT_DATA_RO_ACCESSED       = 0x01,
    GDT_DATA_RW                = 0x02, // Read/Write
    GDT_DATA_RW_ACCESSED       = 0x03,
    GDT_DATA_RO_EXP_DOWN       = 0x04, // Expand-Down
    GDT_DATA_RO_EXP_DOWN_ACC   = 0x05,
    GDT_DATA_RW_EXP_DOWN       = 0x06,
    GDT_DATA_RW_EXP_DOWN_ACC   = 0x07,

    // Code Segment Types
    GDT_CODE_XO                = 0x18, // Execute-Only
    GDT_CODE_XO_ACCESSED       = 0x19,
    GDT_CODE_XR                = 0x1A, // Execute/Read
    GDT_CODE_XR_ACCESSED       = 0x1B,
    GDT_CODE_XO_CONF           = 0x1C, // Conforming
    GDT_CODE_XO_CONF_ACC       = 0x1D,
    GDT_CODE_XR_CONF           = 0x1E,
    GDT_CODE_XR_CONF_ACC       = 0x1F
};

// System descriptor types (used when S = 0)
typedef u8 gdt_sys_type_t;

enum {
    GDT_SYS_RESERVED_00        = 0x00,
    GDT_SYS_TSS_16_AVAIL       = 0x01,
    GDT_SYS_LDT                = 0x02,
    GDT_SYS_TSS_16_BUSY        = 0x03,
    GDT_SYS_CALL_GATE_16       = 0x04,
    GDT_SYS_TASK_GATE          = 0x05,
    GDT_SYS_INT_GATE_16        = 0x06,
    GDT_SYS_TRAP_GATE_16       = 0x07,
    GDT_SYS_RESERVED_08        = 0x08,
    GDT_SYS_TSS_32_AVAIL       = 0x09,
    GDT_SYS_RESERVED_0A        = 0x0A,
    GDT_SYS_TSS_32_BUSY        = 0x0B,
    GDT_SYS_CALL_GATE_32       = 0x0C,
    GDT_SYS_RESERVED_0D        = 0x0D,
    GDT_SYS_INT_GATE_32        = 0x0E,
    GDT_SYS_TRAP_GATE_32       = 0x0F
};

// Descriptor privilege levels (DPL field)
typedef u8 gdt_dpl_t;

enum {
    GDT_RING_0 = 0x00,
    GDT_RING_1 = 0x20,
    GDT_RING_2 = 0x40,
    GDT_RING_3 = 0x60
};

#endif // GDT_DESC_TYPE_H

