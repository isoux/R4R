/*
 * R4R
 *
 * load_grub.c
 *
 * Multiboot load procedures for GRUB version 1.
 *
 * The GRUB loader uses different GDT selector values for code (0x10) and data
 * segments than this system (which expects code at 0x08 and data at 0x10).
 * Therefore, the GDT must be reinitialized and segment selectors updated
 * so that CS = 0x08 and DS = 0x10 before transitioning to kernel code.
 *
 * (C) Copyright 2021-2025 Isa <isa@isoux.org>
 */

#include <multiboot.h>
#include <typedef.h>
#include <sys.h>
#include <gdt_sys.h>

extern void init(void);

void load(void);
void load_mods(u32);
void copy_module(u32, u32, u32);

#define HEADER_FLAGS	PAGE_ALIGN + MEMORY_INFO

__attribute__((section(".multiboot")))
 u_long multi_head[] = {
    HEADER_MAGIC,
    HEADER_FLAGS, -(HEADER_MAGIC + HEADER_FLAGS)
};

u64 multib_gdt[] = {
    0x0000000000000000,	// NULL descriptor
    0x00c09a00000007ff,	// sel. 0x08 CODE 8Mb, base = 0
    0x00c09200000007ff	// sel. 0x10 DATA 8Mb
};

struct __packed_ gdt_opcode_p {
    u16 size;
    u32 *gdt;
};

struct __packed_ idt_opcode_p {
    u16 size;
    u32 *idt;
};

__used_ static struct gdt_opcode_p mb_gdt_opc =
    { .size = 3 * 8 - 1, .gdt = ((u32*) &multib_gdt[0]) };

__used_ static struct idt_opcode_p mb_idt_opc =
    { .size = 0, .idt = 0 };

__naked_ void set_idt_gdt(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "lidt [mb_idt_opc]\n\t"
        "lgdt [mb_gdt_opc]\n\t"
        "ret\n\t"
        ".att_syntax prefix"
    );
}

u8 stack_space[8192] = { 0 };

/* Arguments for Long JMP */
struct ljmp_args {
    u32 func_ptr;
    u16 selector;
} jmp_args = { (u32) load, CORE_CODE };

__naked_ void start(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "cli\n\t"
        "call set_idt_gdt\n\t"

        "xor eax, eax\n\t"
        "mov al, 0x11\n\t"
        "out 0x20, al\n\t"
        "out 0xA0, al\n\t"
        "mov al, 0x20\n\t"
        "out 0x21, al\n\t"
        "mov al, 0x28\n\t"
        "out 0xA1, al\n\t"
        "mov al, 0x04\n\t"
        "out 0x21, al\n\t"
        "mov al, 0x02\n\t"
        "out 0xA1, al\n\t"
        "mov al, 0x01\n\t"
        "out 0x21, al\n\t"
        "out 0xA1, al\n\t"
        "mov al, 0xFF\n\t"
        "out 0x21, al\n\t"
        "out 0xA1, al\n\t"

        "ljmp [jmp_args]\n\t"
        ".att_syntax prefix"
    );
}

__naked_ void load(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "lea eax, [stack_space + 8192]\n\t"
        "mov esp, eax\n\t"
        "push ebx\n\t"
        "call load_mods\n\t"

        "lcall " STR(CORE_CODE) ":" STR(INIT_START) "\n\t"
        "hlt\n\t"
        ".att_syntax prefix"
    );
}

int strncmp(const char *a, const char *b, int n) {
    for (int i = 0; i < n; i++) {
        if (a[i] != b[i])
            return 0;
        if (a[i] == '\0' || b[i] == '\0')
            return 0;
    }
    return 1;
}

/*
 * Actually, this looks for the first space after which 
 * the module name appears.
 */
const char* find_module_name(const char *a) {
    const char *b = a;
    for (int i = 0;; i++) {
        if (a[i] == ' ') {
            b = a + i + 1;
            break;
        }
    }
    return b;
}

void load_mods(u32 info_struc) {
    u32 i;
    const char *cmd;
    const char *mod_name;
    u32 mod_size, out_addr;
    info_t *mb_info;
    module_t *mod;

    mb_info = (info_t*) info_struc;

    for (i = 0, mod = (module_t*) mb_info->mods_addr; i < mb_info->mods_count;
            i++, mod++) {
        mod_size = (unsigned) mod->mod_end - (unsigned) mod->mod_start;
        cmd = (const char*) mod->cmdline;
        mod_name = find_module_name(cmd);

        if (strncmp(mod_name, "init", 4)) {
            out_addr = INIT_START;
        } else if (strncmp(mod_name, "core", 4)) {
            out_addr = CORE_START;
        } else if (strncmp(mod_name, "devs", 4)) {
            out_addr = DEVS_START;
        } else if (strncmp(mod_name, "libs", 4)) {
            out_addr = LIBS_START;
        } else if (strncmp(mod_name, "users", 5)) {
            out_addr = USERS_START;
        } else {
            out_addr = 0;
        }

        if (out_addr)
            copy_module((mod_size + 3) >> 2, mod->mod_start, out_addr);
    }
}

void copy_module(u32 mod_size, u32 in_addr, u32 out_addr) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "cld\n\t"
        "mov ecx, %0\n\t"
        "mov esi, %1\n\t"
        "mov edi, %2\n\t"
        "rep movsd\n\t"
        ".att_syntax prefix"
        :
        : "r" (mod_size), "r" (in_addr), "r" (out_addr)
        : "ecx", "esi", "edi"
    );
}

