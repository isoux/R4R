/*
 * R4R License: MIT
 *
 * - Indentation: 4 spaces
 *
 * gdt/gdt_types.h
 *
 * (C) Copyright 2025 Isa <isa@isoux.org>
 */

#ifndef GDT_TYPES_H
#define GDT_TYPES_H

// GDT descriptor types
#define CODE_DESC 0x9A // Code segment descriptor
#define DATA_DESC 0x92 // Data segment descriptor
#define LDT_DESC  0x82 // Local Descriptor Table (LDT) descriptor
#define TSS_DESC  0x89 // Task State Segment (TSS) descriptor
#define CG_DESC   0x8C // Call Gate descriptor
#define INT_DESC  0x8E // Interrupt Gate descriptor
#define TRAP_DESC 0x8F // Trap Gate descriptor

// More descriptor types can be added if needed.
#define DESC64 1 // Make gdt descriptor

#endif // GDT_TYPES_H
