/*
 * R4R License: MIT
 *
 *  - Indentation: 4 spaces
 *
 * include/page/page.h
 *
 * Paging definitions for core kernel
 *
 * (C) Copyright 2025 Isa <isa@isoux.org>
 */

#ifndef PAGE_H
#define PAGE_H

#include <typedef.h>
#include <sys.h>

#define PAGING_FLAG_PRESENT  0x001
#define PAGING_FLAG_RW       0x002
#define PAGING_FLAG_USER     0x004
#define PAGING_DEFAULT_FLAGS (PAGING_FLAG_PRESENT | PAGING_FLAG_RW | PAGING_FLAG_USER)
#define PAGING_CORE_FLAGS    (PAGING_FLAG_PRESENT | PAGING_FLAG_RW)

#define PG_DIR_ADDR       0x00000000
#define PG_TAB0_ADDR      0x00001000 // Page Table 0: user region 0–4MB (PDE[0], U/S = 1, but first 1MB = U/S=0)
#define PG_TAB1_ADDR      0x00002000 // Page Table 1: mix user and kernel region 4MB–8MB (PDE[1])

#define PAGE_SIZE     0x1000
#define PDE_SIZE      1024
#define PTE_SIZE      1024

#define START_ADDR    (MEM_SIZE - GDT_SIZE - CORE_SIZE)

#define GET_PDE(addr)        ((addr) / (PAGE_SIZE * PTE_SIZE))
#define GET_PTE(addr)        ((addr) / PAGE_SIZE)
#define GET_NR_ENTRY(size)   ((size) / PAGE_SIZE)

#define CLR_ROOT_FL 0xFFFFFFFD

void setup_paging(void);
void set_pte_flags(u32 addr, u16 nr_entry, u32 flags);
void set_task_vmem(u32 addr, u32 mem_size, u32 start_addr);

static inline void flush_tlb(void) {
    __asm__ volatile (
        "mov %%cr3, %%eax\n"
        "mov %%eax, %%cr3"
        : : : "eax"
    );
}

#endif /* PAGE_H */
