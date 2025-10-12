/*
 * R4R License: MIT
 *
 * - Indentation: 4 spaces
 *
 * sys/idt/sys_exceptions.h
 *
 * Declarations for system exception handlers (0–31).
 * Each handler corresponds to one reserved interrupt vector
 * and prints fault/trap/abort information for debugging.
 *
 * (C) Copyright 2021–2025 Isa <isa@isoux.org>
 */

#ifndef _SYS_EXCEPTIONS_H
#define _SYS_EXCEPTIONS_H

#include <typedef.h>
#include <gdt_sys.h>
#include <sys/sys_printr.h>
#include <hw/vga_colors.h>

#define COLOR (FG_RED | BG_BLACK)

/* Intel-defined exceptions: 0–31 */
void sys_int_0(void);   // Divide Error (#DE)
void sys_int_1(void);   // Debug Exception (#DB)
void sys_int_2(void);   // Non-Maskable Interrupt (NMI)
void sys_int_3(void);   // Breakpoint (#BP)
void sys_int_4(void);   // Overflow (#OF)
void sys_int_5(void);   // BOUND Range Exceeded (#BR)
void sys_int_6(void);   // Invalid Opcode (#UD)
void sys_int_7(void);   // Device Not Available (#NM)
void sys_int_8(void);   // Double Fault (#DF)
void sys_int_9(void);   // Coprocessor Segment Overrun (obsolete)
void sys_int_10(void);  // Invalid TSS (#TS)
void sys_int_11(void);  // Segment Not Present (#NP)
void sys_int_12(void);  // Stack Segment Fault (#SS)
void sys_int_13(void);  // General Protection Fault (#GP)
void sys_int_14(void);  // Page Fault (#PF)
void sys_int_15(void);  // Reserved
void sys_int_16(void);  // x87 FPU Floating-Point Error (#MF)
void sys_int_17(void);  // Alignment Check (#AC)
void sys_int_18(void);  // Machine Check (#MC)
void sys_int_19(void);  // SIMD Floating-Point Exception (#XM)
void sys_int_20(void);  // Virtualization Exception (#VE)
void sys_int_21(void);  // Control Protection Exception (#CP)
void sys_int_22(void);  // Reserved
void sys_int_23(void);  // Reserved
void sys_int_24(void);  // Reserved
void sys_int_25(void);  // Reserved
void sys_int_26(void);  // Reserved
void sys_int_27(void);  // Reserved
void sys_int_28(void);  // Reserved
void sys_int_29(void);  // Reserved
void sys_int_30(void);  // Reserved
void sys_int_31(void);  // Reserved

#endif /* _SYS_EXCEPTIONS_H */
