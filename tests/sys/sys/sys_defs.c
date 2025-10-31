/*
 * R4R License: MIT
 *
 *  - Indentation: 4 spaces
 *
 * tests/sys/sys_defs.c
 *
 * Checks and prints system macro definitions.
 * See include/sys.h
 *
 * (C) Copyright 2021–2025 Isa <isa@isoux.org>
 */

#include <stdio.h>
#include <gdt_sys.h>
#include <sys.h>

int main(void)
{
    printf("\n");
    printf("SYS_INIT        :  0x%08x\n", SYS_INIT);
    printf("MEM_LIMIT       :  0x%08x\n\n", MEM_LIMIT);

    printf("CORE_SIZE       :  0x%08x\n", CORE_SIZE);
    printf("DEVS_SIZE       :  0x%08x\n", DEVS_SIZE);
    printf("LIBS_SIZE       :  0x%08x\n", LIBS_SIZE);
    printf("USERS_SIZE      :  0x%08x\n\n", USERS_SIZE);

    printf("IDT_SIZE        :  0x%08x\n", IDT_SIZE);
    printf("GDT_SIZE        :  0x%08x\n", GDT_SIZE);
    printf("GDT_START       :  0x%08x\n", GDT_START);
    printf("IDT_START       :  0x%08x\n\n", IDT_START);

    printf("INIT_START      :  0x%08x\n", INIT_START);
    printf("CORE_START      :  0x%08x\n", CORE_START);
    printf("DEVS_START      :  0x%08x\n", DEVS_START);
    printf("LIBS_START      :  0x%08x\n", LIBS_START);
    printf("USERS_START     :  0x%08x\n\n", USERS_START);

    printf("CORE_STACK      :  0x%08x\n", CORE_STACK);
    printf("DEVS_STACK      :  0x%08x\n", DEVS_STACK);
    printf("LIBS_STACK      :  0x%08x\n", LIBS_STACK);
    printf("USERS_STACK     :  0x%08x\n\n", USERS_STACK);

    printf("SYS_LIMIT       :  0x%08x\n", SYS_LIMIT);
    printf("DEVS_LIMIT      :  0x%08x\n", DEVS_LIMIT);
    printf("LIBS_LIMIT      :  0x%08x\n", LIBS_LIMIT);
    printf("USERS_SYS_LIMIT :  0x%08x\n\n", USERS_SYS_LIMIT);

    return 0;
}
