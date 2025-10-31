/*
 * R4R License: MIT
 *
 *  - Indentation: 4 spaces
 *
 * typedef.h
 *
 * Basic typedefs
 *
 * (C) Copyright 2021-2025 Isa <isa@isoux.org>
 */

#include <stdint.h>

#ifndef TYPES_H
#define TYPES_H

typedef unsigned char   u_char;
typedef unsigned short  u_short;
typedef unsigned int    u_int;
typedef unsigned long   u_long;
typedef long long       l_long;
typedef unsigned int    u_size;

// Signed types
typedef int8_t   i8;   // 8-bit  signed integer
typedef int16_t  i16;  // 16-bit signed integer
typedef int32_t  i32;  // 32-bit signed integer
typedef int64_t  i64;  // 64-bit signed integer

// Unsigned types
typedef uint8_t  u8;   // 8-bit  unsigned integer
typedef uint16_t u16;  // 16-bit unsigned integer
typedef uint32_t u32;  // 32-bit unsigned integer
typedef uint64_t u64;  // 64-bit unsigned integer

// Pointer-sized unsigned integer (32-bit architecture assumed)
typedef u32                uptr;

// Boolean type definition (C-style)
//typedef u8                 bool;
#define true               1
#define false              0

// NULL definition if not already defined
#ifndef NULL
#define NULL ((void *)0)
#endif

#define offsetof(type, member) ((u32) &(((type *)0)->member))

#ifndef u_int
typedef u32 u_int;
#endif


//typedef void (*void_func)(void);        // Function with no args, no return
typedef void (*void_f_int)(u_int);      // Function taking u_int, no return
typedef u_int (*int_f_int)(u_int);      // Function taking and returning u_int

#define __naked_      __attribute__((naked))
#define __packed_     __attribute__((packed))
#define __inlinea_    __attribute__((always_inline))
#define __noret_      __attribute__((__noreturn__))
#define __used_       __attribute__((used))
#define __unusd_      __attribute__((unused))
#define __weak_       __attribute__((weak))
#define __inl_unusd_  __inlinea_ __unusd_ /* always_inline unsuded  */

#define INT_SIZE 4

#define STR2(x) #x
#define STR(x) STR2(x)
#define START_0 0

#endif // TYPES_H
