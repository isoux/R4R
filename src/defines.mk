#
# R4R License: MIT
#
# src/defines.mk
# Common make definitions
#
# (C) Copyright 2021-2025 Isa <isa@isoux.org>

BUILD_DIR = ../../../build
INCLUDE_DIR = ../../../include
DUMP_DIR = $(BUILD_DIR)/dumps

# === Common build directories for modules ===
OBJ_DIR   = $(BUILD_DIR)/$(MODULE)
DUMP_SUBDIR = $(DUMP_DIR)/$(MODULE)

CC = gcc-14
CP = cp -v
LD = ld -m elf_i386
ARCH = i486
CPP = $(CC) -E -nostdinc -I $(INCLUDE_DIR)
OBJDUMP = objdump -D -z -M intel

CP := cp
RM := rm -rf
MKDIR := mkdir -pv

CORE = sys_out/core
DEVS = sys_out/devs
LIBS = sys_out/libs
USRS = sys_out/users
APPS = sys_out/apps

CORE_RAM = core
DEVS_RAM = devs
LIBS_RAM = libs
USRS_RAM = usrs
GDT_RAM = gdt_
APPS_RAM = apps

LOAD = sys_out/load
GDT = core/gdt_mem.bin
SYS = 4ring_os
SYS_LOAD = load
IMG_FILES = img/*
CFG = boot/grub.cfg

IMG_PATH := img
ISO_PATH := iso
BOOT_PATH := $(ISO_PATH)/boot
GRUB_PATH := $(BOOT_PATH)/grub

# GRUB or GRUB2 : One of these must be defined!
export BOOT_WAY = GRUB  # GRUB2
export OBJ_DUMP="TRUE"
# IMPORTANT:
# This line must end with a new line (Enter), otherwise GNU Make may 
# misinterpret the value. The comment below must be on a separate line
# to avoid corrupting the variable value!
# "FALSE"
