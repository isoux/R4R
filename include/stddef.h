/*
 * R4R License: MIT
 *
 *  - Indentation: 4 spaces
 *
 * include/stddef.h
 *
 * (C) Copyright 2025 Isa <isa@isoux.org>
 */

#ifndef _STDDEF_H
#define _STDDEF_H

#ifndef NULL
#define NULL ((void *)0)
#endif

#define offsetof(type, member) ((u32) &(((type *)0)->member))

#endif /* _STDDEF_H */
