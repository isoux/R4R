/*
 * R4R License: MIT
 *
 *  - Indentation: 4 spaces
 *
 * ldt.h
 *
 * Defines for local descriptor tables
 *
 * (C) Copyright 2025 Isa <isa@isoux.org>
 */

#ifndef _LDT_H
#define _LDT_H

/* SELECTORS FOR LOCAL DESC TABLES */
/*
15                                                 3    2        0
+--------------------------------------------------+----+--------+
|          Index                                   | TI |   RPL  |
+--------------------------------------------------+----+--------+

TI = Table Indicator: 0 = GDT, 1 = LDT
*/

// At ring 0
#define CORE_LDT_CODE 0x04
#define CORE_LDT_DATA 0x0c

// At ring 1
#define DEVS_LDT_CODE	0b0101 //A binary literal = 0x05
#define DEVS_LDT_DATA	0b1101 //0x0D


// At ring 2
#define LIBS_LDT_CODE	0b0110 //0x06
#define LIBS_LDT_DATA	0b1110 //0x0E


// At ring 3
#define USERS_LDT_CODE  0x07   //0b0111
#define USERS_LDT_DATA  0x0F   //0b1111
//#define CG_LIBS_PRINTF 0x17    // Call gate

#endif /* _LDT_H */

