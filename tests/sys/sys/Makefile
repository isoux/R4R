# R4R License: MIT
#
#  R4R Project - System Macro Test
#  File: tests/sys/Makefile
#  Purpose: Build and run system definition test program
#
#  (C) Copyright 2025 Isa <isa@isoux.org>
#

# Include common definitions
include ../defines.mk

# Find all .c files in the current directory
SRCS := $(wildcard *.c)

# Generate object files by replacing .c with .o
OBJS := $(SRCS:.c=.o)

# Name of the final executable
TARGET := test_sys

# Default target: build the executable
all: $(TARGET)

# Link the object files into the final executable
$(TARGET): $(OBJS)
	@echo " [LD] $@"
	$(CC) $(CFLAGS) -o $@ $^

# Compile each .c file into .o
%.o: %.c
	@echo " [CC] $<"
	$(CC) $(CFLAGS) -c $< -o $@

# Run target: execute the built binary
run: $(TARGET)
	@echo
	@echo "===== Running $(TARGET) ====="
	@./$(TARGET)

# Clean target: remove object files and executables
clean:
	@echo " [CLEAN]"
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean run

