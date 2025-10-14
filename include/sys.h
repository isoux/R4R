/*
 * R4R License: MIT
 *
 *  - Indentation: 4 spaces
 *
 * include/sys.h
 *
 * Definitions for System memory spaces.
 *
 * (C) Copyright 2021 2025 Isa <isa@isoux.org>
 */

#ifndef _SYS_H
#define _SYS_H

#include <config.h>
#include <typedef.h>
#include <gdt_sys.h>

#define MEM_LIMIT ((MEM_SIZE) / 0x1000) - 1     // 0x7ff

#define CORE_SIZE 64*1024 	                    // 64Kb
#define DEVS_SIZE 64*1024
#define LIBS_SIZE 64*1024
#define USERS_SIZE 64*1024

#define IDT_SIZE    (256 * 8 * 2)               // 0x1000 Align reason
#define GDT_SIZE	(8 * GDT_ENTRIES)			// 64Kb
#define GDT_START	((MEM_SIZE) - (GDT_SIZE))   // 0x7F0000
#define IDT_START   ((GDT_START) - (IDT_SIZE))  // 0x7EF000

#define INIT_START 0x200000
#define CORE_START ((IDT_START)-(CORE_SIZE))	// 0x7DF000
#define DEVS_START ((CORE_START)-(DEVS_SIZE))	// 0x7CF000
#define LIBS_START ((DEVS_START)-(LIBS_SIZE))	// 0x7BF000
#define USERS_START ((LIBS_START)-(USERS_SIZE))	// 0x7AF000

#define CORE_STACK  (IDT_START)  - 4
#define DEVS_STACK  (CORE_START) - 4
#define LIBS_STACK  (DEVS_START) - 4
#define USERS_STACK (LIBS_START) - 4

#define SYS_LIMIT  ((MEM_SIZE)   / 0x1000) - 1
#define DEVS_LIMIT ((CORE_START) / 0x1000) - 1
#define LIBS_LIMIT ((DEVS_START) / 0x1000) - 1
/* Only for the main task in the user space */
#define USERS_SYS_LIMIT ((LIBS_START) / 0x1000) - 1

#endif /* _SYS_H */

