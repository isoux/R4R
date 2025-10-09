/*
 * R4R License: MIT
 *
 * - Indentation: 4 spaces
 *
 * kernels/core/core_syscalls.c
 *
 * (C) Copyright 2025 Isa <isa@isoux.org>
 *
 * With this design, a single call gate provides multiple system calls,
 * demonstrating the versatility of call gates as highlighted in Intel’s
 * manuals. This approach is also faster than using trap gates (as Linux
 * initially did for syscalls), because it avoids additional exception
 * handling overhead.
 */

#include <typedef.h>
#include <gdt_sys.h>
#include <gdt/gdt_types.h>
#include <core/core_print.h>

extern void gdt_call_gate_set(u16, void (*)(void), u8);
extern void gdt_set_desc(u32 index, u64 descriptor);


/* Call-gate entry (Ring0).
 * Expects args passed in registers:
 *   EAX = param_count or tss or limit
 *   EBX = handler_addr or selector or base
 *   ECX = selector
 *   EDX = descriptor type
 * API Notes:
 * ==========
 * This single call-gate entry (`cg_entry_gdt_set`) provides three different
 * system services depending on the value passed in register EDX:
 *
 *   1) CALL_GATE service:
 *      EAX = param_count
 *      EBX = handler_addr
 *      ECX = selector
 *      EDX = CALL_GATE
 *      → Installs a call-gate descriptor via gdt_call_gate_set()
 *
 *   2) TSS_DESC service:
 *      EAX = struct tss32*
 *      EBX = selector
 *      EDX = TSS_DESC
 *      → Installs a TSS descriptor via gdt_tss_set()
 *
 *   3) LDT_DESC service:
 *      EAX = limit
 *      EBX = base
 *      ECX = selector
 *      EDX = LDT_DESC
 *      → Installs an LDT descriptor via gdt_ldt_set()
 */
__attribute__((naked)) void cg_entry_gdt_set(void)
{
    __asm__ __volatile__ (
        // Switch DS to core data segment using SI instead of AX
        "movw %ds, %di\n\t"                  // Save old DS in DI
        "movw $" STR(CORE_DATA) ", %si\n\t"  // Load new segment selector into SI
        "movw %si, %ds\n\t"                  // Move the new selector from SI to DS

        // Now args are already in EAX, EBX, ECX, EDX
        // EDX = descriptor type

        "cmp $" STR(DESC64) ", %edx\n\t"     // compare with DESC64 macro
        "jne 0f\n\t"                         // skip if not DESC64

        "pushl %ebx\n\t"                     // Push upper 32 bits of descriptor
        "pushl %eax\n\t"                     // Push lower 32 bits of descriptor
        "pushl %ecx\n\t"                     // Push index
        "call gdt_set_desc\n\t"              // Call the C function
        "addl  $12, %esp\n\t"                // Clean up the stack (3 arguments * 4 bytes)
        //"jmp 3f\n\t"

        // Speed Up Return
        // Restore DS
        "movw %di, %ds\n\t"                  // Restore DS from DI
        // Return to caller
        "lret \n\t"

        "0:\n\t"

        "cmp $" STR(CG_DESC) ", %edx\n\t"    // compare with CALL_GATE macro
        "jne 1f\n\t"                         // skip if not CALL_GATE

        "pushl %ecx\n\t"                     // Push selector
        "pushl %ebx\n\t"                     // Push handler_addr
        "pushl %eax\n\t"                     // Push param_count
        "call gdt_call_gate_set\n\t"         // Call the C function
        "addl  $12, %esp\n\t"                // Clean up the stack (3 arguments * 4 bytes)
        "jmp 3f\n\t"

        "1:\n\t"

        "cmp $" STR(TSS_DESC) ", %edx\n\t"   // compare with TSS_DESC macro
        "jne 2f\n\t"                         // skip if not TSS_DESC

        "pushl %ebx\n\t"                     // Push Push selector
        "pushl %eax\n\t"                     // Push struct tss32
        "call gdt_tss_set\n\t"               // Call the C function
        "addl  $8, %esp\n\t"                 // Clean up the stack (2 arguments * 4 bytes)
        "jmp 3f\n\t"

        "2:\n\t"

        "cmp $" STR(LDT_DESC) ", %edx\n\t"   // compare with LDT_DESC macro
        "jne 3f\n\t"                         // skip if not LDT_DESC

        "pushl %ecx\n\t"                     // Push selector
        "pushl %ebx\n\t"                     // Push base
        "pushl %eax\n\t"                     // Push limit
        "call gdt_ldt_set\n\t"               // Call the C function
        "addl  $12, %esp\n\t"                // Clean up the stack (3 arguments * 4 bytes)

        "3:\n\t"

        // Restore DS
        "movw %di, %ds\n\t"                  // Restore DS from DI

        // Return to caller, nothing to discard
        "lret \n\t"
    );
}

/*
 * Call-gate entry for formatted printing (Ring 0).
 *
 * Expects arguments passed in registers:
 *   EAX = color attribute (foreground | background)
 *   EBX = pointer to message (const char *)
 *   CX  = optional packed position:
 *         - CL = row
 *         - CH = column
 *         - if CX == 0, position is ignored and current cursor is used
 *
 * API Notes:
 * ==========
 * This single call-gate entry (`cg_entry_printr`) implements two related
 * system services, depending on whether CX contains a cursor position:
 *
 *   1) Default Print Mode (CX == 0)
 *      EAX = color
 *      EBX = message
 *      → Calls core_print_color()
 *        Prints text at the current cursor position using the given color.
 *
 *   2) Positioned Print Mode (CX != 0)
 *      EAX = color
 *      EBX = message
 *      CL  = row
 *      CH  = column
 *      → Calls core_print_color_at()
 *        Prints text at the specified (row, column) position and color.
 *
 * Implementation Notes:
 * =====================
 * - The function dynamically checks CX at runtime to determine which print
 *   service to execute. This allows both printing variants to share the same
 *   call-gate, reducing descriptor usage and transition overhead.
 * - The DS segment is temporarily switched to the core data segment
 *   (CORE_DATA) to ensure safe access to kernel memory from Ring 0.
 * - Stack cleanup and return are performed via `lret`, ensuring the far return
 *   correctly restores user context after the call-gate transition.
 */
__attribute__((naked)) void cg_entry_printr(void)
{
    __asm__ __volatile__ (
        // Switch DS to core data segment using SI instead of AX
        "movw %ds, %di\n\t"                  // Save old DS in DI
        "movw $" STR(CORE_DATA) ", %si\n\t"  // Load CORE segment selector into SI
        "movw %si, %ds\n\t"                  // Move the CODE selector from SI to DS

        // Check if CX != 0 (if row/col parameters were provided)
        "cmpw $0, %cx\n\t"                   // Compare CX with 0
        "jne  1f\n\t"                        // Jump if CX != 0 → has coordinates

        // --- Default print (no position) ---
        "pushl %eax\n\t"                     // Push color
        "pushl %ebx\n\t"                     // Push msg
        "call  core_print_color\n\t"         // Call the C function
        "addl  $8, %esp\n\t"                 // Clean up the stack (2 arguments * 4 bytes)
        // Restore DS
        "movw %di, %ds\n\t"                  // Restore DS from DI
        // Return to caller, nothing to discard
        "lret \n\t"

    "1:\n\t"                                 // --- Print with position ---
        "movzbl %cl, %edx\n\t"               // Zero-extend row (CL) into EDX
        "movzbl %ch, %ecx\n\t"               // Zero-extend col (CH) into ECX
        "pushl  %ecx\n\t"                    // Push col
        "pushl  %edx\n\t"                    // Push row
        "pushl  %eax\n\t"                    // Push color
        "pushl  %ebx\n\t"                    // Push msg
        "call   core_print_color_at\n\t"     // Call the C function
        "addl   $16, %esp\n\t"               // Clean up the stack (4 args * 4 bytes)
        // Restore DS
        "movw %di, %ds\n\t"                  // Restore DS from DI
        // Return to caller, nothing to discard
        "lret \n\t"
    );
}
