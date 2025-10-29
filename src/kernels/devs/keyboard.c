/*
 * R4R License: MIT
 *
 *  - Indentation: 4 spaces
 *
 * kernels/devs/keyboard.c
 *
 *
 * (C) Copyright 2021 2025 Isa <isa@isoux.org>
 */

#include <typedef.h>
#include <gdt_sys.h>

#include <hw/io.h>
#include <devs/interrupt.h>
#include "devs_irq.h"

__attribute__((always_inline))
static inline
void keyboard_reset(void) {
    // Read current keyboard controller state (port 0x61)
    u8 state = inb(0x61);

    // Set bit 7 (acknowledge pulse)
    outb(0x61, state | 0x80);

    // Clear bit 7 (restore original state)
    outb(0x61, state & 0x7F);

    // Send End-of-Interrupt (EOI) to PIC (IRQ1)
    outb(0x20, 0x20);
}

__attribute__((always_inline))
static inline
u8 keyborad_get_value(void) {
    u8 value = inb(0x60);
    return value;
}

__attribute__((always_inline))
static inline
void debug_interrupt(void) {
    // for bochs dbg
    __asm__ volatile ("nop \n\t");
}

__attribute__((always_inline))
static inline
void ret_from_interrupt(void) {
    __asm__ volatile ("iretl \n\t");
}

__attribute__((always_inline))
static inline
void register_irq(void) {

    // Register the interrupt in the devs_irq_task — the main task
    // responsible for handling all device interrupts in Devs.
    __asm__ volatile (
        // Subtract 0x20 to skip the first 32 CPU exceptions.
        "movl $"STR(KEY_INT)" - 0x20, %%gs:(%0)\n\t"
        // Get the stack address of the task that was active
        // when the interrupt occurred.
        // This is useful because, after the IRET instruction restores
        // the previous task context, that task already contains
        // the interrupt result (for example, an ASCII key from the keyboard).
        // The task can then fetch this value from its own ring1 stack.
        "movl 12(%%esp), %%eax\n\t"
        "movl %%eax, %%gs:(%1)\n\t"
        "lcall $" STR(TSS_DEVS_IRQ) ", $0\n\t"
        :
        : "r"(&tss_devs_irq.ebx), "r"(&tss_devs_irq.eax)
        : "memory"
    );
}

void keyboard_flush_buffer() {
    // Read status register at port 0x64
    // Bit 1 indicates if output buffer is full
    while (inb(0x64) & 0x01) {
        // Read scancode from port 0x60
        inb(0x60);
        // We discard the value here, just flushing the buffer
    }
}

// When the interrupt occurs, register_irq() invokes the nested task
// devs_irq_task, which by default keeps the interrupt flag disabled
// for a very short time — just enough to complete processing and
// call the corresponding device interrupt handler registered
// in the devs interrupt function table.
//
// This function (get_keyboard_int) is called from the event loop
// inside devs_irq_task, as the registered handler for the keyboard interrupt.
//
// After this function finishes, an IRET instruction exits
// the nested task and returns to the userspace task,
// where interrupts are re-enabled.
// The processing result is stored in the EAX register
// of this nested task, making it available to the user task.
__attribute__((naked))
void get_keyboard_int(void) {
    u8 *ptr_stack = (u8 *)tss_devs_irq.eax;
    char ascii_char;
    ascii_char = handle_key_press();
    *ptr_stack = ascii_char;
    keyboard_reset();
    // Required because this function is declared as naked
    __asm__ volatile ("ret");
}


char key_to_ascii(uint8_t scancode) {
    static const char scancode_to_ascii[] = {
        0, 27, '1','2','3','4','5','6','7','8','9','0','-','=','\b',     // 0x00–0x0E
        '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',0,     // 0x0F–0x1D
        'a','s','d','f','g','h','j','k','l',';','\'','`',                // 0x1E–0x29
        0,'\\','z','x','c','v','b','n','m',',','.','/',0,'*',0,' '       // 0x2A–0x39
    };

    if (scancode < sizeof(scancode_to_ascii)) {
        return scancode_to_ascii[scancode];
    }
    return 0;
}

char handle_key_press(void) {
    u8 scancode;
    char ascii_char;

    // Read scancode from the keyboard port
    scancode = keyborad_get_value();

    // Check if this is a break code (high bit set)
    if (scancode & 0x80) {
        // Break code: key released
        // Remove the high bit to get the original key code
        scancode &= 0x7F;
        return 0;
        // Here you handle key release
        // For example: mark the key as not pressed in your key state array

    } else {
        // Make code: key pressed
        // Handle key press here
        ascii_char = key_to_ascii(scancode);
        return ascii_char;
    }
}

__attribute__((naked))
void keyboard_interrupt(void) {

    debug_interrupt();     // for bochs dbg
    register_irq();
    ret_from_interrupt();
}
