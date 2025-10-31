#
# R4R License: MIT
#
# tests/sys/defines.mk
# Common make definitions for tests
#
# (C) Copyright 2021-2025 Isa <isa@isoux.org>

ROOT_DIR := $(realpath ../../..)

INCLUDE_DIR := $(ROOT_DIR)/include

CC := gcc-14
CFLAGS := -Wall -Wextra -I$(INCLUDE_DIR) -std=c23
