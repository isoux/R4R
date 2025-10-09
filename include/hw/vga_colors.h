/*
 * R4R License: MIT
 *
 * - Indentation: 4 spaces
 *
 * include/hw/vga_colors.h
 *
 * IBM PC display definitions
 *
 * (C) Copyright 2025 Isa <isa@isoux.org>
 */

/* Color attributes for foreground text */

#ifndef _DISPLAY_H
#define _DISPLAY_H

#define	FG_BLACK	0x00
#define	FG_BLUE		0x01
#define	FG_GREEN	0x02
#define	FG_CYAN		0x03
#define	FG_RED		0x04
#define	FG_MAGEN	0x05
#define	FG_BROWN	0x06
#define	FG_LGREY	0x07
#define	FG_DGREY	0x08
#define	FG_LBLUE	0x09
#define	FG_LGREEN	0x0A
#define	FG_LCYAN	0x0B
#define	FG_LRED		0x0C
#define	FG_LMAG		0x0D
#define	FG_YELLOW	0x0E
#define	FG_WHITE	0x0F
#define	FG_BLINK	0x80

/* Color attributes for text background */

#define	BG_BLACK	0x00
#define	BG_BLUE		0x10
#define	BG_GREEN	0x20
#define	BG_CYAN		0x30
#define	BG_RED		0x40
#define	BG_MAGEN	0x50
#define	BG_BROWN	0x60
#define	BG_LGREY	0x70
#define	BG_BLACK_BLINK	0x80
#define	BG_BLUE_BLINK	0x90
#define BG_GREEN_BLINK	0xA0
#define	BG_CYAN_BLINK	0xB0
#define	BG_RED_BLINK	0xC0
#define	BG_MAG_BLINK	0xD0
#define	BG_YELLOW_BLINK	0xE0
#define	BG_WHITE_BLINK	0xF0

/* Monochrome attributes for foreground text */

#define	FG_UNDERLINE	0x01
#define	FG_INTENSE		0x08

/* Monochrome attributes for text background */

#define	BG_INTENSE		0x10

#endif /* _DISPLAY_H */
