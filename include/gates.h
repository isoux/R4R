/*
 * 4RING_OS License: MIT
 *
 *  - Indentation: 4 spaces
 *
 * gates.h
 *
 * Gates definitions
 *
 * (C) Copyright 2021 Isa <isa@isoux.org>
 */

#ifndef _GATES_H
#define _GATES_H

#include <typedef.h> 

#define CALL_GATE	0x0C
#define INT_GATE	0x0E
#define TRP_GATE	0x0F
#define TASK_GATE	0x05

typedef struct Call_gate
{
    u16 OffsetLow;
    u16 Selector;
    u8 NumberOfArguments:5;
    u8 Reserved:3;
    u8 Type:5;
    u8 Dpl:2;
    u_8 Present:1;
    u16 OffsetHigh;
}call_gate,*p_call_gate;

/* fword argument (6 bytes) for far lcall or ljmp */
struct call_gate_args {
    u16 null0;
    u16 null1;
    u16 selector;
};

extern struct call_gate_args call_args;

#endif /* _GATES_H */
