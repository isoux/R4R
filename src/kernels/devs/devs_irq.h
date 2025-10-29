/*
 * R4R License: MIT
 *
 * - Indentation: 4 spaces
 *
 * kernels/devs/devs_irq.h
 *
 * (C) Copyright 2025 Isa <isa@isoux.org>
 */
#include <task.h>

extern struct tss32 tss_devs_irq;

void devs_irq_task(void);
void get_keyboard_int(void);
char handle_key_press(void);
