/*
 * R4R License: MIT
 *
 * - Indentation: 4 spaces
 *
 * include/hw/io.h
 *
 * (C) Copyright 2025 Isa <isa@isoux.org>
 */

#ifndef _IO_H
#define _IO_H

#include <typedef.h>

static inline void outb(u16 port, u8 val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline u8 inb(u16 port) {
    u8 ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outw(u16 port, u16 val) {
    __asm__ volatile ("outw %0, %1" : : "a"(val), "Nd"(port));
}

static inline u16 inw(u16 port) {
    u16 ret;
    __asm__ volatile ("inw %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

#endif // _IO_H
