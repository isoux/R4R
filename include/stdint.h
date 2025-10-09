/*
 * R4R License: MIT
 *
 *  - Indentation: 4 spaces
 *
 * stdint.h 
 *
 * Minimal fixed-width integer type definitions
 * For freestanding / kernel development
 * Basic typedefs
 *
 * (C) Copyright 2025 Isa <isa@isoux.org>
 */

#ifndef __STDINT_H__
#define __STDINT_H__

/* Signed integer types */
typedef signed char        int8_t;     /*  8-bit signed   */
typedef short              int16_t;    /* 16-bit signed   */
typedef int                int32_t;    /* 32-bit signed   */
typedef long long          int64_t;    /* 64-bit signed   */

/* Unsigned integer types */
typedef unsigned char      uint8_t;    /*  8-bit unsigned */
typedef unsigned short     uint16_t;   /* 16-bit unsigned */
typedef unsigned int       uint32_t;   /* 32-bit unsigned */
typedef unsigned long long uint64_t;   /* 64-bit unsigned */

/* Pointer-sized types (for 32-bit systems) */
typedef int32_t            intptr_t;
typedef uint32_t           uintptr_t;

#endif /* __STDINT_H__ */

