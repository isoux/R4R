/*
 * R4R License: MIT
 *
 * - Indentation: 4 spaces
 *
 * kernels/core/core_call_gates.c
 *
 * (C) Copyright 2025 Isa <isa@isoux.org>
 */

#include <typedef.h>
#include <gdt_sys.h>
#include <sys.h>

// External prototypes
extern void gdt_call_gate_set(u16, void (*)(void), u8);
extern void cg_entry_gdt_set(void);
extern void cg_entry_printr(void);

/* Shared continuation pointer lives in core binary */
void (*core_resume_ptr)(void) = 0;

/*
 * Ring0 call-gate stub used for "resume to core" flow.
 * Contract:
 *   - Caller (RingX) passes continuation address in EAX (param_count=0)
 *   - Stub must NOT clobber EAX before the jump
 *   - Set DS/ES to CORE_DATA, then tail-jump to continuation
 */
__attribute__((naked)) void cg_core_resume_stub(void) {
    __asm__ __volatile__(
        // Use BX to load DS/ES so AX/EAX (continuation) is not altered
        "mov  $" STR(CORE_DATA) ", %bx  \n\t"
        "mov  %bx, %ds;  mov %bx, %es   \n\t"
        "jmp  *%eax                     \n\t"// tail-jump to continuation
        // Execution never reaches here after the jump
    );
}

void setup_core_call_gates(void) {
    //gdt_call_gate_set(CG_CORE_TX_IRQ, syscall_tx_irq, 1);
    gdt_call_gate_set(CG_CORE_PRINTR, cg_entry_printr, 0);
    gdt_call_gate_set(CG_GDT_SET, cg_entry_gdt_set, 0);
    gdt_call_gate_set(CG_CORE_RESUME, cg_core_resume_stub, 0);
}
