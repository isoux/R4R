# R4R License: MIT
#
# Makefile for sys/load
#
# (C) Copyright 2025 Isa <isa@isoux.org>

MODULE = load
include ../../defines.mk

CFLAGS = -std=c23 -m32 -ffreestanding -Wall -Wextra -march=$(ARCH) -c \
    -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin \
    -I $(INCLUDE_DIR) -fpack-struct -DMARCH=$(ARCH) -D$(BOOT_WAY)

ifeq ($(BOOT_WAY), GRUB2)
    LOAD_C = load_grub2.c
else
    LOAD_C = load_grub.c
endif

.PHONY: all clean

OBJ = $(OBJ_DIR)/load.o
DUMP_FILE = $(DUMP_SUBDIR)/load.dump
BIN_FILE  = $(OBJ_DIR)/load.bin

ifeq ($(OBJ_DUMP),"TRUE")
    all: $(OBJ) $(DUMP_FILE)
else
    all: $(OBJ)
endif

$(OBJ): $(LOAD_C)
	$(MKDIR) $(OBJ_DIR)
	$(CC) $(CFLAGS) -o $@ $<

$(DUMP_FILE): $(OBJ)
	$(MKDIR) $(DUMP_SUBDIR)
	$(OBJDUMP) $(OBJ) > $(DUMP_FILE)
    
clean:
	$(RM) $(OBJ) $(DUMP_FILE) $(BIN_FILE)
	
print-env:
	@echo "OBJ_DUMP is: '$(OBJ_DUMP)'"
	
dep:
	sed '/\#\#\# Dependencies/q' < Makefile > tmp_make
	(for i in *.c;do $(CPP) -M $$i;done) >> tmp_make
	cp tmp_make Makefile

# Dependencies

load_grub.o: load_grub.c ../../../include/multiboot.h \
 ../../../include/typedef.h ../../../include/stdint.h \
 ../../../include/sys.h ../../../include/config.h ../../../include/gdt.h \
 ../../../include/io.h ../../../include/gas_regs.h
load_grub2.o: load_grub2.c ../../../include/multiboot.h \
 ../../../include/typedef.h ../../../include/stdint.h \
 ../../../include/sys.h ../../../include/config.h ../../../include/gdt.h
