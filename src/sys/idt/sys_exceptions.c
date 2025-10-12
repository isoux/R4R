/*
 * R4R License: MIT
 *
 * - Indentation: 4 spaces
 *
 * sys/idt/sys_exceptions.c
 *
 * System exception handlers (0–31).
 * Each handler prints detailed information about the fault,
 * trap, or abort that occurred. Execution halts afterward.
 *
 * (C) Copyright 2021–2025 Isa <isa@isoux.org>
 */

#include "sys_exceptions.h"

void sys_print_color(const char* msg, u8 color) {
    syscall_printr(msg, color);
}

void sys_int_0(void) {
    sys_print_color(
        "FAULT: 0 |0x00| #DE | **Divide Error**\n"
        "Caused by DIV or IDIV instruction division by zero.\n",
        COLOR
    );
    while (1);
}

void sys_int_1(void) {
    sys_print_color(
        "TRAP: 1 |0x01| #DB | **Debug Exception**\n"
        "Single-step, data breakpoint, or debug register trigger.\n",
        COLOR
    );
    while (1);
}

void sys_int_2(void) {
    sys_print_color(
        "INTERRUPT: 2 |0x02| NMI | **Non-Maskable Interrupt**\n"
        "Asynchronous hardware-triggered interrupt.\n",
        COLOR
    );
    while (1);
}

void sys_int_3(void) {
    sys_print_color(
        "TRAP: 3 |0x03| #BP | **Breakpoint**\n"
        "Triggered by the INT3 instruction for debugging.\n",
        COLOR
    );
    while (1);
}

void sys_int_4(void) {
    sys_print_color(
        "TRAP: 4 |0x04| #OF | **Overflow**\n"
        "Caused by INTO instruction when OF flag set.\n",
        COLOR
    );
    while (1);
}

void sys_int_5(void) {
    sys_print_color(
        "FAULT: 5 |0x05| #BR | **BOUND Range Exceeded**\n"
        "BOUND instruction detected index outside bounds.\n",
        COLOR
    );
    while (1);
}

void sys_int_6(void) {
    sys_print_color(
        "FAULT: 6 |0x06| #UD | **Invalid Opcode**\n"
        "Processor detected undefined or illegal instruction.\n",
        COLOR
    );
    while (1);
}

void sys_int_7(void) {
    sys_print_color(
        "FAULT: 7 |0x07| #NM | **Device Not Available**\n"
        "FPU unavailable or TS flag set in CR0.\n",
        COLOR
    );
    while (1);
}

void sys_int_8(void) {
    sys_print_color(
        "ABORT: 8 |0x08| #DF | **Double Fault**\n"
        "Exception occurred during handling of another exception.\n",
        COLOR
    );
    while (1);
}

void sys_int_9(void) {
    sys_print_color(
        "FAULT: 9 |0x09| **Coprocessor Segment Overrun**\n"
        "(Floating-point instruction) Legacy 286 error, ignored on modern CPUs.\n",
        COLOR
    );
    while (1);
}

void sys_int_10(void) {
    sys_print_color(
        "FAULT: 10 |0x0A| #TS | **Invalid TSS**\n"
        "Task State Segment access or limit violation.\n",
        COLOR
    );
    while (1);
}

void sys_int_11(void) {
    sys_print_color(
        "FAULT: 11 |0x0B| #NP | **Segment Not Present**\n"
        "Segment present flag (P) cleared in descriptor.\n",
        COLOR
    );
    while (1);
}

void sys_int_12(void) {
    sys_print_color(
        "FAULT: 12 |0x0C| #SS | **Stack Segment Fault**\n"
        "Stack selector or segment limit invalid.\n",
        COLOR
    );
    while (1);
}

void sys_int_13(void) {
    sys_print_color(
        "FAULT: 13 |0x0D| #GP | **General Protection Fault**\n"
        "Violation of segment descriptor privilege or limit.\n",
        COLOR
    );
    while (1);
}

void sys_int_14(void) {
    sys_print_color(
        "FAULT: 14 |0x0E| #PF | **Page Fault**\n"
        "Page not present or protection violation detected.\n",
        COLOR
    );
    while (1);
}

void sys_int_15(void) {
    sys_print_color(
        "RESERVED: 15 |0x0F| **Reserved**\n"
        "Intel reserved exception vector.\n",
        COLOR
    );
    while (1);
}

void sys_int_16(void) {
    sys_print_color(
        "FAULT: 16 |0x10| #MF | **x87 FPU Floating-Point Error**\n"
        "Numeric overflow, underflow, or precision problem.\n",
        COLOR
    );
    while (1);
}

void sys_int_17(void) {
    sys_print_color(
        "FAULT: 17 |0x11| #AC | **Alignment Check**\n"
        "Unaligned memory access (only in Ring 3).\n",
        COLOR
    );
    while (1);
}

void sys_int_18(void) {
    sys_print_color(
        "ABORT: 18 |0x12| #MC | **Machine Check**\n"
        "Hardware-detected internal CPU error.\n",
        COLOR
    );
    while (1);
}

void sys_int_19(void) {
    sys_print_color(
        "FAULT: 19 |0x13| #XM | **SIMD Floating-Point Exception**\n"
        "Numeric error in SSE/AVX operation.\n",
        COLOR
    );
    while (1);
}

void sys_int_20(void) {
    sys_print_color(
        "FAULT: 20 |0x14| #VE | **Virtualization Exception**\n"
        "VMX-root violation in virtualized environment.\n",
        COLOR
    );
    while (1);
}

void sys_int_21(void) {
    sys_print_color(
        "FAULT: 21 |0x15| #CP | **Control Protection Exception**\n"
        "Control-flow enforcement (CET) violation.\n",
        COLOR
    );
    while (1);
}

/* Intel reserved 22–31 */
void sys_int_22(void) { sys_print_color("RESERVED: 22 |0x16| **Reserved**\n", COLOR); while (1); }
void sys_int_23(void) { sys_print_color("RESERVED: 23 |0x17| **Reserved**\n", COLOR); while (1); }
void sys_int_24(void) { sys_print_color("RESERVED: 24 |0x18| **Reserved**\n", COLOR); while (1); }
void sys_int_25(void) { sys_print_color("RESERVED: 25 |0x19| **Reserved**\n", COLOR); while (1); }
void sys_int_26(void) { sys_print_color("RESERVED: 26 |0x1A| **Reserved**\n", COLOR); while (1); }
void sys_int_27(void) { sys_print_color("RESERVED: 27 |0x1B| **Reserved**\n", COLOR); while (1); }
void sys_int_28(void) { sys_print_color("RESERVED: 28 |0x1C| **Reserved**\n", COLOR); while (1); }
void sys_int_29(void) { sys_print_color("RESERVED: 29 |0x1D| **Reserved**\n", COLOR); while (1); }
void sys_int_30(void) { sys_print_color("RESERVED: 30 |0x1E| **Reserved**\n", COLOR); while (1); }
void sys_int_31(void) { sys_print_color("RESERVED: 31 |0x1F| **Reserved**\n", COLOR); while (1); }


