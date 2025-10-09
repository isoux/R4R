/*
 * R4R License: MIT
 *
 * - Indentation: 4 spaces
 *
 * pages_build.c
 *
 * Memory Segmentation Notes:
 * - Full identity mapping of 0–8MB.
 * - pg_tab0 (PDE[0]): maps 0–4MB, user-accessible (U/S=1), except lowher 1MB now U/S=0.
 * - pg_tab1 (PDE[1]): maps 4–8MB with mixed access.
 *   - First 4MB–7936KB are user-accessible (U/S=1).
 *   - Final 128KB (0x007E0000–0x007FFFFF) are supervisor-only (U/S=0).
 * - This layout supports isolated memory domains per ring with segmentation + paging protection.
 *
 * Segmentation Model Note:
 * - This kernel does not use a flat memory model.
 * - Instead, logical segmentation is enforced using real GDT segment base/limit definitions.
 * - Ring isolation is achieved through both segmentation (via segment descriptors with base/limit) and paging (via U/S bit).
 * - Kernel uses supervisor-only regions and restricts access to critical memory below 1MB and top 128KB.
 *
 * Architectural Vision:
 * - R4R kernel demonstrates an alternative direction in OS design on x86 architecture.
 * - Instead of relying solely on the paging-based isolation model adopted in Unix-like kernels (Linux, BSD),
 *   it emphasizes hardware-supported segmentation for domain isolation.
 * - Each privilege level (Ring 0–3) can have physically separated, GDT-constrained execution areas.
 * - This design challenges conventional flat-memory thinking and provides strong, transparent ring containment.
 *
 * (C) Copyright 2025 Isa <isa@isoux.org>
 *
 */

#include <page/page.h>

static u32 *pg_dir0 = (u32*) PG_DIR_ADDR;
static u32 *pg_tab0 = (u32*) PG_TAB0_ADDR;
static u32 *pg_tab1 = (u32*) PG_TAB1_ADDR;

void setup_paging(void) {

    // Clear page directory and tables
    for (int i = 0; i < PTE_SIZE; i++) {
        pg_dir0[i] = 0;
        pg_tab0[i] = 0;
        pg_tab1[i] = 0;
    }

    // Identity map 0–4MB (pg_tab0)
    // First 1MB = U/S = 0 (supervisor-only), rest = U/S = 1
    for (int i = 0; i < PTE_SIZE; i++) {
        u32 addr = i * PAGE_SIZE;
        u32 flags = (i < 256) ? PAGING_CORE_FLAGS : PAGING_DEFAULT_FLAGS; // 256 * 4KB = 1MB
        pg_tab0[i] = addr | flags;
    }

    // Identity map 4MB – 8MB–128KB (user accessible)
    // from 0x00400000 to 0x007E0000
    for (int i = 0; i < (PTE_SIZE - 32); i++) {
        pg_tab1[i] = ((i + PTE_SIZE) * PAGE_SIZE) | PAGING_DEFAULT_FLAGS;
    }

    // Map last 128KB of memory (0x007E0000–0x007FFFFF) for
    // kernel-only (supervisor level).
    // Same page table as above, last 32 entries use U/S = 0
    u32 addr = START_ADDR;
    for (int i = 992; i < 1024; i++, addr += PAGE_SIZE) {
        pg_tab1[i] = addr | PAGING_CORE_FLAGS;
    }

    // Assign Page Directory Entries
    // PDE entries must have U/S=1 so that Ring3 can access
    // pages marked as user in their PTEs. PTE flags still enforce
    // the supervisor-only regions (first 1MB, last 128KB).
    pg_dir0[0] = ((u32) pg_tab0) | PAGING_DEFAULT_FLAGS; // 0–4MB
    pg_dir0[1] = ((u32) pg_tab1) | PAGING_DEFAULT_FLAGS; // 4MB–8MB with mixed U/S

    // Load CR3 and enable paging (set PG bit in CR0)
    __asm__ volatile (
        "mov %0, %%cr3\n"
        "mov %%cr0, %%eax\n"
        "or $0x80000000, %%eax\n"
        "mov %%eax, %%cr0"
        : : "r"(pg_dir0) : "eax"
    );
}

void set_pte_flags(u32 addr, u16 nr_entry, u32 flags) {
    u32 *p_addr = (u32*) ((GET_PTE(addr) + PTE_SIZE) * 4);
    for (; nr_entry > 0; nr_entry--, p_addr++) {
        *p_addr = (*p_addr & ~0xFFF) | (flags & 0xFFF);
    }
    flush_tlb();
}

void set_task_vmem(u32 addr, u32 mem_size, u32 start_addr) {
    u32 *p_addr = (u32*) ((GET_PTE(addr) + PTE_SIZE) * 4);
    u32 nr_entry = GET_NR_ENTRY(mem_size);
    for (; nr_entry > 0; nr_entry--, p_addr++) {
        u32 flags = *p_addr & 0xFFF;
        *p_addr = start_addr | flags;
        start_addr += PAGE_SIZE;
    }
}
