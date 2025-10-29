/*
 * R4R License: MIT
 *
 * - Indentation: 4 spaces
 *
 * kernels/users/users_task.h
 *
 * (C) Copyright 2025 Isa <isa@isoux.org>
 *
 */

#ifndef _USERS_TASK_H
#define _USERS_TASK_H

#include <typedef.h>
#include <task.h>

#define LDT_ENTRIES 2
#define STACK_SIZE 0x100

//extern __attribute__((used, aligned(16)))
extern u64 ldt_users[LDT_ENTRIES];

//extern __attribute__((used, aligned(16)))
extern struct tss32 tss_users_main;

//extern __attribute__((used, aligned(16)))
extern struct tss32 tss_users_nested;

extern u32 ring0_main_stack[STACK_SIZE];
extern struct stack_ptr r0_main_stack;

extern u32 ring1_main_stack[STACK_SIZE];
extern struct stack_ptr r1_main_stack;

extern u32 ring2_main_stack[STACK_SIZE];
extern struct stack_ptr r2_main_stack;

extern u32 task_main_stack[STACK_SIZE];
extern struct stack_ptr stack_main;

extern u32 ring0_nested_stack[STACK_SIZE];
extern struct stack_ptr r0_nested_stack;

extern u32 ring1_nested_stack[STACK_SIZE];
extern struct stack_ptr r1_nested_stack;

extern u32 ring2_nested_stack[STACK_SIZE];
extern struct stack_ptr r2_nested_stack;

extern u32 task_nested_stack[STACK_SIZE];
extern struct stack_ptr stack_nested;

#endif //_USERS_TASK_H
