/*
 * R4R License: MIT
 *
 * - Indentation: 4 spaces
 *
 * include/gdt/gdt_defs.h
 *
 * Enum definitions and descriptor generator for GDT entries
 *
 * (C) Copyright 2025 Isa <isa@isoux.org>
 */

#ifndef __GDT_DEFS_H__
#define __GDT_DEFS_H__

#include <typedef.h> // for u8, u16, u32, u64

// Segment Descriptor Types (used when S = 1)
typedef enum {
    // Data Segment Types
    DATA_READ_ONLY        = 0x10, // Read-only data
    DATA_RO_ACCESSED      = 0x11,
    DATA_READ_WRITE       = 0x12, // Read/write data
    DATA_RW_ACCESSED      = 0x13,
    DATA_EXPAND_DOWN      = 0x14, // Expand-down read-only
    DATA_RO_EXP_DOWN_ACC  = 0x15,
    DATA_EXPAND_DOWN_RW   = 0x16, // Expand-down read/write
    DATA_RW_EXP_DOWN_ACC  = 0x07,

    // Code Segment Types
    CODE_EXECUTE_ONLY     = 0x18, // Execute-only code
    CODE_XO_ACCESSED      = 0x19,
    CODE_EXECUTE_READ     = 0x1A, // Execute/read code
    CODE_XR_ACCESSED      = 0x1B,
    CODE_CONFORMING       = 0x1C, // Conforming execute-only
    CODE_XO_CONF_ACC      = 0x1D,
    CODE_CONF_READ        = 0x1E,  // Conforming execute/read
    CODE_XR_CONF_ACC      = 0x1F
} gdt_segment_type_t;

// System Segment Types (used when S = 0)
typedef enum {
    SYS_RESERVED_00       = 0x00,
    SYS_TSS_16_AVAIL      = 0x01,
    SYS_LDT               = 0x02, // Local Descriptor Table
    SYS_TSS_16_BUSY       = 0x03,
    SYS_CALL_GATE_16      = 0x04,
    SYS_TASK_GATE         = 0x05,
    SYS_INT_GATE_16       = 0x06,
    SYS_TRAP_GATE_16      = 0x07,
    SYS_RESERVED_08       = 0x08,
    SYS_TSS_AVAILABLE     = 0x09, // Available 32-bit TSS
    SYS_TSS_BUSY          = 0x0B, // Busy 32-bit TSS
    SYS_RESERVED_0A       = 0x0A,
    SYS_TSS_32_BUSY       = 0x0B,
    SYS_CALL_GATE         = 0x0C, // Call gate
    SYS_RESERVED_0D       = 0x0D,
    SYS_INTERRUPT_GATE    = 0x0E, // Interrupt gate
    SYS_TRAP_GATE         = 0x0F  // Trap gate
} gdt_system_type_t;

// Descriptor Privilege Levels
typedef enum {
    RING_0 = 0x00, // Kernel
    RING_1 = 0x20,
    RING_2 = 0x40,
    RING_3 = 0x60  // User
} gdt_dpl_t;

// Present bit
typedef enum {
    NOT_PRESENT = 0x00,
    PRESENT     = 0x80
} gdt_present_t;;

// Flags (high nibble of byte 6)
typedef enum {
    FLAG_16_BIT     = 0x00, // 16-bit segment
    FLAG_32_BIT     = 0x40, // 32-bit segment
    FLAG_64_BIT     = 0x20, // 64-bit code segment (only in IA-32e mode)
    FLAG_GRAN_BYTE  = 0x00, // Limit is in bytes
    FLAG_GRAN_4K    = 0x80  // Limit is in 4 KiB pages
} gdt_flags_t;

/// Macro to construct the access byte
#define ACCESS_BYTE(type, dpl, present) ((u8)((present) | (type) | (dpl)))
/// Macro to construct the flags byte
#define FLAGS_BYTE(flags) ((u8)(flags))

/// Function to build a 64-bit GDT descriptor
static inline u64 make_gdt_descriptor(u32 base, u32 limit,
    u8 access, u8 flags) {
    u64 descriptor = 0;

    descriptor  =  (limit & 0xFFFFULL);                // Limit bits 0–15
    descriptor |= ((u64)(base & 0xFFFFFF) << 16);      // Base bits 0–23
    descriptor |= ((u64)access        << 40);          // Access byte
    descriptor |= (((u64)(limit >> 16) & 0x0F) << 48); // Limit bits 16–19
    descriptor |= ((u64)(flags & 0xF0) << 48);         // Flags (upper nibble)
    descriptor |= ((u64)(base >> 24)   << 56);         // Base bits 24–31

    return descriptor;
}

/// Function to build a 64-bit GDT gate descriptor
static inline u64 make_call_gate_descriptor(u16 selector, u32 offset,
    u8 dpl, u8 type, u8 count) {
    // Format:
    // Bits 0–15: 0 offset[0..15]
    // Bits 16–31: selector
    // Bits 32–39: param count
    // Bits 40–47: type (0xC) | DPL | P
    // Bits 48–63: offset[16..31]

    u64 desc = 0;
    desc  = (offset & 0xFFFFULL);                       // offset low
    desc |= ((u64)selector << 16);                      // segment selector
    desc |= ((u64)(count & 0b00011111) << 32);          // param count
    desc |= ((u64)(type | (dpl << 5) | PRESENT) << 40); // type + dpl + present
    desc |= ((u64)(offset >> 16) << 48);                // offset high
    return desc;
}

#endif // __GDT_DEFS_H__

