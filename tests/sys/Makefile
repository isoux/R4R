# R4R License: MIT
#
#  R4R Project - Tests Master Makefile
#  File: tests/Makefile
#  Purpose: Build all test programs in subdirectories
#
# (C) Copyright 2021-2025 Isa <isa@isoux.org>


# Include the common definitions
include defines.mk

# Find all subdirectories
SUBDIRS := $(wildcard */)

# Default target: build all tests
all:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir; \
	done

# Clean all tests
clean:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done

# Run all tests (if they have run target)
run:
	@for dir in $(SUBDIRS); do \
		if [ -f $$dir/Makefile ]; then \
			$(MAKE) -C $$dir run; \
		fi \
	done

.PHONY: all clean run

